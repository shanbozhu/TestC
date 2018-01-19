
#ifndef __RLog__
#define __RLog__

#include <iostream>

class RLog {
public:
    RLog();
    ~RLog();
    static void log(const std::string &line,bool err=false);
    static void log(const std::string &line,int val,bool err=false);
    static void log(const std::string &line,double val,bool err=false);
    static void log(const std::string &line,const std::string& val,bool err=false);
};
extern RLog rlog;

#endif
