
#include "jpg2mp4.h"
using namespace std;


void jpg2mp4(int starNum, int endNum, string jobnameWithDir)
{
    //ofstream fout ("temp.txt", ios::out);
    //fout.close ();
    AviGenerator generator;
    for (int i = starNum; i <= endNum; ++i) {
        char c[200];
        //sprintf (c, "000000%03d", i);//加入jpeg文件，我有mao01~38.jpg文件
        sprintf (c, "/var/lib/ZeegineServer/storage/Snaps/000000%03d", i);//加入jpeg文件，我有mao01~38.jpg文件
        strcat (c, ".jpg");
        cout << c << endl;
        generator.add_frame (c);
    }
        
    cout << "generating avi file..." << endl;
    
    std::string videoName = jobnameWithDir;
    generator.set_avi_file (videoName);
    generator.set_fps (24);
    generator.set_avi_size (230, 100);
    generator.generate_avi ();
    
    cout << "generating avi file finish..." << endl;

}
