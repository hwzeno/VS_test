
/*******************两个数交换位置************************/
//#include<stdio.h>
// void swap4(int &a, int &b)
// {
//     int tem;
// 
//     tem = a;
//     a = b;
//     b = tem;
// }
// 
//
//int main()
//{
//       int i;
//       int a = 5, b= 6;
//       swap4(a, b);
//      
//           printf("%d,%d\n",a,b);
//       return 0;
//}



/*************cType.h 中的字符转换函数*****************/
//#include<stdio.h>
//#include<stdlib.h>
//#include<string.h>
//#include<ctype.h>
//int main(char argc, char* argv[])
//{
//	char *arr = NULL;
//	int i = 0;
//	char a = '0';
//	arr = (char *)malloc(sizeof(char) * 10);
//	scanf("%s", arr);//输入字符时个数不能小于10-1，否则最后一个“\0”会写到堆（heap）中。
//	while( i < strlen(arr))
//	{
//		a = toupper(arr[i]);
//		printf("%c", a);
//		i++;
//	}
//	free(arr);
//	return 0;
//}

/**********************实现字符大小写转换*************************/
//#include<stdio.h>
//int main()
//{
//	char a;
//	while(EOF != scanf("%c", &a))//Ctrl + c 结束
//	{
//		if(97 <= a & a <=127)
//		{
//			printf("%c\n",a-32);
//		}
//		else
//		{
//			printf("%c\n", a)	;
//		}
//	}
//}
/***********************************************/