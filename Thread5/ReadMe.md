### LED Blink Simülasyonu (Thread ile Paralel LED Yakma)

**Bu proje, bir gömülü sistemdeki iki LED'in farklı hızlarda yanıp sönmesini simüle eder. İki ayrı thread, iki farklı LED'in yanıp sönmesini kontrol eder.**

### Genel Yapı

1) pthread_t :
   * pthread_t, POSIX thread (pthread) yapısında iş parçacıklarını temsil eden veri türüdür.
   * Kodda thread1 ve thread2 isimlerinde iki iş parçacığı yer almaktadır.

2) pthread_create :
   * pthread_create fonksiyonu, yeni bir iş parçacığı oluşturur. Burada iki iş parçacığı oluşturulmuş.
     * led1_control iş parçacığı, LED 1'i 1 saniyelik aralıklarla yakıp söndürür.
     * led2_control iş parçacığı, LED 2'yi 2 saniyelik aralıklarla yakıp söndürür.
   * pthread_create fonksiyonunun aldığı parametreler :
     1) İş parçacığının referansı (pthread_t).
     2) Varsayılan özellikler için NULL.
     3) İş parçacığı fonksiyonu (led1_control ve led2_control).
     4) Fonksiyona geçilecek parametre (NULL).

3) pthread_join :
   * pthread_join, iş parçacığının tamamlanmasını bekleyen bir fonksiyondur.
   * Burada, pthread_join iki kez çağrılmış. Bu, ana programın iki iş parçacığının tamamlanmasını bekleyeceği anlamına gelir. Fakat iş parçacıkları sonsuz döngüde oldukları için bu fonksiyonlar, program bitene kadar sürekli çalışacaktır.

4) Çalışma Mantığı :
   * Her iki thread aynı anda çalıştırılır ve bu sayede LED1 her snaiye yanıp sönerken, LED2 her iki saniyede bir yanıp söner.
   * Ana fonksiyonun pthread_join fonksiyonlarıyla iş parçacıklarının tamamlanmasını beklemesi nedeniyle program iş parçacıkları bitene kadar çalışmaya devam eder. Ancak iş parçacıkları sonsuz döngüde olduğu için program hiçbir zaman sonlanmaz.
   
