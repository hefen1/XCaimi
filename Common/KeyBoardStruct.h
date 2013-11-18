/*************************************************************
*	Copyright (c)2010, 恒生电子股份有限公司
*	All rights reserved.
*
*	文件名称：	keyboardstruct.h
*	文件标识：	键盘精灵公共头文件
*	摘	  要：	
*
*	当前版本：	1.0
*	作	  者：	
*	更新日期：	
*
*	修改说明：	
*
***************************************************************/

#ifndef KEYBOARDH
#define KEYBOARDH

#include "QuoteStuctDefine.h"

//页面类型

#define PageInfo_Quote      0x001     //报价
#define PageInfo_Tech       0x002	  //k线 分时
#define PageInfo_Other      0x003     //其他

//热键id
#define KeyBegin			99
#define KeyMainMenu			(KeyBegin+0)               //主菜单
#define KeyInfoData			(KeyBegin+1)            //信息地雷
#define KeyQuickBuy			(KeyBegin+2)            //闪电买
#define KeyQuickSell		(KeyBegin+3)           //闪电卖出
#define KeyQuoteAdd			(KeyBegin+4)           //叠加大盘指数
#define KeyShangAQuote		(KeyBegin+14)			//上A报价
#define KeyShangBQuote		(KeyBegin+15)			//上B报价
#define KeyShengAQuote		(KeyBegin+16)		    //深A报价
#define KeyShengBQuote		(KeyBegin+17)			//深B报价
#define KeyShangBondQuote   (KeyBegin+18)           //上债报价
#define KeyShengBondQuote   (KeyBegin+19)           //深债报价
#define KeyShangFundQuote	(KeyBegin+20)			//上金报价
#define KeyShengFundQuote	(KeyBegin+21)			//深金报价
#define KeySmallStocksQuote  (KeyBegin+22)			//中小盘报价
#define KeyGEM              (KeyBegin+32)           //创业板
#define KeyGEMRange			(KeyBegin+33)			//创业板涨幅
#define KeyARange			(KeyBegin+34)			//A股涨幅
#define KeyAGeneral			(KeyBegin+35)			//A股综合			
#define KeyShangARange		(KeyBegin+36)			//上A涨幅
#define KeyShangBRange		(KeyBegin+37)			//上B涨幅
#define KeyShengARange		(KeyBegin+38)			//深A涨幅
#define KeyShengBRange		(KeyBegin+39)			//深B涨幅
#define KeyShangBondRange	(KeyBegin+40)			//上债涨幅
#define KeyShengBondRange	(KeyBegin+41)			//深债涨幅	
#define KeyShangFundRange	(KeyBegin+42)			//上金涨幅
#define KeyShengFundRange	(KeyBegin+43)			//深金涨幅
#define KeySmallStocksRange	(KeyBegin+44)			//中小盘涨幅
#define KeyShangAGeneral	(KeyBegin+45)			//上A综合
#define KeyShangBGeneral	(KeyBegin+46)			//上B综合
#define KeyShengAGeneral	(KeyBegin+47)			//深A综合
#define KeyShengBGeneral	(KeyBegin+48)			//深B综合
#define KeyShangBondGeneral	(KeyBegin+49)			//上债综合
#define KeyShengBondGeneral	(KeyBegin+50)			//深债综合
#define KeySmallStocksGeneral	(KeyBegin+51)		//中小盘综合
#define KeyAStocks1Minute	(KeyBegin+52)			//A股综合1分钟
#define KeyAStocks2Minute	(KeyBegin+53)			//A股综合2分钟
#define KeyAStocks3Minute	(KeyBegin+54)			//A股综合3分钟
#define KeyAStocks4Minute	(KeyBegin+55)			//A股综合4分钟
#define KeyAStocks5Minute	(KeyBegin+56)			//A股综合5分钟
#define KeyAStocks10Minute	(KeyBegin+57)			//A股综合10分钟
#define KeyAStocks15Minute	(KeyBegin+58)			//A股综合15分钟

