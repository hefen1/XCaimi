//tools.cpp
#include "stdafx.h"
#include <memory.h>
#include <stdlib.h>
#include <io.h>
#include <fcntl.h>
#include <windows.h>
#include <tchar.h>
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
#if !defined(_WIN32_WCE)
#include <stdio.h>
#include <io.h>
#include <dos.h>
#include <direct.h>
#endif

#include "tools.h"

struct Qentry{
	int left;
	int right;
};
//
int smartcmp(const void* pBuff1,const void* pBuff2,short count)
{
	int nRet = 0;
	switch(count)
	{
	case QS_INT:
		nRet = (*(int*)pBuff1 == *(int*)pBuff2) ? 0 :
			(*(int*)pBuff1 >  *(int*)pBuff2) ? 1 : -1;
		break;		
	case QS_UNINT:
		nRet = (*(unsigned int*)pBuff1 == *(unsigned int*)pBuff2) ? 0 :
			(*(unsigned int*)pBuff1 >  *(unsigned int*)pBuff2) ? 1 : -1;
		break;		
	case QS_LONG:
		nRet = (*(long*)pBuff1 == *(long*)pBuff2) ? 0 :
			(*(long*)pBuff1 >  *(long*)pBuff2) ? 1 : -1;
		break;		
	case QS_UNLONG:
		nRet = (*(unsigned long*)pBuff1 == *(unsigned long*)pBuff2) ? 0 :
			(*(unsigned long*)pBuff1 >  *(unsigned long*)pBuff2) ? 1 : -1;
		break;		
	case QS_SHORT:
		nRet = (*(short*)pBuff1 == *(short*)pBuff2) ? 0 :
			(*(short*)pBuff1 >  *(short*)pBuff2) ? 1 : -1;
		break;		
	case QS_UNSHORT:

		nRet = (*(unsigned short*)pBuff1 == *(unsigned short*)pBuff2) ? 0 :
			(*(unsigned short*)pBuff1 >  *(unsigned short*)pBuff2) ? 1 : -1;
		break;		
	case QS_FLOAT:
		nRet = (*(float*)pBuff1 == *(float*)pBuff2) ? 0 :
			(*(float*)pBuff1 >  *(float*)pBuff2) ? 1 : -1;
		break;		
	case QS_DOUBLE:
		nRet = (*(double*)pBuff1 == *(double*)pBuff2) ? 0 :
			(*(double*)pBuff1 >  *(double*)pBuff2) ? 1 : -1;
		break;		
	default:
		if(count > 0)
		{
			const void* p;
			short sLen,sMin = count;
			if((p = memchr(pBuff1,'\0',count)) != NULL)
			{
				sLen = (char*)p - (char*)pBuff1;
				if((sLen > 0) && (sLen < sMin))
				{
					sMin = sLen;
				}	
			}
			if((p = memchr(pBuff2,'\0',count)) != NULL)
			{
				sLen = (char*)p - (char*)pBuff2;
				if((sLen > 0) && (sLen < sMin))
				{
					sMin = sLen;
				}	
			}
			nRet = memcmp(pBuff1,pBuff2,sMin);
		}
		break;					
	}
	return nRet;
}

int	BinarySearch(struct KQuickSort* pQSData,void* pKeySearchFor, BOOL bAscend /*= TRUE*/)
{
	long mid,iLow,iHigh;
	int nRet;
	if((pQSData == NULL) || (pKeySearchFor == NULL))
	{
		return -1;
	}
	iLow = pQSData->m_nLow;
	iHigh = pQSData->m_nHigh;		
	if(iHigh < iLow)
		return -1;
	char* pKey = (char*)pQSData->m_pData + pQSData->m_nKeyOffset;
	char* pKeyCur;
	// 加入降序的折半查找
	if(bAscend)
	{
		do
		{
			mid = (iLow + iHigh) / 2;
			pKeyCur = pKey + mid*pQSData->m_nDataLen;
			nRet = smartcmp(pKeyCur,pKeySearchFor,pQSData->m_nKeyLen);
			if(nRet == 0)
				return mid;
			if(nRet > 0)
				iHigh = mid - 1;
			else
				iLow  = mid + 1;
		}while (iLow <= iHigh);
	}
	else
	{
		do
		{
			mid = (iLow + iHigh) / 2;
			pKeyCur = pKey + mid*pQSData->m_nDataLen;
			nRet = smartcmp(pKeyCur,pKeySearchFor,pQSData->m_nKeyLen);
			if(nRet == 0)
				return mid;
			if(nRet < 0)
				iHigh = mid - 1;
			else
				iLow  = mid + 1;
		}while (iLow <= iHigh);
	}
	return -1;
}

