/*******************************************************************************
* Copyright (c)2010, 杭州恒生电子股份有限公司
* All rights reserved.
*
* 文件名称：HsScrollNews.h
* 摘    要：跑马灯配置实现类
*
* 当前版本：
* 作    者：王超
* 完成日期：2010-12-20
* 备	 注：
*******************************************************************************/

#pragma once

#include "ConfigInterface.h"
#include "ConfigBase.h"

class CHsScrollNews : public IHsScrollNews
{
public:
	CHsScrollNews(void);
	~CHsScrollNews(void);

	void Init();
	BOOL IsInit() {return m_bInited;}

	BOOL Release(BOOL bSaveToFile = TRUE);
	BOOL SaveToFile();
private:
	int  GetStockInfo(TiXmlNode *pNode ,CStringArray &ayStockList);
	BOOL SetStockList(const CString &strPath, const CString &strParentPath, const CStringArray &ayStockList);
//接口实现部分
public:
	virtual int		IsDispThirdIndex();		    // 第三列是否显示
    virtual int		IsDispFourthIndex();		// 第四列是否显示 
	virtual int		IsScrollFixed();			// 跑马灯是否固定在状态栏上
	virtual int		IsScrollMultiLine();		// 跑马灯是否多行显示  
 	virtual int		GetShowScrollAlone();	    // 跑马灯显示方式 0：跑马灯和状态条在同一行；1：跑马灯单独一行在状态条上面；2：状态条单独一行，跑马灯在最下面一行

	virtual CString GetStatusName();          // 获取跑马灯标题名称
	virtual int		GetStatusScrollSpeed();	  // 滚动间隔 秒为单位
	virtual int     GetStockNewName(CStringArray &ayStockList); // 获取股票新名称            
	virtual int     GetUpDownRate();          // 是否显示涨跌幅
	virtual int		GetTotal();               // 是否显示总额
	virtual int		GetShowNewPirce();        // 是否显示最新成交价
	virtual int		GetUpDown();              // 是否显示涨跌

	virtual int		GetScrollSpeed();		  // 行情滚动速度 毫秒为单位
	virtual int		GetHorizonScrollWay();	  // 行情水平滚动方向 3: 向左; 4：向右
	virtual int		GetVerticalScrollWay();   // 行情垂直滚动方向 1: 向上; 2：向下
	virtual int		GetClickWay();			  // 显示行情方式 0：单击; 1：双击 
	virtual void	GetScrollText(CStringArray &ayScrollText);  // 获取跑马灯滚动咨询

	//获取每列指数栏对应的代码列表，一条记录对应一个CStringArray，每个CStringArray里面先后包含证券代码和市场类别
	//调用此函数对每个CStringArray需进行内存释放
	virtual int		GetFirstListStockInfo(CStringArray &ayStockList);
	virtual int		GetSecondListStockInfo(CStringArray &ayStockList);
	virtual int		GetThirdListStockInfo(CStringArray &ayStockList);
	virtual int		GetFourthListStockInfo(CStringArray &ayStockList);

	virtual BOOL	SetDispThirdIndex(int nValue);    // 保存第三列显示开关 
	virtual BOOL	SetDispFourthIndex(int nValue);   // 保存第四列显示开关
	virtual BOOL	SetScrollFixed(int nValue);       // 保存跑马灯固定开关
	virtual BOOL	SetScrollMultiLine(int nValue);   // 保存跑马灯多行开关

    virtual BOOL	SetStatusScrollSpeed(int nValue); // 保存指数显示速度 秒为单位
	virtual BOOL	SetScrollSpeed(int nValue);       // 保存行情滚动速度
	virtual BOOL	SetHorizonScrollWay(int nValue);  // 保存行情水平滚动方向
	virtual BOOL	SetVerticalScrollWay(int nValue); // 保存行情垂直滚动方向
	virtual BOOL	SetClickWay(int nValue);          // 保存行情显示方式

	virtual BOOL	SetFirstListStockInfo(const CStringArray &ayStockList);
	virtual BOOL	SetSecondListStockInfo(const CStringArray &ayStockList);
	virtual BOOL	SetThirdListStockInfo(const CStringArray &ayStockList);
	virtual BOOL	SetFourthListStockInfo(const CStringArray &ayStockList);
private:
	CConfigBase*   m_pConfigBase;    // 文件操作类指针
	CMapWordToPtr  m_CurrColorMap;   // 当前使用的颜色映射表	

	BOOL           m_bInited;        // 是否初始化
	CString        m_strRootPath;    // 逻辑根节点路径 
};
