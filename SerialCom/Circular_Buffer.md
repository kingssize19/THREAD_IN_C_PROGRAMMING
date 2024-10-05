**Bu programdaki buffer (tampon bellek), gelen veriyi okuma sırasında geçici olarak depolamak için kullanılır. Ancak bu buffer'ın kendisi, yalnızca programın her okuma döngüsünde gelen veriyi saklar. Bu nedenle, veri alma sıklığını azaltırsanız, veri kaybı riski ortaya çıkar, çünkü işletim sistemi ve seri portun sürücüsü belirli bir tampon belleğe sahiptir ve bu tampon dolarsa eski veriler üzerine yazılabilir. Seri porttan sürekli gelen veri okuma hızınıza bağlıdır; bu hız yeterli değilse, veriler kaybolabilir.**

### Seri Port ve Buffer'ın Çalışma Mantığı :
* **Seri Port Tamponu :** Seri port sürücüsünde bir tampon bulunur. Bu tampon, işletim sisteminin gelen verileri okuyabilmesi için kısa süreliğine verileri saklar. Ancak bu tampon sınırlı büyüklüktedir. Eğer program, bu tamponu yeterince hızlı boşaltmazsa (yani, veriyi yeterince hızlı okuyup işlemezse), tampon taşar ve eski veriler üzerine yazılır. Bu da veri kaybına neden olur.

* **Programdaki Buffer :** Programda kullandığımız buffer dizisi, her okuma döngüsünde seri porttan gelen veriyi alır ve bu veriyi ekrana yazar. Ancak bu buffer, yalnızca okuma işlemi sırasında geçerli olan bir tampondur. Yani, her okuma işlemi tamamlandığında buffer içeriği güncellenir. Eğer seri porttan gelen veriyi almazsanız, sistem tamponu taşabilir ve bu da verilerin kaybolmasına neden olabilir.

* Bu senaryoda, okuma sıklığını azaltırsak, veriler işletim sisteminin seri port tamponunda birikir. Fakat bu tampon taşarsa, eski veriler kaybolabilir. Örneğin, Sleep(500) gibi bir gecikme eklediğinizde, her 500 ms'de bir veri okursunuz. Eğer gelen veri hızı, bu okuma hızını aşarsa sistem tamponu taşacak ve veri kaybı yaşanacaktır.

* ### Buffer ile Veri Kaybını Önlemek için Çözümler :

* **1. Daha Büyük Bir Buffer Kullanın :** Seri porttan gelen veriyi kendi tamponunuza almak için daha büyük bir buffer yapısı kullanabilirsiniz. Gelen veriler her okuma döngüsünde birikerek bu buffer'a kaydedilir. Ancak bu tampon yine dolabilir ve veri kaybı yaşanabilir. Dolayısıyla tampon büyüklüğü dikkatli seçilmelidir.
* **2. Daha Sık Veri Okuyun :** Okuma sıklığını artırarak veri kaybını önleyebiliriz. Seri porttan daha sık veri okuyarak sistem tamponunun dolmasını engelleriz.
* **3. İkili (Double) Buffer Yapısı :** Bu yöntemle, iki buffer kullanarak bir tampon dolarken diğeri işlenir. Bir tampon dolduğunda işlemci bu tamponu işlerken diğer tampon veri almaya devam eder. Bu yöntem tampon taşmasını engelleyebilir.
* **4. İşlemciye Bağımsız Çalışan Buffer Yapısı (Ring Buffer) :** Bu tür bir yapı, sürekli veri akışı olan durumlarda tampon belleğin taşmasını engeller. Gelen veriler dairesel bir tampon (ring buffer) yapısında tutulur. Eğer tampon dolarsa, en eski veri üzerine yazılır ve böylece tampon taşması bir ölçüde engellenir.


## Örnek Uygulama: Büyük Buffer ile Veri Depolama

**Aşağıda, verilerin buffer'a alınması ve ekrana yazdırılması sıklığını azaltan, ancak buffer sayesinde verilerin birikmesini sağlayan bir program örneği: 

```c
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
```


### Programın Özellikleri : 
**1. circularBuffer :** 1024 byte'lık bir tamponda gelen verileri saklar.

**2. writePos ve readPos :** Tampondaki yazma ve okuma pozisyonlarını kontrol eder.

**3. Ring Buffer Yapısı :** Tampon dolduğunda en eski veri üzerine yazılarak tampon taşması engellenir.

**4. Veri Gönderme ve Alma :** İki ayrı iş parçacığı (thread) veri gönderip alır. Bir iş parçacığı sürekli veri alırken, diğeri tampondaki veriyi ekrana yazdırır.

**Bu yapı, veri okuma ve ekrana yazma işlemlerini ayrı tutarak tamponun taşmasını önler ve veri kaybını minimize eder.**