void QuickSortAscend(struct KQuickSort* pQSData)
{
	long iUp, iDown;
	long iLow,iHigh;
	if(pQSData == NULL)
		return;
	iLow = pQSData->m_nLow;
	iHigh = pQSData->m_nHigh;		

	short  nDataLen = pQSData->m_nDataLen;
	short  nKeyLen  = pQSData->m_nKeyLen;

	if(iLow >= iHigh)	
		return;
	if(nDataLen <= 0)
		return;
	char* pTemp = new char[nDataLen];
	char* pKey = (char*)pQSData->m_pData + pQSData->m_nKeyOffset;
	char* pData = (char*)pQSData->m_pData;

	char* pKeyLow;
	char* pKeyHigh;
	if( (iHigh - iLow) == 1 )
	{
		pKeyLow  = pKey + iLow*nDataLen;
		pKeyHigh = pKey + iHigh*nDataLen;
		if(smartcmp(pKeyLow,pKeyHigh,nKeyLen) > 0)
		{
			memcpy(pTemp,pData+iHigh*nDataLen,nDataLen);
			memcpy(pData+iHigh*nDataLen,pData+iLow*nDataLen,nDataLen);
			memcpy(pData+iLow*nDataLen,pTemp,nDataLen);
		}
	}
	else
	{
		do
		{
			// Move in from both sides towards the pivot element.
			iUp = iLow;
			iDown = iHigh;
			while(iUp < iDown)
			{
				if(smartcmp(pKey+iUp*nDataLen,pKey+iHigh*nDataLen,
					nKeyLen) > 0)
					break;
				iUp++;
			}  
			while(iDown > iUp)
			{
				if(smartcmp(pKey+iDown*nDataLen,pKey+iHigh*nDataLen,
					nKeyLen) < 0)
					break;
				iDown--;
			}
			// If we haven't reached the pivot, it means that two
			// elements on either side are out of order, so swap them.
			//
			if( iUp < iDown )
			{
				memcpy(pTemp,pData+iDown*nDataLen,nDataLen);
				memcpy(pData+iDown*nDataLen,pData+iUp*nDataLen,nDataLen);
				memcpy(pData+iUp*nDataLen,pTemp,nDataLen);
			}
		} while ( iUp < iDown );

		// Move pivot element back to its proper place in the array.
		memcpy(pTemp,pData+iHigh*nDataLen,nDataLen);
		memcpy(pData+iHigh*nDataLen,pData+iUp*nDataLen,nDataLen);
		memcpy(pData+iUp*nDataLen,pTemp,nDataLen);
		if( (iUp - iLow) < (iHigh - iUp) )
		{
			pQSData->m_nLow = iLow;
			pQSData->m_nHigh = iUp - 1;		
			QuickSortAscend(pQSData);
			pQSData->m_nLow = iUp + 1;
			pQSData->m_nHigh = iHigh;		
			QuickSortAscend(pQSData);
		}
		else
		{
			pQSData->m_nLow = iUp + 1;
			pQSData->m_nHigh = iHigh;		
			QuickSortAscend(pQSData);
			pQSData->m_nLow = iLow;
			pQSData->m_nHigh = iUp - 1;		
			QuickSortAscend(pQSData);
		}
	}
	delete[] pTemp;
}
void QuickSortAscend2(struct KQuickSort* pQSData)
{
	long i,j,l,r,top = 0,PivotIndex = 0;

	if(pQSData == NULL) return;
	i = pQSData->m_nLow;
	j = pQSData->m_nHigh;
	int iSize = j - i + 1;

	short  nDataLen = pQSData->m_nDataLen;
	short  nKeyLen  = pQSData->m_nKeyLen;

	if(nDataLen <= 0 || iSize <= 1) return;

	char* pivot = NULL;
	char* pKey	= (char*)pQSData->m_pData + pQSData->m_nKeyOffset;
	char* pData = (char*)pQSData->m_pData;

	char* pTemp = new char[nDataLen];

	if(iSize == 2)
	{
		if(smartcmp(pKey + i * nDataLen,pKey + j * nDataLen,nKeyLen) > 0)
		{
			memcpy(pTemp,pData + j * nDataLen,nDataLen);
			memcpy(pData + j * nDataLen,pData + i * nDataLen,nDataLen);
			memcpy(pData + i * nDataLen,pTemp,nDataLen);
		}
		delete[] pTemp;
		return;
	}

	static short nStackSize = 1000;
	Qentry* pStack = new Qentry[nStackSize];
	pStack->left  = i;
	pStack->right = j;
	while(top >= 0)
	{
		if( top >= nStackSize )
			break;

		//Pop Stack
		i = (pStack + top)->left;
		j = (pStack + top--)->right;

		//Find Pivot
		PivotIndex = (i + j) >> 1;

		pivot = pKey + PivotIndex * nDataLen;

		//Stick pivot at end
		memcpy(pTemp,pData + PivotIndex * nDataLen,nDataLen);
		memcpy(pData + PivotIndex * nDataLen,pData + j * nDataLen,nDataLen);
		memcpy(pData + j * nDataLen,pTemp,nDataLen);

		//Partition
		l = i - 1;
		r = j;
		do
		{
			while(smartcmp((char*)(pKey + (++l) * nDataLen),pivot,nKeyLen) < 0);
			while(r && smartcmp((char*)(pKey + (--r) * nDataLen),pivot,nKeyLen) > 0);
			memcpy(pTemp,pData + l * nDataLen,nDataLen);
			memcpy(pData + l * nDataLen,pData + r * nDataLen,nDataLen);
			memcpy(pData + r * nDataLen,pTemp,nDataLen);
		} while ( l < r );

		//==============================
		//Undo final swap
		memcpy(pTemp,pData + l * nDataLen,nDataLen);
		memcpy(pData + l * nDataLen,pData + r * nDataLen,nDataLen);
		memcpy(pData + r * nDataLen,pTemp,nDataLen);
		//==============================
		//Put pivot value in place
		memcpy(pTemp,pData + l * nDataLen,nDataLen);
		memcpy(pData + l * nDataLen,pData + j * nDataLen,nDataLen);
		memcpy(pData + j * nDataLen,pTemp,nDataLen);
		//==============================		
		if(l - i > 9)
		{
			top++;
			if( top >= nStackSize )
				break;

			(pStack + top)->left = i;
			(pStack + top)->right = l - 1;
		}
		if(j - l > 9)
		{
			top++;					
			if( top >= nStackSize )
				break;

			(pStack + top)->left = l + 1;
			(pStack + top)->right = j;
		}
	}

	delete[] pStack;

	//Insertion Sort
	for(i = pQSData->m_nLow;i < pQSData->m_nHigh + 1;i ++)
		for(j = i;(j > pQSData->m_nLow) && (smartcmp((char*)(pKey + j * nDataLen),(char*)(pKey + (j - 1) * nDataLen),nKeyLen) < 0);j --)
		{
			memcpy(pTemp,pData + j * nDataLen,nDataLen);
			memcpy(pData + j * nDataLen,pData + (j - 1) * nDataLen,nDataLen);
			memcpy(pData + (j  - 1) * nDataLen,pTemp,nDataLen);
		}

		delete[] pTemp;		
}

