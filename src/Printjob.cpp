
#define _CRT_SECURE_NO_WARNINGS // Disable deprecation warning in VS2005
#define _CRT_SECURE_NO_DEPRECATE 
#define _SCL_SECURE_NO_DEPRECATE 

#include <boost/filesystem.hpp>
#include <stdlib.h>
#include <unistd.h>
#include <vector>
#include <stdio.h>
#include <pwd.h>

#include "printer.h"
#include "global_config.h"
#include "RLog.h"
#include "Printjob.h"

using namespace std;
using namespace boost;
using namespace boost::filesystem;

#if defined(_WIN32)
#define _CRT_SECURE_NO_WARNINGS // Disable deprecation warning in VS2005
#endif

typedef vector<path> pvec;             // store paths
typedef list<shared_ptr<Printjob> > pjlist;
std::string _snapsDir = "/var/lib/ZeegineServer/storage/Snaps";
std::string _videoDir = "/var/lib/ZeegineServer/storage/video";

char *substrend(const char *str, int n)
{
    char * substr = (char*) malloc (n+1);
    int length = strlen(str);
    if(n >= length)
    {
        strcpy(substr, str);
        return substr;
    }
    int i, k = 0;
    for(i = strlen(str) - n ; i < strlen(str); i++)
    {
        substr[k++] = str[i];
    }
    substr[k] = '\0';
    return substr;
}
//新建PrintjobManager时做的初始化，共有jobManager，modelManager，scriptManager三个class
PrintjobManager::PrintjobManager(string dir,Printer *_prt,bool _scripts) {
    scripts = _scripts;
    printer = _prt;
    char lc = dir[dir.length()-1];
    if(lc=='/' || lc=='\\') //斜杠或反斜杠
       dir = dir.substr(0,dir.length()-1);
    directory = dir;
    lastid = 0;
    path p(directory);
    try {
        if(!exists(p)) { // First call - create directory
            if(!create_directories(p)) {
                cerr << "error: Unable to create job directory " << dir << "." << endl;
                exit(-1);
            }
        }
        // Test if job directory is directory
        if(!is_directory(p)) {
            cerr << "error: " << dir << " is not a directory!" << endl;
            exit(-1);
        }
        // remove unfinished jobs
        cleanupUnfinsihed();
        
        // Read printjobs contained
        pvec v;
        copy(directory_iterator(p), directory_iterator(), back_inserter(v));
        sort(v.begin(), v.end());
        if(scripts) {
            bool hasStart = false,hasEnd = false,hasPause=false,hasKill=false;
            bool hasScript1=false,hasScript2=false,hasScript3=false,hasScript4=false,hasScript5=false;
            for (pvec::const_iterator it (v.begin()); it != v.end(); ++it)
            {
                PrintjobPtr pj(new Printjob((*it).string(),false));
                files.push_back(pj);
                string name = it->filename().string();
                if(name=="Start.g") hasStart = true;
                if(name=="End.g") hasEnd = true;
                if(name=="Pause.g") hasPause = true;
                if(name=="Kill.g") hasKill = true;
                if(name=="Script 1.g") hasScript1 = true;
                if(name=="Script 2.g") hasScript2 = true;
                if(name=="Script 3.g") hasScript3 = true;
                if(name=="Script 4.g") hasScript4 = true;
                if(name=="Script 5.g") hasScript5 = true;
            }
            if(!hasStart) {files.push_back(PrintjobPtr(new Printjob(directory+"/Start.g",true,true)));}
            if(!hasEnd) {files.push_back(PrintjobPtr(new Printjob(directory+"/End.g",true,true)));}
            if(!hasPause) {files.push_back(PrintjobPtr(new Printjob(directory+"/Pause.g",true,true)));}
            if(!hasKill) {files.push_back(PrintjobPtr(new Printjob(directory+"/Kill.g",true,true)));}
            if(!hasScript1) {files.push_back(PrintjobPtr(new Printjob(directory+"/Script 1.g",true,true)));}
            if(!hasScript2) {files.push_back(PrintjobPtr(new Printjob(directory+"/Script 2.g",true,true)));}
            if(!hasScript3) {files.push_back(PrintjobPtr(new Printjob(directory+"/Script 3.g",true,true)));}
            if(!hasScript4) {files.push_back(PrintjobPtr(new Printjob(directory+"/Script 4.g",true,true)));}
            if(!hasScript5) {files.push_back(PrintjobPtr(new Printjob(directory+"/Script 5.g",true,true)));}
        } else {
            for (pvec::const_iterator it (v.begin()); it != v.end(); ++it)
            {
            
            	if (!strcmp(substrend(directory.c_str(), 4), "jobs")) {
                		if(it->extension()==".g")
                		remove(*it);
                }
                
                PrintjobPtr pj(new Printjob((*it).string(),false));
                if(it->extension()==".b") pj->setBreaked();
                if(!pj->isNotExistent()) {
                    files.push_back(pj);
                }
                // Extract id for last id;
                string sid = it->filename().string();
                size_t upos = sid.find('_');
                if(upos!=string::npos) {
                    sid = sid.substr(0,upos);
                    lastid = atoi(sid.c_str());
                }
            }
        }
    } catch(const filesystem_error& ex)
    {
		cerr << "error: Unable to create or access job directory " << dir << ":" << ex.what() << endl;
        exit(-1);
    }
}

