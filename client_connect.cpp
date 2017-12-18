#include "client_connect.h"


CClientConnect::CClientConnect()
{cout << "do not use this structor function!(CClientConnect)\n";}
CClientConnect::~CClientConnect(){}
CClientConnect::CClientConnect(int printer_no, CClientNet *net)
{
    this->printer_no = printer_no;
    this->net = net;
}
CClientConnect::CClientConnect(CClientConnect &connect)
{
    this->net = connect.net;
    this->printer_no = connect.printer_no;
}


int CClientConnect::request_order(OrderInfo *order)
{
    //The four int are start_num(1314) + printer_no, request_type and sned_length respectively
    int malloc_len = sizeof(int) + sizeof(int) + sizeof(int) + sizeof(int) \
                    + MD5_LEN;
    char *buf =  new char[malloc_len]();

    *((int*)buf) = (int)1314;    
    *((int*)(buf+ sizeof(int))) = this->printer_no;
    *((int*)(buf+ 2*sizeof(int))) = (int)OrderResuest;
    *((int*)(buf+ 3*sizeof(int))) = malloc_len;

    MD5_CTX md5;  
    MD5Init(&md5);  
    MD5Update(&md5, (unsigned char*)buf, malloc_len - MD5_LEN);  
    MD5Final(&md5, (unsigned char*)buf + malloc_len - MD5_LEN);     
    this->net->xsend(buf, malloc_len);  //send order request

    if(NULL == order)
    {
        cout << "order is NULL\n";
        return -1;
    }
    this->net->xrecv((char*)order, (int)sizeof(OrderInfo));

    delete buf;
    return 0;
}
int CClientConnect::request_file(const char *file_url)
{
    if(NULL == file_url)
    {
        cout << "file_url is NULL" << endl;
        return -1;
    }
    //The four int are start_num(1314) + printer_no, request_type and sned_length respectively
    int malloc_len = sizeof(int) + sizeof(int) + sizeof(int) + sizeof(int) \
                    + (strlen(file_url)+1) + MD5_LEN;
    char *buf =  new char[malloc_len]();

    *((int*)buf) = (int)1314;    
    *((int*)(buf+ sizeof(int))) = this->printer_no;
    *((int*)(buf+ 2*sizeof(int))) = (int)FileRequest;
    *((int*)(buf+ 3*sizeof(int))) = malloc_len;
    memcpy(buf + 4*sizeof(int), file_url, strlen(file_url) + 1);

    MD5_CTX md5;  
    MD5Init(&md5);  
    MD5Update(&md5, (unsigned char*)buf, malloc_len - MD5_LEN);  
    MD5Final(&md5, (unsigned char*)buf + malloc_len - MD5_LEN);     

    this->net->xsend(buf, malloc_len);

    delete buf;
    return 0;
}

int CClientConnect::recv_file(const char *file_url, int *schedule)
{
    int file_size;
    this->net->xrecv((char*)(&file_size), sizeof(int));
    char *file_buf = new char[file_size + MD5_LEN]();

    this->net->xrecv(file_buf, file_size, schedule, 1);
    int filefd = creat(file_url, 0777);
    if(-1 == filefd)
    {
        perror("creat");
        return -1;
    }    
    int write_len = file_size;
    int ret;
    char *temp_buf = file_buf;
    while(write_len != 0 && (ret = write(filefd, temp_buf, write_len)) != 0)
    {
        if(ret == -1)
        {
            if(EINTR == errno)
                continue;
            perror("write");
            return -1;
        }
        write_len -= ret;
        temp_buf += ret;
    }
    delete file_buf;
    close (filefd); 
    return 0;
}

int CClientConnect::request_push_complete(int64_t fno)
{
    //The four int are start_num(1314) + printer_no, request_type and sned_length respectively
    int malloc_len = sizeof(int) + sizeof(int) + sizeof(int) + sizeof(int) \
                    + 0 + MD5_LEN;
    char *buf =  new char[malloc_len]();

    *((int*)buf) = (int)1314;
    *((int*)(buf + sizeof(int))) = this->printer_no;
    *((int*)(buf + 2*sizeof(int))) = (int)PushRequest;
    *((int*)(buf + 3*sizeof(int))) = malloc_len;

    MD5_CTX md5;  
    MD5Init(&md5);  
    MD5Update(&md5, (unsigned char*)buf, malloc_len - MD5_LEN);  
    MD5Final(&md5, (unsigned char*)buf + malloc_len - MD5_LEN);     

    this->net->xsend(buf, malloc_len);

    int push_type = CompletePrint;
    this->net->xsend((char*)&push_type, sizeof(push_type));
    this->net->xsend((char*)&fno, sizeof(fno));
    
    delete buf;
    return 0;
}

int CClientConnect::request_time_and_adjust(void)
{
    //The four int are start_num(1314) + printer_no, request_type and sned_length respectively
    int malloc_len = sizeof(int) + sizeof(int) + sizeof(int) + sizeof(int) \
                     + MD5_LEN;
    char *buf =  new char[malloc_len]();

    *((int*)buf) = (int)1314;    
    *((int*)(buf+ sizeof(int))) = this->printer_no;
    *((int*)(buf+ 2*sizeof(int))) = (int)TimeRequest;
    *((int*)(buf+ 3*sizeof(int))) = malloc_len;

    MD5_CTX md5;  
    MD5Init(&md5);  
    MD5Update(&md5, (unsigned char*)buf, malloc_len - MD5_LEN);  
    MD5Final(&md5, (unsigned char*)buf + malloc_len - MD5_LEN);     
    this->net->xsend(buf, malloc_len);
    delete buf;

    long long current_time;
    this->net->xrecv((char*)&current_time, sizeof(long long));
    if(stime((time_t*)(&current_time)))
    {
        perror("stime");
        return -1;
    }
    cout << "adjust time as : " << time(NULL) << endl;
    
    return 0;
}

