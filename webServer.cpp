#include "sockettask.h"

using namespace::std;

string buf1,buf2,buf3;

int main()
{
    ifstream infile;
    infile.open("/home/trent-liu/SeverTest/a.txt", ios::in);
    if (!infile.is_open())
    {
        cout << "读取文件失败" << endl;

    }
    getline(infile,buf1);
    buf1.pop_back();
    getline(infile,buf2);
    buf2.pop_back();
    getline(infile,buf3);
    buf3.pop_back();

    string buf;
    sockettask socktask;
    if (-1 == socktask.startServer()) {
        cout << "服务器崩溃" << endl;
    }

    socktask.startTask();

    return 0;
}