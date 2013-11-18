/*************************************************************
*	Copyright(c) 2010, 杭州恒生信息技术有限公司
*	All rights reserved.
*
*	文件名称：	TechKlineMgr.h
*	文件标识：
*	摘	  要：  技术分析图元接口实现(管理层)
*	
*	当前版本：
*	作	  者：	蒋国宏
*	完成日期：  2010-12-1
*
*	备	  注： 管理层实现统一的数据请求、坐标变换等等
*	修改记录：
*	作	  者：
*	修改日期：
*	修改内容：
*
****************************************************************/
#pragma once
#include "ManagerBase.h"
#include "DialogHisTrend.h"
#include "DrawBox.h"
#include "DrawBottom.h"
#include "TechKLineTab.h"
#include "DrawInfo.h"
#include "DrawUnitPositonCtrl.h"
#include "..\..\..\Resource\QuoteLang\Resource.h"
#include "..\..\..\Common\QuoteLangMngInterFace.h"

enum eTechLineOperator
{
	eTechLineOperator_mainDel		= 0x0001,		//主图删除
	eTechLineOperator_subDel		= 0x0002,		//附图删除
	eTechLineOperator_mainAddnew	= 0x0010,		//主图增加新内容
	eTechLineOperator_subAddnew		= 0x0020,		//附图增加
	eTechLineOperator_mainModify	= 0x0100,		//主图原有修改
	eTechLineOperator_subModify		= 0x0200,		//附图原有修改
};

class CTechKLineMgr : public CDrawBase, public CManagerBase
{
//构造
public:
	CTechKLineMgr();
	~CTechKLineMgr();
protected:
//成员

//	std::list<StockCompDayDataEx*>	m_listData;

	int								m_nRequestPeriod;		//请求数据大小
	std::list<CTechKLineImp*>		m_listKLine;			//K线图元集合
	std::list<CTechKLineImp*>		m_listFenshi;			//分时图元集合
	CDrawUnitPosCtrlMgr*			m_pPostionCtrlMgr;		//位置控制器

	short				m_nKLineType;				//主图类型
	IDataSourceEx*		m_pDataSourceEx;
	IBlockManager*		m_pBlockManager;			// 板块管理
	IQuoteLang*			m_pQuoteLang;				//	资源
//	CImageList			m_imageList;

	//鼠标位置
	CPoint				m_ptMouseFocus;					//鼠标
	CPoint				m_ptCursorLine;					//十字光标
	//信息显示
	CDrawBox*			m_pDrawBox;
	//技术分析中的底部
	CDrawBottom*		m_pDrawBottom;
	//信息地雷
	CDrawInfo*			m_pDrawInfo;

	MouseTipTextInfo	m_sMouseTipTextInfo;			//提示信息
	int					m_nSelDataIndex;
	CurrentFinanceData	m_curFinance;	
	long				m_lFinaceHand;
	WORD				m_wPeriodType;

	CDialogHisTrend*	m_pDialogHisTrend;
	CTechKLineTab		m_bottomTab;					//TAB页签
	CTechKLineTabNotify	m_tabNotify;					//TAB消息反馈

	CString				m_strAppName;
	CString				m_strYPerMsg;
	CString				m_strPosChangeMsg;				//
	BOOL				m_bPosChanged;					//图元位置变更标志
	CArray<ChuQuanData*,ChuQuanData*>	m_ayChuQuanData;		//除权数据

	int					m_nDelayDeleteTimerID;
	CTechKLineImp*		m_pTechLineDelayDelete;
//interface:
public:
	virtual BOOL		Create(IDrawBaseParam *pParam);
	virtual void		SetRegionNode(IRegionNode* pNode);
	virtual BOOL		SetRect(CRect rc);
	virtual CRect		GetRect();
	virtual void		SetVisible(BOOL bVisible);
	virtual BOOL		GetVisible();
	virtual void		UpdateCodeInfo(StockUserInfo* pCodeInfoNew);
	virtual StockUserInfo*	GetCodeInfo();
	virtual short		GetDrawType();
	virtual BOOL		MouseDown(UINT nFlags, CPoint point, hsRefMouseMsg* pData = NULL);
	virtual BOOL		MouseDBDown(UINT nFlags, CPoint point, hsRefMouseMsg* pData = NULL);
	virtual BOOL		MouseUp(UINT nFlags, CPoint point, hsRefMouseMsg* pData = NULL);
	virtual BOOL		MouseMove(UINT nFlags, CPoint point, hsRefMouseMsg* pData = NULL);
	virtual BOOL		RButtonDown(UINT nFlags,CPoint point, HWND hWndParent, hsRefMouseMsg* pData = NULL);
	virtual BOOL		RButtonUp(UINT nFlags,CPoint point, HWND hWndParent, hsRefMouseMsg* pData = NULL);
	virtual BOOL		OnCursor(UINT nFlags, CPoint point);
	virtual int			OnKey(UINT nChar, UINT nRepCnt, UINT nFlags);
	virtual BOOL		OnKeyBoardNotify(WPARAM wParam, LPARAM lParam);

