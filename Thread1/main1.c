#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void func1()
{
    while(1)
    {
        printf("func1 \n");
        sleep(1);
    }
}

void* func2()
{
    while(1)
    {
        printf("func2 \n");
        sleep(1);
    }

}

int main()
{

    func1();        //func1 fonksiyonu içindeki while(1) içerisinden çıkamaz sadece ekrana func1 yazar.
    func2();

    return 0;
}
