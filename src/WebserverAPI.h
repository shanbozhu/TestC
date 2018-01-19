
#ifndef __WebserverAPI__
#define __WebserverAPI__

#include <iostream>

namespace zeegine {
    extern void HandleWebrequest(struct mg_connection *conn);
    extern void* HandlePagerequest(struct mg_connection *conn);
    
    /** Load the given file and translate contents. Store the translated file
     in a string. Function is thread safe.
     @param filename File to load and translate.
     @param lang Language code for translation.
     @param result File content as translated string.
     */
    extern void TranslateFile(const std::string &filename,const std::string &lang,std::string& result);
    extern bool MG_getVar(const mg_request_info *info,const char *name, std::string &output);
    extern bool MG_getPostVar(char *buf,int buflen,const mg_request_info *info,const char *name, std::string &output);
    extern bool doesLanguageExist(std::string lang);
}

#endif 
