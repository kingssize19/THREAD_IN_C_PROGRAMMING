#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <stdint.h>
#include <windows.h>


HANDLE hComm;                                                       // Seri port için handle
pthread_mutex_t lock;                                               // Mutex kilidi


// Seri port üzerinden veri gönderme fonksiyonu
void* uart_write_thread(void* arg) {

    char message[256];
    DWORD dwBytesWritten = 0;

    while (1) {

        //Kullanıcıdan veri alma islemi
        printf("Gondermek istediginiz veriyi yazin : ");
        fgets(message, sizeof(message), stdin);                 //Kullanıcıdan veri oku

        //Satır sonu karakterini sil
        size_t len = strlen(message);
        if (len > 0 && message[len - 1] == '\n') {
            message[len - 1] = '\0';
        }

        pthread_mutex_lock(&lock);

        //Veri gonderme islemi
        if (!WriteFile(hComm, message, strlen(message), &dwBytesWritten, NULL)) {
            printf("Veri Gonderme Hatasi\n");
        }
        else {
            printf("Gonderilen veri : %s\n", message);
        }
        pthread_mutex_unlock(&lock);
    }

    return NULL;
}

//Seri port üzerinden veri okuma fonskiyonu

void* uart_read_thread(void* arg) {

    char buffer[256];
    DWORD dwBytesRead = 0;

    while (1) {
        pthread_mutex_lock(&lock);

        //Veri okuma işlemi
        if (ReadFile(hComm, buffer, sizeof(buffer) - 1, &dwBytesRead, NULL)) {
            if (dwBytesRead > 0) {
                buffer[dwBytesRead] = '\0';             //Alınan veriyi sonlandır.
                printf("Alinan veri : %s\n", buffer);
            }
        }
            else {
                printf("Veri okuma hatasi\n");
            }
            pthread_mutex_unlock(&lock);
            Sleep(500);
    }
    return NULL;
}


//COM portunu ayarlama ve açma
uint8_t setup_uart(const char* com_port) {

    hComm = CreateFile(com_port, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);

    if (hComm == INVALID_HANDLE_VALUE) {
        printf("COM portu acilmadi.\n");
        return -1;
    }

    //Seri port yapılandırması
    DCB dcbSerialParams = {0};
    dcbSerialParams.DCBlength = sizeof(dcbSerialParams);
    if (!GetCommState(hComm, &dcbSerialParams)) {
        printf("Seri port durumu okunamadi.\n");
        return -1;
    }

    dcbSerialParams.BaudRate = CBR_9600;
    dcbSerialParams.ByteSize = 8;
    dcbSerialParams.StopBits = ONESTOPBIT;
    dcbSerialParams.Parity = NOPARITY;

    if (!SetCommState(hComm, &dcbSerialParams)) {
        printf("Seri port durumu ayarlanmadi.\n");
    }

    //Zamanlama Ayarları
    COMMTIMEOUTS timeouts = {0};
    timeouts.ReadIntervalTimeout = 50;
    timeouts.ReadTotalTimeoutConstant = 50;
    timeouts.ReadTotalTimeoutMultiplier = 10;
    timeouts.WriteTotalTimeoutConstant = 50;
    timeouts.WriteTotalTimeoutMultiplier = 10;

    if (!SetCommTimeouts(hComm, &timeouts)) {
        printf("Seri port zaman asimi ayarlanmadi.\n");
        return -1;
    }
    return 0;
}


int main() {

    pthread_t write_thread, read_thread;

    //Com portu yapılandır.
    if (setup_uart("\\\\.\\COM3") != 0) {
        return -1;
    }

    pthread_mutex_init(&lock, NULL);

    pthread_create(&write_thread, NULL, uart_write_thread, NULL);
    pthread_create(&read_thread, NULL, uart_read_thread, NULL);

    pthread_join(write_thread, NULL);
    pthread_join(read_thread, NULL);

    pthread_mutex_destroy(&lock);

    CloseHandle(hComm);
    return 0;

}




