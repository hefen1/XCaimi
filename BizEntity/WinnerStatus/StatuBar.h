#pragma once

#include "..\..\HelperLib\GeneralHelper.h"
#include "HSBizDialog.h"
#include "ScrollNewsBar.h"
#include "ScrollFrame.h"
#include "HSMacro.h"

#include "DataSourceDefine.h"
#include "DataSourceSinkDefine.h"
#include "DataSourceDefineEx.h"
#include "hsstructnew.h"
#include "hsds_comudata.h"

#include "ConfigInterface.h"
#include "TradeComm\TradeCoreIntf.h"
#include ".\XInfoTip\XInfoTip.h"

//自定义消息
#define WM_SCROLL_MOVE      (WM_USER+100)
// CStatuBar 对话框

//要显示数据的结构体
struct strStruct
{
	char name[strStructLenth];      //股票名称
	char nowPrice[strStructLenth];  //现价
	char ricePrice[strStructLenth]; //涨跌
	char riceRatio[strStructLenth]; //涨幅
	char totalMoney[strStructLenth];//总额
	short   trend;     //标志涨跌 -1：跌 0：平 1:涨

	strStruct()
	{
		name[0] = '\0';
		nowPrice[0] = '\0';
		ricePrice[0] = '\0';
		riceRatio[0] = '\0';
		totalMoney[0] = '\0';
		trend = -2;
	}

	~strStruct()
	{

	}

	strStruct& operator=(const strStruct &temp)
	{
		strncpy(name,      temp.name,      strStructLenth);
		strncpy(nowPrice,  temp.nowPrice,  strStructLenth);
		strncpy(ricePrice, temp.ricePrice, strStructLenth);
		strncpy(riceRatio, temp.riceRatio, strStructLenth);
		strncpy(totalMoney,temp.totalMoney,strStructLenth);
		trend = temp.trend;
		return *this;
	}

	BOOL operator==(const strStruct &temp)
	{
		if (memcmp(this, &temp, sizeof(strStruct)))
		{
			return FALSE;
		}
		return TRUE;
	}

	BOOL operator!=(const strStruct &temp)
	{
		if (memcmp(this, &temp, sizeof(strStruct)))
		{
			return TRUE;
		}
		return FALSE;
	}

};

//记录各种股票数据的结构体
struct StatuBarData
{
	StockUserInfo       m_stock;
	long				m_lOpen;         		// 今开盘
	long				m_lMaxPrice;     		// 最高价
	long				m_lMinPrice;     		// 最低价
	long				m_lNewPrice;     		// 最新价
	unsigned long		m_lTotal;				// 成交量(单位:股)
	float				m_fAvgPrice;			// 成交金额
	long		        m_lPreJieSuanPrice;     // 昨结算价 针对期货
	void CopyFromRealTimeData(HSStockRealTime* pData)
	{
		this->m_lOpen            = pData->m_lOpen;
		this->m_lMaxPrice        = pData->m_lMaxPrice;
		this->m_lMinPrice        = pData->m_lMinPrice;
		this->m_lNewPrice        = pData->m_lNewPrice;
		this->m_lTotal           = pData->m_lTotal;
		this->m_fAvgPrice        = pData->m_fAvgPrice;
		this->m_lPreJieSuanPrice = 0;
	}

	void CopyFromRealTimeData(HSQHRealTime* pData)
	{
		this->m_lOpen			 = pData->m_lOpen;
		this->m_lMaxPrice		 = pData->m_lMaxPrice;
		this->m_lMinPrice		 = pData->m_lMinPrice;
		this->m_lNewPrice		 = pData->m_lNewPrice;
		this->m_lTotal			 = pData->m_lTotal;
		this->m_fAvgPrice		 = 0;
		this->m_lPreJieSuanPrice = pData->m_lPreJieSuanPrice;
	}

	void GetStockData(StockUserInfo* pData)
	{
		pData->Copy(&this->m_stock);
	}

	void GetRealTimeData(HSStockRealTime* pData)
	{
		pData->m_lOpen     = this->m_lOpen;
		pData->m_lMaxPrice = this->m_lMaxPrice;
		pData->m_lMinPrice = this->m_lMinPrice;
		pData->m_lNewPrice = this->m_lNewPrice;
		pData->m_lTotal    = this->m_lTotal;
		pData->m_fAvgPrice = this->m_fAvgPrice;

	}

	void GetRealTimeData(HSQHRealTime* pData)
	{
		pData->m_lOpen			  = this->m_lOpen;
		pData->m_lMaxPrice		  = this->m_lMaxPrice;
		pData->m_lMinPrice		  = this->m_lMinPrice;
		pData->m_lNewPrice		  = this->m_lNewPrice;
		pData->m_lTotal			  = this->m_lTotal;
		pData->m_lPreJieSuanPrice = this->m_lPreJieSuanPrice;

	}

