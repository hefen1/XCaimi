/*************************************************************
*	Copyright(c) 2010, ���ݺ�����Ϣ�������޹�˾
*	All rights reserved.
*
*	�ļ����ƣ�	QuoteTableCtrlGeneralSort.h
*	�ļ���ʶ��
*	ժ	  Ҫ��	�����ۺ���������
*	
*	��ǰ�汾��
*	��	  �ߣ�	
*	������ڣ�
*
*	��	  ע��
*	�޸ļ�¼��
*	��	  �ߣ�
*	�޸����ڣ�
*	�޸����ݣ�
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
		char*   strTitle;//����
		int		  nIndex;//����
		short	  nType; //����
		CWnd*     pTable;//Table����
	};
	

public:
	QuoteTableCtrlGeneralSort();
	virtual ~QuoteTableCtrlGeneralSort();

protected:

	DECLARE_MESSAGE_MAP()
public:
	//��Ϣ����
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
//	virtual void  OnDraw(CDC* pDC);

	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnTimer(UINT nIDEvent);
	virtual void OnDraw(CDC* pDC);
	
public:

	//�������ݰ�
	virtual char* GetRequestData(int index, int&, int);
	//������ܵ�����
	BOOL HandleRecvData(AnsGeneralSortEx* pData, int nIndex);

	//������������
	void HandleAutoPushData();

	void AddStock(StockUserInfo* pStock);
	void AddColumn(UINT nID);
	void DeleteStock();
	void RefreshWnd();

	void DrawTitle();
	
	//��ʼ������
	virtual void  InitRowData();
	virtual void  InitColData();
	virtual void  InitFixedDataItem(int nFirst,int nCount);
	virtual void  SetFixedData(int nRow,int nCol,CGridCellBase* pCell, StockUserInfo* pStock = NULL,
		int nNumber = -1, BOOL bRefresh = TRUE, CDC* pDC = NULL);

	int			  GetColIndexByID(UINT uiID);
	UINT GetHeadID(int nPos);

	void SetFlag(QuoteTableCtrlGeneralSort::TableFlag* pTableFlag);

private:
	TableFlag*								m_pTableFlag;				//����������
	CArray<StockUserInfo*,StockUserInfo*>   m_pStock;                   //��Ʊ��Ϣ
	CUIntArray		                        m_ayColInfo;				// ������ID
	short									m_sDisplayRow;               //��ʾ������
	CPoint									m_LastDownPoint;
	IHsColumnProperty*						m_iColProp;                          //������
	IHsColor*							    m_iColorFontProt;	                 //��ɫ���� 
	IHsFont*								m_iFont;                           //��������
};
#endif