#include<stdio.h>
#include<string.h>
#include<stdlib.h>
//#define len 6

typedef struct _Range 
{
	int start, end;
} Range;


Range new_Range(int s, int e) ;
void swap(int *x, int *y) ;
void quick_sort(int arr[], const int len);