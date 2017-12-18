#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>  
#include <sys/stat.h>
#include <fcntl.h>

#define SIZE_PARAM (1.9073486328125f)
#define PAGES_PARAM (5000000.0f)

int main(int argc, char *argv[])
{
    if(4 != argc)
    {
        fprintf(stderr, "use arg : print file_url start_page end_page\n");
        return -1;
    }
    struct stat sb;
    int ret = stat(argv[1], &sb);
    if (ret) 
    {
        perror("stat");
        return -1;
    }
    int file_size = (int)sb.st_size;
    int num_need_printed_pages = atoi(argv[3]) - atoi(argv[2]) + 1;
    unsigned int usec = (unsigned int)(SIZE_PARAM * (float)(file_size) + \
                        PAGES_PARAM * (float)(num_need_printed_pages));
    fprintf(stderr, "sleep sec : %f s\n", (float)usec / 1000000.0f);
    usleep(usec);
    fprintf(stderr, "complete printing\n");
    return 0;
}
