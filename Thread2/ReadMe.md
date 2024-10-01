# main.c programını analiz edelim. Bu programda, kullanıcı 1 veya 0 girdiğinde iş parçacıkları çalışmayı durduracak. Program iki iş parçacığını (thread) başlatır ve kullanıcıdan gelen girdiye göre çalışmayı sonlandırır.

```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
```
* #include <stdio.h>   : Standart girdi/çıktı işlemleri için gerekli kütüphane. printf, scanf gibi fonksiyonları kullanmak için.
* #include <stdlib.h>  : Genel amaçlı yardımcı fonksiyonlar için gerekli kütüphane. Bellek yönetimi ve program kontrolü için kullanılır.
* #include <unistd.h>  : Unix/Linux sistem çağrıları için gerekli kütüphane. sleep gibi fonksiyonları kullanmak için.
* #include <pthread.h> : Çok iş parçacıklı programlama için gerekli kütüphane. pthread_create, phtread_join gibi fonksiyonları içerir.


```c
volatile int stop = 0;
```
* stop adlı bir global değişken tanımlanır ve başlangıç değeri 0 olarak ayarlanır.
* volatile anahtar kelimesi, bu değişkenin değerinin farklı iş parçacıkları tarafından değiştirilebileceğini belirtir. Böylece, derleyici değişkenin değerinin optimize edilmesine izin vermez, her seferinde gerçek bellek adresinden okunur.


```c
void* func1() {
    while(!stop) {  // stop bayrağı 1 olduğunda döngüden çıkacak
        printf("func1 \n");
        sleep(1);
    }
    printf("func1 sonlandı.\n");
    return NULL;
}
```
* void* func1()       : func1 adlı bir iş parçacığı fonksiyonu tanımlanır. Geri dönüş tipi void* 'dır, bu iş parçacığı sonunda hiçbir değer döndürmeyecek.
* while (!stop)       : stop değişkeni 1 olmadıkça (yani 0 olduğunda) döngü devam eder. Bu, iş parçacığının sürekli çalışmasını sağlar.
* printf("func1 \n"); : Ekrana "func1" yazdırılır. Her döngüde bir kez gerçekleştirilir.
* sleep(1);           : Her döngüde 1 saniye bekler. Bu, iş parçacığının sistem kaynaklarını aşırı kullanmasını önler.
* printf("func1 sonlandı.\n"); : Eğer döngüden çıkılırsa, "func1 sonlandı." mesajı ekrana yazdırılır. Bu, iş parçacığının tamamlandığını belirtir.
* return NULL;        : Fonksiyon NULL döndürerek sonlanır.

```c
void* func2() {
    while(!stop) {  // stop bayrağı 1 olduğunda döngüden çıkacak
        printf("func2 \n");
        sleep(1);
    }
    printf("func2 sonlandı.\n");
    return NULL;
}
```
* void* func2() : func2 adlı bir iş parçacığı fonksiyonu tanımlanır. func1 ile benzer şekilde çalışır.
* Fonksiyonun içeriği func1 ile aynıdır, ancak "func2" mesajını yazdırır. Bu, iki iş parçacığının paralel olarak çalışmasını sağlar.

```c
int main() {
    pthread_t t1, t2;
```
* pthread_t t1, t2; : İki iş parçacığı tanımlayıcısı (pthread_t), t1 ve t2, oluşturulacak iş parçacıklarını temsil eder.

```c
    pthread_create(&t1, NULL, func1, NULL);
    pthread_create(&t2, NULL, func2, NULL);
```
* pthread_create : Yeni iş parçacıkları oluşturur:

* &t1   : İlk iş parçacığı için tanımlayıcı.
* NULL  : Varsayılan iş parçacığı özelliklerini kullan.
* func1 : Bu iş parçacığında çalışacak fonksiyon.
* NULL  : func1 'e herhangi bir argüman gönderilmez.
  
* İkinci pthread_create çağrısı func2 fonksiyonu için yapılır, aynı şekilde çalışır.


```c
uint8_t input;
    while(1) {
        printf("Devam etmek için 1, durdurmak için 0 girin: ");
        scanf("%zu", &input);

        if (input == 0) {
            stop = 1;  // stop bayrağını 1 yaparak thread'lerin durmasını sağlıyoruz
            break;     // Girdiyi aldıktan sonra ana döngüden çıkıyoruz
        }
    }
```
* uint8_t input;    : Kullanıcıdan alınacak girdi için 8 bitlik değişken tanımlanır.
* while (1)         : Sonsuz döngü, kullanıcıdan sürekli girdi bekler.
* Kullanıcının girdiği değer 0 olur ise stop değişkeninin değerini 1 yapar.

```c
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
```
* phtread_join  : İş parçacıklarının tamamlanmasını bekler.
* t1 ve t2 için iş parçacıkları tamamlanana kadar ana programın beklemesini sağlar. Bu, ana programın, iş parçacıklarının işlerini bitirmeden sonlanmamasını sağlar.


### ÖZET
Bu program, iki iş parçacığını (func1 ve func2) paralel olarak çalıştırır ve kullanıcıdan 0 veya 1 girişi alır. Kullanıcı 0 girdiğinde iş parçacıkları durdurulur ve program sonlanır. Program, iş parçacıkları arasında veri paylaşımı için bir global bayrak (stop) kullanır ve volatile anahtar kelimesi ile iş parçacıkları tarafından değiştirilebileceği belirtilir.



