void QuickSortDescend(struct KQuickSort* pQSData)
{
	long iUp, iDown;
	long iLow,iHigh;
	if(pQSData == NULL)
		return;
	iLow = pQSData->m_nLow;
	iHigh = pQSData->m_nHigh;		

	short  nDataLen = pQSData->m_nDataLen;
	short  nKeyLen  = pQSData->m_nKeyLen;

	if(iLow >= iHigh)
		return;
	if(nDataLen <= 0)
		return;
	char* pTemp = new char[nDataLen];
	char* pKey = (char*)pQSData->m_pData + pQSData->m_nKeyOffset;
	char* pData = (char*)pQSData->m_pData;

	char* pKeyLow;
	char* pKeyHigh;
	if( (iHigh - iLow) == 1 )
	{
		pKeyLow  = pKey + iLow*nDataLen;
		pKeyHigh = pKey + iHigh*nDataLen;
		if(smartcmp(pKeyLow,pKeyHigh,nKeyLen) < 0)
		{
			memcpy(pTemp,pData+iHigh*nDataLen,nDataLen);
			memcpy(pData+iHigh*nDataLen,pData+iLow*nDataLen,nDataLen);
			memcpy(pData+iLow*nDataLen,pTemp,nDataLen);
		}

	}
	else
	{
		do
		{
			// Move in from both sides towards the pivot element.
			iUp = iLow;
			iDown = iHigh;
			while(iUp < iDown)
			{
				if(smartcmp(pKey+iUp*nDataLen,pKey+iHigh*nDataLen,
					nKeyLen) < 0)
					break;
				iUp++;
			}  
			while(iDown > iUp)
			{
				if(smartcmp(pKey+iDown*nDataLen,pKey+iHigh*nDataLen,
					nKeyLen) > 0)
					break;
				iDown--;
			}
			// If we haven't reached the pivot, it means that two
			// elements on either side are out of order, so swap them.
			//
			if( iUp < iDown )
			{
				memcpy(pTemp,pData+iDown*nDataLen,nDataLen);
				memcpy(pData+iDown*nDataLen,pData+iUp*nDataLen,nDataLen);
				memcpy(pData+iUp*nDataLen,pTemp,nDataLen);
			}
		} while ( iUp < iDown );

		// Move pivot element back to its proper place in the array.
		memcpy(pTemp,pData+iHigh*nDataLen,nDataLen);
		memcpy(pData+iHigh*nDataLen,pData+iUp*nDataLen,nDataLen);
		memcpy(pData+iUp*nDataLen,pTemp,nDataLen);
		// Recursively call the QuickSort procedure (pass the smaller
		// subdivision first to use less stack space).
		//
		if( (iUp - iLow) < (iHigh - iUp) )
		{
			pQSData->m_nLow = iLow;
			pQSData->m_nHigh = iUp - 1;		
			QuickSortDescend(pQSData);
			pQSData->m_nLow = iUp + 1;
			pQSData->m_nHigh = iHigh;		
			QuickSortDescend(pQSData);
		}
		else
		{
			pQSData->m_nLow = iUp + 1;
			pQSData->m_nHigh = iHigh;		
			QuickSortDescend(pQSData);
			pQSData->m_nLow = iLow;
			pQSData->m_nHigh = iUp - 1;		
			QuickSortDescend(pQSData);
		}
	}
	delete[] pTemp;
}
void QuickSortDescend2(struct KQuickSort* pQSData)
{
	long i,j,l,r,top = 0,PivotIndex = 0;

	if(pQSData == NULL) return;
	i = pQSData->m_nLow;
	j = pQSData->m_nHigh;
	int iSize = j - i + 1;

	short  nDataLen = pQSData->m_nDataLen;
	short  nKeyLen  = pQSData->m_nKeyLen;

	if(nDataLen <= 0 || iSize <= 1) return;

	char* pivot = NULL;
	char* pKey	= (char*)pQSData->m_pData + pQSData->m_nKeyOffset;
	char* pData = (char*)pQSData->m_pData;

	char* pTemp = new char[nDataLen];
	char* pKeyLow;
	char* pKeyHigh;

	if(iSize == 2)
	{
		pKeyLow  = pKey + i * nDataLen;
		pKeyHigh = pKey + j * nDataLen;
		if(smartcmp(pKeyLow,pKeyHigh,nKeyLen) < 0)
		{
			memcpy(pTemp,pData + j * nDataLen,nDataLen);
			memcpy(pData + j * nDataLen,pData + i * nDataLen,nDataLen);
			memcpy(pData + i * nDataLen,pTemp,nDataLen);
		}
		delete[] pTemp;
		return;
	}
	Qentry* pStack = new Qentry[100];
	pStack->left  = i;
	pStack->right = j;
	while(top >= 0)
	{
		//Pop Stack
		i = (pStack + top)->left;
		j = (pStack + top--)->right;

		//Find Pivot
		PivotIndex = (i + j) >> 1;

		pivot = pKey + PivotIndex * nDataLen;

		//Stick pivot at end
		memcpy(pTemp,pData + PivotIndex * nDataLen,nDataLen);
		memcpy(pData + PivotIndex * nDataLen,pData + j * nDataLen,nDataLen);
		memcpy(pData + j * nDataLen,pTemp,nDataLen);

		//Partition
		l = i - 1;
		r = j;
		do
		{
			while(smartcmp(pKey + (++l) * nDataLen,pivot,nKeyLen) > 0);
			while(r && smartcmp((char*)(pKey + (--r) * nDataLen),pivot,nKeyLen) < 0);
			memcpy(pTemp,pData + l * nDataLen,nDataLen);
			memcpy(pData + l * nDataLen,pData + r * nDataLen,nDataLen);
			memcpy(pData + r * nDataLen,pTemp,nDataLen);
		} while ( l < r );
		//==============================
		//Undo final swap
		memcpy(pTemp,pData + l * nDataLen,nDataLen);
		memcpy(pData + l * nDataLen,pData + r * nDataLen,nDataLen);
		memcpy(pData + r * nDataLen,pTemp,nDataLen);
		//==============================
		//Put pivot value in place
		memcpy(pTemp,pData + l * nDataLen,nDataLen);
		memcpy(pData + l * nDataLen,pData + j * nDataLen,nDataLen);
		memcpy(pData + j * nDataLen,pTemp,nDataLen);
		//==============================		
		if(l - i > 9)
		{
			top++;
			(pStack + top)->left = i;
			(pStack + top)->right = l - 1;
		}
		if(j - l > 9)
		{
			top++;
			(pStack + top)->left = l + 1;
			(pStack + top)->right = j;
		}
	}
	delete[] pStack;
	//Insertion Sort
	for(i = pQSData->m_nLow;i < pQSData->m_nHigh + 1;i ++)
		for(j = i;(j > pQSData->m_nLow) && (smartcmp(pKey + j * nDataLen,pKey + (j - 1) * nDataLen,nKeyLen) > 0);j --)
		{
			memcpy(pTemp,pData + j * nDataLen,nDataLen);
			memcpy(pData + j * nDataLen,pData + (j - 1) * nDataLen,nDataLen);
			memcpy(pData + (j  - 1) * nDataLen,pTemp,nDataLen);
		}
		delete[] pTemp;		
}

