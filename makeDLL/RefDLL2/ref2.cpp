#undef UNICODE
#include <windows.h>
#include <iostream>
using namespace std;
typedef int (*AddFunc)(int a, int b);
int main(int argc, char *argv[])
{
      HMODULE hDll = LoadLibrary("secondDLL.dll");//或在引号前加L
      if (hDll != NULL)
      {
            AddFunc add = (AddFunc)GetProcAddress(hDll, "Add");//Add这个名字之所以这么简单，是因为写了extern “C”的缘故。
            if (add != NULL)
            {
                  cout<<add(2, 5)<<endl;
            }
            FreeLibrary(hDll);
      }
	  system("pause");
	  return 0;
}