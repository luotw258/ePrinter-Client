#ifndef __CLIENT_CONNECT_H_
#define __CLIENT_CONNECT_H_

#include "client_net.h"
#include "order_info.h"
#include "md5.h"
#include <time.h>

#define OrderResuest (0)
#define FileRequest (1)
#define PushRequest (2)
#define TimeRequest (3)

#define CompletePrint (0)

class CClientConnect
{
public:
    CClientConnect();
    ~CClientConnect();
    CClientConnect(int printer_no, CClientNet *net);
    CClientConnect(CClientConnect &connect);

public:
    int request_order(OrderInfo *order);
    int request_file(const char *file_url);
    int recv_file(const char *file_url, int *schedule = NULL);
    int request_push_complete(int64_t fno);
    int request_time_and_adjust(void);

private:
    int printer_no;
    CClientNet *net;
};

#endif //!__CLIENT_CONNECT_H_