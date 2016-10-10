#include"free_dump.h"


int main()
{
	char a1[10]="123456";
	 THsPack sPack  =
	{
		&a1,
		3,
		5,
		&a1,
		3,
		3,
		&a1,
		3,
		3,
		3,
		'a',
		3,
		"123",
		&a1,
		'a'


	} ;
	char *a = "abcd";
	hsParseData(&sPack, a);

	return 0;
}

/*解析数据包,返回实际解析出来的行数，如果无数据或异常等都返回0*/
int  hsParseData(THsPack *hsPack, const char *sData)
{
  char *pScan,  *pDataStart;
  int iFldCnt, iRowCnt, i, j, ilen;
  byte iFldLen; 
  if(sData)
  {
    if (hsPackClear(hsPack))
    {
       return 0;
    }
    pScan = (char *) sData;
    if (*pScan == '@')
    {
     pScan ++;
     hsPack->bUseOldPack = 0;
     if (*pScan == 0) return 0;
     pScan ++;
     iRowCnt = StrToInt(pScan, 3);
     if (iRowCnt >= hsPack->iBufRowCount)
     {
      if (hsInternalExtRows(hsPack , hsPack->iRowCount))
      { 
        return 0; 
      } 
     }
     pScan+=3;
     iFldCnt = StrToByte(pScan); 
     if (iFldCnt)
     {
       pScan += 1;
       pDataStart = (char *)sData + StrToInt(pScan, 3);
       pScan += 3;

       for (i=0; i < iFldCnt; i ++)
       {
         iFldLen = (unsigned char)StrToByte(pScan);
         pScan ++; 
         pScan[iFldLen] = 0;
         hsAddFieldEx(hsPack, pScan, 0, -1);
         pScan = pScan + iFldLen +  1;
       }
       if (pScan != pDataStart) 
         return 0;
        else
        {
          
         for (i = 0; i < iRowCnt; i ++)
         {
          
          hsAppendRow(hsPack);
            pScan += 3;
          iFldLen = (unsigned char)StrToByte(pScan);
          pScan ++; 
          pDataStart = pScan + ( iFldLen * CONST_DATASIZELEN); 
          for (j = 0; j < iFldLen; j++)
          {
            ilen = StrToInt(pScan, CONST_DATASIZELEN);
            pScan += CONST_DATASIZELEN; 
            if (pDataStart[ilen] == 0 || pDataStart[ilen] == CONST_PACKSPLIT)
              pDataStart[ilen] = 0;
            else
                return 0;
            if (hsInternalAddString(&hsPack->arrRows[i].arrCols[j], pDataStart, -1, ilen)) 
              return 0;
             
            
            pDataStart = pDataStart + ilen + 1;

          }
          hsPack->arrRows[i].iColCnt = iFldLen;
          pScan = pDataStart;  
         }
         return hsPack->iRowCount; 
        }
             
     }
     else
       return 0;
    }
    else  /*旧版的包*/
    {
      while ((*pScan != '<') && (*pScan != 0)) pScan++;
      if (*pScan == '<')
      {
        hsPack->bUseOldPack = 1;
        /*if (pScan == NULL)
          return -1;*/
        if (strcmp(pScan, "") == 0) 
          return -1;
        if (hsAppendRow(hsPack)) 
          return -1;
        do 
        { 
          pScan = ParseOldPackage(hsPack, pScan);
          if (pScan == NULL)
            break;
          /*if (*name != 0) 
            PAddItemString(Package,name, value);
          else 
            break; */
        } 
        while (*pScan!=0);
        return 0;
      }
      else
        return 0;
    }
  }
  else  /*不能传空包进来*/
    return 0;
}




int  hsPackClear(THsPack *hsPack)
{
  int i;
  for (i=hsPack->iRowCount - 1; i >= 0 ; i--  ) /*无记录时不需要释放*/
  {
	printf("i=%d;hsPack=%s;hsPack->iRowCount - 1=%d\n",i,*hsPack,hsPack->iRowCount - 1);
    hsFreeOneRow(&hsPack->arrRows[i], 0); 
  } 
  hsPack->iRowCount = 0; 
  for (i=0 ; i < hsPack->iFieldCount; i++)  /*实际无字段时，也不需要释放*/
  {
    hsPack->arrFields[i].iLen = 0;  
    hsPack->arrFields[i].sFieldName[0] = 0;
    hsPack->arrFields[i].sLinkName = NULL;
  } 
  hsPack->iFieldCount = 0; 
  hsPack->iRecNo = -1;
  hsPack->bEof = 1;
  hsPack->bUseOldPack = 0;
  
  return 0;
}




