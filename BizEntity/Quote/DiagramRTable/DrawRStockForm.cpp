#include "StdAfx.h"
#include "resource.h"
#include "DrawRStockForm.h"
#include <atlcore.h>
#include "GeneralHelper.h"
#include "HSCalculate.h"
#include "..\..\..\ServiceEntity\HSFormulaManDll\FormulaCalc\ExpPublic.h"
#include "HSMessage.h"
#include "HSWinner.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// 右键菜单ID
#define ID_QUICKTRADE_BUY   0x0001
#define ID_QUICKTRADE_SELL  0x0002

CDrawRStockForm::CDrawRStockForm(IDataSourceEx* pDataSource, BOOL bLevel)
:CDrawTableBase()
{
	SetStyle(TABLE_STYLE_SCREEN | TABLE_HEIGHT_CHANGABLE | TABLE_WIDTH_CHANGABLE);
	m_preCodeInfo.SetEmpty();	
	m_sLevelData.m_ciStockCode.SetEmpty();
	m_pDataSource = pDataSource;
	m_bLevel = bLevel;
	m_nNewPrice = 0;
	//m_bLevel = TRUE;
}

CDrawRStockForm::~CDrawRStockForm()
{
	
}

BOOL CDrawRStockForm::CreateSelf()
{
	WORD nMarket = MakeMarket(m_stkInfo.m_ciStockCode.m_cCodeType);
	if (nMarket == STOCK_MARKET)
	{// 股票市场
		WORD nSubMarket = MakeSubMarket(m_stkInfo.m_ciStockCode.m_cCodeType);
		switch (nSubMarket)
		{
		case KIND_BOND:
			return CreateBonds();
		case KIND_FUND:
		case KIND_LOF:
			return CreateFund();
		case KIND_ETF:
			return CreateEFT();
		case KIND_INDEX:
			return CreateIndex();
		default:
			return CreateStock();
		}
	}
	else if (nMarket == FUTURES_MARKET)
	{// 期货
		return CreateFuture();
	}
	return FALSE;
}

BOOL CDrawRStockForm::CreateBonds() // 创建债券五档
{
	CString strTitle = "";
	CString strName = "";
	DWORD lItemStyle(0); // 单元小格格式
	DWORD lCellStyle(0); // 单元格格式

	//////////////////////////////////////////////////////////////////////////
	// 读取资源字符串
	CAtlBaseModule _Module;
	//CComModule _Module;
	HINSTANCE instance = _Module.GetResourceInstance();
	char* buf = new char[1000];
	int   nBufLenth = 1000;
	//

	////////////////////////////  标题开始  //////////////////////////////////////
	lItemStyle = ITEM_TEXT_LEFT;
	lCellStyle = CELL_BORDER_BOTTOM;
	::LoadString(instance, IDS_STRING_WEIBI, buf, nBufLenth);    //委比
	strName = buf;
	CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, lCellStyle);

	if (!m_bLevel)
	{// level1 显示五档行情
		lItemStyle = ITEM_TEXT_LEFT;
		::LoadString(instance, IDS_STRING_SELL, buf, nBufLenth);
		strTitle = buf;					
		for (int i = 4; i >= 1; i--)
		{
			// 卖5~卖2
			::LoadString(instance, IDS_STRING_NUM1+i, buf, nBufLenth);
			strName = buf;
			CreateCell(strTitle+strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, 0);
		}
		// 卖1
		lItemStyle = ITEM_TEXT_LEFT;
		lCellStyle = CELL_BORDER_BOTTOM;
		::LoadString(instance, IDS_STRING_NUM1, buf, nBufLenth);
		strName = buf;
		CreateCell(strTitle+strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, lCellStyle);

		lItemStyle = ITEM_TEXT_LEFT;
		::LoadString(instance, IDS_STRING_BUY, buf, nBufLenth);
		strTitle = buf;
		for (int i = 0; i < 4; i++)
		{
			// 买1~买4
			::LoadString(instance, IDS_STRING_NUM1+i, buf, nBufLenth);
			strName = buf;
			CreateCell(strTitle+strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, 0);
		}
		// 买5
		lItemStyle = ITEM_TEXT_LEFT;
		lCellStyle = CELL_BORDER_BOTTOM;
		::LoadString(instance, IDS_STRING_NUM1+4, buf, nBufLenth);
		strName = buf;
		CreateCell(strTitle+strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, lCellStyle);
	}

	lItemStyle = ITEM_TEXT_LEFT ;
	::LoadString(instance, IDS_STRING_CHENGJIAO, buf, nBufLenth);  //成交
	strName = buf;  
	CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, 0);

	::LoadString(instance, IDS_STRING_ZHANGDIE, buf, nBufLenth);   //涨跌
	strName = buf;
	CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, 0);

	::LoadString(instance, IDS_STRING_FUDU, buf, nBufLenth);       //幅度
	strName = buf;
	CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, 0);

	::LoadString(instance, IDS_STRING_ZONGSHOU, buf, nBufLenth);   //总手
	strName = buf;
	CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, 0);

	::LoadString(instance, IDS_STRING_XIANSHOU, buf, nBufLenth);   //现手
	strName = buf;
	CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, 0);

	lItemStyle = ITEM_TEXT_LEFT;
	lCellStyle = CELL_BORDER_BOTTOM;
	::LoadString(instance, IDS_STRING_ZONGE, buf, nBufLenth);      //总额
	strName = buf;
	CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, lCellStyle);

	lCellStyle = CELL_BORDER_LEFT;
	::LoadString(instance, IDS_STRING_LIXI, buf, nBufLenth);       //利息
	strName = buf;                    
	CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, lCellStyle);

	if (m_bLevel)
	{// level2 要显示总笔
		lCellStyle = CELL_BORDER_LEFT;
		::LoadString(instance, IDS_STRING_ZONGBI, buf, nBufLenth);  // 总笔
		strName = buf;
		CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, lCellStyle);
	}

	lItemStyle = ITEM_TEXT_LEFT;
	lCellStyle = CELL_BORDER_BOTTOM;
	::LoadString(instance, IDS_STRING_ZHANGTING, buf, nBufLenth);  //涨停
	strName = buf;                            
	CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, lCellStyle);

	lItemStyle = ITEM_TEXT_LEFT;
	::LoadString(instance, IDS_STRING_SHIYING, buf, nBufLenth);    //市盈
	strName = buf;
	CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, 0);

	::LoadString(instance, IDS_STRING_LIUTONG, buf, nBufLenth);    //流通
	strName = buf;
	CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, 0);

	::LoadString(instance, IDS_STRING_SHIZHI, buf, nBufLenth);     //市值
	strName = buf;
	CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, 0);

	//		TRACE1("第一列 %d \n", m_ayCell.GetSize());
	////////////////////////////////////// 第二列  ////////////////////////////////////////
	lItemStyle = ITEM_TEXT_RIGHT;
	lCellStyle = CELL_BORDER_BOTTOM;
	CreateCell("",  0, TEXT_FONT_DYNAMICAL, lItemStyle,lCellStyle);                  //委差

	if (!m_bLevel)
	{// level1显示五档行情
		lItemStyle = ITEM_TEXT_RIGHT | ITEM_TEXT_SELECT;
		for (int i = 0; i < 4; i++)
		{
			//卖5~卖2				
			CreateCell("", 0, TEXT_FONT_DYNAMICAL, lItemStyle, 0);
		}
		// 卖1
		lItemStyle = ITEM_TEXT_RIGHT | ITEM_TEXT_SELECT;
		lCellStyle = CELL_BORDER_BOTTOM;
		CreateCell("", 0, TEXT_FONT_DYNAMICAL, lItemStyle, lCellStyle);

		lItemStyle = ITEM_TEXT_RIGHT | ITEM_TEXT_SELECT;
		for (int i = 0; i < 4; i++)
		{
			// 买1~买4				
			CreateCell("", 0, TEXT_FONT_DYNAMICAL, lItemStyle, 0);
		}
		// 买5
		lItemStyle = ITEM_TEXT_RIGHT | ITEM_TEXT_SELECT;
		lCellStyle = CELL_BORDER_BOTTOM;
		CreateCell("", 0, TEXT_FONT_DYNAMICAL, lItemStyle, lCellStyle);
	}
	
	lItemStyle = ITEM_TEXT_RIGHT | ITEM_TEXT_SELECT ;                                   
	CreateCell("", 0, TEXT_FONT_DYNAMICAL, lItemStyle, 0);                     // 成交价

	lItemStyle = ITEM_TEXT_RIGHT ;
	CreateCell("", 0, TEXT_FONT_DYNAMICAL, lItemStyle, 0);	                   // 涨跌

	CreateCell("", 0, TEXT_FONT_DYNAMICAL, lItemStyle, 0);	                   // 幅度

	CreateCell("", TEXT_COLOR_TOTALHAND, TEXT_FONT_DYNAMICAL, lItemStyle, 0);  // 总手

	CreateCell("", TEXT_COLOR_XIANHAND, TEXT_FONT_DYNAMICAL, lItemStyle, 0);   // 现手

	lItemStyle = ITEM_TEXT_RIGHT;
	lCellStyle = CELL_BORDER_BOTTOM;
	CreateCell("", TEXT_COLOR_CHENGJIAO, TEXT_FONT_DYNAMICAL, lItemStyle, lCellStyle); // 总额

	lItemStyle = ITEM_TEXT_RIGHT;
	CreateCell("", TEXT_COLOR_UP, TEXT_FONT_DYNAMICAL, lItemStyle, 0);                 // 利息

	if (m_bLevel)
	{// level2 显示总笔                                           
		lItemStyle = ITEM_TEXT_RIGHT;
		CreateCell("", TEXT_COLOR_ZONGBI, TEXT_FONT_DYNAMICAL, lItemStyle, 0);         // 总笔
	}

	lItemStyle = ITEM_TEXT_RIGHT;
	lCellStyle = CELL_BORDER_BOTTOM;
	CreateCell("", TEXT_COLOR_UP, TEXT_FONT_DYNAMICAL, lItemStyle, lCellStyle);        // 涨停

	lItemStyle = ITEM_TEXT_RIGHT;
	CreateCell("", TEXT_COLOR_UP, TEXT_FONT_DYNAMICAL, lItemStyle, 0);  	           // 市盈

	CreateCell("", TEXT_COLOR_UP, TEXT_FONT_DYNAMICAL, lItemStyle, 0);	               // 流通

	CreateCell("", TEXT_COLOR_UP, TEXT_FONT_DYNAMICAL, lItemStyle, 0);                 // 市值
	//		TRACE1("第二列 %d \n", m_ayCell.GetSize());
	//////////////////////////////////////// 第三列开始 //////////////////////////////////////////
	lItemStyle = ITEM_TEXT_LEFT;
	lCellStyle = CELL_BORDER_BOTTOM;
	::LoadString(instance, IDS_STRING_WEICHA, buf, nBufLenth);       // 委差
	strName = buf;
	CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, lCellStyle);

	if (!m_bLevel)
	{// level1 显示五档行情报价
		lItemStyle = ITEM_TEXT_RIGHT;   
		for (int i = 0; i < 4; i++)
		{	// 卖5~卖2
			CreateCell("", TEXT_COLOR_XIANHAND, TEXT_FONT_DYNAMICAL, lItemStyle, 0); 
		}  
		// 卖1
		lItemStyle = ITEM_TEXT_RIGHT;
		lCellStyle = CELL_BORDER_BOTTOM;
		CreateCell("", TEXT_COLOR_XIANHAND, TEXT_FONT_DYNAMICAL, lItemStyle, lCellStyle);

		lItemStyle = ITEM_TEXT_RIGHT ;
		for (int i = 0; i < 4; i++)
		{   // 买1~买4
			CreateCell("", TEXT_COLOR_XIANHAND, TEXT_FONT_DYNAMICAL, lItemStyle, 0);
		}
		// 买5
		lItemStyle = ITEM_TEXT_RIGHT;
		lCellStyle = CELL_BORDER_BOTTOM;
		CreateCell("", TEXT_COLOR_XIANHAND, TEXT_FONT_DYNAMICAL, lItemStyle, lCellStyle);
	}
	
	lItemStyle = ITEM_TEXT_LEFT;  
	::LoadString(instance, IDS_STRING_JUNJIA, buf, nBufLenth);      // 均价
	strName = buf;
	CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, 0);

	::LoadString(instance, IDS_STRING_KAIPAN, buf, nBufLenth);      // 开盘
	strName = buf;
	CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, 0);

	::LoadString(instance, IDS_STRING_ZUIGAO, buf, nBufLenth);      // 最高
	strName = buf;
	CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, 0);

	::LoadString(instance, IDS_STRING_ZUIDI, buf, nBufLenth);       // 最低
	strName = buf;
	CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, 0);

	::LoadString(instance, IDS_STRING_LIANGBI, buf, nBufLenth);     // 量比
	strName = buf;
	CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, 0);

	lItemStyle = ITEM_TEXT_LEFT;
	lCellStyle = CELL_BORDER_BOTTOM;
	::LoadString(instance, IDS_STRING_HUANSHOU, buf, nBufLenth);    // 换手
	strName = buf;
	CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, lCellStyle);	

	lItemStyle = ITEM_TEXT_LEFT;                                
	::LoadString(instance, IDS_STRING_QUANJIA, buf, nBufLenth);     // 全价
	strName = buf;
	CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, 0);

	if (m_bLevel)
	{// level2 显示每笔
		lItemStyle = ITEM_TEXT_LEFT;
		::LoadString(instance, IDS_STRING_MEIBI, buf, nBufLenth);   // 每笔
		strName = buf;
		CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_DYNAMICAL, lItemStyle, 0);
	}

	lItemStyle = ITEM_TEXT_LEFT;
	lCellStyle = CELL_BORDER_BOTTOM;
	::LoadString(instance, IDS_STRING_DIETING, buf, nBufLenth);     // 跌停
	strName = buf;
	CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, lCellStyle);

	lItemStyle = ITEM_TEXT_LEFT;
	::LoadString(instance, IDS_STRING_SHIJING, buf, nBufLenth);     // 市净
	strName = buf;
	CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, 0);

	::LoadString(instance, IDS_STRING_ZONGGU, buf, nBufLenth);      // 总股
	strName = buf;
	CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, 0);

	::LoadString(instance, IDS_STRING_ZONGZHI, buf, nBufLenth);     // 总值
	strName = buf;
	CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, 0);
	//		TRACE1("第三列 %d \n", m_ayCell.GetSize());
	//////////////////////////////////////// 第四列开始 //////////////////////////////////////////

	lItemStyle = ITEM_TEXT_RIGHT;
	lCellStyle = CELL_BORDER_BOTTOM;
	CreateCell("", 0, TEXT_FONT_DYNAMICAL, lItemStyle, lCellStyle);  // 委差

	if (!m_bLevel)
	{// level1 显示委托量变化
		//委托量变化
		lItemStyle = ITEM_TEXT_RIGHT;  
		for (int i = 0; i < 4; i++)
		{
			// 卖5~卖2
			CreateCell("", 0, TEXT_FONT_DYNAMICAL, lItemStyle, 0); 
		}
		// 卖1
		lItemStyle = ITEM_TEXT_RIGHT;
		lCellStyle = CELL_BORDER_BOTTOM;
		CreateCell("", 0, TEXT_FONT_DYNAMICAL, lItemStyle, lCellStyle);

		lItemStyle = ITEM_TEXT_RIGHT;
		for (int i = 0; i < 4; i++)
		{
			// 买1~买4
			CreateCell("", 0, TEXT_FONT_DYNAMICAL, lItemStyle, 0); 
		}
		// 买5
		lItemStyle = ITEM_TEXT_RIGHT;
		lCellStyle = CELL_BORDER_BOTTOM ;
		CreateCell("", 0, TEXT_FONT_DYNAMICAL, lItemStyle, lCellStyle);
	}
	
	lItemStyle = ITEM_TEXT_RIGHT | ITEM_TEXT_SELECT ;               
	CreateCell("", 0, TEXT_FONT_DYNAMICAL, lItemStyle, 0);                // 均价

	CreateCell("", 0, TEXT_FONT_DYNAMICAL, lItemStyle, 0);                // 开盘

	CreateCell("", 0, TEXT_FONT_DYNAMICAL, lItemStyle, 0);                // 最高

	CreateCell("", 0, TEXT_FONT_DYNAMICAL, lItemStyle, 0);                // 最低

	lItemStyle = ITEM_TEXT_RIGHT;
	CreateCell("", TEXT_COLOR_LIANGBI, TEXT_FONT_DYNAMICAL, lItemStyle, 0); // 量比

	lItemStyle = ITEM_TEXT_RIGHT;
	lCellStyle = CELL_BORDER_BOTTOM;               
	CreateCell("", TEXT_COLOR_HUANSHOU, TEXT_FONT_DYNAMICAL, lItemStyle, lCellStyle); // 换手

	lItemStyle = ITEM_TEXT_RIGHT;               
	CreateCell("", TEXT_COLOR_FLAT, TEXT_FONT_DYNAMICAL, lItemStyle, 0);          // 全价

	if (m_bLevel)
	{
		lItemStyle = ITEM_TEXT_RIGHT;
		CreateCell("", TEXT_COLOR_MEIBI, TEXT_FONT_DYNAMICAL, lItemStyle, 0);     // 每笔 
	}

	lItemStyle = ITEM_TEXT_RIGHT;
	lCellStyle = CELL_BORDER_BOTTOM;               
	CreateCell("", TEXT_COLOR_DOWN, TEXT_FONT_DYNAMICAL, lItemStyle, lCellStyle); // 跌停

	lItemStyle = ITEM_TEXT_RIGHT;               
	CreateCell("", TEXT_COLOR_UP, TEXT_FONT_DYNAMICAL, lItemStyle, 0);            // 市净

	CreateCell("", TEXT_COLOR_UP, TEXT_FONT_DYNAMICAL, lItemStyle, 0);            // 总股

	CreateCell("", TEXT_COLOR_UP, TEXT_FONT_DYNAMICAL, lItemStyle, 0);            // 总值
	//		TRACE1("第四列 %d \n", m_ayCell.GetSize());

	// 初始化基类变量
	SetColCount(4);
	SetRowCount(m_ayCell.GetCount() / 4);
	SetFixColCount(0);
	SetFixRowCount(0);

	delete[] buf;
	return TRUE;
}

