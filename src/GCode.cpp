
#define _CRT_SECURE_NO_WARNINGS // Disable deprecation warning in VS2005
#define _CRT_SECURE_NO_DEPRECATE 
#define _SCL_SECURE_NO_DEPRECATE 

#include <stdio.h>
#include <boost/algorithm/string.hpp>

#include "GCode.h"
#include "printer.h"
#include "Printjob.h"
#include "RLog.h"


using namespace std;
using namespace boost::algorithm;

//初始化GCodeDataPacket类中的成员变量
GCodeDataPacket::GCodeDataPacket(int _length,uint8_t *_data) {
    length = _length;
    data = _data;
}
//释放对象资源
GCodeDataPacket::~GCodeDataPacket() {
    delete[] data;
}
//初始化GCode类中的成员变量
GCode::GCode(Printer &p,string const& cmd) {
    orig = cmd;
    text = "";
    hostCommand = false;
    forceASCII = false;
    parse(&p);
    printer = &p;
}
//释放对象资源
GCode::~GCode() {
}

//设置发送代码的序号
void GCode::setN(int32_t line) {
    n = line;
    fields |= 1;
}
//分析GCode，提取里面的字符与数值
void GCode::parse(Printer *printer) {
    fields = 128;
    fields2 = 0;
    string cmd = orig;
    size_t l = cmd.length(),i;
    int mode = 0; // 0 = search code, 1 = search value
    char code = ';';
    size_t p1=0;
    size_t rangelocation,rangelength;
    for (i = 0; i < l; i++)
    {
        char c = cmd[i];
        if(i==0 && c=='@') {
            hostCommand = true;
            return;
        }
        if (mode == 0 && c >= 'A' && c <= 'Z')
        {
            code = c;
            mode = 1;
            p1 = i + 1;
            continue;
        }
        else if (mode == 1)
        {
            if (c == ' ' || c=='\t' || c==';')
            {
                rangelocation = p1;
                rangelength = i-p1;
                addCode(printer,code,cmd.substr(rangelocation,rangelength));
                mode = 0;
                if (hasM() && (m == 23 || m == 28 || m == 29 || m == 32 || m == 30 || m == 117))
                {
                    size_t pos = i;
                    while (pos < l && isspace(cmd[pos])) pos++;
                    size_t end = l;
                    //while (end < l && !isspace(cmd[end])) end++;
                    rangelocation = pos;
                    rangelength = end-pos;
                    text = cmd.substr(rangelocation,rangelength);
                    fields |=32768;
                    break;
                }
            }
        }
        if (c == ';') break;
    }
    if (mode == 1) {
        rangelocation = p1;
        rangelength = l-p1;
        addCode(printer,code,cmd.substr(rangelocation,rangelength));
    }
    comment = fields == 128;
}
//获取一条二进制的完全命令（包含序号和校验）
GCodeDataPacketPtr GCode::getBinary()
{
    uint8_t data[128];
    int datalen=0;
    uint16_t ns = (n & 65535);
    bool v2 = isV2();
    *(uint16_t *)data = fields;datalen+=2;
    if(v2) {
        *(uint16_t *)&data[datalen] = fields2;datalen+=2;
        if(hasText()) {
            ((uint8_t*)data)[datalen++]= (uint8_t)text.length();
        }
    }
    if (hasN()) {*(uint16_t *)&data[datalen] = ns;datalen+=2;}
    if(v2) {
        if (hasM()) {*(uint16_t *)&data[datalen] = m;datalen+=2;};
        if (hasG()) {*(uint16_t *)&data[datalen] = g;datalen+=2;}
    } else {
        if (hasM()) {data[datalen++] = (uint8_t)m;}
        if (hasG()) {data[datalen++] = (uint8_t)g;}
    }
    if (hasX()) {*(float *)&data[datalen] = x;datalen+=4;};
    if (hasY()) {*(float *)&data[datalen] = y;datalen+=4;};
    if (hasZ()) {*(float *)&data[datalen] = z;datalen+=4;};
    if (hasE()) {*(float *)&data[datalen] = e;datalen+=4;};
    if (hasF()) {*(float *)&data[datalen] = f;datalen+=4;};
    if (hasT()) {*(uint8_t *)&data[datalen] = t;datalen+=1;};
    if (hasS()) {*(int32_t *)&data[datalen] = s;datalen+=4;};
    if (hasP()) {*(int32_t *)&data[datalen] = p;datalen+=4;};;
    if(v2) {
        if (hasI()) {*(float *)&data[datalen] = ii;datalen+=4;};
        if (hasJ()) {*(float *)&data[datalen] = j;datalen+=4;};
        if (hasR()) {*(float *)&data[datalen] = r;datalen+=4;};
    }
    if (hasText())
    {
        size_t i, len = (int)text.length();
        if (len > 16 && !v2) len = 16;
            for (i = 0; i < len; i++)
            {
                uint8_t ch = text[i];
                data[datalen++] =ch;
            }
        if(!v2)
            for(;i<16;i++) data[datalen++] = 0;
    }
    // compute fletcher-16 checksum
    uint16_t sum1 = 0, sum2 = 0;
    int blen = (int)datalen,i;
    for (i=0;i<blen;i++)
    {
        int c = data[i]; // Analyzer reports this falsely as uninitalized!
        sum1 = (sum1 + c) % 255;
        sum2 = (sum2 + sum1) % 255;
    }
    uint8_t bsum1 = sum1 & 255;
    uint8_t bsum2 = sum2 & 255;
    data[datalen++] = bsum1;
    data[datalen++] = bsum2;
    uint8_t *dp = new uint8_t[datalen];
    memcpy(dp,data,datalen);
    return shared_ptr<GCodeDataPacket>(new GCodeDataPacket(datalen,dp));
}
//获取一条Ascii编码的完全命令（包含序号和校验）
GCodeDataPacketPtr GCode::getAscii(bool inclLine,bool inclChecksum,bool isJobComm)
{
	//RLog::log("GCode in");
	//PrintjobPtr job;
	float sizeMul = 1;
	if(printer->getJobManager()->getRunningJob().get() && isJobComm){
		sizeMul = printer->getJobManager()->getRunningJob()->getJobSizeMul();
		//RLog::log("isRunning");
	}
    string st;
    char b[100];
    if(hasM() && m==117) inclChecksum = false; // For marlin
    if (inclLine && hasN())  {
        sprintf(b,"N%d ",(int)n);
        st+=b;
    }
    if(forceASCII) {
        size_t cp = orig.find(';');
        if(cp==string::npos)
            st+=orig;
        else {
            string withoutComment =orig.substr(0,cp);
            trim(withoutComment);
            st+=withoutComment;
        }
    }
    else {
        if (hasM())
        {
            sprintf(b,"M%d",(int)m);
            st+=b;
        }
        if (hasG())
        {
            sprintf(b,"G%d",(int)g);
            st+=b;
        }
        if (hasT())
        {
            if (hasM()) st+=" ";
            sprintf(b,"T%d",(int)t);
            st+=b;
        }
        if (hasX())
        {
		    sprintf(b," X%.2f ",x*sizeMul);//记得与analysis对应，在PrinterState里面
		    //RLog::log("JobSizeMul=@",(float)sizeMul);
            st+=b;
        }
        if (hasY())
        {
            sprintf(b," Y%.2f ",y*sizeMul);
            st+=b;
        }
        if (hasZ())
        {
            sprintf(b," Z%.2f ",z*sizeMul);
            st+=b;
        }
        if (hasE())
        {
            sprintf(b," E%.4f ",e*sizeMul*sizeMul*sizeMul);//挤出量要三次方，长度，厚度，宽度
            st+=b;
        }
        if (hasF())
        {
            sprintf(b," F%.2f ",f/sizeMul);//需要减速，挤出量太大了
            st+=b;
        }
        if (hasI())
        {
            sprintf(b," I%.2f ",ii);
            st+=b;
        }
        if (hasJ())
        {
            sprintf(b," J%.2f ",j);
            st+=b;
        }
        if (hasR())
        {
            sprintf(b," R%.2f ",r);
            st+=b;
        }
        if (hasS())
        {
            sprintf(b," S%d ",(int)s);
            st+=b;
        }
        if (hasP())
        {
            sprintf(b," P%d ",(int)p);
            st+=b;
        }
        if (hasText())
        {
            st+=" "+text;
        }
    }
    if(hasM() && (m==117)) //M117：在屏幕上打印信息
        inclChecksum = false;
    if (inclChecksum)
    {
        int check = 0;
        int l = (int)st.length(),iii;
        for (iii=0;iii<l;iii++) {
            check ^= (st[iii] & 0xff);
        }
        check ^= 32;
        sprintf(b," *%d",(int)check);
        st +=b;
    }
    st+='\n';
    uint8_t *dp = new uint8_t[st.length()];
    memcpy(dp,st.c_str(),st.length());
    return shared_ptr<GCodeDataPacket>(new GCodeDataPacket((int)st.length(),dp));
}
void GCode::ActivateV2OrForceAscii(Printer *printer)
{
    if (printer->binaryProtocol < 2)
    {
        forceASCII = true;
        return;
    }
    fields |= 4096;
}

