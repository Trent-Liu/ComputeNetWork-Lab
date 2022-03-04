#ifndef _TASK_
#define _TASK_

#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/sendfile.h>
#include <sys/wait.h>
#include <iostream>
#include <string>
using namespace::std;


extern string buf3;
const int BUFFER_SIZE = 4096;
class task
{
private:
    int connfd;
public:
    task(){}
    task(int fd):connfd(fd){}
    ~task(){}

    void response(char *message, int status)
    {
        char buf[512];
        sprintf(buf, "HTTP/1.1 %d OK\r\nConnection: Close\r\n"
                     "content-length:%d\r\n\r\n", status, strlen(message));

        sprintf(buf, "%s%s", buf, message);
        write(connfd, buf, strlen(buf));

    }
    void response_file(int size, int status)
    {
        char buf[128];
        sprintf(buf, "HTTP/1.1 %d OK\r\nConnection: Close\r\n"
                     "content-length:%d\r\n\r\n", status, size);
        write(connfd, buf, strlen(buf));
    }

    void response_get(char *filename);

    void response_post(char *filename, char *argv);

    void doit();
};



#endif //