BOOL CDrawRStockForm::CreateStock() // 创建股票五档
{
	CString strTitle = "";
	CString strName = "";
	DWORD lItemStyle(0); // 单元小格格式
	DWORD lCellStyle(0); // 单元格格式

	//////////////////////////////////////////////////////////////////////////
	// 读取资源字符串
	CAtlBaseModule _Module;
	//CComModule _Module;
	HINSTANCE instance = _Module.GetResourceInstance();
	char* buf = new char[1000];
	int   nBufLenth = 1000;
	//

	////////////////////////////  标题开始  //////////////////////////////////////
	lItemStyle = ITEM_TEXT_LEFT;
	lCellStyle = CELL_BORDER_BOTTOM;
	::LoadString(instance, IDS_STRING_WEIBI, buf, nBufLenth);    //委比
	strName = buf;
	CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, lCellStyle);

	if (!m_bLevel)
	{// level1 显示五档行情
		lItemStyle = ITEM_TEXT_LEFT;
		::LoadString(instance, IDS_STRING_SELL, buf, nBufLenth);
		strTitle = buf;					
		for (int i = 4; i >= 1; i--)
		{
			// 卖5~卖2
			::LoadString(instance, IDS_STRING_NUM1+i, buf, nBufLenth);
			strName = buf;
			CreateCell(strTitle+strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, 0);
		}
		// 卖1
		lItemStyle = ITEM_TEXT_LEFT;
		lCellStyle = CELL_BORDER_BOTTOM;
		::LoadString(instance, IDS_STRING_NUM1, buf, nBufLenth);
		strName = buf;
		CreateCell(strTitle+strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, lCellStyle);

		lItemStyle = ITEM_TEXT_LEFT;
		::LoadString(instance, IDS_STRING_BUY, buf, nBufLenth);
		strTitle = buf;
		for (int i = 0; i < 4; i++)
		{
			// 买1~买4
			::LoadString(instance, IDS_STRING_NUM1+i, buf, nBufLenth);
			strName = buf;
			CreateCell(strTitle+strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, 0);
		}
		// 买5
		lItemStyle = ITEM_TEXT_LEFT;
		lCellStyle = CELL_BORDER_BOTTOM;
		::LoadString(instance, IDS_STRING_NUM1+4, buf, nBufLenth);
		strName = buf;
		CreateCell(strTitle+strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, lCellStyle);
	}
	
	lItemStyle = ITEM_TEXT_LEFT ;
	::LoadString(instance, IDS_STRING_CHENGJIAO, buf, nBufLenth);  //成交
	strName = buf;  
	CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, 0);

	::LoadString(instance, IDS_STRING_ZHANGDIE, buf, nBufLenth);   //涨跌
	strName = buf;
	CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, 0);

	::LoadString(instance, IDS_STRING_FUDU, buf, nBufLenth);       //幅度
	strName = buf;
	CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, 0);

	::LoadString(instance, IDS_STRING_ZONGSHOU, buf, nBufLenth);   //总手
	strName = buf;
	CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, 0);

	::LoadString(instance, IDS_STRING_XIANSHOU, buf, nBufLenth);   //现手
	strName = buf;
	CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, 0);

	lItemStyle = ITEM_TEXT_LEFT;
	lCellStyle = CELL_BORDER_BOTTOM;
	::LoadString(instance, IDS_STRING_ZONGE, buf, nBufLenth);      //总额
	strName = buf;
	CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, lCellStyle);

	lItemStyle = ITEM_TEXT_LEFT;
	lCellStyle = 0;
	::LoadString(instance, IDS_STRING_WAIPAN, buf, nBufLenth);     //外盘
	strName = buf;                    
	CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, lCellStyle);

	if (m_bLevel)
	{// level2 要显示总笔
		lCellStyle = CELL_BORDER_LEFT;
		::LoadString(instance, IDS_STRING_ZONGBI, buf, nBufLenth); // 总笔
		strName = buf;
		CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, lCellStyle);
	}

	lItemStyle = ITEM_TEXT_LEFT;
	lCellStyle = CELL_BORDER_BOTTOM;
	::LoadString(instance, IDS_STRING_ZHANGTING, buf, nBufLenth);  //涨停
	strName = buf;                            
	CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, lCellStyle);

	lItemStyle = ITEM_TEXT_LEFT;
	::LoadString(instance, IDS_STRING_SHIYING, buf, nBufLenth);    //市盈
	strName = buf;
	CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, 0);

	::LoadString(instance, IDS_STRING_LIUTONG, buf, nBufLenth);    //流通
	strName = buf;
	CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, 0);

	::LoadString(instance, IDS_STRING_SHIZHI, buf, nBufLenth);     //市值
	strName = buf;
	CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, 0);

	//		TRACE1("第一列 %d \n", m_ayCell.GetSize());
	////////////////////////////////////// 第二列  ////////////////////////////////////////
	lItemStyle = ITEM_TEXT_RIGHT;
	lCellStyle = CELL_BORDER_BOTTOM;
	CreateCell("",  0, TEXT_FONT_DYNAMICAL, lItemStyle,lCellStyle);                  //委差

	if (!m_bLevel)
	{// level1 显示五档报价
		lItemStyle = ITEM_TEXT_RIGHT | ITEM_TEXT_SELECT;
		for (int i = 0; i < 4; i++)
		{
			//卖5~卖2				
			CreateCell("", 0, TEXT_FONT_DYNAMICAL, lItemStyle, 0);
		}
		// 卖1
		lItemStyle = ITEM_TEXT_RIGHT | ITEM_TEXT_SELECT;
		lCellStyle = CELL_BORDER_BOTTOM;
		CreateCell("", 0, TEXT_FONT_DYNAMICAL, lItemStyle, lCellStyle);

		lItemStyle = ITEM_TEXT_RIGHT | ITEM_TEXT_SELECT;
		for (int i = 0; i < 4; i++)
		{
			// 买1~买4				
			CreateCell("", 0, TEXT_FONT_DYNAMICAL, lItemStyle, 0);
		}
		// 买5
		lItemStyle = ITEM_TEXT_RIGHT | ITEM_TEXT_SELECT;
		lCellStyle = CELL_BORDER_BOTTOM;
		CreateCell("", 0, TEXT_FONT_DYNAMICAL, lItemStyle, lCellStyle);
	}
	
	lItemStyle = ITEM_TEXT_RIGHT | ITEM_TEXT_SELECT ;                                   
	CreateCell("", 0, TEXT_FONT_DYNAMICAL, lItemStyle, 0);                     // 成交价

	lItemStyle = ITEM_TEXT_RIGHT ;
	CreateCell("", 0, TEXT_FONT_DYNAMICAL, lItemStyle, 0);	                   // 涨跌

	CreateCell("", 0, TEXT_FONT_DYNAMICAL, lItemStyle, 0);	                   // 幅度

	CreateCell("", TEXT_COLOR_TOTALHAND, TEXT_FONT_DYNAMICAL, lItemStyle, 0);  // 总手

	CreateCell("", TEXT_COLOR_XIANHAND, TEXT_FONT_DYNAMICAL, lItemStyle, 0);   // 现手

	lItemStyle = ITEM_TEXT_RIGHT;
	lCellStyle = CELL_BORDER_BOTTOM;
	CreateCell("", TEXT_COLOR_CHENGJIAO, TEXT_FONT_DYNAMICAL, lItemStyle, lCellStyle); // 总额

	lItemStyle = ITEM_TEXT_RIGHT;
	CreateCell("", TEXT_COLOR_UP, TEXT_FONT_DYNAMICAL, lItemStyle, 0);                 // 外盘

	if (m_bLevel)
	{// level2 显示总笔                                           
		lItemStyle = ITEM_TEXT_RIGHT;
		CreateCell("", TEXT_COLOR_ZONGBI, TEXT_FONT_DYNAMICAL, lItemStyle, 0);         // 总笔
	}

	lItemStyle = ITEM_TEXT_RIGHT;
	lCellStyle = CELL_BORDER_BOTTOM;
	CreateCell("", TEXT_COLOR_UP, TEXT_FONT_DYNAMICAL, lItemStyle, lCellStyle);        // 涨停

	lItemStyle = ITEM_TEXT_RIGHT;
	CreateCell("", TEXT_COLOR_UP, TEXT_FONT_DYNAMICAL, lItemStyle, 0);  	           // 市盈

	CreateCell("", TEXT_COLOR_UP, TEXT_FONT_DYNAMICAL, lItemStyle, 0);	               // 流通

	CreateCell("", TEXT_COLOR_UP, TEXT_FONT_DYNAMICAL, lItemStyle, 0);                 // 市值
	//		TRACE1("第二列 %d \n", m_ayCell.GetSize());
	//////////////////////////////////////// 第三列开始 //////////////////////////////////////////
	lItemStyle = ITEM_TEXT_LEFT;
	lCellStyle = CELL_BORDER_BOTTOM;
	::LoadString(instance, IDS_STRING_WEICHA, buf, nBufLenth);       // 委差
	strName = buf;
	CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, lCellStyle);

	if (!m_bLevel)
	{// level1 显示五档行情报价
		lItemStyle = ITEM_TEXT_RIGHT ;   
		for (int i = 0; i < 4; i++)
		{	// 卖5~卖2
			CreateCell("", TEXT_COLOR_XIANHAND, TEXT_FONT_DYNAMICAL, lItemStyle, 0); 
		}  
		// 卖1
		lItemStyle = ITEM_TEXT_RIGHT;
		lCellStyle = CELL_BORDER_BOTTOM;
		CreateCell("", TEXT_COLOR_XIANHAND, TEXT_FONT_DYNAMICAL, lItemStyle, lCellStyle);

		lItemStyle = ITEM_TEXT_RIGHT ;
		for (int i = 0; i < 4; i++)
		{   // 买1~买4
			CreateCell("", TEXT_COLOR_XIANHAND, TEXT_FONT_DYNAMICAL, lItemStyle, 0);
		}
		// 买5
		lItemStyle = ITEM_TEXT_RIGHT;
		lCellStyle = CELL_BORDER_BOTTOM;
		CreateCell("", TEXT_COLOR_XIANHAND, TEXT_FONT_DYNAMICAL, lItemStyle, lCellStyle);
	}
	
	lItemStyle = ITEM_TEXT_LEFT;  
	::LoadString(instance, IDS_STRING_JUNJIA, buf, nBufLenth);      // 均价
	strName = buf;
	CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, 0);

	::LoadString(instance, IDS_STRING_KAIPAN, buf, nBufLenth);      // 开盘
	strName = buf;
	CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, 0);

	::LoadString(instance, IDS_STRING_ZUIGAO, buf, nBufLenth);      // 最高
	strName = buf;
	CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, 0);

	::LoadString(instance, IDS_STRING_ZUIDI, buf, nBufLenth);       // 最低
	strName = buf;
	CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, 0);

	::LoadString(instance, IDS_STRING_LIANGBI, buf, nBufLenth);     // 量比
	strName = buf;
	CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, 0);

	lItemStyle = ITEM_TEXT_LEFT;
	lCellStyle = CELL_BORDER_BOTTOM;
	::LoadString(instance, IDS_STRING_HUANSHOU, buf, nBufLenth);    // 换手
	strName = buf;
	CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, lCellStyle);	

	lItemStyle = ITEM_TEXT_LEFT;                                
	::LoadString(instance, IDS_STRING_NEIPAN, buf, nBufLenth);      // 内盘
	strName = buf;
	CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, 0);

	if (m_bLevel)
	{// level2 显示每笔
		lItemStyle = ITEM_TEXT_LEFT;
		::LoadString(instance, IDS_STRING_MEIBI, buf, nBufLenth);   // 每笔
		strName = buf;
		CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_DYNAMICAL, lItemStyle, 0);
	}

	lItemStyle = ITEM_TEXT_LEFT;
	lCellStyle = CELL_BORDER_BOTTOM;
	::LoadString(instance, IDS_STRING_DIETING, buf, nBufLenth);     // 跌停
	strName = buf;
	CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, lCellStyle);

	lItemStyle = ITEM_TEXT_LEFT;
	::LoadString(instance, IDS_STRING_SHIJING, buf, nBufLenth);     // 市净
	strName = buf;
	CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, 0);

	::LoadString(instance, IDS_STRING_ZONGGU, buf, nBufLenth);      // 总股
	strName = buf;
	CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, 0);

	::LoadString(instance, IDS_STRING_ZONGZHI, buf, nBufLenth);     // 总值
	strName = buf;
	CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, 0);
	//		TRACE1("第三列 %d \n", m_ayCell.GetSize());
	//////////////////////////////////////// 第四列开始 //////////////////////////////////////////

	lItemStyle = ITEM_TEXT_RIGHT;
	lCellStyle = CELL_BORDER_BOTTOM;
	CreateCell("", 0, TEXT_FONT_DYNAMICAL, lItemStyle, lCellStyle);  // 委差

	if (!m_bLevel)
	{// level1 显示五档行情
		//委托量变化
		lItemStyle = ITEM_TEXT_RIGHT;  
		for (int i = 0; i < 4; i++)
		{
			// 卖5~卖2
			CreateCell("", 0, TEXT_FONT_DYNAMICAL, lItemStyle, 0); 
		}
		// 卖1
		lItemStyle = ITEM_TEXT_RIGHT;
		lCellStyle = CELL_BORDER_BOTTOM;
		CreateCell("", 0, TEXT_FONT_DYNAMICAL, lItemStyle, lCellStyle);

		lItemStyle = ITEM_TEXT_RIGHT;
		for (int i = 0; i < 4; i++)
		{
			// 买1~买4
			CreateCell("", 0, TEXT_FONT_DYNAMICAL, lItemStyle, 0); 
		}
		// 买5
		lItemStyle = ITEM_TEXT_RIGHT;
		lCellStyle = CELL_BORDER_BOTTOM ;
		CreateCell("", 0, TEXT_FONT_DYNAMICAL, lItemStyle, lCellStyle);
	}
	
	lItemStyle = ITEM_TEXT_RIGHT | ITEM_TEXT_SELECT ;               
	CreateCell("", 0, TEXT_FONT_DYNAMICAL, lItemStyle, 0);                // 均价

	CreateCell("", 0, TEXT_FONT_DYNAMICAL, lItemStyle, 0);                // 开盘

	CreateCell("", 0, TEXT_FONT_DYNAMICAL, lItemStyle, 0);                // 最高

	CreateCell("", 0, TEXT_FONT_DYNAMICAL, lItemStyle, 0);                // 最低

	lItemStyle = ITEM_TEXT_RIGHT;
	CreateCell("", TEXT_COLOR_LIANGBI, TEXT_FONT_DYNAMICAL, lItemStyle, 0); // 量比

	lItemStyle = ITEM_TEXT_RIGHT;
	lCellStyle = CELL_BORDER_BOTTOM;               
	CreateCell("", TEXT_COLOR_HUANSHOU, TEXT_FONT_DYNAMICAL, lItemStyle, lCellStyle); // 换手

	lItemStyle = ITEM_TEXT_RIGHT;               
	CreateCell("", TEXT_COLOR_DOWN, TEXT_FONT_DYNAMICAL, lItemStyle, 0);          // 内盘

	if (m_bLevel)
	{
		lItemStyle = ITEM_TEXT_RIGHT;
		CreateCell("", TEXT_COLOR_MEIBI, TEXT_FONT_DYNAMICAL, lItemStyle, 0);     // 每笔 
	}

	lItemStyle = ITEM_TEXT_RIGHT;
	lCellStyle = CELL_BORDER_BOTTOM;               
	CreateCell("", TEXT_COLOR_DOWN, TEXT_FONT_DYNAMICAL, lItemStyle, lCellStyle); // 跌停

	lItemStyle = ITEM_TEXT_RIGHT;               
	CreateCell("", TEXT_COLOR_UP, TEXT_FONT_DYNAMICAL, lItemStyle, 0);            // 市净

	CreateCell("", TEXT_COLOR_UP, TEXT_FONT_DYNAMICAL, lItemStyle, 0);            // 总股

	CreateCell("", TEXT_COLOR_UP, TEXT_FONT_DYNAMICAL, lItemStyle, 0);            // 总值
	//		TRACE1("第四列 %d \n", m_ayCell.GetSize());

	// 初始化基类变量
	SetColCount(4);
	SetRowCount(m_ayCell.GetCount() / 4);
	SetFixColCount(0);
	SetFixColCount(0);

	delete[] buf;
	return TRUE;
}


BOOL CDrawRStockForm::CreateFund()  // 创建基金五档
{
	CString strTitle = "";
	CString strName = "";
	DWORD lItemStyle(0); // 单元小格格式
	DWORD lCellStyle(0); // 单元格格式

	//////////////////////////////////////////////////////////////////////////
	// 读取资源字符串
	CAtlBaseModule _Module;
	//CComModule _Module;
	HINSTANCE instance = _Module.GetResourceInstance();
	char* buf = new char[1000];
	int   nBufLenth = 1000;
	//

	////////////////////////////  标题开始  //////////////////////////////////////
	lItemStyle = ITEM_TEXT_LEFT;
	lCellStyle = CELL_BORDER_BOTTOM;
	::LoadString(instance, IDS_STRING_WEIBI, buf, nBufLenth);    //委比
	strName = buf;
	CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, lCellStyle);

	if (!m_bLevel)
	{// level1 显示五档行情
		lItemStyle = ITEM_TEXT_LEFT;
		::LoadString(instance, IDS_STRING_SELL, buf, nBufLenth);
		strTitle = buf;					
		for (int i = 4; i >= 1; i--)
		{
			// 卖5~卖2
			::LoadString(instance, IDS_STRING_NUM1+i, buf, nBufLenth);
			strName = buf;
			CreateCell(strTitle+strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, 0);
		}
		// 卖1
		lItemStyle = ITEM_TEXT_LEFT;
		lCellStyle = CELL_BORDER_BOTTOM;
		::LoadString(instance, IDS_STRING_NUM1, buf, nBufLenth);
		strName = buf;
		CreateCell(strTitle+strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, lCellStyle);

		lItemStyle = ITEM_TEXT_LEFT;
		::LoadString(instance, IDS_STRING_BUY, buf, nBufLenth);
		strTitle = buf;
		for (int i = 0; i < 4; i++)
		{
			// 买1~买4
			::LoadString(instance, IDS_STRING_NUM1+i, buf, nBufLenth);
			strName = buf;
			CreateCell(strTitle+strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, 0);
		}
		// 买5
		lItemStyle = ITEM_TEXT_LEFT;
		lCellStyle = CELL_BORDER_BOTTOM;
		::LoadString(instance, IDS_STRING_NUM1+4, buf, nBufLenth);
		strName = buf;
		CreateCell(strTitle+strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, lCellStyle);
	}

	lItemStyle = ITEM_TEXT_LEFT ;
	::LoadString(instance, IDS_STRING_CHENGJIAO, buf, nBufLenth);  //成交
	strName = buf;  
	CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, 0);

	::LoadString(instance, IDS_STRING_ZHANGDIE, buf, nBufLenth);   //涨跌
	strName = buf;
	CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, 0);

	::LoadString(instance, IDS_STRING_FUDU, buf, nBufLenth);       //幅度
	strName = buf;
	CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, 0);

	::LoadString(instance, IDS_STRING_ZONGSHOU, buf, nBufLenth);   //总手
	strName = buf;
	CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, 0);

	::LoadString(instance, IDS_STRING_XIANSHOU, buf, nBufLenth);   //现手
	strName = buf;
	CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, 0);

	lItemStyle = ITEM_TEXT_LEFT;
	lCellStyle = CELL_BORDER_BOTTOM;
	::LoadString(instance, IDS_STRING_ZONGE, buf, nBufLenth);      //总额
	strName = buf;
	CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, lCellStyle);

	lItemStyle = ITEM_TEXT_LEFT;
	lCellStyle = 0;
	::LoadString(instance, IDS_STRING_WAIPAN, buf, nBufLenth);     //外盘
	strName = buf;                    
	CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, lCellStyle);

	if (m_bLevel)
	{// level2 要显示总笔
		lCellStyle = CELL_BORDER_LEFT;
		::LoadString(instance, IDS_STRING_ZONGBI, buf, nBufLenth); // 总笔
		strName = buf;
		CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, lCellStyle);
	}

	lItemStyle = ITEM_TEXT_LEFT;
	lCellStyle = CELL_BORDER_BOTTOM;
	::LoadString(instance, IDS_STRING_ZHANGTING, buf, nBufLenth);  //涨停
	strName = buf;                            
	CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, lCellStyle);

	lItemStyle = ITEM_TEXT_LEFT;
	::LoadString(instance, IDS_STRING_SHIYING, buf, nBufLenth);    //市盈
	strName = buf;
	CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, 0);

	::LoadString(instance, IDS_STRING_LIUTONG, buf, nBufLenth);    //流通
	strName = buf;
	CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, 0);

	::LoadString(instance, IDS_STRING_SHIZHI, buf, nBufLenth);     //市值
	strName = buf;
	CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, 0);

	//		TRACE1("第一列 %d \n", m_ayCell.GetSize());
	////////////////////////////////////// 第二列  ////////////////////////////////////////
	lItemStyle = ITEM_TEXT_RIGHT;
	lCellStyle = CELL_BORDER_BOTTOM;
	CreateCell("",  0, TEXT_FONT_DYNAMICAL, lItemStyle,lCellStyle);                  //委差

	if (!m_bLevel)
	{// level1 显示五档行情
		lItemStyle = ITEM_TEXT_RIGHT | ITEM_TEXT_SELECT;
		for (int i = 0; i < 4; i++)
		{
			//卖5~卖2				
			CreateCell("", 0, TEXT_FONT_DYNAMICAL, lItemStyle, 0);
		}
		// 卖1
		lItemStyle = ITEM_TEXT_RIGHT | ITEM_TEXT_SELECT;
		lCellStyle = CELL_BORDER_BOTTOM;
		CreateCell("", 0, TEXT_FONT_DYNAMICAL, lItemStyle, lCellStyle);

		lItemStyle = ITEM_TEXT_RIGHT | ITEM_TEXT_SELECT;
		for (int i = 0; i < 4; i++)
		{
			// 买1~买4				
			CreateCell("", 0, TEXT_FONT_DYNAMICAL, lItemStyle, 0);
		}
		// 买5
		lItemStyle = ITEM_TEXT_RIGHT | ITEM_TEXT_SELECT;
		lCellStyle = CELL_BORDER_BOTTOM;
		CreateCell("", 0, TEXT_FONT_DYNAMICAL, lItemStyle, lCellStyle);
	}

	lItemStyle = ITEM_TEXT_RIGHT | ITEM_TEXT_SELECT ;                                   
	CreateCell("", 0, TEXT_FONT_DYNAMICAL, lItemStyle, 0);                     // 成交价

	lItemStyle = ITEM_TEXT_RIGHT ;
	CreateCell("", 0, TEXT_FONT_DYNAMICAL, lItemStyle, 0);	                   // 涨跌

	CreateCell("", 0, TEXT_FONT_DYNAMICAL, lItemStyle, 0);	                   // 幅度

	CreateCell("", TEXT_COLOR_TOTALHAND, TEXT_FONT_DYNAMICAL, lItemStyle, 0);  // 总手

	CreateCell("", TEXT_COLOR_XIANHAND, TEXT_FONT_DYNAMICAL, lItemStyle, 0);   // 现手

	lItemStyle = ITEM_TEXT_RIGHT;
	lCellStyle = CELL_BORDER_BOTTOM;
	CreateCell("", TEXT_COLOR_CHENGJIAO, TEXT_FONT_DYNAMICAL, lItemStyle, lCellStyle); // 总额

	lItemStyle = ITEM_TEXT_RIGHT;
	CreateCell("", TEXT_COLOR_UP, TEXT_FONT_DYNAMICAL, lItemStyle, 0);                 // 外盘

	if (m_bLevel)
	{// level2 显示总笔                                           
		lItemStyle = ITEM_TEXT_RIGHT;
		CreateCell("", TEXT_COLOR_ZONGBI, TEXT_FONT_DYNAMICAL, lItemStyle, 0);         // 总笔
	}

	lItemStyle = ITEM_TEXT_RIGHT;
	lCellStyle = CELL_BORDER_BOTTOM;
	CreateCell("", TEXT_COLOR_UP, TEXT_FONT_DYNAMICAL, lItemStyle, lCellStyle);        // 涨停

	lItemStyle = ITEM_TEXT_RIGHT;
	CreateCell("", TEXT_COLOR_UP, TEXT_FONT_DYNAMICAL, lItemStyle, 0);  	           // 市盈

	CreateCell("", TEXT_COLOR_UP, TEXT_FONT_DYNAMICAL, lItemStyle, 0);	               // 流通

	CreateCell("", TEXT_COLOR_UP, TEXT_FONT_DYNAMICAL, lItemStyle, 0);                 // 市值
	//		TRACE1("第二列 %d \n", m_ayCell.GetSize());
	//////////////////////////////////////// 第三列开始 //////////////////////////////////////////
	lItemStyle = ITEM_TEXT_LEFT;
	lCellStyle = CELL_BORDER_BOTTOM;
	::LoadString(instance, IDS_STRING_WEICHA, buf, nBufLenth);       // 委差
	strName = buf;
	CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, lCellStyle);

	if (!m_bLevel)
	{// level1 显示五档行情
		lItemStyle = ITEM_TEXT_RIGHT ;   
		for (int i = 0; i < 4; i++)
		{	// 卖5~卖2
			CreateCell("", TEXT_COLOR_XIANHAND, TEXT_FONT_DYNAMICAL, lItemStyle, 0); 
		}  
		// 卖1
		lItemStyle = ITEM_TEXT_RIGHT;
		lCellStyle = CELL_BORDER_BOTTOM;
		CreateCell("", TEXT_COLOR_XIANHAND, TEXT_FONT_DYNAMICAL, lItemStyle, lCellStyle);

		lItemStyle = ITEM_TEXT_RIGHT ;
		for (int i = 0; i < 4; i++)
		{   // 买1~买4
			CreateCell("", TEXT_COLOR_XIANHAND, TEXT_FONT_DYNAMICAL, lItemStyle, 0);
		}
		// 买5
		lItemStyle = ITEM_TEXT_RIGHT;
		lCellStyle = CELL_BORDER_BOTTOM;
		CreateCell("", TEXT_COLOR_XIANHAND, TEXT_FONT_DYNAMICAL, lItemStyle, lCellStyle);
	}
	
	lItemStyle = ITEM_TEXT_LEFT;  
	::LoadString(instance, IDS_STRING_JUNJIA, buf, nBufLenth);      // 均价
	strName = buf;
	CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, 0);

	::LoadString(instance, IDS_STRING_KAIPAN, buf, nBufLenth);      // 开盘
	strName = buf;
	CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, 0);

	::LoadString(instance, IDS_STRING_ZUIGAO, buf, nBufLenth);      // 最高
	strName = buf;
	CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, 0);

	::LoadString(instance, IDS_STRING_ZUIDI, buf, nBufLenth);       // 最低
	strName = buf;
	CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, 0);

	::LoadString(instance, IDS_STRING_LIANGBI, buf, nBufLenth);     // 量比
	strName = buf;
	CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, 0);

	lItemStyle = ITEM_TEXT_LEFT;
	lCellStyle = CELL_BORDER_BOTTOM;
	::LoadString(instance, IDS_STRING_JINGZHI, buf, nBufLenth);    // 净值
	strName = buf;
	CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, lCellStyle);	

	lItemStyle = ITEM_TEXT_LEFT;                                
	::LoadString(instance, IDS_STRING_NEIPAN, buf, nBufLenth);      // 内盘
	strName = buf;
	CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, 0);

	if (m_bLevel)
	{// level2 显示每笔
		lItemStyle = ITEM_TEXT_LEFT;
		::LoadString(instance, IDS_STRING_MEIBI, buf, nBufLenth);   // 每笔
		strName = buf;
		CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_DYNAMICAL, lItemStyle, 0);
	}

	lItemStyle = ITEM_TEXT_LEFT;
	lCellStyle = CELL_BORDER_BOTTOM;
	::LoadString(instance, IDS_STRING_DIETING, buf, nBufLenth);     // 跌停
	strName = buf;
	CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, lCellStyle);

	lItemStyle = ITEM_TEXT_LEFT;
	::LoadString(instance, IDS_STRING_SHIJING, buf, nBufLenth);     // 市净
	strName = buf;
	CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, 0);

	::LoadString(instance, IDS_STRING_ZONGGU, buf, nBufLenth);      // 总股
	strName = buf;
	CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, 0);

	::LoadString(instance, IDS_STRING_ZONGZHI, buf, nBufLenth);     // 总值
	strName = buf;
	CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, 0);
	//		TRACE1("第三列 %d \n", m_ayCell.GetSize());
	//////////////////////////////////////// 第四列开始 //////////////////////////////////////////

	lItemStyle = ITEM_TEXT_RIGHT;
	lCellStyle = CELL_BORDER_BOTTOM;
	CreateCell("", 0, TEXT_FONT_DYNAMICAL, lItemStyle, lCellStyle);  // 委差

	if (!m_bLevel)
	{// level1 显示五档行情
		//委托量变化
		lItemStyle = ITEM_TEXT_RIGHT;  
		for (int i = 0; i < 4; i++)
		{
			// 卖5~卖2
			CreateCell("", 0, TEXT_FONT_DYNAMICAL, lItemStyle, 0); 
		}
		// 卖1
		lItemStyle = ITEM_TEXT_RIGHT;
		lCellStyle = CELL_BORDER_BOTTOM;
		CreateCell("", 0, TEXT_FONT_DYNAMICAL, lItemStyle, lCellStyle);

		lItemStyle = ITEM_TEXT_RIGHT;
		for (int i = 0; i < 4; i++)
		{
			// 买1~买4
			CreateCell("", 0, TEXT_FONT_DYNAMICAL, lItemStyle, 0); 
		}
		// 买5
		lItemStyle = ITEM_TEXT_RIGHT;
		lCellStyle = CELL_BORDER_BOTTOM ;
		CreateCell("", 0, TEXT_FONT_DYNAMICAL, lItemStyle, lCellStyle);
	}

	lItemStyle = ITEM_TEXT_RIGHT | ITEM_TEXT_SELECT ;               
	CreateCell("", 0, TEXT_FONT_DYNAMICAL, lItemStyle, 0);                // 均价

	CreateCell("", 0, TEXT_FONT_DYNAMICAL, lItemStyle, 0);                // 开盘

	CreateCell("", 0, TEXT_FONT_DYNAMICAL, lItemStyle, 0);                // 最高

	CreateCell("", 0, TEXT_FONT_DYNAMICAL, lItemStyle, 0);                // 最低

	lItemStyle = ITEM_TEXT_RIGHT;
	CreateCell("", TEXT_COLOR_LIANGBI, TEXT_FONT_DYNAMICAL, lItemStyle, 0); // 量比

	lItemStyle = ITEM_TEXT_RIGHT;
	lCellStyle = CELL_BORDER_BOTTOM;               
	CreateCell("", TEXT_COLOR_XIANHAND, TEXT_FONT_DYNAMICAL, lItemStyle, lCellStyle); // 净值

	lItemStyle = ITEM_TEXT_RIGHT;               
	CreateCell("", TEXT_COLOR_DOWN, TEXT_FONT_DYNAMICAL, lItemStyle, 0);          // 内盘

	if (m_bLevel)
	{
		lItemStyle = ITEM_TEXT_RIGHT;
		CreateCell("", TEXT_COLOR_MEIBI, TEXT_FONT_DYNAMICAL, lItemStyle, 0);     // 每笔 
	}

	lItemStyle = ITEM_TEXT_RIGHT;
	lCellStyle = CELL_BORDER_BOTTOM;               
	CreateCell("", TEXT_COLOR_DOWN, TEXT_FONT_DYNAMICAL, lItemStyle, lCellStyle); // 跌停

	lItemStyle = ITEM_TEXT_RIGHT;               
	CreateCell("", TEXT_COLOR_UP, TEXT_FONT_DYNAMICAL, lItemStyle, 0);            // 市净

	CreateCell("", TEXT_COLOR_UP, TEXT_FONT_DYNAMICAL, lItemStyle, 0);            // 总股

	CreateCell("", TEXT_COLOR_UP, TEXT_FONT_DYNAMICAL, lItemStyle, 0);            // 总值
	//		TRACE1("第四列 %d \n", m_ayCell.GetSize());

	// 初始化基类变量
	SetColCount(4);
	SetRowCount(m_ayCell.GetCount() / 4);
	SetFixColCount(0);
	SetFixColCount(0);

	delete[] buf;
	return TRUE;
}

