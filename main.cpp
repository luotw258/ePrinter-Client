#include "client_net.h"
#include "client_connect.h"
#include <iostream>
#include <string>
#include "order_info.h"
#include "print_queue.h"

using std::string;
using std::to_string;
using std::cout;
using std::endl;

void *printer_pthread(void *message);

int main(int argc, char *argv[])
{
    int printer_no = argc == 2 ? atoi(argv[1]) : 0;
    
    CClientNet net("139.199.229.138", 1314);
    net.connect_to_server();

    CClientConnect connection(printer_no, &net);
    OrderInfo order;
    CPrintQueue print_queue;

    int cnt = 0;
    string save_url;
    ElementType one_job;

    pthread_t print_pthread;
    void *message_queue[2] = {(void*)(&print_queue), \
                              (void*)(&connection)};
    cout << message_queue[0] << endl << \
        message_queue[1] << endl << \
        message_queue << endl;
    pthread_create(&print_pthread, NULL, \
                    printer_pthread, (void*)message_queue);
    while(1)
	{
		order.fno = -1;
		connection.request_order(&order);
		if(-1 == order.fno)
		{
			cout << printer_no << " did not recieved order!(" << ++cnt << ")\n";
			sleep(5);
			continue;
		}
		
		connection.request_time_and_adjust();
		cout << printer_no << " : " << order.fname << endl;
		
		connection.request_file(order.furl);
		
		save_url = "/home/ePrinter/user_file/";
		save_url += to_string(order.fno) + "-" + order.uphone + "-" + order.fname;
		cout << save_url <<endl;

        one_job.order = order;
		strcpy(one_job.file_url, save_url.c_str());

		one_job.download_schedule = 50;
		print_queue.insert(one_job);
		cout << "one job has beel inserted!" << endl;

		//插入后打印队列有变化，更新
		if(print_queue.update_queue_file())
            cout << "update_queue_file failed!" << endl;
        else
            cout << "update_queue_file complete!" <<endl;
		connection.recv_file(save_url.c_str(), \
		            print_queue.get_schedule_point(one_job.order.fno));
	}
    return 0;
}
