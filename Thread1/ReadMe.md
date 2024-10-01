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

     