BOOL CDrawRStockForm::CreateEFT() // 创建ETF五档
{
	CString strTitle = "";
	CString strName = "";
	DWORD lItemStyle(0); // 单元小格格式
	DWORD lCellStyle(0); // 单元格格式

	//////////////////////////////////////////////////////////////////////////
	// 读取资源字符串
	CAtlBaseModule _Module;
	//CComModule _Module;
	HINSTANCE instance = _Module.GetResourceInstance();
	char* buf = new char[1000];
	int   nBufLenth = 1000;
	//

	////////////////////////////  标题开始  //////////////////////////////////////
	lItemStyle = ITEM_TEXT_LEFT;
	lCellStyle = CELL_BORDER_BOTTOM;
	::LoadString(instance, IDS_STRING_WEIBI, buf, nBufLenth);    //委比
	strName = buf;
	CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, lCellStyle);

	if (!m_bLevel)
	{// level1 显示五档行情
		lItemStyle = ITEM_TEXT_LEFT;
		::LoadString(instance, IDS_STRING_SELL, buf, nBufLenth);
		strTitle = buf;					
		for (int i = 4; i >= 1; i--)
		{
			// 卖5~卖2
			::LoadString(instance, IDS_STRING_NUM1+i, buf, nBufLenth);
			strName = buf;
			CreateCell(strTitle+strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, 0);
		}
		// 卖1
		lItemStyle = ITEM_TEXT_LEFT;
		lCellStyle = CELL_BORDER_BOTTOM;
		::LoadString(instance, IDS_STRING_NUM1, buf, nBufLenth);
		strName = buf;
		CreateCell(strTitle+strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, lCellStyle);

		lItemStyle = ITEM_TEXT_LEFT;
		::LoadString(instance, IDS_STRING_BUY, buf, nBufLenth);
		strTitle = buf;
		for (int i = 0; i < 4; i++)
		{
			// 买1~买4
			::LoadString(instance, IDS_STRING_NUM1+i, buf, nBufLenth);
			strName = buf;
			CreateCell(strTitle+strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, 0);
		}
		// 买5
		lItemStyle = ITEM_TEXT_LEFT;
		lCellStyle = CELL_BORDER_BOTTOM;
		::LoadString(instance, IDS_STRING_NUM1+4, buf, nBufLenth);
		strName = buf;
		CreateCell(strTitle+strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, lCellStyle);
	}

	lItemStyle = ITEM_TEXT_LEFT ;
	::LoadString(instance, IDS_STRING_CHENGJIAO, buf, nBufLenth);  //成交
	strName = buf;  
	CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, 0);

	::LoadString(instance, IDS_STRING_ZHANGDIE, buf, nBufLenth);   //涨跌
	strName = buf;
	CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, 0);

	::LoadString(instance, IDS_STRING_FUDU, buf, nBufLenth);       //幅度
	strName = buf;
	CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, 0);

	::LoadString(instance, IDS_STRING_ZONGSHOU, buf, nBufLenth);   //总手
	strName = buf;
	CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, 0);

	::LoadString(instance, IDS_STRING_XIANSHOU, buf, nBufLenth);   //现手
	strName = buf;
	CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, 0);

	lItemStyle = ITEM_TEXT_LEFT;
	lCellStyle = CELL_BORDER_BOTTOM;
	::LoadString(instance, IDS_STRING_ZONGE, buf, nBufLenth);      //总额
	strName = buf;
	CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, lCellStyle);

	lItemStyle = ITEM_TEXT_LEFT;
	lCellStyle = 0;
	::LoadString(instance, IDS_STRING_WAIPAN, buf, nBufLenth);     //外盘
	strName = buf;                    
	CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, lCellStyle);

	if (m_bLevel)
	{// level2 要显示总笔
		lCellStyle = CELL_BORDER_LEFT;
		::LoadString(instance, IDS_STRING_ZONGBI, buf, nBufLenth); // 总笔
		strName = buf;
		CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, lCellStyle);
	}

	lItemStyle = ITEM_TEXT_LEFT;
	lCellStyle = CELL_BORDER_BOTTOM;
	::LoadString(instance, IDS_STRING_ZHANGTING, buf, nBufLenth);  //涨停
	strName = buf;                            
	CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, lCellStyle);

	lItemStyle = ITEM_TEXT_LEFT;
	::LoadString(instance, IDS_STRING_SHIYING, buf, nBufLenth);    //市盈
	strName = buf;
	CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, 0);

	::LoadString(instance, IDS_STRING_LIUTONG, buf, nBufLenth);    //流通
	strName = buf;
	CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, 0);

	::LoadString(instance, IDS_STRING_SHIZHI, buf, nBufLenth);     //市值
	strName = buf;
	CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, 0);

	//		TRACE1("第一列 %d \n", m_ayCell.GetSize());
	////////////////////////////////////// 第二列  ////////////////////////////////////////
	lItemStyle = ITEM_TEXT_RIGHT;
	lCellStyle = CELL_BORDER_BOTTOM;
	CreateCell("",  0, TEXT_FONT_DYNAMICAL, lItemStyle,lCellStyle);                  //委差

	if (!m_bLevel)
	{// level1 显示五档行情
		lItemStyle = ITEM_TEXT_RIGHT | ITEM_TEXT_SELECT;
		for (int i = 0; i < 4; i++)
		{
			//卖5~卖2				
			CreateCell("", 0, TEXT_FONT_DYNAMICAL, lItemStyle, 0);
		}
		// 卖1
		lItemStyle = ITEM_TEXT_RIGHT | ITEM_TEXT_SELECT;
		lCellStyle = CELL_BORDER_BOTTOM;
		CreateCell("", 0, TEXT_FONT_DYNAMICAL, lItemStyle, lCellStyle);

		lItemStyle = ITEM_TEXT_RIGHT | ITEM_TEXT_SELECT;
		for (int i = 0; i < 4; i++)
		{
			// 买1~买4				
			CreateCell("", 0, TEXT_FONT_DYNAMICAL, lItemStyle, 0);
		}
		// 买5
		lItemStyle = ITEM_TEXT_RIGHT | ITEM_TEXT_SELECT;
		lCellStyle = CELL_BORDER_BOTTOM;
		CreateCell("", 0, TEXT_FONT_DYNAMICAL, lItemStyle, lCellStyle);
	}

	lItemStyle = ITEM_TEXT_RIGHT | ITEM_TEXT_SELECT ;                                   
	CreateCell("", 0, TEXT_FONT_DYNAMICAL, lItemStyle, 0);                     // 成交价

	lItemStyle = ITEM_TEXT_RIGHT ;
	CreateCell("", 0, TEXT_FONT_DYNAMICAL, lItemStyle, 0);	                   // 涨跌

	CreateCell("", 0, TEXT_FONT_DYNAMICAL, lItemStyle, 0);	                   // 幅度

	CreateCell("", TEXT_COLOR_TOTALHAND, TEXT_FONT_DYNAMICAL, lItemStyle, 0);  // 总手

	CreateCell("", TEXT_COLOR_XIANHAND, TEXT_FONT_DYNAMICAL, lItemStyle, 0);   // 现手

	lItemStyle = ITEM_TEXT_RIGHT;
	lCellStyle = CELL_BORDER_BOTTOM;
	CreateCell("", TEXT_COLOR_CHENGJIAO, TEXT_FONT_DYNAMICAL, lItemStyle, lCellStyle); // 总额

	lItemStyle = ITEM_TEXT_RIGHT;
	CreateCell("", TEXT_COLOR_UP, TEXT_FONT_DYNAMICAL, lItemStyle, 0);                 // 外盘

	if (m_bLevel)
	{// level2 显示总笔                                           
		lItemStyle = ITEM_TEXT_RIGHT;
		CreateCell("", TEXT_COLOR_ZONGBI, TEXT_FONT_DYNAMICAL, lItemStyle, 0);         // 总笔
	}

	lItemStyle = ITEM_TEXT_RIGHT;
	lCellStyle = CELL_BORDER_BOTTOM;
	CreateCell("", TEXT_COLOR_UP, TEXT_FONT_DYNAMICAL, lItemStyle, lCellStyle);        // 涨停

	lItemStyle = ITEM_TEXT_RIGHT;
	CreateCell("", TEXT_COLOR_UP, TEXT_FONT_DYNAMICAL, lItemStyle, 0);  	           // 市盈

	CreateCell("", TEXT_COLOR_UP, TEXT_FONT_DYNAMICAL, lItemStyle, 0);	               // 流通

	CreateCell("", TEXT_COLOR_UP, TEXT_FONT_DYNAMICAL, lItemStyle, 0);                 // 市值
	//		TRACE1("第二列 %d \n", m_ayCell.GetSize());
	//////////////////////////////////////// 第三列开始 //////////////////////////////////////////
	lItemStyle = ITEM_TEXT_LEFT;
	lCellStyle = CELL_BORDER_BOTTOM;
	::LoadString(instance, IDS_STRING_WEICHA, buf, nBufLenth);       // 委差
	strName = buf;
	CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, lCellStyle);

	if (!m_bLevel)
	{// level1 显示五档行情
		lItemStyle = ITEM_TEXT_RIGHT ;   
		for (int i = 0; i < 4; i++)
		{	// 卖5~卖2
			CreateCell("", TEXT_COLOR_XIANHAND, TEXT_FONT_DYNAMICAL, lItemStyle, 0); 
		}  
		// 卖1
		lItemStyle = ITEM_TEXT_RIGHT;
		lCellStyle = CELL_BORDER_BOTTOM;
		CreateCell("", TEXT_COLOR_XIANHAND, TEXT_FONT_DYNAMICAL, lItemStyle, lCellStyle);

		lItemStyle = ITEM_TEXT_RIGHT ;
		for (int i = 0; i < 4; i++)
		{   // 买1~买4
			CreateCell("", TEXT_COLOR_XIANHAND, TEXT_FONT_DYNAMICAL, lItemStyle, 0);
		}
		// 买5
		lItemStyle = ITEM_TEXT_RIGHT;
		lCellStyle = CELL_BORDER_BOTTOM;
		CreateCell("", TEXT_COLOR_XIANHAND, TEXT_FONT_DYNAMICAL, lItemStyle, lCellStyle);
	}

	lItemStyle = ITEM_TEXT_LEFT;  
	::LoadString(instance, IDS_STRING_JUNJIA, buf, nBufLenth);      // 均价
	strName = buf;
	CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, 0);

	::LoadString(instance, IDS_STRING_KAIPAN, buf, nBufLenth);      // 开盘
	strName = buf;
	CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, 0);

	::LoadString(instance, IDS_STRING_ZUIGAO, buf, nBufLenth);      // 最高
	strName = buf;
	CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, 0);

	::LoadString(instance, IDS_STRING_ZUIDI, buf, nBufLenth);       // 最低
	strName = buf;
	CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, 0);

	::LoadString(instance, IDS_STRING_LIANGBI, buf, nBufLenth);     // 量比
	strName = buf;
	CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, 0);

	lItemStyle = ITEM_TEXT_LEFT;
	lCellStyle = CELL_BORDER_BOTTOM;
	::LoadString(instance, IDS_STRING_IOPV, buf, nBufLenth);        // LOPV
	strName = buf;
	CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, lCellStyle);	

	lItemStyle = ITEM_TEXT_LEFT;                                
	::LoadString(instance, IDS_STRING_NEIPAN, buf, nBufLenth);      // 内盘
	strName = buf;
	CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, 0);

	if (m_bLevel)
	{// level2 显示每笔
		lItemStyle = ITEM_TEXT_LEFT;
		::LoadString(instance, IDS_STRING_MEIBI, buf, nBufLenth);   // 每笔
		strName = buf;
		CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_DYNAMICAL, lItemStyle, 0);
	}

	lItemStyle = ITEM_TEXT_LEFT;
	lCellStyle = CELL_BORDER_BOTTOM;
	::LoadString(instance, IDS_STRING_DIETING, buf, nBufLenth);     // 跌停
	strName = buf;
	CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, lCellStyle);

	lItemStyle = ITEM_TEXT_LEFT;
	::LoadString(instance, IDS_STRING_SHIJING, buf, nBufLenth);     // 市净
	strName = buf;
	CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, 0);

	::LoadString(instance, IDS_STRING_ZONGGU, buf, nBufLenth);      // 总股
	strName = buf;
	CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, 0);

	::LoadString(instance, IDS_STRING_ZONGZHI, buf, nBufLenth);     // 总值
	strName = buf;
	CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, 0);
	//		TRACE1("第三列 %d \n", m_ayCell.GetSize());
	//////////////////////////////////////// 第四列开始 //////////////////////////////////////////

	lItemStyle = ITEM_TEXT_RIGHT;
	lCellStyle = CELL_BORDER_BOTTOM;
	CreateCell("", 0, TEXT_FONT_DYNAMICAL, lItemStyle, lCellStyle);  // 委差

	if (!m_bLevel)
	{// level1 显示五档行情
		//委托量变化
		lItemStyle = ITEM_TEXT_RIGHT;  
		for (int i = 0; i < 4; i++)
		{
			// 卖5~卖2
			CreateCell("", 0, TEXT_FONT_DYNAMICAL, lItemStyle, 0); 
		}
		// 卖1
		lItemStyle = ITEM_TEXT_RIGHT;
		lCellStyle = CELL_BORDER_BOTTOM;
		CreateCell("", 0, TEXT_FONT_DYNAMICAL, lItemStyle, lCellStyle);

		lItemStyle = ITEM_TEXT_RIGHT;
		for (int i = 0; i < 4; i++)
		{
			// 买1~买4
			CreateCell("", 0, TEXT_FONT_DYNAMICAL, lItemStyle, 0); 
		}
		// 买5
		lItemStyle = ITEM_TEXT_RIGHT;
		lCellStyle = CELL_BORDER_BOTTOM ;
		CreateCell("", 0, TEXT_FONT_DYNAMICAL, lItemStyle, lCellStyle);
	}

	lItemStyle = ITEM_TEXT_RIGHT | ITEM_TEXT_SELECT ;               
	CreateCell("", 0, TEXT_FONT_DYNAMICAL, lItemStyle, 0);                // 均价

	CreateCell("", 0, TEXT_FONT_DYNAMICAL, lItemStyle, 0);                // 开盘

	CreateCell("", 0, TEXT_FONT_DYNAMICAL, lItemStyle, 0);                // 最高

	CreateCell("", 0, TEXT_FONT_DYNAMICAL, lItemStyle, 0);                // 最低

	lItemStyle = ITEM_TEXT_RIGHT;
	CreateCell("", TEXT_COLOR_LIANGBI, TEXT_FONT_DYNAMICAL, lItemStyle, 0); // 量比

	lItemStyle = ITEM_TEXT_RIGHT;
	lCellStyle = CELL_BORDER_BOTTOM;               
	CreateCell("", TEXT_COLOR_XIANHAND, TEXT_FONT_DYNAMICAL, lItemStyle, lCellStyle); // 净值

	lItemStyle = ITEM_TEXT_RIGHT;               
	CreateCell("", TEXT_COLOR_DOWN, TEXT_FONT_DYNAMICAL, lItemStyle, 0);          // 内盘

	if (m_bLevel)
	{
		lItemStyle = ITEM_TEXT_RIGHT;
		CreateCell("", TEXT_COLOR_MEIBI, TEXT_FONT_DYNAMICAL, lItemStyle, 0);     // 每笔 
	}

	lItemStyle = ITEM_TEXT_RIGHT;
	lCellStyle = CELL_BORDER_BOTTOM;               
	CreateCell("", TEXT_COLOR_DOWN, TEXT_FONT_DYNAMICAL, lItemStyle, lCellStyle); // 跌停

	lItemStyle = ITEM_TEXT_RIGHT;               
	CreateCell("", TEXT_COLOR_UP, TEXT_FONT_DYNAMICAL, lItemStyle, 0);            // 市净

	CreateCell("", TEXT_COLOR_UP, TEXT_FONT_DYNAMICAL, lItemStyle, 0);            // 总股

	CreateCell("", TEXT_COLOR_UP, TEXT_FONT_DYNAMICAL, lItemStyle, 0);            // 总值
	//		TRACE1("第四列 %d \n", m_ayCell.GetSize());

	// 初始化基类变量
	SetColCount(4);
	SetRowCount(m_ayCell.GetCount() / 4);
	SetFixColCount(0);
	SetFixColCount(0);

	delete[] buf;
	return TRUE;
}

