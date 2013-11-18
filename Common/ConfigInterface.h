/*******************************************************************************
* Copyright (c)2010, 杭州恒生电子股份有限公司
* All rights reserved.
*
* 文件名称：ConfigInterface.h
* 摘    要：配置接口定义头文件
*
* 当前版本：
* 作    者：王超
* 完成日期：2010-12-10
* 备	 注：
*******************************************************************************/
#pragma once
#include "ConfigStruct.h"
#include "NetEngineInterface.h"


#define OBJ_COLORCFG	   _T("ConfigCore.CHsColor")		    // 颜色配置
#define OBJ_FONTCFG        _T("ConfigCore.CHsFont")             // 字体配置
#define OBJ_QUOTESYS       _T("ConfigCore.CHsQuoteSys")         // 行情系统配置
#define OBJ_GRAPH          _T("ConfigCore.CHsGraph")            // 图形相关配置
#define OBJ_MIS            _T("ConfigCore.CHsMiscellany")       // 杂项配置
#define OBJ_SCROLLNEW      _T("ConfigCore.CHsScrollNews")       // 跑马灯配置
#define OBJ_COLUMNPRO	   _T("ConfigCore.CHsColumnProperty")	// 报价列信息配置
#define OBJ_TAB            _T("ConfigCore.CHsTabCfg")           // 报价标签配置
#define OBJ_COMM           _T("ConfigCore.CHsCommuncation")     // 通讯配置
#define OBJ_USERDATA       _T("ConfigCore.CHsUserDataCfg")      // 运行中的用户数据读写配置
#define OBJ_BLOCKMANAGER   _T("ConfigCore.CBlockManager")		// 板块管理
#define OBJ_KEYBOADR       _T("ConfigCore.CHsKeyboardCfg")      // 热键配置
#define OBJ_INFO           _T("ConfigCore.CHsInfoCfg")          // 资讯配置
#define OBJ_KLINETAB       _T("ConfigCore.CHsKLineTab")         // K线tab
#define OBJ_KLINEPOSCTRL   _T("ConfigCore.CHsKLinePosCtrol")    // K线位置控制

interface INotify
{
// 	virtual void SendChangedNotify( /* 通知类型——变更的类型：颜色、字体等(枚举) */ ) = 0;
// 	virtual void SetChangedNotify(/* 回调函数指针——主框架通知函数 */) = 0;
};

interface ICfgBase
{
	virtual BOOL SaveToFile() = 0;                  //保存文件
};


///////////////////////////////////  HsColor ///////////////////////////////////
//颜色配置
interface IHsColor : public INotify, public ICfgBase
{ //参数名 

	////////////////////////////  读取部分  ///////////////////////////////////////////
	virtual const COLORREF GetColumnColor(UINT nColumnId)=0; 
	virtual const COLORREF GetBlackgroundColor()=0;  // 获取背景色
	virtual const COLORREF GetFixedCodeColor()=0;    // 获取代码标题文本色
	virtual const COLORREF GetFixedNameColor()=0;    // 报价证券名称色
	virtual const COLORREF GetFixedNameSelfColor()=0;// 报价证券自选股名称
	virtual const COLORREF GetUpColor()=0;           // 获取上涨颜色
	virtual const COLORREF GetDownColor()=0;         // 获取下跌颜色
	virtual const COLORREF GetEqualColor()=0;        // 获取平盘颜色
	virtual COLORREF GetTotalColor()=0;              // 获取总手颜色
	virtual COLORREF GetNowColor()=0;                // 获取先手颜色
	virtual COLORREF GetDoneColor()=0;               // 获取成交颜色
	virtual COLORREF GetChangeColor()=0;             // 获取换手颜色
	virtual COLORREF GetQuantitativeRateColor()=0;   // 获取量比颜色 
	virtual COLORREF GetOrderBuyVolColor()=0;        // 获取委买量颜色
	virtual COLORREF GetOrderSellVolColor()=0;       // 获取委卖量颜色
 
	virtual const COLORREF GetStatusTextColor()=0;   // 获取状态栏文本颜色
	virtual const COLORREF GetScrollTestColor()=0;   // 获取跑马灯文本颜色
	virtual const COLORREF GetScrollBKColor()=0;     // 获取跑马灯背景颜色