int hsFreeOneRow(THsPackRow *arrRows, const int bNeedFree)
{
  int j;
  printf("j=%d;arrRows=%s;bNeedFree=%d\n",j, arrRows,bNeedFree);
  for (j=0; j < arrRows->iColCnt; j ++)
  {
	  printf("j=%d;arrRows->iColCnt=%d\n",j, arrRows->iColCnt);
    if (arrRows->arrCols[j].sData && arrRows->arrCols[j].bCanFree)
    { 
      free(arrRows->arrCols[j].sData);
    }
    memset(&arrRows->arrCols[j], 0 , sizeof(arrRows->arrCols[j])); 
  }
  
  if (bNeedFree)
    free(arrRows->arrCols); 
  return 0;
}



int StrToInt(const char *sVal, const int iMaxLen)
{
  int i, iRet;
  byte *pVal;
  iRet = 0; 
  for (i = 0 ; i < iMaxLen ; i++)
  { 
    if (sVal[i] == 0) 
      return 0;
    pVal = (byte *) &sVal[i]; 
    switch (i)
    {
      case 0:
        iRet = iRet + (*pVal ^ 0xFF);
        break;
      case 1:
        iRet = iRet + (*pVal ^ 0xFF) * 255;
        break;
      case 2:
        iRet = iRet + (*pVal ^ 0xFF) * 65025;
        break;
      default:
        break;
    }
  }
  return iRet;
}



int hsInternalExtRows(THsPack *hsPack, int iNeedNewRowCount)
{
  int oldRow;
  if (iNeedNewRowCount < CONST_BUFROWCNT)
    iNeedNewRowCount = CONST_BUFROWCNT;
  oldRow = hsPack->iBufRowCount;
  hsPack->iBufRowCount = hsPack->iBufRowCount + iNeedNewRowCount;
  hsPack->arrRows = (THsPackRow *)realloc(hsPack->arrRows , (unsigned int)hsPack->iBufRowCount * CONST_PACKROWSIZE);/*这里*/
  if (hsPack->arrRows)
  {
    memset(&hsPack->arrRows[oldRow] , 0, (unsigned int)iNeedNewRowCount * CONST_PACKROWSIZE);
    return 0;
  }
  else
  {
    hsPack->iBufRowCount = 0;
    hsPack->iRowCount = 0;
    hsPack->iRecNo = -1;
    hsPack->bEof = 1;
    return -1;
  }
  
}


int  hsAppendRow(THsPack *hsPack)
{
   int iTemVl = hsPack->iRowCount;
   unsigned int iRowSize ;
   if (iTemVl + 1 >= hsPack->iBufRowCount )
   {
    if (hsInternalExtRows(hsPack, 1))
    {
      return  -1;
    }
   }
   hsPack->iRowCount ++;
   /*检查上次有无申请过*/
   if (hsPack->arrRows[iTemVl].iBufColCnt)
   {
    hsPack->arrRows[iTemVl].iColCnt = 0;
    if (hsPack->iFieldCount > hsPack->arrRows[iTemVl].iBufColCnt)
    {
      iRowSize = (unsigned int)hsPack->iFieldCount * CONST_PACKCOLSIZE;
	  hsPack->arrRows[iTemVl].arrCols =  (THsPackColData *)realloc(hsPack->arrRows[iTemVl].arrCols, iRowSize);/*这里*/
      hsPack->arrRows[iTemVl].iBufColCnt = hsPack->iFieldCount;
      if ( hsPack->arrRows[iTemVl].arrCols)
      {
        memset(hsPack->arrRows[iTemVl].arrCols, 0, iRowSize);
        hsPack->iRecNo = iTemVl ;
        hsPack->bEof = 0;
        return 0;
      }
      else
      {
        hsPack->arrRows[iTemVl].iBufColCnt = 0;
        return -1;
      }
    }
    else
    {
      hsPack->iRecNo = iTemVl ;
      hsPack->bEof = 0;
      return 0;
    }
   }
   else
   {
     hsPack->arrRows[iTemVl].iBufColCnt = hsPack->iFieldCount;
     hsPack->arrRows[iTemVl].iColCnt = 0;
     iRowSize = (unsigned int) hsPack->iFieldCount * CONST_PACKCOLSIZE;
	 hsPack->arrRows[iTemVl].arrCols =  (THsPackColData *)malloc(iRowSize);/*这里*/
     if ( hsPack->arrRows[iTemVl].arrCols)
     {
       memset(hsPack->arrRows[iTemVl].arrCols, 0, iRowSize);
       hsPack->iRecNo = iTemVl ;
       hsPack->bEof = 0;
       return 0;
     }
     else
     {
       hsPack->arrRows[iTemVl].iBufColCnt = 0;
       return -1;
     }
   }
   /*return 0;*/
 
}



