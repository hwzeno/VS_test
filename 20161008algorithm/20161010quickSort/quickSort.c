
//#include"quickSort.h"

#include<stdio.h>
#include<string.h>
#include<stdlib.h>


void swap(int *a, int *b)
{
	int tmp = 0;
	tmp = *a;
	*a = *b;
	*b = tmp;
}
void quickSort(int arr[], int len)
{
	int low, high, pivot;
	low = 0;//初始化低位
	high = len - 1;//初始化高位
	pivot = 0;//初始化枢轴

	while(low < high)
	{
		if(arr[low] > arr[pivot])
		{
			swap(&arr[low],&arr[pivot]);
			pivot = low;
		}
		if(arr[high] < arr[pivot])
		{
			swap(&arr[pivot], &arr[high]);
			pivot = high;
		}
		low++;
		high--;

	}





}

int main(int argc, char* argv[])
{
	int i;
	int arr[6] = {9,3,6,2,5,1};
	quickSort(arr, sizeof(arr)/sizeof(int));
	for(i = 0; i < 6; i++)
	{
		printf("arr[%d]\n",arr[i]);
	}

/*	return 0;
	

	int x = 5;
	int y = 6;
	int* xp = &x;
	int* yp = &y;
	swap(xp, yp)
	swap(&x, &y);
	printf("x=%d, y=%d\n", x, y);;*/
	return 0;
}