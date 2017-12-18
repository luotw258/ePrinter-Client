#include <iostream>
#include "print_queue.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
using std::cout;
using std::endl;

int main(void)
{
    ElementType x;
    CPrintQueue test_queue;
    cout << sizeof(x) << endl << sizeof(test_queue) << endl;
    while(1)
    {
        cout << test_queue.delete_min().order.fuploadtime << endl;
        sleep(10);
    }
    return 0;
}