//others functions
static TCHAR* Stack[10];
static int top = 0;

TCHAR* _strtok(TCHAR* s1,TCHAR* s2)
{
	register const TCHAR * sp;
	TCHAR* tok = 0;
	TCHAR* Ss  = 0;
	if(s1) 
	{
		if (top == 10) 
			return 0;
		Stack[top++] = s1;
		Ss = s1;
	}
	else 
	{
		if(top == 0)
			return 0;
		Ss = Stack[top-1];
	}  

	while(*Ss)
	{
		for(sp=s2; *sp; sp++)
			if(*sp == *Ss)
				break;
		if(*sp == 0)
			break;
		Ss++;
	}
	if(*Ss == 0)
	{
		top--;
		return 0;
	}   
	tok = Ss;
	while(*Ss)
	{
		for(sp=s2; *sp; sp++)
			if(*sp == *Ss)
			{
				*Ss++ = 0;
				Stack[top-1] = Ss;
				return tok;
			}
			Ss++;
	}
	Stack[top-1] = Ss;
	return tok;                  
}

static int _tempnum;
static TCHAR  tmfile[128];
TCHAR* _tempfname(const TCHAR* dir, const TCHAR* fhead, const TCHAR* ext)
{
	if (dir == NULL)
	{
		return NULL;
	}
	_tcscpy(tmfile, dir);
	int nLen = _tcslen(tmfile);
	if((nLen != 0) && (tmfile[nLen-1] != '\\'))
	{
		_tcscat(tmfile, _T("\\"));
		nLen++;
	}   
	if((fhead != NULL) && (*fhead != '\0'))
	{
		nLen += _tcslen(fhead);
		_tcscat(tmfile, fhead);
	}
	do 
	{
		_stprintf(tmfile + nLen, _T("%d"), _tempnum++);
		if(ext != NULL && *ext != '\0')
		{
			_tcscat(tmfile, _T("."));
			_tcscat(tmfile, ext);
		} 
#if defined(_WIN32_WCE)
	}while(GetFileAttributes(tmfile) != 0xFFFFFFFF);       
#else	
	}while(_taccess(tmfile, 0) != -1);       
#endif
	return tmfile;
}

//
double _atof(const TCHAR* str,int nLen/* = -1*/)
{
	TCHAR* pEnd;
	if(nLen <= -1)
	{
		nLen = _tcslen(str);
	}
	TCHAR Buff[32];
	memset(Buff, 0, sizeof(Buff));
	strcpyn(Buff, sizeof(Buff) - 1, str, nLen);
	LTrim(Buff);
	return _tcstod(Buff, &pEnd);
}

long _atol(const TCHAR* str,int nLen/* = -1*/)
{
	TCHAR Buff[32];
	TCHAR* pEnd;
	if(nLen == -1)
	{
		nLen = _tcslen(str);
	}
	memset(Buff, 0, sizeof(Buff));
	strcpyn(Buff, sizeof(Buff) - 1, str, nLen);
	LTrim(Buff);
	return _tcstol(Buff, &pEnd, 10);
}

int _strlen(const TCHAR* str, int nLen/* = -1*/)
{
	if(nLen == -1)
	{
		return _tcslen(str);
	}
	int i;
	for(i = 0; i < nLen; i++)
	{
		if(str[i] == '\0')
		{
			break;
		}
	}
	return i;
}

TCHAR* _strncpy(TCHAR* string1, const TCHAR* string2, int nCount)
{
	int  i;
	for(i = nCount-1; i >= 0; i--)
	{
		if(string2[i] != 0x20)
		{
			break;
		}
	}
	_tcsncpy(string1, string2, i+1);
	string1[i+1] = '\0';
	return string1;
}

void* smemcpy(void* dest, int nDest, const void* src, int count)
{
	count = min(nDest, count);
	char* p = (char*)src;
	count = min(strlen(p), count);
	memset(dest, 0x20, nDest);
	memcpy(dest, src, count);
	return dest;
}

/////////////////////////////////////////////////////////////////////////////
//非常安全的字符串复制函数
TCHAR* strcpyn(TCHAR* strDest, int nDest, const TCHAR* strSource, int nSource/* = -1*/, BOOL bTrim/* = FALSE*/)
{
	int i;
	if(nSource < 0)
	{
		nSource = _tcslen(strSource);
	}
	for(i = nSource - 1; i >= 0; i--)
	{
		if(!_istspace(strSource[i]) || (strSource[i] & 0x80) || !bTrim)
		{
			break;
		}
	}
	int nLen = min(i + 1, nDest);
	if(nLen > 0)
	{
		memcpy(strDest, strSource, nLen);
	}
	if(nLen < nDest)
	{
		strDest[nLen] = '\0';
	}
	return strDest;
}

int wstrcpyn(char* strDest, int nDest, const TCHAR* strSource, int nSource/* = -1*/)
{
	if(nSource < 0)
	{
		nSource = _tcslen(strSource);
	}
	int nLen = min(nSource, nDest);
	if(nLen > 0)
	{
		memset(strDest, 0, nDest);
#if defined(UNICODE)
		nLen = WideCharToMultiByte(CP_ACP, 0, strSource, -1, strDest, nDest, NULL, NULL) - 1;
#else
		memcpy(strDest, strSource, nLen);
#endif
	}
	return nLen;
}

