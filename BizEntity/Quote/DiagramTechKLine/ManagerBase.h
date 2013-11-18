/*************************************************************
*	Copyright(c) 2010, 杭州恒生信息技术有限公司
*	All rights reserved.
*
*	文件名称：	TechKlineMgr.h
*	文件标识：
*	摘	  要：  技术分析图元接口(管理层)
*	
*	当前版本：
*	作	  者：	蒋国宏
*	完成日期：  2010-4-25
*
*	备	  注： 管理层实现统一的数据请求、坐标变换等等
*	修改记录：
*	作	  者：
*	修改日期：
*	修改内容：
*
****************************************************************/

#pragma once
#include "..\..\..\Common\hsds_comudata.h"
#include "..\..\..\Common\QuoteComm\DrawBase.h"
#include "..\..\..\Common\FormulaManPublic.h"
#include "..\..\..\Common\FormulaManInterFace.h"
#include "..\..\..\Common\QuoteComm\QuoteDiagram.h"
#include "..\..\..\Common\DataSourceDefineEx.h"
#include "..\..\..\Common\HSMessage.h"

#include "HSLinear.h"
#include "HSDrawBaseStruct.h"

#define MAX_FENSHI_DAYS	10
#define PKLINE_TYPE_FENSHI   		0x0001  // 分时图
#define PKLINE_TYPE_K       		0x0002  // k 线图
#define PKLINE_NAME_FENSHI   		"PKLINE_NAME_FENSHI"  // 分时图
#define PKLINE_NAME_K       		"PKLINE_NAME_K"  // k 线图
//////////////////////////////////////////////////////////////////////////
//
#define HsTechLine_Left_ChuQuan	    0x00000001 // 左边除权
#define HsTechLine_Back_ChuQuan		0x00000002 // 向后除权

struct CTechKLineData
{
	StockDay*                       m_pStockDayData;				//原始数据(针对除权数据)
	int								m_nCurrentDataLen;				//当前数据长度
	float							m_afKLineHorizCellWidth[20];	//横向单元格宽度数组
	int								m_nWidthIndex;					//当前使用数组数据项
	BOOL							m_bDataFinished;				//是否存有所有技术线数据?
	int								m_nCurrentRequestLen;			//当前请求的长度
	BOOL							m_bNeedRefresh;					//更新股票代码时，是否需要重新请求数据的判断依据
	int								m_nCurrentDataPos;				//当前定位点
	long							m_lChuquanMask;					//除权复权标志
	StockDay*						m_pStockDayDataChuquan;			//除权计算数据

	CTechKLineData();
	BOOL				Init(int nSize);					//初始化
	BOOL				Release();							//释放内存
	BOOL				Refresh(int nNewSize);				//刷新
	BOOL				CopyData(StockCompDayDataEx* pData, int nSize);
	float				GetKLineHorizCellWidth();
	float				GetNextWidth(BOOL bNext);
	BOOL				IsKLineDataFinished(int nReqireLen, int nCurIndex);
	long				GetIndexDate(int nIndex);
	BOOL				SetRefreshFlag(BOOL bNeedRefresh);

	/*分时中分钟线的数据处理类型*/
	int					ResetPos(int nNewLen);
//	StockCompDayDataEx*	GetStockCompDayDataEx();
	StockDay*			GetStockDayData();
	int					GetStockCompDayDataLen();

//除权复权处理
	long				GetChuquanMask();
	BOOL				CalcChuQuan(int nDayType, CArray<ChuQuanData*,ChuQuanData*>& ayChuQuanData, 
							long lMask, long nPriceUnit);
	long				GetSpanDays(long lBgnDate,long lEndDate);
	BOOL				InitChuquanDayData(BOOL bRelease = FALSE);
	StockDay*			GetCurrentDrawData();
};

//////////////////////////////////////////////////////////////////////////
class CTechKLineMgr;
struct CFenshiData
{
	StockUserInfo					m_info;
	DrawFenShiData*					m_pPriceValues; // 价络数据数据指针
	long*							m_pChengJiaoValues; // 成交量数据指针
	long*							m_pAvePriceValues; // 均价数据指针
	double long						m_lTotalVol; // 总量当日
	double long						m_lTotalPrice; // 总金额当日
	int								m_nTimeID;		//定时器-填充如果整个分钟都没有任何主推数据
	CTechKLineMgr*					m_pMgr;