//将上传未完成的任务清除
void PrintjobManager::cleanupUnfinsihed() {
    path p(directory);
    pvec v;
    copy(directory_iterator(p), directory_iterator(), back_inserter(v));
    for (pvec::const_iterator it (v.begin()); it != v.end(); ++it)
    {
        if(it->extension()==".u") {
            remove(*it);
        }
    }
}

//清理job列表
void PrintjobManager::clearList(){
	files.clear();
}
//清理打印job文件夹
void PrintjobManager::clearFolder(){
	//RLog::log("-1");
    path p(directory);
    pvec v;
    //RLog::log("0");
    copy(directory_iterator(p), directory_iterator(), back_inserter(v));
    //RLog::log("1");
    for (pvec::const_iterator it (v.begin()); it != v.end(); ++it) remove(*it);
    //RLog::log("2");
}

//将打印任务的ID编码到文件名中（初始化文件名）
std::string PrintjobManager::encodeName(int id,std::string name,std::string postfix,bool withDir) {
    string res;
    char buf[16];
    sprintf(buf,"%08d_",id);
    if(withDir)
        res = directory+"/";
    res+=string(buf)+"90000000T"+"90000000M"+"900000L"+"90000000I~"+name+"."+postfix;
    //%%%%%%%%%%%%%%%%%%%时间%%%%%%%耗材量%%%%%%层数%%%%%速度累加%%%%%%%%%%%%%%%%%%%%%%%%%%%%%//
    return res;
}