	virtual const COLORREF GetTipTitleBk()=0;		 // tip提示标题背景
	virtual const COLORREF GetTipTitleFg()=0;		 // tip提示标题前景
	virtual const COLORREF GetTipTextFg()=0;		 // tip提示文本背景
	virtual const COLORREF GetTipTextBk()=0;		 // tip提示文本前景

	virtual COLORREF GetQuoteFixedRowColBKColor()=0;			    // 获取行情报价固定行固定列背景色
	virtual COLORREF GetQuoteFixedRowBKColor()=0;					// 获取行情报价固定行背景色
	virtual COLORREF GetQuoteFixedColBKColor()=0;					// 获取行情报价固定列背景色
	virtual COLORREF GetQuoteHighLightFgColor()=0;					// 获取行情报价高亮前景色
	virtual COLORREF GetQuoteHighLightBKColor()=0;					// 获取行情报价高亮背景色
	virtual COLORREF GetQuotePriceChangeBKColor()=0;				// 获取行情报价更新.高亮文本背景
	virtual COLORREF GetQuoteTabBKColor()=0;							// 获取行情报价标签页背景颜色
	virtual COLORREF GetQuoteTabTextColor()=0;							// 获取行情报价标签页字体颜色
	virtual COLORREF GetQuoteGuideTabBKColor()=0;						// 获取行情报价向导标签页背景颜色
	virtual COLORREF GetQuoteGuideTabTextColor()=0;						// 获取行情报价向导标签页字体颜色
	virtual COLORREF GetQuoteActiveTabBKColor()=0;						// 获取行情报价激活标签页背景颜色
	virtual COLORREF GetQuoteActiveTabTextColor()=0;					// 获取行情报价激活标签页字体颜色
	virtual const ColorProperty* GetQuoteTabFrameColorProp()=0;			// 获取行情报价标签页边框线形与颜色
	virtual const ColorProperty* GetQuoteGuideTabFrameColorProp()=0;	// 获取行情报价向导标签页边框线形与颜色
	virtual COLORREF GetQuoteTabButtonBKColor()=0;						// 获取行情报价标签页按钮背景颜色

	virtual COLORREF GetRTitleCode()=0;          // 右小图标题股票代码颜色
	virtual COLORREF GetRTitleCodeName()=0;      // 右小图标题股票名称颜色
	virtual COLORREF GetRTableFixedName()=0;     // 右小图固定文本颜色
	virtual COLORREF GetRTableTime()=0;          // 右小图时间颜色
	virtual COLORREF GetRTableVer()=0;           // 右小图竖线颜色
	virtual COLORREF GetRTableHor()=0;           // 右小图横线颜色
	virtual COLORREF GetTableTitle()=0;          // 表格标题图元字体颜色
	virtual COLORREF GetTableTitleBK()=0;        // 表格标题图元背景颜色
	virtual COLORREF GetTradeTableFixedName()=0; // 交易图元名称列

	virtual COLORREF GetZiXunBK()=0;             // 资讯背景色
	virtual COLORREF GetZiXunFG()=0;             // 资讯文本前景色

	virtual COLORREF GetDiagramLayoutBKColor() = 0;              // 图层标签页背景
	virtual COLORREF GetDiagramLayoutActiveFillColor() = 0;      // 图层标签页激活
	virtual COLORREF GetDiagramLayoutFillColor() = 0;            // 图层标签页常规
	virtual COLORREF GetDiagramLayoutForeColor() = 0;            // 图层标签页前景
	virtual COLORREF GetDiagramLayoutBorderColor() = 0;          // 图层标签页边框


	virtual COLORREF GetTechLineColor(int nID) = 0;   // 获取技术线颜色

	////////////////////////////  写入部分  ///////////////////////////////////////////
	virtual void  SetScrollTextColor(const COLORREF textColor)=0; // 设置跑马灯文本颜色
	virtual void  SetScrollBKColor(const COLORREF bkColor)=0;     // 设置跑马灯背景颜色
};

