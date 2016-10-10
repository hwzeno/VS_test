#include<stdio.h>
#include<math.h>
struct
{
	int i;
	char ptr[89];
}Obj;
int main()
{
	int a = sizeof(Obj);
	double b = ceil(89.0/4)*4 + 4;
	printf("a=%d\n",a);
	printf("b=%f\n",b);
	return 0;
}