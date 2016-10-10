#include<stdio.h>
#include <stdio.h>
#include <stdlib.h>    
#include <assert.h>    
#include <stdarg.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <sys/timeb.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <limits.h>

#define CONST_BUFROWCNT    100 /*每次扩展行*/
#define CONST_PACKROWSIZE  sizeof(THsPackRow)
#define CONST_PACKCOLSIZE sizeof(THsPackColData)
#define CONST_DATASIZELEN 2 
#define CONST_PACKSPLIT  1
#define CONST_MAXSTRINGLEN      (254 * 254)  /* 目前每字段值的最大长度限制在254 * 254字节，超过后自动截掉 16581374*/
#define MAX_ITEMNAME_LEN 100
#define CONST_BUFFIELDCNT  30 /*每次扩展字段数*/
#define  gIPackFieldSize sizeof(THsPackField) 
typedef unsigned char byte;


typedef struct HsPackField
{
  char sFieldName[100]; 
  char *sLinkName;
  int iLen; /*字段名长度*/
  int iFldIdx; /*当前字段所在位置*/
}THsPackField;


typedef struct HsPackColData
{
  char *sData;
  byte bCanFree;
  int iLen;
}THsPackColData;

typedef struct HsPackRow
{
  THsPackColData *arrCols;
  int iColCnt;  /*当前列数*/
  int iBufColCnt; /*已申请列数*/
}THsPackRow;



typedef struct HsPack
{
  char *sData; /*数据区*/
  int iLen;
  int iBufLen;
  THsPackField *arrFields; /*保存字段列表*/
  int iFieldCount;
  int iBufFieldCount;
  THsPackRow *arrRows;  /*保存数据行*/
  int iRowCount;  /*当前行数*/
  int iBufRowCount; /*缓存行数*/
  int iRecNo; /*当前位置,从0开始*/
  byte bEof;
  int iPackVer; /*当前数据包类型*/

  char sValue[30];
  char *pOptData;
  byte bUseOldPack; /*当前是否解析了旧版本的包，下次重新打包时，如果此值为真，并且只有一行时，还是打包成旧版的包*/
}THsPack;



int  hsParseData(THsPack *hsPack, const char *sData);
int  hsPackClear(THsPack *hsPack);
int hsFreeOneRow(THsPackRow *arrRows, const int bNeedFree);
int StrToInt(const char *sVal, const int iMaxLen);
int hsInternalExtRows(THsPack *hsPack, int iNeedNewRowCount);
int  hsAppendRow(THsPack *hsPack);
int StrToByte(const char *sVal) ;
int  hsAddFieldEx(THsPack *hsPack,  const char *sFieldName, const int bNeedChkName, const int bLinkName);
int hsInternalAddString(THsPackColData *pCol, const char *Value, const int bIsLink, const int iDataLen);
char* ParseOldPackage(THsPack *hsPack, char *S  );
int  hsAddFieldEx(THsPack *hsPack,  const char *sFieldName, const int bNeedChkName, const int bLinkName);
int hsInternalExtFields(THsPack *hsPack);
int  hsGetFieldIdx(THsPack *hsPack,  const char *sFieldName);
char * hsInlGetFieldName(THsPackField * pPackField);
int  hsAddField(THsPack *hsPack, const char *sFieldName);
int  hsAddStringEx(THsPack *hsPack, int iFldIdx,const char *Value, const int iValLen);
int hsInternalAddString(THsPackColData *pCol, const char *Value, const int bIsLink, const int iDataLen);
int hsInternalCheckCol(THsPack *hsPack, int iFldIdx );
int  hsPackEof(THsPack *hsPack);