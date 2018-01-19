
#ifndef _Server__printer__
#define _Server__printer__

#include <iostream>
#include <deque>
#include <list>
#include <boost/shared_ptr.hpp>
#include <boost/thread.hpp>
#include <boost/cstdint.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

#include "json_spirit_value.h"
#include "libconfig.h++"

using namespace boost;

#define MAX_HISTORY_SIZE 50
#define Repetier_FirmWare 0
#define Marlin_FirmWare 1
#define defaultTimeRatio 1.08

class PrinterSerial;
class PrintjobManager;
class PrinterState;
class Printjob;
class GCode;
class GCodeDataPacket;

class PrinterResponse {
public:
    uint32_t responseId;
    boost::posix_time::ptime time;
    std::string message;
    /** 
     1 : Commands
     2 : ACK responses like ok, wait, temperature
     4 : Other responses
     8 : EEPROM 信息
     16：SD卡文件信息
     */
    uint8_t logtype;
    PrinterResponse(const std::string& message,uint32_t id,uint8_t tp);
    std::string getTimeString();
};

class PrinterHistoryLine {
public:
    uint32_t line;
    std::string command;
    PrinterHistoryLine(const std::string& com,int l):command(com),line(l) {}
};

class Printer {
    friend class PrintjobManager;
    
    libconfig::Config config;
    PrintjobManager *jobManager;
    PrintjobManager *modelManager;
    PrintjobManager *scriptManager;
    volatile bool stopRequested;
    boost::shared_ptr<boost::thread> thread;
    boost::mutex mutex;
    boost::mutex responseMutex;
    boost::mutex sendMutex;
    std::deque<boost::shared_ptr<PrinterResponse> > responses;
    PrinterSerial *serial;
    uint32_t lastResponseId;

	std::string confFile;
    
    boost::posix_time::ptime lastTemp; ///< Last temp read. Always access with lastTempMutex
    boost::mutex lastTempMutex;
    void run();
    bool extract(const std::string& source,const std::string& ident,std::string &result);
	std::deque<std::string> manualCommands;
	std::deque<std::string> jobCommands; ///< Buffer of commands comming from a job. Not necessaryly the complete job! Job may refill the buffer if it gets empty.
	std::deque<boost::shared_ptr<GCode> > history; ///< Buffer of the last commands send.
	std::deque<boost::shared_ptr<GCode> > resendLines; ///< Lines for which a resend was requested.
	std::deque<int> nackLines; ///< Length of unacknowledged lines send.
    // Communication handline
    bool readyForNextSend; ///< In pingpong mode indicates that ok was received for the last line.
    bool garbageCleared;
    bool ignoreNextOk; ///< Ignore the next ok because resend already indicated receive
    int receiveCacheFill; ///< Bytes send to the printer that are not acknowledged.
    int resendError;
    int errorsReceived;
    boost::posix_time::ptime lastCommandSend;
    int linesSend;
    std::size_t bytesSend;
    bool filelist;
    int updateTempEvery;
    
    void resendLine(size_t line);
    
    /** Trys to send a GCodeDataPacket. Returns true if the cache rules
     allowed sending. If it was successfull, the line is send and also stored 
     in the history.
     @params dp data packet to send.
     @params gc gcode to save in history.
     @returns true on success. */
    bool trySendPacket(boost::shared_ptr<GCodeDataPacket> &dp,boost::shared_ptr<GCode> &gc);
    void trySendNextLine(); // Send another line if possible
    void close();
    void manageHostCommand(boost::shared_ptr<GCode> &cmd);

public:
	//bool jobPause;  ///1:pause  0:contine
	bool paused;
	//float timeRatio;
	float tRatioK;		//用于迭代的ratioK
	float rRatioK;		//根据实际打印时间计算出来的
	int sleepMs;
	bool running;
    double xmin,xmax;
    double ymin,ymax;
    double zmin,zmax;
    double homex,homey,homez;
    double speedx,speedy,speedz,speedeExtrude,speedeRetract;
    bool hasHeatedBed;
    std::string name;
    bool delta;
    int firmware; //0:Repetier  1:Marlin
    std::string slugName;
    
    std::string device;
    int32_t baudrate;
    bool pingpong;
    int32_t cacheSize;
    bool okAfterResend;
    uint32_t lastSDId;
    uint32_t lastEPRId;
    bool SDendflag;
    bool EPRflag;
    int32_t extruderCount;
    bool active;
    
    int binaryProtocol;
    PrinterState *state;
    
    //构造函数
    Printer(std::string conf);
    ~Printer();
    
    void updateLastTempMutex();
	void adjustTimeRatio(int realTime,int jobEtime,int speedCont);
	
    /** The serial reader calls this for each line received. */
    void analyseResponse(std::string &resp);
    
    /** Add response string to list of responses. Removes oldest response if the
     list gets too long. Thread safe. */
    void addResponse(const std::string& msg,uint8_t rtype);
    
    /** Returns a list with responses, where id is greater as the given response id.
     That way a client can keep track of all responses return from the printer.
     Thread safe. 
     @param resId last known response id.
     @param filter filter selecting which response types should be returned.
     @param lastid last response id contained in list or resId if list is empty.
     */
	boost::shared_ptr<std::list<boost::shared_ptr<PrinterResponse> > > getResponsesSince(uint32_t resId,uint8_t filter,uint32_t &lastid);

    bool shouldInjectCommand(const std::string& cmd);
    
    /** Push a new manual command into the command queue. Thread safe. */
    void injectManualCommand(const std::string& cmd);
    
    /** Push a new command into the job queue. Thread safe. */
    void injectJobCommand(const std::string& cmd);
    
    /** Number of job commands stored */
    size_t jobCommandsStored();
    
    void fillJSONObject(json_spirit::Object &obj);
    void move(double x,double y,double z,double e);
    
    int getOnlineStatus();
    bool realOnline; //by zeegine 需要验证非repetier固件兼容性
    bool getActive();
    void setActive(bool v);
    void getJobStatus(json_spirit::Object &obj);
    void connectionClosed();
    inline PrintjobManager *getJobManager() {return jobManager;}
    inline PrintjobManager *getModelManager() {return modelManager;}
    inline PrintjobManager *getScriptManager() {return scriptManager;}
    
    /** Stop previous pause command */
    void stopPause();
    
    // Public interthread communication methods
    void startThread();
    void stopThread();
    
    void PauseJob();
    void ContinueJob();
    void clearCommands();
};

#endif
