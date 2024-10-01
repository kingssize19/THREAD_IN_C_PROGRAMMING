### Bu kod iki iş parçacığının (thread) aynı anda çalışarak aynı global değişkeni (counter) artırdığı ve bunun yanında veri yarışmasını (race condition) önlemek için pthread_mutex_t kullanıldığı bir örnektir. 

#### Kodun Mantığı

1) Global Değişken ve Mutex Tanımı : 
   * counter global değişkendir ve her iki iş parçacığı da bu değişkeni artıracaktır.
   * phtread_mutex_t lock : Bu, bir "mutex" yani kilittir. Aynı anda birden fazla iş parçacığının counter değişkenini aynı anda güncellemesini önlemek için kullanılır. Mutex, kritik bölgelerde veri yarışmasını önler.

2) increment_counter Fonksiyonu :
   * Bu fonksiyon, bir iş parçacığının 1.000.000 defa counter değişkenini artırmasını sağlar.
   * Ancak her artırma işlemi pthread_mutex_lock(&lock) ve pthread_mutex_unlock(&lock) arasında yapılır. Bu, iki iş parçacığının aynı anda counter üzerinde işlem yapmasını önleyerek, düzgün bir şekilde sayacın güncellenmesini sağlar.

 3) main Fonksiyonu :
    * Burada iki iş parçacığı (thread1 ve thread2) yaratılır ve her iki iş parçacığına da aynı increment_counter fonksiyonu verilir.
    * İş parçacıkları başlatıldıktan sonra, pthread_join fonksiyonları kullanılarak ana program, her iki iş parçacığının tamamlanmasını bekler.
    * Tüm iş parçacıkları tamamlandığında, counter değeri ekrana yazdırılır.
    * Son olarak, kullanılan mutex kilidi pthread_mutex_destroy ile temizlenir.

### Detaylı Analiz : 

1) Global Değişken ve Mutex Tanımı :
   ```c
   int counter = 0;
   pthread_mutex_t lock;
   ```
   * counter değişkeni globaldir ve başlangıç değeri 0 olarak atanmıştır. Bu değişken, her iki iş parçacığı tarafından artırılacaktır.
   * phtread_mutex_t lock : Bu, bir "mutex" yani karşılıklı dışlama kilididir. Aynı anda sadece bir iş parçacığının counter değişkenine erişmesine izin vermek için kullanılır.

2) increment_counter Fonksiyonu :
   ```c
   void* increment_counter(void* arg) {
    for (int i = 0; i < 1000000; i++) {
        pthread_mutex_lock(&lock);
        counter++;
        pthread_mutex_unlock(&lock);
    }
    return NULL;
   }
   ```   
   * increment_counter fonksiyonu, bir iş parçacığı tarafından çalıştırıldığında, 1.000.000 kez counter değişkenini artırır.
   * Mutex Kullanımı :
       * pthread_mutex_lock(&lock) ile counter artırılmadan önce bir kilit alınır. Bu, başka bir iş parçacığının aynı anda counter üzerinde işlem yapmasını engeller.
       * counter++; satırı, counter değişkenini artırır.
       * pthread_mutex_unlock(&lock) ile kilit serbest bırakılır, böylece başka bir iş parçacığı counter üzerinde işlem yapabilir.

3) Ana Program :
  ```c
  int main() {
    pthread_t thread1, thread2;
    pthread_mutex_init(&lock, NULL);
  ```
  * pthread_t thread1, thread2;       : thread1 ve thread2 isimli iki iş parçacığı tanımlar.
  * pthread_mutex_init(&lock, NULL);  : lock isimli mutex (kilit) başlatılır. Bu kilit, counter değişkenini güvenli bir şekilde güncelleyebilmek için kullanılacaktır.

  ```c
    pthread_create(&thread1, NULL, increment_counter, NULL);
    pthread_create(&thread2, NULL, increment_counter, NULL);
  ```
  * pthread_create fonksiyonu, iş parçacıklarını başlatır.
  * &thread1 : thread1 iş parçacığına bir referans.
  * increment_counter : İş parçacığının çalıştıracağı fonksiyon.
  * Bu satırda iki iş parçacığı yaratılır ve her iki iş parçacığı da aynı increment_counter fonksiyonunu çalıştırır.

  ```c
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);
  ```
  * pthread_join fonksiyonu, ana programı iş parçacıklarının tamamlanmasını beklemeye zorlar.
  * İlk olarak thread1 iş parçacığının tamamlanması beklenir, ardından thread2 iş parçacığının tamamlanması beklenir.
  * Bu satırlar olmadan, ana program iş parçacıkları tamamlanmadan sona erebilir ve counter değeri doğru olarak ekrana yazdırılmayabilir.

  ```c
    printf("Counter değeri: %d\n", counter);
  ```
  * Bu satırda, iki iş parçacığı tamamlandıktan sonra, counter değişkeninin değeri ekrana yazdırılır.
  * Beklenen sonuç : counter değeri 2.000.000 olacaktır çünkü her iş parçacığı 1.000.000 kez artırma işlemi yapar.

  ```c
    pthread_mutex_destroy(&lock);
    return 0;
  }
  ```
  * pthread_mutex_destroy(&lock);  : Kullanılan mutex kilidi temizlenir.
  * Program başarıyla sona erer ve ana iş parçacığı 0 döner.

  ### Genel Değerlendirme
  * Bu program, iş parçacıklarının güvenli bir şekilde paylaşılan verilere erişimi nasıl koordine edebileceğini gösterir.
  * Eğer pthread_mutex_lock ve pthread_mutex_unlock kullanılmasaydı, iki iş parçacığının aynı anda counter'a erişmeye çalışması veri yarışmasına (race condition) yol açacaktı. Bu, hatalı sonuçlar veya beklenmedik davranışlara neden olabilirdi.
  
  
    













