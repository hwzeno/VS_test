#include<stdio.h>
 void swap4(int &a, int &b)
 {
     int tem;
 
     tem = a;
     a = b;
     b = tem;
 }
 

int main()
{
       int i;
       int a = 5, b= 6;
       swap4(a, b);
      
           printf("%d,%d\n",a,b);
       return 0;
}