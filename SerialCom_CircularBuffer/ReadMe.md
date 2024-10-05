#### Bu program, seri porttan veri alıp gönderme işlemlerini çoklu iş parçacığı (multi-threading) ile gerçekleştiren bir C uygulamasıdır. Gelen veriler bir dairesel tampon (ring buffer) içinde depolanır ve düzenli olarak ekrana yazdırılır. Aynı zamanda, kullanıcı tarafından girilen veriler de seri port üzerinden gönderilir.

### 1. Tampon ve Global Değişkenler
```c
#define BUFFER_SIZE 1024    // Buffer büyüklüğü
HANDLE hComm;               // Seri port için handle
pthread_mutex_t lock;       // Mutex kilidi
char circularBuffer[BUFFER_SIZE];   // Gelen veriler için dairesel tampon
int writePos = 0;           // Yazma pozisyonu
int readPos = 0;            // Okuma pozisyonu
```
* **BUFFER_SIZE :** Dairesel tamponun (circular buffer) büyüklüğü burada 1024 bayt olarak tanımlanmıştır.
* **hComm :** Seri port ile bağlantı kurmak için kullanılan **HANDLE** türünde bir değişken. Bu, seri porttan veri okumak ve yazmak için kullanılır.
* **lock :** Veri bütünlüğünü sağlamak için kullanılan bir mutex kilidi. Aynı anda birden fazla iş parçacığının (thread) tamponu manipüle etmesini önler.
* **circularBuffer :** Gelen verilerin geçici olarak depolandığı buffer. Dairesel bir yapıdadır, bu da tampon dolduğunda en eski verilerin üzerine yazılmasını sağlar.
* **writePos ve readPos :** **writePos**, tamponda yazılacak pozisyonu belirtir. **readPos**, tampondan okunacak pozisyonu takip eder.

### 2. Dairesel Tampon (Circular Buffer) Fonksiyonu 
```c
int get_next_pos(int currentPos) {
    return (currentPos + 1) % BUFFER_SIZE;
}
```
* Bu fonksiyon, tampondaki pozisyonların dairesel olarak artmasını sağlar. Eğer tamponun sonuna ulaşırsanız (BUFFER_SIZE), pozisyon başa döner (% BUFFER_SIZE). Bu, tamponun sonsuz bir döngü gibi çalışmasını sağlar ve "dairesel tampon" mantığı oluşturur.

### 3. Veri Gönderme İş Parçacığı (uart_write_thread)
```c
void* uart_write_thread(void* arg) {
    char message[256];
    DWORD dwBytesWritten = 0;

    while (1) {
        // Kullanıcıdan veri alma
        printf("Gondermek istediginiz veriyi yazin : ");
        fgets(message, sizeof(message), stdin);

        // Satır sonu karakterini sil
        size_t len = strlen(message);
        if (len > 0 && message[len - 1] == '\n') {
            message[len - 1] = '\0';
        }

        pthread_mutex_lock(&lock);

        // Veriyi seri porta yaz
        if (!WriteFile(hComm, message, strlen(message), &dwBytesWritten, NULL)) {
            printf("Veri Gonderme Hatasi\n");
        } else {
            printf("Gonderilen veri : %s\n", message);
        }

        pthread_mutex_unlock(&lock);
    }

    return NULL;
}
```
* **Kullanıcı Girdisi :** Kullanıcıdan bir mesaj alır (**fgets**) ve bu mesajı seri port üzerinden gönderir.
* **Satır Sonu Karakteri :** Kullanıcıdan alınan girdinin sonunda satır sonu karakteri (**\n**) varsa bu karakter silinir, çünkü veri iletilirken bu karakter gereksiz olabilir.
* **Veri Gönderme :** **WriteFile()** fonksiyonu, seri port üzerinden veriyi gönderir. Gönderme sırasında mutex kilidi kullanılır, bu da başka bir iş parçacığının aynı anda tamponu manipüle etmesini engeller.

### 4. Veri Alma İş Parçacığı (uart_read_thread)
```c
void* uart_read_thread(void* arg) {
    char buffer[256];
    DWORD dwBytesRead = 0;

    while (1) {
        pthread_mutex_lock(&lock);

        // Veriyi seri porttan oku
        if (ReadFile(hComm, buffer, sizeof(buffer) - 1, &dwBytesRead, NULL)) {
            if (dwBytesRead > 0) {
                buffer[dwBytesRead] = '\0';  // Alınan veriyi sonlandır
                // Gelen veriyi dairesel tamponda sakla
                for (DWORD i = 0; i < dwBytesRead; i++) {
                    circularBuffer[writePos] = buffer[i];
                    writePos = get_next_pos(writePos);

                    // Eğer yazma pozisyonu okuma pozisyonunu geçerse veri kaybı olur
                    if (writePos == readPos) {
                        printf("Veri kaybi: Buffer tasma\n");
                        readPos = get_next_pos(readPos);  // Eski veriyi üzerine yaz
                    }
                }
            }
        } else {
            printf("Veri okuma hatasi\n");
        }

        pthread_mutex_unlock(&lock);
        Sleep(500);  // Okuma sıklığını azaltmak
    }
    return NULL;
}
```
* **Veri Alma :** **ReadFile()** fonksiyonu seri porttan gelen veriyi okur. Bu veriler bir tamponda saklanır (buffer). Eğer okunan byte sayısı sıfırdan büyükse, veri tamponda depolanır.
* **Circular Buffer :** Gelen veri, dairesel tamponda saklanır. Eğer tampon dolarsa (yani writePos okuma pozisyonunu geçerse), veri kaybı yaşanır ve eski veriler üzerine yazılır. Bu durumda, readPos güncellenir.
* **Veri Kaybı :** Eğer buffer dolarsa ve yeni veri geldiğinde tamponda yer yoksa, eski veriler üzerine yazılacağı için bir veri kaybı mesajı görüntülenir.
* **Kilitleme :** Bu işlem sırasında da mutex kilidi kullanılarak, tamponun aynı anda birden fazla iş parçacığı tarafından kullanılmasının önüne geçilir.

