/*******************************************************
  源程序名称:PackEntry.cpp
  软件著作权:恒生电子股份有限公司
  系统名称:  投资赢家金融理财终端V1.0
  模块名称:  交易中心
  功能说明:  Ar1打包解包器
  作    者:  shenglq
  开发日期:  20100830
*********************************************************/
#include "stdafx.h"
#include "Ar1Packet.h"
#include "TradeFunc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

/*//用MFC CString之Find处理字符串以下方法将一个字符串分解后放到一个CStringArray中
void ExtractString(CStringArray& arr, const CString strSrc, const CString sep = "\r\n" )
{
	CString str(strSrc);
	if(str.IsEmpty())
		return;

	// 开始分解
	int pos = str.Find(sep);
	if(pos == -1) // 未找到分隔符
		arr.Add(str);
	else    // 找到分隔符
	{
		//str += sep;     // think

		CString s;
		while (pos != -1)
		{
			s = str.Left(pos);
			if(!s.IsEmpty())
			{
				arr.Add(s);
			}
			else
			{
				arr.Add("");
			}

			str = str.Mid(pos + sep.GetLength());
			pos = str.Find(sep);
		}
	}
}*/

void InitHeader(AR1PacketHeader* pHeader)
{
	pHeader->HeaderVersion = 1;
	pHeader->PacketType = 0;
	pHeader->Branch = 0;
	pHeader->FunctionNo = 0;
	pHeader->SenderID = 0;
	pHeader->AddressLevel = 0;
	pHeader->DataLength = 0;
	pHeader->ReturnCode = 0;
}

CAr1Packer::CAr1Packer()
{
	m_Header = new AR1PacketHeader;
	InitHeader(m_Header);
}

CAr1Packer::~CAr1Packer()
{
	delete m_Header;
}

AR1PacketHeader* CAr1Packer::GetHeader()
{
	return m_Header;
}

void CAr1Packer::AddData(const CString sField, const CString sValue)
{
	m_Ar1DataList.SetAt(sField, sValue);

}

char* CAr1Packer::GetPackBuf(int& iLen)
{
	CString sFieldBuf = "";
	CString sDataBuf = "";
	POSITION ipos = m_Ar1DataList.GetStartPosition();
	while(ipos!=NULL)
	{
		CString sField;
		CString sValue;
		m_Ar1DataList.GetNextAssoc(ipos,sField,sValue);
		sFieldBuf += sField;
		sFieldBuf += SOH;
		sDataBuf += sValue;
		sDataBuf += SOH;
	} 
	CString sColRow;
	sColRow.Format("%d",m_Ar1DataList.GetCount());
	CString sBuf = sColRow;
	sBuf +=  SOH;
	sBuf += "1";
	sBuf += SOH;	
	sBuf += sFieldBuf;
	sBuf += sDataBuf;
	m_Header->DataLength = sBuf.GetLength();
	iLen = PacketAR1Size + m_Header->DataLength;
	char* lpBuf = new char[iLen];
	memcpy(lpBuf, m_Header, PacketAR1Size);
	memcpy(lpBuf + PacketAR1Size, sBuf, m_Header->DataLength);
	return lpBuf;
}

//=========================================================================================

CAr1UnPacker::CAr1UnPacker()
{
	m_CurrRow = 0;
	m_nRowCount = 0;
	m_nColCount = 0;
	m_Header = new AR1PacketHeader;
}

CAr1UnPacker::~CAr1UnPacker()
{
	delete m_Header;
	Clear();

}
void CAr1UnPacker::Clear()
{
	for (int i = 0; i < m_Ar1DataList.GetCount(); i++)
	{
		delete(m_Ar1DataList[i]);
	}
	m_Ar1DataList.RemoveAll();
}

