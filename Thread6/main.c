#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <stdint.h>
#include <time.h>

pthread_mutex_t lock;
uint8_t sensor_data1;
uint8_t sensor_data2;
uint16_t total_data;



uint8_t random_value1(void)
{

    return rand() % 256 + 1;
}

uint8_t random_value2(void)
{

    return rand() % 256 + 1;
}

void* read_sensor1(void* arg)
{
    while (1) {

        srand(time(NULL));
        pthread_mutex_lock(&lock);
        sensor_data1 = random_value1();
        printf("Sensor Data (1) : %zu\n", sensor_data1);
        pthread_mutex_unlock(&lock);
        sleep(1);
    }
}

void* read_sensor2(void* arg)
{

    while (1) {

        pthread_mutex_lock(&lock);
        sensor_data2 = random_value2();
        printf("Sensor Data (2) : %zu\n", sensor_data2);
        pthread_mutex_unlock(&lock);
        sleep(1);
    }
}

void* process_data(void* arg)
{
    while (1) {

        pthread_mutex_lock(&lock);
        total_data = sensor_data1 + sensor_data2;
        printf("Total Data : %zu\n", total_data);
        pthread_mutex_unlock(&lock);
        sleep(1);
    }

}


int main()
{
    pthread_t thread1, thread2, thread3;

    pthread_mutex_init(&lock, NULL);

    pthread_create(&thread1, NULL, read_sensor1, NULL);
    pthread_create(&thread2, NULL, read_sensor2, NULL);
    pthread_create(&thread3, NULL, process_data, NULL);

    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);
    pthread_join(thread3, NULL);

    pthread_mutex_destroy(&lock);
    return 0;
}