int StrToByte(const char *sVal) 
{ 
  /*byte iRet;*/
  byte *pVal;
  if (*sVal == 0) 
    return 0;
  /*iRet = 0;*/  
  pVal = (byte *) &sVal[0]; 
  return (*pVal ^ 0xFF);
}



int  hsAddFieldEx(THsPack *hsPack,  const char *sFieldName, const int bNeedChkName, const int bLinkName)
{
  int iFldIdx ;
  if (hsPack->iFieldCount >= hsPack->iBufFieldCount) /*需要扩展*/
  {
      if (hsInternalExtFields(hsPack))
        return -1;
  }
  
  if (bNeedChkName)
  {
    iFldIdx = hsGetFieldIdx(hsPack, sFieldName);
  }
  else
  {
    iFldIdx = -1; 
  }
  
  if (iFldIdx < 0) 
  {
    hsPack->arrFields[hsPack->iFieldCount].iLen = strlen(sFieldName);
    if (hsPack->arrFields[hsPack->iFieldCount].iLen)
    {
      if (bLinkName)
      {
        hsPack->arrFields[hsPack->iFieldCount].sLinkName = (char *)sFieldName;
        hsPack->arrFields[hsPack->iFieldCount].sFieldName[0] = 0;
      }
      else
      {
        strcpy(hsPack->arrFields[hsPack->iFieldCount].sFieldName, sFieldName);  /*这里strcpy*/
        hsPack->arrFields[hsPack->iFieldCount].sLinkName = NULL; 
      }
      hsPack->arrFields[hsPack->iFieldCount].iFldIdx = hsPack->iFieldCount;
      hsPack->iFieldCount ++; 
      return hsPack->arrFields[hsPack->iFieldCount - 1].iFldIdx;
    }
    else /*字段名不允许为空*/
    {
       return -1;
    }
  }
  else
  {
    return iFldIdx;
  } 
}




int hsInternalAddString(THsPackColData *pCol, const char *Value, const int bIsLink, const int iDataLen)
{
  /*外部需要对各项内部状态进行检查通过后，方可调用此方法*/
  if (pCol->sData && pCol->bCanFree)
  {
    free(pCol->sData);
  }
  if (bIsLink)
  {
    pCol->bCanFree = 0;
    pCol->iLen = iDataLen;
    pCol->sData = (char *)Value;
    return 0;
  }
  else
  {
    
    pCol->bCanFree = 1;
    pCol->iLen = iDataLen;
    if (pCol->iLen > CONST_MAXSTRINGLEN)
    {
      pCol->iLen = CONST_MAXSTRINGLEN;
      
    }
    pCol->sData = (char *)malloc(pCol->iLen + 1);/*这里*/
    if (pCol->sData)
    {
      if (pCol->iLen == CONST_MAXSTRINGLEN)
      {
        memmove(pCol->sData, Value, CONST_MAXSTRINGLEN);
        pCol->sData[CONST_MAXSTRINGLEN] = 0;
      }
      else
        strcpy(pCol->sData, Value);/*这里strcpy*/
      return 0;
    }
    else
    {
      pCol->bCanFree = 0;
      pCol->iLen = 0;
      return -1;
      
    }
    
  }
}



