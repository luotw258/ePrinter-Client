#include "print_queue.h"

CPrintQueue::CPrintQueue()
{
    strcpy(this->MIN_DATA.order.fuploadtime, "1996-11-21 13:14:00");
    strcpy(this->MIN_DATA.order.uphone, "18292877903");
    strcpy(this->MIN_DATA.order.fname, "This is the head of print queue!");
    this->MIN_DATA.order.fplantime = -1;
    this->MIN_DATA.download_schedule = 100;
    this->print_queue = this->initialize();
	this->print_all_job();
    sleep(5);
}
CPrintQueue::~CPrintQueue()
{
    this->make_empty();
    if(-1 == munmap(this->print_queue->Element, \
                    sizeof(uint32_t)*2 + \
                    (QUEUE_CAPACITY + 1)*sizeof(ElementType)));
    {
        perror("munmap");
    }
}

bool CPrintQueue::a_more_than_b(ElementType a, ElementType b)
{
    struct tm tm_a, tm_b;  
    time_t t_a, t_b;
    strptime(a.order.fuploadtime, "%Y-%m-%d %H:%M:%S", &tm_a); //将字符串转换为tm时间  
    strptime(b.order.fuploadtime, "%Y-%m-%d %H:%M:%S", &tm_b); //将字符串转换为tm时间  
    tm_a.tm_isdst = -1;  
    tm_b.tm_isdst = -1;  
    t_a  = mktime(&tm_a); //将tm时间转换为秒时间
    t_b  = mktime(&tm_b); //将tm时间转换为秒时间

    if(a.order.fplantime == b.order.fplantime)
    {
        return t_a > t_b;
    }
    return a.order.fplantime > b.order.fplantime;
}
bool CPrintQueue::a_less_than_b(ElementType a, ElementType b)
{
    struct tm tm_a, tm_b;  
    time_t t_a, t_b;
    strptime(a.order.fuploadtime, "%Y-%m-%d %H:%M:%S", &tm_a); //将字符串转换为tm时间  
    strptime(b.order.fuploadtime, "%Y-%m-%d %H:%M:%S", &tm_b); //将字符串转换为tm时间  
    tm_a.tm_isdst = -1;  
    tm_b.tm_isdst = -1;  
    t_a  = mktime(&tm_a); //将tm时间转换为秒时间
    t_b  = mktime(&tm_b); //将tm时间转换为秒时间

    if(a.order.fplantime == b.order.fplantime)
    {
        return t_a < t_b;
    }
    return a.order.fplantime < b.order.fplantime;
}

PriorityQueue CPrintQueue::initialize(void)
{
    PriorityQueue H = (PriorityQueue)malloc(sizeof(HeapStruct));
	if (NULL == H)
	{
		printf("Out of space!\r\n");
		return NULL;
	}
    	
    if(-1 == access(QUEUE_FILE_PATH, F_OK))
    {
        //如果不存在队列文件则创建
        int fd = open(QUEUE_FILE_PATH, O_RDWR | O_CREAT, 0777);
        if(-1 == fd)
        {
            perror("creat");
        }
        char* temp = \
        new char[2*sizeof(uint32_t) + \
        (QUEUE_CAPACITY + 1)*sizeof(ElementType)]();
        
        this->xwrite(fd, \
                     temp, \
                     2*sizeof(uint32_t) + \
                    (QUEUE_CAPACITY + 1)*sizeof(ElementType));
        delete temp;

        
        this->mmap_point = \
        (char*)mmap(0, \
        2*sizeof(uint32_t) + (QUEUE_CAPACITY+1)*sizeof(ElementType), \
        PROT_WRITE | PROT_READ, MAP_SHARED, fd, 0);
        if(MAP_FAILED == H->Capacity)
        {
            perror("mmap");
            cout << H->Capacity << endl;
            return NULL;
        }
        close(fd);

        H->Capacity = (uint32_t*)this->mmap_point;
        H->Size = (uint32_t*)((char*)this->mmap_point + sizeof(uint32_t));
        H->Element = (ElementType*)((char*)this->mmap_point + 2*sizeof(uint32_t));

        *H->Capacity = QUEUE_CAPACITY;
        *H->Size = 0;
        H->Element[0] = MIN_DATA;

        return H;
    }
    else
    {
        int fd = open(QUEUE_FILE_PATH, O_RDWR);
        if(-1 == fd)
        {
            perror("open");
        }

        this->mmap_point = \
        (char*)mmap(0, \
        2*sizeof(uint32_t) + (QUEUE_CAPACITY+1)*sizeof(ElementType), \
        PROT_WRITE | PROT_READ, MAP_SHARED, fd, 0);
        if(MAP_FAILED == H->Capacity)
        {
            perror("mmap");
            cout << H->Capacity << endl;
            return NULL;
        }
        close(fd);

        H->Capacity = (uint32_t*)this->mmap_point;
        H->Size = (uint32_t*)((char*)this->mmap_point + sizeof(uint32_t));
        H->Element = (ElementType*)((char*)this->mmap_point + 2*sizeof(uint32_t));
        close(fd);
        return H;
    }
    cout << "initialize complete! " << H << endl;
}
void CPrintQueue::make_empty(void)
{
    if (NULL != this->print_queue)
	{
		free(this->print_queue->Element);
		free(this->print_queue);
	}
}
Index CPrintQueue::insert(ElementType X)
{
    if (this->is_full())
	{
		printf("Priority queue is full!\r\n");
		return -1;
	}
    this->print_queue->Element[*this->print_queue->Size + 1] = X;
    (*this->print_queue->Size)++;
    this->HeapSort(this->print_queue->Element, *this->print_queue->Size + 1);
	return 0;
}
ElementType CPrintQueue::delete_min(void)
{
   	if (this->is_empty())
	{
		printf("Priority queue is empty!\r\n");
		return this->MIN_DATA;
	}
	ElementType MinElement = this->print_queue->Element[1];
	for(uint32_t i = 1; i < (*this->print_queue->Size); i++)
	{
	    this->print_queue->Element[i] = this->print_queue->Element[i+1];
	}
	(*this->print_queue->Size)--;
	return MinElement;
}
ElementType CPrintQueue::find_min(void)
{
    return this->print_queue->Element[1];
}
bool CPrintQueue::is_empty(void)
{
    return 0 == (*this->print_queue->Size);
}
bool CPrintQueue::is_full(void)
{
    return (*this->print_queue->Capacity) == (*this->print_queue->Size);
}