///////////////////////////////////  HsFont ///////////////////////////////////
//字体配置
interface IHsFont: public INotify, public ICfgBase
{
	////////////////////////////  读取部分  ///////////////////////////////////////////
	virtual CFont* GetTipTitleFont()=0;       // 获取TIP提示标题字体
	virtual CFont* GetTipTxtFont()=0;         // 获取TIP提示文本字体

	virtual CFont* GetQuoteTabFont()=0;		  // 获取行情报价标签页字体
	virtual CFont* GetQuoteGuideTabFont()=0;  // 获取行情报价向导标签页字体
	virtual CFont* GetStatusFontProt()=0;	  // 获取状态栏指数字体
	virtual CFont* GetScrollFontProt()=0;     // 获取跑马灯数据字体
	 
	virtual CFont* GetQuoteRefreshFont()=0;	  // 获取行情报价更新字体
	virtual CFont* GetQuoteColHeadFont()=0;	  // 获取行情报价表头字体
	virtual CFont* GetQuoteStringFont()=0;	  // 获取行情报价文字字体(中文字)
	virtual CFont* GetQuoteCodeFont()=0;	  // 获取行情报价股票代码字体
	virtual CFont* GetQuoteDynamicFont()=0;	  // 获取行情报价动态文本字体
	virtual CFont* GetQuoteFixedFont()=0;	  // 获取行情报价固定文本字体（序）

	virtual CFont* GetRTableFixedFont()=0;        // 获取右小图字段名称字体
	virtual CFont* GetRTableDynamicalFont()=0;    // 获取右小图动态字段字体
	virtual CFont* GetRTableTitleCodeFont()=0;    // 获取右小图标题股票代码字体
	virtual CFont* GetRTableTitleNameFont()=0;    // 获取右小图标题股票名称字体
	virtual CFont* GetRTableFinanceFont()=0;      // 获取右小图配财值页文本
	virtual CFont* GetRTableTimeFont()=0;         // 获取右小图时间列字体
	virtual CFont* GetDiagramLayoutTabFont()=0;   // 获取图层标签页字体
	virtual CFont* GetDiagramTableTitleFont()=0;  // 获取表格标题图元文本字体

	virtual CFont* GetZiXunFont()=0;              // 获取资讯字体
	virtual CFont* GetDiagramTradeFont()=0;       // 获取交易图元字体
	////////////////////////////  写入部分  ///////////////////////////////////////////
	virtual BOOL SetScrollFontProt(CFont* pFontScroll)=0;             // 设置跑马灯数据字体
};

///////////////////////////////////  HsSystem ///////////////////////////////////
//开关类配置以及数据类配置
interface IHsQuoteSys: public INotify, public ICfgBase
{
	virtual BOOL IsSupportLevel2() = 0; // 是否支持Level2
	virtual BOOL IsSupportFuture() = 0;//是否支持期货行情
	virtual CString GetQuoteServerInitMarket() = 0;
	virtual CString GetFutureServerInitMarket() = 0;
};

//图形类配置
interface IHsGraph: public INotify, public ICfgBase
{
	virtual int GetIndexTrendAxisDecimal() =0;		// 指数分时图，纵坐标显示的小数位
	virtual int GetIndexKLineAxisDecimal() =0;		// 指数K线图，纵坐标显示的小数位

	//分时，K线Tab页显示的指标信息
	virtual CString GetTabTechIndexDispOrder() =0;	// 指标显示的队列顺序
};

//资讯类配置
interface IHsInfo: public INotify, public ICfgBase
{
	virtual CString GetInfoProvider() =0;   // 获取资讯提供商
};

//跑马灯配置(包含状态栏)
interface IHsScrollNews: public INotify, public ICfgBase
{
	virtual int		IsDispThirdIndex()=0;		// 第三列是否显示
	virtual int		IsDispFourthIndex()=0;		// 第四列是否显示 
	virtual int		IsScrollFixed() =0;			// 跑马灯是否固定在状态栏上
	virtual int		IsScrollMultiLine() =0;		// 跑马灯是否多行显示  	
 	virtual int		GetShowScrollAlone() =0;	// 跑马灯显示方式 0：跑马灯和状态条在同一行；1：跑马灯单独一行在状态条上面；2：状态条单独一行，跑马灯在最下面一行

