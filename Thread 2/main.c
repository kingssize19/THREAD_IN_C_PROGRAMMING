#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <stdint.h>


volatile uint8_t stop = 0;      //Tüm iş parçacıklarının erişebileceği global bayrak (flag)


void* func1()
{
    while (!stop) {
        printf("func1 \n");
        sleep(1);
    }

    printf("func1 sonlandi. \n");
    return NULL;
}

void* func2()
{

    while (!stop) {
        printf("func2 \n");
        sleep(1);
    }

    printf("func2 sonlandi. \n");
    return NULL;
}

int main()
{
    pthread_t t1, t2;


    pthread_create(&t1, NULL, func1, NULL);
    pthread_create(&t2, NULL, func2, NULL);

    uint8_t input;
    while (1) {
        printf("Devam etmek icin 1, Durdurmak icin 0 girin : \n");
        scanf("%zu", &input);

        if (input == 0) {
            stop = 1;
            break;
        }
    }

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    printf("Program Sonlandi...");

    return 0;
}

