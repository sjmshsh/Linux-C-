#include <iostream>
#include <cerrno>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string>

const uint16_t port = 8080;
#define NUM 1024
int main() 
{
    // 1. 创建套接字, 打开网络文件
    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0)
    {
        std::cerr << "socket create error" << errno << std::endl;
        return 1;
    }
    // 2. 给该服务器绑定端口和ip(特殊处理)

    struct sockaddr_in local;
    local.sin_family = AF_INET;
    local.sin_port = htons(port); //此处的端口号，是我们计算机上的变量,是属于主机序列,所以要转成网络序列
    local.sin_addr.s_addr = INADDR_ANY;

    if(bind(sock, (struct sockaddr*)&local, sizeof(local)) < 0) 
    {
        std::cerr << "bin error : " << errno << std::endl;
        return 2;
    }

    // 3.提供服务
    bool quit = false;
    char buffer[NUM];
    while (!quit)
    {
        struct sockaddr_in peer;
        socklen_t len = sizeof(peer);
        recvfrom(sock, buffer, sizeof(buffer) - 1, 0, (struct sockaddr*)&peer, &len);
        std::cout << "client# " << buffer << std::endl;
        std::string echo_hello = "hello";
        sendto(sock, echo_hello.c_str(), echo_hello.size(), 0, (struct sockaddr*)&peer, len);
    }
    
    return 0;
}
