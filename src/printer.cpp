
#define _CRT_SECURE_NO_WARNINGS // Disable deprecation warning in VS2005
#define _CRT_SECURE_NO_DEPRECATE 
#define _SCL_SECURE_NO_DEPRECATE

#include <ctype.h>
#include <boost/filesystem.hpp>
#include "printer.h"
#include "Printjob.h"
#include "GCode.h"
#include "PrinterSerial.h"
#include "PrinterState.h"
#include "global_config.h"
#include "json_spirit.h"
#include "RLog.h"

using namespace std;
using namespace boost::filesystem;
using namespace boost::posix_time;
using namespace boost::gregorian;
using namespace boost;

void writelog(const char *str, const char* a) {
		FILE *fp;
		char s[20] = {0};
		struct tm *newtime;
		time_t aclock;
		time(&aclock);
		newtime = localtime(&aclock);
		strftime(s, sizeof(s), "%Y-%m-%d,%H:%M:%S", newtime);
		fp = fopen("/home/zhushanbo/zeegine/printer/debug.log", "a+");
		//fp = fopen("/home/pi/zeegine/printer/debug.log", "a+");
		if (fp == NULL) {
			perror("fopen failed");
			exit(1);
		}
		fwrite(s, strlen(s), 1, fp);
		fwrite(str, strlen(str), 1, fp);
		int ret = fwrite(a, strlen(a), 1, fp);
		if(ret <= 0) {
			perror("fwrite failed");
			exit(1);
		}
		fwrite("\n", 1, 1, fp);
		fclose(fp);
}

//一条PrinterResponse包含：id，time，text，type
PrinterResponse::PrinterResponse(const string& mes,uint32_t id,uint8_t tp):message(mes) {
    responseId = id;
    logtype = tp;
    time = boost::posix_time::microsec_clock::local_time();
}
//将PrinterResponse中的time书写成时间格式
std::string PrinterResponse::getTimeString() {
    tm tm = boost::posix_time::to_tm(time);
    char buf[40];
    sprintf(buf,"%2d:%02d:%02d",tm.tm_hour,tm.tm_min,tm.tm_sec);
    return string(buf);
}
//初始化Printer类
Printer::Printer(string conf) {
	confFile = conf;
    stopRequested = false;
    okAfterResend = true;
    sleepMs = 10;
    try {
        config.readFile(conf.c_str());
        bool ok = true;
        ok &= config.lookupValue("printer.name", name);
        ok &= config.lookupValue("printer.delta", delta);				//判断是否是delta机型
        ok &= config.lookupValue("printer.slugName", slugName);
        ok &= config.lookupValue("printer.tRatioK", tRatioK);		//时间修正系数
        ok &= config.lookupValue("printer.firmware", firmware);			//判断打印机固件类型：0:Repetier  1:Marlin
        ok &= config.lookupValue("printer.connection.device", device);
        ok &= config.lookupValue("printer.connection.baudrate", baudrate);
        ok &= config.lookupValue("printer.connection.pingPong", pingpong);
        ok &= config.lookupValue("printer.connection.readCacheSize", cacheSize);
        ok &= config.lookupValue("printer.connection.protocol", binaryProtocol);
        ok &= config.lookupValue("printer.connection.okAfterResend", okAfterResend);
        ok &= config.lookupValue("printer.dimension.xmin", xmin);
        ok &= config.lookupValue("printer.dimension.ymin", ymin);
        ok &= config.lookupValue("printer.dimension.zmin", zmin);
        ok &= config.lookupValue("printer.dimension.xmax", xmax);
        ok &= config.lookupValue("printer.dimension.ymax", ymax);
        ok &= config.lookupValue("printer.dimension.zmax", zmax);
        ok &= config.lookupValue("printer.homing.xhome", homex);
        ok &= config.lookupValue("printer.homing.yhome", homey);
        if (delta) 
        	ok &= config.lookupValue("printer.homing.zhome", homez);
        else 
        	homez = 0;
        ok &= config.lookupValue("printer.sleepMs", sleepMs);
        ok &= config.lookupValue("printer.extruder.count", extruderCount);
        ok &= config.lookupValue("printer.extruder.tempUpdateEvery",updateTempEvery);
        ok &= config.lookupValue("active", active);
        ok &= config.lookupValue("printer.speed.xaxis", speedx);
        ok &= config.lookupValue("printer.speed.yaxis", speedy);
        ok &= config.lookupValue("printer.speed.zaxis", speedz);
        ok &= config.lookupValue("printer.speed.eaxisExtrude", speedeExtrude);
        ok &= config.lookupValue("printer.speed.eaxisRetract", speedeRetract);
        if(!config.lookupValue("printer.extruder.heatedBed",hasHeatedBed))
            hasHeatedBed = true;
        if(!ok) {
            exit(4);
        }
        lastResponseId = 0;
        lastSDId = 0;
    	lastEPRId = 0;
        state = new PrinterState(this);
        serial = new PrinterSerial(*this);
        resendError = 0;
        errorsReceived = 0;
        linesSend = 0;
        bytesSend = 0;
        paused = false;
        running = false;
        filelist = false;
    }
    catch(libconfig::ParseException& pe) {
        exit(4);
    }
    catch(...) {
        exit(4);
    }
#ifdef DEBUG
    cout << "Printer configuration read: " << name << endl;
    cout << "Port:" << device << endl;
#endif
    jobManager = new PrintjobManager(gconfig->getStorageDirectory()+slugName+"/"+"jobs",this);
    modelManager = new PrintjobManager(gconfig->getStorageDirectory()+slugName+"/"+"models",this);
    scriptManager = new PrintjobManager(gconfig->getStorageDirectory()+slugName+"/"+"scripts",this,true);
}

