#pragma once
/*************************************************************
*	Copyright(c) 2010, ���ݺ�����Ϣ�������޹�˾
*	All rights reserved.
*
*	�ļ����ƣ�	DrawInfo.h
*	�ļ���ʶ��
*	ժ	  Ҫ��  ��Ϣ������ʾ
*	
*	��ǰ�汾��
*	��	  �ߣ�	������
*	������ڣ�  2010-5-6
*
*	��	  ע��
*	�޸ļ�¼��
*	��	  �ߣ�
*	�޸����ڣ�
*	�޸����ݣ�
*
****************************************************************/
#include "..\..\..\Common\QuoteComm\DrawBase.h"
#include "..\..\..\Common\ConfigInterface.h"
#include "..\DiagramFoundation\DiagramComm.h"
#include "..\DiagramFoundation\AnalysisTextData.h"
#include "..\DiagramFoundation\DHtmlShowDlg.h"

class CTechKLineMgr;

struct CDrawInfoUnitData
{
	long	m_lDateTime;				//����ʱ��
	CPoint	m_ptPosition;				//��Ϣ���׶�Ӧ�������
	CPoint	m_ptChuquan;				//��Ȩ���Ӧ������
};

struct InfoIndexEx : public InfoIndex
{
	BOOL	m_bIsShow;
	CRect	m_rect;
	BOOL	m_bSelected;

	InfoIndexEx();
	BOOL	Copy(InfoIndex* pInfo);
	void	SetRect(CRect rc = CRect(0, 0, 0, 0));
};

class CDrawInfo : public CDrawBase
{
protected:
	CTechKLineMgr*				m_pMgr;
	CString						m_strInfoPath;
	IHsInfo*					m_pInfo;
	CArray<InfoIndexEx*>		m_ayInfoIndex;	// ��ѯ��������
//	CArray<InfoIndexEx*>		m_ayInfoChuquan;// ��Ȩ��������
	CArray<CDrawInfoUnitData>	m_ayUnitData;	//��ͼ����
	int							m_nSelInfoIndex;	//ѡ�еĶ���
	CRect						m_rcInvalidateOld;	//ԭ�е�ˢ����
	int							m_nTipTimeID;		//��ʱˢ��ID
	CDHtmlShowDlg*				m_pDlgShowHtml;		// html�Ի���
	CArray<InfoIndexEx*>		m_ayInfoIndexSelect;// ��ѯ��������:ѡ�еĲ���
	int							m_nItemHeight;
public:
	CDrawInfo(CTechKLineMgr* pMgr, IHsInfo* pInfo, IRegionNode* pNode);
	virtual ~CDrawInfo();

//method:
	virtual BOOL		GetRequestData(IRequestCollector* pReqCollector,StockUserInfo* pStockInfo, BOOL bForce);
	virtual int			OnUpdateData(DataHead* pData);
	virtual BOOL		MouseDown(UINT nFlags, CPoint point, hsRefMouseMsg* pData = NULL);
	virtual BOOL		MouseMove(UINT nFlags, CPoint point, hsRefMouseMsg* pData = NULL);

	virtual int			Draw(CDC* pDC);
	int					EmptyData();
	void				KillTipTimer();
	void				RequestText(int nSelIndex);
	int					ResetSelectInfoIndex();
	int					DrawInfo(CDC* pDC);
	int					EmptyChuquanData();
	
	static VOID CALLBACK    ShowTipProc(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime);  // ��ʾTip     
};