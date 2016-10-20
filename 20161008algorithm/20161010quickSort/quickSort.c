
//#include"quickSort.h"

#include<stdio.h>
#include<string.h>
#include<stdlib.h>

#define L 6
void swap(int *a, int *b)
{
	int tmp = 0;
	tmp = *a;
	*a = *b;
	*b = tmp;
}
int partition(int arr[], int low, int high)
{
	int pivot;
	pivot = arr[low];//≥ı ºªØ ‡÷·
	arr[0] = arr[low];
	while(low < high)
	{
		if(arr[low] < arr[high]  && pivot <= high)
		{
			high --;
		}
		swap(&arr[low], &arr[high]);


		if(arr[low] < arr[high] && low >= pivot)
		{
			
			low ++;
		}
		swap(&arr[low],&arr[high]);
	return low;
	}
}//end of partition
void quickSort(int arr[], int low, int high)
{
	int privot;
	if(low < high)
	{
		privot = partition(arr, low, high);
		quickSort(arr, low, privot - 1);
		quickSort(arr, privot + 1, high);
	}
}

int main(int argc, char* argv[])
{
	int i;
	int arr[L] = {9,3,6,2,5,1};
	quickSort(arr, 0, L - 1);
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
	printf("x=%d, y=%d\n", x, y);*/
	return 0;
}