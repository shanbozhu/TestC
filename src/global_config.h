/*=====================================================
# Author:        Zeegine Team
# Copyright:     www.thu3d.com        
# Last modified: 2015-03-17 14:19:27 
# Filename:		 global_config.h
# Description:   
=====================================================*/


#ifndef __global_config__
#define __global_config__

#define Zeegine_SERVER_VERSION "0.24"

#include <iostream>
#include <vector>
#include <list>

#include "printer.h"
#include "GCode.h"
#include "Printjob.h"
#include "RLog.h"

//ZeegineMessage类
class ZeegineMessage {
public:
    std::string message; 
    std::string finishLink; //remove the message
    int mesgId;  
};

//typedef ZeegineMessage* ZeegineMsgPtr;
typedef shared_ptr<ZeegineMessage> ZeegineMsgPtr;

class GlobalConfig {
	Printer *printer;
    libconfig::Config config;
    std::string wwwDir; 
    std::string printerConfigDir; 
    std::string storageDir; 
    std::string languageDir; 
    std::string ports; 
    std::string defaultLanguage;
    std::vector<Printer*> printers;
    int backlogSize;
    mutex msgMutex; ///< Mutex for thread safety of message system.
    int msgCounter; ///< Last used message id.
    std::list<ZeegineMsgPtr> msgList; ///< List with active messages.
public:
    bool daemon;
    inline const std::string& getWebsiteRoot() {return wwwDir;}
    inline const std::string& getPrinterConfigDir() {return printerConfigDir;}
    inline const std::string& getStorageDirectory() {return storageDir;}
    inline const int getBacklogSize() {return backlogSize;}
    inline const std::string& getPorts() {return ports;}
    inline const std::string& getLanguageDir() {return languageDir;}
    inline const std::string& getDefaultLanguage() {return defaultLanguage;}
    
    GlobalConfig(std::string filename);
    void readPrinterConfigs();
    void startPrinterThreads();
    void stopPrinterThreads();
   
    Printer *findPrinterSlug(const std::string& slug);
    inline std::vector<Printer*> &getPrinterList() {return printers;}
    
    void fillJSONMessages(json_spirit::Array &obj);
    void createMessage(std::string &msg,std::string &link);
    void removeMessage(int id);
};
extern GlobalConfig *gconfig;
std::string intToString(int number);
#endif 