	void SetEmpty()
	{
		m_stock.SetEmpty();
		this->m_lOpen            = 0;
		this->m_lMaxPrice        = 0;
		this->m_lMinPrice        = 0;
		this->m_lNewPrice        = 0;
		this->m_lTotal           = 0;
		this->m_fAvgPrice        = 0;
		this->m_lPreJieSuanPrice = 0;
	}
	
};



struct TIndexData
{
	short	m_nRiseCount;	//上涨家数
	short	m_nFallCount;	//下跌家数
	short	m_nLead;            //领先指标
	short	m_nRiseTrend;       //上涨趋势
	short	m_nFallTrend;       //下跌趋势
	short   m_nTotalStock;		//股票总数
	short   m_nPreRiseTrend;	//上一次的上涨趋势值
	short   m_nPreFallTrend;	//上一次的下跌趋势值
	short   m_nFlags[8];

	TIndexData()
	{
		m_nRiseCount = 0;
		m_nFallCount = 0;
		m_nLead      = 0;
		m_nRiseTrend = 0;
		m_nFallTrend = 0;
		m_nTotalStock   = 0;
		m_nPreRiseTrend = 0;
		m_nPreFallTrend = 0;

		for( int i = 0; i < 8;i++ )
		{
			m_nFlags[i] = 3;
		}
	}
};


