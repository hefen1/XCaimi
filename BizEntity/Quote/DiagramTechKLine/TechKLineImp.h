/*************************************************************
*	Copyright(c) 2010, ���ݺ�����Ϣ�������޹�˾
*	All rights reserved.
*
*	�ļ����ƣ�	TechKlineImp.h
*	�ļ���ʶ��
*	ժ	  Ҫ��  ��������ͼԪ�ӿ�ʵ��
*	
*	��ǰ�汾��
*	��	  �ߣ�	������
*	������ڣ�  2010-12-1
*
*	��	  ע��
*	�޸ļ�¼��
*	��	  �ߣ�
*	�޸����ڣ�
*	�޸����ݣ�
*
****************************************************************/
#pragma once

#define SPLIT_COLUMN  2		//ÿ�������ϵļ��
#include "ManagerBase.h"

enum etechLineType
{
	etechLineType_main		= 0x01,		//��ͼ
	etechLineType_tech		= 0x02,		//ָ��
	etechLineType_stock		= 0x04,		//���Ӵ��루��Ʊ��ָ����

	etechLineType_all		= 0x0f,		//����		
};

class CTechKLineImp : public CDrawBase
{
//����
public:
	CTechKLineImp(CManagerBase* pMgr, IRegionNode* pRegion, float fHorizCellWidth);
	virtual ~CTechKLineImp();
protected:
//��Ա
	CManagerBase*		m_pTeckKLineMgr;

	CRect				m_rcGird;			//����ߴ�Լλ��
	int					m_nGridNum;			//�����Ŀ��
	int					m_nHoriPoints;			//����, ���ݸ���, һ���ʱͼ��Ϊ241��
	int					m_nHoriCellNum;			//����, С��Ԫ����, һ���ʱͼ��Ϊ241��
	int					m_nMaxHoriCellNum;
	float				m_fHoriCellWidth;		//����Ԫ����
	float				m_fKLineWidth;			//K�߿��
	CRect				m_rcVarTitle;			//��������

	CHSLinear<float>*						m_pLinear;			// ��ͼ��
	DrawPointInfo*							m_pPointPos;		//ָ��������ݽṹ
	CKLineChartParam						m_sChartParam;		//K�߲���
	BOOL									m_bFenshiDrawType;	//��ͼ�Ƿ�Ϊ��ʱͼ��
	BOOL									m_bAddLine;			//����ͼ
	BOOL									m_bBottomLine;		//�ײ�ͼ
	DWORD									m_dwLinearStyle;

protected:	
	CDispMaxMinValue					m_lCurMinValue;	//��Сֵ���ݽṹ
	CDispMaxMinValue					m_lCurMaxValue; //���ֵ���ݽṹ

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
	virtual long		OnMenuMsg(WPARAM wParam, LPARAM lParam);
	virtual int			Draw(CDC* pDC);
	virtual BOOL		GetRequestData(IRequestCollector* pReqCollector,StockUserInfo* pStockInfo, BOOL bForce);
	virtual int			OnUpdateData(DataHead* pData);
	virtual int			Delete();
	virtual	short		GetLineType();
	virtual void		SetTeckKLineName(CString strName);
	virtual CString		GetTeckKLineName();

//����:
	CHSLinear<float>*	GetLinear() {return m_pLinear;}
	virtual void		GetMaxMinData(HSDouble& dMax, HSDouble& dMin, HSDouble& dOpenPrice);
	virtual BOOL		IsUp(int nPosition);
	virtual void		SetFenshiDrawType(BOOL bType);
	virtual BOOL		IsFenshiDrawType();
	virtual CManagerBase*		GetTechLineMgr();
	/*�Ƿ�Ϊ��ͼ*/
	virtual BOOL		IsMainLine();
	/*��ȡ����ͼ*/
	virtual CRect		GetMarginRect();
	virtual void		SetHorizCellWidth(float fNewCellWidth);
	virtual float		GetHorizCellWidth();
	virtual void		SetHorizCellNum(int nHorizCellNum);
	virtual int			GetHorizCellNum();
	virtual int			GetMaxHorizCellNum();
	virtual	void		SetAddLine(BOOL bAddLine);
	virtual	BOOL		IsAddLine();
	virtual BOOL		GetVarTitle(CArray< CToptextType<double>*,CToptextType<double>* >& ayTitle, int nDataIndex);
	virtual int			GetDataXPos(int nDataIndex);
	virtual int			GetDataYPos(int nDataIndex);
	virtual int			GetInMarginYPos(int nSourceYPos);
	virtual int			HitTest(CPoint pt);
	virtual BOOL		GetTipText(MouseTipTextInfo* pMouseTipTextInfo,int nPosition,CPoint& point);
	virtual void		SetIsBottom(BOOL bIsBottom);
	virtual BOOL		GetIsBottom();
	virtual DWORD		ModifyLinearStyle(DWORD dwAddStyle, DWORD dwRemoveStyle);
	virtual void		SetVarTitleRect(CRect rc, BOOL bAdd = FALSE);
	virtual CRect		GetVarTitleRect();
	void                SetChartType(int nType) { m_sChartParam.m_dwType = nType;}
	CDispMaxMinValue*	GetCurMaxValue();
	CDispMaxMinValue*	GetCurMinValue();
};

