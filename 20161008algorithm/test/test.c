#include<stdio.h>

void insertion_sort(int array[], int first, int last){
	int i, j, temp;
	for (i = first + 1; i <= last; i++){
		temp = array[i]; //與已排序的數逐一比較，大於temp時，該數向後移
		for(j = i - 1; j >= first && array[j] > temp; j--) //当first=0，j循环到-1时，由于[[短路求值]](http://zh.wikipedia.org/wiki/%E7%9F%AD%E8%B7%AF%E6%B1%82%E5%80%BC)，不会运算array[-1]
			array[j + 1] = array[j];
		array[j+1] = temp; //被排序数放到正确的位置
	}
}

int main()
{
       int i;
       int arr[6]={5,3,7,2,6,9};
       insertion_sort(arr,0,5);
       for(i = 0; i < 6; i++)
           printf("%d\n",arr[i]);
       return 0;
}