//删除打印机
Printer::~Printer() {
    serial->close();
    delete state;
    delete serial;
    delete modelManager;
    delete jobManager;
    delete scriptManager;
}


//建立一个线程，用于执行Printer::run函数
void Printer::startThread() {
    assert(!thread);
    thread = boost::shared_ptr<boost::thread>(new boost::thread(boost::bind(&Printer::run, this)));
}


void Printer::updateLastTempMutex() {
    mutex::scoped_lock l(lastTempMutex);
    lastTemp = boost::posix_time::microsec_clock::local_time();
}

//打印机主循环
void Printer::run() {
    while (!stopRequested)
    {
        try {
            boost::this_thread::sleep(boost::posix_time::milliseconds(sleepMs)); //10ms
            if(!active) {
                if(serial->isConnected())
                    serial->close();
                boost::this_thread::sleep(boost::posix_time::milliseconds(1000));
                continue; // 如果没有激活，每1秒钟循环检测一次
            }
            if(!serial->isConnected()) {
                boost::this_thread::sleep(boost::posix_time::milliseconds(1000));
                serial->tryConnect(); //如果已激活，串口未打通，每秒尝试连接一次
            } else { //如果已激活且串口已经打通，每10ms进行依次循环
                    time_duration td;
                    {
                        mutex::scoped_lock l(lastTempMutex);
                        posix_time::ptime now = boost::posix_time::microsec_clock::local_time();
                        td = now-lastTemp;
                    } // Must close mutex to prevent deadlock!
                    if(manualCommands.size()<5 && updateTempEvery>0 && td.seconds()>=updateTempEvery) {
                        injectManualCommand("M105");
                        lastTemp = microsec_clock::local_time();
                    }
                jobManager->manageJobs(); 	//查看是否需要读取并填充打印命令（一次十条）
            }
            
            trySendNextLine();				//发送缓存的打印命令
        } catch( boost::thread_interrupted) {
            stopRequested = true;
        }
    }
}

//暂停打印任务
void Printer::PauseJob()
{
    paused = true;
    state->storePause();
    scriptManager->pushCompleteJobNoBlock("Pause",true);
    injectManualCommand("M83");
	injectManualCommand("G1 E-10");
	injectManualCommand("M82");
	injectManualCommand("G91");
	injectManualCommand("G1 Z10");
	injectManualCommand("G90");
	if(delta) injectManualCommand("G28");
	else injectManualCommand("G28 X0 Y0");
 }
 
 //继续打印任务
 void Printer::stopPause() {
 	//mutex::scoped_lock l(sendMutex);
	state->injectUnpause();
    paused = false;
}

