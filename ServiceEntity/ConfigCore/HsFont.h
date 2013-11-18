/*******************************************************************************
* Copyright (c)2010, 杭州恒生电子股份有限公司
* All rights reserved.
*
* 文件名称：HsFont.h
* 摘    要：字体相关配置实现类
*
* 当前版本：
* 作    者：王超
* 完成日期：2010-12-17
* 备	 注：
*******************************************************************************/
#pragma once

#include "ConfigInterface.h"
#include "ConfigBase.h"
#include "ConfigStruct.h"

class CHsFont :
 	public IHsFont
{
public:
	CHsFont(void);
	~CHsFont(void);

	void Init();
	BOOL IsInit() {return m_bInited;}

	BOOL Release(BOOL bSaveToFile = TRUE);
	BOOL SaveToFile();

private:

	BOOL           SetFontNode(short nID, FontProperty* pColor);   //根据字体ID 更改字体
	TiXmlNode*     GetFontNode(short nID, DocType nType = Default); //根据字体ID、文档类型 返回字体节点
	FontProperty*  GetFontProperty(short nID); 	//根据字体ID 返回颜色属性指针

public:
	//////////////////////////////////////////////////////////////////////////
	// 读取
	virtual CFont* GetTipTitleFont();       //获取TIP提示标题字体
	virtual CFont* GetTipTxtFont();         //获取TIP提示文本字体

	virtual CFont* GetQuoteTabFont();		//获取行情报价标签页字体
	virtual CFont* GetQuoteGuideTabFont();  //获取行情报价向导标签页字体
	virtual CFont* GetStatusFontProt();	    //获取状态栏指数字体
	virtual CFont* GetScrollFontProt();     //获取跑马灯数据字体

	virtual CFont* GetQuoteRefreshFont();	// 获取行情报价更新字体
	virtual CFont* GetQuoteColHeadFont();	// 获取行情报价表头字体
	virtual CFont* GetQuoteStringFont();	// 获取行情报价文字字体(中文字)
	virtual CFont* GetQuoteCodeFont();	    // 获取行情报价股票代码字体
	virtual CFont* GetQuoteDynamicFont();	// 获取行情报价动态文本字体
	virtual CFont* GetQuoteFixedFont();	    // 获取行情报价固定文本字体（序）
	
	virtual CFont* GetRTableFixedFont();        // 获取右小图字段名称字体
	virtual CFont* GetRTableDynamicalFont();    // 获取右小图动态字段字体
	virtual CFont* GetRTableTitleCodeFont();    // 获取右小图标题股票代码字体
	virtual CFont* GetRTableTitleNameFont();    // 获取右小图标题股票名称字体
	virtual CFont* GetRTableFinanceFont();      // 获取右小图配财值页文本字体
	virtual CFont* GetRTableTimeFont();         // 获取右小图时间列字体
	virtual CFont* GetDiagramLayoutTabFont();   // 获取图层标签页字体
	virtual CFont* GetDiagramTableTitleFont();  // 获取表格标题图元文本字体

	virtual CFont* GetZiXunFont();              // 获取资讯字体
	virtual CFont* GetDiagramTradeFont();       // 获取交易图元字体
	//////////////////////////////////////////////////////////////////////////
	// 写入
	virtual BOOL SetScrollFontProt(CFont* pFontScroll); // 设置跑马灯数据字体


private:
    CConfigBase*    m_pConfigBase;     // 文件操作类指针
	CMapWordToPtr   m_CurrFontMap;     // 当前使用的字体映射表	

	BOOL     m_bInited;          //是否初始化

	CFont m_QuoteDefaultFont;
	CFont m_TradeDefaultFont;
};