//从编码后的文件名中提取任务名称
std::string PrintjobManager::decodeNamePart(std::string file) {
    path p(file);
    string name = p.filename().stem().string();
    size_t upos = name.find('~');
    if(upos!=string::npos)
        name = name.substr(upos+1);
    return name;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////

//将GCodeData的信息编码到文件名中
std::string PrintjobManager::encodeDataPart(GCodeData* data , std::string file, bool withDir) {
    
    path p(file);
    string name = p.filename().string();
    string res;
    char tbuf[16];
	char mbuf[16];
	char lbuf[16];
	char ibuf[16];
    sprintf(tbuf,"%08ldT",(long)data->time);  		//时间     8位
	sprintf(mbuf,"%08ldM",(long)data->Mlength);		//耗材     8位
	sprintf(lbuf,"%06ldL",(long)data->layer);		//层数信息 6位	
	sprintf(ibuf,"%08ldI",(long)data->speedCont);	//总速度   8位
	size_t vpos = name.find('~');
	size_t upos = name.find('_');
    if(withDir)
        res = directory+"/";
    res+= name.substr(0,upos+1)+string(tbuf)+string(mbuf)+string(lbuf)+string(ibuf)+"~"+name.substr(vpos+1);
    return res;
}

//将打印任务的文件名解码成GCodeDate结构体
GCodeData PrintjobManager::decodeDataPart(std::string file) {
    path p(file);
    GCodeData data;
	string filename = p.filename().stem().string();
    size_t _pos = filename.find('_');
    size_t Tpos = filename.find("T");
	size_t Mpos = filename.find("M");
	size_t Lpos = filename.find("L");
	size_t Ipos = filename.find("I");
    if(_pos!=string::npos || Tpos!=string::npos)
        data.time = atoi(filename.substr(_pos+1,Tpos-1).c_str());
	else data.time = 0;
	if(Tpos!=string::npos || Mpos!=string::npos)
        data.Mlength = atoi(filename.substr(Tpos+1,Mpos-1).c_str());
	else data.Mlength = 0;
	if(Mpos!=string::npos || Lpos!=string::npos)
        data.layer = atoi(filename.substr(Mpos+1,Lpos-1).c_str());
	else data.layer = 0;
	if(Lpos!=string::npos || Ipos!=string::npos)
        data.layer = atoi(filename.substr(Lpos+1,Ipos-1).c_str());
	else data.speedCont = 0;
	return data;
}

//返回打印任务的运算结果
//需要NA端对返回值90000000进行判断
long Printjob::getTime(){
	if(PrintjobManager::decodeDataPart(file).time != 90000000) jdata->time = PrintjobManager::decodeDataPart(file).time;
	if(jdata->time < 0 || jdata->time > 90000000) jdata->time = 0;
	return (long)jdata->time;
}
long Printjob::getLayer(){
	if(PrintjobManager::decodeDataPart(file).layer != 900000) jdata->layer = PrintjobManager::decodeDataPart(file).layer;
	if(jdata->layer < 0 || jdata->layer > 900000) jdata->layer = 0;
	return (long)jdata->layer;
}
long Printjob::getMaterial(){
	if(PrintjobManager::decodeDataPart(file).Mlength != 90000000) jdata->Mlength = PrintjobManager::decodeDataPart(file).Mlength;
	if(jdata->Mlength < 0 || jdata->Mlength > 90000000) jdata->Mlength = 0;
	return (long)jdata->Mlength;
}
long Printjob::getSpeedCon(){
	if(PrintjobManager::decodeDataPart(file).speedCont != 90000000) jdata->speedCont = PrintjobManager::decodeDataPart(file).speedCont;
	if(jdata->speedCont < 0 || jdata->speedCont > 90000000) jdata->speedCont = 0;
	return (long)jdata->speedCont;
}

void Printjob::setTimeRatio(Printer *p){
	timeRatio = 1.0+p->tRatioK/100.0*jdata->speedCont/jdata->time;
	if(timeRatio>1.5 || timeRatio<1) timeRatio = defaultTimeRatio;//假如次模型运算后的timeRatio大于1.5，采用默认ratio 1.08
}

////////////////////////////////////////////////////////////////////////////////////////////////
//解析任务ID
int PrintjobManager::decodeIdPart(std::string file) {
    path p(file);
    string name = p.filename().stem().string();
    //size_t vpos = name.find("~");
    size_t upos = name.find('_');
    if(upos!=string::npos)
        name = name.substr(0,upos);
    else return -1;
    return atoi(name.c_str());
}

//push out打印任务的信息，组织json Object数据
void PrintjobManager::fillSJONObject(std::string name,json_spirit::Object &o) {
    mutex::scoped_lock l(filesMutex);
    using namespace json_spirit;
    Array a;
    list<shared_ptr<Printjob> >::iterator it = files.begin(),ie = files.end();
    for(;it!=ie;it++) {
        Object j;
        Printjob *job = (*it).get();
        j.push_back(Pair("id",job->getId()));
        j.push_back(Pair("name",job->getName()));
        j.push_back(Pair("length",(int)job->getLength()));
		if(job->getState() == Printjob::startUpload) j.push_back(Pair("uploadPos",(int)job->uploadPos));
		else if(job->Calculating) 
		{
			if(job->isJob)
			{
				PrintjobPtr sourceJob = printer->getModelManager()->findById(job->getSourceid());
				if(sourceJob->Calculating) {
					int calPer = (100*sourceJob->calPos/sourceJob->getLength());
					j.push_back(Pair("time",-calPer));
					j.push_back(Pair("material",-calPer));
					j.push_back(Pair("layer",-calPer));
				}
				else{
					job->setdata(sourceJob->getdata());
					string newjobname = printer->getJobManager()->encodeDataPart(job->getdata(),job->getFilename(),true);
					
					rename(job->getFilename(),newjobname);
					job->setFilename(newjobname);
					
					job->Calculating = false;
					job->setTimeRatio(printer);
					j.push_back(Pair("time",(int)(job->getTime()*job->timeRatio)));
					//RLog::log("job->timeRatio=@", job->timeRatio);
					j.push_back(Pair("material",(int)job->getMaterial()));
					j.push_back(Pair("layer",(int)job->getLayer()));
				}
			}
			else
			{
				int calPerM = (100*job->calPos/job->getLength());
				//RLog::log("calperM = @",calPerM);
				j.push_back(Pair("time",(int)-calPerM));
				j.push_back(Pair("material",(int)-calPerM));
				j.push_back(Pair("layer",(int)-calPerM));
			}
		}
		
		else{
			j.push_back(Pair("time",(int)(job->getTime()*job->timeRatio)));
			j.push_back(Pair("material",(int)job->getMaterial()));
			j.push_back(Pair("layer",(int)job->getLayer()));
			
		}
        switch(job->getState()) {
            case Printjob::startUpload:
                j.push_back(Pair("state","uploading"));
                break;
            case Printjob::stored:
                j.push_back(Pair("state","stored"));
			//	if(job->getTime() != 0) j.push_back(Pair("time",job->getTime()));
			//	else j.push_back(Pair("time","Calculating" ));
                break;
            case Printjob::running:
                j.push_back(Pair("state","running"));
                j.push_back(Pair("done",job->percentDone()));
                j.push_back(Pair("jobprintingTime",(int)(printer->state->jobprintingTime*job->timeRatio)));
    			//j.push_back(Pair("printingTime",(int)printer->state->printingTime));
    			j.push_back(Pair("jobpaused",printer->paused));
				j.push_back(Pair("jobrunning",printer->running));
                break;
            case Printjob::finished:
                j.push_back(Pair("state","finsihed"));
                break;
            case Printjob::doesNotExist:
                j.push_back(Pair("state","error"));
                break;
            case Printjob::breaked:
                j.push_back(Pair("state","breaked"));
                break;
        }
        a.push_back(j);
    }
    o.push_back(Pair(name,a));
}

//返回打印任务的名称与进度（用于打印机列表页）
void PrintjobManager::getJobStatus(json_spirit::Object &obj) {
    mutex::scoped_lock l(filesMutex);
    using namespace json_spirit;
    Printjob *job = runningJob.get();
    if(job==NULL) {
        obj.push_back(Pair("job","none"));
    } else {
        obj.push_back(Pair("job",job->getName()));
        obj.push_back(Pair("document done",job->percentDone()));
        obj.push_back(Pair("ID",job->getId()));
    }
}

//通过ID寻找打印任务，不另外加文件锁
PrintjobPtr PrintjobManager::findByIdInternal(int id) {
    pjlist::iterator it = files.begin(),ie=files.end();
    for(;it!=ie;it++) {
        if((*it)->getId()==id)
            return *it;
    }
    return shared_ptr<Printjob>();
}

//通过名字寻找打印任务（用于script）
PrintjobPtr PrintjobManager::findByName(string name) {
    mutex::scoped_lock l(filesMutex);
    pjlist::iterator it = files.begin(),ie=files.end();
    for(;it!=ie;it++) {
        if((*it)->getName()==name)
            return *it;
    }
    return shared_ptr<Printjob>();
}

//通过FileName寻找打印任务
PrintjobPtr PrintjobManager::findByFile(string file) {
    mutex::scoped_lock l(filesMutex);
    pjlist::iterator it = files.begin(),ie=files.end();
    for(;it!=ie;it++) {
        if((*it)->getFilename()==file)
            return *it;
    }
    return shared_ptr<Printjob>();
}

//通过ID寻找打印任务
PrintjobPtr PrintjobManager::findById(int id) {
    mutex::scoped_lock l(filesMutex);
    return findByIdInternal(id);
}

//创建打印任务
PrintjobPtr PrintjobManager::createNewPrintjob(std::string name) {
    mutex::scoped_lock l(filesMutex);
    lastid++;
    PrintjobPtr job(new Printjob(encodeName(lastid, name, "u", true),true));
    files.push_back(job);
    return job;
}
//重命名打印任务
void PrintjobManager::RenamePrintjob(PrintjobPtr job,string newname){
    path p(job->getFilename());
    string name = p.string();
    size_t upos = name.find('~');
    if(upos!=string::npos)
        name = name.substr(0,upos+1)+newname+"."+name.substr(name.length()-1);
        
    rename(job->getFilename(),name); //变量，内存
    job->setFilename(name); //文件，外存
}
//完成打印任务的创建
void PrintjobManager::finishPrintjobCreation(PrintjobPtr job,string namerep,size_t sz)
{
    mutex::scoped_lock l(filesMutex);
    if(job->getName().length()>0)
        namerep = job->getName();
    if(namerep.length()==0) {
        char buf[50];
        sprintf(buf,"Job %d",job->getId());
        namerep = static_cast<string>(buf);
    }
    string newname = encodeName(job->getId(),namerep,"g", true);
    try {
        rename(job->getFilename(), newname);
        job->setFilename(newname);
        job->setLength(sz);
        job->setStored();
    } catch(std::exception e) {
        RLog::log("Error creating new job: @",e.what());
        string msg= static_cast<string>("Error creating new job: ")+e.what();
        string answer = static_cast<string>("/printer/msg/")+printer->slugName+
            static_cast<string>("?a=ok");
        gconfig->createMessage(msg,answer);
        files.remove(job);
    }    
}

//删除打印任务及文件
void PrintjobManager::RemovePrintjob(PrintjobPtr job) {
    mutex::scoped_lock l(filesMutex);
    path p(job->getFilename());
    if(exists(p) && is_regular_file(p))
        remove(p);
    files.remove(job);
}

//开始打印任务，打开文件流
void PrintjobManager::startJob(int id) {
    mutex::scoped_lock l(filesMutex);
    if(runningJob.get()) return; // Can't start if old job is running
    runningJob = findByIdInternal(id);
    if(!runningJob.get()) return; // unknown job
    runningJob->setRunning();
    runningJob->start();
    printer->getScriptManager()->pushCompleteJob("Start");
    if(jobin.is_open()) jobin.close();
    jobin.open(runningJob->getFilename().c_str(),ifstream::in);
    if(!jobin.good()) {
        RLog::log("Failed to open job file @",runningJob->getFilename());
        string msg= "Failed to open job file "+runningJob->getFilename();
        string answer = "/printer/msg/"+printer->slugName+"?a=ok";
        gconfig->createMessage(msg,answer);
    }
	printer->state->jobprintingTime = 0;
	runningJob->last_percentDone = 0;
	printer->running = true;
	std::system("rm -rf /var/lib/ZeegineServer/storage/Snaps/*");
}

//终止打印任务
void PrintjobManager::killJob(int id) {
    mutex::scoped_lock l(filesMutex);
    if(!runningJob.get()) return; // Can't start if old job is running
    if(jobin.is_open() && jobin.eof()) {
        jobin.close();
    }
    try {
        files.remove(runningJob);
        remove(path(runningJob->getFilename())); // Delete file from disk
    } catch(std::exception &e) {
        string msg= "Failed to remove killed job file "+runningJob->getFilename();
        string answer = "/printer/msg/"+printer->slugName+"?a=ok";
        gconfig->createMessage(msg,answer);
    }
    runningJob.reset();
    mutex::scoped_lock l2(printer->sendMutex); // Remove buffered commands
    printer->jobCommands.clear();
    l2.unlock();
    //终止后关闭打印机功率器件并归零
    if(printer->delta) printer->injectManualCommand("G28");
	else printer->injectManualCommand("G28 X0 Y0");
	printer->injectManualCommand("M104 S0");
	printer->injectManualCommand("M140 S0");
	printer->injectManualCommand("M106 S0");
	printer->injectManualCommand("M84");
	printer->paused = false;
	printer->running = false;
    printer->getScriptManager()->pushCompleteJob("End");
}


//存储中断任务的状态
void PrintjobManager::StoreCurrentJob() {  
    mutex::scoped_lock l(filesMutex);
    if(!runningJob.get()) return; // no running job
    //runningJob->setRPos(jobin.tellg()-runningJob->storedByte);
    //RLog::log("read Bytes @",(int)jobin.tellg());
    if(jobin.is_open() && jobin.eof()) {
        jobin.close();
    }
    runningJob->setBreaked();
    path p(runningJob->getFilename());
	string filename = p.filename().stem().string();
    string newname = directory+"/"+ filename+".b";
    
    rename(runningJob->getFilename(),newname);
    runningJob->setFilename(newname);
    
    mutex::scoped_lock l2(printer->sendMutex); // Remove buffered commands
    printer->jobCommands.clear();
    l2.unlock();
    string bdFile = gconfig->getStorageDirectory() + printer->slugName + "/"+"BreakData.cfg";
    try {
		BreakData.readFile(bdFile.c_str());
		libconfig::Setting &refresh       =    BreakData.lookup("refresh");
		libconfig::Setting &jobName       =    BreakData.lookup("printer.jobInfo.jobName");
		libconfig::Setting &jobPath       =    BreakData.lookup("printer.jobInfo.jobPath");
		libconfig::Setting &jobID         =    BreakData.lookup("printer.jobInfo.jobID");
		libconfig::Setting &breakPosition =    BreakData.lookup("printer.jobInfo.breakPosition");
		libconfig::Setting &jobPrintTime  =    BreakData.lookup("printer.jobInfo.jobPrintTime");
		libconfig::Setting &xBreak        =    BreakData.lookup("printer.Position.xBreak");
		libconfig::Setting &yBreak        =    BreakData.lookup("printer.Position.yBreak");
		libconfig::Setting &zBreak        =    BreakData.lookup("printer.Position.zBreak");
		libconfig::Setting &eBreak        =    BreakData.lookup("printer.Position.eBreak");
		libconfig::Setting &relative      =    BreakData.lookup("printer.Position.relative");
		libconfig::Setting &eRelative     =    BreakData.lookup("printer.Position.eRelative");
		libconfig::Setting &BreakSpeed    =    BreakData.lookup("printer.BreakSpeed");
		libconfig::Setting &EtemperSet    =    BreakData.lookup("printer.extruder.temperSet");
		libconfig::Setting &BtemperSet    =    BreakData.lookup("printer.bed.temperSet");
		libconfig::Setting &coolingFan    =    BreakData.lookup("printer.coolingFan");
		
		refresh = true;
		jobName = runningJob->getName();
		jobPath = runningJob->getFilename();
		jobID = runningJob->getId();
		breakPosition = (int)runningJob->getRPos();
		jobPrintTime = printer->state->jobprintingTime;
		xBreak = printer->state->pauseX;
		yBreak = printer->state->pauseY;
		zBreak = printer->state->pauseZ;
		eBreak = printer->state->pauseE;
		relative = printer->state->pauseRelative;
		eRelative = printer->state->pauseErelative;
		BreakSpeed = printer->state->pauseF;
		EtemperSet = printer->state->pauseETem;
		BtemperSet = printer->state->pauseBTem;
		coolingFan = printer->state->pauseFanVoltage;
		
		BreakData.writeFile(bdFile.c_str());
	} catch(libconfig::ParseException &pe) {
		cerr << "error: " << pe.getError() << " line:" << pe.getLine() << " file:" << pe.getFile() << endl;
		exit(-1);
	}
	runningJob.reset();
	printer->running = false;
}

//恢复中断的任务
void PrintjobManager::RecoverCurrentJob() {  
    string bdFile = gconfig->getStorageDirectory() + printer->slugName + "/"+"BreakData.cfg";
    std::string jobName;
	std::string jobPath;
	int jobID;
	int breakPosition;
	float jobPrintTime;
	float xBreak;
	float yBreak;
	float zBreak;
	float eBreak;
	bool relative;
	bool eRelative ;
	float BreakSpeed;
	float EtemperSet;
	float BtemperSet;
	int coolingFan;
    try {
		BreakData.readFile(bdFile.c_str());
        libconfig::Setting &refresh = BreakData.lookup("refresh");
        refresh = false;
		BreakData.lookupValue("printer.jobInfo.jobName",jobName);
		BreakData.lookupValue("printer.jobInfo.jobPath",jobPath);
		BreakData.lookupValue("printer.jobInfo.jobID",jobID);
		BreakData.lookupValue("printer.jobInfo.breakPosition",breakPosition);
		BreakData.lookupValue("printer.jobInfo.jobPrintTime",jobPrintTime);
		BreakData.lookupValue("printer.Position.xBreak",xBreak);
		BreakData.lookupValue("printer.Position.yBreak",yBreak);
		BreakData.lookupValue("printer.Position.zBreak",zBreak);
		BreakData.lookupValue("printer.Position.eBreak",eBreak);
		BreakData.lookupValue("printer.Position.relative",relative);
		BreakData.lookupValue("printer.Position.eRelative",eRelative);
		BreakData.lookupValue("printer.BreakSpeed",BreakSpeed);
		BreakData.lookupValue("printer.extruder.temperSet",EtemperSet);
		BreakData.lookupValue("printer.bed.temperSet",BtemperSet);
		BreakData.lookupValue("printer.coolingFan",coolingFan);
		BreakData.writeFile(bdFile.c_str());
		
    } catch(libconfig::ParseException &pe) {
		cerr << "error: " << pe.getError() << " line:" << pe.getLine() << " file:" << pe.getFile() << endl;
		exit(-1);
	}
    if(!exists(jobPath.c_str())) return;
    mutex::scoped_lock l(filesMutex);
    /**************************恢复打印机状态********************************/
    char buf[200];
    //恢复底板温度
	if(BtemperSet >30) {
		sprintf(buf,"M190 S%.2f",BtemperSet);
		printer->injectManualCommand(buf);
	}
	//恢复挤出头温度
	if(EtemperSet >60) {
		sprintf(buf,"M109 S%.2f",EtemperSet);
		printer->injectManualCommand(buf);
	}
	if(printer->delta) printer->injectManualCommand("G28");			//归零
	else {
		printer->injectManualCommand("G28 X0 Y0");					//xyz打印机水平归零，z方向设置当前坐标
		sprintf(buf,"G92 Z%.2f",zBreak);
		printer->injectManualCommand(buf);
	}
	printer->injectManualCommand("G90");							//设置绝对坐标
	sprintf(buf,"G1 X%.2f Y%.2f Z%.2f F9000",xBreak,yBreak,zBreak);	//运动到终止位置
	printer->injectManualCommand(buf);
	sprintf(buf,"G92 E%.4f",eBreak-5);								//挤出机位置恢复
	printer->injectManualCommand(buf);
    printer->injectManualCommand(relative ? "G91" : "G90");			//恢复中断时的坐标模式
    sprintf(buf,"G1 F%.0f",BreakSpeed);								//恢复中断时的打印速度
    printer->injectManualCommand(buf);
    sprintf(buf,"M106 S%d",coolingFan); 							//恢复中断时的风扇速度
    printer->injectManualCommand(buf);
    /**************************继续中断任务********************************/
    if(runningJob.get()) return; // Can't start if old job is running
    runningJob = findByIdInternal(jobID);
    if(!runningJob.get()) return; // unknown job
    runningJob->setRunning();
    runningJob->start();
    printer->getScriptManager()->pushCompleteJob("Start");
    if(jobin.is_open()) jobin.close();
    jobin.open(runningJob->getFilename().c_str(),ifstream::in);
    if(!jobin.good()) {
        RLog::log("Failed to open job file @",runningJob->getFilename());
        string msg= "Failed to open job file "+runningJob->getFilename();
        string answer = "/printer/msg/"+printer->slugName+"?a=ok";
        gconfig->createMessage(msg,answer);
    }
    jobin.seekg((int)breakPosition,ios::beg);
    //RLog::log("pos:@",(int)jobin.tellg());
    char buff[200];
    jobin.getline(buff, 200);
	printer->state->jobprintingTime = jobPrintTime;
	printer->running = true;
}


//清理中断的任务
void PrintjobManager::CleanBreakJob(){
	std::string jobPath;
	try {
			string bdFile = gconfig->getStorageDirectory() + printer->slugName + "/"+"BreakData.cfg";
			BreakData.readFile(bdFile.c_str());
			BreakData.lookupValue("printer.jobInfo.jobPath",jobPath);
			//RLog::log("bdFile=@", bdFile);
			//RLog::log("jobpath=@", jobPath);
	        
	        remove(path(jobPath));	 				//删除中断的任务文件
	        files.remove(findByFile(jobPath)); 		//删除中断的任务class
	        
	        libconfig::Setting &refresh       =    BreakData.lookup("refresh");
        	refresh 		= 	 false;
        	BreakData.writeFile(bdFile.c_str());
	    } catch(std::exception) {
	        RLog::log("error: Failed to remove finished job @",runningJob->getFilename());
	        string msg= "Failed to remove finished job file "+runningJob->getFilename();
	        string answer = "/printer/msg/"+printer->slugName+"?a=ok";
	        gconfig->createMessage(msg,answer);
	    }
}


//管理打印任务，定时循环
void PrintjobManager::manageJobs() {
    mutex::scoped_lock l(filesMutex);
    if(!runningJob.get()) return; // unknown job
    if(jobin.good()) {
        string line;
        size_t n = 100-printer->jobCommandsStored();  	//每次读取十条GCode
        if(n>10) n = 10;
        char buf[200];									//每条长度不大于200
       // if(jobin.eof()) printer->running = false;		//读到文件尾部时设置打印结束
        while(n && !jobin.eof()) {
        	
            jobin.getline(buf, 200); // Strips \n
            size_t l = strlen(buf);
            if(buf[l]=='\r')
                buf[l] = 0;
            //printf("1buf = %s\n", buf);
            printer->injectJobCommand(static_cast<string>(buf));
            //printf("2buf = %s\n", buf);
            runningJob->incrementLinesSend();
            n--;
        }
        runningJob->setPos(jobin.tellg());				//设置读文件指针
        //共计10秒钟，240帧的视频，0.0417*10
        if((runningJob->percentDone()-0.417) > runningJob->last_percentDone)
        {
        	runningJob->last_percentDone += 0.417;		
        	std::system("curl http://localhost:8081/?action=video_make");
        }
    }
    //读到文件尾部时设置打印结束 
    if(jobin.is_open() && !jobin.good() ) {
        jobin.close();
        runningJob->stop(printer,runningJob->getdata()->time,runningJob->getdata()->speedCont);
        files.remove(runningJob);
        printer->running = false;
        //删除完成的任务
		try {
		    remove(path(runningJob->getFilename())); // Delete file from disk
		} catch(std::exception) {
		    RLog::log("error: Failed to remove finished job @",runningJob->getFilename());
		    string msg= "Failed to remove finished job file "+runningJob->getFilename();
		    string answer = "/printer/msg/"+printer->slugName+"?a=ok";
		    gconfig->createMessage(msg,answer);
		}
		string rjn = runningJob->getFilename();
		runningJob.reset();
		l.unlock();
		printer->scriptManager->pushCompleteJobNoBlock("End");
		 
	    //完成打印job写入日志
		FILE *fp;
		char action[] = " <finish job> ";
		char s[20] = {0};
		char src[80] = {0};
		struct tm *newtime;
		time_t aclock;
		time(&aclock);
		newtime = localtime(&aclock);
		strftime(s, sizeof(s), "%Y-%m-%d,%H:%M:%S", newtime);
		strcpy(src, getpwuid(getuid())->pw_dir);
		strcat(src, "/zeegine/printer/zeelinker.log");
		//fp = fopen("/home/pi/zeegine/printer/zeelinker.log", "a+");
		fp = fopen("/home/zhushanbo/zeegine/printer/zeelinker.log", "a+");
		//fp = fopen(src, "a+");
		if (fp == NULL) {
			perror("fopen failed");
			exit(1);
		}
		fwrite(s, strlen(s), 1, fp);
		fwrite(action, strlen(action), 1, fp);
		//RLog::log("rjn = @", rjn);
		path p(rjn);
		string filename = p.filename().stem().string();
		int ret = fwrite(filename.c_str(), filename.length(), 1, fp);
		if(ret <= 0) {
			perror("fwrite failed");
			exit(1);
		}
		fwrite("\n", 1, 1, fp);
		fclose(fp);
		
		/*
        //================================开始制作视频==============================//
		try {
			//判断snaps文件夹是否存在
			path pv(_snapsDir);
		    if(!exists(pv)) { 
		        if(!create_directories(pv)) {
		            cerr << "error: Unable to create snaps directory " << _snapsDir << "." << endl;
		            exit(-1);
		        }
		    }
		    //判断snaps是否是文件夹
		    if(!is_directory(pv)) {
		        cerr << "error: " << _snapsDir << " is not a directory!" << endl;
		        exit(-1);
		    }
		    //判断snaps文件夹是否为空
		    if(!boost::filesystem::is_empty(pv)){
		    	pvec v;
				copy(directory_iterator(pv), directory_iterator(), back_inserter(v));				
				sort(v.begin(), v.end());				//按文件名排序				
				pvec::const_iterator itb(v.begin());				
				pvec::const_iterator ite(v.end()-1);				
				path pB((*itb).string());				
				path pE((*ite).string());
				string nameB = pB.filename().stem().string();
				string nameE = pE.filename().stem().string();
				int jpgBN = atoi(nameB.c_str());		//读取首张与最后一张图片的序号（如果中间有空的可以考虑用迭代器）
				int jpgEN = atoi(nameE.c_str());
				path p(rjn);
				string jWithDir = _videoDir + "/" + p.filename().string() + ".avi";
				//string jTempWithDirAvi = _videoDir + "/" + "temp.avi";
				//string jTempWithDirMp4 = _videoDir + "/" + "zeegine.mp4";
				jpg2mp4(jpgBN, jpgEN, jWithDir);
				
		    	std::system("rm -rf /var/lib/ZeegineServer/storage/Snaps/*");
        	}
        }catch(std::exception){
        
        	RLog::log("error: Failed to create video.");
        }
		//===========================完成制作视频===========================================//
		*/
    }
}

//一次性将script的GCode全部push到Commands list 里面
void PrintjobManager::pushCompleteJob(std::string name,bool beginning) {
    PrintjobPtr pj = findByName(name);
    if(!pj.get()) return;
    mutex::scoped_lock l(filesMutex);
    ifstream in;
    mutex::scoped_lock l2(printer->sendMutex);
    std::deque<std::string> &list = ((*pj).getName()=="Pause" ? printer->manualCommands : printer->jobCommands);
    try {
        in.open(pj->getFilename().c_str(),ifstream::in);
        char buf[200];
        size_t line = 0;
        
        while(!in.eof()) {
            in.getline(buf, 200); // Strips \n
            size_t l = strlen(buf);
            if(buf[l]=='\r')
                buf[l] = 0;
            string cmd(buf);
            if(!printer->shouldInjectCommand(cmd)) continue;
            line++;
            if(beginning) {
                list.push_front(cmd);
            } else {
                list.push_back(cmd);
            }
        }
        if(beginning) { // Reverse lines at beginning
            reverse(printer->jobCommands.begin(),printer->jobCommands.begin()+line);
        }
        in.close();
    } catch(std::exception) {}
}
//一次性将script的GCode全部push到Commands list 里面
void PrintjobManager::pushCompleteJobNoBlock(std::string name,bool beginning) {
    PrintjobPtr pj = findByName(name);
    if(!pj.get()) return;
    mutex::scoped_lock l(filesMutex);
    ifstream in;
    std::deque<std::string> &list = ((*pj).getName()=="Pause" ? printer->manualCommands : printer->jobCommands);
    try {
        in.open(pj->getFilename().c_str(),ifstream::in);
        char buf[200];
        size_t line = 0;
        
        while(!in.eof()) {
            in.getline(buf, 200); // Strips \n
            size_t l = strlen(buf);
            if(buf[l]=='\r')
                buf[l] = 0;
            string cmd(buf);
            if(!printer->shouldInjectCommand(cmd)) continue;
            line++;
            if(beginning) {
                list.push_front(cmd);
            } else {
                list.push_back(cmd);
            }
        }
        if(beginning) { // Reverse lines at beginning
            reverse(printer->jobCommands.begin(),printer->jobCommands.begin()+line);
        }
        in.close();
    } catch(std::exception) {}
}

// ============= Printjob =============================
Printjob::Printjob(string _file,bool newjob,bool _script) {
    file = _file;
    script = _script;
    path p(file);
    pos = 0;
    state = stored;
    length = 0;
    calPos = 0;
    Rpos = 0;
    jobSizeMul = 1;//缩放比例，默认1
	dataCal = new TimeEstimateCalculator(this); 
	isJob = false;
	Calculating = false;
	timeSet = false;
	timeRatio = defaultTimeRatio; //初始默认值在Printer.h
	sourceid = 0;
	dataCal->data = PrintjobManager::decodeDataPart(file);
	setdata(&dataCal->data);
    //material_zg = 0;
	
    if(script) id=0;
    else id = PrintjobManager::decodeIdPart(file);
    if(script && newjob) {
        fstream f;
        f.open( file.c_str(), ios::out );
        f << flush;
        f.close();
        return;
    }
    else if(newjob) {state = startUpload; return;}
    try {
        if(exists(p) && is_regular_file(p))
            length = (size_t)file_size(file);
        else state = doesNotExist;
    } catch(const filesystem_error& )
    {
        state = doesNotExist;
    }

}

std::string Printjob::getName() {
    return PrintjobManager::decodeNamePart(file);
}

//开始任务后的一些初始化
void Printjob::start() {
    linesSend = 0;
    Rpos = 0;
    time = boost::posix_time::microsec_clock::local_time();
}
//结束任务后的一些总结
void Printjob::stop(Printer *p , int jobEtime, int jobspeedCont) {
    posix_time::ptime now  = boost::posix_time::microsec_clock::local_time();
    posix_time::time_duration td(now-time);
    char b[100];
    sprintf(b,"%d:%02d:%02d",td.hours(),td.minutes(),td.seconds());
    string msg = "Print of "+getName()+" on printer "+p->name+ " finished. Send "+intToString(linesSend)+" lines. Printing time: "+b;
    string url = "/printer/msg/"+p->slugName+"?a=jobfinsihed";
    gconfig->createMessage(msg, url);
    int realTime = td.hours()*3600+td.minutes()*60+td.seconds();
    p->adjustTimeRatio(realTime,jobEtime,jobspeedCont);
}
