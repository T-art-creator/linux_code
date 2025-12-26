// 进程池
#pragma once

#include <iostream>
#include <vector>
#include <unistd.h>
#include <cstdlib>
#include <sys/wait.h>
#include <string>
#include <cstring>
#include <ctime>
#include <cstdio>
// 任务函数指针
typedef void (*fun)();

// 三个方法

void Print()
{
    // 打印
    std::cout << "开始打印" << std::endl;
}

void Update()
{
    // 上传网络资源
    std::cout << "开始上传网络资源" << std::endl;
}

void Downdate()
{
    // 下载网络资源
    std::cout << "开始下载网络资源" << std::endl;
}

// 先描述
const int gdefultn = 5; // 默认创建五个信道
// 封装信道
class Channel
{
public:
    Channel() = default;
    Channel(int wfd, pid_t pid)
        : _wfd(wfd),
          _pid(pid)
    {
        _name = "Channel--" +std::to_string(_wfd) + "--" +std::to_string(_pid);
    }

    std::string Name()
    {
        return _name;
    }
    int Wfd()
    {
        return _wfd;
    }
    pid_t Pid()
    {
        return _pid;
    }

    void send(int code)
    {
        // 父进程向管道发送任务码
        ssize_t n = write(_wfd, &code, sizeof(code));
    }

    // 关闭信道写端
    void Close()
    {
        // 关闭
        int n = close(_wfd);
        (void)n; // 绕过编译
    }

    // 回收子进程资源
    void Wait()
    {
        int status = 0;
        int n = waitpid(_pid, &status, 0); // 阻塞等待
    }

    ~Channel()
    {
    }

private:
    int _wfd;   // 写端
    pid_t _pid; // 子进程pid
    std::string _name;
};

// 在组织

// 信道管理
class ChannelMerrage
{
public:
    ChannelMerrage()
        : _next(0)
    {
    }

    // 添加信道
    void Insert(int wfd, pid_t pid)
    {
        Channel tmp(wfd, pid);
        _cm.emplace_back(tmp);
    }

    int Size()
    {
        return _cm.size();
    }

    Channel Select()
    {
        // 采用轮询访问 的方式 选取信道
        auto &c = _cm[_next];
        _next++;
        _next = _next % _cm.size();
        return c;
    }

    // 关闭所有父进程的写端
    void CloseProcess()
    {
        // 关闭
        for (auto &e : _cm)
        {
            e.Close();
        }
    }

    // 回收所有子进程的资源
    void WaitProcess()
    {
        for (auto &e : _cm)
        {
            e.Wait();
        }
    }


    void CloseAndWait()
    {
        //关闭所有写端 并且回收子进程

        //解决方案1： 信道的写端从下至上  即 从后到前关闭写端 使得每一个信道写端引用计数变为0 成功关闭
        // for(int i=_cm.size()-1;i>=0;i--)
        // {
        //     //关闭当前信道的写端
        //     _cm[i].Close();
        //     //回收子进程
        //     _cm[i].Wait();
        // }

        //解决方案二 每次创建子进程时 把从父进程拷贝的兄弟的进程写端全部关闭  从前往后正常关闭回收
        for(auto& e:_cm)
        {
            e.Close();//关闭信道的写端
            e.Wait();//回收子进程
        }

    }


    ~ChannelMerrage()
    {
    }

private:
    std::vector<Channel> _cm;
    int _next; // 下一个信道的坐标
};

// 任务管理
class TaskMerge
{
public:
    TaskMerge()
    {
        srand(time(nullptr)); // 时间戳
        // 填充函数指针 完善任务方法

        // 三个方法
        _codes.emplace_back(Update);   // 上传数据
        _codes.emplace_back(Downdate); // 下载数据
        _codes.emplace_back(Print);    // 打印数据
    }

    // 挑选任务码
    int code()
    {
        // 随机的方式挑选

        return rand() % _codes.size();
    }

    void Exe(int code)
    {
        // 根据任务码 执行不同函数

        if (code >= _codes.size())
        {
            // 非法
            std::cout << "任务码非法" << std::endl;
            exit(-1);
        }
        // 执行任务
        std::cout << std:: endl;//美观
        _codes[code]();
    }

    ~TaskMerge()
    {
    }

private:
    std::vector<fun> _codes; // 存储任务码对应的函数方法指针
};

class ProcessPool
{
public:
    ProcessPool()
        : _ProcessNum(_pp.Size())
    {
    }

    bool Work(int rfd)
    {

        // 读取合法性判断
        while (true)
        {
            // 读取任务码
            int code = 0;
            ssize_t n = read(rfd, &code, sizeof(code));
            if (n > 0)
            {
                if (n != sizeof(code))
                {
                    // 未正确读到任务码
                    continue;
                }
                // 读到任务码
               std::printf("我是子进程 pid:%d  任务码：%d",getpid(),code);

                // 执行工作
                _tm.Exe(code);
                sleep(5);
            }
            else if (n == 0)
            {
                // 读取非法  文件已关闭
                std::cout << "子进程退出" << std::endl;
                return false;
            }
            else
            {
                std::cout << "读取错误" << std::endl;
                return false;
            }
        }
        return true;
    }

    void Start(int num = gdefultn)
    {
        // 创立进程池
        while (num--)
        {
            // 构建匿名通道
            int pipefd[2] = {0}; // 0 为读端，1 为写端
            int pfd = pipe(pipefd);
            (void)pfd; // 不关注 返回值，绕过编译
            // 创建子进程 形成信道
            pid_t pid = fork();
            if (pid < 0)
            {
                // 创建失败
                std::cerr << "ProcessPoll fork failed" << std::endl;
                exit(-1); // 退出
            }
            else if (pid == 0)
            {
                // 子进程
                // 父写 子读

                // 关闭写端

                //关闭兄弟进程的所有信道的写端
                _pp.CloseProcess();// 子进程拿到父进程的信道数据 执行关闭操作 发生写实拷贝  不影响父进程的信道资源
                //关闭自己的写端
                int c_f = close(pipefd[1]);
                if (c_f < 0)
                {
                    // 关闭失败
                    perror("close failed\n");
                }
                Work(pipefd[0]); // 子进程工作

                // 关闭读端
                close(pipefd[0]);
                // 退出
                exit(EXIT_SUCCESS);
            }
            // 父进程
            // 关闭读端
            close(pipefd[0]);

            // 组织管理 信道
            _pp.Insert(pipefd[1], pid);
        }
    }

    // 进程池运行
    void Run()
    {
        // 挑选任务码
        int code = _tm.code();
        // 挑选 轮询方法
        auto c = _pp.Select();
        // 通过写端 向通道写入任务码
        std::cout << "选择了一个子进程:" << c.Name() << std::endl;
        std::cout << "选择了一个任务码：" << code << std::endl;
        c.send(code); // 发送任务码
    }

    // 进程池停止
    void Stop()
    {
        // 对于父亲 子读 只需要 切断父的写入端口 os 自动杀死子进程

        // // 关闭所有父进程的写端
        // _pp.CloseProcess();

        // // 回收所有子进程资源
        // _pp.WaitProcess();

        _pp.CloseAndWait();

    }

    ~ProcessPool()
    {
    }

private:
    ChannelMerrage _pp; // 信道管理组
    int _ProcessNum;    // 信道个数
    TaskMerge _tm;      // 任务码管理组
};