#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <stdint.h>


uint8_t seconds = 0;
uint8_t minutes = 0;
pthread_mutex_t lock;

void* count_seconds(void* arg)
{
    while (1) {
        pthread_mutex_lock(&lock);
        seconds++;
        if (seconds == 60) {
            seconds = 0;
        }
        printf("Minutes : %zu   Seconds : %zu\n", minutes, seconds);
        pthread_mutex_unlock(&lock);
        sleep(1);
    }
}

void* count_minutes(void* arg)
{
    while (1) {
        pthread_mutex_lock(&lock);
        if (seconds == 0) {
            minutes++;
            if (minutes == 60) {
                minutes = 0;
            }
        }
        pthread_mutex_unlock(&lock);
        sleep(1);
    }
}



int main()
{
    pthread_t thread1, thread2;
    pthread_mutex_init(&lock, NULL);

    pthread_create(&thread1, NULL, count_seconds, NULL);
    pthread_create(&thread2, NULL, count_minutes, NULL);

    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    pthread_mutex_destroy(&lock);
    return 0;
}