char* ParseOldPackage(THsPack *hsPack, char *S  )
{ 
  char *nameend;
  char tmpName[MAX_ITEMNAME_LEN+5];
  int i, idx, ilen;
  
  if (S == NULL)  return NULL;
  memset(tmpName, 0, MAX_ITEMNAME_LEN+5);
  
  while ((*S != '<') && (*S != 0)) S++;
  if (*S == 0) {
    return NULL;
  } 
  else 
    S++;
  
  i=0;
  while ((*S != '>') && (*S != 0)) tmpName[i++] = *(S++);
  if (*S == 0) { 
    return NULL;
  } else S++;
  
  tmpName[i] = 0;
  idx = hsAddField(hsPack, tmpName); /*找到字段*/ 
  tmpName[i] = '>';
  tmpName[i + 1] = 0; 
  memmove(tmpName + 2 ,tmpName , i + 2);
  tmpName[0] = '<';
  tmpName[1] = '/';  
  
  nameend = strstr(S, tmpName);
  if (nameend) 
  { 
    ilen = (size_t)(nameend - S);
    S[ilen] = 0;
    hsAddStringEx(hsPack, idx, S, ilen);
    S[ilen] = '<';
    nameend += strlen(tmpName);
  }
  return nameend;
}




int hsInternalExtFields(THsPack *hsPack)
{
   int oldCnt;
   oldCnt = hsPack->iBufFieldCount;
   hsPack->iBufFieldCount = hsPack->iBufFieldCount + CONST_BUFFIELDCNT;
   hsPack->arrFields = (THsPackField *)realloc(hsPack->arrFields , (unsigned int)hsPack->iBufFieldCount * gIPackFieldSize);
   if (hsPack->arrFields)
   {
     memset(&hsPack->arrFields[oldCnt] , 0, (unsigned int) CONST_BUFFIELDCNT * gIPackFieldSize);
     return 0;
   }
   else
   {
     hsPack->iBufFieldCount = 0;
     hsPack->iFieldCount = 0;
     return -1;
   }
}

/*获取字段名对应的索引号,未找到返回-1，取数据时都通过索引号进行，暂不支持按字段名取数据（按字段名太慢）*/
int  hsGetFieldIdx(THsPack *hsPack,  const char *sFieldName)
{
  int i , iret; 
  iret = -1;
  for (i=0; i < hsPack->iFieldCount ; i ++)
  { 
    if (strcmp(sFieldName, hsInlGetFieldName(&hsPack->arrFields[i])) == 0)
    {
      iret = hsPack->arrFields[i].iFldIdx;
      break;
    }
  }
  return iret;
}
char * hsInlGetFieldName(THsPackField * pPackField)
{
   if (pPackField->sLinkName)
   {
     return pPackField->sLinkName;
   }
   else
   {
     return pPackField->sFieldName;
   }
}

/*添加一个字段名，同时返回当前字段的索引号*/
int  hsAddField(THsPack *hsPack, const char *sFieldName)
{ 
  return hsAddFieldEx(hsPack, sFieldName, -1,  0);
}

int  hsAddStringEx(THsPack *hsPack, int iFldIdx,const char *Value, const int iValLen)
{ 
  if (hsInternalCheckCol(hsPack, iFldIdx))
  { /*无法插入数据*/
    return -1;
  }
  return hsInternalAddString(&hsPack->arrRows[hsPack->iRecNo].arrCols[iFldIdx], Value,  0, iValLen);
}





