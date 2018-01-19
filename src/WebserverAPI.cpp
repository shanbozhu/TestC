/*=====================================================
# Author:        Zeegine Team
# Copyright:     www.thu3d.com        
# Last modified: 2015-03-17 14:16:37 
# Filename:		 WebserverAPI.cpp
# Description:   
=====================================================*/

#include <map> 
#include <set>
#include <boost/bind.hpp>
#include <pwd.h>
#include <fstream>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/filesystem.hpp>
#include <boost/thread.hpp>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <vector>
#include <boost/shared_ptr.hpp>

#include "moFileReader.h"
#include "mongoose.h"
#include "WebserverAPI.h"
#include "global_config.h"
#include "printer.h"
#include "json_spirit.h"
#include "PrinterState.h"
#include "Printjob.h"
#include "RLog.h"
#include "timeEstimate.h"

#if defined(_WIN32)
#include <io.h>
#endif

using namespace std;
using namespace json_spirit;
using namespace boost;
using namespace boost::filesystem;
typedef list<shared_ptr<Printjob> > pjlist;

#if defined(_WIN32) && !defined(__SYMBIAN32__) // Windows specific
#pragma warning( disable : 4996 )
// Visual Studio 6 does not know __func__ or __FUNCTION__
// The rest of MS compilers use __FUNCTION__, not C99 __func__
// Also use _strtoui64 on modern M$ compilers
#if defined(_MSC_VER) && _MSC_VER < 1300
#define STRX(x) #x
#define STR(x) STRX(x)
#define __func__ "line " STR(__LINE__)
#define strtoull(x, y, z) strtoul(x, y, z)
#define strtoll(x, y, z) strtol(x, y, z)
#else
#define __func__  __FUNCTION__
#define strtoull(x, y, z) _strtoui64(x, y, z)
#define strtoll(x, y, z) _strtoi64(x, y, z)
#endif // _MSC_VER

#define O_EXLOCK 0
#define O_CLOEXEC 0
#define O_CREAT _O_CREAT
#define O_APPEND _O_APPEND
#define O_TRUNC _O_TRUNC
#define O_RDWR _O_RDWR
#define snprintf _snprintf
#define strnstr _strnstr
#define popen(x, y) _popen(x, y)
#define pclose(x) _pclose(x)
#define myclose(x) _close(x)
#define myopen(x,y,z) _open(x,y,z)
#define fseeko(x, y, z) _lseeki64(_fileno(x), (y), (z))
#define fdopen(x, y) _fdopen((x), (y))
#define mywrite(x, y) _write((x), (y))
//#define read(x, y, z) _read((x), (y), (unsigned) z)
#else
#define myclose(x) close(x)
#define myopen(x,y,z) open(x,y,z)
#define mywrite(x, y) write((x), (y))
#endif
#ifndef O_EXLOCK
#define O_EXLOCK 0
#endif

namespace zeegine {
    static const char *HTTP_500 = "HTTP/1.0 500 Server Error\r\n\r\n";
    
	char *mystrnstr(const char *s,const char *needle,int len) {
		int ln = (int)strlen(needle);
		if(!ln) return (char*)s;
		char *s2 = (char *)s;
		for(int o=0;o<len-ln;o++) {
			s2 = (char*)&s[o];
			bool ok = true;
			for(int j=0;j<ln && ok;j++) {
				ok = s2[j]==needle[j];
			}
			if(ok) return s2;
		}
		return NULL;
	}

