#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>
#include <pthread.h>
#include <sys/wait.h>

using std::cout;
using std::endl;

void *printer_pthread(void *message)
{
    cout << "I'm a thread" << endl;
    pid_t pid;
    pid = fork();
    if(-1 == pid)
    {
        perror("fork");
    }
    if(!pid)
    {
        cout << "child pid : " << pid << endl;
        int ret;
        ret = execl("/home/ePrinter/print_file", "print_file", \
                    "/home/ePrinter/user_file/2-18292877903-光盘目录说明.txt", \
                    "1", "2", NULL);
        if(-1 == ret)
        {
            perror("execl");
            exit(-1);
        }
    }
    int status;
    waitpid(pid, &status, 0);
    cout << "pthread exit" << endl;
    return NULL;
}

int main(int argc, char *argv[])
{
    pthread_t print_pthread;
    pthread_create(&print_pthread, NULL, printer_pthread, NULL);
    int cnt = 0;
    while(1)
    {
        cout << "parent is still exist(" << ++cnt << ")" << endl;
        sleep(1);
    }
    return 0;
}