	virtual CString GetStatusName()=0;          // 获取跑马灯标题名称
	virtual int		GetStatusScrollSpeed()=0;	// 滚动间隔 秒为单位
	virtual int     GetStockNewName(CStringArray &ayStockList)=0; // 获取股票新名称            
	virtual int     GetUpDownRate()=0;          // 是否显示涨跌幅
	virtual int		GetTotal()=0;               // 是否显示总额
	virtual int		GetShowNewPirce()=0;        // 是否显示最新成交价
	virtual int		GetUpDown()=0;              // 是否显示涨跌

	virtual int		GetScrollSpeed() =0;			// 行情滚动速度 毫秒为单位
	virtual int		GetHorizonScrollWay() =0;		// 行情水平滚动方向 3: 向左; 4：向右
	virtual int		GetVerticalScrollWay() =0;		// 行情垂直滚动方向 1: 向上; 2：向下
	virtual int		GetClickWay() =0;				// 显示行情方式 0：单击; 1：双击 
	virtual void	GetScrollText(CStringArray &ayScrollText)=0;  // 获取跑马灯滚动咨询

	//获取每列指数栏对应的代码列表，一条记录对应一个CStringArray，每个CStringArray里面先后包含证券代码和市场类别
	//调用此函数对每个CStringArray需进行内存释放
	virtual int		GetFirstListStockInfo(CStringArray &ayStockList) =0;
	virtual int		GetSecondListStockInfo(CStringArray &ayStockList) =0;
	virtual int		GetThirdListStockInfo(CStringArray &ayStockList) =0;
	virtual int		GetFourthListStockInfo(CStringArray &ayStockList) =0;
	//////////////////////////////////////////////////////////////////////////

	virtual BOOL	SetDispThirdIndex(int nValue) =0;    // 保存第三列显示开关 
	virtual BOOL	SetDispFourthIndex(int nValue) =0;   // 保存第四列显示开关
	virtual BOOL	SetScrollFixed(int nValue) =0;       // 保存跑马灯固定开关
	virtual BOOL	SetScrollMultiLine(int nValue) =0;   // 保存跑马灯多行开关

	virtual BOOL	SetStatusScrollSpeed(int nValue) =0; // 保存指数显示速度 秒为单位
	virtual BOOL	SetScrollSpeed(int nValue) =0;       // 保存行情滚动速度
	virtual BOOL	SetHorizonScrollWay(int nValue) =0;  // 保存行情水平滚动方向
	virtual BOOL	SetVerticalScrollWay(int nValue) =0; // 保存行情垂直滚动方向
	virtual BOOL	SetClickWay(int nValue) =0;          // 保存行情显示方式

	virtual	BOOL	SetFirstListStockInfo(const CStringArray &ayStockList) =0;
	virtual BOOL	SetSecondListStockInfo(const CStringArray &ayStockList) =0;
	virtual BOOL	SetThirdListStockInfo(const CStringArray &ayStockList) =0;
	virtual BOOL	SetFourthListStockInfo(const CStringArray &ayStockList) =0;
};

//杂项配置:	除系统基本参数配置 Tab页签 报价列属性，分时，k线，资讯，信息地雷，跑马灯配置之外的系统杂项配置
//          包括是否显示工具栏，状态栏，工具栏高度，状态条简称，右上角按钮配置等
interface IHsMiscellany: public INotify, public ICfgBase
{
	//virtual CString GetRYZQEQWebSite()=0;		//瑞银研究报告EQ用户链接地址
	//virtual int IsLevel2Must()=0;					//1：具备level2账号时必须登录level2行情才能使用系统；默认为0，不输入level2账号可以使用level1行情

	//virtual int IsDispBulletin()=0;					//登录系统时是否提示紧急公告，默认1：提示，0：不提示；提示紧急公告的内容从服务器取(welcome.htm)
	//virtual int IsDispTradeTime()=0;				//控制成交明细的时间是否完全显示。默认0：分笔成交时间不完全显示；1：分笔成交时间完全显示
	//virtual int IsDispSubTitle()=0;					//是否在框架标题栏显示子窗口标题，默认1：不显示；0：显示
	//virtual int IsCommunicationModified()=0;		//禁止修改通讯配置，默认0：允许对通讯配置进行删除、修改；1：不允许对通讯配置进行删除、修改

