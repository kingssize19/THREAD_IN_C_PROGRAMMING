**POSIX (Portable Operating System Interface) standardına uygun pthread kütüphanesi, çok çekirdekli işlemcilerde çoklu iş parçacıklarını (threads) kullanarak paralel programlama yapmayı sağlayan bir kütüphanedir. pthread kütüphanesinin sağladığı fonksiyonlar sayesinde, programcılar birden fazla işlemi aynı anda yürütebilirler.**

### 1. pthread_create

**Bu fonksiyon, yeni thread oluşturmak için kullanılır.**
```c
int pthread_create(pthread_t *thread, const pthread_attr_t *attr, void *(*start_routine)(void *), void *arg);
```
* **thread :** Oluşturulacak iş parçacığının kimliğini tutan bir **pthread_t** değişkeninin adresi.
* **attr   :** İş parçacığına ait öznitelikleri belirtir. NULL verilirse varsayılan ayarlar kullanılır.
* **start_routine :** İş parçacığının çalıştıracağı fonksiyonun göstericisidir.
* **arg :** **start_routine** fonksiyonuna geçirilecek argümandır.

Başarı durumunda 0, hata durumunda pozitif bir hata kodu döner.

### 2. pthread_join

**Bir iş parçacığının sonlanmasını bekler. İş parçacığının tamamlandığında döndüreceği değeri alabilir.**
```c
int pthread_join(pthread_t thread, void **retval);
```
* **thread :** Beklenecek iş parçacığının kimliği.
* **retval :** İş parçacığının döndürdüğü değeri almak için kullanılan pointer.
Bir iş parçacığına **pthread_join** ile bağlanılmazsa, onun sonlanmasıyla ilgili kaynaklar temizlenmez ve **"zombie"** durumu oluşur.

### 3. pthread_exit

**Bir iş parçacığının çalışmasını sonlandırır.**
```c
void pthread_exit(void *retval);
```
* **retval :** İş parçacığı sonlandığında döndürülecek değerdir.
**pthread_exit**, iş parçacığını sonlandırırken kaynakları temizler ve **pthread_join** fonksiyonuyla başka iş parçacıklarına sonuç döndürebilir.

### 4. pthread_self

**Mevcut iş parçacığının kimliğini döndürür.**
```c
pthread_t pthread_self(void);
```
* **Dönüş Değeri :** Bu fonksiyon çağrıldığında çalışmakta olan iş parçacığının kimliği (pthread_t tipi).

### 5. pthread_detach

**Bir iş parçacığını ayrık hale getirir. Bu iş parçacığı kendi kendine tamamlanır ve sonlandığında kaynaklar otomatik olarak serbest bırakılır.**
```c
int pthread_detach(pthread_t thread);
```
* **thread :** Ayrık hale getirilecek iş parçacığının kimliği.
Ayrık iş parçacıklarında **pthread_join** kullanılamaz.

### 6. pthread_equal

**İki iş parçacığının kimliklerini karşılaştırır.**
```c
int pthread_equal(pthread_t t1, pthread_t t2);
```
* **t1, t2 :** Karşılaştırılacak iş parçacığı kimlikleridir.
* **Dönüş Değeri :** Eğer iki iş parçacığı aynı ise 0 dışında bir değer, değilse 0 döner.

### 7. pthread_mutex_init

**Bir mutex'i başlatır.**
```c
int pthread_mutex_init(pthread_mutex_t *mutex, const pthread_mutexattr_t *attr);
```
* **mutex :** Başlatılacak mutex değişkeni.
* **attr :** Mutexin öznitelikleri. NULL verilirse varsayılan olarak kullanılır.
Bu fonksiyon ile oluşturulan mutex'ler **pthread_mutex_lock** veya **pthread_mutex_trylock** ile kilitlenebilir.

### 8. pthread_mutex_destroy

**Bir mutex'i yok eder.**
```c
int pthread_mutex_destroy(pthread_mutex_t *mutex);
```
* **mutex :** Yok edilecek mutex değişkeni.

### 9. pthread_mutex_lock

