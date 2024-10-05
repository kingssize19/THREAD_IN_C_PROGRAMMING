#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <stdint.h>
#include <windows.h>

#define BUFFER_SIZE     1024                    //Buffer Büyüklüğü

HANDLE hComm;                                   //Seri Port için handle
pthread_mutex_t lock;                           //Mutex kilidi

char circularBuffer[BUFFER_SIZE];               //Gelen veriler için dairesel buffer
uint8_t writePos = 0;                           //Yazma Pozisyonu
uint8_t readPos = 0;                            //Okuma pozisyonu

//Veri okuma pozisyonunu dairesel olarak ayarlayan fonksiyon.
uint8_t get_next_pos(uint8_t currentPos)
{
    return (currentPos + 1) % BUFFER_SIZE;
}


//Seri port üzerinden veri gönderme fonksiyonu
void* uart_write_thread(void* arg)
{
    char message[256];
    DWORD dwBytesWritten = 0;

    while (1) {

        printf("Gondermek istediginiz veriyi yazin : ");
        fgets(message, sizeof(message), stdin);

        //Satır sonu karakterini silin
        size_t len = strlen(message);
        if (len > 0 && message[len - 1] == '\n') {
            message[len - 1] = '\0';
        }

        pthread_mutex_lock(&lock);

        //Veri gönderme işlemi
        if (!WriteFile(hComm, message, strlen(message), &dwBytesWritten, NULL)) {
            printf("Veri Gonderme Hatasi\n");
        }
        else {
            printf("Gonderilen Veri : %s\n", message);
        }

        pthread_mutex_unlock(&lock);

    }

    return NULL;

}

//Seri port üzerinden veri okuma fonksiyonu
void* uart_read_thread(void* arg)
{
    char buffer[256];
    DWORD dwBytesRead = 0;

    while (1) {

        pthread_mutex_lock(&lock);

        //Veri okuma işlemi
        if (ReadFile(hComm, buffer, sizeof(buffer) - 1, &dwBytesRead, NULL))
        {
            if (dwBytesRead > 0)
            {
                buffer[dwBytesRead] = '\0';         //Alınan veriyi sonlandır.

                //Gelen veriyi dairesel tamponda sakla
                for (DWORD i = 0; i < dwBytesRead; ++i)
                {
                    circularBuffer[writePos] = buffer[i];
                    writePos = get_next_pos(writePos);

                    //Eğer yazma pozisyonu okuma pozisyonunu geçerse veri kaybı olur
                    if (writePos == readPos)
                    {
                        printf("Veri kaybi : Buffer tasma\n");
                        readPos = get_next_pos(readPos);
                    }
                }
            }
        }

        else
        {
           printf("Veri Okuma Hatasi\n");
        }

        pthread_mutex_unlock(&lock);
        Sleep(500);
    }
    return NULL;
}


//Tampondaki verileri ekrana yazdıran fonksiyon
void* display_buffer_thread(void* arg)
{
    while (1)
    {
        pthread_mutex_lock(&lock);

        //Dairesel tamponda veri varsa ekrana yazdır.
        while (readPos != writePos)
        {
            printf("%c", circularBuffer[readPos]);
            readPos = get_next_pos(readPos);
        }

        pthread_mutex_unlock(&lock);
        Sleep(1000);
    }
    Sleep(1000);
}


//COM Portunu ayarlama ve açma
uint8_t setup_uart(const char* com_port)
{
    hComm = CreateFile(com_port, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);

    if (hComm == INVALID_HANDLE_VALUE) {
        printf("COM portu acilmadi.\n");
        return -1;
    }

    //Seri port yapılandırılması
    DCB dcbSerialParams = {0};
    dcbSerialParams.DCBlength = sizeof(dcbSerialParams);

    if (!GetCommState(hComm, &dcbSerialParams))
    {
        printf("Seri port durumu okunmadi\n");
        return -1;
    }

    dcbSerialParams.BaudRate = CBR_9600;
    dcbSerialParams.ByteSize = 8;
    dcbSerialParams.StopBits = ONESTOPBIT;
    dcbSerialParams.Parity = NOPARITY;

    if (!SetCommState(hComm, &dcbSerialParams))
    {
        printf("Seri port durumu ayarlanmadi\n");
    }

    //Zaman aşımı ayarları
    COMMTIMEOUTS timeouts = {0};
    timeouts.ReadIntervalTimeout = 50;
    timeouts.ReadTotalTimeoutConstant = 50;
    timeouts.ReadTotalTimeoutMultiplier = 10;
    timeouts.WriteTotalTimeoutConstant = 50;
    timeouts.WriteTotalTimeoutMultiplier = 10;

    if (!SetCommTimeouts(hComm, &timeouts))
    {
        printf("Seri port zaman asimi ayarlanmadi.\n");
        return -1;
    }

    return 0;
}

int main() {

    pthread_t write_thread, read_thread, display_thread;

    if (setup_uart("\\\\.\\COM3") != 0)
    {
        return -1;
    }

    pthread_mutex_init(&lock, NULL);

    pthread_create(&write_thread, NULL, uart_write_thread, NULL);
    pthread_create(&read_thread, NULL, uart_read_thread, NULL);
    pthread_create(&display_thread, NULL, display_buffer_thread, NULL);

    pthread_join(write_thread, NULL);
    pthread_join(read_thread, NULL);
    pthread_join(display_thread, NULL);

    CloseHandle(hComm);

    pthread_mutex_destroy(&lock);

    return 0;
}


