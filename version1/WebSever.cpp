//
#include <unistd.h>
#include <fcntl.h>

#include <arpa/inet.h>
#include <sys/socket.h>



#include <stdio.h>
#include <string.h>
#include <stdlib.h>



/*
 * 做好连接的准备工作
*/
int initServer();


/*
 * 处理客户端连接
 */
void handleClient(int fd);



int main() {

    //1.实现连接
    int serverFd = initServer();  //得到服务器Fd

    int clientFd;


    // 2.处理请求
    while(1) {
        //2.1等待客户端连接

        clientFd = accept(serverFd,NULL,NULL);
        if (-1 == clientFd) {
            printf ("服务器崩溃！\n");
            break;
        }
        printf("有客户端连接服务器了！\n");
        //2.2处理客户端请求
        handleClient(clientFd);

        //2.3关闭
        close(clientFd);

    }

    close(serverFd);







    return 0;
}


int initServer() {
    //1.创建 sockedFd
    int fd = socket(AF_INET,SOCK_STREAM,0);
    if (-1 == fd) {
        perror("创建socket失败:");
        return -1;
    }
    printf("创建socket成功\n");

    //不阻塞    使用setsockopt函数设置一下fd的工作状态
    int n = 1;
    setsockopt(fd, SOL_SOCKET,SO_REUSEADDR,&n,4);


    //2.设置服务器协议地址簇
    struct sockaddr_in addr;
    memset(&addr,0,sizeof(addr));


    addr.sin_family = AF_INET;  //和socket函数第一个端口一致
    //htons：一般pc机都是大端模式，网络服务器都是小端模式，小端转大端
    addr.sin_port = htons(80);         //所有的http都是80端口
    addr.sin_addr.s_addr = htonl(INADDR_ANY);          //当前电脑的任意网卡都可以
    //addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    //addr.sin_addr.s_addr = inet_addr("10.21.202.173");              //本机IP


    //3.绑定 fd和地址簇

    int r = bind(fd,(struct sockaddr*)&addr,sizeof(addr));
    if (-1 == r) {
        perror("绑定失败：");
        close(fd);
        return -1;
    }
    printf("绑定成功\n");

    //4.监听
    r = listen(fd,100);
    if (-1 == r) {
        perror("监听失败：");
        close(fd);
        return -1;
    }
    printf("监听成功\n");

    return fd;
}


void handleClient(int fd) {
    //接受客户端发来的信息
    char buff[1024*1024] = {0}; //1M
    int r = read(fd,buff,sizeof(buff));

    if (r > 0) {
        printf("接收到客户端发来的请求:%s\n",buff);
    }

    //解析客户端请求
    char fileName[20] = {0};
    sscanf(buff,"GET /%s", fileName);
    printf("解析出的文件名为： %s\n", fileName);

    //根据文件名得到文件类型，告诉浏览器发送什么文件
    char* mime = NULL;
    if (strstr(fileName,".html")) {
        mime = (char*)"text/html";     //文本或html类型
    } else if (strstr(fileName,".jpg")) {
        mime = (char*)"image/jpg";     //图片类型
    }


    char response[1024*1024] = {0};         //相应头
    sprintf(response,"HTTP/1.1 200 ok\r\nContent-Type: %s\r\n\r\n", mime);

    int responseLen = strlen(response);

    int fileFd = open(fileName,O_RDONLY);   //可以检查
    int fileLen = read(fileFd, responseLen+response, sizeof(response)-responseLen);

    write(fd, response, responseLen+fileLen);
    close(fileFd);
    sleep(1);

}