	int								m_nCurrentDataLen;		//当前数据长度
	int								m_nLastReqDay;			//上一次请求的天数
	int								m_nLastRecvDay;			//上一次收到的多日分时天数
	int								m_nOneDayDataNum;		//每日数据个数
	int								m_nRecvDataLen;			//当前已经填充数据个数
	int								m_nCurFenshiPos;		//当前的定位点--不必要反复请求数据

	DrawFenShiData*					m_pPriceValuesCur;		//当前定位点- 价络数据数据
	long*							m_pChengJiaoValuesCur;	//当前定位点- 成交量数据指针
	long*							m_pAvePriceValuesCur;	//当前定位点- 均价数据指针
	long							m_ayDayData[MAX_FENSHI_DAYS + 1];
	long							m_ayPrevClose[MAX_FENSHI_DAYS + 1];		//昨收	
	CTechKLineData					m_minData;				//保存分钟线数据

	CFenshiData();
	BOOL				Init(StockUserInfo* pInfo, int nSize, BOOL bForce);	//初始化
	BOOL				Release();							//释放内存
	BOOL				Refresh();							//刷新
	int					UpdataHisTrend(AnsHisTrend* pTrendData, int& nCurDataLen);
	int					UpdataTreadEx(AnsTrendData_Ext* pExtData);
	int					UpdateRealTimeData(CommRealTimeData_Ext* pCommRealTimeData);
	int					UpdateFenshiDataPos(int nDayPosNew, int& nLeftLen);
	int					GetPosTime(int nPos, CString& strText, BOOL bIntegral = TRUE);
	long				GetPrevClose();
	BOOL				GetMinDataRequest(IRequestCollector* pReqCollector, int nReqLen);
	int					UpdateMinTechData(DataHead* pData);
	BOOL				GetNeedRefresh(StockUserInfo* pInfo);
	CString				GetLastNewPrice(BOOL bBuy);				//获取最新价格（买一或卖一）
	void				UpdateOneMinuteEmptyData();				//一分钟空数据
	void				SetTechMgr(CTechKLineMgr* pMgr);
	void				StartTimer();
	void				StopTimer();

	static VOID CALLBACK    MinuteProc(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime);  // 分时分钟线数据填充定时器
};

//////////////////////////////////////////////////////////////////////////
class CManagerBase
{
public:
	CManagerBase();
	~CManagerBase();
protected:
	//分时数据
	CFenshiData						m_dataFenshi;		
	//K线数据
	CTechKLineData					m_dataKLine;
	//数据长度
	long							m_lDataVauleLen;
	IFormulaMan*					m_pFormulaMan;
	int								m_nStartPos;
	int								m_nEndPos;
	//日期
	long							m_lDate;
	StockType						m_stockType;
	CGraphics::CAxesType			m_nAxesType;

public:
//	virtual		int									GetStockCompDayDataEx(StockCompDayDataEx*& pStockCompDayData);
	virtual		int									GetStockDayData(StockDay*& pStockDaya);
	virtual		int									GetStockDay(StockDay*& pStockDay);
	virtual		int									GetStockFenShiData(DrawFenShiData*& pDrawFenshiData);
	virtual		int									GetStockChengJiaoValues(long*& pChengJiaoValues);
	virtual		int									GetStockAveFenshiPrice(long*& pAvePriceValues);
	virtual		BOOL								GetStartEndPos(int& nStartPos, int& nEndPos);
	virtual		IFormulaMan*						GetFormulaMan();
	virtual		CString								GetTimeByPos(int nCursorPos,CString& strText,int nType = 0);
	virtual		long								GetFenshiPrevClose();
	virtual		CGraphics::CAxesType				GetAxesType();
	virtual		int									DelayDeleteTechLine(CTechKLineImp* pTechLine);
	virtual 	StockUserInfo*						GetStockUserInfo() = 0;
	virtual		void								GetTopMaxMinData(HSDouble& dMax, HSDouble& dMin, HSDouble& dOpenPrice) = 0;
	virtual		int									DrawTitle(void* pRefDraw) = 0;
	virtual		short								GetKLineType() = 0;
	virtual		CWnd*								GetParentWnd() = 0;
	virtual		void								GetMaxMinData(HSDouble& dMax, HSDouble& dMin, HSDouble& dOpenPrice) = 0;
};