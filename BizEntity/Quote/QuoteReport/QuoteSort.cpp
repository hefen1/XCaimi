#include "StdAfx.h"
#include "QuoteSort.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

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

void FindStockPos(CArray<StockUserInfo*,StockUserInfo*>& payStock,CodeInfo& codeinfo,int& nPos)
{
	int nTotal = payStock.GetSize();
	if( nTotal <= 0 )
		return;
	if (codeinfo.m_cCodeType<=0 || codeinfo.GetCode().IsEmpty())
	{
		return;
	}
	

	StockUserInfo* pStock;
	for(int i = 0; i < nTotal; i++)
	{
		pStock = payStock.GetAt(i);

		if( pStock && 
			!memcmp(&pStock->m_ciStockCode,&codeinfo,sizeof(codeinfo)) )
		{
			nPos = i;
			break;
		}
	}
}
