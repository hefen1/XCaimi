/*************************************************************
*	Copyright(c) 2010, 杭州恒生信息技术有限公司
*	All rights reserved.
*
*	文件名称：	QuoteTableCtrlGeneralSort.h
*	文件标识：
*	摘	  要：	行情综合排名表类
*	
*	当前版本：
*	作	  者：	
*	完成日期：
*
*	备	  注：
*	修改记录：
*	作	  者：
*	修改日期：
*	修改内容：
****************************************************************/
#ifndef  QUOTEGeneralSortTABLECTRL
#define  QUOTEGeneralSortTABLECTRL


#include "HSGridTable.h"
#include "ConfigInterface.h"
#pragma once

struct InitTableData
{
	DWORD m_dStyle;
	int   m_nRowCount;
	int   m_nColCount;
	int   m_nFixRowCount;
	int   m_nFixColCount;
	void* m_pData;

	CString m_strGroupName;

	InitTableData()
	{
		//m_dStyle = INIT_FIXED_HEAD;
		m_nRowCount = 1;
		m_nColCount = 3;
		m_nFixRowCount = 1;
		m_nFixColCount = 3;
		m_pData = NULL;
	}
	BOOL IsStyle(DWORD dStyle) {return (m_dStyle & dStyle);}
};

class QuoteTableCtrlGeneralSort: public CHSGridCtrl
{
	DECLARE_DYNCREATE(QuoteTableCtrlGeneralSort)
	struct TableFlag
	{
		char*   strTitle;//标题
		int		  nIndex;//索引
		short	  nType; //类型
		CWnd*     pTable;//Table对象
	};
	

public:
	QuoteTableCtrlGeneralSort();
	virtual ~QuoteTableCtrlGeneralSort();

protected:

	DECLARE_MESSAGE_MAP()
public:
	//消息处理
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
//	virtual void  OnDraw(CDC* pDC);

	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnTimer(UINT nIDEvent);
	virtual void OnDraw(CDC* pDC);
	
public:

	//请求数据包
	virtual char* GetRequestData(int index, int&, int);
	//处理接受的数据
	BOOL HandleRecvData(AnsGeneralSortEx* pData, int nIndex);

	//处理主推数据
	void HandleAutoPushData();

	void AddStock(StockUserInfo* pStock);
	void AddColumn(UINT nID);
	void DeleteStock();
	void RefreshWnd();

	void DrawTitle();
	
	//初始化行列
	virtual void  InitRowData();
	virtual void  InitColData();
	virtual void  InitFixedDataItem(int nFirst,int nCount);
	virtual void  SetFixedData(int nRow,int nCol,CGridCellBase* pCell, StockUserInfo* pStock = NULL,
		int nNumber = -1, BOOL bRefresh = TRUE, CDC* pDC = NULL);

	int			  GetColIndexByID(UINT uiID);
	UINT GetHeadID(int nPos);

	void SetFlag(QuoteTableCtrlGeneralSort::TableFlag* pTableFlag);

private:
	TableFlag*								m_pTableFlag;				//请求表格类型
	CArray<StockUserInfo*,StockUserInfo*>   m_pStock;                   //股票信息
	CUIntArray		                        m_ayColInfo;				// 保存列ID
	short									m_sDisplayRow;               //显示总行数
	CPoint									m_LastDownPoint;
	IHsColumnProperty*						m_iColProp;                          //列属性
	IHsColor*							    m_iColorFontProt;	                 //颜色配置 
	IHsFont*								m_iFont;                           //字体配置
};
#endif