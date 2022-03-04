//
// Created by trent-liu on 2021/10/15.
//

#include "sockettask.h"

using namespace::std;
extern string buf1;
extern string buf2;


sockettask::sockettask(int portnum) {
    port = portnum;
}

sockettask::~sockettask() {
    close(sockfd);
    close(connfd);
}

int sockettask::startServer() {



    //设置服务端的sockaddr_in
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr(buf1.c_str());
    servaddr.sin_port = htons(atoi(buf2.c_str()));

    //创建socket
    try {
        sockfd = socket(AF_INET, SOCK_STREAM, 0);
        if(-1 == sockfd) {
            cerr << "创建socket失败\n";
            throw -1;
        }
        else {
            cout << "创建socket成功" << endl;
        }
    }
    catch (int e){
        return e;
    }

    //绑定
    int ret = bind(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr));
    if(-1 == ret)
    {
        cerr << "绑定失败：";
        return -1;
    }
    cout << "绑定成功" << endl;
    //监听
    ret = listen(sockfd, 10);
    if(-1 == ret)
    {
        cerr << "监听失败：";
        return -1;
    }
    cout << "监听成功" << endl;
    return 1;

}

void sockettask::startTask() {
    //创建线程池
    threadpool<task> pool(20);
    pool.start();  //线程池开始运行

    while(1)
    {
        //printf("new connection\n");
        socklen_t len = sizeof(client);
        //接受连接
        connfd = accept(sockfd, (struct sockaddr *)&client, &len);
        if (-1 == connfd) {
            cout << "服务器崩溃！" << endl;
            break;
        }
        cout << "有客户端连接服务器了" << endl <<
        "客户端地址为 ："  << inet_ntoa(client.sin_addr) << endl <<
        "客户端端口为 ："<< client.sin_port << endl;

        task *ta = new task(connfd);
        //向线程池添加任务
        pool.append_task(ta);
    }
}