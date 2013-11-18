/*************************************************************
*	Copyright(c) 2010, ���ݺ����ɷ����޹�˾
*	All rights reserved.
*
*	�ļ����ƣ�	CDrawInfo.h
*	�ļ���ʶ��
*	ժ	  Ҫ��  ��ѯͼԪ
*	
*	��ǰ�汾��
*	��	  �ߣ�	����
*	������ڣ�  2011-3-17
*
*	��	  ע��
*	�޸ļ�¼��
*	��	  �ߣ�
*	�޸����ڣ�
*	�޸����ݣ�
*
****************************************************************/
#pragma once
#include "DrawTableBase.h"
#include "QuoteComm\InfoStruct.h"
#include "HSViewInfo.h"

#define INFO_COL_COUNT       10   // ����
#define INFO_LAST_COL_WIDTH  45   // ���һ�п��20
#define INFO_ROW_HEIGHT      30   // �и�

class CDrawInfo :
	public CDrawTableBase,
	public IViewInfoNotify
{
public:
	CDrawInfo(IHsColor* pColor, IHsInfo* pInfo);
	~CDrawInfo(void);

	void  CreateSelf();
	void  DelAllInfoIndex();           // ɾ������������Ϣ
	int   GetArrayIndexFromPoint(CPoint pos);    // �ӵ�ǰ��Ԫ�Ķ�ά��������Ѷ����������±�
	void  CalcImageRect(CRect* pRcIamge, CRect* pRcItem, BOOL bIsLeft = TRUE); // �ӵ�ǰ�����С�м����ͼ���С 

	void  RequestText(int nAyIndex);   // ��ȡ��Ѷ����
	// interface
	virtual BOOL    Create(IDrawBaseParam *pParam);
	virtual BOOL	SetRect(CRect rc);
	virtual BOOL    Draw(CDC* pDC);
	virtual BOOL    DrawSelf(CDC* pDC, CPoint pos, int nItemID, CRect& rcItem);	
	virtual	BOOL	GetRequestData(IRequestCollector* pReqCollector,StockUserInfo* pStockInfo, BOOL bForce = FALSE);
	virtual int		OnUpdateData(DataHead* pData);
	virtual BOOL	MouseDown(UINT nFlags, CPoint point, hsRefMouseMsg* pData = NULL);
	virtual BOOL    OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	virtual int	    OnKey(UINT nChar, UINT nRepCnt, UINT nFlags);
	//////////////////////////////////////////////////////////////////////////
	// ʵ���ı����ͼԪ����ص�����
	// ˢ�²���
	virtual void  ViewInfoInvalidate(CRect* pRect, BOOL bErase = TRUE);
	// ��ȡ�������
	virtual HWND  ViewInfoGetHandle();
private:
	IHsColor*     m_pColor;   // ��ɫ����ָ��
	IHsInfo*      m_pInfo;    // ��Ѷ����ָ��

	CodeInfo      m_preCodeInfo;   // ͼԪ֮ǰ�Ĺ�Ʊ��Ϣ   
	CString       m_strInfoPath;   // ��ѯ·��
	BOOL          m_bSizeMax;      // F10�Ƿ�ȫ����ʾ

	CArray<InfoIndex*>  m_ayInfoIndex; // ��ѯ��������
	CHSViewInfo*        m_pViewInfo;   // ���������ͼԪ����

	CImageList          m_lstImage;    // ͼ����Դ
	CRect               m_rcMaxScreen; // ȫ��Ŀ������
	CRect               m_rcClose;     // �ر�ͼ������
	CRect               m_rcUp;        // ����ͼ������
	CRect               m_rcDown;      // ����ͼ������

	static int           m_nActiveCellID;     // ��ǰѡ�еĵ�Ԫ�� 
};