#if !defined(_WIN32_WCE)
int _smkdir(const TCHAR* szPath)
{
	if(szPath == NULL || szPath[0] == '\0')
	{
		return -1;
	}
	TCHAR* dir;
	TCHAR fpath[_MAX_PATH];
	int oldDrive;
	int drive;
	TCHAR oldPath[_MAX_PATH];

	_tfullpath(fpath, szPath, _MAX_PATH);
	TCHAR* c = _tcsrchr(fpath, '\\');
	if(c != NULL)
	{
		*c = '\0';
	}
	if(_taccess(fpath, 0) == 0)
	{
		return 0;
	}
	dir = _tcstok(fpath, _T("\\"));
	dir[0] = _totupper(dir[0]);
	drive = dir[0] - 'A' + 1;
	oldDrive = _getdrive();
	_tgetcwd(oldPath,_MAX_PATH);

	_chdrive(drive);
	_tchdir(_T("\\"));
	dir = _tcstok(NULL, _T("\\"));
	while(dir)
	{
		if(_tchdir(dir))
		{
			_tmkdir(dir);
			_tchdir(dir);
		}
		dir = _tcstok(NULL, _T("\\"));
	}
	_chdrive(oldDrive);
	_tchdir(oldPath);
	return 0;
}

time_t LongToTime(unsigned long lDate)
{
	struct tm atm;
	memset(&atm, 0, sizeof(tm));
	TCHAR szTime[32];
	_stprintf(szTime, _T("%ld"), lDate);
	int nLen = _tcslen(szTime);
	if(nLen <= 8)
	{
		atm.tm_year = int(lDate/10000);
		if(atm.tm_year < 1900)
		{
			atm.tm_mon    = atm.tm_year/100 - 1;
			atm.tm_mday   = atm.tm_year%100;
			atm.tm_hour   = (int)((lDate%10000)/100);
			atm.tm_min    = (int)(lDate%100);
			time_t ct     = time(NULL);
			atm.tm_year   = localtime(&ct)->tm_year + 1900;
		}
		else
		{
			atm.tm_mon    = (int)((lDate%10000)/100) - 1;
			atm.tm_mday   = (int)(lDate%100);
			atm.tm_hour   = 0;
			atm.tm_min    = 0;
		}
	}
	else if(nLen > 8)
	{
		atm.tm_year = (int)(lDate/100000000 + 1990);
		int nTemp   = (int)(lDate%100000000)/10000;
		atm.tm_mon  = nTemp/100 - 1;
		atm.tm_mday = nTemp%100;
		atm.tm_hour = (int)((lDate%10000)/100);
		atm.tm_min  = (int)(lDate%100);
	}
	atm.tm_year -= 1900;
	if(atm.tm_mday < 1 || atm.tm_mday > 31 || 
		atm.tm_mon  < 0 || atm.tm_mon  > 11 ||
		atm.tm_hour < 0 || atm.tm_hour > 23 ||
		atm.tm_min  < 0 || atm.tm_min  > 59 ||
		atm.tm_sec  < 0 || atm.tm_sec  > 59)
	{
		return -1;
	}
	return mktime(&atm);
}

time_t StringToTime(const TCHAR* pszTime)
{
	if(pszTime == NULL || pszTime[0] == '\0')
	{
		return time(NULL);
	}
	struct tm atm;
	memset(&atm, 0, sizeof(tm));
	_stscanf(pszTime, _T("%d-%d-%d %d:%d:%d"), 
		&atm.tm_year, &atm.tm_mon, &atm.tm_mday, 
		&atm.tm_hour, &atm.tm_min, &atm.tm_sec);

	if(atm.tm_year > 2033 || atm.tm_year < 1980)
	{
		atm.tm_year = 2033;
	}
	atm.tm_year -= 1900;
	if(atm.tm_mday < 1 || atm.tm_mday > 31)
	{
		atm.tm_mday = 1;
	}
	atm.tm_mon--;
	if(atm.tm_mon < 0 || atm.tm_mon > 11)
	{
		atm.tm_mon = 0;
	}
	if(atm.tm_hour < 0 || atm.tm_hour > 23)
	{
		atm.tm_hour = 0;
	}
	if(atm.tm_min < 0 || atm.tm_min > 59)
	{
		atm.tm_hour = 0;
	}
	if(atm.tm_sec < 0 || atm.tm_sec > 59)
	{
		atm.tm_sec = 0;
	}
	return mktime(&atm);
}

static TCHAR szTime[32];
TCHAR* TimeToString(time_t tm)
{
	struct tm *ptm = localtime(&tm);
	szTime[0] = '\0';
	if(ptm != NULL)
	{
		_stprintf(szTime, _T("%d-%d-%d %d:%d:%d"), 
			ptm->tm_year, ptm->tm_mon, ptm->tm_mday, 
			ptm->tm_hour, ptm->tm_min, ptm->tm_sec);
	}
	return szTime;
}

#endif

int _wcmemcmp(const void* buf1, const void* buf2, size_t count1, size_t count2)
{
	if(buf1 == buf2)
	{
		return 0;
	}
	else if(buf1 == NULL && buf2 != NULL)
	{
		return -1;
	}
	else if(buf1 != NULL && buf2 == NULL)
	{
		return 1;
	}
	size_t i,j;
	const TCHAR* p1 = (const TCHAR*)buf1;
	const TCHAR* p2 = (const TCHAR*)buf2;
	if(count1 == -1)
	{
		count1 = _tcslen(p1);
	}
	if(count2 == -1)
	{
		count2 = _tcslen(p2);
	}

	for(i = 0, j = 0; (i < count1 && j < count2); i++, j++)
	{
		if(p1[i] == '*')
		{
			i++;
			if(p1[i] == '\0')
			{
				j = count2;
				count1 = i;
				break;
			}
			for(; j < count2; j++)
			{
				if(_totupper(p2[j]) == _totupper(p1[i]))
				{
					break;
				}
			}

		}
		else if(p2[j] == '*')
		{
			j++;
			if(p2[j] == '\0')
			{
				i = count1;
				count2 = j;
				break;
			}
			for(; i < count1; i++)
			{
				if(_totupper(p1[i]) == _totupper(p2[j]))
				{
					break;
				}
			}
		}
		else if(_totupper(p1[i]) != _totupper(p2[j]) && p1[i] != '?' && p2[j] != '?')
		{
			return p1[i] > p2[j] ? 1 : -1;
		}
	}
	if(i >= count1 && j < count2)// && (_tcschr(p2, '*') == NULL))
	{
		return -1;
	}
	else if(i < count1 && j >= count2)// && (_tcschr(p1, '*') == NULL))
	{
		return 1;
	}
	return 0;
}