**Bir mutex'i kilitler. Eğer mutex başka bir iş parçacığı tarafından kilitlenmişse, bu fonksiyon kilidin serbest bırakılmasını bekler.**
```c
int pthread_mutex_lock(pthread_mutex_t *mutex);
```
* **mutex :** Kilitlenecek mutex.

### 10. pthread_mutex_trylock

**Bir mutex'i kilitler ancak başka bir iş parçacığı tarafından kilitlenmişse beklemeden hemen geri döner.**
```c
int pthread_mutex_trylock(pthread_mutex_t *mutex);
```
* **mutex :** Kilitlenmeye çalışılacak mutex.
Dönüş değeri **EBUSY** ise mutex zaten kilitlenmiştir.

### 11. pthread_mutex_unlock

**Bir mutex'in kilidini açar.**
```c
int pthread_mutex_unlock(pthread_mutex_t *mutex);
```
* **mutex :** Kilidi açılacak mutex.

### 12. pthread_cond_init

**Bir koşul değişkeni (condition variable) başlatır.**
```c
int pthread_cond_init(pthread_cond_t *cond, const pthread_condattr_t *attr);
```
* **cond :** Başlatılacak koşul değişkeni.
* **attr :** Koşul değişkenine ait öznitelikler.
Koşul değişkenleri, belirli bir koşulun gerçekleşmesini beklemek için kullanılır.

### 13. pthread_cond_wait 

**Bir koşul değişkeni üzerinde bekler. Mutex kilitliyken çağrılır, beklemeye geçmeden önce mutex'i serbest bırakır ve koşul gerçekleştiğinde mutex'i tekrar kilitler.
```c
int pthread_cond_wait(pthread_cond_t *cond, pthread_mutex_t *mutex);
```
* **cond :** Beklenecek koşul değişkeni.
* **mutex :** Beklemeden önce kilidi serbest bırakılacak mutex.

### 14. pthread_cond_signal

**Bir iş parçacığını uyandırır, koşul değişkeni üzerinde bekleyen iş parçacıklarından bir tanesini uyandırır.**
```c
int pthread_cond_signal(pthread_cond_t *cond);
```
* **cond :** Uyandırılacak iş parçacığının beklediği koşul değişkeni.

### 15. pthread_cond_broadcast

**Bir koşul değişkeni üzerinde bekleyen tüm iş parçacıklarını uyandırır.**
```c
int pthread_cond_broadcast(pthread_cond_t *cond);
```
* **cond :** Bekleyen tüm iş parçacıklarının uyandırılacağı koşul değişkeni.

### 16. pthread_cancel

**Belirtilen bir iş parçacığını sonlandırır.**
```c
int pthread_cancel(pthread_t thread);
```
* **thread :** İptal edilecek iş parçacığının kimliği.
İptal edilen iş parçacığı, iptal edilebilir durumda ise çalışmasını sonlandırır. İş parçacıkları **pthread_cleanup_push** ve **pthread_cleanup_pop** kullanarak temizlik işlemleri yapabilir.

### 17. pthread_setcancelstate

**Bir iş parçacığının iptal edilebilirliğini ayarlar.**
```c
int pthread_setcancelstate(int state, int *oldstate);
```
* **state :** İptal edilebilirlik durumu. **PTHREAD_CANCEL_ENABLE** ile etkinleştirilir, **PTHREAD_CANCEL_DISABLE** ile devre dışı bırakılır.
* **oldstate :** Önceki iptal durumu.

### 18. pthread_setcanceltype 

**İptal edilebilirliğin anlık mı yoksa belirli bir nokta bekleyen türde mi olduğunu ayarlar.**
```c
int pthread_setcanceltype(int type, int *oldtype);
```
* **type :** İptal türü. **PTHREAD_CANCEL_ASYNCHRONOUS** veya **PTHREAD_CANCEL_DEFERRED**.
* **oldtype :** Önceki iptal türü.

### 19. sched_yield