	//virtual int IsDispSelfBlockTip()=0;				//加了自选股之后是否提示信息，默认1：提示；0：不提示
	//virtual int IsDispIPAndPort()=0;				//1：通讯配置里不显示IP地址，0：显示

	//virtual int IsDispZixun()=0;					//是否显示资讯
	//virtual int IsNotShowWindowList()=0;			//主菜单中“窗口”菜单下是否显示打开过的窗口列表，0或不写该键值：打开窗口列表；1：不打开窗口列表
	//virtual int IsShowLoginStatur()=0;				//框架标题栏是否显示交易登录状态，默认1：显示；0：不显示
	//virtual int IsTradeMaximal()=0;					//默认0：行情+交易方式登录系统时交易界面不是最大化显示；1：行情+交易方式登录系统时交易界面最大化显示
	//virtual int IsLevel2AccountSave()=0;			//是否已保存Level2账号

	//virtual int IsSupportMainMenu()=0;				//默认0或不写：不支持主菜单界面；1：支持主菜单界面
	//virtual int IsHQUseBestSite()=0;				//行情登录使用最快站点
	//virtual int IsDispLevel2()=0;					//是否显示Level2面板
	//virtual int IsSupportLevel2()=0;                //是否支持Level2,如之前老版本的编译开关
	//virtual int IsGetLevel2AccoutFromServer()=0;	//是否重服务器取Level2账号

	//virtual int IsOpenTipInKeyBoard();				//键盘000弹出今日提;默认0或不写：不支持打开今日提示；1：支持打开今日提示(welcome.htm)，
	//virtual int IsDispDailyBulletin()=0;			//登录完成后显示的券商的每日信息公告
	//virtual int IsShowUseTip()=0;					//投资赢家客户端使用介绍Tip
	//virtual int GetUseTipFilePos()=0;				//投资赢家客户端使用介绍Tip的文件位置

	//virtual int IsEnableStrategeTrade()=0;			//策略交易是否有效
	//virtual int IsStrategeTradeRelateKline()=0;		//是否将策略测算结果关联到技术指标，1：关联到技术指标；0：不关联到技术指标

	//virtual int QuoteExcludeIsUsed()=0;				//光大客户行情报价是否使用过滤功能
	//virtual int QuoteExcludeIsNowNew()=0;			//是否剔除上市新股
	//virtual int QuoteExcludeIsNotUpStop()=0;		//是否剔除无涨跌停股票
	//virtual int QuoteExcludeIsNotSmall()=0;			//是否剔除上市新股
	//virtual int QuoteExcludeIsNotGrow()=0;			//是否剔除创业板股票
	//virtual int QuoteExcludeIsNotSCapt()=0;			//是否剔除流通盘小于一定数值股票
	//virtual int QuoteExcludeIsShowSmall()=0;		//是否剔除中小板股票
	//virtual int QuoteExcludeAmtRatio()=0;			//当日成交量/涨停挂单量 阀值:
	//virtual int QuoteExcludeAmtCapital()=0;			//流通盘股票数量


	//virtual int GetLoginType()=0;			 //1：新模式登录即登录界面在初始化等完成后在消失，此需要后台服务器支持；0:为老模式，初始化等工作在登录界面消失后进行
	//virtual int GetTabBarPos()=0;			 //报价界面的Tab工具条显示位置，默认0：底部；1：顶部
	//virtual int GetReportScrollBarType()=0;  //报价标签页边框风格选择，默认1：红色边框风格，0：原风格

	//virtual int GetMultiMenu()=0;			//系统板块中板块显示方式，默认0：原来样式，单列显示；1：多列显示
	//virtual int GetShowScrollAlone()=0;		//是否将跑马灯单独一行显示，默认2：状态条单独一行，跑马灯在最下面一行，0：跑马灯和状态条在同一行；1：跑马灯单独一行在状态条上面

