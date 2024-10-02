#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <stdint.h>


void* led1_control(void* arg)
{
    while (1) {
        printf("Led1 on!!\n");
        sleep(1);
        printf("Led1 off!!\n");
        sleep(1);
    }
}

void* led2_control(void* arg)
{
    while (1) {
        printf("Led2 on!!\n");
        sleep(2);
        printf("Led2 off!!\n");
        sleep(2);
    }
}



int main()
{
    pthread_t thread1, thread2;

    pthread_create(&thread1, NULL, led1_control, NULL);
    pthread_create(&thread2, NULL, led2_control, NULL);

    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    return 0;
}

