#include <windows.h>
#include <iostream>
//#include "..\\DLLDemo1\\MyCode.h"
using namespace std;
//#pragma comment(lib, "..\\Debug\\secondDLL.lib")
#pragma comment(lib, "secondDLL.lib")
extern "C" _declspec(dllimport) int Add(int a, int b);
int main(int argc, char *argv[])
{
      cout<<Add(2, 3)<<endl;
	  system("pause");
      return 0;
}