int _mwcstrcmp(const TCHAR* psz1, const TCHAR* psz2, TCHAR cToken)
{
	if(psz1 == NULL || psz2 == NULL)
	{
		return 0;
	}
	int nLen = _tcslen(psz1);
	if(nLen <= 0)
	{
		return 0;
	}
	TCHAR* pTemp = new TCHAR[nLen + 1];
	memcpy(pTemp, psz1, nLen);
	pTemp[nLen] = '\0';
	TCHAR* p = pTemp;
	TCHAR szToken[2];
	szToken[0] = cToken;
	szToken[1] = '\0';
	p = _tcstok(p, szToken);
	int nRet = 0;
	while(p != NULL)
	{
		nRet = _wcmemcmp(p, psz2);
		if(nRet == 0)
		{
			break;
		}
		p = _tcstok(NULL, szToken);
	}
	delete[] pTemp;
	return nRet;
}

const TCHAR* RTrim(const TCHAR* strSrc)
{
	int sLen = _tcslen(strSrc);
	if (sLen !=0)
	{
		TCHAR* pstr = (TCHAR*)strSrc + sLen - 1;
		while(*pstr == ' ')  *pstr-- = '\0';
	}
	return strSrc;    
}

const TCHAR* LTrim(const TCHAR* strSrc)
{
	if( *strSrc == '\0')
	{
		return strSrc;
	}
	TCHAR* pHead= (TCHAR*)strSrc;
	TCHAR* pstr= (TCHAR*)strSrc;
	while(*(pstr++) == ' ');
	pstr--;
	if(strSrc != pstr)
	{
		int i;
		for( i = 0 ; *pstr != '\0'; i++,pstr++)
		{
			pHead[i] = *pstr;
		}
		pHead[i] = *pstr; 
	}
	return strSrc;
}

const TCHAR* AllTrim(const TCHAR* strSrc)
{
	return RTrim(LTrim(strSrc));
}

void strins(TCHAR* strPos, int c)
{
	int nLen = _tcslen(strPos);
	for(int i= nLen ; i >= 0; i--)
	{
		strPos[i+1] = strPos[i];
	}
	*strPos = c;
}

void InsertNumberSpace(TCHAR* strNumber,int cSpace/* = ','*/)
{
	if(*strNumber == '-')
	{
		strNumber++;
	}
	TCHAR* pDot = _tcschr(strNumber,'.');
	TCHAR* pEnd = _tcslen(strNumber) + strNumber;
	TCHAR* p;
	if(pDot != NULL)
	{
		for( p = pDot-3; p > strNumber; p -= 3)
		{
			strins(p,cSpace);
			pEnd++;
		}  
		pDot = _tcschr(strNumber,'.');
		for( p = pDot+4; p < pEnd; p += 4)
		{
			strins(p,cSpace);
			pEnd++;
		}  
	}
	else
	{
		for( p = pEnd-3; p > strNumber; p -= 3)
		{
			strins(p,cSpace);
		}
	}
}

void regulatePathString(TCHAR* pszPath)
{
	if(pszPath != NULL)
	{
		int nLen = _tcslen(pszPath);
		if(nLen > 0 && pszPath[nLen - 1] != '\\')
		{
			pszPath[nLen]	= '\\';
			pszPath[nLen+1] = '\0';
		}
	}
}

//////////////////////////////////////////////////////////////////////
//memory
void* MemoryAllocate(DWORD dwSize)
{
	if(dwSize <= 0)
	{
		return NULL;
	}
	return HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, dwSize);
}

void MemoryFree(void** lpMemory)
{
	if(*lpMemory == NULL)
	{
		return;
	}
	HeapFree(GetProcessHeap(), 0, *lpMemory);
	*lpMemory = NULL;
}


DWORD MemorySize(void* lpMemory)
{
	if(lpMemory == NULL)
	{
		return NULL;
	}
	return HeapSize(GetProcessHeap(), 0, lpMemory);
}

void* MemoryResize(void* lpMemory, DWORD dwNewSize)
{
	if(lpMemory == NULL)
	{
		return MemoryAllocate(dwNewSize);
	}
	else
	{
		return HeapReAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY | HEAP_REALLOC_IN_PLACE_ONLY, 
			lpMemory, dwNewSize);
	}
}

void* DebugGlobalAllocPtr(UINT fuAlloc, DWORD cbAlloc)
{
	char *p = new char[cbAlloc];
	memset(p, 0, cbAlloc);
	return p;
}

BOOL  DebugGlobalFreePtr(void *p)
{
	delete [] (char *)p;
	return TRUE;
}

static void ConvertStringToHex(const TCHAR *pszSrc, int nCount, TCHAR* pszDest, int nDest)
{
	if(pszSrc == NULL || nCount <= 0 || pszDest == NULL || nDest <= 0)
	{
		return;
	}
	int i;
	TCHAR *p = new TCHAR[2 * nCount + 1];
	for (i = 0; i < nCount; i++)
	{
		int n1 = pszSrc[i] % 16;
		int n2 = pszSrc[i] / 16;
		p[2 * i] = 'A' + n1;
		p[2 * i + 1] = 'A' + n2;
	}

	memset(pszDest, 0, nDest);
	p[2 * i] = '\0';
	strcpyn(pszDest, nDest, p);
	delete [] p;
}

