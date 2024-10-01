#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <stdint.h>


void* print_message(void* arg)
{
    char* message = (char *)arg;
    for (int i = 0; i < 5; ++i) {
        printf("%s\n", message);
        sleep(1);
    }
   return NULL;
}


int main()
{
    pthread_t thread1, thread2;

    pthread_create(&thread1, NULL, print_message, "Hayati");
    pthread_create(&thread2, NULL, print_message, "Irem");

    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    printf("Program Tamamlandi...\n");


    return 0;
}

