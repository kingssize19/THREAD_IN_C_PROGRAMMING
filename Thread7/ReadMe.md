### Bu kod, iki ayrı iş parçacığı kullanarak bir saat simülasyonu yapıyor. Biri saniyeleri, diğeri ise dakikaları sayıyor. İş parçacıkları arasındaki veri yarışmasını (race condition) önlemek için bir pthread_mutex_t kilidi kullanılıyor.

#### İşleyiş : 
**1. Global Değişkenler :**
  * **uint8_t seconds :** Saniyeleri takip eden değişken.
  * **uint8_t minutes :** Dakikaları takip eden değişken.
  * **pthread_mutex_t lock :** İş parçacıkları arasında veri tutarsızlığını önlemek için kullanılan kilit.

**2. İş Parçacıkları :**
  * **count_seconds() :**
    * Saniyeyi 1'er artırır.
    * Saniye 60 olduğunda sıfırlanır.
    * Her saniye ekrandaki dakika ve saniye bilgisini günceller.
    * Kilitleme ile veri bütünlüğü sağlanır.
    * sleep(1) ile her saniyede bir çalışır.

  * **count_minutes() :**
    * Her 60 saniyede bir dakikayı artırır.
    * Dakika 60 olduğunda sıfırlanır.
    * Kilit ile veriler güvence altına alınır.
    * Saniye 0 olduğunda dakikayı artırır ve 1 snaiye uyur.

**3. Ana Program (main) :**
  * İki iş parçacığı başlatılır: biri saniyeleri, diğeri dakikaları saymak için.
  * İş parçacıkları çalışmaya başladıktan sonra program sonsuz döngüde kalır (iş parçacıkları beklenir).
  * Kilit düzgün şekilde yok edilir.
















