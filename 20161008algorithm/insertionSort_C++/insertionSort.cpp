//我的程序也报这个错
#include<iostream>
using namespace std;
template<typename T> //整數或浮點數皆可使用,若要使用物件(class)時必須設定大於(>)的運算子功能
void insertion_sort(T arr[], int len)
{
       int i, j;
       T temp;
       for (i = 1; i < len; i++)
       {
              temp = arr[i];
              for (j = i - 1; j >= 0 && arr[j] > temp; j--)
                     arr[j + 1] = arr[j];
              arr[j+1] = temp;
       }
}
int main()
{
       int i;
       int arr[6]={5,4,3,2,1,0};
       insertion_sort(arr,6);
       for(i = 0; i < 6; i++)
              cout<<arr[i]<<"\n"<<endl;
       return 0;
}