**Mevcut iş parçacığı, işletim sistemine gönüllü olarak CPU'sunu serbest bırakır ve diğer iş parçacıklarına izin verir.**
```c
int sched_yield(void);
```
* **Dönüş Değeri :** Fonksiyon başarılı olursa 0, hata durumunda hata kodu döner.

### 20. pthread_attr_init

**Bir iş parçacığı öznitelik nesnesini başlatır.**
```c
int pthread_attr_init(pthread_attr_t *attr);
```
* **attr :** Başlatılacak öznitelik nesnesi.

### 21. pthread_attr_destroy

**Bir iş parçacığı öznitelik nesnesini yok eder.**
```c
int pthread_attr_destroy(pthread_attr_t *attr);
```
* **attr :** Yok edilecek öznitelik nesnesi.

### 22. pthread_attr_setdetachstate

**İş parçacığı özniteliklerinde, ayrık (detached) durumu ayarlar.**
```c
int pthread_attr_setdetachstate(pthread_attr_t *attr, int detachstate);
```
* **attr :** İş parçacığı öznitelik nesnesi.
* **detachstate :** Durum. **PTHREAD_CREATE_JOINABLE** veya **PTHREAD_CREATE_DETEACHED**.

### 23. pthread_attr_getdetachstate

**Bir iş parçacığının ayrık (detached) ya da katılabilir (joinable) durumda olup olmadığını öğrenmek için kullanılır.
```c
int pthread_attr_getdetachstate(const pthread_attr_t *attr, int *detachstate);
```
* **attr :** İş parçacığı öznitelik nesnesi.
* **detachstate :** İş parçacığının durumunu döndürecek değişken. **PTHREAD_CREATE_JOINABLE** ya da **PTHREAD_CREATE_DETACHED** döndürülür.

### 24. pthread_attr_setstacksize

**İş parçacığı özniteliklerinde iş parçacığına ayrılacak yığın belleği (stack size) boyutunu ayarlar.
```c
int pthread_attr_setstacksize(pthread_attr_t *attr, size_t stacksize);
```
* **attr :** İş parçacığı öznitelik nesnesi.
* **stacksize :** İş parçacığı için belirlenecek yığın boyutu.
Yığın boyutu varsayılan ayarların dışında bir boyut gerektiren uygulamalar için bu fonksiyonla ayarlanabilir.

### 25. pthread_att_getstacksize

**İş parçacığının yığın belleği boyutunu elde etmek için kullanılır.**
```c
int pthread_attr_getstacksize(const pthread_attr_t *attr, size_t *stacksize);
```
* **attr:** İş parçacığı öznitelik nesnesi.
* **stacksize:** Yığın boyutunu döndürecek değişken.

### 26. pthread_key_create

**İş parçacığı-özel veriyi tutmak için bir anahtar (key) oluşturur.**
```c
int pthread_key_create(pthread_key_t *key, void (*destructor)(void*));
```
* **key:** Yeni oluşturulacak anahtarı tutan değişken.
* **destructor:** İş parçacığı sonlandığında, anahtara bağlı veriyi temizlemek için kullanılacak fonksiyon.
İş parçacığı özel veri, her iş parçacığının bağımsız veri tutabilmesine olanak sağlar.

### 27. pthread_key_delete

**Daha önce oluşturulmuş bir anahtarı siler.**
```c
int pthread_key_delete(pthread_key_t key);
```
* key:** Silinecek anahtar.
Anahtarın silinmesi, ona bağlı veriyi otomatik olarak serbest bırakmaz.

### 28. pthread_set_specific

**Bir iş parçacığı için özel verilere erişmek amacıyla, anahtar kullanarak veri ataması yapar.**
```c
int pthread_setspecific(pthread_key_t key, const void *value);
```
* **key :** İş parçacığı özel veri anahtarı.
* **value :** Atanacak veri.

### 29. pthread_getspecific

**Bir iş parçacığına özel veriyi, anahtar kullanarak elde etmek için kullanılır.**
```c
void *pthread_getspecific(pthread_key_t key);
```
* **key :** Elde edilecek verinin anahtarı.
Her iş parçacığı, anahtarı kullanarak kendi verisini tutar ve elde eder.



























