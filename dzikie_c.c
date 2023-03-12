#include <stdio.h>

//W trakcie poszukiwań dzikich fragmentów języka C
//


void f(int x){
  printf("%d",x);
}

//przekazywanie funkcji
void fun(void func(int)){
    func(1);
}

extern int max(int a, int b)
{
    return a > b ? a : b;
}


int main()
{
    volatile int x = 5; //zmienne ulotne

    //Dzika notacja
    int a<::> = <% 1, 2, 3 %>;

    for(int i =0; i<3; i++)<%
        printf("%d",i); ;
    %>
    printf("%ld",sizeof &x); //sizeof unary-expression

    int i=1, j=2, k=3;
    _Bool f = 1;

    i=j*=k;
    printf("%d",j);

    //a[i]=i[a]

    //zmienne restrict
    int * restrict p;


    return 0;
}