BOOL CDrawRStockForm::CreateIndex()   // 创建指数
{
	CString strTitle = "";
	CString strName = "";
	DWORD lItemStyle(0); // 单元小格格式
	DWORD lCellStyle(0); // 单元格格式

	//////////////////////////////////////////////////////////////////////////
	// 读取资源字符串
	CAtlBaseModule _Module;
	//CComModule _Module;
	HINSTANCE instance = _Module.GetResourceInstance();
	char* buf = new char[1000];
	int   nBufLenth = 1000;
	//

	//////////////////////////////////////// 第一列开始 //////////////////////////////////////////
	lItemStyle = ITEM_TEXT_LEFT;
	::LoadString(instance, IDS_STRING_CHENGJIAO, buf, nBufLenth);        // 成交
	strName = buf;
	CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, 0);

	lItemStyle = ITEM_TEXT_LEFT;
	::LoadString(instance, IDS_STRING_ZHANGDIE, buf, nBufLenth);         // 涨跌
	strName = buf;
	CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, 0);

	lItemStyle = ITEM_TEXT_LEFT;
	::LoadString(instance, IDS_STRING_ZHANGFU, buf, nBufLenth);          // 涨幅
	strName = buf;
	CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, 0);

	lItemStyle = ITEM_TEXT_LEFT;
	::LoadString(instance, IDS_STRING_ZONGSHOU, buf, nBufLenth);         // 总手
	strName = buf;
	CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, 0);

	lItemStyle = ITEM_TEXT_LEFT;
	lCellStyle = CELL_BORDER_BOTTOM;
	::LoadString(instance, IDS_STRING_JINE, buf, nBufLenth);             // 金额
	strName = buf;
	CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, lCellStyle);

	lItemStyle = ITEM_TEXT_LEFT;
	lCellStyle = CELL_BORDER_BOTTOM;
	::LoadString(instance, IDS_STRING_WEIBI, buf, nBufLenth);            // 委比
	strName = buf;
	CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, lCellStyle);

	lItemStyle = ITEM_TEXT_LEFT;
	::LoadString(instance, IDS_STRING_BUYSHOUSHU, buf, nBufLenth);       // 委买手数
	strName = buf;
	CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, 0);

	lItemStyle = ITEM_TEXT_LEFT;
	lCellStyle = CELL_BORDER_BOTTOM;
	::LoadString(instance, IDS_STRING_SELLSHSOUSHU, buf, nBufLenth);     // 委卖手数
	strName = buf;
	CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, lCellStyle);

	lItemStyle = ITEM_TEXT_LEFT;
	::LoadString(instance, IDS_STRING_SHANGZHANGJIASHU, buf, nBufLenth); // 上涨家数
	strName = buf;
	CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, 0);

	lItemStyle = ITEM_TEXT_LEFT;
	::LoadString(instance, IDS_STRING_PINGPANJIASHU, buf, nBufLenth);    // 平盘家数
	strName = buf;
	CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, 0);

	lItemStyle = ITEM_TEXT_LEFT;
	::LoadString(instance, IDS_STRING_XIADIEJIASHU, buf, nBufLenth);     // 下跌家数
	strName = buf;
	CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, 0);

	//////////////////////////////////////// 第二列开始 //////////////////////////////////////////
	lItemStyle = ITEM_TEXT_RIGHT;
	CreateCell("", 0, TEXT_FONT_DYNAMICAL, lItemStyle, 0);               // 成交

	lItemStyle = ITEM_TEXT_RIGHT;
	CreateCell("", 0, TEXT_FONT_DYNAMICAL, lItemStyle, 0);               // 涨跌

	lItemStyle = ITEM_TEXT_RIGHT;
	CreateCell("", 0, TEXT_FONT_DYNAMICAL, lItemStyle, 0);               // 涨幅

	lItemStyle = ITEM_TEXT_RIGHT;
	CreateCell("", TEXT_COLOR_UP, TEXT_FONT_DYNAMICAL, lItemStyle, 0);   // 总手

	lItemStyle = ITEM_TEXT_RIGHT;
	lCellStyle = CELL_BORDER_BOTTOM;
	CreateCell("", TEXT_COLOR_UP, TEXT_FONT_DYNAMICAL, lItemStyle, lCellStyle);   // 金额

	lItemStyle = ITEM_TEXT_RIGHT;
	lCellStyle = CELL_BORDER_BOTTOM;
	CreateCell("", 0, TEXT_FONT_DYNAMICAL, lItemStyle, lCellStyle);      // 委比

	CreateCell("");                                                      // 委买手数

	CreateCell("", 0, 0, 0, CELL_BORDER_BOTTOM);                         // 委卖手数

	CreateCell("");                                                      // 上涨家数

	CreateCell("");                                                      // 平盘家数

	CreateCell("");                                                      // 下跌家数

	//////////////////////////////////////// 第三列开始 //////////////////////////////////////////	
	lItemStyle = ITEM_TEXT_LEFT;
	::LoadString(instance, IDS_STRING_ZUOSHOU, buf, nBufLenth);          // 昨收
	strName = buf;
	CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, 0);

	lItemStyle = ITEM_TEXT_LEFT;
	::LoadString(instance, IDS_STRING_KAIPAN, buf, nBufLenth);           // 开盘
	strName = buf;
	CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, 0);

	lItemStyle = ITEM_TEXT_LEFT;
	::LoadString(instance, IDS_STRING_ZUIGAO, buf, nBufLenth);          // 最高
	strName = buf;
	CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, 0);

	lItemStyle = ITEM_TEXT_LEFT;
	::LoadString(instance, IDS_STRING_ZUIDI, buf, nBufLenth);           // 最低
	strName = buf;
	CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, 0);

	lItemStyle = ITEM_TEXT_LEFT;
	lCellStyle = CELL_BORDER_BOTTOM;
	::LoadString(instance, IDS_STRING_ZHENFU, buf, nBufLenth);          // 振幅
	strName = buf;
	CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, lCellStyle);

	lItemStyle = ITEM_TEXT_LEFT;
	lCellStyle = CELL_BORDER_BOTTOM;
	::LoadString(instance, IDS_STRING_WEICHA, buf, nBufLenth);          // 委差
	strName = buf;
	CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, lCellStyle);
	
	CreateCell("");                                                      // 委买手数

	CreateCell("", 0, 0, 0, CELL_BORDER_BOTTOM);                         // 委卖手数

	CreateCell("");                                                      // 上涨家数

	CreateCell("");                                                      // 平盘家数

	CreateCell("");                                                      // 下跌家数
	
	//////////////////////////////////////// 第四列开始 //////////////////////////////////////////	
	lItemStyle = ITEM_TEXT_RIGHT;
	CreateCell("", 0, TEXT_FONT_DYNAMICAL, lItemStyle, 0);               // 昨收

	lItemStyle = ITEM_TEXT_RIGHT;
	CreateCell("", 0, TEXT_FONT_DYNAMICAL, lItemStyle, 0);               // 开盘

	lItemStyle = ITEM_TEXT_RIGHT;
	CreateCell("", 0, TEXT_FONT_DYNAMICAL, lItemStyle, 0);               // 最高

	lItemStyle = ITEM_TEXT_RIGHT;
	CreateCell("", 0, TEXT_FONT_DYNAMICAL, lItemStyle, 0);               // 最低

	lItemStyle = ITEM_TEXT_RIGHT;
	lCellStyle = CELL_BORDER_BOTTOM;
	CreateCell("", TEXT_COLOR_UP, TEXT_FONT_DYNAMICAL, lItemStyle, lCellStyle);  // 振幅

	lItemStyle = ITEM_TEXT_RIGHT;
	lCellStyle = CELL_BORDER_BOTTOM;
	CreateCell("", 0, TEXT_FONT_DYNAMICAL, lItemStyle, lCellStyle);      // 委差

	lItemStyle = ITEM_TEXT_RIGHT;
	CreateCell("", TEXT_COLOR_UP, TEXT_FONT_DYNAMICAL, lItemStyle, 0);   // 委买手数

	lItemStyle = ITEM_TEXT_RIGHT;
	lCellStyle = CELL_BORDER_BOTTOM;
	CreateCell("", TEXT_COLOR_DOWN, TEXT_FONT_DYNAMICAL, lItemStyle, lCellStyle);  // 委卖手数

	lItemStyle = ITEM_TEXT_RIGHT;
	CreateCell("", TEXT_COLOR_UP, TEXT_FONT_DYNAMICAL, lItemStyle, 0);  // 上涨家数

	lItemStyle = ITEM_TEXT_RIGHT;
	CreateCell("", TEXT_COLOR_FLAT, TEXT_FONT_DYNAMICAL, lItemStyle, 0);// 平盘家数

	lItemStyle = ITEM_TEXT_RIGHT;
	CreateCell("", TEXT_COLOR_DOWN, TEXT_FONT_DYNAMICAL, lItemStyle, 0);// 下跌家数

	// 初始化基类变量
	SetColCount(4);
	SetRowCount(m_ayCell.GetCount() / 4);
	SetFixColCount(0);
	SetFixColCount(0);

	delete[] buf;
	return TRUE;
}

BOOL CDrawRStockForm::CreateFuture() // 创建期货
{
	CString strTitle = "";
	CString strName = "";
	DWORD lItemStyle(0); // 单元小格格式
	DWORD lCellStyle(0); // 单元格格式

	//////////////////////////////////////////////////////////////////////////
	// 读取资源字符串
	CAtlBaseModule _Module;
	//CComModule _Module;
	HINSTANCE instance = _Module.GetResourceInstance();
	char* buf = new char[1000];
	int   nBufLenth = 1000;
	//

	//////////////////////////////////////////////////////////////////////////
	// 第一列
	lItemStyle = ITEM_TEXT_LEFT;
	lCellStyle = 0;
	::LoadString(instance, IDS_STRING_SELLPRICE, buf, 1000);
	strName = buf;
	CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, lCellStyle);      // 卖价

	lItemStyle = ITEM_TEXT_LEFT;
	lCellStyle = CELL_BORDER_BOTTOM;
	::LoadString(instance, IDS_STRING_BUYPRICE, buf, 1000);
	strName = buf;
	CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, lCellStyle);      // 买价

	lItemStyle = ITEM_TEXT_LEFT;
	lCellStyle = 0;
	::LoadString(instance, IDS_STRING_NEW, buf, 1000);                    
	strName = buf;
	CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, lCellStyle);      // 最新

	lItemStyle = ITEM_TEXT_LEFT;
	lCellStyle = 0;
	::LoadString(instance, IDS_STRING_ZHANGDIE, buf, 1000);
	strName = buf;
	CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, lCellStyle);      // 涨跌

	lItemStyle = ITEM_TEXT_LEFT;
	lCellStyle = 0;
	::LoadString(instance, IDS_STRING_ZHANGFU, buf, 1000);
	strName = buf;
	CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, lCellStyle);      // 涨幅

	lItemStyle = ITEM_TEXT_LEFT;
	lCellStyle = 0;
	::LoadString(instance, IDS_STRING_ZONGSHOU, buf, 1000);
	strName = buf;
	CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, lCellStyle);      // 总手

	lItemStyle = ITEM_TEXT_LEFT;
	lCellStyle = 0;
	::LoadString(instance, IDS_STRING_XIANSHOU, buf, 1000);
	strName = buf;
	CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, lCellStyle);      // 现手

	lItemStyle = ITEM_TEXT_LEFT;
	lCellStyle = 0;
	::LoadString(instance, IDS_STRING_WAIPAN, buf, 1000);
	strName = buf;
	CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, lCellStyle);      // 外盘

	lItemStyle = ITEM_TEXT_LEFT;
	lCellStyle = 0;
	::LoadString(instance, IDS_STRING_ZHANGTING, buf, 1000);
	strName = buf;
	CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, lCellStyle);      // 涨停

	lItemStyle = ITEM_TEXT_LEFT;
	lCellStyle = 0;
	::LoadString(instance, IDS_STRING_JIESUAN, buf, 1000);
	strName = buf;
	CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, lCellStyle);      // 结算

	lItemStyle = ITEM_TEXT_LEFT;
	lCellStyle = 0;
	::LoadString(instance, IDS_STRING_ZONGCHI, buf, 1000);
	strName = buf;
	CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, lCellStyle);      // 总持

	//////////////////////////////////////////////////////////////////////////
	// 第二列
	lItemStyle = ITEM_TEXT_RIGHT | ITEM_TEXT_SELECT;
	lCellStyle = 0;
	CreateCell("", 0, TEXT_FONT_DYNAMICAL, lItemStyle, lCellStyle);                          // 卖价

	lItemStyle = ITEM_TEXT_RIGHT | ITEM_TEXT_SELECT;
	lCellStyle = CELL_BORDER_BOTTOM;
	CreateCell("", 0, TEXT_FONT_DYNAMICAL, lItemStyle, lCellStyle);                          // 买价

	lItemStyle = ITEM_TEXT_RIGHT | ITEM_TEXT_SELECT;
	lCellStyle = 0;
	CreateCell("", 0, TEXT_FONT_DYNAMICAL, lItemStyle, lCellStyle);                          // 最新

	lItemStyle = ITEM_TEXT_RIGHT;
	lCellStyle = 0;
	CreateCell("", 0, TEXT_FONT_DYNAMICAL, lItemStyle, lCellStyle);                          // 涨跌

	lItemStyle = ITEM_TEXT_RIGHT;
	lCellStyle = 0;
	CreateCell("", 0, TEXT_FONT_DYNAMICAL, lItemStyle, lCellStyle);                          // 涨幅

	lItemStyle = ITEM_TEXT_RIGHT;
	lCellStyle = 0;
	CreateCell("", TEXT_COLOR_TOTALHAND, TEXT_FONT_DYNAMICAL, lItemStyle, lCellStyle);       // 总手

	lItemStyle = ITEM_TEXT_RIGHT;
	lCellStyle = 0;
	CreateCell("", TEXT_COLOR_XIANHAND, TEXT_FONT_DYNAMICAL, lItemStyle, lCellStyle);        // 现手

	lItemStyle = ITEM_TEXT_RIGHT;
	lCellStyle = 0;
	CreateCell("", TEXT_COLOR_UP, TEXT_FONT_DYNAMICAL, lItemStyle, lCellStyle);              // 外盘

	lItemStyle = ITEM_TEXT_RIGHT;
	lCellStyle = 0;
	CreateCell("", TEXT_COLOR_UP, TEXT_FONT_DYNAMICAL, lItemStyle, lCellStyle);              // 涨停

	lItemStyle = ITEM_TEXT_RIGHT;
	lCellStyle = 0;
	CreateCell("", 0, TEXT_FONT_DYNAMICAL, lItemStyle, lCellStyle);                          // 结算

	lItemStyle = ITEM_TEXT_RIGHT;
	lCellStyle = 0;
	CreateCell("", 0, TEXT_FONT_DYNAMICAL, lItemStyle, lCellStyle);                          // 总持

	//////////////////////////////////////////////////////////////////////////
	// 第三列
	lItemStyle = ITEM_TEXT_LEFT;
	lCellStyle = 0;
	::LoadString(instance, IDS_STRING_SELLCOUNT, buf, 1000);
	strName = buf;
	CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, lCellStyle);      // 卖量

	lItemStyle = ITEM_TEXT_LEFT;
	lCellStyle = CELL_BORDER_BOTTOM;
	::LoadString(instance, IDS_STRING_BUYCOUNT, buf, 1000);
	strName = buf;
	CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, lCellStyle);      // 买量

	lItemStyle = ITEM_TEXT_LEFT;
	lCellStyle = 0;
	::LoadString(instance, IDS_STRING_JUNJIA, buf, 1000);
	strName = buf;
	CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, lCellStyle);      // 均价

	lItemStyle = ITEM_TEXT_LEFT;
	lCellStyle = 0;
	::LoadString(instance, IDS_STRING_KAIPAN, buf, 1000);
	strName = buf;
	CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, lCellStyle);      // 开盘

	lItemStyle = ITEM_TEXT_LEFT;
	lCellStyle = 0;
	::LoadString(instance, IDS_STRING_ZUIGAO, buf, 1000);
	strName = buf;
	CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, lCellStyle);      // 最高

	lItemStyle = ITEM_TEXT_LEFT;
	lCellStyle = 0;
	::LoadString(instance, IDS_STRING_ZUIDI, buf, 1000);
	strName = buf;
	CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, lCellStyle);      // 最低

	lItemStyle = ITEM_TEXT_LEFT;
	lCellStyle = 0;
	::LoadString(instance, IDS_STRING_WEIBI, buf, 1000);
	strName = buf;
	CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, lCellStyle);      // 委比

	lItemStyle = ITEM_TEXT_LEFT;
	lCellStyle = 0;
	::LoadString(instance, IDS_STRING_NEIPAN, buf, 1000);
	strName = buf;
	CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, lCellStyle);      // 内盘

	lItemStyle = ITEM_TEXT_LEFT;
	lCellStyle = 0;
	::LoadString(instance, IDS_STRING_DIETING, buf, 1000);
	strName = buf;
	CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, lCellStyle);      // 跌停

	lItemStyle = ITEM_TEXT_LEFT;
	lCellStyle = 0;
	::LoadString(instance, IDS_STRING_QIANJIE, buf, 1000);
	strName = buf;
	CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, lCellStyle);      // 前结

	lItemStyle = ITEM_TEXT_LEFT;
	lCellStyle = 0;
	::LoadString(instance, IDS_STRING_RIZENG, buf, 1000);
	strName = buf;
	CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, lCellStyle);      // 日增

	//////////////////////////////////////////////////////////////////////////
	// 第四列
	lItemStyle = ITEM_TEXT_RIGHT;
	lCellStyle = 0;
	CreateCell("", TEXT_COLOR_SELLCOUNT, TEXT_FONT_DYNAMICAL, lItemStyle, lCellStyle);        // 卖量

	lItemStyle = ITEM_TEXT_RIGHT;
	lCellStyle = CELL_BORDER_BOTTOM;
	CreateCell("", TEXT_COLOR_BUYCOUNT, TEXT_FONT_DYNAMICAL, lItemStyle, lCellStyle);         // 买量

	lItemStyle = ITEM_TEXT_RIGHT;
	lCellStyle = 0;
	CreateCell("", 0, TEXT_FONT_DYNAMICAL, lItemStyle, lCellStyle);                           // 均价

	lItemStyle = ITEM_TEXT_RIGHT;
	lCellStyle = 0;
	CreateCell("", 0, TEXT_FONT_DYNAMICAL, lItemStyle, lCellStyle);                           // 开盘

	lItemStyle = ITEM_TEXT_RIGHT;
	lCellStyle = 0;
	CreateCell("", 0, TEXT_FONT_DYNAMICAL, lItemStyle, lCellStyle);                           // 最高

	lItemStyle = ITEM_TEXT_RIGHT;
	lCellStyle = 0;
	CreateCell("", 0, TEXT_FONT_DYNAMICAL, lItemStyle, lCellStyle);                           // 最低

	lItemStyle = ITEM_TEXT_RIGHT;
	lCellStyle = 0;
	CreateCell("", 0, TEXT_FONT_DYNAMICAL, lItemStyle, lCellStyle);                           // 委比

	lItemStyle = ITEM_TEXT_RIGHT;
	lCellStyle = 0;
	CreateCell("", TEXT_COLOR_DOWN, TEXT_FONT_DYNAMICAL, lItemStyle, lCellStyle);             // 内盘

	lItemStyle = ITEM_TEXT_RIGHT;
	lCellStyle = 0;
	CreateCell("", TEXT_COLOR_DOWN, TEXT_FONT_DYNAMICAL, lItemStyle, lCellStyle);             // 跌停

	lItemStyle = ITEM_TEXT_RIGHT;
	lCellStyle = 0;
	CreateCell("", 0, TEXT_FONT_DYNAMICAL, lItemStyle, lCellStyle);                           // 前结

	lItemStyle = ITEM_TEXT_RIGHT;
	lCellStyle = 0;
	CreateCell("", 0, TEXT_FONT_DYNAMICAL, lItemStyle, lCellStyle);                           // 日增

	// 初始化基类变量
	SetColCount(4);
	SetRowCount(m_ayCell.GetCount() / 4);
	SetFixColCount(0);
	SetFixColCount(0);

	delete[] buf;
	return TRUE;
}

BOOL CDrawRStockForm::GetRequestData(IRequestCollector* pReqCollector,StockUserInfo* pStockInfo, BOOL bForce)
{
	if (pStockInfo == NULL)
	{
		return FALSE;
	}
	if (pStockInfo->m_ciStockCode.m_cCodeType != m_preCodeInfo.m_cCodeType)
	{
		// 不同市场需要重新生成单元格
		RemoveAllCell();
		CreateSelf();
		CalCulateSize();
	}
	m_preCodeInfo = pStockInfo->m_ciStockCode;

	//////////////////////////////////////////////////////////////////////////
	// 将一些变量恢复初始值
	m_sellMap.RemoveAll();
	m_buyMap.RemoveAll();
	m_nNewPrice = 0;
	memset(&m_sRealTime, 0, sizeof(HSStockRealTime));
	//////////////////////////////////////////////////////////////////////////
	WORD wMask = m_bLevel?RT_LEVEL_REALTIME:RT_REALTIME_EXT;
	
	int lAskLen = sizeof(AskData);
	char* pData = new char[lAskLen]; 
	AskData*  pAskData = (AskData*)pData;
	memset(pAskData, 0, lAskLen);
	pAskData->m_nType  = wMask;
	if (MakeSubMarket(m_stkInfo.m_ciStockCode.m_cCodeType) == KIND_INDEX)
	{// 如果是指数 请求领先指数
		pAskData->m_nType = RT_LEAD;
	}
	pAskData->m_nSize  = 1;
	memcpy(pAskData->m_pCode, &pStockInfo->m_ciStockCode,sizeof(CodeInfo));
	pReqCollector->AddRequest((char*)pAskData,lAskLen);
	
	wMask = m_bLevel?RT_LEVEL_AUTOPUSH:RT_AUTOPUSH_EXT;
	pAskData->m_nType  = wMask;
	if (MakeSubMarket(m_stkInfo.m_ciStockCode.m_cCodeType) == KIND_INDEX)
	{// 如果是指数 请求领先指数
		pAskData->m_nType = RT_AUTOPUSH;
	}
	pReqCollector->AddRequest((char*)pAskData,lAskLen);
	delete[] pData;

	return TRUE;
}
int	CDrawRStockForm::OnUpdateData(DataHead* pData)
{
	
	//////////////////////////////////////////////////////////////////////////
	// 指数
	if (pData->m_nType == RT_LEAD)
	{
		AnsLeadData* pRealData = (AnsLeadData*)pData;
		UpdateIndexData(&pRealData->m_indData);
		InValidate(NULL, FALSE);
		return 1;
	}
	else if (pData->m_nType == RT_AUTOPUSH)
	{
		AnsHSAutoPushData* pEveryData = (AnsHSAutoPushData*)pData;
		if (pEveryData->m_nSize <= 0)
		{
			return -1;
		}
		else
		{
			CommRealTimeData* pRealData = (CommRealTimeData*)pEveryData->m_pstData;
			if (pRealData->m_ciStockCode.CompareCode(&m_stkInfo.m_ciStockCode))
			{
				UpdateIndexData(&pRealData->GetShareData()->m_indData);
			}
			return 1;
		}
	}

	//////////////////////////////////////////////////////////////////////////
	// Level1
	if (!m_bLevel)
	{
		if( pData->m_nType == RT_REALTIME_EXT)
		{// level1扩展行情
			AnsRealTime*  pNowData  = (AnsRealTime*)pData;
			if(pNowData->m_nSize <= 0 )
			{
				return -1;
			}
			else
			{
				CommRealTimeData_Ext* pRealData = (CommRealTimeData_Ext *)pNowData->m_pnowData;
				HandleNowData( (DataHead*)pData, pRealData, pNowData->m_nSize);
				return 1;
			}
		}
		else if (pData->m_nType == RT_AUTOPUSH_EXT)
		{// level1主推行情
			AnsHSAutoPushData* pEveryData = (AnsHSAutoPushData*)pData;
			if (pEveryData->m_nSize <= 0)
			{
				return -1;
			}
			else
			{
				CommRealTimeData_Ext* pRealData = (CommRealTimeData_Ext *)pEveryData->m_pstData;
				HandleNowData( (DataHead*)pData, pRealData, pEveryData->m_nSize);
				return 1;
			}
		}
	}
	//////////////////////////////////////////////////////////////////////////
	// Level2
	else
	{
		if (pData->m_nType == RT_LEVEL_AUTOPUSH || pData->m_nType == RT_LEVEL_REALTIME)
		{
			// level2扩展行情 和 主推包
			AnsHSAutoPushLevel* pLevelAuto = (AnsHSAutoPushLevel*)pData;
			if (pLevelAuto->m_nSize <= 0)
			{
				return -1;
			}
			else
			{
				if (memcmp(&m_sLevelData, pLevelAuto->m_pstData, sizeof(RealTimeDataLevel)) == 0)
					return 0;
				memcpy(&m_sLevelData, pLevelAuto->m_pstData, sizeof(RealTimeDataLevel));
				
				CodeInfo* pCodeInfo = &(pLevelAuto->m_pstData->m_ciStockCode);
				if (pCodeInfo->CompareCode(&m_preCodeInfo))
				{
					LevelRealTime* pLevelReal = &pLevelAuto->m_pstData->m_sLevelRealTime;
					LevelStockOtherData* pOtherData = &pLevelAuto->m_pstData->m_othData;

					WORD nMarket = MakeMarket(m_stkInfo.m_ciStockCode.m_cCodeType);
					if (nMarket == STOCK_MARKET)
					{// 股票市场
						WORD nSubMarket = MakeSubMarket(m_stkInfo.m_ciStockCode.m_cCodeType);
						if (nSubMarket == KIND_BOND)
						{// 债券
							UpdateBondsData(pLevelReal, pOtherData);
						}
						else if (nSubMarket == KIND_FUND || nSubMarket == KIND_LOF)
						{// 基金 LOF
							UpdateFundData(pLevelReal, pOtherData);
						}
						else if (nSubMarket == KIND_ETF)
						{// ETF
							UpdateETFData(pLevelReal, pOtherData);
						}
						else if (MakeIndexMarket(m_stkInfo.m_ciStockCode.m_cCodeType))
						{// 指数		

						}
						else
						{// 股票
							UpdateStockData(pLevelReal, pOtherData);
						}
					}
					else if (nMarket == FUTURES_MARKET)
					{// 期货市场


					}
					InValidate(NULL, FALSE);
					return 1;
				}
			}
		}
	}
	return 0;
}

