/*******************************************************************************
* Copyright (c)2010, 杭州恒生电子股份有限公司
* All rights reserved.
*
* 文件名称：HsColor.h
* 摘    要：颜色配置实现类
*
* 当前版本：
* 作    者：王超
* 完成日期：2010-12-10
* 备	 注：
*******************************************************************************/

#pragma  once 
#include "ConfigBase.h"
#include "ConfigDefined.h"
#include "ConfigStruct.h"
#include "ConfigInterface.h"

enum ColorType{ClassicalColor };
enum TYPE {COLWIDTH = 0x00, COLOR = 0x01,FONT = 0x02,COLGROUP = 0x04,SETTING = 0x08,DYNNEWS = 0x10, CHUQUAN = 0x20, MOVECOST = 0x40, PROGRAMSET = 0x80};

class CHsColor :public IHsColor
{
public:
	CHsColor();
	~CHsColor();

	void Init();
	BOOL IsInit() {return m_bInited;}

public:
	BOOL Release(BOOL bSaveToFile = TRUE);
	BOOL SaveToFile();
	
private:
	COLORREF StringToColor(const char *pColor);
	CString  ColorToString(COLORREF lColor);
	
	BOOL           SetColorNode(short nID, ColorProperty* pColor); //根据颜色ID 更改颜色
	TiXmlNode*     GetColorNode(short nID, DocType nType = Default); //根据颜色ID、文档类型 返回颜色节点
	ColorProperty* GetColorProperty(short nID); 	//根据颜色ID 返回颜色属性指针
	TiXmlElement*  GetGroupElementFromId( WORD nColorId ); //根据ID 获取Group节点

public:
	virtual const COLORREF GetColumnColor(UINT nColorId); 
	virtual const COLORREF GetBlackgroundColor( );  
	virtual const COLORREF GetFixedCodeColor( );   // 获取代码标题文本
	virtual const COLORREF GetFixedNameColor( );   // 报价证券名称
	virtual const COLORREF GetFixedNameSelfColor();// 报价证券自选股名称
	virtual const COLORREF GetUpColor( );          // 获取上涨颜色
	virtual const COLORREF GetDownColor( );        // 获取下跌颜色
	virtual const COLORREF GetEqualColor( );       // 获取平盘颜色
	virtual COLORREF GetTotalColor();              // 获取总手颜色
	virtual COLORREF GetNowColor();                // 获取现手颜色
	virtual COLORREF GetDoneColor();               // 获取成交颜色
	virtual COLORREF GetChangeColor();             // 获取换手颜色
	virtual COLORREF GetQuantitativeRateColor();   // 获取量比颜色 
	virtual COLORREF GetOrderBuyVolColor();        // 获取委买量颜色
	virtual COLORREF GetOrderSellVolColor();       // 获取委卖量颜色

	virtual const COLORREF GetStatusTextColor( );  // 获取状态栏文本颜色
	virtual const COLORREF GetScrollTestColor( );  // 获取跑马灯文本颜色
	virtual const COLORREF GetScrollBKColor( );    // 获取跑马灯背景颜色
	virtual void  SetScrollTextColor(const COLORREF clrText); // 设置跑马灯文本颜色
	virtual void  SetScrollBKColor(const COLORREF clrBk);     // 设置跑马灯背景颜色

	virtual const COLORREF GetTipTitleBk( );	// tip提示标题背景
	virtual const COLORREF GetTipTitleFg( );	// tip提示标题前景
	virtual const COLORREF GetTipTextFg( );		// tip提示文本背景
	virtual const COLORREF GetTipTextBk( );		// tip提示文本前景

	virtual COLORREF GetQuoteFixedRowColBKColor( );	  // 获取行情报价固定行固定列背景色
	virtual COLORREF GetQuoteFixedRowBKColor( );	  // 获取行情报价固定行背景色
	virtual COLORREF GetQuoteFixedColBKColor( );	  // 获取行情报价固定列背景色
	virtual COLORREF GetQuoteHighLightFgColor( );	  // 获取行情报价高亮前景色
	virtual COLORREF GetQuoteHighLightBKColor( );	  // 获取行情报价高亮背景色
	virtual COLORREF GetQuotePriceChangeBKColor( );	  // 获取行情报价更新.高亮文本背景

	virtual COLORREF GetQuoteTabBKColor( );				// 获取行情报价标签页背景颜色
	virtual COLORREF GetQuoteTabTextColor( );			// 获取行情报价标签页字体颜色
	virtual COLORREF GetQuoteGuideTabBKColor( );		// 获取行情报价向导标签页背景颜色
	virtual COLORREF GetQuoteGuideTabTextColor( );		// 获取行情报价向导标签页字体颜色
	virtual COLORREF GetQuoteActiveTabBKColor( );		// 获取行情报价激活标签页背景颜色
	virtual COLORREF GetQuoteActiveTabTextColor( );		// 获取行情报价激活标签页字体颜色
	virtual COLORREF GetQuoteTabButtonBKColor( );		// 获取行情报价标签页按钮背景颜色

	virtual const ColorProperty* GetQuoteTabFrameColorProp( );	    // 获取行情报价标签页边框线形与颜色
	virtual const ColorProperty* GetQuoteGuideTabFrameColorProp( ); // 获取行情报价向导标签页边框线形与颜色

	virtual COLORREF GetRTitleCode();          // 右小图标题股票代码颜色
	virtual COLORREF GetRTitleCodeName();      // 右小图标题股票名称颜色
	virtual COLORREF GetRTableFixedName();     // 右小图固定文本颜色
	virtual COLORREF GetRTableTime();          // 右小图时间颜色
	virtual COLORREF GetRTableVer();           // 右小图竖线颜色
	virtual COLORREF GetRTableHor();           // 右小图横线颜色
	virtual COLORREF GetTableTitle();          // 表格标题图元字体颜色
	virtual COLORREF GetTableTitleBK();        // 表格标题图元背景颜色
	virtual COLORREF GetTradeTableFixedName(); // 交易图元名称列

	virtual COLORREF GetZiXunBK();             // 资讯背景色
	virtual COLORREF GetZiXunFG();             // 资讯文本前景色

	virtual COLORREF GetDiagramLayoutBKColor();              // 图层标签页背景
	virtual COLORREF GetDiagramLayoutActiveFillColor();      // 图层标签页激活
	virtual COLORREF GetDiagramLayoutFillColor();            // 图层标签页常规
	virtual COLORREF GetDiagramLayoutForeColor();            // 图层标签页前景
	virtual COLORREF GetDiagramLayoutBorderColor();          // 图层标签页边框

	virtual COLORREF GetTechLineColor(int nID);         //获取技术线颜色

private:
    CConfigBase*   m_pConfigBase;    // 文件操作类指针
	CMapWordToPtr  m_CurrColorMap;   // 当前使用的颜色映射表	

	BOOL     m_bInited;          //是否初始化
};