#include <stdio.h>

//W trakcie poszukiwań dzikich fragmentów języka C
//

//funkcje tak szybkie jak możliwe
inline void f(int x){
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


//q1=q2=q3
    short q1[4][3][2] = {
                { 1 },
                { 2, 3 },
                { 4, 5, 6 }
    };
    short q2[4][3][2] = {
            1, 0, 0, 0, 0, 0,
            2, 3, 0, 0, 0, 0,
            4, 5, 6
    };

    short q3[4][3][2] = {
            { {1}, },
            { {2,3}, },
            { {4,5}, {6},}
          }

    return 129;
}