void CDrawRStockForm::HandleNowData(DataHead* pHead, CommRealTimeData_Ext* pnowData, int nSize)
{
	if( pnowData == NULL || nSize <= 0 )
		return;

	CommRealTimeData_Ext* pData = pnowData;
	CommRealTimeData_Ext* pNextData = NULL;

	int nNowSize = 0;
	for(int i = 0; i < nSize; i++)
	{
		pNextData = pData->GetNext(&pData->m_ciStockCode,nNowSize);
		if( pNextData == pData )
		{
			pNextData = pData->GetNext(&pHead->m_nPrivateKey.m_pCode,nNowSize);
		}
		
		if (pData->m_ciStockCode.CompareCode(&m_stkInfo.m_ciStockCode))
		{
			WORD nMarket = MakeMarket(m_stkInfo.m_ciStockCode.m_cCodeType);
			if (nMarket == STOCK_MARKET)
			{// 股票市场
				WORD nSubMarket = MakeSubMarket(m_stkInfo.m_ciStockCode.m_cCodeType);
				if (nSubMarket == KIND_BOND)
				{// 债券
					if(UpdateBondsData(pData->GetShareData(), &pData->m_othData) == FALSE)
					{
						pData = pNextData;
						continue;
					}
					else
						break;
				}
				else if (nSubMarket == KIND_FUND || nSubMarket == KIND_LOF)
				{// 基金 LOF
					if(UpdateFundData(pData->GetShareData(), &pData->m_othData) == FALSE)
					{
						pData = pNextData;
						continue;
					}
					else
						break;
				}
				else if (nSubMarket == KIND_ETF)
				{// ETF
					if(UpdateETFData(pData->GetShareData(), &pData->m_othData) == FALSE)
					{
						pData = pNextData;
						continue;
					}
					else
						break;
				}
				else if (MakeIndexMarket(m_stkInfo.m_ciStockCode.m_cCodeType))
				{// 指数		
				}
				else
				{// 股票
					if(UpdateStockData(pData->GetShareData(), &pData->m_othData) == FALSE)
					{
						pData = pNextData;
						continue;
					}
					else
						break;
				}
			}
		}
		pData = pNextData;		
	}
	InValidate(NULL, FALSE);
}

BOOL  CDrawRStockForm::UpdateBondsData(ShareRealTimeData_Ext* pStock, StockOtherData* pOther)
{
	if ( pStock == NULL || ::IsBadReadPtr(pStock, 1) || pOther == NULL || ::IsBadReadPtr(pOther, 1))
	{
		return FALSE;
	}

	HSStockRealTime_Ext* pRealTimeExt = &pStock->m_nowDataExt;
	if (pRealTimeExt == NULL || ::IsBadReadPtr(pRealTimeExt,1))
	{
		return FALSE;
	}

	HSStockRealTime* pRealTime = &pRealTimeExt->m_stockRealTime;
	if (pRealTime == NULL || ::IsBadReadPtr(pRealTime,1))
	{
		return FALSE;
	}

	if ( memcmp(pRealTime, &m_sRealTime, sizeof(HSStockRealTime)) == 0)
	{// 过滤相同的主推包
		return FALSE;
	}
	memcpy(&m_sRealTime, pRealTime, sizeof(HSStockRealTime));

	int nHand = m_stkInfo.m_nHandNum; 
	//////////////////////////////////////////////////////////////////////////
	// 得到财务数据
	CArray<CodeInfo> ayCode;
	CArray<CurrentFinanceData> ayFinanceData;
	ayCode.Add(m_stkInfo.m_ciStockCode);
	m_pDataSource->HSDataSourceEx_GetStocksFinanceData(ayCode, ayFinanceData);
	CHSFinanceData* pFinanceData = NULL;
	if (ayFinanceData.GetCount() == 1)
		pFinanceData = new CHSFinanceData(&ayFinanceData.GetAt(0));
	//

	BOOL bIsStop = TRUE;//是否停牌
	if (pRealTimeExt->m_stockOther.m_lStopFlag == 0)
	{
		bIsStop = FALSE;
	}

	CString str;
	char    strText[64];
	CPoint  pos(0,0);
	WORD    lClr;

	long    nClose   = m_stkInfo.m_lPrevClose;
	int     nUnit    = m_stkInfo.m_nPriceUnit;
	int     nDecimal = m_stkInfo.m_nDecimal;

	long BuyCount[] = {pRealTime->m_lBuyCount1, pRealTime->m_lBuyCount2, pRealTime->m_lBuyCount3,
		pRealTime->m_lBuyCount4, pRealTime->m_lBuyCount5};
	long SellCount[] = {pRealTime->m_lSellCount1, pRealTime->m_lSellCount2, pRealTime->m_lSellCount3,
		pRealTime->m_lSellCount4, pRealTime->m_lSellCount5};
	long BuyPrice[] = {pRealTime->m_lBuyPrice1, pRealTime->m_lBuyPrice2, pRealTime->m_lBuyPrice3,
		pRealTime->m_lBuyPrice4, pRealTime->m_lBuyPrice5};
	long SellPrice[] = {pRealTime->m_lSellPrice1, pRealTime->m_lSellPrice2, pRealTime->m_lSellPrice3,
		pRealTime->m_lSellPrice4, pRealTime->m_lSellPrice5};
	// 初始化价格map
	InitPriceMap(m_buyMap,  BuyPrice);
	InitPriceMap(m_sellMap, SellPrice);

	double dValue = 0.0;
	
	//从第2栏开始
	// 委比
	pos.x = 1;
	pos.y = 0; 
	dValue = CHSCalculate::GetWeiBi(pRealTime, strText);
	lClr = GetTextColor(dValue, 0);
	SetCellText(pos, 0, strText, lClr);

	if (!m_bLevel)
	{
		//卖5~卖1 价
		for (int i = 4; i >= 0; i--)
		{
			dValue = SellPrice[i];
			CGeneralHelper::LongToString(dValue, 0, nUnit, nDecimal, strText, LTOA_MASK_HORLINE_IFZERO);
			pos.y++;
			lClr = GetTextColor(dValue, nClose);
			SetCellText(pos, 0, strText, lClr);
		}

		//买1~买5 价
		for (int i = 0; i < 5; i++)
		{
			dValue = BuyPrice[i];
			CGeneralHelper::LongToString(dValue, 0, nUnit, nDecimal, strText, LTOA_MASK_HORLINE_IFZERO);
			pos.y++;
			lClr = GetTextColor(dValue, nClose);
			SetCellText(pos, 0, strText, lClr);
		}
	}

	//成交
	dValue = pRealTime->m_lNewPrice;
	m_nNewPrice = pRealTime->m_lNewPrice;
	CGeneralHelper::LongToString(dValue, nClose, nUnit, nDecimal, strText, LTOA_MASK_HORLINE_IFZERO);
	pos.y++;
	lClr = GetTextColor(dValue, nClose);
	SetCellText(pos, 0, strText, lClr);

	//涨跌
	dValue = CHSCalculate::GetZhangdie(bIsStop, &m_stkInfo, pRealTime, strText);
	pos.y++;
	lClr = GetTextColor(dValue, 0);
	SetCellText(pos, 0, strText, lClr);

	//幅度(涨幅=(现价-昨收)/昨收)
	dValue = CHSCalculate::GetFudu(bIsStop, &m_stkInfo,pRealTime, strText);
	pos.y++;
	lClr = GetTextColor(dValue, 0);
	SetCellText(pos, 0, strText, lClr);

	//总手
	dValue = CHSCalculate::GetZongshou(&m_stkInfo, pRealTime, strText);
	pos.y++;
	SetCellText(pos, 0, strText);

	//现手
	dValue = CHSCalculate::GetXianshou(&m_stkInfo, pOther, strText);
	pos.y++;
	SetCellText(pos, 0, strText);

	//总额
	dValue = pRealTime->m_fAvgPrice;
	CGeneralHelper::LongToString(dValue, 0, 10000, 2, strText, LTOA_MASK_HORLINE_IFZERO);
	pos.y++;
	SetCellText(pos, 0, strText);

	//利息
	dValue = pRealTime->m_lNationalDebtRatio;
	CGeneralHelper::LongToString(dValue, 0, 1000, nDecimal, strText, LTOA_MASK_HORLINE_IFZERO);
	pos.y++;
	SetCellText(pos, 0, strText);

	//涨停
	dValue = m_stkInfo.m_fUpPrice < 0 ? 0:m_stkInfo.m_fUpPrice;
	CGeneralHelper::LongToString(dValue, 0, nUnit, nDecimal, strText, LTOA_MASK_HORLINE_IFZERO);
	pos.y++;
	SetCellText(pos, 0, strText);

	//市盈 
	dValue = CHSCalculate::GetShiying(&m_stkInfo, pRealTime, pFinanceData, strText);
	pos.y++;
	SetCellText(pos, 0, strText);

	//流通
	dValue = CHSCalculate::Getliutong(pFinanceData, strText);
	pos.y++;
	SetCellText(pos, 0, strText);

	//市值
	dValue = CHSCalculate::GetShizhi(pRealTime, pFinanceData, strText);
	pos.y++;
	SetCellText(pos, 0, strText);

	////////////////////////// 第三列 ////////////////////////////////////////
	pos.x = 2;
	pos.y = 0;
	//卖5 ~ 卖1量
	for (int i = 4; i >= 0; i--)
	{
		pos.y++;
		dValue = CHSCalculate::GetBuySellCount(&m_stkInfo, SellCount[i], strText);		
		SetCellText(pos, 0, strText);

		// 同时计算该档的成交量
		lClr = GetSellChange(SellPrice[i], (long)dValue, strText);
		SetCellText(CPoint(pos.x+1, pos.y), 0, strText, lClr);
	}

	// 买1 ~ 买5量
	for (int i = 0; i < 5; i++)
	{
		pos.y++;
		dValue = CHSCalculate::GetBuySellCount(&m_stkInfo, BuyCount[i], strText);
		SetCellText(pos, 0, strText);

		// 同时计算该档的成交量
		lClr = GetBuyChange(BuyPrice[i], (long)dValue, strText);
		SetCellText(CPoint(pos.x+1, pos.y), 0, strText, lClr);
	}

	/////////////////////////////////  第四列 /////////////////////////////////////
	//委差=(委买手数－委卖手数)
	dValue = CHSCalculate::GetWeicha(pRealTime, nHand, strText);
	pos.x = 3;
	pos.y = 0;
	lClr = GetTextColor(dValue, 0);
	SetCellText(pos, 0, strText, lClr);

	//卖5 ~ 卖1 量变化
	for (int i = 4; i >= 0; i--)
		pos.y++;

	//买1 ~ 买5 量变化
	for (int i = 0; i < 5; i++)
		pos.y++;

	//均价 ＝（E分时成交的量×成交价）/总成交股数
	pos.y++;
	dValue = CHSCalculate::GetJunjia(&m_stkInfo, pRealTime, strText);
	lClr = GetTextColor(dValue, nClose);
	SetCellText(pos, 0, strText, lClr);

	//开盘
	dValue = pRealTime->m_lOpen;
	CGeneralHelper::LongToString(dValue, nClose, nUnit, nDecimal, strText, LTOA_MASK_HORLINE_IFZERO);
	pos.y++;
	lClr = GetTextColor(dValue, nClose);
	SetCellText(pos, 0, strText, lClr);

	//最高
	dValue = pRealTime->m_lMaxPrice;
	CGeneralHelper::LongToString(dValue, nClose, nUnit, nDecimal, strText, LTOA_MASK_HORLINE_IFZERO);
	pos.y++;
	lClr = GetTextColor(dValue, nClose);
	SetCellText(pos, 0, strText, lClr);

	//最低
	dValue = pRealTime->m_lMinPrice;
	CGeneralHelper::LongToString(dValue, nClose, nUnit, nDecimal, strText, LTOA_MASK_HORLINE_IFZERO);
	pos.y++;
	lClr = GetTextColor(dValue, nClose);
	SetCellText(pos, 0, strText, lClr);

	//量比
	pos.y++;
	dValue = CHSCalculate::GetLiangbi(&m_stkInfo, pRealTime, pOther, m_pDataSource, strText);
	SetCellText(pos, 0, strText);

	//换手
	pos.y++;
	dValue = CHSCalculate::GetHuanshou(&m_stkInfo, pRealTime, pFinanceData, strText);
	SetCellText(pos, 0, strText);

	//全价
	pos.y++;
	dValue = CHSCalculate::GetQuanjia(&m_stkInfo, pRealTime, strText);
	CGeneralHelper::LongToString(dValue, 0, nUnit, 2, strText, LTOA_MASK_HORLINE_IFZERO);
	SetCellText(pos, 0, strText);

	//跌停
	dValue = m_stkInfo.m_fDownPrice < 0 ? 0:m_stkInfo.m_fDownPrice;
	CGeneralHelper::LongToString(dValue, 0, nUnit, nDecimal, strText, LTOA_MASK_HORLINE_IFZERO);
	pos.y++;
	SetCellText(pos, 0, strText);

	//市净
	pos.y++;
	dValue = CHSCalculate::GetShijing(pRealTime, pFinanceData, strText);
	SetCellText(pos, 0, strText);

	//总股
	pos.y++;
	dValue = CHSCalculate::GetZonggu(pFinanceData, strText);
	SetCellText(pos, 0, strText);

	//总值
	pos.y++;
	dValue = CHSCalculate::GetZongzhi(pRealTime, pFinanceData, strText);
	SetCellText(pos, 0, strText);

	if (pFinanceData)
	{
		delete pFinanceData;
		pFinanceData = NULL;
	}

	return TRUE;
}

BOOL CDrawRStockForm::UpdateBondsData(LevelRealTime* pLevelData, LevelStockOtherData* pOtherData)  // 更新债券数据
{
	if (pLevelData == NULL || pOtherData == NULL)
		return FALSE;

	int nHand = m_stkInfo.m_nHandNum;
	//////////////////////////////////////////////////////////////////////////
	// 得到财务数据
	CArray<CodeInfo> ayCode;
	CArray<CurrentFinanceData> ayFinanceData;
	ayCode.Add(m_stkInfo.m_ciStockCode);
	m_pDataSource->HSDataSourceEx_GetStocksFinanceData(ayCode, ayFinanceData);
	CHSFinanceData* pFinanceData = NULL;
	if (ayFinanceData.GetCount() == 1)
		pFinanceData = new CHSFinanceData(&ayFinanceData.GetAt(0));
	//

	BOOL bIsStop = TRUE;//是否停牌
	if (pLevelData->m_lOpen > 0)
		bIsStop = FALSE;

	CString str;
	char    strText[64];
	CPoint  pos(0,0);
	WORD    lClr;

	long    nClose   = m_stkInfo.m_lPrevClose;
	int     nUnit    = m_stkInfo.m_nPriceUnit;
	int     nDecimal = m_stkInfo.m_nDecimal;

	double dValue = 0.0;
	//从第2栏开始
	//委比＝(委买手数－委卖手数)/(委买手数＋委卖手数)×100％
	pos.x = 1;
	pos.y = 0; 
	dValue = CHSCalculate::GetWeiBi(pLevelData, strText);
	lClr = GetTextColor(dValue, 0);
	SetCellText(pos, 0, strText, lClr);

	//成交
	dValue = pLevelData->m_lNewPrice;
	m_nNewPrice = pLevelData->m_lNewPrice;
	CGeneralHelper::LongToString(dValue, nClose, nUnit, nDecimal, strText, LTOA_MASK_HORLINE_IFZERO);
	pos.y++;
	lClr = GetTextColor(dValue, nClose);
	SetCellText(pos, 0, strText, lClr);

	//涨跌
	dValue = CHSCalculate::GetZhangdie(bIsStop, &m_stkInfo, pLevelData, strText);
	pos.y++;
	lClr = GetTextColor(dValue, 0);
	SetCellText(pos, 0, strText, lClr);

	//幅度(涨幅=(现价-昨收)/昨收)
	dValue = CHSCalculate::GetFudu(bIsStop, &m_stkInfo, pLevelData, strText);
	pos.y++;
	lClr = GetTextColor(dValue, 0);
	SetCellText(pos, 0, strText, lClr);

	//总手
	dValue = CHSCalculate::GetZongshou(&m_stkInfo, pLevelData, strText);
	pos.y++;
	SetCellText(pos, 0, strText);

	//现手
	dValue = CHSCalculate::GetXianshou(&m_stkInfo, pOtherData, strText);
	pos.y++;
	SetCellText(pos, 0, strText);

	//总额
	dValue = pLevelData->m_fAvgPrice;
	CGeneralHelper::LongToString(dValue, 0, 10000, 2, strText, LTOA_MASK_HORLINE_IFZERO);
	pos.y++;
	SetCellText(pos, 0, strText);

	//利息
	dValue = pLevelData->m_IPOV;
	CGeneralHelper::LongToString(dValue, 0, 1000, 3, strText, LTOA_MASK_HORLINE_IFZERO);
	pos.y++;
	SetCellText(pos, 0, strText);

	//总笔
	dValue = pLevelData->m_lTickCount;
	pos.y++;
	CGeneralHelper::LongToString(dValue, 0, 1, 0, strText, LTOA_MASK_HORLINE_IFZERO);
	SetCellText(pos, 0, strText);

	//涨停
	dValue = m_stkInfo.m_fUpPrice < 0 ? 0:m_stkInfo.m_fUpPrice;
	CGeneralHelper::LongToString(dValue, 0, nUnit, nDecimal, strText, LTOA_MASK_HORLINE_IFZERO);
	pos.y++;
	SetCellText(pos, 0, strText);

	//市盈 
	dValue = CHSCalculate::GetShiying(&m_stkInfo, pLevelData, pFinanceData, strText);
	pos.y++;
	SetCellText(pos, 0, strText);

	//流通
	dValue = CHSCalculate::Getliutong(pFinanceData, strText);
	pos.y++;
	SetCellText(pos, 0, strText);

	//市值
	dValue = CHSCalculate::GetShizhi(pLevelData, pFinanceData, strText);
	pos.y++;
	SetCellText(pos, 0, strText);

	/////////////////////////////////  第四列 /////////////////////////////////////
	//委差=(委买手数－委卖手数)
	dValue = CHSCalculate::GetWeicha(pLevelData, 1, strText);
	pos.x = 3;
	pos.y = 0;
	lClr = GetTextColor(dValue, 0);
	SetCellText(pos, 0, strText, lClr);

	//均价 ＝（E分时成交的量×成交价）/总成交股数
	dValue = CHSCalculate::GetJunjia(&m_stkInfo, pLevelData, strText);
	pos.y++;
	lClr = GetTextColor(dValue, nClose);
	SetCellText(pos, 0, strText, lClr);

	//开盘
	dValue = pLevelData->m_lOpen;
	CGeneralHelper::LongToString(dValue, nClose, nUnit, nDecimal, strText, LTOA_MASK_HORLINE_IFZERO);
	pos.y++;
	lClr = GetTextColor(dValue, nClose);
	SetCellText(pos, 0, strText, lClr);

	//最高
	dValue = pLevelData->m_lMaxPrice;
	CGeneralHelper::LongToString(dValue, nClose, nUnit, nDecimal, strText, LTOA_MASK_HORLINE_IFZERO);
	pos.y++;
	lClr = GetTextColor(dValue, nClose);
	SetCellText(pos, 0, strText, lClr);

	//最低
	dValue = pLevelData->m_lMinPrice;
	CGeneralHelper::LongToString(dValue, nClose, nUnit, nDecimal, strText, LTOA_MASK_HORLINE_IFZERO);
	pos.y++;
	lClr = GetTextColor(dValue, nClose);
	SetCellText(pos, 0, strText, lClr);

	//量比
	dValue = CHSCalculate::GetLiangbi(&m_stkInfo, pLevelData, pOtherData, m_pDataSource, strText);
	pos.y++;
	SetCellText(pos, 0, strText);

	//换手
	dValue = CHSCalculate::GetHuanshou(&m_stkInfo, pLevelData, pFinanceData, strText);
	pos.y++;
	SetCellText(pos, 0, strText);

	//全价
	dValue = CHSCalculate::GetQuanjia(&m_stkInfo, pLevelData, strText);
	pos.y++;
	SetCellText(pos, 0, strText);

	//每笔
	dValue = CHSCalculate::GetMeiBi(pLevelData, strText);
	pos.y++;
	SetCellText(pos, 0, strText);

	//跌停
	dValue = m_stkInfo.m_fDownPrice < 0 ? 0:m_stkInfo.m_fDownPrice;
	CGeneralHelper::LongToString(dValue, 0, nUnit, nDecimal, strText, LTOA_MASK_HORLINE_IFZERO);
	pos.y++;
	SetCellText(pos, 0, strText);

	//市净
	dValue = CHSCalculate::GetShijing(pLevelData, pFinanceData, strText);
	pos.y++;
	SetCellText(pos, 0, strText);

	//总股
	dValue = CHSCalculate::GetZonggu(pFinanceData, strText);
	pos.y++;
	SetCellText(pos, 0, strText);

	//总值
	dValue = CHSCalculate::GetZongzhi(pLevelData, pFinanceData, strText);
	pos.y++;
	SetCellText(pos, 0, strText);

	if (pFinanceData)
	{
		delete pFinanceData;
		pFinanceData = NULL;
	}

	return TRUE;
}