void GCode::addCode(Printer *printer,char c,const string& val) {
    double d = atof(val.c_str());
    switch (c)
    {
        case 'N':
        case 'n':
            n = (int32_t)d;
            fields|=1;
            break;
        case 'G':
        case 'g':
            g = (uint16_t)d;
            if(d>255) ActivateV2OrForceAscii(printer);
            fields|=4;
            break;
        case 'M':
        case 'm':
            m = (uint16_t)d;
            if(d>255) ActivateV2OrForceAscii(printer);
            fields|=2;
            break;
        case 'T':
        case 't':
            t = (uint8_t)d;
            fields|=512;
            break;
        case 'S':
        case 's':
            s = (int)d;
            fields|=1024;
            break;
        case 'P':
        case 'p':
            p = (int)d;
            fields|=2048;
            break;
        case 'X':
        case 'x':
            x = (float)d;
            fields|=8;
            break;
        case 'Y':
        case 'y':
            y = (float)d;
            fields|=16;
            break;
        case 'Z':
        case 'z':
            z = (float)d;
            fields|=32;
            break;
        case 'E':
        case 'e':
            e = (float)d;
            fields|=64;
            break;
        case 'A':
        case 'a':
            e = (float)d;
            fields|=64;
            forceASCII = true;
            break;
        case 'F':
        case 'f':
            f = (float)d;
            fields|=256;
            break;
        case 'i':
        case 'I':
            ii = (float)d;
            fields2|=1;
            ActivateV2OrForceAscii(printer);
            break;
        case 'j':
        case 'J':
            j = (float)d;
            fields2|=2;
            ActivateV2OrForceAscii(printer);
            break;
        case 'r':
        case 'R':
            r = (float)d;
            fields2|=4;
            ActivateV2OrForceAscii(printer);
            break;
        default: // Unsupported, so send line instead
            forceASCII = true;
            break;
    }
}
string GCode::hostCommandPart()
{
    size_t pos = orig.find(' ');
    if (pos==string::npos) return orig;
    return orig.substr(0,pos);
}
string GCode::hostParameter()
{
    size_t pos = orig.find(' ');
    if (pos==string::npos) return string("");
    return orig.substr(pos+1);
}
