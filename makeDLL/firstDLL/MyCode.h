#ifndef _MYCODE_H_
#define _MYCONDE_H_
#ifdef DLLDEMO1_EXPORTS
#define EXPORTS_DEMO _declspec( dllexport )
#else
#define EXPORTS_DEMO_ _declspec(dllimport)
#endif
extern "c" EXPORTS_DEMO int Add (int a, int b);
#endif
