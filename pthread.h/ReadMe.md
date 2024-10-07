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





