	//virtual int GetBulletinWidth()=0;		//BulletinWidth，紧急公告提示框宽度
	//virtual int GetBulletinHeight()=0;		//BulletinHeight，紧急公告提示框高度
	//virtual int GetReportGridWidth()=0;		//表格行高度，报价列表的行高
	//virtual int GetLogStockCount()=0;		//退格键支持股票数，在分时或K线界面切换几个代码，然后按退格键(Backspace键)可以返回上一个代码，配置表示可以退到之前多少个代码
	//virtual float  GetCalculator()=0;			//浮动计算器税率

	//virtual int GetExportFileType()=0;			//导出文件功能，导出文件的类型
	//virtual CString GetExportFilePath()=0;		//导出文件功能，导出文件的路径
	//virtual CString GetPanHouPath()=0;			//数据读取路径，本地缓存数据存放路径，默认盘后数据目录为data，默认盘中数据目录为LOND\dat
	//virtual CString GetPanzhongPath()=0;		//数据读取路径，本地缓存数据存放路径，默认盘中数据目录为data，默认盘中数据目录为LOND\dat
	//virtual CString GetVersionNumber();			//关于对话框显示的版本号

	//virtual int GetCurrentPageID()=0;			//页面ID
	//virtual CString GetCurrentPageType()=0;		//使用页面类型
	//virtual CString GetFontTemplet()=0;			//字体模板
	//virtual CString GetColorTemplet()=0;		//颜色模板
	//virtual CString GetLineTemplet()=0;			//线型使用模板

	virtual  int SetSectionValue(CString lpszSection, CString lpszValue) = 0;		//按照配置文件格式内容，传入section对应的值， 
	virtual  CString GetSectionValue(CString lpszSection) = 0;		//按照配置文件格式内容，读取section对应的值， 
};


///////////////////////////////////  HsColumnProperty ///////////////////////////////////
//报价列属性
interface IHsColumnProperty: public INotify, public ICfgBase
{
	virtual TableColProperty*	GetColumn(int nID)=0;	    // 获取列属性
 	virtual CString		GetDefDispColGroupName()=0;		    // 获取默认显示列组名称
 	virtual CString		GetDefaultColGroupName()=0;		    // 获取默认列组名称 可删除先保留
 	virtual long		GetAllColumnGroupName(CStringArray& ayGroup)=0;		// 获取所有列组名称
 	virtual CColPropArray* GetColumnGroupProp(CString strGroupName)=0;		// 获取指定列组所有列属性
	virtual BOOL           GetColumnGroupPropEx(CColPropIDIndex &mapProp, CString strGroupName) = 0;    // 功能同上 返回值不同
	virtual CString		GetFinanceGroupName()=0;			// 获取财务数据列组名称
	virtual long		GetAllDispColTemplName(CStringArray& ayGroup)=0;	// 获取所有显示列模板名称
 
	virtual RefreshDispTextProp* GetQuoteRefreshProp()=0; // 获取刷新属性结构

	virtual CString		GetColumnName(int nID)=0;     // 获取列名称
	virtual long		GetColumnMask(int nID)=0;     // 获取列掩码
	virtual long		GetColumnWidth(int nID)=0;    // 获取列宽度

	//获取显示列中的各个列的属性 ayGroup中奇数项存放ColumnID，偶数项存放ColumnWidth
	virtual long        GetDispColGroup(CString strGroup, CUIntArray*& ayGroup)=0; 
	virtual void		SetDispColGroup(CString strGroup, CUIntArray* ayGroup)=0;	// 更改显示列组内容
	virtual BOOL		AddDispColGroup(CString strGroup, CUIntArray* ayGroup)=0;	// 添加显示列组
	virtual BOOL		DelDispColGroup(CString strGroup)=0;		// 删除显示列组
};

///////////////////////////////////  HsTabCfg ///////////////////////////////////
//Tab页签接口
interface IHsTabCfg : public INotify, public ICfgBase
{
	virtual CString		GetDefaultTabGroup()=0;							// 获取默认显示的标签页组名称
	virtual int 		GetDefaultTabItemID()=0;						// 获取默认选中的标签页ID

	virtual long		GetAllTabGroups(CStringArray& ayTabGroup)=0;	// 获取所有标签页组名称
	virtual long		GetSysTabGroups(CStringArray& ayTabGroup)=0;	// 获取系统默认标签页组名称
	virtual long		GetUserTabGroups(CStringArray& ayTabGroup)=0;	// 获取用户自定义标签页组名称