#define KeyF1				(KeyBegin+61)	
#define KeyF2				(KeyBegin+62)	
#define KeyF3				(KeyBegin+63)	
#define KeyF4				(KeyBegin+64)	
#define KeyF5				(KeyBegin+65)	
#define KeyF6				(KeyBegin+66)	
#define KeyF7				(KeyBegin+67)	
#define KeyF8				(KeyBegin+68)	
#define KeyCtrlX			(KeyBegin+69)	
#define KeyF10				(KeyBegin+70)	

#define KeyOneMinute		(KeyBegin+101)			//1分钟线
#define KeyFiveMinute		(KeyBegin+102)			//5分钟线
#define Key15Minute			(KeyBegin+103)            //15分钟线
#define Key30Minute			(KeyBegin+104)			//30分钟
#define Key60Minute			(KeyBegin+105)			//60分钟
#define KeyDayData			(KeyBegin+106)			//日线
#define KeyWeekData			(KeyBegin+107)			//周线
#define KeyYearData			(KeyBegin+108)			//年线
#define KeyMultiDayData		(KeyBegin+109)			//多日线

// 键盘精灵
struct HSShowKeyboardInfo 
{
	enum Type{left = 0x1,top = 0x2,right = 0x4,bottom = 0x8,
		all = 0x10,stock = 0x20,key = 0x40,express = 0x80};

	CRect   m_oRect;
	CWnd*   m_pWnd;
	WORD	m_cStyle;
	WORD    m_nPageType;

	// 键盘列表过滤
	HSMarketDataType	m_GP; // 股票市场
	HSMarketDataType	m_GG; // 港股
	HSMarketDataType	m_WH; // 外汇
	HSMarketDataType	m_WP; // 外盘
	HSMarketDataType	m_QH; // 期货

	char m_cShortKey;  // 快键
	char m_cExpress;   // 指标/公式选择
	char m_cEmptyAll;  // 清空所有
	char m_cDispAll;   // 显示所有

	char m_cResever[7];   // 保留

	HSShowKeyboardInfo(){memset(this,0,sizeof(HSShowKeyboardInfo));}
	BOOL IsStyle(WORD cStyle) { return (m_cStyle & cStyle); }
};

struct HSCurKeyInfo
{
	// wParam 数值
	enum
	{
		KeyStock = 0x1000,	    // 股票
		KeyDataQuick,				// 1~6、61~66、81~86...
		UpdateExpressData,	    // 更新公式
		DragUpdateExpressData2,	// 托放更新公式
		DragUpdateExpressData3,	// 托放更新公式
		OpenBlock,			    // 打开板块
		OpenUnionTechChart,     // 打开指标图组合
		UpDataUnionTechChart,   // 更新指标图组合
		OpenUsrDefPage,		    // 打开定义的页面
		OpenIE,				    // 打开ie
		UpdateDefExpress,		// 更新常用指标状态
		PreSynchronization,		// 刷新公式前的处理
		OpenHKBrokerTip,		// 打开港股经纪信息TIP 加入 2004年7月6日
		OpenHKBrokerDeal,		// 打开港股经纪追踪页面
		OpenHKStock,			// 打开港股股票
		GetSplSize,				// 得到拆分窗口大小,在拆分中使用
		KeyDataEx,				// 特别的打开处理
		KeyUsedViewInfo,		// 键盘精灵打开，传送包含PageViewInfo数据，主要在自定义键盘里使用
		KeyCommand,
		KeyStockAdd             //股票叠加
	};

	int			m_nOpen;	 // 打开类型,具体参照函数：void CKeyboardDataList::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
	char		m_szText[256];   // 文本(指标名或Key名)
	CodeInfo    m_cCode;     // 股票信息

	PageViewInfo m_pInfo;
	HSCurKeyInfo();
	CodeInfo* GetCode();
	void Empty();
	BOOL IsEmpty();

	void Copy(const HSCurKeyInfo* pCode);
	void Copy(const char* strText);
	void Copy(const CodeInfo* pCode);
	void Copy(const PageViewInfo* pCode);

	CString GetText();
	BOOL    IsText();
	BOOL	IsBlock();
};
#endif