static void ConvertHexToString(const TCHAR *pszSrc, int nCount, TCHAR* pszDest, int nDest)
{
	if(pszSrc == NULL || pszDest == NULL)
	{
		return;
	}
	int i,j;
	memset(pszDest, 0, nDest);
	for ( i = 0, j = 0; i < nDest; i++)
	{
		j = 2 * i;
		if(j >= (nCount - 1))
		{
			break;
		}

		int n1 = pszSrc[j] - 'A';
		int n2 = pszSrc[j + 1]  - 'A';
		pszDest[i] = n2 * 16 + n1;
	}
	pszDest[i] = '\0';
}

//static TCHAR GenRandomChar()
//{
//	srand((unsigned)time(NULL)); 
//	TCHAR ch = '\0';
//	while(ch == '\0')
//	{
//		ch = rand() % 256;
//	}
//	return ch;
//}

//static TCHAR g_EncodeBufffer[256];
//
//TCHAR* EncodeString(const TCHAR *pszSrc)
//{
//	TCHAR *p = new TCHAR[_tcslen(pszSrc) + 2];
//	p[0] = GenRandomChar();
//	_tcscpy(&p[1], pszSrc);
//	int nCount = _tcslen(p);
//	for (int i = nCount - 1; i > 0; i--)
//	{
//		p[i] = p[i - 1] ^ p[i];
//	}
//	
//	ConvertStringToHex(p, nCount, g_EncodeBufffer, sizeof(g_EncodeBufffer));
//	delete [] p;
//	return g_EncodeBufffer;
//}
//
//static TCHAR g_DecodeBufffer[256];
//
//TCHAR* DecodeString(const TCHAR *pszSrc)
//{
//	int nCount = _tcslen(pszSrc); /// 2;
//
//	memset(g_DecodeBufffer, 0, sizeof(g_DecodeBufffer));
//	if(nCount % 2)
//	{
//		return g_DecodeBufffer;
//	}
//	int nDest = nCount / 2 + 2;
//	TCHAR *p = new TCHAR[nDest];
//	
//	ConvertHexToString(pszSrc, nCount, p, nDest);
//	nDest -= 2;
//	for (int i = 1; i < nDest; i++)
//	{
//		p[i] = p[i - 1] ^ p[i];
//	}
//	p[i] = '\0';
//	strcpyn(g_DecodeBufffer, sizeof(g_DecodeBufffer), &p[1]);
//	delete [] p;
//	return g_DecodeBufffer;
//}

#if defined(_WIN32_WCE)
unsigned long time(unsigned long* ltime)
{
	SYSTEMTIME rTime;
	GetLocalTime( &rTime );
	return rTime.wMilliseconds + rTime.wSecond + rTime.wDay + rTime.wMonth + rTime.wYear;
}
#endif

#define SS_MINTIME				100000000	//最小时间

int	g_CRCtable[256] =  
{0x00000000, 0x77073096, 0xEE0E612C, 0x990951BA, 0x076DC419, 0x706AF48F, 0xE963A535, 0x9E6495A3,
0x0EDB8832, 0x79DCB8A4, 0xE0D5E91E, 0x97D2D988, 0x09B64C2B, 0x7EB17CBD, 0xE7B82D07, 0x90BF1D91,
0x1DB71064, 0x6AB020F2, 0xF3B97148, 0x84BE41DE, 0x1ADAD47D, 0x6DDDE4EB, 0xF4D4B551, 0x83D385C7,
0x136C9856, 0x646BA8C0, 0xFD62F97A, 0x8A65C9EC, 0x14015C4F, 0x63066CD9, 0xFA0F3D63, 0x8D080DF5,
0x3B6E20C8, 0x4C69105E, 0xD56041E4, 0xA2677172, 0x3C03E4D1, 0x4B04D447, 0xD20D85FD, 0xA50AB56B,
0x35B5A8FA, 0x42B2986C, 0xDBBBC9D6, 0xACBCF940, 0x32D86CE3, 0x45DF5C75, 0xDCD60DCF, 0xABD13D59,
0x26D930AC, 0x51DE003A, 0xC8D75180, 0xBFD06116, 0x21B4F4B5, 0x56B3C423, 0xCFBA9599, 0xB8BDA50F,
0x2802B89E, 0x5F058808, 0xC60CD9B2, 0xB10BE924, 0x2F6F7C87, 0x58684C11, 0xC1611DAB, 0xB6662D3D,
0x76DC4190, 0x01DB7106, 0x98D220BC, 0xEFD5102A, 0x71B18589, 0x06B6B51F, 0x9FBFE4A5, 0xE8B8D433,
0x7807C9A2, 0x0F00F934, 0x9609A88E, 0xE10E9818, 0x7F6A0DBB, 0x086D3D2D, 0x91646C97, 0xE6635C01,
0x6B6B51F4, 0x1C6C6162, 0x856530D8, 0xF262004E, 0x6C0695ED, 0x1B01A57B, 0x8208F4C1, 0xF50FC457,
0x65B0D9C6, 0x12B7E950, 0x8BBEB8EA, 0xFCB9887C, 0x62DD1DDF, 0x15DA2D49, 0x8CD37CF3, 0xFBD44C65,
0x4DB26158, 0x3AB551CE, 0xA3BC0074, 0xD4BB30E2, 0x4ADFA541, 0x3DD895D7, 0xA4D1C46D, 0xD3D6F4FB,
0x4369E96A, 0x346ED9FC, 0xAD678846, 0xDA60B8D0, 0x44042D73, 0x33031DE5, 0xAA0A4C5F, 0xDD0D7CC9,
0x5005713C, 0x270241AA, 0xBE0B1010, 0xC90C2086, 0x5768B525, 0x206F85B3, 0xB966D409, 0xCE61E49F,
0x5EDEF90E, 0x29D9C998, 0xB0D09822, 0xC7D7A8B4, 0x59B33D17, 0x2EB40D81, 0xB7BD5C3B, 0xC0BA6CAD,
0xEDB88320, 0x9ABFB3B6, 0x03B6E20C, 0x74B1D29A, 0xEAD54739, 0x9DD277AF, 0x04DB2615, 0x73DC1683,
0xE3630B12, 0x94643B84, 0x0D6D6A3E, 0x7A6A5AA8, 0xE40ECF0B, 0x9309FF9D, 0x0A00AE27, 0x7D079EB1,
0xF00F9344, 0x8708A3D2, 0x1E01F268, 0x6906C2FE, 0xF762575D, 0x806567CB, 0x196C3671, 0x6E6B06E7,
0xFED41B76, 0x89D32BE0, 0x10DA7A5A, 0x67DD4ACC, 0xF9B9DF6F, 0x8EBEEFF9, 0x17B7BE43, 0x60B08ED5,
0xD6D6A3E8, 0xA1D1937E, 0x38D8C2C4, 0x4FDFF252, 0xD1BB67F1, 0xA6BC5767, 0x3FB506DD, 0x48B2364B,
0xD80D2BDA, 0xAF0A1B4C, 0x36034AF6, 0x41047A60, 0xDF60EFC3, 0xA867DF55, 0x316E8EEF, 0x4669BE79,
0xCB61B38C, 0xBC66831A, 0x256FD2A0, 0x5268E236, 0xCC0C7795, 0xBB0B4703, 0x220216B9, 0x5505262F,
0xC5BA3BBE, 0xB2BD0B28, 0x2BB45A92, 0x5CB36A04, 0xC2D7FFA7, 0xB5D0CF31, 0x2CD99E8B, 0x5BDEAE1D,
0x9B64C2B0, 0xEC63F226, 0x756AA39C, 0x026D930A, 0x9C0906A9, 0xEB0E363F, 0x72076785, 0x05005713,
0x95BF4A82, 0xE2B87A14, 0x7BB12BAE, 0x0CB61B38, 0x92D28E9B, 0xE5D5BE0D, 0x7CDCEFB7, 0x0BDBDF21,
0x86D3D2D4, 0xF1D4E242, 0x68DDB3F8, 0x1FDA836E, 0x81BE16CD, 0xF6B9265B, 0x6FB077E1, 0x18B74777,
0x88085AE6, 0xFF0F6A70, 0x66063BCA, 0x11010B5C, 0x8F659EFF, 0xF862AE69, 0x616BFFD3, 0x166CCF45,
0xA00AE278, 0xD70DD2EE, 0x4E048354, 0x3903B3C2, 0xA7672661, 0xD06016F7, 0x4969474D, 0x3E6E77DB,
0xAED16A4A, 0xD9D65ADC, 0x40DF0B66, 0x37D83BF0, 0xA9BCAE53, 0xDEBB9EC5, 0x47B2CF7F, 0x30B5FFE9,
0xBDBDF21C, 0xCABAC28A, 0x53B39330, 0x24B4A3A6, 0xBAD03605, 0xCDD70693, 0x54DE5729, 0x23D967BF,
0xB3667A2E, 0xC4614AB8, 0x5D681B02, 0x2A6F2B94, 0xB40BBE37, 0xC30C8EA1, 0x5A05DF1B, 0x2D02EF8D}; 