int hsInternalCheckCol(THsPack *hsPack, int iFldIdx )
{ 
  int i;
  if (hsPackEof(hsPack)) /*当前记录位置不对*/
  {
    return -1;
  }
  if(iFldIdx < 0 || iFldIdx >= hsPack->iFieldCount)
  {
    return -1;
  }
  else
  { 
    if ( iFldIdx >= hsPack->arrRows[hsPack->iRecNo].iBufColCnt  )
    {  /*字段有新增*/
      
		hsPack->arrRows[hsPack->iRecNo].arrCols = (THsPackColData *)realloc(hsPack->arrRows[hsPack->iRecNo].arrCols, (unsigned int)hsPack->iFieldCount * CONST_PACKCOLSIZE);/*这里*/
      if(hsPack->arrRows[hsPack->iRecNo].arrCols)
      {
        i = hsPack->arrRows[hsPack->iRecNo].iBufColCnt;
        memset(&hsPack->arrRows[hsPack->iRecNo].arrCols[i], 0,
        (unsigned int)(hsPack->iFieldCount - i) * CONST_PACKCOLSIZE);
       
          hsPack->arrRows[hsPack->iRecNo].iBufColCnt = hsPack->iFieldCount;
      }
      else
      {
        hsPack->arrRows[hsPack->iRecNo].iColCnt = 0;
        hsPack->arrRows[hsPack->iRecNo].iBufColCnt = 0;
        return -1;
      }
    }
    if ( iFldIdx >= hsPack->arrRows[hsPack->iRecNo].iColCnt )
      hsPack->arrRows[hsPack->iRecNo].iColCnt =  iFldIdx +1;
    return 0;
  }
}


int  hsPackEof(THsPack *hsPack)
{ 
  if (hsPack->bEof == 0)
  {
    if (hsPack->iRecNo < 0 || hsPack->iRecNo >= hsPack->iRowCount)
    {
      hsPack->bEof = 1;
    }
  }
  return hsPack->bEof;
}


/**






int  hsAddFieldEx(THsPack *hsPack,  const char *sFieldName, const int bNeedChkName, const int bLinkName)
{
  int iFldIdx ;
  if (hsPack->iFieldCount >= hsPack->iBufFieldCount) 
  {
      if (hsInternalExtFields(hsPack))
        return -1;
  }
  
  if (bNeedChkName)
  {
    iFldIdx = hsGetFieldIdx(hsPack, sFieldName);
  }
  else
  {
    iFldIdx = -1; 
  }
  
  if (iFldIdx < 0) 
  {
    hsPack->arrFields[hsPack->iFieldCount].iLen = strlen(sFieldName);
    if (hsPack->arrFields[hsPack->iFieldCount].iLen)
    {
      if (bLinkName)
      {
        hsPack->arrFields[hsPack->iFieldCount].sLinkName = (char *)sFieldName;
        hsPack->arrFields[hsPack->iFieldCount].sFieldName[0] = 0;
      }
      else
      {
        strcpy(hsPack->arrFields[hsPack->iFieldCount].sFieldName, sFieldName);  
        hsPack->arrFields[hsPack->iFieldCount].sLinkName = NULL; 
      }
      hsPack->arrFields[hsPack->iFieldCount].iFldIdx = hsPack->iFieldCount;
      hsPack->iFieldCount ++; 
      return hsPack->arrFields[hsPack->iFieldCount - 1].iFldIdx;
    }
    else 
    {
       return -1;
    }
  }
  else
  {
    return iFldIdx;
  } 
}






int hsInternalAddString(THsPackColData *pCol, const char *Value, const int bIsLink, const int iDataLen)
{
  外部需要对各项内部状态进行检查通过后，方可调用此方法
  if (pCol->sData && pCol->bCanFree)
  {
    free(pCol->sData);
  }
  if (bIsLink)
  {
    pCol->bCanFree = 0;
    pCol->iLen = iDataLen;
    pCol->sData = (char *)Value;
    return 0;
  }
  else
  {
    
    pCol->bCanFree = 1;
    pCol->iLen = iDataLen;
    if (pCol->iLen > CONST_MAXSTRINGLEN)
    {
      pCol->iLen = CONST_MAXSTRINGLEN;
      
    }
    pCol->sData = malloc(pCol->iLen + 1);
    if (pCol->sData)
    {
      if (pCol->iLen == CONST_MAXSTRINGLEN)
      {
        memmove(pCol->sData, Value, CONST_MAXSTRINGLEN);
        pCol->sData[CONST_MAXSTRINGLEN] = 0;
      }
      else
        strcpy(pCol->sData, Value);
      return 0;
    }
    else
    {
      pCol->bCanFree = 0;
      pCol->iLen = 0;
      return -1;
      
    }
    
  }
}






**/