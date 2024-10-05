### Bu C kodu, Windows işletim sistemi üzerinde iki ayrı iş parçacığı (thread) kullanarak bir seri port (UART) üzerinden veri gönderme ve alma işlemlerini gerçekleştiriyor. Program, kullanıcıdan veri alıp COM portuna gönderiyor ve aynı zamanda COM portundan gelen verileri okuyor. 

#### 1. Gerekli Kütüphaneler
* **stdio.h   :** Standart I/O işlemleri için (örneğin, printf, fgets vb.)
* **unistd.h  :** POSIX sistemlerinde çeşitli işlemler için (Windows'ta gerekli değil ve burada kullanılmıyor. Windows'ta Sleep fonksiyonu windows.h ile sağlanıyor.)
* **pthread.h :** POSIX iş parçacıkları kütüphanesi. İki iş parçacığı kullanılarak veri gönderme ve okuma paralel olarak yapılır.
* **stdint.h  :** Sabit genişlikte veri türleri için (uint8_t gibi)
* **windows.h :** Windows API'lerine erişim sağlar. Bu programda seri portu yönetmek ve işlemleri yapmak için kullanılır.

#### 2. Global Değişkenler
* **HANDLE hComm :** Seri portu temsil eden bir handle (işaretçi). Bu, CreateFile fonksiyonu ile COM portuna erişimi sağlar.
* **pthread_mutex_t lock :** Veri gönderme ve okuma işlemleri sırasında paylaşılan kaynaklara (seri port) eşzamanlı erişim önlemek için bir kilit (mutex). İş parçacıkları arasında veri güvenliğini sağlar.

#### 3. uart_write_thread Fonksiyonu (Veri Gönderme Thread'i)
**Bu iş parçacığı, kullanıcıdan giriş alarak seri port üzerinden veri gönderir.**
* **fgets(message, sizeof(message), stdin) :** Kullanıcıdan veri alır. Bu veri, kulanıcı klavyesinden girilir.
* Kullanıcıdan akınan veri satır sonu karakterini (\n) içeriyorsa, bu karakter silinir.
* **pthread_mutex_lock(&lock) :** Veri gönderme işlemi sırasında seri portun başka bir iş parçacığı tarafından kullanılmasını engeller. Böylece veri gönderme işlemi güvenli bir şekilde yapılır.
* **WriteFile :** Bu fonksiyon, seri porta veri göndermek için kullanılır. Alınan veri (girdi), seri porta yazılır.
* **pthread_mutex_unlock(&lock) :** Seri port üzerindeki kilit serbest bırakılır, böylece başka bir iş parçacığı seri portu kullanabilir.

#### 4. uart_read_thread Fonksiyonu (Veri Okuma Thread'i)
**Bu iş parçacığı, sürekli olarak seri porttan veri okur ve ekrana yazar.**
* **ReadFile :** Bu fonksiyon, seri porttan veri okumak için kullanılır. Veri okuma başarılı olduğunda, alınan veriler ekrana yazdırılır.
* **Sleep(500) :** 500 milisaniyelik bir bekleme süresi. Bu, iş parçacığının sürekli olarak işlemciyi meşgul etmesini önler ve belirli aralıklarla veri okumasını sağlar.

#### 5. setup_uart Fonksiyonu (COM Portunu Ayarlama)
**Bu fonksiyon, COM portunu açmak ve yapılandırmak için kullanılır.**

* **CreateFile :** Bu fonksiyon, belirtilen COM portunu açar. (\\.\COM3 burada COM3 portunu temsil eder.)
  * **GENERIC_READ | GENERIC_WRITE :** Bu, portu hem okuma hem yazma modunda açar.
  * **OPEN_EXISTING :** Port zaten mevcutsa açılır.
  * Eğer port açılmazsa, **INVALID_HANDLE_VALUE** döner ve hata mesajı yazdırılır.

* **DCB Yapılandırılması (Data Control Block) :**
  * **dcbSerialParams.BaudRate = CBR_9600 :** Seri portun baud hızı 9600 olarak ayarlanır.
  * **dcbSerialParams.ByteSize = 8 :** Veri bit sayısı 8 olarak ayarlanır.
  * **dcbSerialParams.StopBits = ONESTOPBIT :** 1 stop biti kullanılır.
  * **dcbSerialParams.Parity = NOPARITY :** Parite kontrolü yok.
  **Seri port durumu ayarlanmazsa bir hata mesajı yazdırılır.**

* **Zaman Aşımı Ayarları :**
  * **COMMTIMEOUTS** yapısı, okuma ve yazma işlemlerinin zaman aşımı değerlerini ayarlamak için kullanılır.
  * **ReadIntervalTimeout = 50 :** İki okuma işlemi arasındaki maksimum süre (ms).
  * **ReadTotalTimeoutConstant** ve **ReadTotalTimeoutMultiplier :** Okuma işleminin zaman aşımı için sabit ve çarpan değerleri.
  * **WriteTotalTimeoutConstant** ve **WriteTotalTimeoutMuultiplier :** Yazma işleminin zaman aşımı için sabit ve çarpan değerleri.

#### 6. main Fonksiyonu
* İki iş parçacığı (**write_thread** ve **read_thread**) oluşturulur :
  * **pthread_create :** İki iş parçacığı oluşturur; biri veri yazmak, diğeri veri okumak için.
  * Bu iş parçacıkları aynı anda çalışarak paralel olarak seri port üzerinden iletişim sağlar.
* **pthread_join :** Ana program, iş parçacıkları tamamlanana kadar bekler.
* **pthread_mutex_destroy :** Kullanılan mutex (kilit) yok edilir.
* **CloseHandle(hComm) :** Program sonunda, açılan COM portu kapatılır.

#### 7. Çalışma Mantığı 
Bu program, kullanıcının girdiği mesjaları seri port üzerinden gönderir ve aynı zamanda gelen mesajları okur. İş parçacıkları sayesinde bu iki işlem aynı anda gerçekleştirilebilir. Kullanıcı, veri göndermek istediğinde terminalden yazıp enter'a bastığında, veri seri port üzerinden gönderilir ve diğer iş parçacığı sürekli olarak seri portu ve gelen verileri ekrana yazar.