//停止打印机线程
void Printer::stopThread() {
    thread->interrupt();
    thread->join();
#ifdef DEBUG
    cout << "Thread for printer " << name << " finished" << endl;
#endif
}

//关闭当前连接时Printer类执行的操作
void Printer::connectionClosed() {
    jobManager->StoreCurrentJob();//by zeegine
    mutex::scoped_lock l(sendMutex);
    manualCommands.clear();
    jobCommands.clear();
}
//自动计算打印时间的修正因子
void Printer::adjustTimeRatio(int realTime,int jobEtime,int speedCont){
	
	if(jobEtime>realTime&&jobEtime<(1.5*realTime)){
		rRatioK = 100*((realTime/jobEtime)-1)*jobEtime/speedCont; //  % 大约10左右
		tRatioK = 0.7*tRatioK+0.3*rRatioK;
		try{
			config.readFile(confFile.c_str());
			libconfig::Setting &RatioK = config.lookup("printer.tRatioK");
			RatioK = tRatioK;
			config.writeFile(confFile.c_str());
		}catch(libconfig::ParseException& pe) {
	    exit(4);
	    }
	}
}
//将某一条消息msg添加到Response列表
void Printer::addResponse(const std::string& msg,uint8_t rtype) {
    mutex::scoped_lock l(responseMutex);
    shared_ptr<PrinterResponse> newres(new PrinterResponse(msg,++lastResponseId,rtype));
    responses.push_back(newres);
    if(responses.size()>(size_t)gconfig->getBacklogSize())
        responses.pop_front();
}
//将注释“;xxx”和空指令剔除
bool Printer::shouldInjectCommand(const std::string& cmd) {
    if(cmd=="@kill") {
        serial->resetPrinter();
        return false;
    }
    if(cmd.length()<2) return false; // Don't waste time with empty lines
    size_t p = cmd.find(';');
    if(p<2) return false;
    return true;
}
 //将手动命令推到命令列表
void Printer::injectManualCommand(const std::string& cmd) {
    if(!shouldInjectCommand(cmd)) return;
    {
        mutex::scoped_lock l(sendMutex);
        manualCommands.push_back(cmd);
    } // need parantheses to prevent deadlock with trySendNextLine
    trySendNextLine(); // Check if we need to send the command immediately
}

//将任务命令完全推入命令列表（不做判断），在发送时做判断
void Printer::injectJobCommand(const std::string& cmd) 
	{
    //if(!shouldInjectCommand(cmd)) return;
    mutex::scoped_lock l(sendMutex);
    //size_t ll = strlen(cmd.c_str());
    jobCommands.push_back(cmd);
    //jobManager->getRunningJob()->storedByte += ll;
    // No need to trigger job commands early. There will most probably follow more very soon
    // and the job should already run.
}

//手动移动某个长度
void Printer::move(double x,double y,double z,double e) {
    if(x!=0)
        injectManualCommand(state->getMoveXCmd(x, speedx*60.0));
    if(y!=0)
        injectManualCommand(state->getMoveYCmd(y, speedy*60.0));
    if(z!=0)
        injectManualCommand(state->getMoveZCmd(z, speedz*60.0));
    if(e!=0)
        injectManualCommand(state->getMoveECmd(e,60.0 * (e>0 ? speedeExtrude : speedeRetract)));
}

//查询当前jobCommand条数
size_t Printer::jobCommandsStored() {
    mutex::scoped_lock l(sendMutex);
    return jobCommands.size();
}