    // Modified verion from mongoose examples
    //上传文件操作
    bool handleFileUpload(struct mg_connection *conn,const string& filename,string& name,long &size,bool append,PrintjobPtr job) {
        name.clear();
        const char *cl_header;
        char post_data[16 * 1024],  file_name[1024], mime_type[100],boundary[100],
        buf[BUFSIZ*2], *eop, *s, *p;
        // char path[999];
        FILE *fp;
        long long int cl, written;
        int fd, n, post_data_len;
        
        // Figure out total content length. Return if it is not present or invalid.
        cl_header = mg_get_header(conn, "Content-Length");
        if (cl_header == NULL || (cl = strtoll(cl_header, NULL, 10)) <= 0) {
            mg_printf(conn, "%s%s", HTTP_500, "Invalid Content-Length");
            return false;
        }
        
        // Read the initial chunk into memory. This should be multipart POST data.
        // Parse headers, where we should find file name and content-type.
        post_data_len = mg_read(conn, post_data, sizeof(post_data));
        file_name[0] = mime_type[0] = '\0';
        for (s = p = post_data; p < &post_data[post_data_len]; p++) {
            if (p[0] == '\r' && p[1] == '\n') {
                if (s == p && *file_name) {
                    p += 2;
                    break;  // End of headers
                }
                p[0] = p[1] = '\0';
                sscanf(s, "Content-Type: %99s", mime_type);
                // (lsm): don't expect filename to be the 3rd field,
                // parse the header properly instead.
                sscanf(s, "Content-Disposition: %*s %*s filename=\"%1023[^\"]",
                       file_name);
                s = p + 2;
            }
        }
        if(strlen(post_data)>100) {
            mg_printf(conn, "%s%s", HTTP_500, "Boundary too long");
            return false;
        }
        boundary[0] = '\r';
        boundary[1] = '\n';
        strcpy(&boundary[2],post_data);
        // Finished parsing headers. Now "p" points to the first byte of data.
        // Calculate file size
        cl -= p - post_data;      // Subtract headers size
                                  // cl -= strlen(post_data);  // Subtract the boundary marker at the end
                                  //   cl -= 6;                  // Subtract "\r\n" before and after boundary
        
        // Construct destination file name. Write to /tmp, do not allow
        // paths that contain slashes.
        /*if ((s = strrchr(file_name, '/')) == NULL) {
            s = file_name;
        }
        snprintf(path, sizeof(path), "/tmp/%s", s);*/
        
        if (file_name[0] == '\0') {
            mg_printf(conn, "%s%s", HTTP_500, "Can't get file name");
        } else if (cl <= 0) {
            mg_printf(conn, "%s%s", HTTP_500, "Empty file");
        } else if ((fd = myopen(filename.c_str(), O_CREAT | (append ? O_APPEND : O_TRUNC) |
                              O_RDWR /*| O_WRONLY | O_EXLOCK | O_CLOEXEC*/,0666)) < 0) {
            // We're opening the file with exclusive lock held. This guarantee us that
            // there is no other thread can save into the same file simultaneously.
            mg_printf(conn, "%s%s", HTTP_500, "Cannot open file");
        } else if ((fp = fdopen(fd,(append ? "a+" : "w"))) == NULL) {
            mg_printf(conn, "%s%s", HTTP_500, "Cannot reopen file stream");
            myclose(fd);
        } else {
            bool finished = false;
            name = file_name;
            int boundlen = (int)strlen(boundary);
            // Success. Write data into the file.
            eop = post_data + post_data_len;
            n = p + cl > eop ? (int) (eop - p) : (int) cl;
            char *p2 = mystrnstr(p,boundary,n);
            size_t startnew = 0;
            if(p2!=NULL) { // End boundary detected
                finished = true;
                n = (int)(p2-p);
            } else if(n>boundlen) {
                n-=boundlen;
                startnew = boundlen;
            }
            (void) fwrite(p, 1, n, fp);
            written = n;
            if(!finished)
                memcpy(buf,&p[n],boundlen);
            while (!finished && written < cl &&
                   (n = mg_read(conn, &buf[startnew],(size_t)( cl - written > sizeof(buf)-startnew ?
                                sizeof(buf)-startnew : cl - written))) > 0) {
                n+=startnew;
                p2 = mystrnstr(buf,boundary,n);
                startnew = 0;
                job->uploadPos = written;
                if(p2!=NULL) { // End boundary detected
                    finished = true;
                    n = (int)(p2-buf);
                } else if(n>boundlen) {
                    n-=boundlen;
                    startnew = boundlen;
                }

                (void) fwrite(buf, 1, n, fp);
                written += n;
                if(!finished)
                    memcpy(buf,&buf[n],boundlen);
            }
            (void) fclose(fp);
            size = (long)written;
            return true;
        }
        return false;
    }
    void listPrinter(Object &ret) {
        Array parr;
        std::vector<Printer*> *list = &gconfig->getPrinterList();
        for(vector<Printer*>::iterator i=list->begin();i!=list->end();++i) {
            Object pinfo;
            Printer *p = *i;
            pinfo.push_back(Pair("name",p->name));
            pinfo.push_back(Pair("slug",p->slugName));
            pinfo.push_back(Pair("online",(p->getOnlineStatus())));
            p->getJobStatus(pinfo);
            pinfo.push_back(Pair("active",p->getActive()));
            parr.push_back(pinfo);
        }
        ret.push_back(Pair("data",parr));
        Array msg;
        gconfig->fillJSONMessages(msg);
        ret.push_back(Pair("messages",msg));
    }
    
    
    /*
     * 处理网页请求
     */
    void HandleWebrequest(struct mg_connection *conn) {
        mg_printf(conn, "HTTP/1.0 200 OK\r\n"
                  "Cache-Control:public, max-age=0\r\n"
                  "Server: Zeegine-Server\r\n"
                  "Content-Type: text/html; charset=utf-8\r\n\r\n");
        const struct mg_request_info *ri = mg_get_request_info(conn);
        // First check the path for its parts
        int start(9); // /printer/
        int end(9);
        string error;
        Printer *printer = NULL;
        const char* uri = ri->uri;
        while(uri[end] && uri[end]!='/') end++;
        string cmdgroup(&uri[start],end-start);
        if(uri[end]) { // Read printer string
            start = end = end+1;
            while(uri[end] && uri[end]!='/') end++;
            printer = gconfig->findPrinterSlug(string(&uri[start],end-start));        
        }
        Object ret;
        if(cmdgroup=="list") {
            listPrinter(ret);
        } else if(printer==NULL) {
            error = "Unknown printer";
        } else if(cmdgroup=="job") {
        //////////////////////////////////////任务相关操作////////////////////////////////////////////////
            string a;
            MG_getVar(ri,"a",a);
            if(a=="list") {
                printer->getJobManager()->fillSJONObject("data",ret);
                Array msg;
                gconfig->fillJSONMessages(msg);
                ret.push_back(Pair("messages",msg));
            } /*else if(a=="upload") {
#ifdef DEBUG
                cout << "Upload job" << endl;
#endif
                string name,jobname;
                long size;
                MG_getVar(ri,"name", jobname);
                PrintjobPtr job = printer->getJobManager()->createNewPrintjob(jobname);
                handleFileUpload(conn,job->getFilename(), name,size,false,job);
                printer->getJobManager()->finishPrintjobCreation(job,name,size);
				job->isJob = true;
                printer->getJobManager()->fillSJONObject("data",ret);
#ifdef DEBUG
                cout << "Name:" << name << " Size:" << size << endl;
#endif
            } else if(a=="remove") {
                string sid;
                if(MG_getVar(ri,"id",sid)) {
                    int id = atoi(sid.c_str());
                    PrintjobPtr job = printer->getJobManager()->findById(id);
                    if(job.get())
                        printer->getJobManager()->RemovePrintjob(job);
                }
                printer->getJobManager()->fillSJONObject("data",ret);
            } else if(a=="start") {
                string sid;
                if(MG_getVar(ri,"id",sid)) {
                    int id = atoi(sid.c_str());
                    PrintjobPtr job = printer->getJobManager()->findById(id);
                    if(job.get()) {
                        printer->getJobManager()->startJob(id);
                    }
                }
                printer->getJobManager()->fillSJONObject("data",ret);
            }*/ 
            else if(a=="stop") {
            	//停止任务
                string sid;
                if(MG_getVar(ri,"id",sid)) {
                    int id = atoi(sid.c_str());
                    PrintjobPtr job = printer->getJobManager()->findById(id);
                    if(job.get()) {
                        printer->getJobManager()->killJob(id);
                        printer->getScriptManager()->pushCompleteJob("Kill");
                    }
                }
                printer->getJobManager()->fillSJONObject("data",ret);                
            } else if(a=="pause" && printer->running) {
                //暂停任务
                if(!printer->paused) printer->PauseJob();
                printer->getJobManager()->fillSJONObject("data",ret);
			} else if(a=="continue" && printer->running) {
                //继续任务
                if(printer->paused) printer->stopPause();
                printer->getJobManager()->fillSJONObject("data",ret);
            } else if(a=="restore" && !printer->running && printer->getOnlineStatus()) {
				//恢复意外中断的任务
                printer->getJobManager()->RecoverCurrentJob();
                printer->getJobManager()->fillSJONObject("data",ret);
            } else if(a=="cleanBreak") {
                //清除意外中断的任务
                printer->getJobManager()->CleanBreakJob();
                printer->getJobManager()->fillSJONObject("data",ret);

            }   
        } else if(cmdgroup=="setting") {
        ////////////////////////////////////////打印机设置////////////////////////////////////////
        	string a;
            MG_getVar(ri,"a",a);
            if(a == "delta"){
            //设置机型为delta
            	printer->delta = true;
            	try{
		    		libconfig::Config pConfig;
		    		std::string pConfigFile;
		    		pConfigFile = gconfig->getPrinterConfigDir() + "/prusai3_1.cfg";
		    		pConfig.readFile(pConfigFile.c_str());
					
					libconfig::Setting &Mdelta       =    pConfig.lookup("printer.delta");
					
					Mdelta = true;
					pConfig.writeFile(pConfigFile.c_str());
				}
				catch(libconfig::ParseException &pe) {
					cerr << "error: " << pe.getError() << " line:" << pe.getLine() << " file:" << pe.getFile() << endl;
					exit(-1);
				}
            	if(printer->firmware == Repetier_FirmWare) printer->injectManualCommand("M205");
            	else if(printer->firmware == Marlin_FirmWare) printer->injectManualCommand("M501");
            }else if(a == "cartesian"){
            //设置机型为方形
            	printer->delta = false;
            	printer->homez = 0;
            	try{
		    		libconfig::Config pConfig;
		    		std::string pConfigFile;
		    		pConfigFile = gconfig->getPrinterConfigDir() + "/prusai3_1.cfg";
		    		pConfig.readFile(pConfigFile.c_str());
					libconfig::Setting &zhome       =    pConfig.lookup("printer.homing.zhome");
					libconfig::Setting &Mdelta       =    pConfig.lookup("printer.delta");
					zhome = printer->homez;
					Mdelta = false;
					pConfig.writeFile(pConfigFile.c_str());
				}
				catch(libconfig::ParseException &pe) {
					cerr << "error: " << pe.getError() << " line:" << pe.getLine() << " file:" << pe.getFile() << endl;
					exit(-1);
				}
            }else if(a == "marlin")
            {
            //设置固件为marlin
            	printer->firmware = Marlin_FirmWare;
            }else if(a == "repetier")
            {
            //设置固件为repetier
            	printer->firmware = Repetier_FirmWare;
            }
         }else if(cmdgroup=="function") {
        /////////////////////////////////////打印机特殊功能///////////////////////////////////////
        	string a;
            MG_getVar(ri,"a",a);
            if(a == "autoleveling"){
            //执行自动调平操作
		        if(!printer->running && printer->delta && printer->firmware == Repetier_FirmWare){
					printer->injectManualCommand("G28");
					printer->injectManualCommand("G91");
					printer->injectManualCommand("G1 Z-100");
					printer->injectManualCommand("G90");
					printer->injectManualCommand("G29 S2");
					printer->injectManualCommand("G28");
					printer->injectManualCommand("G91");
					printer->injectManualCommand("G1 Z-100");
					printer->injectManualCommand("G90");
					printer->injectManualCommand("G32 S1");
					printer->delta = true;
				}else if(printer->running)//打印中，不执行操作
				{
					error = "Printer running";
				}else if(printer->delta || printer->firmware != Repetier_FirmWare)//非delta不执行，非Repetier不执行
				{
					error = "autoleveling is not available on your printer";
				}
            }else if(a == "preheat")
            {
            //执行预热操作
            	string filament;
                if(MG_getVar(ri,"filament",filament)){
                    if(filament == "abs"){					//ABS 预热
		                printer->injectManualCommand("M140 S90");
		                printer->injectManualCommand("M104 S230");
		                printer->injectManualCommand("M117 PreHeating ABS");
                    }else if(filament == "pla"){			//PLA 预热
		                printer->injectManualCommand("M140 S60");
		                printer->injectManualCommand("M104 S210");
		                printer->injectManualCommand("M117 PreHeating PLA");
                    }else error = "unknown filament";  		//未知耗材
            	}
            }else if(a == "getSD")			
            {
            //执行SD卡读取操作  M20  - List SD card
            	printer->injectManualCommand("M20");
            	boost::posix_time::ptime begin  = boost::posix_time::microsec_clock::local_time();
            	while(!printer->SDendflag){
            		boost::posix_time::ptime now  = boost::posix_time::microsec_clock::local_time();
    				boost::posix_time::time_duration td(now-begin);
    				if(td.seconds() > 5) {
    					error = "no printer response"; 
    					break;
    				}
            	}
			    boost::shared_ptr<list<boost::shared_ptr<PrinterResponse> > > rlist = printer->getResponsesSince(printer->lastSDId,16, printer->lastSDId);
			    Object lobj;
			    lobj.push_back(Pair("lastid",(int)start));
			    Array a;
			    list<boost::shared_ptr<PrinterResponse> >::iterator it = rlist->begin(),end=rlist->end();
			    for(;it!=end;++it) {
			        PrinterResponse *resp = (*it).get();
			        Object o;
			        o.push_back(Pair("id",(int)resp->responseId));
			        o.push_back(Pair("time",resp->getTimeString()));
			        o.push_back(Pair("text",resp->message));
			        o.push_back(Pair("type",resp->logtype));
			        a.push_back(o);
			    }
			    lobj.push_back(Pair("lines",a));
			    Object state;
			    printer->state->fillJSONObject(state);
			    lobj.push_back(Pair("state",state));
			    ret.push_back(Pair("data",lobj));
            }else if(a == "getEPR")
            {
            //执行获取EPRsetting操作 
            	if(printer->firmware == Repetier_FirmWare) printer->injectManualCommand("M205");
            	else if(printer->firmware == Marlin_FirmWare) printer->injectManualCommand("M501");
            	boost::posix_time::ptime begin  = boost::posix_time::microsec_clock::local_time();
            	while(!printer->EPRflag){
            		boost::posix_time::ptime now  = boost::posix_time::microsec_clock::local_time();
    				boost::posix_time::time_duration td(now-begin);
    				if(td.seconds() > 5) {
    					error = "no printer response"; 
    					break;
    				}
            	}
            	while(printer->EPRflag){
            		boost::posix_time::ptime now  = boost::posix_time::microsec_clock::local_time();
    				boost::posix_time::time_duration td(now-begin);
    				if(td.seconds() > 5) {
    					error = "no printer response"; 
    					break;
    				}
            	}
            	boost::shared_ptr<list<boost::shared_ptr<PrinterResponse> > > rlist = printer->getResponsesSince(printer->lastEPRId,8, printer->lastEPRId);
			    Object lobj;
			    lobj.push_back(Pair("lastid",(int)start));
			    Array a;
			    list<boost::shared_ptr<PrinterResponse> >::iterator it = rlist->begin(),end=rlist->end();
			    for(;it!=end;++it) {
			        PrinterResponse *resp = (*it).get();
			        Object o;
			        o.push_back(Pair("id",(int)resp->responseId));
			        o.push_back(Pair("time",resp->getTimeString()));
			        o.push_back(Pair("text",resp->message));
			        o.push_back(Pair("type",resp->logtype));
			        a.push_back(o);
			    }
			    lobj.push_back(Pair("lines",a));
			    Object state;
			    printer->state->fillJSONObject(state);
			    lobj.push_back(Pair("state",state));
			    ret.push_back(Pair("data",lobj));
            }        
        } else if(cmdgroup=="model") {
        /////////////////////////////////////////模型相关操作///////////////////////////////////////
            string a;
            MG_getVar(ri,"a",a);
            if(a=="list") {
                printer->getModelManager()->fillSJONObject("data",ret);
            } else if(a=="upload") {
#ifdef DEBUG
                cout << "Upload model" << endl;
#endif
				//上传模型，并完成GCode的解析，获得时间，耗材，层数信息
                string name,jobname;
                long size=0;
                MG_getVar(ri,"name", jobname);
                PrintjobPtr job = printer->getModelManager()->createNewPrintjob(jobname);
                handleFileUpload(conn,job->getFilename(), name,size,false,job);
                printer->getModelManager()->finishPrintjobCreation(job,name,size);
                printer->getModelManager()->fillSJONObject("data",ret);
				if(job->getLength() == 0) return;
				RLog::log("Calculating begin");
				job->Calculating = true;
				
				if(access(job->getFilename().c_str(),0)){ 
					return ;
				}
				job->getdataCal()->Cal_reset();
				job->setdata(job->getdataCal()->estimateTotalPrintTime(printer,job));
				double totalTime = job->getdata()->time;
				int totallayer = job->getdata()->layer;
				double totalMlength = job->getdata()->Mlength;
				int totalSpeed = job->getdata()->speedCont;
                RLog::log("totalTime = @", totalTime);
                RLog::log("totallayer = @", totallayer);
                RLog::log("totalMlength = @", totalMlength);
                RLog::log("totalSpeed = @", totalSpeed);
                //RLog::log("job->getFilename() = @", job->getFilename());
				string newname = printer->getModelManager()->encodeDataPart(job->getdata(),job->getFilename(),true);
				rename(job->getFilename(),newname);
				job->setFilename(newname);
				job->setTimeRatio(printer);
				
				//上传model写入日志
		        FILE *fp;
				char action[] = " <upload job> ";
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
				path p(job->getFilename());
				string filename = p.filename().stem().string();
				int ret = fwrite(filename.c_str(), filename.length(), 1, fp);
				if(ret <= 0) {
					perror("fwrite failed");
					exit(1);
				}
				fwrite("\n", 1, 1, fp);
				fclose(fp);
				
				job->Calculating = false;

#ifdef DEBUG
                cout << "Name:" << name << " Size:" << size << endl;
#endif
            } else if(a=="remove") {
            	//删除模型
                string sid;
                if(MG_getVar(ri,"id",sid)) {
                    int id = atoi(sid.c_str());
                    PrintjobPtr job = printer->getModelManager()->findById(id);
                    if(job.get())
                        printer->getModelManager()->RemovePrintjob(job);
                }	
                printer->getModelManager()->fillSJONObject("data",ret);
            } else if(a=="rename") {
            	//重命名模型
                string sid;
                string newname;
                if(MG_getVar(ri,"id",sid)) {
                	if(MG_getVar(ri,"newname",newname)) {
		                int id = atoi(sid.c_str());
		                PrintjobPtr job = printer->getModelManager()->findById(id);
		                if(job.get())
		                    printer->getModelManager()->RenamePrintjob(job,newname);
		                else error = "invalid job";
		            }
                }	
                printer->getModelManager()->fillSJONObject("data",ret);
            } else if(a=="start" || a=="copy") {
            	//开始打印模型
                string sid;
                if(MG_getVar(ri,"id",sid)) {
                    int id = atoi(sid.c_str());
                    PrintjobPtr model = printer->getModelManager()->findById(id);
                    if(model.get() && !printer->running) { //模型存在，并且打印机待机
                   		printer->getJobManager()->clearFolder();//清理job文件夹
						printer->getJobManager()->clearList();//清理job列表
                        PrintjobPtr job = printer->getJobManager()->createNewPrintjob(model->getName());
						job->isJob = true;
                        job->setLength(model->getLength());
                        try 
						{	//拷贝模型到job列表
                            std::ifstream  src(model->getFilename().c_str());
                            std::ofstream  dst(job->getFilename().c_str());
                            dst << src.rdbuf();
                            dst.close();
                            src.close();
                            printer->getJobManager()->finishPrintjobCreation(job, model->getName(), model->getLength());
							if(!model->Calculating)
							{
								job->setdata(model->getdata());
								string newjobname = printer->getJobManager()->encodeDataPart(job->getdata(),job->getFilename(),true);
								rename(job->getFilename(),newjobname);
				                job->setFilename(newjobname);
							}
							else job->Calculating = true;
							job->setSourceid(model->getId());
						} catch(const std::exception& ex)
		                {
		                    cerr << "error: Unable to create job file " << job->getFilename() << ":" << ex.what() << endl;
		                }
		                 string SizeMul;
                		if(MG_getVar(ri,"SizeMul",SizeMul)){
                			float Mul = atof(SizeMul.c_str());
                			job->setSizeMul(Mul);
                		}
		                //开始打印job
				        if(job.get()) {
		                    printer->getJobManager()->startJob(job->getId());
		                    
		                    //开始打印job写入日志
		                    FILE *fp;
							char action[] = " <start job> ";
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
							path p(job->getFilename());
							string filename = p.filename().stem().string();
							int ret = fwrite(filename.c_str(), filename.length(), 1, fp);
							if(ret <= 0) {
								perror("fwrite failed");
								exit(1);
							}
							fwrite("\n", 1, 1, fp);
							fclose(fp);
							
		                }else error = "fail to creat job";
                    }else if(printer->running) error = "printer is running";
                }
                printer->getJobManager()->fillSJONObject("data",ret);
            }else if(a=="calculate") {
            	//计算GCode参数
                string sid;
                if(MG_getVar(ri,"id",sid)) {
                    int id = atoi(sid.c_str());
                    PrintjobPtr job = printer->getModelManager()->findById(id);
                    if(job.get() && !job->Calculating){
                    	job->Calculating = true;
                    	job->getdataCal()->Cal_reset();
						job->setdata(job->getdataCal()->estimateTotalPrintTime(printer,job));
						double totalTime = job->getdata()->time;
						int totallayer = job->getdata()->layer;
						double totalMlength = job->getdata()->Mlength;
						int totalSpeed = job->getdata()->speedCont;
				        RLog::log("totalTime = @", totalTime);
				        RLog::log("totallayer = @", totallayer);
				        RLog::log("totalMlength = @", totalMlength);
				        RLog::log("totalSpeed = @", totalSpeed);
						string newname = printer->getModelManager()->encodeDataPart(job->getdata(),job->getFilename(),true);
						rename(job->getFilename(),newname);
						job->setFilename(newname);
						job->setTimeRatio(printer);
						job->Calculating = false;
                    }
                }	
                printer->getModelManager()->fillSJONObject("data",ret);
            }
        } else if(cmdgroup=="script") {
        ///////////////////////////////////////////////script相关操作//////////////////////////////////////////////////
            string a;
            MG_getVar(ri,"a",a);
            if(a=="list") {
                printer->getScriptManager()->fillSJONObject("data",ret);
            } if(a=="") {
                const int MAX_VAR_LEN = 256*1024;
                char buffer[MAX_VAR_LEN+1];
                int post_data_len = mg_read(conn, buffer, MAX_VAR_LEN);
                MG_getPostVar(buffer,post_data_len,ri,"a",a);
                if(a=="save") {
                    string name,jobname;
                    MG_getPostVar(buffer,post_data_len,ri,"f", jobname);
                    PrintjobPtr job = printer->getScriptManager()->findByName(jobname);
                    string text;
                    MG_getPostVar(buffer,post_data_len,ri,"text", text);
                    try {
                        ofstream out;
                        out.open(job->getFilename().c_str());
                        out << text;
                        out.close();
                    } catch(std::exception &ex) {
                        RLog::log("Error writing script: @",static_cast<const string>(ex.what()));
                    }
                } else if(a=="load") {
                    string name;
                    if(MG_getPostVar(buffer,post_data_len,ri,"f",name)) {
                    PrintjobPtr job = printer->getScriptManager()->findByName(name);
                    if(job.get()) {
                        try {
                            ifstream in;
                            in.open(job->getFilename().c_str());
                            char buf[200];
                            while(!in.eof()) {
                                in.getline(buf, 200); // Strips \n
                                size_t l = strlen(buf);
                                if(buf[l]=='\r')
                                    buf[l] = 0;
                                    mg_printf(conn,"%s\n",buf);
                                }
                                in.close();
                            } catch(std::exception &ex) {
                                RLog::log("Error reading script: @",static_cast<string>(ex.what()));
                            }
                        }
                    }
                    return;
                }
            }
        } else if(cmdgroup=="msg") {
        ///////////////////////////////////////////////////////msg相关操作//////////////////////////////////////////
            string a;
            string sid;
            MG_getVar(ri,"a",a);
            MG_getVar(ri,"id",sid);
            int id = 0;
            if(sid.length()>0) id = atoi(sid.c_str());
            //if(a=="unpause") {
            //    printer->stopPause();
            //}
            if(id) {
                gconfig->removeMessage(id);
                Array msg;
                gconfig->fillJSONMessages(msg);
                ret.push_back(Pair("messages",msg));
			}
        } else if(cmdgroup == "pconfig") {
        ///////////////////////////////////////////////////打印机连接操作/////////////////////////////////////
            string a;
            MG_getVar(ri,"a",a);
            if(a=="active") {
                string mode;
                if(MG_getVar(ri, "mode",mode)) {
                    printer->setActive(mode=="1");
                }
                listPrinter(ret);
            }
        } else if(printer->getOnlineStatus()==0) {
            error = "Printer offline";
            // ============ ONLINE COMMANDS FROM HERE ==============
        } else if(cmdgroup=="send") {
        ////////////////////////////////////////////////发送Gcode操作/////////////////////////////////////////
            string cmd;
            if(MG_getVar(ri,"cmd", cmd) /*&& !printer->running*/) {
                printer->injectManualCommand(cmd);
            }
        } else if(cmdgroup=="response") { // Return log
        ////////////////////////////////////////////////获取特定信息///////////////////////////////////////////
            string sfilter,sstart;
            uint8_t filter=0;
            uint32_t start=0;
            if(MG_getVar(ri,"filter",sfilter))
                filter = atoi(sfilter.c_str());
            if(MG_getVar(ri,"start",sstart))
                start = (uint32_t)atol(sstart.c_str());
            boost::shared_ptr<list<boost::shared_ptr<PrinterResponse> > > rlist = printer->getResponsesSince(start,filter, start);
            Object lobj;
            lobj.push_back(Pair("lastid",(int)start));
            Array a;
            list<boost::shared_ptr<PrinterResponse> >::iterator it = rlist->begin(),end=rlist->end();
            for(;it!=end;++it) {
                PrinterResponse *resp = (*it).get();
                Object o;
                o.push_back(Pair("id",(int)resp->responseId));
                o.push_back(Pair("time",resp->getTimeString()));
                o.push_back(Pair("text",resp->message));
                o.push_back(Pair("type",resp->logtype));
                a.push_back(o);
            }
            lobj.push_back(Pair("lines",a));
            Object state;
            printer->state->fillJSONObject(state);
            lobj.push_back(Pair("state",state));
            ret.push_back(Pair("data",lobj));
        } else if(cmdgroup=="move") {
        /////////////////////////////////////////////////手动操作坐标////////////////////////////////////////
            string sx,sy,sz,se;
            double x=0,y=0,z=0,e=0;
            if(MG_getVar(ri,"x",sx)) x = atof(sx.c_str());
            if(MG_getVar(ri,"y",sy)) y = atof(sy.c_str());
            if(MG_getVar(ri,"z",sz)) z = atof(sz.c_str());
            if(MG_getVar(ri,"e",se)) e = atof(se.c_str());
            printer->move(x, y, z, e);
        }
        ret.push_back(Pair("error",error));
    
        // Print result
		mg_printf(conn,"%s",write(ret,json_spirit::raw_utf8).c_str());
    }
    bool MG_getVar(const mg_request_info *info,const char *name, std::string &output)
    {
        output.clear();
        if(info->query_string==NULL) return false;
        const int MAX_VAR_LEN = 4096;
        char buffer[MAX_VAR_LEN];
        int len = mg_get_var(info->query_string, strlen(info->query_string), name, buffer, MAX_VAR_LEN - 1);
        if (len >= 0) {
            output.append(buffer, len);
            return true;
        }
        return false;
    }
    bool MG_getPostVar(char *buf,int buflen,const mg_request_info *info,const char *name, std::string &output) {
        output.clear();
        char *buffer = new char[buflen+1];
        int len = mg_get_var(buf,buflen, name, buffer, buflen - 1);
        if (len >= 0) {
            output.append(buffer, len);
            delete[] buffer;
            return true;
        }
        delete[] buffer;
        return false;        
    }
    string JSONValueAsString(const Value &v) {
        switch(v.type()) {
            case str_type:
                return v.get_str();
            case int_type:
            {
                char b[40];
                sprintf(b,"%d",v.get_int());
                return string(b);
            }
            case real_type: {
                char b[40];
                sprintf(b,"%f",v.get_real());
                return string(b);
            }
            case bool_type:
                if(v.get_bool()) return string("true");
                return string("false");
            case array_type:
                return "array";
            case obj_type:
                return "object";
            case null_type:
                return "null";
        }
        return("Unsupported type");
    }
    Value* findVariable(list<Value> &vars,const string& name) {
        list<Value>::iterator istart = vars.begin(),iend = vars.end();
        bool found = false;
        for(;istart!=iend && !found;istart++) {
            Value &v = *istart;
            if(v.type()==obj_type) {
                Object &obj = v.get_obj();
                vector<Pair>::iterator oit = obj.begin(),oend = obj.end();
                for(;oit!=oend;oit++) {
                    Pair &p = *oit;
                    if(p.name_ == name) {
                        return &p.value_;
                    }
                }
            }
        }
        return NULL;
    }
    void FillTemplateRecursive(string& text,string& result,list<Value>& vars,size_t start,size_t end) {
        size_t pos(start),pos2,posclose;
        while(pos<end) {
            pos2 = text.find("{{",pos);
            if(pos2==string::npos || pos2+3>=end) { // Finished, no more vars etc
                result.append(text,pos,end-pos);
                return;
            }
            pos2+=2;
            posclose = text.find("}}",pos2);
            if(posclose==string::npos) { // Finished, no more vars etc
                result.append(text,pos,end-pos);
                return;
            }
            
            result.append(text,pos,pos2-pos-2);
            char tp = text[pos2];
            
            if(tp == '#') { // foreach loop
                string name = text.substr(pos2+1,posclose-pos2-1);
                size_t spacePos = name.find(' ');
                string cmd = "";
                if(spacePos!=string::npos) {
                    cmd = name.substr(0,spacePos);
                    name = name.substr(spacePos+1);
                    pos2+=cmd.length()+1;
                }
                string ename = "{{/"+name+"}}";
                size_t epos = text.find(ename,posclose);
                pos2+=name.length()+3;
                posclose = epos+ename.length()-2; // Continue after block
                Value *v = findVariable(vars,name);
                if(v!=NULL) {
                if(cmd.length()==0) {
                    if(v!=NULL && v->type()==array_type) {
                        Array &a = v->get_array();
                        vector<Value>::iterator it = a.begin(),iend = a.end();
                        for(;it!=iend;++it) {
                            vars.push_front(*it);
                            FillTemplateRecursive(text, result, vars, pos2, epos);
                            vars.pop_front();
                        }
                    }
                } else if(cmd=="if" && v->type()==bool_type) {
                    if(v->get_bool()) {
                        FillTemplateRecursive(text, result, vars, pos2, epos);
                    }
                } else if(cmd=="ifnot" && v->type()==bool_type) {
                    if(!v->get_bool()) {
                        FillTemplateRecursive(text, result, vars, pos2, epos);
                    }
                }
                }
            } else if(tp=='!') { // Comment, simply ignore it
            } else { // Variable
                string name = text.substr(pos2,posclose-pos2);
                Value *v = findVariable(vars,name);
                if(v!=NULL)
                    result.append(JSONValueAsString(*v));
            }
            pos = posclose+2;
        }
    }
    void FillTemplate(string &text,string& result,Object& data) {
        result.clear();
        size_t start(0),end(text.length());
        result.reserve(end*2);
        list<Value> vars;
        vars.push_front(Value(data));
        FillTemplateRecursive(text,result,vars,start,end);
    }
    void* HandlePagerequest(struct mg_connection *conn) {
        const struct mg_request_info *ri = mg_get_request_info(conn);
        string uri(ri->uri);
        if(uri.length()<=1) uri="/index.php";
        if(uri.length()<5 || uri.substr(uri.length()-4,4)!=".php") return NULL;        
        
        // Step 1: Find translation file
        char *alang = (char*)mg_get_header(conn, "Accept-Language");
        string lang = "";
        if(alang!=NULL) {
            size_t n = strlen(alang);
            int mode = 0;
            size_t lstart;
            for(size_t i=0;i<n;i++) {
                char c = alang[i];
                if(mode==0 && isspace(c)) continue;
                if(mode == 0) {
                    lstart = i;
                    mode = 1;
                    continue;
                }
                if(mode ==1 && i==lstart+2) {
                    mode = 2;
                    alang[i]=0;
                    string tlang(&alang[lstart]);
                    alang[i] = c; // restore const value
                    if(doesLanguageExist(tlang)) {
                        lang = tlang;
                        break;
                    }
                }
                if(mode==2 && c==',') mode=0; // next acceptable language
            }
        }
        if(lang == "")
            lang = gconfig->getDefaultLanguage();
        string content;
        TranslateFile(gconfig->getWebsiteRoot()+uri,lang,content);
        // Step 2: Fill template parameter
        Object obj;
        string param;
        if(MG_getVar(ri,"pn", param)) {
            Printer *p = gconfig->findPrinterSlug(param);
            if(p) p->fillJSONObject(obj);
        }
        obj.push_back(Pair("version",string(Zeegine_SERVER_VERSION)));
        // Step 3: Run template
        string content2;
        FillTemplate(content, content2, obj);
        mg_printf(conn, "HTTP/1.0 200 OK\r\n"
                  "Cache-Control:public, max-age=0\r\n"
                  "Server: Repetier-Server\r\n"
                  "Content-Type: text/html; charset=utf-8\r\n\r\n");
        mg_write(conn, content2.c_str(), content2.length());
        return (void*)"";
    }
    static map<string,boost::shared_ptr<moFileLib::moFileReader> > rmap;
    static set<string> langNotExist;
    static boost::mutex langMutex;
    bool doesLanguageExist(string lang) {
        moFileLib::moFileReader *r = NULL;
        if((rmap[lang].get()) == NULL) {
            boost::mutex::scoped_lock l(langMutex);
            if(langNotExist.size()>0 && langNotExist.find(lang)==langNotExist.end())
                return false;
            string mofile = gconfig->getLanguageDir()+lang+".mo";
            if(!boost::filesystem::exists(mofile)) {
                langNotExist.insert(lang);
                return false;
            }
            r = new moFileLib::moFileReader();
            r->ReadFile(mofile.c_str());
            rmap[lang].reset(r);
            return true;
        } else return true;
        
    }
    void TranslateFile(const std::string &filename,const std::string &lang,std::string& result) {
        result.clear();
        
        // read mo file if not cached
        moFileLib::moFileReader *r = NULL;
        if((rmap[lang].get()) == NULL) {
            boost::mutex::scoped_lock l(langMutex);
            string mofile = gconfig->getLanguageDir()+lang+".mo";
            if(!boost::filesystem::exists(mofile))
                mofile = gconfig->getLanguageDir()+gconfig->getDefaultLanguage()+".mo";
            if(!boost::filesystem::exists(mofile)) return;
            r = new moFileLib::moFileReader();
            r->ReadFile(mofile.c_str());
            rmap[lang].reset(r);
        } else r = rmap[lang].get();
        // Read file contents
        string contents;
		std::ifstream in(filename.c_str(), ios::in | ios::binary);
		if (in.good())
        {
            in.seekg(0, std::ios::end);
            contents.resize((size_t)in.tellg());
            in.seekg(0, std::ios::beg);
			in.read(&contents[0], contents.size());
			in.close();
        } else {
            result.clear();
            return;
        }
        // Replace translations
        size_t start(0),pos(0),end(contents.length()),pos2,tstart,tend;
        result.reserve(end+end/10); // Reserve some extra space to prevent realloc
        while(pos<end) {
            pos = contents.find("<?php",start);
            if(pos==string::npos) { // End reached, copy rest of file
                result.append(contents,start,end-start);
                break;
            }
            pos2 = contents.find("?>",pos+5);
            if(pos2==string::npos) { // format error, copy rest of file
                result.append(contents,start,end-start);
                break;
            }
            result.append(contents,start,pos-start);
            start = pos2+2;
            tstart = contents.find("_(\"",pos);
            tend = contents.rfind("\")",pos2);
            if(tstart<tend && tend!=string::npos) {
                string key = contents.substr(tstart+3,tend-tstart-3);
                result.append(r->Lookup(key.c_str()));
            }
        }
    }
}

