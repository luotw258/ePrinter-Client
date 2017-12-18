#include <iostream>
#include <time.h>
#include <string.h>
using std::cout;
using std::endl;

int main(void)
{
    struct tm tm_;  
    time_t t_;  
    char buf[128]= {0};  
      
    strcpy(buf, "2017-11-16 14:24:21");  
    strptime(buf, "%Y-%m-%d %H:%M:%S", &tm_); //将字符串转换为tm时间  
    tm_.tm_isdst = -1;  
    t_  = mktime(&tm_); //将tm时间转换为秒时间  
    cout << t_ << endl;
    cout << time(NULL) - t_ << endl;
}
