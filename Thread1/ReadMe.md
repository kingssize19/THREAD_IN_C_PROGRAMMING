# main1.c
**Bu kod, iki fonksiyondan (func1 ve func2) oluşan bir programdır. Ancak programın genel akışında, bazı sorunlar var ve bu nedenle kod beklenildiği gibi çalışmayacak.
func1() fonksiyonu :** 
```c
void func1() {
    while(1) {
        printf("func1 \n");
        sleep(1);
    }
}
```
* func1, sonsuz bir döngüde (while(1)) sürekli olarak ekrana "func1 \n" yazdırır.
* sleep(1); ifadesiyle program, her yazdırma işleminden sonra 1 saniye uyur.
* Bu fonksiyon, çağrıldığında sonsuza kadar çalışır ve geri dönmez (çıkmaz).

**func2() fonksiyonu :**
```c
void* func2() {
    while(1) {
        printf("func2 \n");
        sleep(1);
    }
}
```

* func2, aynı şekilde sonsuz döngü ile ekrana "func2 \n" yazdırır ve her seferinde 1 saniye uyur.
* Bu fonksiyonun geri dönüş tipi void* olarak tanımlanmış.

**main() fonksiyonu :**
```c
int main() {
    func1();        // func1 fonksiyonu içindeki while(1) içerisinden çıkamaz sadece ekrana func1 yazar.
    func2();
    return 0;
}
```

* main fonksiyonu önce func1() fonksiyonunu çağırır.
* func1() sonsuz bir döngüye girdiği için bu fonksiyon asla bitmez ve kontrol func2()'ye geçmez.
* Yani program sadece "func1 \n" çıktısını tekrar tekrar ekrana yazdıracaktır. ve asla "func2 \n" yazdırmayacaktır.
* func2()'nin çağrılması mümkün olmayacak çünkü func1() sonsuz döngüde kilitlenmiş durumdadır.

**Fonksiyonların Paralel Çalışması :**

* İki fonksiyonun da aynı anda (paralel) çalışmasını istiyorsak, pthread (iş parçacıkları) kütüphanesini kullanarak her iki fonksiyonu ayrı thread'ler olarak çalıştırabilir. Ya da aşağıdaki uygulandığı şekilde bu işlemi thread_main1.c programındaki gibi gerçekleştirebiliriz. Aşağıda ayrıntılı bir şekilde thread_main1.c programı analiz edilmektedir.


# ------------------------------------------------------------------------------------------------------------------------------------------------------------

# thread_main1.c
**Bu programda, iki fonksiyon (func1 ve func2) paralel olarak çalıştırılmaya çalışılıyor. Programda iş parçacıkları (threads) kullanılarak aynı anda hem func1 hem de func2 fonksiyonlarının çıktıları ekranda gösterilmek isteniyor.**
```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
```
* Bu kütüphaneler temel I/O işlemleri (stdio.h), bellek yönetimi (stdlib.h), zaman gecikmeleri (unistd.h) ve iş parçacıkları (pthread.h) için gerekli.

```c
void func1() {
    while(1) {
        printf("func1 \n");
        sleep(1);
    }
}
```
* func1 fonksiyonu sonsuz bir döngüye shaiptir (while(1)).
* Döngü içinde ekrana "func1 \n" yazdırılır.
* Her yazdırmadan sonra program 1 saniye bekler (sleep(1)).

```c
void* func2() {
    while(1) {
        printf("func2 \n");
        sleep(1);
    }
}
```
* func2 de aynı şekilde sonsuz bir döngüye sahiptir.
* Ekrana "func2 \n" yazdırılır ve her yazdırmadan sonra 1 saniye bekler.
* Geri dönüş tipi void* olarak belirtilmiş, bu iş parçacığı (thread) fonksiyonu olduğu için önemlidir.

```c
int main() {
    pthread_t t1;
```
* pthread_t t1 : Bir iş parçacığı tanımlayıcısı (pthread_t), iş parçacığını temsil eder.

```c
pthread_create(&t1, NULL, func2, NULL);
```
* pthread_create : Yeni bir iş parçacığı oluşturur.
* &t1 : Oluşturulan iş parçacığı için tanımlayıcı.
* NULL : Varsayılan iş parçacığı özelliklerini kullan.
* func2 : Bu iş parçacığında çalışacak fonksiyon.
* NULL : func2 'ye gönderilen argüman yok.

```c
    func1();
```
* Ana iş parçacığı, func1 fonksiyonunu çağırır ve bu fonksiyon çalışmaya başlar.
* Sonsuz döngüde olduğu için bu noktadan sonra kontrol ana iş parçacığına geri dönmez.

```c
    sleep(1);
```
* 1 saniyelik bekleme yapılır, ancak func1 sonsuz döngüde olduğundan bu satıra ulaşılmaz.