	virtual long		OnMenuMsg(WPARAM wParam, LPARAM lParam);
	virtual int			Draw(CDC* pDC);
	virtual BOOL		GetRequestData(IRequestCollector* pReqCollector,StockUserInfo* pStockInfo, BOOL bForce);
	virtual int			OnUpdateData(DataHead* pData);
	virtual void		OnLayoutNotify(UINT nMsgId,IDrawBaseParam* pParam);
	virtual int			Delete();

//CManagerBase interface
//	virtual int									GetStockCompDayDataEx(StockCompDayDataEx*& pStockCompDayData);
	virtual	int									GetStockDayData(StockDay*& pStockDaya);
	virtual StockUserInfo*						GetStockUserInfo();
	virtual void								GetTopMaxMinData(HSDouble& dMax, HSDouble& dMin, HSDouble& dOpenPrice);
	virtual int									DrawTitle(void* pRefDraw);
	virtual short								GetKLineType();
	virtual CWnd*								GetParentWnd();
	/*获取当前的图元集合*/
	std::list<CTechKLineImp*>*					GetCurrentTechLines();
	/*获取主图上的最大最小值*/
	void										GetMaxMinData(HSDouble& dMax, HSDouble& dMin, HSDouble& dOpenPrice);
	virtual HWND								GetParentHWnd();
	virtual int									DelayDeleteTechLine(CTechKLineImp* pTechLine);

//属性
	void								SetChildRect();
	void								ReCreate(IDrawBaseParam *pParam);
	
	void								DrawCursorLine(CDC* pDC);
	int									GetKLineNextTime(int nIndex, char* pszTime, WORD wPeriodType);
	int									GetDataXPos(int nDataIndex);
	CString								FormatTip(int nDataIndex, WORD wPeriodType);
	int									HitTest(CPoint point);
	int									SwitchPage(WORD wPageType);
	int									ChangeType();

	/*获取下一个周期类型*/
	WORD								GetNextPageType(BOOL bNext);
	/*实时发送请求*/
	int									SendRealTimeRequest();							
	/*获取顶点子图元*/
	CTechKLineImp*						GetTopLine();
	/*获取底部子图元*/
	CTechKLineImp*						GetBottomLine();
	/*切换起始坐标*/
	int									MovePosition(CPoint ptMove);
	/*以当前为基准，移动十字光标位置*/
	int									MoveCursorPos(BOOL bNext);				
	/*显示历史回忆*/
	int									ShowHisTrend();
	/*刷新*/
	void								DiagramInValidate(CRect rcInvalidate = CRect(0, 0, 0, 0));
	/*获取原先的叠加线*/
	int									GetAddLines(std::list<CTechKLineImp*>&listKLine, short nType);
	/*指标线操作*/
	int									ModifyTechLine(ExpPropery* pExp, BOOL bDelOld = TRUE);
	/*附加（股票）代码操作*/
	int									ModifyTechLine(StockUserInfo* pStock, BOOL bDelOld = TRUE);
	/*主图叠加线数据接收*/
	int									UpdateAddLinesData(DataHead* pData);
	/*tab项改变*/
	int									OnTabItemChanged(TechIndex* pTechIndex);
	int									OnTabItemChanged(TechTemplete* pTechTemplete);
	/*二三四六九图间的切换*/
	int									SwitchMultiDiagram(CStringArray& ayStrtemplete, float fRadio);
	/*信息地雷上需要的日期坐标转换值*/
	int									UpdateDrawInfoData(CArray<CDrawInfoUnitData>& ayData, long lDateLast);
	/*删除指定线*/
	int									DeleteLine(CTechKLineImp* pImpDel = NULL);
	/*除权复权处理*/
	int									CalcChuquanMark(int nMenuID);
	int									ClearOldChuquanData();
	CArray<ChuQuanData*,ChuQuanData*>*	GetChuquanData();

	static VOID CALLBACK				DelayDelete(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime); 
};