	virtual long		GetUserDefMenuID(int nID)=0;

	virtual long		GetTabList(CString strGroupName,CTabItemArray& ayTabItem)=0;	// 获取Tab列表
	virtual long		GetSysTabList(CString strGroupName,CTabItemArray& ayTabItem)=0;	// 获取系统Tab列表
	virtual BOOL		SetTabList(CString strGroupName,CTabItemArray* ayTabItem)=0;	// 设置指定分类Tab列表
	virtual BOOL		DelTabList(CString strGroupName)=0;				// 删除指定标签页分类
};

///////////////////////////////////  IHsCommunication ///////////////////////////////////
//通讯相关配置项
interface IHsCommunication : public INotify, public ICfgBase
{
	virtual int GetSysServer(Server_type type, CCommInfoArray *pAy, int nCellID = -1) = 0;    // 获取指定类型的系统默认站点
 	virtual int GetUsrServer(Server_type type, CCommInfoArray *pAy, int nCellID = -1) = 0;    // 获取指定类型的用户自定义站点
	virtual int GetAllSysServer(CCommInfoArray *pAy, int nCellID) = 0;   // 获取所有类型的系统默认站点
	virtual int GetAllUsrServer(CCommInfoArray *pAy, int nCellID) = 0;   // 获取所有类型的用户自定义站点
 	virtual void SaveUsrServer(Server_type type, CCommInfoArray *pAy, int nCellID = -1) = 0;  // 保存指定类型的用户自定义站点
 	virtual void SaveAllUsrServer(CCommInfoArray *pAy, int nCellID) = 0;  // 保存所有类型的用户自定义默认站点
 	virtual BOOL GetProxy(ProxyInfo *proxy) = 0; // 获取代理信息
	virtual void SaveProxy(ProxyInfo *proxy) = 0;// 保存代理信息

 	virtual BOOL GetAuto() = 0;                  // 获取自动测速开关
 	virtual BOOL SetAutoChooseBest(UINT au) = 0; // 获取自动连接最快站点开关

	virtual int  GetHeartBeat(Server_type type) = 0; // 获取心跳间隔

 	virtual BOOL GetSwitch(Server_type type) = 0; // 获取指定类型显示开关
	virtual void GetAllSwitch(CMap<Server_type,Server_type,BOOL,BOOL> *pAy) = 0;  // 获取所有类型显示开关
 	virtual CString GetDefaultSite(Server_type type, int nCellID = -1) = 0; // 获取指定类型默认站点
	virtual void    GetAllDefaultSite(CMap<Server_type,Server_type,CString,CString> *pAy, int nCellID) = 0; // 获取所有类型默认站点
	virtual void    SetDefaultSite(Server_type type, CCommInfoArray *pSysAy, CCommInfoArray *pUsrAy, int nCellID = -1) = 0; // 设置指定类型默认站点
	virtual void    SetAllDefaultSite(CCommInfoArray *pSysAy, CCommInfoArray *pUsrAy, int nCellID) = 0;     // 设置所有类型默认站点
	virtual void    RemoveSysMap(Server_type type, int nCellID) = 0; // 移出map中内容
	virtual void    RemoveUsrMap(Server_type type, int nCellID) = 0; // 移出map中内容

	virtual int     GetSaveAccount() = 0;             // 获取保存账号开关
	virtual BOOL    SetSaveAccount(UINT unSave) = 0;  // 设置保存账号开关
	virtual int     GetHideAccount() = 0;             // 获取隐藏账号开关
	virtual BOOL    SetHideAccount(UINT unHide) = 0;  // 设置隐藏帐号开关

	virtual BOOL    GetLevelAcc(CString &accName, CString &accPwd) = 0; // 获取配置Level2账号、密码
	// 获取/保存账号信息 ayAccInfo中按账号名称、账号类型、营业部ID存放 nAccCount表示存储账号个数
	virtual int     GetAccount(CStringArray& ayAccInfo) = 0;
	virtual BOOL    SaveAccount(CStringArray& ayAccInfo, int nAccCount) = 0; 
	virtual BOOL    RemoveAllAccount() = 0; // 删除所有存储的账号
};