BOOL CDrawRStockForm::UpdateStockData(ShareRealTimeData_Ext* pStock, StockOtherData* pOther)
{
	if ( pStock == NULL || ::IsBadReadPtr(pStock, 1) || pOther == NULL || ::IsBadReadPtr(pOther, 1))
	{
		return FALSE;
	}

	HSStockRealTime_Ext* pRealTimeExt = &pStock->m_nowDataExt;
	if (pRealTimeExt == NULL || ::IsBadReadPtr(pRealTimeExt,1))
	{
		return FALSE;
	}

	HSStockRealTime* pRealTime = &pRealTimeExt->m_stockRealTime;
	if (pRealTime == NULL || ::IsBadReadPtr(pRealTime,1))
	{
		return FALSE;
	}

	if ( memcmp(pRealTime, &m_sRealTime, sizeof(HSStockRealTime)) == 0)
	{// 过滤相同的主推包
		return FALSE;
	}
	memcpy(&m_sRealTime, pRealTime, sizeof(HSStockRealTime));

	int nHand = m_stkInfo.m_nHandNum;
	//////////////////////////////////////////////////////////////////////////
	// 得到财务数据
	CArray<CodeInfo> ayCode;
	CArray<CurrentFinanceData> ayFinanceData;
	ayCode.Add(m_stkInfo.m_ciStockCode);
	m_pDataSource->HSDataSourceEx_GetStocksFinanceData(ayCode, ayFinanceData);
	CHSFinanceData* pFinanceData = NULL;
	if (ayFinanceData.GetCount() == 1)
		pFinanceData = new CHSFinanceData(&ayFinanceData.GetAt(0));
	//

	BOOL bIsStop = TRUE;//是否停牌
	if (pRealTimeExt->m_stockOther.m_lStopFlag == 0)
	{
		bIsStop = FALSE;
	}

	CString str;
	char    strText[64];
	CPoint  pos(0,0);
	WORD    lClr;

	long    nClose   = m_stkInfo.m_lPrevClose;
	int     nUnit    = m_stkInfo.m_nPriceUnit;
	int     nDecimal = m_stkInfo.m_nDecimal;

	long BuyCount[] = {pRealTime->m_lBuyCount1, pRealTime->m_lBuyCount2, pRealTime->m_lBuyCount3,
	    pRealTime->m_lBuyCount4, pRealTime->m_lBuyCount5};
	long SellCount[] = {pRealTime->m_lSellCount1, pRealTime->m_lSellCount2, pRealTime->m_lSellCount3,
		pRealTime->m_lSellCount4, pRealTime->m_lSellCount5};
	long BuyPrice[] = {pRealTime->m_lBuyPrice1, pRealTime->m_lBuyPrice2, pRealTime->m_lBuyPrice3,
		pRealTime->m_lBuyPrice4, pRealTime->m_lBuyPrice5};
	long SellPrice[] = {pRealTime->m_lSellPrice1, pRealTime->m_lSellPrice2, pRealTime->m_lSellPrice3,
		pRealTime->m_lSellPrice4, pRealTime->m_lSellPrice5};
	// 初始化价格map
	InitPriceMap(m_buyMap,  BuyPrice);
	InitPriceMap(m_sellMap, SellPrice);

	double dValue = 0.0;
	//从第2栏开始
	//委比＝(委买手数－委卖手数)/(委买手数＋委卖手数)×100％
	pos.x = 1;
	pos.y = 0; 
	dValue = CHSCalculate::GetWeiBi(pRealTime, strText);
	lClr = GetTextColor(dValue, 0);
	SetCellText(pos, 0, strText, lClr);
	
	if (!m_bLevel)
	{
		//卖5~卖1 价
		for (int i = 4; i >=0; i--)
		{
			dValue = SellPrice[i];
			CGeneralHelper::LongToString(dValue, 0, nUnit, nDecimal, strText, LTOA_MASK_HORLINE_IFZERO);
			pos.y++;
			lClr = GetTextColor(dValue, nClose);
			SetCellText(pos, 0, strText, lClr);
		}

		//买1~买5 价
		for (int i = 0; i < 5; i++)
		{
			dValue = BuyPrice[i];
			CGeneralHelper::LongToString(dValue, 0, nUnit, nDecimal, strText, LTOA_MASK_HORLINE_IFZERO);
			pos.y++;
			lClr = GetTextColor(dValue, nClose);
			SetCellText(pos, 0, strText, lClr);
		}
	}

	//成交
	dValue = pRealTime->m_lNewPrice;
	m_nNewPrice = pRealTime->m_lNewPrice;
	CGeneralHelper::LongToString(dValue, nClose, nUnit, nDecimal, strText, LTOA_MASK_HORLINE_IFZERO);
	pos.y++;
	lClr = GetTextColor(dValue, nClose);
	SetCellText(pos, 0, strText, lClr);

	//涨跌
	dValue = CHSCalculate::GetZhangdie(bIsStop, &m_stkInfo, pRealTime, strText);
	pos.y++;
	lClr = GetTextColor(dValue, 0);
	SetCellText(pos, 0, strText, lClr);

	//幅度(涨幅=(现价-昨收)/昨收)
	pos.y++;
	dValue = CHSCalculate::GetFudu(bIsStop, &m_stkInfo, pRealTime, strText);
	lClr = GetTextColor(dValue, 0);
	SetCellText(pos, 0, strText, lClr);

	//总手
	pos.y++;
	dValue = CHSCalculate::GetZongshou(&m_stkInfo, pRealTime, strText);
	SetCellText(pos, 0, strText);

	//现手
	pos.y++;
	dValue = CHSCalculate::GetXianshou(&m_stkInfo, pOther, strText);
	SetCellText(pos, 0, strText);

	//总额
	dValue = pRealTime->m_fAvgPrice;
	CGeneralHelper::LongToString(dValue, 0, 10000, 2, strText, LTOA_MASK_HORLINE_IFZERO);
	pos.y++;
	SetCellText(pos, 0, strText);

	//外盘
	dValue = CHSCalculate::GetWaipan(&m_stkInfo, pOther, strText);
	pos.y++;
	SetCellText(pos, 0, strText);

	//涨停
	dValue = m_stkInfo.m_fUpPrice;
	CGeneralHelper::LongToString(dValue, 0, nUnit, nDecimal, strText, LTOA_MASK_HORLINE_IFZERO);
	pos.y++;
	SetCellText(pos, 0, strText);

	//市盈 
	pos.y++;
	dValue = CHSCalculate::GetShiying(&m_stkInfo, pRealTime, pFinanceData, strText);
	SetCellText(pos, 0, strText);

	//流通
	pos.y++;
	dValue = CHSCalculate::Getliutong(pFinanceData, strText);
	SetCellText(pos, 0, strText);

	//市值
	pos.y++;
	dValue = CHSCalculate::GetShizhi(pRealTime, pFinanceData, strText);
	SetCellText(pos, 0, strText);

	////////////////////////// 第三列 ////////////////////////////////////////
	pos.x = 2;
	pos.y = 0;
	//卖5 ~ 卖1量
	for (int i = 4; i >= 0; i--)
	{
		pos.y++;
		dValue = CHSCalculate::GetBuySellCount(&m_stkInfo, SellCount[i], strText);		
		SetCellText(pos, 0, strText);

		// 同时计算该档的成交量
		lClr = GetSellChange(SellPrice[i], (long)dValue, strText);
		SetCellText(CPoint(pos.x+1, pos.y), 0, strText, lClr);
	}

	// 买1 ~ 买5量
	for (int i = 0; i < 5; i++)
	{
		pos.y++;
		dValue = CHSCalculate::GetBuySellCount(&m_stkInfo, BuyCount[i], strText);
		SetCellText(pos, 0, strText);

		// 同时计算该档的成交量
		lClr = GetBuyChange(BuyPrice[i], (long)dValue, strText);
		SetCellText(CPoint(pos.x+1, pos.y), 0, strText, lClr);
	}

	/////////////////////////////////  第四列 /////////////////////////////////////
	//委差=(委买手数－委卖手数)
	dValue = CHSCalculate::GetWeicha(pRealTime, nHand, strText);
	pos.x = 3;
	pos.y = 0;
	lClr = GetTextColor(dValue, 0);
	SetCellText(pos, 0, strText, lClr);

	//卖5 ~ 卖1 量变化
	for (int i = 4; i >= 0; i--)
		pos.y++;

	//买1 ~ 买5 量变化
	for (int i = 0; i < 5; i++)
		pos.y++;

	//均价 ＝（E分时成交的量×成交价）/总成交股数
	pos.y++;
	dValue = CHSCalculate::GetJunjia(&m_stkInfo, pRealTime, strText);
	lClr = GetTextColor(dValue, nClose);
	SetCellText(pos, 0, strText, lClr);

	//开盘
	dValue = pRealTime->m_lOpen;
	CGeneralHelper::LongToString(dValue, nClose, nUnit, nDecimal, strText, LTOA_MASK_HORLINE_IFZERO);
	pos.y++;
	lClr = GetTextColor(dValue, nClose);
	SetCellText(pos, 0, strText, lClr);

	//最高
	dValue = pRealTime->m_lMaxPrice;
	CGeneralHelper::LongToString(dValue, nClose, nUnit, nDecimal, strText, LTOA_MASK_HORLINE_IFZERO);
	pos.y++;
	lClr = GetTextColor(dValue, nClose);
	SetCellText(pos, 0, strText, lClr);

	//最低
	dValue = pRealTime->m_lMinPrice;
	CGeneralHelper::LongToString(dValue, nClose, nUnit, nDecimal, strText, LTOA_MASK_HORLINE_IFZERO);
	pos.y++;
	lClr = GetTextColor(dValue, nClose);
	SetCellText(pos, 0, strText, lClr);

	//量比
	pos.y++;
	dValue = CHSCalculate::GetLiangbi(&m_stkInfo, pRealTime, pOther, m_pDataSource, strText);
	SetCellText(pos, 0, strText);
	
	//换手
	pos.y++;
	dValue = CHSCalculate::GetHuanshou(&m_stkInfo, pRealTime, pFinanceData, strText);
	SetCellText(pos, 0, strText);

	//内盘
	dValue = CHSCalculate::GetNeipan(&m_stkInfo, pOther, strText);
	pos.y++;
	SetCellText(pos, 0, strText);

	//跌停
	dValue = m_stkInfo.m_fDownPrice;
	CGeneralHelper::LongToString(dValue, 0, nUnit, nDecimal, strText, LTOA_MASK_HORLINE_IFZERO);
	pos.y++;
	SetCellText(pos, 0, strText);

	//市净
	pos.y++;
	dValue = CHSCalculate::GetShijing(pRealTime, pFinanceData, strText);
	SetCellText(pos, 0, strText);

	//总股
	pos.y++;
	dValue = CHSCalculate::GetZonggu(pFinanceData, strText);
	SetCellText(pos, 0, strText);

	//总值
	pos.y++;
	dValue = CHSCalculate::GetZongzhi(pRealTime, pFinanceData, strText);
	SetCellText(pos, 0, strText);

	if (pFinanceData)
	{
		delete pFinanceData;
		pFinanceData = NULL;
	}

	return TRUE;
}

BOOL CDrawRStockForm::UpdateStockData(LevelRealTime* pLevelData, LevelStockOtherData* pOtherData) // 更新股票数据
{
	if (pLevelData == NULL || pOtherData == NULL)
		return FALSE;

	int nHand = m_stkInfo.m_nHandNum;
	//////////////////////////////////////////////////////////////////////////
	// 得到财务数据
	CArray<CodeInfo> ayCode;
	CArray<CurrentFinanceData> ayFinanceData;
	ayCode.Add(m_stkInfo.m_ciStockCode);
	m_pDataSource->HSDataSourceEx_GetStocksFinanceData(ayCode, ayFinanceData);
	CHSFinanceData* pFinanceData = NULL;
	if (ayFinanceData.GetCount() == 1)
		pFinanceData = new CHSFinanceData(&ayFinanceData.GetAt(0));
	//

	BOOL bIsStop = TRUE;//是否停牌
	if (pLevelData->m_lOpen > 0)
		bIsStop = FALSE;

	CString str;
	char    strText[64];
	CPoint  pos(0,0);
	WORD    lClr;

	long    nClose   = m_stkInfo.m_lPrevClose;
	int     nUnit    = m_stkInfo.m_nPriceUnit;
	int     nDecimal = m_stkInfo.m_nDecimal;

	double dValue = 0.0;
	//从第2栏开始
	//委比＝(委买手数－委卖手数)/(委买手数＋委卖手数)×100％
	pos.x = 1;
	pos.y = 0; 
	dValue = CHSCalculate::GetWeiBi(pLevelData, strText);
	lClr = GetTextColor(dValue, 0);
	SetCellText(pos, 0, strText, lClr);

	//成交
	dValue = pLevelData->m_lNewPrice;
	m_nNewPrice = pLevelData->m_lNewPrice;
	CGeneralHelper::LongToString(dValue, nClose, nUnit, nDecimal, strText, LTOA_MASK_HORLINE_IFZERO);
	pos.y++;
	lClr = GetTextColor(dValue, nClose);
	SetCellText(pos, 0, strText, lClr);

	//涨跌
	dValue = CHSCalculate::GetZhangdie(bIsStop, &m_stkInfo, pLevelData, strText);
	pos.y++;
	lClr = GetTextColor(dValue, 0);
	SetCellText(pos, 0, strText, lClr);

	//幅度(涨幅=(现价-昨收)/昨收)
	dValue = CHSCalculate::GetFudu(bIsStop, &m_stkInfo, pLevelData, strText);
	pos.y++;
	lClr = GetTextColor(dValue, 0);
	SetCellText(pos, 0, strText, lClr);

	//总手
	dValue = CHSCalculate::GetZongshou(&m_stkInfo, pLevelData, strText);
	pos.y++;
	SetCellText(pos, 0, strText);

	//现手
	dValue = CHSCalculate::GetXianshou(&m_stkInfo, pOtherData, strText);
	pos.y++;
	SetCellText(pos, 0, strText);

	//总额
	dValue = pLevelData->m_fAvgPrice;
	CGeneralHelper::LongToString(dValue, 0, 10000, 2, strText, LTOA_MASK_HORLINE_IFZERO);
	pos.y++;
	SetCellText(pos, 0, strText);

	//外盘
	dValue = CHSCalculate::GetWaipan(&m_stkInfo, pOtherData, strText);
	pos.y++;
	SetCellText(pos, 0, strText);

	//总笔
	dValue = pLevelData->m_lTickCount;
	pos.y++;
	CGeneralHelper::LongToString(dValue, 0, 1, 0, strText, LTOA_MASK_HORLINE_IFZERO);
	SetCellText(pos, 0, strText);

	//涨停
	dValue = m_stkInfo.m_fUpPrice;
	CGeneralHelper::LongToString(dValue, 0, nUnit, nDecimal, strText, LTOA_MASK_HORLINE_IFZERO);
	pos.y++;
	SetCellText(pos, 0, strText);

	//市盈 
	dValue = CHSCalculate::GetShiying(&m_stkInfo, pLevelData, pFinanceData, strText);
	pos.y++;
	SetCellText(pos, 0, strText);

	//流通
	dValue = CHSCalculate::Getliutong(pFinanceData, strText);
	pos.y++;
	SetCellText(pos, 0, strText);

	//市值
	dValue = CHSCalculate::GetShizhi(pLevelData, pFinanceData, strText);
	pos.y++;
	SetCellText(pos, 0, strText);

	/////////////////////////////////  第四列 /////////////////////////////////////
	//委差=(委买手数－委卖手数)
	dValue = CHSCalculate::GetWeicha(pLevelData, nHand, strText);
	pos.x = 3;
	pos.y = 0;
	lClr = GetTextColor(dValue, 0);
	SetCellText(pos, 0, strText, lClr);

	//均价 ＝（E分时成交的量×成交价）/总成交股数
	dValue = CHSCalculate::GetJunjia(&m_stkInfo, pLevelData, strText);
	pos.y++;
	lClr = GetTextColor(dValue, nClose);
	SetCellText(pos, 0, strText, lClr);

	//开盘
	dValue = pLevelData->m_lOpen;
	CGeneralHelper::LongToString(dValue, nClose, nUnit, nDecimal, strText, LTOA_MASK_HORLINE_IFZERO);
	pos.y++;
	lClr = GetTextColor(dValue, nClose);
	SetCellText(pos, 0, strText, lClr);

	//最高
	dValue = pLevelData->m_lMaxPrice;
	CGeneralHelper::LongToString(dValue, nClose, nUnit, nDecimal, strText, LTOA_MASK_HORLINE_IFZERO);
	pos.y++;
	lClr = GetTextColor(dValue, nClose);
	SetCellText(pos, 0, strText, lClr);

	//最低
	dValue = pLevelData->m_lMinPrice;
	CGeneralHelper::LongToString(dValue, nClose, nUnit, nDecimal, strText, LTOA_MASK_HORLINE_IFZERO);
	pos.y++;
	lClr = GetTextColor(dValue, nClose);
	SetCellText(pos, 0, strText, lClr);

	//量比
	dValue = CHSCalculate::GetLiangbi(&m_stkInfo, pLevelData, pOtherData, m_pDataSource, strText);
	pos.y++;
	SetCellText(pos, 0, strText);

	//换手
	dValue = CHSCalculate::GetHuanshou(&m_stkInfo, pLevelData, pFinanceData, strText);
	pos.y++;
	SetCellText(pos, 0, strText);

	//内盘
	dValue = CHSCalculate::GetNeipan(&m_stkInfo, pOtherData, strText);
	pos.y++;
	SetCellText(pos, 0, strText);

	//每笔
	dValue = CHSCalculate::GetMeiBi(pLevelData, strText);
	pos.y++;
	SetCellText(pos, 0, strText);

	//跌停
	dValue = m_stkInfo.m_fDownPrice;
	CGeneralHelper::LongToString(dValue, 0, nUnit, nDecimal, strText, LTOA_MASK_HORLINE_IFZERO);
	pos.y++;
	SetCellText(pos, 0, strText);

	//市净
	dValue = CHSCalculate::GetShijing(pLevelData, pFinanceData, strText);
	pos.y++;
	SetCellText(pos, 0, strText);

	//总股
	dValue = CHSCalculate::GetZonggu(pFinanceData, strText);
	pos.y++;
	SetCellText(pos, 0, strText);

	//总值
	dValue = CHSCalculate::GetZongzhi(pLevelData, pFinanceData, strText);
	pos.y++;
	SetCellText(pos, 0, strText);

	if (pFinanceData)
	{
		delete pFinanceData;
		pFinanceData = NULL;
	}

	return TRUE;
}

