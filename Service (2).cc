// 简单写一个 命名管道框架

// 本进程为 服务端 只读客服发的数据

#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string>
#include <cstdio>
#include <unistd.h>
#define _FIFO_FILE "fifo"

int main()
{

    // 创建命名管道 fifo
    umask(0); // 防止umask 影响 fifo 文件的权限设置
    int n = mkfifo(_FIFO_FILE, 0666);
    if (n < 0)
    {
        // 创建失败
        std::cerr << "Service : mkfifo failed" << std::endl;
        exit(-1);
    }
    // 创建成功

    // 文件操作  Service 为读数据端口

    int f=open(_FIFO_FILE,O_RDONLY);//只读

    while(true)
    {
        //读取数据

        //建立缓冲区
        char buffer[1024];
        int n=read(f,buffer,sizeof(buffer)-1);
        std:: cout << "Service: Clinet Say###" << std::endl;
        buffer[n]=0;
        //打印数据
        std::printf("%s\n",buffer);
    }
    
    

    return 0;
}