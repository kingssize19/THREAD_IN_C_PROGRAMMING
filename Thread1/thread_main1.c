#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

void func1()
{
    while(1)
    {
        printf("func1 \n");
        sleep(1);
    }
}

void* func2()
{
    while(1)
    {
        printf("func2 \n");
        sleep(1);
    }

}

int main()
{

    pthread_t t1;

    pthread_create(&t1, NULL, func2, NULL);
    func1();
    sleep(1);

    return 0;
}
