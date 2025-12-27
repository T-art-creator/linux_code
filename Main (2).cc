#include <iostream>

#include "ProcessPool.hpp"


int main()
{
    //创建进程池
    ProcessPool pp;

    //进程池初始化
    pp.Start();

    //运行进程池
    int n=10;
    while(n--)
    {
        pp.Run();
        sleep(3);
    }

    //释放进程池

    pp.Stop();
    return 0;
}