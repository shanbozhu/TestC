/*=====================================================
# Author:        Zeegine Team
# Copyright:     www.thu3d.com        
# Last modified: 2015-03-17 14:16:47 
# Filename:		 timeEstimate.h
# Description:   
=====================================================*/

#ifndef TIME_ESTIMATE_H
#define TIME_ESTIMATE_H

#include <vector>
#include <algorithm>

#include "printer.h"
#include "GCode.h"
#include "Printjob.h"
#include "PrinterState.h"

struct GCodeData
{
	double time;
	int layer;
	double Mlength;
	int speedCont;
	float xL;
	float yL;
	float zL;
};

/**
    The TimeEstimateCalculator class generates a estimate of printing time calculated with acceleration in mind.
    Some of this code has been addapted from the Marlin sources.
*/
class TimeEstimateCalculator
{
	Printjob* printjob;
    double x, y, z, e,emax,f,eprinter;
    double lastX,lastY,lastZ,lastE,lastF;
	double xOffset, yOffset, zOffset, eOffset;
	double minX,minY,minZ,maxX,maxY,maxZ;
	bool hasXHome, hasYHome, hasZHome;
    bool eChanged;
	double lastZPrint;
	//GCodeData* data;
	int sumLayer;
	long long sumSpeed;
    bool relative;
    bool eRelative;
    std::ifstream jobCal;
    //std::fstream jobCal;
	double sumTime; 
	double moveLength; 
public:
	GCodeData* estimateTotalPrintTime(Printer *printer , boost::shared_ptr<Printjob> job);
	void analyzeGCode(GCode &code,Printer *printer);
	GCodeData data;
	void Cal_reset();
	
	TimeEstimateCalculator(Printjob* pj);
	~TimeEstimateCalculator();

};

typedef boost::shared_ptr<TimeEstimateCalculator> TimeEstimateCalculatorPtr;

const class nullptr_t
{
public:
	template<class T>
	inline operator T*() const
	{ return 0; }

	template<class C, class T>
	inline operator T C::*() const
	{ return 0; }

private:
	void operator&() const;
} nullptr = {};

#endif