//用于返回某一ID之后新的Response
boost::shared_ptr<list<boost::shared_ptr<PrinterResponse> > > Printer::getResponsesSince(uint32_t resId,uint8_t filter,uint32_t &lastid) {
    lastid = resId;
    shared_ptr<list<shared_ptr<PrinterResponse> > > retList(new list<shared_ptr<PrinterResponse> >());
    mutex::scoped_lock l(responseMutex);
    deque<shared_ptr<PrinterResponse> >::iterator act = responses.begin();
    deque<shared_ptr<PrinterResponse> >::iterator e = responses.end();
    while(act!=e) {
        if((*act)->responseId>resId && ((*act)->logtype & filter)!=0) {
            retList->push_back(*act);
            lastid = (*act)->responseId;
        }
        act++;
    }
    return retList;
}
//关闭连接
void Printer::close() {
    serial->close();
}
//重新发送某一条命令
void Printer::resendLine(size_t line)
{
    {
        mutex::scoped_lock l(sendMutex);
        ignoreNextOk = okAfterResend;
        resendError++;
        errorsReceived++;
        if(!pingpong && errorsReceived==3 && cacheSize>63) {
            cacheSize = 63;
        }
        if (pingpong)
            readyForNextSend = true;
        else  {
            nackLines.clear();
            receiveCacheFill = 0;
        }
        if (resendError > 8) {
            string msg = "Receiving only error messages. Reset communication.";
            string url = "/printer/msg/"+slugName+"?a=close";
            gconfig->createMessage(msg, url);
            serial->resetPrinter();
        }
        line &=65535;
        resendLines.clear();
        bool addLines = false;
        for(deque<shared_ptr<GCode> >::iterator it=history.begin();it!=history.end();++it) {
            GCode &gc = **it;
            if (gc.hasN() && (gc.getN() & 65535) == line)
                addLines = true;
            if(addLines) resendLines.push_back(*it);
        }
        if (binaryProtocol != 0) {
            boost::this_thread::sleep(boost::posix_time::milliseconds(320000/baudrate));
            uint8_t  buf[32];
            for (int i = 0; i < 32; i++) buf[i] = 0;
            serial->writeBytes(buf,32);
            boost::this_thread::sleep(boost::posix_time::milliseconds(320000/baudrate));
        } else {
            boost::this_thread::sleep(boost::posix_time::milliseconds(cacheSize*10000/baudrate)); // Wait for buffer to empty
        }
    } // unlock mutex or we get deadlock!
    trySendNextLine();
}
// manageHOstCmmands is called with sendMutex locked!
void Printer::manageHostCommand(boost::shared_ptr<GCode> &cmd) {
    string c = cmd->hostCommandPart();
    if(c=="@pause") {
        string msg= "Printer "+name+" paused:"+cmd->hostParameter();
        string answer = "/printer/msg/"+slugName+"?a=unpause";
        gconfig->createMessage(msg,answer);
        paused = true;
        state->storePause();
        scriptManager->pushCompleteJobNoBlock("Pause",true);
    } else if(c=="@isathome") {
        state->setIsathome();
    } else if(c=="@kill") {
        serial->resetPrinter();
    }
}
//dp将发送序号，checksum编入：Nxx G1 ... *xx
bool Printer::trySendPacket(boost::shared_ptr<GCodeDataPacket> &dp,shared_ptr<GCode> &gc) {
    if((pingpong && readyForNextSend) || (!pingpong && cacheSize>receiveCacheFill+dp->length)) {
        serial->writeBytes(dp->data,dp->length);
        if(!pingpong) {
            receiveCacheFill += dp->length;
            nackLines.push_back(dp->length);
        } else readyForNextSend = false;
        history.push_back(gc);
        if(history.size()>MAX_HISTORY_SIZE)
            history.pop_front();
        lastCommandSend = boost::posix_time::microsec_clock::local_time();
        bytesSend+=dp->length;
        linesSend++;
        addResponse(gc->getOriginal(), 1);
        return true;
    }
    return false;
}

//提取出一个字符后面的数字，直到句尾或者空格处
bool Printer::extract(const string& source,const string& ident,string &result)
{
    size_t pos = 0; //source.find(ident);
    size_t len = source.length();
    do
    {
        if(pos>0) pos++;
        pos = source.find(ident, pos);
        if (pos == string::npos) return false;
        if(pos==0) break;
    } while (source[pos-1]!=' ');
    size_t start = pos + ident.length();
    while(start<len && source[pos]==' ') start++;
    size_t end = start;
    while (end < len && source[end] != ' ') end++;
    result = source.substr(start,end-start);
    return true;
}

