/*******************************************************
  Դ��������:TradeFunc.h
  �������Ȩ:�������ӹɷ����޹�˾
  ϵͳ����:  Ͷ��Ӯ�ҽ�������ն�V1.0
  ģ������:  ����
  ����˵��:  ��̬����
  ��    ��:  shenglq
  ��������:  20100910
  *********************************************************/
#pragma once
#include "..\HsStructnew.h"
typedef CString TradeStockMarket;			  

#define SHACCOUNT "1"
#define SZACCOUNT "2"
#define SHBACCOUNT "D"
#define SZBACCOUNT "H"
#define SBACCOUNT "9"
#define SBBACCOUNT "A"

static TradeStockMarket MakeStockMarket(HSMarketDataType x)
{
	CString sMarket = SHACCOUNT;
	if (MakeMarket(x) == STOCK_MARKET)
	{
		switch (MakeMidMarket(x))
		{
		case SH_BOURSE:
			{
				switch (MakeSubMarket(x))
				{
				case KIND_STOCKB:
					{
						sMarket = SHBACCOUNT;
						break;
					}
				default:
					{
						sMarket = SHACCOUNT;
					}
				}				 
				break;
			}
		case SZ_BOURSE:
			{
				switch (MakeSubMarket(x))
				{
				case KIND_STOCKB:
					{
						sMarket = SZBACCOUNT;
						break;
					}
				case KIND_THREEBOAD:
					{
						sMarket = SBACCOUNT;
						break;
					}
				default:
					{
						sMarket = SZACCOUNT;
					}
				}				 
				break;
			}
		}
	}
	return sMarket;
}

//��MFC CString֮Find�����ַ������·�����һ���ַ����ֽ��ŵ�һ��CStringArray��
static void ExtractString(CStringArray& arr, const CString strSrc, const CString sep = "\r\n" )
{
	CString str(strSrc);
	if(str.IsEmpty())
		return;

	// ��ʼ�ֽ�
	int pos = str.Find(sep);
	if(pos == -1) // δ�ҵ��ָ���
		arr.Add(str);
	else    // �ҵ��ָ���
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
}

static DWORD MyGetTickCount()
{
	LARGE_INTEGER tFrequency, tCounter;
	if (QueryPerformanceFrequency(&tFrequency))
	{
		QueryPerformanceCounter(&tCounter);
		return DWORD(tCounter.QuadPart * 1000 / tFrequency.QuadPart);
	}
	else
		return GetTickCount();
	
}