
#ifndef __GCode__
#define __GCode__

#include <iostream>
#include <boost/cstdint.hpp>
#include <boost/shared_ptr.hpp>
#include "printer.h"

using namespace boost;

//GCodeDataPacket类
class GCodeDataPacket {
public:
    GCodeDataPacket(int _length,uint8_t *_data);
    ~GCodeDataPacket();
    int length;
    uint8_t *data;
};

//typedef GCodeDataPacket* GCodeDataPacketPtr;
typedef boost::shared_ptr<GCodeDataPacket> GCodeDataPacketPtr;

//GCode类
class GCode {
    void ActivateV2OrForceAscii(Printer *printer);
    void parse(Printer *printer);
    void addCode(Printer *printer,char c,const std::string & val);
    Printer *printer;
public:
    uint16_t fields,fields2;
    int32_t n;
    uint8_t t;
    uint16_t g, m;
    float x, y, z, e, f,ii,j,r;
    int32_t s;
    int32_t p;
    std::string text;
    std::string orig;
    bool comment;
    bool hostCommand;
    bool forceASCII;
    GCode(Printer &printer, std::string const &cmd);
    ~GCode();

    inline bool hasM() {return (fields & 2)!=0;}
    inline bool hasN() {return (fields & 1)!=0;}
    inline bool hasG() {return (fields & 4)!=0;}
    inline bool hasT() {return (fields & 512)!=0;}
    inline bool hasX() {return (fields & 8)!=0;}
    inline bool hasY() {return (fields & 16)!=0;}
    inline bool hasZ() {return (fields & 32)!=0;}
    inline bool hasE() {return (fields & 64)!=0;}
    inline bool hasF() {return (fields & 256)!=0;}
    inline bool hasS() {return (fields & 1024)!=0;}
    inline bool hasP() {return (fields & 2048)!=0;}
    inline bool hasI() {return (fields2 & 1)!=0;}
    inline bool hasJ() {return (fields2 & 2)!=0;}
    inline bool hasR() {return (fields2 & 4)!=0;}
    inline bool hasText() {return (fields & 32768)!=0;}
    inline bool hasComment() {return comment;}
    inline bool isV2() {return (fields & 4096)!=0;}

    inline const std::string& getText() {return text;}
    inline uint16_t getG() {return g;}
    inline uint16_t getM() {return m;}
    inline uint8_t getT() {return t;}
    inline int32_t getN() {return n;}
    inline int32_t getS() {return s;}
    inline int32_t getP() {return p;}
    inline float getX() {return x;}
    inline float getY() {return y;}
    inline float getZ() {return z;}
    inline float getE() {return e;}
    inline float getF() {return f;}
    inline const std::string& getOriginal() {return orig;}
    void setN(int32_t line);
    GCodeDataPacketPtr getAscii(bool inclLine,bool inclChecksum,bool isJobComm);
    GCodeDataPacketPtr getBinary();
    std::string hostCommandPart();
    std::string hostParameter();
};
#endif 
