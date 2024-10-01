### Bu kod iki iş parçacığının (thread) aynı anda çalışarak aynı global değişkeni (counter) artırdığı ve bunun yanında veri yarışmasını (race condition) önlemek için pthread_mutex_t kullanıldığı bir örnektir. 

#### Kodun Mantığı

1) Global Değişken ve Mutex Tanımı : 
   * counter global değişkendir ve her iki iş parçacığı da bu değişkeni artıracaktır.
   * phtread_mutex_t lock : Bu, bir "mutex" yani kilittir. Aynı anda birden fazla iş parçacığının counter değişkenini aynı anda güncellemesini önlemek için kullanılır. Mutex, kritik bölgelerde veri yarışmasını önler.

2) increment_counter Fonksiyonu :
*Bu fonksiyon, bir iş parçacığının 1.000.000 defa counter değişkenini artırmasını sağlar.
* Ancak her artırma işlemi pthread_mutex_lock(&lock) ve pthread_mutex_unlock(&lock) arasında yapılır. Bu, iki iş parçacığının aynı anda counter üzerinde işlem yapmasını önleyerek, düzgün bir şekilde sayacın güncellenmesini sağlar.
 