BOOL CDrawRStockForm::UpdateFundData(ShareRealTimeData_Ext* pStock, StockOtherData* pOther)
{
	if ( pStock == NULL || ::IsBadReadPtr(pStock, 1) || pOther == NULL || ::IsBadReadPtr(pOther, 1))
	{
		return FALSE;
	}

	HSStockRealTime_Ext* pRealTimeExt = &pStock->m_nowDataExt;
	if (pRealTimeExt == NULL || ::IsBadReadPtr(pRealTimeExt,1))
	{
		return FALSE;
	}

	HSStockRealTime* pRealTime = &pRealTimeExt->m_stockRealTime;
	if (pRealTime == NULL || ::IsBadReadPtr(pRealTime,1))
	{
		return FALSE;
	}

	if ( memcmp(pRealTime, &m_sRealTime, sizeof(HSStockRealTime)) == 0)
	{// 过滤相同的主推包
		return FALSE;
	}
	memcpy(&m_sRealTime, pRealTime, sizeof(HSStockRealTime));

	int nHand = m_stkInfo.m_nHandNum;  // 基金一手100份
	//////////////////////////////////////////////////////////////////////////
	// 得到财务数据
	CArray<CodeInfo> ayCode;
	CArray<CurrentFinanceData> ayFinanceData;
	ayCode.Add(m_stkInfo.m_ciStockCode);
	m_pDataSource->HSDataSourceEx_GetStocksFinanceData(ayCode, ayFinanceData);
	CHSFinanceData* pFinanceData = NULL;
	if (ayFinanceData.GetCount() == 1)
		pFinanceData = new CHSFinanceData(&ayFinanceData.GetAt(0));
	//

	BOOL bIsStop = TRUE;//是否停牌
	if (pRealTimeExt->m_stockOther.m_lStopFlag == 0)
	{
		bIsStop = FALSE;
	}

	CString str;
	char    strText[64];
	CPoint  pos(0,0);
	WORD    lClr;

	long    nClose   = m_stkInfo.m_lPrevClose;
	int     nUnit    = m_stkInfo.m_nPriceUnit;
	int     nDecimal = m_stkInfo.m_nDecimal;

	long BuyCount[] = {pRealTime->m_lBuyCount1, pRealTime->m_lBuyCount2, pRealTime->m_lBuyCount3,
		pRealTime->m_lBuyCount4, pRealTime->m_lBuyCount5};
	long SellCount[] = {pRealTime->m_lSellCount1, pRealTime->m_lSellCount2, pRealTime->m_lSellCount3,
		pRealTime->m_lSellCount4, pRealTime->m_lSellCount5};
	long BuyPrice[] = {pRealTime->m_lBuyPrice1, pRealTime->m_lBuyPrice2, pRealTime->m_lBuyPrice3,
		pRealTime->m_lBuyPrice4, pRealTime->m_lBuyPrice5};
	long SellPrice[] = {pRealTime->m_lSellPrice1, pRealTime->m_lSellPrice2, pRealTime->m_lSellPrice3,
		pRealTime->m_lSellPrice4, pRealTime->m_lSellPrice5};
	// 初始化价格map
	InitPriceMap(m_buyMap,  BuyPrice);
	InitPriceMap(m_sellMap, SellPrice);

	double dValue = 0.0;
	//从第2栏开始
	//委比＝(委买手数－委卖手数)/(委买手数＋委卖手数)×100％
	pos.x = 1;
	pos.y = 0; 
	dValue = CHSCalculate::GetWeiBi(pRealTime, strText);
	lClr = GetTextColor(dValue, 0);
	SetCellText(pos, 0, strText, lClr);

	if (!m_bLevel)
	{
		//卖5~卖1 价
		for (int i = 4; i >=0; i--)
		{
			dValue = SellPrice[i];
			CGeneralHelper::LongToString(dValue, 0, nUnit, nDecimal, strText, LTOA_MASK_HORLINE_IFZERO);
			pos.y++;
			lClr = GetTextColor(dValue, nClose);
			SetCellText(pos, 0, strText, lClr);
		}

		//买1~买5 价
		for (int i = 0; i < 5; i++)
		{
			dValue = BuyPrice[i];
			CGeneralHelper::LongToString(dValue, 0, nUnit, nDecimal, strText, LTOA_MASK_HORLINE_IFZERO);
			pos.y++;
			lClr = GetTextColor(dValue, nClose);
			SetCellText(pos, 0, strText, lClr);
		}
	}

	//成交
	dValue = pRealTime->m_lNewPrice;
	m_nNewPrice = pRealTime->m_lNewPrice;
	CGeneralHelper::LongToString(dValue, nClose, nUnit, nDecimal, strText, LTOA_MASK_HORLINE_IFZERO);
	pos.y++;
	lClr = GetTextColor(dValue, nClose);
	SetCellText(pos, 0, strText, lClr);

	//涨跌
	dValue = CHSCalculate::GetZhangdie(bIsStop, &m_stkInfo, pRealTime, strText);
	pos.y++;
	lClr = GetTextColor(dValue, 0);
	SetCellText(pos, 0, strText, lClr);

	//幅度(涨幅=(现价-昨收)/昨收)
	dValue = CHSCalculate::GetFudu(bIsStop, &m_stkInfo, pRealTime, strText);
	pos.y++;
	lClr = GetTextColor(dValue, 0);
	SetCellText(pos, 0, strText, lClr);

	//总手
	dValue = CHSCalculate::GetZongshou(&m_stkInfo, pRealTime, strText);
	pos.y++;
	SetCellText(pos, 0, strText);

	//现手
	dValue = CHSCalculate::GetXianshou(&m_stkInfo, pOther, strText);
	pos.y++;
	SetCellText(pos, 0, strText);

	//总额
	dValue = pRealTime->m_fAvgPrice;
	CGeneralHelper::LongToString(dValue, 0, 10000, 2, strText, LTOA_MASK_HORLINE_IFZERO);
	pos.y++;
	SetCellText(pos, 0, strText);

	//外盘
	dValue = CHSCalculate::GetWaipan(&m_stkInfo, pOther, strText);
	pos.y++;
	SetCellText(pos, 0, strText);

	//涨停
	dValue = m_stkInfo.m_fUpPrice;
	CGeneralHelper::LongToString(dValue, 0, nUnit, nDecimal, strText, LTOA_MASK_HORLINE_IFZERO);
	pos.y++;
	SetCellText(pos, 0, strText);

	//市盈 
	dValue = CHSCalculate::GetShiying(&m_stkInfo, pRealTime, pFinanceData, strText);
	pos.y++;
	SetCellText(pos, 0, strText);

	//流通
	dValue = CHSCalculate::Getliutong(pFinanceData, strText);
	pos.y++;
	SetCellText(pos, 0, strText);

	//市值
	dValue = CHSCalculate::GetShizhi(pRealTime, pFinanceData, strText);
	pos.y++;
	SetCellText(pos, 0, strText);

	////////////////////////// 第三列 ////////////////////////////////////////
	pos.x = 2;
	pos.y = 0;
	//卖5 ~ 卖1量
	for (int i = 4; i >= 0; i--)
	{
		pos.y++;
		dValue = CHSCalculate::GetBuySellCount(&m_stkInfo, SellCount[i], strText);		
		SetCellText(pos, 0, strText);

		// 同时计算该档的成交量
		lClr = GetSellChange(SellPrice[i], (long)dValue, strText);
		SetCellText(CPoint(pos.x+1, pos.y), 0, strText, lClr);
	}

	// 买1 ~ 买5量
	for (int i = 0; i < 5; i++)
	{
		pos.y++;
		dValue = CHSCalculate::GetBuySellCount(&m_stkInfo, BuyCount[i], strText);
		SetCellText(pos, 0, strText);

		// 同时计算该档的成交量
		lClr = GetBuyChange(BuyPrice[i], (long)dValue, strText);
		SetCellText(CPoint(pos.x+1, pos.y), 0, strText, lClr);
	}

	/////////////////////////////////  第四列 /////////////////////////////////////
	//委差=(委买手数－委卖手数)
	dValue = CHSCalculate::GetWeicha(pRealTime, nHand, strText);
	pos.x = 3;
	pos.y = 0;
	lClr = GetTextColor(dValue, 0);
	SetCellText(pos, 0, strText, lClr);

	//卖5 ~ 卖1 量变化
	for (int i = 4; i >= 0; i--)
		pos.y++;

	//买1 ~ 买5 量变化
	for (int i = 0; i < 5; i++)
		pos.y++;

	//均价 ＝（E分时成交的量×成交价）/总成交股数
	dValue = CHSCalculate::GetJunjia(&m_stkInfo, pRealTime, strText);
	pos.y++;
	lClr = GetTextColor(dValue, nClose);
	SetCellText(pos, 0, strText, lClr);

	//开盘
	dValue = pRealTime->m_lOpen;
	CGeneralHelper::LongToString(dValue, nClose, nUnit, nDecimal, strText, LTOA_MASK_HORLINE_IFZERO);
	pos.y++;
	lClr = GetTextColor(dValue, nClose);
	SetCellText(pos, 0, strText, lClr);

	//最高
	dValue = pRealTime->m_lMaxPrice;
	CGeneralHelper::LongToString(dValue, nClose, nUnit, nDecimal, strText, LTOA_MASK_HORLINE_IFZERO);
	pos.y++;
	lClr = GetTextColor(dValue, nClose);
	SetCellText(pos, 0, strText, lClr);

	//最低
	dValue = pRealTime->m_lMinPrice;
	CGeneralHelper::LongToString(dValue, nClose, nUnit, nDecimal, strText, LTOA_MASK_HORLINE_IFZERO);
	pos.y++;
	lClr = GetTextColor(dValue, nClose);
	SetCellText(pos, 0, strText, lClr);

	//量比
	dValue = CHSCalculate::GetLiangbi(&m_stkInfo, pRealTime, pOther, m_pDataSource, strText);
	pos.y++;
	SetCellText(pos, 0, strText);

	//净值
	dValue = CHSCalculate::GetJingZhi(&m_stkInfo, pRealTime, pFinanceData, strText);
	pos.y++;
	SetCellText(pos, 0, strText);

	//内盘
	dValue = CHSCalculate::GetNeipan(&m_stkInfo, pOther, strText);
	pos.y++;
	SetCellText(pos, 0, strText);

	//跌停
	dValue = m_stkInfo.m_fDownPrice;
	CGeneralHelper::LongToString(dValue, 0, nUnit, nDecimal, strText, LTOA_MASK_HORLINE_IFZERO);
	pos.y++;
	SetCellText(pos, 0, strText);


	//市净
	dValue = CHSCalculate::GetShijing(pRealTime, pFinanceData, strText);
	pos.y++;
	SetCellText(pos, 0, strText);

	//总股
	dValue = CHSCalculate::GetZonggu(pFinanceData, strText);
	pos.y++;
	SetCellText(pos, 0, strText);

	//总值
	dValue = CHSCalculate::GetZongzhi(pRealTime, pFinanceData, strText);
	pos.y++;
	SetCellText(pos, 0, strText);

	if (pFinanceData)
	{
		delete pFinanceData;
		pFinanceData = NULL;
	}

	return TRUE;
}

BOOL CDrawRStockForm::UpdateFundData(LevelRealTime* pLevelData, LevelStockOtherData* pOtherData)
{
	if (pLevelData == NULL || pOtherData == NULL)
		return FALSE;

	int nHand = m_stkInfo.m_nHandNum;  // 基金一手100份
	//////////////////////////////////////////////////////////////////////////
	// 得到财务数据
	CArray<CodeInfo> ayCode;
	CArray<CurrentFinanceData> ayFinanceData;
	ayCode.Add(m_stkInfo.m_ciStockCode);
	m_pDataSource->HSDataSourceEx_GetStocksFinanceData(ayCode, ayFinanceData);
	CHSFinanceData* pFinanceData = NULL;
	if (ayFinanceData.GetCount() == 1)
		pFinanceData = new CHSFinanceData(&ayFinanceData.GetAt(0));
	//

	BOOL bIsStop = TRUE;//是否停牌
	if (pLevelData->m_lOpen > 0)
		bIsStop = FALSE;
	
	CString str;
	char    strText[64];
	CPoint  pos(0,0);
	WORD    lClr;

	long    nClose   = m_stkInfo.m_lPrevClose;
	int     nUnit    = m_stkInfo.m_nPriceUnit;
	int     nDecimal = m_stkInfo.m_nDecimal;

	double dValue = 0.0;
	//从第2栏开始
	//委比＝(委买手数－委卖手数)/(委买手数＋委卖手数)×100％
	pos.x = 1;
	pos.y = 0; 
	dValue = CHSCalculate::GetWeiBi(pLevelData, strText);
	lClr = GetTextColor(dValue, 0);
	SetCellText(pos, 0, strText, lClr);
	
	//成交
	dValue = pLevelData->m_lNewPrice;
	m_nNewPrice = pLevelData->m_lNewPrice;
	CGeneralHelper::LongToString(dValue, nClose, nUnit, nDecimal, strText, LTOA_MASK_HORLINE_IFZERO);
	pos.y++;
	lClr = GetTextColor(dValue, nClose);
	SetCellText(pos, 0, strText, lClr);

	//涨跌
	dValue = CHSCalculate::GetZhangdie(bIsStop, &m_stkInfo, pLevelData, strText);
	pos.y++;
	lClr = GetTextColor(dValue, 0);
	SetCellText(pos, 0, strText, lClr);

	//幅度(涨幅=(现价-昨收)/昨收)
	dValue = CHSCalculate::GetFudu(bIsStop, &m_stkInfo, pLevelData, strText);
	pos.y++;
	lClr = GetTextColor(dValue, 0);
	SetCellText(pos, 0, strText, lClr);

	//总手
	dValue = CHSCalculate::GetZongshou(&m_stkInfo, pLevelData, strText);
	pos.y++;
	SetCellText(pos, 0, strText);

	//现手
	dValue = CHSCalculate::GetXianshou(&m_stkInfo, pOtherData, strText);
	pos.y++;
	SetCellText(pos, 0, strText);

	//总额
	dValue = pLevelData->m_fAvgPrice;
	CGeneralHelper::LongToString(dValue, 0, 10000, 2, strText, LTOA_MASK_HORLINE_IFZERO);
	pos.y++;
	SetCellText(pos, 0, strText);

	//外盘
	dValue = CHSCalculate::GetWaipan(&m_stkInfo, pOtherData, strText);
	pos.y++;
	SetCellText(pos, 0, strText);
	
	//总笔
	dValue = pLevelData->m_lTickCount;
	pos.y++;
	CGeneralHelper::LongToString(dValue, 0, 1, 0, strText, LTOA_MASK_HORLINE_IFZERO);
	SetCellText(pos, 0, strText);

	//涨停
	dValue = m_stkInfo.m_fUpPrice;
	CGeneralHelper::LongToString(dValue, 0, nUnit, nDecimal, strText, LTOA_MASK_HORLINE_IFZERO);
	pos.y++;
	SetCellText(pos, 0, strText);

	//市盈 
	dValue = CHSCalculate::GetShiying(&m_stkInfo, pLevelData, pFinanceData, strText);
	pos.y++;
	SetCellText(pos, 0, strText);

	//流通
	dValue = CHSCalculate::Getliutong(pFinanceData, strText);
	pos.y++;
	SetCellText(pos, 0, strText);

	//市值
	dValue = CHSCalculate::GetShizhi(pLevelData, pFinanceData, strText);
	pos.y++;
	SetCellText(pos, 0, strText);

	/////////////////////////////////  第四列 /////////////////////////////////////
	//委差=(委买手数－委卖手数)
	dValue = CHSCalculate::GetWeicha(pLevelData, nHand, strText);
	pos.x = 3;
	pos.y = 0;
	lClr = GetTextColor(dValue, 0);
	SetCellText(pos, 0, strText, lClr);

	//均价 ＝（E分时成交的量×成交价）/总成交股数
	dValue = CHSCalculate::GetJunjia(&m_stkInfo, pLevelData, strText);
	pos.y++;
	lClr = GetTextColor(dValue, nClose);
	SetCellText(pos, 0, strText, lClr);

	//开盘
	dValue = pLevelData->m_lOpen;
	CGeneralHelper::LongToString(dValue, nClose, nUnit, nDecimal, strText, LTOA_MASK_HORLINE_IFZERO);
	pos.y++;
	lClr = GetTextColor(dValue, nClose);
	SetCellText(pos, 0, strText, lClr);

	//最高
	dValue = pLevelData->m_lMaxPrice;
	CGeneralHelper::LongToString(dValue, nClose, nUnit, nDecimal, strText, LTOA_MASK_HORLINE_IFZERO);
	pos.y++;
	lClr = GetTextColor(dValue, nClose);
	SetCellText(pos, 0, strText, lClr);

	//最低
	dValue = pLevelData->m_lMinPrice;
	CGeneralHelper::LongToString(dValue, nClose, nUnit, nDecimal, strText, LTOA_MASK_HORLINE_IFZERO);
	pos.y++;
	lClr = GetTextColor(dValue, nClose);
	SetCellText(pos, 0, strText, lClr);

	//量比
	dValue = CHSCalculate::GetLiangbi(&m_stkInfo, pLevelData, pOtherData, m_pDataSource, strText);
	pos.y++;
	SetCellText(pos, 0, strText);

	//净值
	dValue = CHSCalculate::GetJingZhi(&m_stkInfo, pLevelData, pFinanceData, strText);
	pos.y++;
	SetCellText(pos, 0, strText);

	//内盘
	dValue = CHSCalculate::GetNeipan(&m_stkInfo, pOtherData, strText);
	pos.y++;
	SetCellText(pos, 0, strText);

	//每笔
	dValue = CHSCalculate::GetMeiBi(pLevelData, strText);
	pos.y++;
	SetCellText(pos, 0, strText);

	//跌停
	dValue = m_stkInfo.m_fDownPrice;
	CGeneralHelper::LongToString(dValue, 0, nUnit, nDecimal, strText, LTOA_MASK_HORLINE_IFZERO);
	pos.y++;
	SetCellText(pos, 0, strText);

	//市净
	dValue = CHSCalculate::GetShijing(pLevelData, pFinanceData, strText);
	pos.y++;
	SetCellText(pos, 0, strText);

	//总股
	dValue = CHSCalculate::GetZonggu(pFinanceData, strText);
	pos.y++;
	SetCellText(pos, 0, strText);

	//总值
	dValue = CHSCalculate::GetZongzhi(pLevelData, pFinanceData, strText);
	pos.y++;
	SetCellText(pos, 0, strText);

	if (pFinanceData)
	{
		delete pFinanceData;
		pFinanceData = NULL;
	}

	return TRUE;

}

BOOL CDrawRStockForm::UpdateETFData(ShareRealTimeData_Ext* pStock, StockOtherData* pOther)
{
	if ( pStock == NULL || ::IsBadReadPtr(pStock, 1) || pOther == NULL || ::IsBadReadPtr(pOther, 1))
	{
		return FALSE;
	}

	HSStockRealTime_Ext* pRealTimeExt = &pStock->m_nowDataExt;
	if (pRealTimeExt == NULL || ::IsBadReadPtr(pRealTimeExt,1))
	{
		return FALSE;
	}

	HSStockRealTime* pRealTime = &pRealTimeExt->m_stockRealTime;
	if (pRealTime == NULL || ::IsBadReadPtr(pRealTime,1))
	{
		return FALSE;
	}

	if ( memcmp(pRealTime, &m_sRealTime, sizeof(HSStockRealTime)) == 0)
	{// 过滤相同的主推包
		return FALSE;
	}
	memcpy(&m_sRealTime, pRealTime, sizeof(HSStockRealTime));

	int nHand = m_stkInfo.m_nHandNum; // EFT 1手 = 100一份
	//////////////////////////////////////////////////////////////////////////
	// 得到财务数据
	CArray<CodeInfo> ayCode;
	CArray<CurrentFinanceData> ayFinanceData;
	ayCode.Add(m_stkInfo.m_ciStockCode);
	m_pDataSource->HSDataSourceEx_GetStocksFinanceData(ayCode, ayFinanceData);
	CHSFinanceData* pFinanceData = NULL;
	if (ayFinanceData.GetCount() == 1)
		pFinanceData = new CHSFinanceData(&ayFinanceData.GetAt(0));
	//

	BOOL bIsStop = TRUE;//是否停牌
	if (pRealTimeExt->m_stockOther.m_lStopFlag == 0)
	{
		bIsStop = FALSE;
	}

	CString str;
	char    strText[64];
	CPoint  pos(0,0);
	WORD    lClr;

	long    nClose   = m_stkInfo.m_lPrevClose;
	int     nUnit    = m_stkInfo.m_nPriceUnit;
	int     nDecimal = m_stkInfo.m_nDecimal;

	long BuyCount[] = {pRealTime->m_lBuyCount1, pRealTime->m_lBuyCount2, pRealTime->m_lBuyCount3,
		pRealTime->m_lBuyCount4, pRealTime->m_lBuyCount5};
	long SellCount[] = {pRealTime->m_lSellCount1, pRealTime->m_lSellCount2, pRealTime->m_lSellCount3,
		pRealTime->m_lSellCount4, pRealTime->m_lSellCount5};
	long BuyPrice[] = {pRealTime->m_lBuyPrice1, pRealTime->m_lBuyPrice2, pRealTime->m_lBuyPrice3,
		pRealTime->m_lBuyPrice4, pRealTime->m_lBuyPrice5};
	long SellPrice[] = {pRealTime->m_lSellPrice1, pRealTime->m_lSellPrice2, pRealTime->m_lSellPrice3,
		pRealTime->m_lSellPrice4, pRealTime->m_lSellPrice5};
	// 初始化价格map
	InitPriceMap(m_buyMap,  BuyPrice);
	InitPriceMap(m_sellMap, SellPrice);
	
	//从第2栏开始
	double dValue = 0.0;
	//委比＝(委买手数－委卖手数)/(委买手数＋委卖手数)×100％	
	dValue = CHSCalculate::GetWeiBi(pRealTime, strText);
	pos.x = 1;
	pos.y = 0; 
	lClr = GetTextColor(dValue, 0);
	SetCellText(pos, 0, strText, lClr);

	if (!m_bLevel)
	{
		//卖5~卖1 价
		for (int i = 4; i >=0; i--)
		{
			dValue = SellPrice[i];
			CGeneralHelper::LongToString(dValue, 0, nUnit, nDecimal, strText, LTOA_MASK_HORLINE_IFZERO);
			pos.y++;
			lClr = GetTextColor(dValue, nClose);
			SetCellText(pos, 0, strText, lClr);
		}

		//买1~买5 价
		for (int i = 0; i < 5; i++)
		{
			dValue = BuyPrice[i];
			CGeneralHelper::LongToString(dValue, 0, nUnit, nDecimal, strText, LTOA_MASK_HORLINE_IFZERO);
			pos.y++;
			lClr = GetTextColor(dValue, nClose);
			SetCellText(pos, 0, strText, lClr);
		}
	}
	
	//成交
	dValue = pRealTime->m_lNewPrice;
	m_nNewPrice = pRealTime->m_lNewPrice;
	CGeneralHelper::LongToString(dValue, nClose, nUnit, nDecimal, strText, LTOA_MASK_HORLINE_IFZERO);
	pos.y++;
	lClr = GetTextColor(dValue, nClose);
	SetCellText(pos, 0, strText, lClr);

	//涨跌
	dValue = CHSCalculate::GetZhangdie(bIsStop, &m_stkInfo, pRealTime, strText);
	pos.y++;
	lClr = GetTextColor(dValue, 0);
	SetCellText(pos, 0, strText, lClr);

	//幅度(涨幅=(现价-昨收)/昨收)
	dValue = CHSCalculate::GetFudu(bIsStop, &m_stkInfo, pRealTime, strText);
	pos.y++;
	lClr = GetTextColor(dValue, 0);
	SetCellText(pos, 0, strText, lClr);

	//总手
	dValue = CHSCalculate::GetZongshou(&m_stkInfo, pRealTime, strText);
	pos.y++;
	SetCellText(pos, 0, strText);

	//现手
	dValue = CHSCalculate::GetXianshou(&m_stkInfo, pOther, strText);
	pos.y++;
	SetCellText(pos, 0, strText);

	//总额
	dValue = pRealTime->m_fAvgPrice;
	CGeneralHelper::LongToString(dValue, 0, 10000, 2, strText, LTOA_MASK_HORLINE_IFZERO);
	pos.y++;
	SetCellText(pos, 0, strText);

	//外盘
	dValue = CHSCalculate::GetWaipan(&m_stkInfo, pOther, strText);
	pos.y++;
	SetCellText(pos, 0, strText);

	//涨停
	dValue = m_stkInfo.m_fUpPrice;
	CGeneralHelper::LongToString(dValue, 0, nUnit, nDecimal, strText, LTOA_MASK_HORLINE_IFZERO);
	pos.y++;
	SetCellText(pos, 0, strText);

	//市盈 
	dValue = CHSCalculate::GetShiying(&m_stkInfo, pRealTime, pFinanceData, strText);
	pos.y++;
	SetCellText(pos, 0, strText);

	//流通
	dValue = CHSCalculate::Getliutong(pFinanceData, strText);
	pos.y++;
	SetCellText(pos, 0, strText);

	//市值
	dValue = CHSCalculate::GetShizhi(pRealTime, pFinanceData, strText);
	pos.y++;
	SetCellText(pos, 0, strText);

	////////////////////////// 第三列 ////////////////////////////////////////
	pos.x = 2;
	pos.y = 0;
	//卖5 ~ 卖1量
	for (int i = 4; i >= 0; i--)
	{
		pos.y++;
		dValue = CHSCalculate::GetBuySellCount(&m_stkInfo, SellCount[i], strText);		
		SetCellText(pos, 0, strText);

		// 同时计算该档的成交量
		lClr = GetSellChange(SellPrice[i], (long)dValue, strText);
		SetCellText(CPoint(pos.x+1, pos.y), 0, strText, lClr);
	}

	// 买1 ~ 买5量
	for (int i = 0; i < 5; i++)
	{
		pos.y++;
		dValue = CHSCalculate::GetBuySellCount(&m_stkInfo, BuyCount[i], strText);
		SetCellText(pos, 0, strText);

		// 同时计算该档的成交量
		lClr = GetBuyChange(BuyPrice[i], (long)dValue, strText);
		SetCellText(CPoint(pos.x+1, pos.y), 0, strText, lClr);
	}

	/////////////////////////////////  第四列 /////////////////////////////////////
	//委差=(委买手数－委卖手数)
	dValue = CHSCalculate::GetWeicha(pRealTime, nHand, strText);
	pos.x = 3;
	pos.y = 0;
	lClr = GetTextColor(dValue, 0);
	SetCellText(pos, 0, strText, lClr);

	//卖5 ~ 卖1 量变化
	for (int i = 4; i >= 0; i--)
		pos.y++;

	//买1 ~ 买5 量变化
	for (int i = 0; i < 5; i++)
		pos.y++;

	//均价 ＝（E分时成交的量×成交价）/总成交股数
	dValue = CHSCalculate::GetJunjia(&m_stkInfo, pRealTime, strText);
	pos.y++;
	lClr = GetTextColor(dValue, nClose);
	SetCellText(pos, 0, strText, lClr);

	//开盘
	dValue = pRealTime->m_lOpen;
	CGeneralHelper::LongToString(dValue, nClose, nUnit, nDecimal, strText, LTOA_MASK_HORLINE_IFZERO);
	pos.y++;
	lClr = GetTextColor(dValue, nClose);
	SetCellText(pos, 0, strText, lClr);

	//最高
	dValue = pRealTime->m_lMaxPrice;
	CGeneralHelper::LongToString(dValue, nClose, nUnit, nDecimal, strText, LTOA_MASK_HORLINE_IFZERO);
	pos.y++;
	lClr = GetTextColor(dValue, nClose);
	SetCellText(pos, 0, strText, lClr);

	//最低
	dValue = pRealTime->m_lMinPrice;
	CGeneralHelper::LongToString(dValue, nClose, nUnit, nDecimal, strText, LTOA_MASK_HORLINE_IFZERO);
	pos.y++;
	lClr = GetTextColor(dValue, nClose);
	SetCellText(pos, 0, strText, lClr);

	//量比
	dValue = CHSCalculate::GetLiangbi(&m_stkInfo, pRealTime, pOther, m_pDataSource, strText);
	pos.y++;
	SetCellText(pos, 0, strText);

	//IOPV
	dValue = CHSCalculate::GetIOPV(&m_stkInfo, pRealTimeExt, strText);
	pos.y++;
	SetCellText(pos, 0, strText);

	//内盘
	dValue = CHSCalculate::GetNeipan(&m_stkInfo, pOther, strText);
	pos.y++;
	SetCellText(pos, 0, strText);

	//跌停
	dValue = m_stkInfo.m_fDownPrice;
	CGeneralHelper::LongToString(dValue, 0, nUnit, nDecimal, strText, LTOA_MASK_HORLINE_IFZERO);
	pos.y++;
	SetCellText(pos, 0, strText);

	//市净
	dValue = CHSCalculate::GetShijing(pRealTime, pFinanceData, strText);
	pos.y++;
	SetCellText(pos, 0, strText);

	//总股
	dValue = CHSCalculate::GetZonggu(pFinanceData, strText);
	pos.y++;
	SetCellText(pos, 0, strText);

	//总值
	dValue = CHSCalculate::GetZongzhi(pRealTime, pFinanceData, strText);
	pos.y++;
	SetCellText(pos, 0, strText);

	if (pFinanceData)
	{
		delete pFinanceData;
		pFinanceData = NULL;
	}

	return TRUE;
}

