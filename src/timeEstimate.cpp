/*=====================================================
# Author:        Zeegine Team
# Copyright:     www.thu3d.com        
# Last modified: 2015-03-17 14:16:55 
# Filename:		 timeEstimate.cpp
# Description:   
=====================================================*/

#include <math.h>
#include <time.h>
#include <stdio.h>
#include <string.h>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <boost/filesystem.hpp>
#include <boost/thread/mutex.hpp>

#include "Printjob.h"
#include "timeEstimate.h"
#include "GCode.h"
#include "printer.h"
#define MINIMUM_PLANNER_SPEED 0.05// (mm/sec)
using namespace std;
const double max_feedrate[4] = {600, 600, 40, 25};
const double minimumfeedrate = 0.01;
const double acceleration = 3000;
const double max_acceleration[4] = {9000,9000,100,10000};
const double max_xy_jerk = 20.0;
const double max_z_jerk = 0.4;
const double max_e_jerk = 5.0;

template<typename T> const T square(const T& a) { return a * a; }
boost::mutex filesMutex;
GCodeData* TimeEstimateCalculator::estimateTotalPrintTime(Printer *printer , boost::shared_ptr<Printjob> job) 
{
    data.time = 0;
	data.layer = 0;
	data.Mlength = 0;
	if(job->getLength() == 0) return &data;
    mutex::scoped_lock l(filesMutex);
    if(!job.get()) return &data; // unknown job
    if(jobCal.is_open() && jobCal.eof()) jobCal.close();
    jobCal.open(job->getFilename().c_str(),ifstream::in);
	char buf[100];
	//循环计算
	while(jobCal.good() && (-1 != jobCal.tellg())) {
		
		if(access(job->getFilename().c_str(),0)){ 
        	jobCal.close();
        	return &data;
        }
	
        jobCal.getline(buf, 100);   
        sumSpeed += f;
		job->calPos = jobCal.tellg();
		shared_ptr<GCode>  gc = shared_ptr<GCode>(new GCode(*printer,string(buf)));
        analyzeGCode( *gc ,printer);
	}
    jobCal.close();
    data.time = sumTime;
    data.layer = sumLayer;
    data.Mlength = e+eOffset;
    data.speedCont = (int)(sumSpeed/60000); //m/s
    data.xL = maxX-minX;
    data.yL = maxY-minY;
    data.zL = maxZ-minZ;
    return &data;
}

void TimeEstimateCalculator::analyzeGCode(GCode &code,Printer *printer)
{
    if (code.hasG())
    {
        switch (code.getG())
        {
            case 0:

            case 1:
            {
                eChanged = false;
                if(code.hasF()) f = code.getF();
//                	if(!printer->jobPause) PrinterPauseFeedrate = f;
                    if (relative)
                    {
                        if(code.hasX()) x += code.getX();
                        if(code.hasY()) y += code.getY();
                        if(code.hasZ()) z += code.getZ();
                        if(code.hasE()) {
                            eChanged = code.getE()!=0;
                            e += code.getE();
                            eprinter += code.getE();
                        }
                    }
                    else
                    {
                        if (code.hasX()) x = xOffset+code.getX();
                            if (code.hasY()) y = yOffset+code.getY();
                                if (code.hasZ()) {
                                    z = zOffset+code.getZ();
                                }
                        if (code.hasE())
                        {
                            if (eRelative) {
                                eChanged = code.getE()!=0;
                                e += code.getE();
                                eprinter += code.getE();
                            } else {
                                eChanged = (eOffset+code.getE())!=e;
                                e = eOffset + code.getE();
                                eprinter = code.getE();
                            }
                        }
                    }
                if (x < printer->xmin) { x = printer->xmin; hasXHome = false; }
                if (y < printer->ymin) { y = printer->ymin; hasYHome = false; }
                if (z < printer->zmin) { z = printer->zmin; hasZHome = false; }
                if (x > printer->xmax) { x = printer->xmax; hasXHome = false; }
                if (y > printer->ymax) { y = printer->ymax; hasYHome = false; }
                if (z > printer->zmax) { z = printer->zmax; hasZHome = false; }
                if (e > emax) {
                    emax = e;
                    if(z!=lastZPrint) {
                    	//layerHight = z-lastZPrint;
                        lastZPrint = z;
                        sumLayer++;
                    }
                }
                double dx = abs(x - lastX);
                double dy = abs(y - lastY);
                double dz = abs(z - lastZ);
                double de = abs(e - lastE);
                if (dx + dy + dz > 0.001)
                {
                    sumTime += sqrt(dx * dx + dy * dy + dz * dz) * 60.0f / f;
                }
                else sumTime += de * 60.0f / f;
                lastX = x;
                lastY = y;
                lastZ = z;
                lastE = e;
                minX = min(x,minX);
				minY = min(y,minY);
				minZ = min(z,minZ);
				maxX = max(x,maxX);
				maxY = max(y,maxY);
				maxZ = max(z,maxZ);
            }
                break;
            case 28:
            case 161:
            {
                bool homeAll = !(code.hasX() || code.hasY() || code.hasZ());
                if (code.hasX() || homeAll) { xOffset = 0; x = printer->homex; hasXHome = true; }
                if (code.hasY() || homeAll) { yOffset = 0; y = printer->homey; hasYHome = true; }
                if (code.hasZ() || homeAll) { zOffset = 0; z = printer->homez; hasZHome = true; }
                if (code.hasE()) { eOffset = 0; e = 0; emax = 0; }
                break;
            }
            case 162:
            {
                bool homeAll = !(code.hasX() || code.hasY() || code.hasZ());
                if (code.hasX() || homeAll) { xOffset = 0; x = printer->xmax; hasXHome = true; }
                if (code.hasY() || homeAll) { yOffset = 0; y = printer->ymax; hasYHome = true; }
                if (code.hasZ() || homeAll) { zOffset = 0; z = printer->zmax; hasZHome = true; }
                break;
            }
            case 90:
                relative = false;
				break;
            case 91:
                relative = true;
                break;
            case 92:
                if (code.hasX()) { xOffset = x-code.getX(); x = xOffset; }
                if (code.hasY()) { yOffset = y-code.getY(); y = yOffset; }
                if (code.hasZ()) { zOffset = z-code.getZ(); z = zOffset; }
                if (code.hasE()) { eOffset = e-code.getE(); lastE = e = eOffset; eprinter = code.getE();}
                break;
        }
	}
	else if (code.hasM())
	{
        switch (code.getM())
        {
            case 82:
                eRelative = false;
                break;
            case 83:
                eRelative = true;
                break;
        }
    }
 }



TimeEstimateCalculator::TimeEstimateCalculator(Printjob* pj){
	printjob = pj;
	Cal_reset();
}

void TimeEstimateCalculator::Cal_reset(){
	
    sumTime = 0;
	moveLength = 0;
	sumLayer = 0;
	sumSpeed = 0;
    relative = false;
    eRelative = false;
	xOffset = 0;
	yOffset = 0;
	zOffset = 0;
	eOffset = 0;
	minX = 0;
	minY = 0;
	minZ = 0;
	maxX = 0;
	maxY = 0;
	maxZ = 0;
	x = y = z = e = emax = 0;
    f = 1000;
    lastX = lastY = lastZ = lastE = eprinter = 0;
    xOffset = yOffset = zOffset = eOffset = 0;
    data.time = 0;
	data.layer = 0;
	data.Mlength = 0;
	data.speedCont = 0;
}
TimeEstimateCalculator::~TimeEstimateCalculator() {
}
    
