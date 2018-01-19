/*=====================================================
# Author:        Zeegine Team
# Copyright:     www.thu3d.com        
# Last modified: 2015-03-17 14:18:21 
# Filename:		 PrinterState.h
# Description:   
=====================================================*/


#ifndef __PrinterState__
#define __PrinterState__

#include <iostream>
#include <boost/thread.hpp>
#include <boost/cstdint.hpp>

#include "json_spirit_value.h"

using namespace boost;

#define PRINTER_E_REL 83
#define PRINTER_E_ABS 82
#define PRINTER_C_REL 91
#define PRINTER_C_ABS 90

struct PrinterTemp {
    uint64_t time;
    float tempSet;
    float tempRead;
    int8_t output;
};

class Printer;
class GCode;

/**
 The PrinterState stores variable values which are
 changed by sending commands or measured by external sensors
 in the firmware.
 The class is thread safe.
 */
class PrinterState {
    Printer *printer;
    boost::mutex mutex; // Used for thread safety
    int activeExtruder;
    bool uploading;
    PrinterTemp bed;
    PrinterTemp* extruder;
    float x, y, z, e,emax,f,eprinter;
    float lastX,lastY,lastZ,lastE;
    float xOffset, yOffset, zOffset, eOffset;
    float lastZPrint;
    bool fanOn;
    bool Bwaiting;
    bool Ewaiting;
    bool OK_twice;
    long OK_id;
    int fanVoltage;
    bool powerOn;
    bool relative;
    bool eRelative;
    int debugLevel;
    uint32_t lastline;
    bool hasXHome, hasYHome, hasZHome;
    float printerWidth, printerHeight, printerDepth;
    int tempMonitor;
    bool drawing;
    int layer;
    bool isG1Move;
    bool eChanged;
    float printingTime;
  
    bool sdcardMounted;
    
    std::string firmware;
    std::string firmwareURL;
    std::string protocol;
    std::string machine;
    int binaryVersion;
    bool isRepetier;
    bool isMarlin;
    int extruderCountSend;
    int speedMultiply;
    int flowMultiply;
    
    PrinterTemp& getExtruder(int extruderId);
       
public:
    float pauseX,pauseY,pauseZ,pauseE,pauseF,pauseETem,pauseBTem;
    int pauseFanVoltage;
    bool pauseRelative,pauseErelative;
   
    bool   PrinterPowerDown;
    double jobprintingTime;
    void storePause();
    void injectUnpause();
    bool homeSetFlag;
    
    PrinterState(Printer *p);
    ~PrinterState();
    
    void reset();
    
    /** Returns the extruder temperature structure. 
     @param extruderId Id of the extruder. -1 for active extruder.
     @returns Temperature state of selected extruder.
     */
    const PrinterTemp& getExtruder(int extruderId) const;
    
    /** Analyses the gcode and changes the status variables accordingly. */
    void analyze(GCode &code , bool isJobLine);
    
    /** Analyse the response */
    void analyseResponse(const std::string &res,uint8_t &rtype);
    bool extract(const std::string& source,const std::string& ident,std::string &result);
    
    /** Increases the line counter.
     @returns Increased line number. */
    uint32_t increaseLastline();
    uint32_t decreaseLastline();
    
    void setIsathome();
    uint32_t getLastline() {boost::mutex::scoped_lock l(mutex);return lastline;}
    
    void fillJSONObject(json_spirit::Object &obj);
    std::string getMoveXCmd(double dx,double f);
    std::string getMoveYCmd(double dy,double f);
    std::string getMoveZCmd(double dz,double f);
    std::string getMoveECmd(double de,double f);

};
#endif 
