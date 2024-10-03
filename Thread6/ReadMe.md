### Sensör Verisi Okuma Simülasyonu Thread

Bu kodda, üç farklı iş parçacığı (thread) kullanılarak iki ayrı sensör verisinin okunduğu ve bu verilerin işlenerek toplam verinin hesaplandığı bir sistem simüle edilmiştir.

#### Kod Analizi : 
**Global Değişkenler :**
* **pthread_mutex_t lock :** Mutex kilidi, sensör verilerine aynı anda erişimi kontrol etmek için kullanılıyor. Bu, verilerin doğru şekilde okunup yazılmasını garanti eder ve race condition (yarış durumu) hatalarını önler.
* **uint8_t sensor_data1 :** 1. sensörden okunan veriyi tutan 8-bitlik değişken.
* **uint8_t sensor_data2 :** 2. sensörden okunan veriyi tutan 8-bitlik değişken.
* **uint16_t total_data  :** Her iki sensör verisinin toplanarak işlendiği 16-bitlik değişken.

**Fonksiyonlar :**
**1) random_value1 ve random_value2 :**
   * Her iki fonksiyon da rastgele bir değer döndürür. rand() fonksiyonu ile 1 ile 256 arasında rastgele bir değer üretilir. Sensör verilerini simüle etmek için kullanılıyor.

**2) read_sensor1 ve read_sensor2 :**
   * Bu iki fonksiyon ayrı iş parçacıkları olarak çalıştırılı ve her biri bir sensör verisi okuma işlemini simüle eder.

   * **read_sensor1 :**
       * İlk sensör için rastgele bir veri üretilir (sensor_data1).
       * pthread_mutex_lock ile kilit alınır, böylece diğer iş parçacıkları bu sırada sensör verisine erişemez.
       * Verinin okunduğu ve ekrana yazdırıldığı anda diğer işlemler kilitlenmiştir.
       * pthread_mutex_unlock ile kilit serbest bırakılır.
       * Bu işlem sürekli olarak 1 saniye aralıklarla tekrarlanır.
   
   * **read_sensor2 :**
       * İkinci sensör için de benzer işlemler yapılır, ancak sensor_data2 değeri için aynı prosedür uygulanır.

**3) process_data :**
   * Bu fonksiyon, iki sensörden gelen verilerin toplanmasını sağlar.
   * pthread_mutex_lock ile kilit alınır ve sensor_data1 ile sensor_data2 toplanarak total_data değişkenine yazılır.
   * Bu işlem de her bir saniyede gerçekleştirilir ve sonuç ekrana yazdırılır.
   * Yine kilit serbest bırakılır.
  
### Ana Fonksiyon
**1) pthread_mutex_init :**
   * Mutex kilidi başlatılır. Bu işlem, iş parçacıkları arasında veri paylaşımını güvenli hale getirmek için gereklidir.

**2) İş Parçacıklarının Oluşturulması :**
   * pthread_create fonksiyonu ile üç iş parçacığı oluşturulmuş : 
     * thread1 için read_sensor1 işlevi çağrılır.
     * thread2 için read_sensor2 işlevi çağrılır.
     * thread3 için process_data işlevi çağrılır.
   * Her iş parçacığı bağımsız olarak çalışır ve sensör verilerini okuyup işleyerek ekrana yazdırır.

**3) pthread_join :**
   * Bu fonksiyonlar, iş parçacıklarının tamamlanmasını bekler. Ancak sonsuz döngülerde çalıştıkları için bu noktada iş parçacıkları hiç bitmeyecek, yani program sürekli çalışacaktır.

**4) pthread_mutex_destroy :**
   * Program sonunda kilit serbest bırakılır ve kaynaklar temizlenir.

### Çalışma Mantığı : 
**1) Sensör Verilerinin Okunması :**
  * Her iki sensör iş parçacığı 1 saniye aralıklarla rastgele veriler üretir ve bu veriler ekrana yazdırılır.
  * pthread_mutex_lock ve pthread_mutex_unlock ile verilerin güvenli şekilde paylaşımı sağlanır. Bu, aynı anda birden fazla iş parçacığının sensor_data değişkenlerine erişimini engeller.
**2) Verilerin İşlenmesi :**
  * Üçüncü iş parçacığı, iki sensör verisini toplayıp total_data olarak ekrana yazdırır. Bu da her 1 saniyede bir gerçekleşir ve diğer iki iş parçacığına aynı anda müdahale etmez.

### Özet : 
Bu kod, bir sistemde sensör verilerinin eş zamanlı olarak işlenmesi ve birleştirilmesini simüle eder. FreeRTOS gibi bir çoklu görev sisteminde tipik bir uygulama olan, kaynakların güvenli paylaşımı ve iş parçacıkları arasında veri senkronizasyonu için mutex kilidi kullanılmıştır. Sensör verileri rastgele üretilir ve ardından bu veriler işlenip toplam sonuç ekrana yazdırılır.




