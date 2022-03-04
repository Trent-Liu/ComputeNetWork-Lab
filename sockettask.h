//
// Created by trent-liu on 2021/10/15.
//

#ifndef SEVERTEST_SOCKETTASK_H
#define SEVERTEST_SOCKETTASK_H

#include "task.h"
#include "thread_pool.h"
#include <iostream>
#include <fstream>

class sockettask {
private:
    int sockfd, connfd, port;
    struct sockaddr_in servaddr, client;
public:
    sockettask(int portnum = 8080);
    ~sockettask();
    int startServer();
    void startTask();
};


#endif //SEVERTEST_SOCKETTASK_H