class CStatuBar : public CHSBizDialog,
	public IDataSourceSink, //实现数据引擎的回调函数
	public IAccountChange   //实现账号管理器的回调函数
{
	DECLARE_DYNAMIC(CStatuBar)

public:
	CStatuBar(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CStatuBar();

// 对话框数据
	enum { IDD = IDD_STATUEBAR };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	DECLARE_MESSAGE_MAP()
//消息函数
public:
	virtual BOOL OnInitDialog();
	virtual BOOL OnInit();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();
	afx_msg int  OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);

	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnDestroy();
	//功能函数
public:	

	BOOL ReadCfg();   //从配置文件里读取数据 
	BOOL ReadColor(); //从颜色配置文件中读取上涨、下跌、平盘、股票代码名称的颜色
	BOOL ReadFont();  //从字体配置文件中读取字体

	void WriteCfg();    //写配置文件

	//通过账号管理器 读取当前账号信息 并转化为字符数组(服务器类型 服务器名称 连接状态)这个顺序存放
	void GetAccountMng(); 
	//将服务器类型装换成字符串信息 bSwitch=true匹配账号管理器;bSwitch=false匹配通信库
	void SwitchTypeToString(int nType, CString &strServer, BOOL bSwitch);  
	void AddAccountMng(IAccount* pAccount); //添加账号信息

	//绘制部分
	void DrawIndexPanel(CDC* dc);  //绘制状态栏指数
	void DrawTimePanel(CDC* dc);   //绘制时间 
	void DrawProgress(CDC* dc);    //绘制通讯状态
	void DrawStatusName(CDC* dc);  //绘制状态栏名称

	void DrawRedGreenLine(CDC* dc);//绘制红绿灯
	void DrawRedGreenFlag(CDC* pDC, CRect rc, short offset );//绘制红绿灯里面的图片
	short DrawRGRectOffset(short flags ); //根据flag值确定偏移量
	short CalWidth(short total, short value,short width);

	void FillDockBar(CDC* dc, CRect& rc);
	void SetSize();
	
	void GetShowString();           //获得要显示的数据
	void ReplaceName(char* name, char* code);   //根据券商要求显示名字
	void MoveScrollNewsBarWnd();    //调整跑马灯的位置

	//数据引擎接口
	BOOL HSDataSourceSink_OnCommNotify(void* pData);
	BOOL HSDataSourceSink_OnRecvData(void* pData, int nLen);
	void RequestRealTimeData(BOOL bNeedAutoPush=TRUE);  //请求实时数据
	
	void GetServerTime();                               //请求服务器时间


	BOOL OnRecvData( void* pData, int nLen );           //消息处理函数
	void HandleAutoPushData(DataHead* pHead, CommRealTimeData* pnowData, int nSize);
	void HandleNowData(DataHead* pHead, CommRealTimeData* pnowData, int nSize);
	
	short SetFlags( short lead, short risetrend,short falltrend, short prerise, short prefall);//上证 深证数据处理函数
	
	void UpdateDataEntry(  StatuBarData* pStatus, int index, int nType = MAKELONG(0,LTOA_MASK_DEFAULT) );
	void UpdateIndexData(  StockUserInfo* pStock, long nUnit, short nDecimal, long lClose, HSStockRealTime* pData, int index, int nType = 0);
	void UpdateStockData(  StockUserInfo* pStock, long nUnit, short nDecimal, long lClose, HSStockRealTime* pData, int index, int nType = 0);
	void UpdateFuturesData(StockUserInfo* pStock, long nUnit, short nDecimal, HSQHRealTime* pData,    int index, int nType = 0);
	void GetStockInfo();  //根据codeInfo请求stockUserInfo

	BOOL UpdateRealTimeData(CodeInfo* pCode,ShareRealTimeData* pStock); //将返回的数据保存下来
	
	//账号管理回调函数
	void OnAccountChange(IAccount* lpValue, EAccountChange nChangeType); //账户发生改变
	void OnAccountStatus(IAccount* lpValue); //账户状态发生改变
	//成员变量
private:
	//跑马灯变量
	CScrollNewsBar m_scrollNewsBarWnd;
	CRect m_rectScroll;       //跑马灯的矩形

	//跑马灯框架变量
	CScrollFrame m_scrollFrame;

    CRect m_rectScrollFloat;  //跑马灯浮动的矩形
	CRect m_rectIndex;        //指数的矩形
	CRect m_rectRedGreen;     //红绿均的矩形
	CRect m_rectTimePanel;    //时间的矩形
	CRect m_rectProgress;     //通讯状态矩形
	CRect m_rectHomeUrl;      //状态栏左侧名称的矩形
	long  m_lRed_Green_Panel;

	CImageList   m_lstCommStatusImage;   // 通信状态图标
	CBitmap      m_bmpRedFlag;           // 红绿均图片

	CString m_strTime;        //现在的时间
	long    m_lServerTime;    //服务器时间
	DWORD   m_dwTick;         //请求回来之后的Tick值

//	CTimeSpan m_timeSpace;    //与服务器时间的间隔
//	CTime     m_timeServer;   //服务器时间

	CString   m_strStatusName;   //最下面状态栏左侧显示的名称 在happy.ini中[主框架]下“主标题”

	int m_nIsUpDownRate;    // 标记涨跌幅是否显示
	int m_nIsTotal;         // 标记总额是否显示
	int m_nIsShowNewPirce;  // 标记成交价是否显示
	int m_nIsUpDown;        // 标记涨跌是否显示

	BOOL m_threeShow; //标记第三列是否显示
	BOOL m_fourShow;  //标记第四列是否显示
	unsigned int m_scrollSpeed; //记录翻滚间隔时间
	//数据类型变量
	CArray<StatuBarData> m_oneList;  //第一列股票
	CArray<StatuBarData> m_twoList;  //第二列股票
	CArray<StatuBarData> m_threeList;//第三列股票
	CArray<StatuBarData> m_fourList; //第四列股票

	CStringArray m_newNameList;   //记录根据券商要求 显示的股票名称

	unsigned int m_nShowIndex;    //标记当前翻滚显示的行号  
	int          m_nIndexOff[INDEX_MAX_COUNT];	 //指数栏上相对左边位置
	
	// 接口指针
	IBlockManager*  m_pBlockManager;    // 板块管理指针
	IDataSourceEx*	m_pDataSource;		// 数据引擎指针
	long			m_nDataID;			// 数据请求ID

	IHsScrollNews*    m_pScrollCfg;   // 跑马灯配置文件读写接口
	IHsCommunication* m_pCommConf;    // 通讯配置文件读写接口
	ITradeCoreObj*	  m_pTradeCore;   // 获取交易接口对象
	IHsColor*         m_pColorCfg;    // 颜色读写接口
	IHsFont*          m_pFontCfg;     // 字体读写接口

	TIndexData    m_TSHIndex; // 上证数据
	TIndexData    m_TSZIndex; // 深证数据

   	//定时器事件标记
	int m_nTimeIndexID;   // 绘制时间事件  
	int m_nScrollIndexID; // 滚动显示事件

	CXInfoTip  m_tip;
	CStringArray m_ayConnectNotify; //通讯信息 根据(服务器类型 服务器名称 连接状态)这个顺序存放
//	CRITICAL_SECTION m_lock; //对通信信息进行加锁 防止出现读写冲突

public:
	strStruct m_indexString[INDEX_MAX_COUNT];//需要显示的字符

	COLORREF         m_clrUp;       // 上涨颜色
	COLORREF         m_clrDown;     // 下跌颜色
	COLORREF         m_clrEqual;    // 平盘颜色
	COLORREF         m_clrCodeName; // 代码名字颜色
	CFont*           m_pStatusFont; // 状态栏指数字体
	//static CString   m_HomeUrlName;

};