void CPrintQueue::print_all_job(void)
{
    cout << "********************************************" << endl;
    cout << "total " << *this->print_queue->Size << " jobs." << endl;
    for(uint32_t i = 0; i < (1 + *this->print_queue->Size); i++)
    {
        cout << i << " : " << this->print_queue->Element[i].order.uphone \
        << "--" << this->print_queue->Element[i].order.fuploadtime \
        << "--" << this->print_queue->Element[i].order.fplantime \
        << "--" << this->print_queue->Element[i].file_url << endl;
    }
    cout << "********************************************" << endl;
}


#define LeftChild(i) (2*(i)+1) //与二叉堆的不同之处在于,二叉堆序号以1开始，而此处以0开始，所以左孩子是2*i+1

void CPrintQueue::PerDown(ElementType A[], uint64_t i, uint64_t N)	//“下滤”函数，将 堆A 中第i元素下滤
{
	uint64_t Child; //该变量相当于堆中“空穴”位置
	ElementType Tmp;

	//判断条件很重要！！！
	for (Tmp = A[i]; LeftChild(i) < N; i = Child) //判断左孩子是否存在 
	{
		Child = LeftChild(i);
		//这里的判断条件很重要！！！
		if (Child != N - 1 && \
            this->a_more_than_b(A[Child + 1], A[Child])\
            )//判断顺序一定不能反过来，前一个判断确定是否有右孩子
			Child++;
		if (this->a_less_than_b(Tmp, A[Child]))	//将较大的值往上挪
			A[i] = A[Child];
		else
			break;
	}
	A[i] = Tmp;
}

//交换元素 
void CPrintQueue::Swap(ElementType *Ele1, ElementType *Ele2) 
{
	ElementType Tmp;
	Tmp = *Ele1;
	*Ele1 = *Ele2;
	*Ele2 = Tmp;
}
void CPrintQueue::HeapSort(ElementType A[], uint64_t N)
{
	for (int64_t i = N / 2; i >= 0; i--) //构建二叉堆
	{
		this->PerDown(A, i, N);
	}
	for (uint64_t i = N - 1; i > 0; i--)	 
	{
		this->Swap(&A[0], &A[i]); //直接利用堆的内存空间，这样做节省内存
		this->PerDown(A, 0, i); //依次将最大值放在位置“0”处
	}
}

int CPrintQueue::xwrite(int fd, void *buf, int len)
{
    int ret;
    while(len != 0 && (ret = write(fd, buf, len)) != 0)
    {
        if(-1 == ret)
        {
            if(EINTR == errno)
                continue;
            perror("write in update_queue_file");
            break;
        }
        len -= ret;
        buf += ret;
    }
    return 0 == len;
}

int CPrintQueue::xread(int fd, void *buf, int len)
{
    int ret;
    while(len != 0 && (ret = read(fd, buf, len)) != 0)
    {
        if(-1 == ret)
        {
            if(EINTR == errno)
                continue;
            perror("read in update_queue_file");
            break;
        }
        len -= ret;
        buf += ret;
    }
    return 0 == len;
}

int CPrintQueue::update_queue_file(void)
{
    if(-1 == msync(this->mmap_point, \
        2*sizeof(uint32_t) + ((*this->print_queue->Size)+1)*sizeof(ElementType), \
        MS_SYNC))
    {
        perror("msync");
        return -1;
    }
    return 0;
}

int *CPrintQueue::get_schedule_point(int fno)
{
    for(uint32_t i = 0; i <= (*this->print_queue->Size); i++)
    {
        if(fno == (this->print_queue->Element[i].order.fno))
        {
            return (&(this->print_queue->Element[i].download_schedule));
        }
    }
    return NULL;
}