BOOL CDrawRStockForm::UpdateETFData(LevelRealTime* pLevelData, LevelStockOtherData* pOtherData)          // 更新EFT数据
{
	if (pLevelData == NULL || pOtherData == NULL)
		return FALSE;

	int nHand = m_stkInfo.m_nHandNum; // EFT 1手 = 100一份
	//////////////////////////////////////////////////////////////////////////
	// 得到财务数据
	CArray<CodeInfo> ayCode;
	CArray<CurrentFinanceData> ayFinanceData;
	ayCode.Add(m_stkInfo.m_ciStockCode);
	m_pDataSource->HSDataSourceEx_GetStocksFinanceData(ayCode, ayFinanceData);
	CHSFinanceData* pFinanceData = NULL;
	if (ayFinanceData.GetCount() == 1)
		pFinanceData = new CHSFinanceData(&ayFinanceData.GetAt(0));
	//

	BOOL bIsStop = TRUE;//是否停牌
	if (pLevelData->m_lOpen > 0)
		bIsStop = FALSE;

	CString str;
	char    strText[64];
	CPoint  pos(0,0);
	WORD    lClr;

	long    nClose   = m_stkInfo.m_lPrevClose;
	int     nUnit    = m_stkInfo.m_nPriceUnit;
	int     nDecimal = m_stkInfo.m_nDecimal;

	double dValue = 0.0;
	//从第2栏开始
	//委比＝(委买手数－委卖手数)/(委买手数＋委卖手数)×100％
	pos.x = 1;
	pos.y = 0; 
	dValue = CHSCalculate::GetWeiBi(pLevelData, strText);
	lClr = GetTextColor(dValue, 0);
	SetCellText(pos, 0, strText, lClr);

	//成交
	dValue = pLevelData->m_lNewPrice;
	m_nNewPrice = pLevelData->m_lNewPrice;
	CGeneralHelper::LongToString(dValue, nClose, nUnit, nDecimal, strText, LTOA_MASK_HORLINE_IFZERO);
	pos.y++;
	lClr = GetTextColor(dValue, nClose);
	SetCellText(pos, 0, strText, lClr);

	//涨跌
	dValue = CHSCalculate::GetZhangdie(bIsStop, &m_stkInfo, pLevelData, strText);
	pos.y++;
	lClr = GetTextColor(dValue, 0);
	SetCellText(pos, 0, strText, lClr);

	//幅度(涨幅=(现价-昨收)/昨收)
	dValue = CHSCalculate::GetFudu(bIsStop, &m_stkInfo, pLevelData, strText);
	pos.y++;
	lClr = GetTextColor(dValue, 0);
	SetCellText(pos, 0, strText, lClr);

	//总手
	dValue = CHSCalculate::GetZongshou(&m_stkInfo, pLevelData, strText);
	pos.y++;
	SetCellText(pos, 0, strText);

	//现手
	dValue = CHSCalculate::GetXianshou(&m_stkInfo, pOtherData, strText);
	pos.y++;
	SetCellText(pos, 0, strText);

	//总额
	dValue = pLevelData->m_fAvgPrice;
	CGeneralHelper::LongToString(dValue, 0, 10000, 2, strText, LTOA_MASK_HORLINE_IFZERO);
	pos.y++;
	SetCellText(pos, 0, strText);

	//外盘
	dValue = CHSCalculate::GetWaipan(&m_stkInfo, pOtherData, strText);
	pos.y++;
	SetCellText(pos, 0, strText);

	//总笔
	dValue = pLevelData->m_lTickCount;
	pos.y++;
	CGeneralHelper::LongToString(dValue, 0, 1, 0, strText, LTOA_MASK_HORLINE_IFZERO);
	SetCellText(pos, 0, strText);

	//涨停
	dValue = m_stkInfo.m_fUpPrice;
	CGeneralHelper::LongToString(dValue, 0, nUnit, nDecimal, strText, LTOA_MASK_HORLINE_IFZERO);
	pos.y++;
	SetCellText(pos, 0, strText);

	//市盈 
	dValue = CHSCalculate::GetShiying(&m_stkInfo, pLevelData, pFinanceData, strText);
	pos.y++;
	SetCellText(pos, 0, strText);

	//流通
	dValue = CHSCalculate::Getliutong(pFinanceData, strText);
	pos.y++;
	SetCellText(pos, 0, strText);

	//市值
	dValue = CHSCalculate::GetShizhi(pLevelData, pFinanceData, strText);
	pos.y++;
	SetCellText(pos, 0, strText);

	/////////////////////////////////  第四列 /////////////////////////////////////
	//委差=(委买手数－委卖手数)
	dValue = CHSCalculate::GetWeicha(pLevelData, nHand, strText);
	pos.x = 3;
	pos.y = 0;
	lClr = GetTextColor(dValue, 0);
	SetCellText(pos, 0, strText, lClr);

	//均价 ＝（E分时成交的量×成交价）/总成交股数
	dValue = CHSCalculate::GetJunjia(&m_stkInfo, pLevelData, strText);
	pos.y++;
	lClr = GetTextColor(dValue, nClose);
	SetCellText(pos, 0, strText, lClr);

	//开盘
	dValue = pLevelData->m_lOpen;
	CGeneralHelper::LongToString(dValue, nClose, nUnit, nDecimal, strText, LTOA_MASK_HORLINE_IFZERO);
	pos.y++;
	lClr = GetTextColor(dValue, nClose);
	SetCellText(pos, 0, strText, lClr);

	//最高
	dValue = pLevelData->m_lMaxPrice;
	CGeneralHelper::LongToString(dValue, nClose, nUnit, nDecimal, strText, LTOA_MASK_HORLINE_IFZERO);
	pos.y++;
	lClr = GetTextColor(dValue, nClose);
	SetCellText(pos, 0, strText, lClr);

	//最低
	dValue = pLevelData->m_lMinPrice;
	CGeneralHelper::LongToString(dValue, nClose, nUnit, nDecimal, strText, LTOA_MASK_HORLINE_IFZERO);
	pos.y++;
	lClr = GetTextColor(dValue, nClose);
	SetCellText(pos, 0, strText, lClr);

	//量比
	dValue = CHSCalculate::GetLiangbi(&m_stkInfo, pLevelData, pOtherData, m_pDataSource, strText);
	pos.y++;
	SetCellText(pos, 0, strText);

	//IOPV
	dValue = pLevelData->m_IPOV;
	CGeneralHelper::LongToString(dValue, nClose, nUnit, nDecimal, strText, LTOA_MASK_HORLINE_IFZERO);
	pos.y++;
	SetCellText(pos, 0, strText);

	//内盘
	dValue = CHSCalculate::GetNeipan(&m_stkInfo, pOtherData, strText);
	pos.y++;
	SetCellText(pos, 0, strText);

	//每笔
	dValue = CHSCalculate::GetMeiBi(pLevelData, strText);
	pos.y++;
	SetCellText(pos, 0, strText);

	//跌停
	dValue = m_stkInfo.m_fDownPrice;
	CGeneralHelper::LongToString(dValue, 0, nUnit, nDecimal, strText, LTOA_MASK_HORLINE_IFZERO);
	pos.y++;
	SetCellText(pos, 0, strText);

	//市净
	dValue = CHSCalculate::GetShijing(pLevelData, pFinanceData, strText);
	pos.y++;
	SetCellText(pos, 0, strText);

	//总股
	dValue = CHSCalculate::GetZonggu(pFinanceData, strText);
	pos.y++;
	SetCellText(pos, 0, strText);

	//总值
	dValue = CHSCalculate::GetZongzhi(pLevelData, pFinanceData, strText);
	pos.y++;
	SetCellText(pos, 0, strText);

	if (pFinanceData)
	{
		delete pFinanceData;
		pFinanceData = NULL;
	}

	return TRUE;
}

BOOL CDrawRStockForm::UpdateIndexData(HSIndexRealTime* pIndex)   
{
	if ( pIndex == NULL || ::IsBadReadPtr(pIndex, 1))
	{
		return FALSE;
	}

	long    nClose   = m_stkInfo.m_lPrevClose;
	int     nUnit    = m_stkInfo.m_nPriceUnit;
	int     nDecimal = m_stkInfo.m_nDecimal;
	int     nHand    = m_stkInfo.m_nHandNum; 

	CString str;
	char    strText[64];
	double dValue(0);
	CPoint pos(0,0);
	WORD    lClr;
	////////////////////////// 第二列 ////////////////////////////////////////
	pos.x = 1;
	pos.y = 0;
	
	dValue = pIndex->m_lNewPrice;
	lClr = GetTextColor(dValue, nClose);
	CGeneralHelper::LongToString(dValue, nClose, nUnit, nDecimal, strText, LTOA_MASK_HORLINE_IFZERO);
	SetCellText(pos, 0, strText, lClr);                // 成交

	pos.y = 1;
	dValue = pIndex->m_lNewPrice;
	if (dValue <= 0)
		CGeneralHelper::LongToString((long)0, nClose, nUnit, nDecimal, strText, LTOA_MASK_HORLINE_IFZERO);
	else
		CGeneralHelper::LongToString((dValue - nClose), 0, nUnit, nDecimal, strText, LTOA_MASK_HORLINE_IFZERO);
	lClr = GetTextColor(dValue - nClose, 0);
	SetCellText(pos, 0, strText, lClr);                // 涨跌

	pos.y = 2;
	if (nClose == 0)
		CGeneralHelper::LongToString((long)0, 0, 0, 0, strText, LTOA_MASK_SHOW_PERSENT);
	else
		CGeneralHelper::LongToString((dValue-nClose)*100, 0, nClose, nDecimal, strText, LTOA_MASK_SHOW_PERSENT);
	SetCellText(pos, 0, strText, lClr);                // 涨幅

	pos.y = 3;
	CHSCalculate::GetZongshou(pIndex, nHand, LTOA_MASK_SHOWEX_UNITAUTO, strText);
	SetCellText(pos, 0, strText);                      // 总手

	pos.y = 4;
	dValue = pIndex->m_fAvgPrice * 100;
	CGeneralHelper::LongToString(dValue, 0, 1, 2, strText, LTOA_MASK_SHOWEX_UNITAUTO | LTOA_MASK_HORLINE_IFZERO);
	SetCellText(pos, 0, strText);                      // 金额

	pos.y = 5;
	dValue = CHSCalculate::GetWeiBi(pIndex, strText);
	lClr = GetTextColor(dValue, 0);
	SetCellText(pos, 0, strText, lClr);                   // 委比

	////////////////////////// 第四列 ////////////////////////////////////////
	pos.x = 3;
	pos.y = 0;

	dValue = nClose;
	CGeneralHelper::LongToString(dValue, nClose, nUnit, nDecimal, strText, LTOA_MASK_HORLINE_IFZERO);
	lClr = GetTextColor(dValue, nClose);
	SetCellText(pos, 0, strText, lClr);                       // 昨收

	pos.y = 1;
	dValue = pIndex->m_lOpen;
	CGeneralHelper::LongToString(dValue, nClose, nUnit, nDecimal, strText, LTOA_MASK_HORLINE_IFZERO);
	lClr = GetTextColor(dValue, nClose);                // 开盘
	SetCellText(pos, 0, strText, lClr);

	pos.y = 2;
	dValue = pIndex->m_lMaxPrice;                   // 最高
	CGeneralHelper::LongToString(dValue, nClose, nUnit, nDecimal, strText, LTOA_MASK_HORLINE_IFZERO);
	lClr = GetTextColor(dValue, nClose);
	SetCellText(pos, 0, strText, lClr);

	pos.y = 3;
	dValue = pIndex->m_lMinPrice;                   // 最低
	CGeneralHelper::LongToString(dValue, nClose, nUnit, nDecimal, strText, LTOA_MASK_HORLINE_IFZERO);
	lClr = GetTextColor(dValue, nClose);
	SetCellText(pos, 0, strText, lClr);

	pos.y = 4;
	if (nClose == 0)
		dValue = 0.0;
	else
		dValue = (pIndex->m_lMaxPrice - pIndex->m_lMinPrice) * 100;
	CGeneralHelper::LongToString(dValue, 0, nClose, 2, strText, LTOA_MASK_SHOW_PERSENT | LTOA_MASK_HORLINE_IFZERO);
	lClr = GetTextColor(dValue, 0);
	SetCellText(pos, 0, strText, lClr);                        // 振幅

	pos.y = 5;
	dValue = CHSCalculate::GetWeicha(pIndex, nHand, strText);
	lClr = GetTextColor(dValue, 0);
	SetCellText(pos, 0, strText, lClr);                        // 委差

	pos.y = 6;
	dValue = pIndex->m_lBuyCount / nHand;
	CGeneralHelper::LongToString(dValue, 0, 1, 0, strText, LTOA_MASK_HORLINE_IFZERO);
	SetCellText(pos, 0, strText);                        // 委买手数

	pos.y = 7;
	dValue = pIndex->m_lSellCount / nHand;
	CGeneralHelper::LongToString(dValue, 0, 1, 0, strText, LTOA_MASK_HORLINE_IFZERO);
	SetCellText(pos, 0, strText);                        // 委卖手数

	pos.y = 8;
	dValue = pIndex->m_nRiseCount;
	CGeneralHelper::LongToString(dValue, 0, 1, 0, strText, LTOA_MASK_HORLINE_IFZERO);
	SetCellText(pos, 0, strText);                        // 上涨家数

	pos.y = 9;
	long nTotalStock = pIndex->m_nTotalStock2;
	if(nTotalStock == 0)
		nTotalStock = pIndex->m_nTotalStock1;
	dValue = nTotalStock - pIndex->m_nFallCount - pIndex->m_nRiseCount; 
	dValue = dValue<0?0:dValue;
	CGeneralHelper::LongToString(dValue, 0, 1, 0, strText, LTOA_MASK_HORLINE_IFZERO);
	SetCellText(pos, 0, strText);                        // 平盘家数

	pos.y = 10;
	dValue = pIndex->m_nFallCount;                        
	CGeneralHelper::LongToString(dValue, 0, 1, 0, strText, LTOA_MASK_HORLINE_IFZERO);
	SetCellText(pos, 0, strText);                        // 下跌家数
	
	return TRUE;
}

void  CDrawRStockForm::InitPriceMap(CMap<long,long,long,long> &lMap, long lPrice[5])
{
	long lKey = 0;
	long lValue = 0;
	// 去除map中过时的价格
	POSITION pos = lMap.GetStartPosition();
	while(pos)
	{
		lMap.GetNextAssoc(pos, lKey, lValue);

		BOOL bExist = FALSE;
		for (int i = 0; i < 5; i++)
		{
			if (lKey == lPrice[i])
			{
				bExist = TRUE;
				break;
			}
		}

		if (bExist == FALSE)
			lMap.RemoveKey(lKey);
	}
}

WORD CDrawRStockForm::GetTextColor( double newValue, double preValue )
{
	double diff = newValue - preValue;
	if (diff > 0)
	{
		return TEXT_COLOR_UP;   // 涨的颜色
	}
	else if (diff < 0)
	{
		return TEXT_COLOR_DOWN; // 跌的颜色
	}
	else
	{
		return TEXT_COLOR_FLAT; // 平盘颜色
	}
}

BOOL CDrawRStockForm::GetQuickTradeDirection(CPoint pos, double dPrice)
{
	double dNewPrice = (double)m_nNewPrice / m_stkInfo.m_nPriceUnit;
	if (!m_bLevel)
	{// level1 面板
		if (pos.x == 1)
		{
			if (pos.y >= 1 && pos.y <= 5)
			// 五档卖盘
				return 1;
			else if (pos.y >= 6 && pos.y <= 10)
			// 五档买盘
				return 0;
		}
		// 除去买卖盘 闪电下单的方向与最新成交价比
		if (dPrice >= dNewPrice)
		// 大于成交价 买入	
			return 1;
		else 
			return 0;
	}
	else
	{// level2 面板
		if (dPrice >= dNewPrice)
			// 大于成交价 买入	
			return 1;
		else 
			return 0;
	}
}

WORD CDrawRStockForm::GetSellChange(long lSellPrice, long lNowSellCount, char* strText)
{
	double dValue = 0.0;
	if (lSellPrice)
	{// 过滤价格为0
		long lPreSellCount = -1;
		if (m_sellMap.Lookup(lSellPrice, lPreSellCount))
			dValue = lNowSellCount - lPreSellCount;
		m_sellMap.SetAt(lSellPrice, lNowSellCount);
	}

	CGeneralHelper::LongToString(dValue, 0, 1, 0, strText, LTOA_MASK_NOTHING_IFZERO);
	return GetTextColor(dValue, 0);
}

WORD CDrawRStockForm::GetBuyChange(long lBuyPrice, long lNowBuyCount, char* strText)
{
	double dValue = 0.0;
	if (lBuyPrice)
	{// 过滤价格为0
		long lPreBuyCount = -1;
		if (m_buyMap.Lookup(lBuyPrice, lPreBuyCount))
			dValue = lNowBuyCount - lPreBuyCount;
		m_buyMap.SetAt(lBuyPrice, lNowBuyCount);
	}

	CGeneralHelper::LongToString(dValue, 0, 1, 0, strText, LTOA_MASK_NOTHING_IFZERO);
	return GetTextColor(dValue, 0);
}

BOOL CDrawRStockForm::MouseDBDown( UINT nFlags, CPoint point, hsRefMouseMsg* pData /*= NULL*/ )
{
	int nActiveCellID = GetActiveCellID();
	CDrawCell* pCell = GetCell( nActiveCellID );
	if ( pCell && GetSignal() == NEED_TEXTHIGHLIGHT)
	{
		CString str("");
		GetCellText(GetActiveCellID(), 0, str);
		double dPrice = strtod(str, NULL);
		if (dPrice - 0.0001 > 0)
		{// 价格不为0 
			CPoint pos = IndexToPoint(nActiveCellID);
			BOOL bBuyDirection = GetQuickTradeDirection(pos, dPrice);
			
			CString strFormat("%");
			strFormat.AppendFormat(".%df", m_stkInfo.m_nDecimal);
			if (bBuyDirection)
			{// 闪电买入
				//////////////////////////////////////////////////////////////////////////
				// 添加参数
				CDrawBaseParam param;
				str.Format(strFormat, dPrice);
				param.AddParam(PARAM_QUICKTRADE_PRICE, str);
				param.AddParam(PARAM_QUICKTRADE_CODE, m_stkInfo.GetCode());
				m_pRegion->SendLinkageMsg(PUBLIC_MESSAGE_QUICKTRADE, &param, PN_QUICKTRADE_BUY);
			}
			else
			{// 闪电卖出
				//////////////////////////////////////////////////////////////////////////
				// 添加参数
				CDrawBaseParam param;
				str.Format(strFormat, dPrice);
				param.AddParam(PARAM_QUICKTRADE_PRICE, str);
				param.AddParam(PARAM_QUICKTRADE_CODE, m_stkInfo.GetCode());
				m_pRegion->SendLinkageMsg(PUBLIC_MESSAGE_QUICKTRADE, &param, PN_QUICKTRADE_SELL);
			}
		}
	}
	return TRUE;
}

BOOL CDrawRStockForm::RButtonDown( UINT nFlags,CPoint point, HWND hWndParent, hsRefMouseMsg* pData /*= NULL*/ )
{
	int nActiveCellID = GetActiveCellID();
	CDrawCell* pCell = GetCell( nActiveCellID );
	if ( pCell && GetSignal() == NEED_TEXTHIGHLIGHT)
	{
		CString str("");
		GetCellText(GetActiveCellID(), 0, str);
		double dPrice = strtod(str, NULL);
		if (dPrice - 0.0001 > 0)
		{// 价格不为0
				
			ResetCell(pCell);
			//////////////////////////////////////////////////////////////////////////
			// 读取资源字符串
			CAtlBaseModule _Module;
			//CComModule _Module;
			HINSTANCE instance = _Module.GetResourceInstance();
			char* buf = new char[1000];
			int   nBufLenth = 1000;
			//

			CMenu popMenu;
			popMenu.CreatePopupMenu();
			::LoadString(instance, IDS_STRING_QUICKBUY, buf, nBufLenth);  // 闪电买入
			str = buf;
			popMenu.AppendMenu(0, ID_QUICKTRADE_BUY, str);

			::LoadString(instance, IDS_STRING_QUICKSELL, buf, nBufLenth); // 闪电卖出
			str = buf;
			popMenu.AppendMenu(0, ID_QUICKTRADE_SELL, str);

			int nMenuID(-1);
			if (popMenu.GetMenuItemCount() > 0)
			{
				::ClientToScreen(hWndParent, &point);
				CWnd* pWnd = CWnd::FromHandle(hWndParent);
				nMenuID = popMenu.TrackPopupMenu(TPM_VERPOSANIMATION | TPM_LEFTALIGN | TPM_RIGHTBUTTON | TPM_RETURNCMD,point.x,point.y, pWnd);
			}

			if (nMenuID > 0)
			{
				BOOL bBuyDirection = (nMenuID == ID_QUICKTRADE_BUY)?1:0;
				
				CString strFormat("%");
				strFormat.AppendFormat(".%df", m_stkInfo.m_nDecimal);
				if (bBuyDirection)
				{// 闪电买入
					//////////////////////////////////////////////////////////////////////////
					// 添加参数
					CDrawBaseParam param;
					str.Format(strFormat, dPrice);
					param.AddParam(PARAM_QUICKTRADE_PRICE, str);
					param.AddParam(PARAM_QUICKTRADE_CODE, m_stkInfo.GetCode());
					m_pRegion->SendLinkageMsg(PUBLIC_MESSAGE_QUICKTRADE, &param, PN_QUICKTRADE_BUY);
				}
				else
				{// 闪电卖出
					//////////////////////////////////////////////////////////////////////////
					// 添加参数
					CDrawBaseParam param;
					str.Format(strFormat, dPrice);
					param.AddParam(PARAM_QUICKTRADE_PRICE, str);
					param.AddParam(PARAM_QUICKTRADE_CODE, m_stkInfo.GetCode());
					m_pRegion->SendLinkageMsg(PUBLIC_MESSAGE_QUICKTRADE, &param, PN_QUICKTRADE_SELL);
				}
			}
			popMenu.DestroyMenu();
			delete[] buf;	
		}
	}

	return TRUE;
}