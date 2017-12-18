#ifndef __PRINT_QUEUE_H_
#define __PRINT_QUEUE_H_

#include <iostream>
#include <string>
#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include "order_info.h"
#include "time.h"
#include "string.h"
#include <sys/mman.h>
using std::cout;
using std::endl;
using std::string;

#define MIN_SIZE (10)
#define QUEUE_CAPACITY (35000)
#define QUEUE_FILE_PATH \
("/home/ePrinter/print_queue_file/print_queue.queue")
typedef struct element
{
    OrderInfo order;
    char file_url[256];
    int download_schedule;
    time_t time_start_print;
    time_t time_finish_print;
    int job_id;
    int job_state;
} ElementType;

typedef struct 
{
	uint32_t *Capacity;
	uint32_t *Size;
	ElementType *Element;
} HeapStruct;
typedef HeapStruct *PriorityQueue;
typedef uint32_t Index;

class CPrintQueue
{
public:
    CPrintQueue();
    ~CPrintQueue();
    
public:
    Index insert(ElementType X);
    ElementType delete_min(void);
    ElementType find_min(void);
    bool is_empty(void);
    bool is_full(void);
    void print_all_job(void);
    int update_queue_file(void);
    int *get_schedule_point(int fno);
    
private:
    PriorityQueue print_queue;
    ElementType MIN_DATA;
    char *mmap_point;
private:
    PriorityQueue initialize();
    void make_empty(void);

    bool a_more_than_b(ElementType a, ElementType b);
    bool a_less_than_b(ElementType a, ElementType b);
    int xwrite(int fd, void *buf, int len);
    int xread(int fd, void *buf, int len);
    
    
    //“下滤”函数，将 堆A 中第i元素下滤;
    void PerDown(ElementType A[], uint64_t i, uint64_t N);
    void Swap(ElementType *Ele1, ElementType *Ele2);
    void HeapSort(ElementType A[], uint64_t N);
};


#endif //!__PRINT_QUEUE_H_