void CalcCRC32 (unsigned char *p, unsigned long nbyte, unsigned long *crcvalue)
{	
	unsigned long	i = 0;
	for (i = 0; i < nbyte; i++)
	{
		(*crcvalue) = ((*crcvalue) >> 8) ^ g_CRCtable[ unsigned char(p[i] ^ ((*crcvalue) & 0x000000ff)) ];
	}
}
//#define _HS_WRITE_LOG_
#ifdef _HS_WRITE_LOG_
void HS_WRITE_LOG(char *filename)
{
	long			fd = 0;
	fd = _open("c:\\2.txt", _O_RDWR|_O_TEXT, 0); 
	if (fd == -1)  
	{
		return;
	}
	_lseek(fd, 0, SEEK_END);	
	_write(fd, filename, strlen(filename));
	_close(fd);
}
#endif
unsigned long CalcFileCRC32(char *filename, long start/* = -1*/, long end /*= -1*/)
{
	unsigned long	n = 0; 
	unsigned long	result	= 0x00000000;
	unsigned char	*buffer = NULL;

	long			fd = 0;
	long			nFileLen = 0;

#ifdef _HS_WRITE_LOG_
	HS_WRITE_LOG(filename);
#endif
	fd = _open(filename, _O_RDONLY|_O_TEXT, 0);
	if (fd == -1)
	{
#ifdef _HS_WRITE_LOG_
		HS_WRITE_LOG("\r\ncann't open file");
#endif
		return(result);
	}

	nFileLen	= _filelength(fd);//_lseek(fd, 0, SEEK_END);	
	start		= ((start < 0 || start > nFileLen) ? 0 : start);
	end			= ((end < 0 || end > nFileLen) ? nFileLen : end);

	if ( start >= end )
	{
#ifdef _HS_WRITE_LOG_
		HS_WRITE_LOG("\r\nstart >= end ");
#endif
		_close(fd);
		return result;
	}

	buffer = (unsigned char *)malloc(end - start);
#ifdef _HS_WRITE_LOG_
	char strlog[128];
	memset(strlog, 0, 127);
	sprintf(strlog, "\r\nmalloc:  %d",  end-start);
	HS_WRITE_LOG(strlog);
#endif	
	_lseek(fd, start, SEEK_SET);
	n = _read(fd, buffer, end - start);
	_close(fd);
#ifdef _HS_WRITE_LOG_

	memset(strlog, 0, 127);
	sprintf(strlog, "\r\nRead: %d , File Len: %d", n, end-start);
	HS_WRITE_LOG(strlog);
#endif
	if( n > 0)
	{
#if 1
		CalcCRC32(buffer, n, &result);
#else
		unsigned long	i = 0;
		for (i = 0; i < n; i++)
		{
			result = ((result >> 8) ^ g_CRCtable[ unsigned char(buffer[i] ^ (result & 0x000000ff)) ]);
		}
#endif
#ifdef _HS_WRITE_LOG_
		memset(strlog, 0, 127);
		sprintf(strlog, "\r\nCRC: %d", result);
		HS_WRITE_LOG(strlog);
#endif
	}

	//	_close(fd);
	free(buffer);

	return(result);
}