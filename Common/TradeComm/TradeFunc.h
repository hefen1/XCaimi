/*******************************************************
  源程序名称:TradeFunc.h
  软件著作权:恒生电子股份有限公司
  系统名称:  投资赢家金融理财终端V1.0
  模块名称:  交易
  功能说明:  静态函数
  作    者:  shenglq
  开发日期:  20100910
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

//用MFC CString之Find处理字符串以下方法将一个字符串分解后放到一个CStringArray中
static void ExtractString(CStringArray& arr, const CString strSrc, const CString sep = "\r\n" )
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