#include "client_net.h"

CClientNet::CClientNet()
{cout << "do not use this structure function(CClientNet)\n";}

CClientNet::~CClientNet()
{
    delete this->server_ip;
}
CClientNet::CClientNet(const char *server_ip, unsigned short port)
{
    this->server_ip = new char[16]();
    strcpy(this->server_ip, server_ip);
    this->port = port;
}
CClientNet::CClientNet(CClientNet &net){}

int CClientNet::connect_to_server(void)
{
    // 创建通信端点：套接字  
    this->sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd < 0)  
    {  
        perror("socket");
    }
    // 设置服务器地址结构体  
    bzero(&(this->server_addr),sizeof(struct sockaddr_in)); // 初始化服务器地址  
    this->server_addr.sin_family = AF_INET;   // IPv4  
    this->server_addr.sin_port = htons(this->port); // 端口  
    inet_pton(AF_INET, this->server_ip, &server_addr.sin_addr);// ip
    // 主动连接服务器  
    int err_log = connect(sockfd, (struct sockaddr*)&(this->server_addr), sizeof(struct sockaddr_in));       
    if(err_log != 0)  
    {  
        perror("connect");  
        close(this->sockfd);  
        return -1;  
    }  
    return 0;
}
int CClientNet::xrecv(char *buf, int len, int* schedule, int display_schedule)
{
    int ret;
    
    int recv_size = 0;
    char *schedule_dis = new char[102]();
    memset(schedule_dis, ' ', 101);
    const char *schedule_rotate = "|/-\\";
    int rate = 0;
    int max_len = len;
    int cnt = 0;
    
    while(len != 0 && (ret = recv(this->sockfd, (char*)buf, len, 0)) != 0)
    {
        if(-1 == ret)
        {
            if(EINTR == errno)
                continue;
            perror("recv in xrecv");
            return -1;
        } 
        len -= ret;
        buf += ret;

        
        if(1 == display_schedule)
        {
            recv_size += ret;
            rate = (int)((float)recv_size/(float)max_len*100.0f);
            if(NULL != schedule)
            {
                cout << schedule <<endl;
                (*schedule) = rate;
                cout << "set schedule as " << *schedule << endl;
            }
            
            schedule_dis[rate] = '#';
            printf("[%-101s][%3d%%][%c]\r", schedule_dis, rate, schedule_rotate[(++cnt)%4]);
            fflush(stdout);
        }
    }
    if(1 == display_schedule)
        fprintf(stderr, "\n");
    delete schedule_dis;
    
    return 0;
}
int CClientNet::xsend(char *buf, int len, int* schedule, int display_schedule)
{
    int ret;
    int send_size = 0;
    char *schedule_dis = new char[102]();
    memset(schedule_dis, ' ', 101);
    const char *schedule_rotate = "|/-\\";
    int rate = 0;
    int max_len = len;
    int cnt = 0;
    while(len != 0 && (ret = send(this->sockfd, (char*)buf, len, 0)) != 0)
    {
        if(-1 == ret)
        {
            if(EINTR == errno)
                continue;
            perror("send in xsend");
            return -1;
        }
        len -= ret;
        buf += ret;
        
        if(1 == display_schedule)
        {
            send_size += ret;
            rate = (int)((float)send_size/(float)max_len*100.0f);
            if(NULL != schedule)
            {
                (*schedule) = rate;
                cout << "set schedule as " << *schedule << endl;
            }
                
            schedule_dis[rate] = '#';
            printf("[%-101s][%3d%%][%c]\r", schedule_dis, rate, schedule_rotate[(++cnt)%4]);
            fflush(stdout);
        }
    }
    if(1 == display_schedule)
        fprintf(stderr, "\n");
    delete schedule_dis;

    return 0;
}





