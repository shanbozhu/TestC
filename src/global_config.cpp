/*=====================================================
# Author:        Zeegine Team
# Copyright:     www.thu3d.com        
# Last modified: 2015-03-17 14:19:35 
# Filename:		 global_config.cpp
# Description:   
=====================================================*/

#include "global_config.h"
#include <boost/filesystem.hpp>

using namespace std;
using namespace boost::filesystem;


GlobalConfig *gconfig;
//创建一个gconfig，并从config文件中读取设置
GlobalConfig::GlobalConfig(string filename) {
    daemon = false;
    msgCounter = 0;
	try {
		config.readFile(filename.c_str());
	} catch(libconfig::ParseException &pe) {
		cerr << "error: " << pe.getError() << " line:" << pe.getLine() << " file:" << pe.getFile() << endl;
		exit(-1);
	}
    bool ok = true;
    ok &= config.lookupValue("printer_config_directory",printerConfigDir);
    ok &= config.lookupValue("data_storage_directory",storageDir);
    ok &= config.lookupValue("website_directory", wwwDir);
    ok &= config.lookupValue("languages_directory", languageDir);
    ok &= config.lookupValue("default_language", defaultLanguage);
    ok &= config.lookupValue("ports",ports);
    backlogSize = 1000;
    config.lookupValue("backlogSize", backlogSize);
    if(!ok) {
        cerr << "error: Global configuration is missing options!" << endl;
        exit(3);
    }
#ifdef DEBUG
    cout << "Global configuration:" << endl;
    cout << "Web directory: " << wwwDir << endl;
    cout << "Printer config directory: " << printerConfigDir << endl;
    cout << "Storage directory: " << storageDir << endl;
#endif
}
//通过读取打印机config，新建打印机
void GlobalConfig::readPrinterConfigs() {
    printers.clear();
    if ( !exists( printerConfigDir ) ) return;
    directory_iterator end_itr; // default construction yields past-the-end
    for ( directory_iterator itr( printerConfigDir );itr != end_itr;++itr )
    {
        if ( is_regular(itr->status()) )
        {
        	//重启server清空job目录
        	//std::system("rm -rf /var/lib/ZeegineServer/storage/prusa_i3_1/jobs/*");
            cout << "Printer config: " << itr->path() << endl;
            Printer *p = new Printer(itr->path().string());
            printers.push_back(p);
        }      
    }
}
//为打印机开启一个线程
void GlobalConfig::startPrinterThreads() {
    vector<Printer*>::iterator pi;
    for(pi=printers.begin();pi!=printers.end();pi++) {
        (*pi)->startThread();
    }
}
//停止打印机线程
void GlobalConfig::stopPrinterThreads() {
    vector<Printer*>::iterator pi;
    for(pi=printers.begin();pi!=printers.end();pi++) {
        (*pi)->stopThread();
    }
}
//通过slug找到一台打印机
Printer *GlobalConfig::findPrinterSlug(const std::string& slug) {
    for(vector<Printer*>::iterator it=printers.begin();it!=printers.end();it++) {
        Printer *p = *it;
        if(p->slugName == slug) return p;
    }
    return NULL;
}
//组合json Messages数据
void GlobalConfig::fillJSONMessages(json_spirit::Array &arr) {
    mutex::scoped_lock l(msgMutex);
    list<ZeegineMsgPtr>::iterator it = msgList.begin(),ie = msgList.end(); //it二重指针
    for(;it!=ie;++it) {
        using namespace json_spirit;
        Object obj;
        obj.push_back(Pair("id",(*it)->mesgId));
        obj.push_back(Pair("msg",(*it)->message));
        obj.push_back(Pair("link",(*it)->finishLink));
        arr.push_back(obj);
    }
}
//创建一条message属性的推送
void GlobalConfig::createMessage(std::string &msg,std::string &link) {
    mutex::scoped_lock l(msgMutex);
    ZeegineMsgPtr p(new ZeegineMessage());
    p->mesgId = ++msgCounter;
    p->message = msg;
    p->finishLink = link+"&id="+intToString(p->mesgId);
    msgList.push_back(p);
}
//删除message
void GlobalConfig::removeMessage(int id) {
    mutex::scoped_lock l(msgMutex);
    list<ZeegineMsgPtr>::iterator it = msgList.begin(),ie = msgList.end(); //it二重指针
    for(;it!=ie;++it) {
        if((*it)->mesgId == id) {
            msgList.remove(*it);
            break;
        }
    }
}
//int转换为string
std::string intToString(int number) {
    stringstream s;
    s << number;
    return s.str();
}
