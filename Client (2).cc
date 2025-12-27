// 该进程用来完成客户 对 命名管道 传输数据 让 服务器 读取

#include <iostream>
#include <string>
#include <fcntl.h>
#include <string>
#include <unistd.h>
#define _FIFO_FILE "fifo"

int main()
{

    int f=open(_FIFO_FILE,O_WRONLY|O_TRUNC);//写入并清空
    //写数据
    while(true)
    {
        //建立缓冲区
        std::string buffer;
        std::cout << " Client:Please Entry###" << std::endl;
        std::cin >> buffer;

        int n=write(f,buffer.c_str(),buffer.size());

    }

    return 0;
}
