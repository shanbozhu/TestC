
#ifndef __Printjob__
#define __Printjob__

#include <iostream>
#include <list>
#include <boost/shared_ptr.hpp>
#include <boost/thread.hpp>
#include <fstream>
#include <boost/cstdint.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

#include "json_spirit_value.h"
#include "timeEstimate.h"
#include "jpg2mp4.h"

using namespace boost;
class TimeEstimateCalculator;
class Printer;

class Printjob {
friend class TimeEstimateCalculator;
TimeEstimateCalculator *dataCal;
public:
    enum PrintjobState {startUpload,stored,running,finished,doesNotExist,breaked};

    
	inline TimeEstimateCalculator *getdataCal() {return dataCal;}
    Printjob(std::string _file,bool newjob,bool _script=false);
    
    inline bool isNotExistent() {return state==doesNotExist;}
//get
    std::string getName();
    inline int getId() {return id;}
	inline int getSourceid() {return sourceid;}
    inline size_t getLength() {return length;}
	GCodeData* getdata() {return jdata;}
	long getTime();//需要根据放大倍率jobSizeMul去调整打印时间
	long getLayer();
    long getMaterial();
    long getSpeedCon();
    inline long getRPos() {return Rpos;}
    inline std::string getFilename() {return file;}
    inline PrintjobState getState() {return state;}
    inline float getJobSizeMul() {return jobSizeMul;}
//set
	inline void setdata(GCodeData* d) {jdata = d;}
    inline void setFilename(std::string fname) {file = fname;}
    inline void setBreaked() {state = breaked;}
    inline void setStored() {state = stored;}
    inline void setRunning() {state = running;}
    inline void setLength(size_t l) {length = l;}
    inline void setTime(int t) {jdata->time = t;}
    inline void setLayer(int l) {jdata->layer = l;}
    inline void setMaterial(int m) {jdata->Mlength = m;}
    inline void setPos(long long p) {pos = p;}
    inline void setRPos(long p) {Rpos = p;}
	inline void setSourceid(int i) {sourceid = i;}
	void setTimeRatio(Printer *p);
	inline void setSizeMul(float Mul) {jobSizeMul = Mul; }
	
	inline double percentDone() {return 100.0*pos/(double)length;}
	double last_percentDone ;
    inline void incrementLinesSend() {linesSend++;}
	inline TimeEstimateCalculator* getTimeCal(){return estimateCal;}
    void start();
    void stop(Printer *p , int jobEtime, int jobspeedCont);
	bool isJob;
	bool Calculating;
	bool timeSet;
	long long calPos;
	long long uploadPos;
	long Rpos; //已经成功发送至打印机的GCode字节数，用于恢复中断任务
	float timeRatio;//用于计算打印时长
private:
	TimeEstimateCalculator* estimateCal;
    bool script;
    int id;
	int sourceid;
    std::string file;
    size_t length; //任务文件大小（字节数）
	GCodeData* jdata;
    long long pos; //已读取字节数
    float jobSizeMul;//用于设置模型打印时的缩放比例

    PrintjobState state;
    int linesSend;
    boost::posix_time::ptime time;
};
typedef boost::shared_ptr<Printjob> PrintjobPtr;

/** The PrintjobManager manages a directory full of possible print jobs.
 The files follow a naming convention to force the order of files added.
 Each file has a number_<Name>.<state>
 
 state is u for the time until it is uploaded completely and gets renamed to g
 after upload is complete. At the start all files with .u get deleted as they
 never finished.
 */
class PrintjobManager {
    libconfig::Config BreakData;
    std::string directory;
    std::list<PrintjobPtr> files;
    int lastid;
    boost::mutex filesMutex;
    PrintjobPtr runningJob;
    std::ifstream jobin;
    PrintjobPtr findByIdInternal(int id);
    bool scripts;
    Printer *printer;
public:
	PrintjobPtr getRunningJob() {return runningJob;} 
    PrintjobManager(std::string dir,Printer *p,bool _scripts=false);
    void cleanupUnfinsihed();
    //清空文件夹
    void clearFolder();
    void clearList();
    
    std::string encodeName(int id,std::string name,std::string postfix,bool withDir);
    static std::string decodeNamePart(std::string file);
    static int decodeIdPart(std::string file);
	std::string encodeDataPart(GCodeData* data , std::string file, bool withDir);
	static GCodeData decodeDataPart(std::string file);
    void fillSJONObject(std::string name,json_spirit::Object &o);
	//寻找打印任务
    PrintjobPtr findById(int id);
    PrintjobPtr findByName(std::string name);
    PrintjobPtr findByFile(std::string file);
	//新建打印任务
    PrintjobPtr createNewPrintjob(std::string name);
    void finishPrintjobCreation(PrintjobPtr job,std::string namerep,size_t sz);
	//删除打印任务
    void RemovePrintjob(PrintjobPtr job);
    void startJob(int id);
    /** Kills the current job without removing it from queue. This
     is needed in case the printer gets disconnected. */
    void killJob(int id);
    //重命名打印任务
    void RenamePrintjob(PrintjobPtr job,string newname);
	//意外中断的打印任务的处理
    void StoreCurrentJob();
    void RecoverCurrentJob();
    void CleanBreakJob();
    /** This method is the workhorse for the job printing. It gets called
     frequently and makes sure, the job queue is filled enough for a
     undisrupted print. It will always queue up to 100 commands but no more
     then 10 commands for a call. */
    void manageJobs();
    void getJobStatus(json_spirit::Object &obj);
    /** Pushes the complete content of a job to the job queue
     @param name Name of the printjob 
     @param p Printer for output
     @param beginning Send it to the beginning of the job queue or the end.
     */
    void pushCompleteJob(std::string name,bool beginning = false);
    void pushCompleteJobNoBlock(std::string name,bool beginning = false);
};
#endif