BOOL CAr1UnPacker::open(char* lpBuf, int iLen)
{

	DWORD nBegin = MyGetTickCount();
	DWORD nEnd = 0;
	TRACE1(_T("Ar1UnPacker Begin：%d,\n"), nBegin);

	
	Clear();
	m_CurrRow = 0;
	m_nColCount = 0;
	m_nRowCount = 0;
	CStringArray sArray;
	memcpy(m_Header, lpBuf, PacketAR1Size);
	int iBufLen = iLen - PacketAR1Size + 1;
	char* szBuf = new char[iBufLen];
	memcpy(szBuf, lpBuf + PacketAR1Size, iBufLen - 1);
	szBuf[iBufLen - 1] = SOH;
	int nPos = -1;
	for (int i = 0; i < iBufLen; i++)
	{
		if (SOH == szBuf[i])
		{
			szBuf[i] = '\0';
			if ((i - nPos) == 1)
			{
				sArray.Add("");
			}
			else		
			{
				CString str = "";
				char* szTmp = new char[i - nPos];
				memcpy(szTmp, szBuf + nPos + 1, i - nPos);
				str = szTmp;
				delete[] szTmp;
				szTmp = NULL;
				sArray.Add(str);
			}
			nPos = i;			      
		}
	}
	delete[] szBuf;
	szBuf = NULL;

	nEnd = MyGetTickCount();
	TRACE2(_T("Ar1UnPacker End：%d, UseTime：%d\n"), nEnd, nEnd - nBegin);


	if (sArray.GetCount() >= 2)
	{
		CString sTmp = "";
		sTmp = sArray[0];
		m_nColCount = atoi(sTmp);
		sTmp = sArray[1];
		m_nRowCount = atoi(sTmp);

	}
	else
		return FALSE;
	for (int i = 0; i < m_nRowCount; i++)
	{
		CMapStringToString* pRowData = new CMapStringToString;
		pRowData->InitHashTable(UINT(m_nColCount * 1.2));
		for (int j = 2; j < (m_nColCount + 2); j++)
		{
			CString sField = sArray[j];
			CString sValue = sArray[(i + 1) * m_nColCount + j];
			pRowData->SetAt(sField, sValue);
		}
		m_Ar1DataList.Add(pRowData);
	}


	nEnd = MyGetTickCount();
	TRACE2(_T("Ar1UnPacker FillList End：%d, UseTime：%d\n"), nEnd, nEnd - nBegin);

	return TRUE; 
}

AR1PacketHeader* CAr1UnPacker::GetHeader()
{
	return m_Header;
	InitHeader(m_Header);
}

BOOL CAr1UnPacker::isEof()
{	
	return m_CurrRow >= m_nRowCount;
}

void CAr1UnPacker::Next()
{
	m_CurrRow++;
}

CMapStringToString* CAr1UnPacker::GetItem()
{
	if (!isEof())
	{
		return m_Ar1DataList[m_CurrRow];
	}
	else
		return NULL;
}

int CAr1UnPacker::GetColCount()
{
	return m_nColCount;
}

int CAr1UnPacker::GetRowCount()
{
	return m_nRowCount;
}

//===========================================================================================

CAr1Packet::CAr1Packet()
{
	m_Ar1Packer = NULL;
	m_Ar1UnPacker = NULL;

}

CAr1Packet::~CAr1Packet()
{
	if (m_Ar1Packer != NULL)
	{
		delete m_Ar1Packer;
	}
	if (m_Ar1UnPacker != NULL)
	{
		delete m_Ar1UnPacker;
	}

}

CAr1Packer* CAr1Packet::GetAr1Packer()
{
	if (m_Ar1Packer == NULL)
	{
		m_Ar1Packer = new CAr1Packer;
	}
	return m_Ar1Packer;
}

CAr1UnPacker* CAr1Packet::GetAr1UnPacker(char* lpBuf, int iLen)
{
	if (m_Ar1UnPacker == NULL)
	{
		m_Ar1UnPacker = new CAr1UnPacker;
	}	 
	m_Ar1UnPacker->open(lpBuf, iLen);
	return m_Ar1UnPacker;
}
void CAr1Packet::FreePacker()
{
	if (m_Ar1Packer != NULL)
	{
		delete m_Ar1Packer;
		m_Ar1Packer = NULL;
	}
}

void CAr1Packet::FreeUnPacker()
{
	if (m_Ar1UnPacker != NULL)
	{
		delete m_Ar1UnPacker;
		m_Ar1UnPacker = NULL;
	}
}