///////////////////////////////////////// IHsKeyboardCfg /////////////////////////////////
// 键盘精灵热键 只读
interface IHsKeyboardCfg : public INotify 
{
	virtual int GetOrderMarket(CStringArray &ay, CString strKey) = 0;  //获取市场排序
	virtual int GetCutShort(CStringArray &ay) = 0; //读取热键
};


///////////////////////////////////  IHsUserDataCfg ///////////////////////////////////
// 运行中的用户数据读写接口
interface IHsUserDataCfg : public INotify, public ICfgBase
{
	virtual const HsCodeMark* SearchMark(IN CString& strKeyWord)=0; //根据关键字查找标记内容
	virtual BOOL DelMark(IN CString& strKeyWord)=0;		        // 删除指定标记
	virtual BOOL SetMark(IN const HsCodeMark& CodeMark)=0;		// 设定标记
};

interface IBlockManager : public INotify, public ICfgBase
{
	// 板块操作
	virtual long	GetAllBlockGroupName(OUT CStringArray& ayBlockGroupName)=0;	// 获取所有板块分类名称
	virtual long	GetAllSysBlockGroupName(OUT CStringArray& aySysBlockGroupName)=0;
	virtual long	GetAllUserBlockGroupName(OUT CStringArray& ayUserBlockGroupName)=0;
	virtual long	GetBlockName(IN CString& strBlockGroupName, OUT CStringArray& ayBlockName)=0;	// 获取指定板块分类的所有板块名称
	virtual long	GetAllBlockName(OUT CBlockGroupArray& ayBlockName)=0;	    // 获取所有板块名称

	// 获取成分股信息
	virtual long	GetStockList(IN CString strBlockName, OUT CArray<CodeInfo>& ayStockList)=0;	// 获取指定板块的成分股列表
	virtual long    GetStockList(IN CString strBlockName, OUT CMapStringToPtr& mapStockList)=0; // 获取指定板块的成分股列表 加到map中
	virtual long	GetStockListCount(IN CString strBlockName)=0;		// 获取指定板块成分股个数
	virtual long	GetStockAdscription(IN CodeInfo* pCode,OUT CStringArray& ayBlockName)=0;		// 查找所属板块

	// 操作成分股列表
	virtual BOOL	AddStockToBlock(IN CString strBlockName, IN CodeInfo* code)=0;		// 添加一个成分股到板块末尾
	virtual BOOL	InsertStockToBlock(IN CString strBlockName, IN CodeInfo* code, IN long nIndex)=0;	// 插入一个成分股到指定板块的指定位置
	virtual long	DelStockFromBlock(IN CString strBlockName, IN CodeInfo* code)=0;	// 从指定板块删除指定成分股

};

// k线tab配置项
interface IHsKLineTab
{
	virtual CString GetDefaultGroupName()=0;   // 获取默认选中的名称

	virtual int  GetTechIndexAyCount()=0;                 // 获取指标项的数量
	virtual CString GetTechIndexName(IN int nIndex)=0;    // 获取某一指标项的名称
	virtual int  GetTechIndex(IN int nIndex, OUT CTechIndexArray& ayTechIndex)=0;          // 获取指标项
	virtual int  GetTechTempleteAyCount()=0;              // 获取模板项的数量
	virtual CString GetTechTempleteName(IN int nIndex)=0; // 获取某一模板项的名称
	virtual int  GetTechTemplete(IN int nIndex, OUT CTechTempleteArray& ayTechTemplete)=0; // 获取模板项
};

//K线分时线子图元位置管理项
interface IHSKLinePosition
{
	virtual int				GetTechLines(IN int nType, CStringArray& ayStrRet, BOOL bResearch = FALSE) = 0;	//获取位置控制前的所有图元
	virtual float			GetPosCtrolRatio(IN int nType) = 0;												//获取位置控制的比例
	virtual int				SetTechLines(IN int nType, IN CStringArray& strKLIneName, 
								BOOL bFirst = FALSE, BOOL bLast = FALSE) = 0;								//写入图元
	virtual int				SetPosCtrolRatio(IN int nType, IN float fRatio) = 0;							//写入位置控制
};