### 5. Tampondaki Verileri Yazdırma İş Parçacığı (display_buffer_thread)
```c
void* display_buffer_thread(void* arg) {
    while (1) {
        pthread_mutex_lock(&lock);

        // Tampondaki verileri ekrana yaz
        while (readPos != writePos) {
            printf("%c", circularBuffer[readPos]);
            readPos = get_next_pos(readPos);
        }

        pthread_mutex_unlock(&lock);
        Sleep(1000);  // Ekrana yazdırma sıklığı
    }
    return NULL;
}
```
* **Buffer'dan Okuma ve Yazdırma :** Bu iş parçacığı, dairesel tampondaki verileri ekrana yazdırır. Okuma pozisyonu, tamponda yazma pozisyonuna ulaşana kadar devam eder.
* **Tampondan Veriyi İşleme :** readPos, her okunan veri sonrasında bir ileri pozisyona kaydırılır.
* **Zaman Aralığı :** Her 1000 ms'de (1 saniye) bir ekrana yazdırma işlemi yapılır. Bu, tampondaki verilerin sürekli ekrana yazılmasını sağlar.

### 6. Seri Port Ayarları (setup_uart)
```c
uint8_t setup_uart(const char* com_port) {
    hComm = CreateFile(com_port, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);

    if (hComm == INVALID_HANDLE_VALUE) {
        printf("COM portu acilmadi.\n");
        return -1;
    }

    // Seri port yapılandırması
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

    // Zaman aşımı ayarları
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
```
* **Seri Port Açma :** **CreateFile()** ile belirlenen COM portu açılır. Eğer port açılmazsa, hata mesajı görüntülenir.
* **Seri Port Yapılandırma :** **DCB** yapı parametreleri ile seri portun baud rate, veri uzunluğu, stop bit ve parity ayarları yapılır. Bu örnekte, 9600 baud hızı, 8 bit veri uzunluğu, 1 stop bit ve parity olmadan çalışacak şekilde yapılandırılmıştır.
* **Zaman Aşımı Ayarları :** **COMMTIMEOUTS** ile okuma ve yazma işlemlerinin zaman aşımı parametreleri ayarlanır.

### 7. main Fonksiyon
```c
int main() {
    pthread_t write_thread, read_thread, display_thread;

    if (setup_uart("\\\\.\\COM5") != 0) {
        return -1;
    }

    pthread_mutex_init(&lock, NULL);

    // İş parçacıklarını oluştur
    pthread_create(&write_thread, NULL, uart_write_thread, NULL);
    pthread_create(&read_thread, NULL, uart_read_thread, NULL);
    pthread_create(&display_thread, NULL, display_buffer_thread, NULL);

    // İş parçacıklarının tamamlanmasını bekle
    pthread_join(write_thread, NULL);
    pthread_join(read_thread, NULL);
    pthread_join(display_thread, NULL);

    CloseHandle(hComm);
    pthread_mutex_destroy(&lock);

    return 0;
}
```
* **Seri Port Ayarları :** Program **setup_uart()** fonksiyonu ile COM portunu açar ve yapılandırır. COM portu burada COM5 olarak ayarlanmıştır.
* **İş Parçacıkları :** Üç ayrı iş parçacığı (**pthread_create**) başlatılır: biri veri yazmak için, biri veri almak için, ve diğeri tampondaki verileri ekrana yazdırmak için.
* **Kilit İşlemleri :** İş parçacıklarının seri port ve tampon üzerinde aynı anda işlem yapmasını engellemek için mutex kilidi (**pthread_mutex_lock** ve **pthread_mutex_unlock**) kullanılır.
* **İş Parçacıkları Tamamlanana Kadar Bekleme :** **pthread_join()** ile iş parçacıklarının tamamlanması beklenir.
* **Kapatma :** Program sonunda seri port kapatılı ve mutex kilidi yok edilir.

### ÖZET
Bu program, **multi-threading** kullanarak dairesel bir tamponla seri porttan veri alıp göndermeyi sağlar. Programdaki temel mekanizma, dairesel tampon kullanarak veri kaybını önlemek ve verilerin düzenli olarak ekrana yazdırılmasını sağlamaktır. **Kilitleme mekanizması** ile aynı anda birden fazla iş parçacığının tampon üzerinde işlem yapması engellenir.




