//清除jobCommond和manualCommand里面存储的命令，用于中断连接时的操作
void Printer::clearCommands(){
	jobCommands.clear();
	manualCommands.clear();
}
//发送下一条指令（优先级：重发>手动>任务）
void Printer::trySendNextLine() {
    if (!garbageCleared) return;
    mutex::scoped_lock l(sendMutex);
    if (pingpong && !readyForNextSend) {return;}
    if (!serial->isConnected()) {return;} // Not ready yet
    shared_ptr<GCode> gc;
    GCodeDataPacketPtr dp;
    //首先处理未发送成功的命令
    if (resendLines.size()>0) {
        gc = resendLines.front();
        if (binaryProtocol == 0 || gc->forceASCII)
            dp = gc->getAscii(true,true,false);
        else
            dp = gc->getBinary();
        if(trySendPacket(dp,gc))
        {
            //[rhlog addText:[@"Resend: " stringByAppendingString:[gc getAsciiWithLine:YES withChecksum:YES]]];
        }
        return;
    }
    if (resendError > 0) resendError--; // Drop error counter
                                        // then check for manual commands
    if (manualCommands.size() > 0)  {
        gc = shared_ptr<GCode>(new GCode(*this,manualCommands.front()));
        if (gc->hostCommand)
        {
            manageHostCommand(gc);
            manualCommands.pop_front();
            return;
        }
        if(gc->m!=117)
            gc->setN(state->increaseLastline());
        if (binaryProtocol == 0 || gc->forceASCII)
            dp = gc->getAscii(true,true,false);
        else
            dp = gc->getBinary();
        if(trySendPacket(dp,gc)) {
        	//RLog::log("manual line : @",(std::string) manualCommands.front());
            manualCommands.pop_front();
            state->analyze(*gc , false);
        } else if(gc->hasN() && !(gc->hasM() && gc->getM()==110)) state->decreaseLastline();
        return;
    }
    // do we have a printing job?
	if (jobCommands.size()>0 && !paused)  {
		size_t l = strlen(jobCommands.front().c_str());//首先计算即将发送的命令的字节数，用于处理中断任务时文件指针定位
		if(!shouldInjectCommand(static_cast<string>(jobCommands.front().c_str()))) {
			jobCommands.pop_front();
			
			if(jobManager->getRunningJob().get()) jobManager->getRunningJob()->Rpos += l;//每个pop_front之后计数累加
			return;
		}
		//printf("1:%s\n", jobCommands.front().c_str());
        gc = shared_ptr<GCode>(new GCode(*this,jobCommands.front()));
        //printf("2:%s\n", jobCommands.front().c_str());
        if (gc->hostCommand)
        {
            manageHostCommand(gc);
            jobCommands.pop_front();
            if(jobManager->getRunningJob().get()) jobManager->getRunningJob()->Rpos += l;//每个pop_front之后计数累加
            return;
        }
        
        if(gc->m!=117)
            gc->setN(state->increaseLastline());
        if (binaryProtocol == 0 || gc->forceASCII){
            dp = gc->getAscii(true,true,true);
            //RLog::log("isAscii");
        }else
            dp = gc->getBinary();
        if(trySendPacket(dp,gc)) {
        	//RLog::log("send line : @",(std::string) jobCommands.front());
            jobCommands.pop_front();
            if(jobManager->getRunningJob().get()) jobManager->getRunningJob()->Rpos += l;//每个pop_front之后计数累加
            state->analyze(*gc , true);
        } else if(gc->hasN() && !(gc->hasM() && gc->getM()==110)) state->decreaseLastline();
        return;
    }
}

