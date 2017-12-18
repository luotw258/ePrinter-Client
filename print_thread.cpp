#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>
#include <pthread.h>
#include <print_queue.h>
#include <time.h>
#include <string>
#include <sys/wait.h>
#include "client_connect.h"

using std::cout;
using std::endl;
using std::to_string;
using std::string;

#define SECONDS_OF_DAY (86400) //24*60*60(s)
#define ORDER_PERIOD (300) //5*60(s) 
//获取到的是零区时间，加上东八区的八个小时
#define ADJUST_TIME (28800) //8*60*60(s)  


#define PAPERSIZE_A4 (0)
#define PAPERSIZE_16K (1)

static pid_t print_one_file(ElementType one_job)
{
    pid_t pid = fork();
    if(-1 == pid)
    {
        perror("fork");
    }
    if(!pid)
    {
        cout << "child pid : " << pid << endl;

        string paper_ranges = ("page-ranges=\"" + \
                    to_string(one_job.order.fstart)+ "-" + \
                    to_string(one_job.order.fend) + "\"");
                    
        string paper_size = "media=";
        paper_size += one_job.order.fpapersize == PAPERSIZE_A4 ? \
                    "A4" : "legal";
        
        int ret;
        ret = execl("/usr/bin/lpr", "lpr", \
                    one_job.file_url, \
                    "-#", to_string(one_job.order.fcopies).c_str(), \
                    "-o", paper_ranges.c_str(), \
                    "-o", paper_size.c_str(), \
                    NULL);
        if(-1 == ret)
        {
            perror("execl");
            return -1;
        }
    }
    return pid;
}
static int convert_to_plantime(time_t time)
{
    return (time%SECONDS_OF_DAY)/ORDER_PERIOD;
}
void *printer_pthread(void *message)
{
    cout << "print pthread is runing..." << endl;
    CPrintQueue *print_queue = \
        (CPrintQueue*)(*((CPrintQueue**)(message)));
    CClientConnect *connection = \
        (CClientConnect*)(*((CClientConnect**)(message + sizeof(void*))));
        
    ElementType one_job;
    int current_time;
    pid_t pid;
    int status;
    while(1)
    {
        print_queue->print_all_job();
        //获取时间
        current_time = convert_to_plantime(ADJUST_TIME + time(NULL));
        cout << "current time : " << current_time << endl;
        //是否有需要打印的订单
        if(print_queue->is_empty())
        {
            sleep(5);
            continue;   //等5秒在查
        }
        one_job = print_queue->find_min();
        if(current_time < (one_job.order.fplantime-1)) //时间还没到，不打印
        {
            sleep(5);
            continue;   //等5秒在查
        }

        //打印
        one_job.time_start_print = time(NULL); //更新任务状态
        if(-1 == print_one_file(one_job))
        {
            continue;
        }
        
        waitpid(pid, &status, 0);
        one_job.time_finish_print = time(NULL); //更新任务状态
        connection->request_push_complete(one_job.order.fno);
        
        print_queue->delete_min();
        //删除任务后队列有变化，更新文件
        if(print_queue->update_queue_file())
            cout << "update_queue_file failed!" << endl;
        else
            cout << "update_queue_file complete!" <<endl;
    }
    
    cout << "pthread exit" << endl;
    return NULL;
}

