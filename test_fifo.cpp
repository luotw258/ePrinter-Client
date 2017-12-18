#include <sys/types.h>
#include <sys/stat.h>
#include <iostream>
#include <errno.h>

using std::cout;
using std::endl;

int main(int argc, char *argv[])
{
    int ret = -1;
    ret = mkfifo("/home/ePrinter/fifo/job_queue.fifo", 777);
    if(ret)
    {
        perror("mkfifo");
    }
    
}