//分析每条收到消息的内容
void Printer::analyseResponse(string &res) {
#ifdef DEBUG
    //   cout << "Response:" << res << endl;
#endif
    //uint8_t rtype = 4;
    size_t Tpos = res.find("T:");
    if (Tpos!=string::npos)  realOnline = true; //接收到温度，判定真正完成连接
    uint8_t rtype;
    if (res.find("Begin file list") == 0)
       filelist = true; 
    else if(res.find("End file list") == 0){
       filelist = false; 
       SDendflag = true;
    }
    if (filelist) rtype = 16;
    else rtype = 4;
    while(res.length()>0 && res[0]<32)
        res = res.substr(1);
    string h;
    // 解析打印机状态信息
    state->analyseResponse(res,rtype); 
    // 接收到start表明打印机reset了
    size_t fpos = res.find("start");
    if (fpos==0 ||
        (garbageCleared==false && fpos!=string::npos))
    {
        {
            mutex::scoped_lock l(sendMutex);
            state->reset();//重置打印机的所有状态,lastLine = 0,与M110 N0对应
            history.clear();//将发送历史记录清空
            readyForNextSend = true;
            nackLines.clear();//将未识别的消息清空
            receiveCacheFill = 0;
            garbageCleared = true;
            manualCommands.clear();//将手动命令清空
            //jobManager->undoCurrentJob();
        }
        injectManualCommand("M110 N0");//将发送的消息数设为0
        injectManualCommand("M115");
        injectManualCommand("M114");//获取最新打印机位置状态
   }
    if (extract(res,"Resend:",h))
    {
        size_t line = atoi(h.c_str());
        resendLine(line);			//如果收到resend请求，执行resend
    }
    else if (res.find("ok")==0)
    {
    	SDendflag = false;
    	EPRflag = false;
    	filelist = false;
        rtype = 2;
        garbageCleared = true;
        //if(Main.main.logView.toolACK.Checked)
        //    log(res, true, level);
        if (!ignoreNextOk)  // ok in response of resend?
        {
            if (pingpong) readyForNextSend = true;
            else
            {
                mutex::scoped_lock l(sendMutex);
                if (nackLines.size() > 0) {
                    receiveCacheFill-= nackLines.front();
                    nackLines.pop_front();
                }
            }
            resendError = 0;
            trySendNextLine();
        } else
            ignoreNextOk = false;
    }
    else if (res=="wait") 
    {
        rtype = 2;
        mutex::scoped_lock l(sendMutex);
        boost::posix_time::ptime now = boost::posix_time::microsec_clock::local_time();
        time_duration td(now-lastCommandSend);
        if(td.total_seconds()>5) {
            if (pingpong) readyForNextSend = true;
            else
            {
                nackLines.clear();
                receiveCacheFill = 0;
            }
        }
        resendError = 0;
    }
    addResponse(res,rtype);
    trySendNextLine();
}
//获取打印机在线状态
int Printer::getOnlineStatus() {
    if(serial->isConnected() && realOnline) return 1;//需要收到T: 
    //if(serial->isConnected()) return 1;
    return 0;
}
//获取打印机激活状态
bool Printer::getActive() {
    return active;
}
//更改打印机激活状态
void Printer::setActive(bool v) {
    active = v;
}
//获取运行中打印任务的状态，名字，进度
void Printer::getJobStatus(json_spirit::Object &obj) {
    jobManager->getJobStatus(obj);
}
//填充推送信息
void Printer::fillJSONObject(json_spirit::Object &obj) {
    using namespace json_spirit;
    obj.push_back(Pair("active",active));
    obj.push_back(Pair("paused",paused));
    obj.push_back(Pair("printerName",name));
    obj.push_back(Pair("slug",slugName));
    obj.push_back(Pair("device",device));
    obj.push_back(Pair("baudrate",baudrate));
    obj.push_back(Pair("xmin",xmin));
    obj.push_back(Pair("xmax",xmax));
    obj.push_back(Pair("ymin",ymin));
    obj.push_back(Pair("ymax",ymax));
    obj.push_back(Pair("zmin",zmin));
    obj.push_back(Pair("zmax",zmax));
    obj.push_back(Pair("speedx",speedx));
    obj.push_back(Pair("speedy",speedy));
    obj.push_back(Pair("speedz",speedz));
    obj.push_back(Pair("speedeExtrude",speedeExtrude));
    obj.push_back(Pair("speedeRetract",speedeRetract));
    obj.push_back(Pair("extruderCount",extruderCount));
    obj.push_back(Pair("hasHeatedBed",hasHeatedBed));
    Array ea;
    for(int i=0;i<extruderCount;i++) {
        Object e;
        e.push_back(Pair("extruderid",i));
        e.push_back(Pair("extrudernum",i+1));
        ea.push_back(e);
    }
    obj.push_back(Pair("extruder",ea));
}
