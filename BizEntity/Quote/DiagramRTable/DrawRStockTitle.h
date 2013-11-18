/*************************************************************
*	Copyright(c) 2010, ���ݺ����ɷ����޹�˾
*	All rights reserved.
*
*	�ļ����ƣ�	CDrawRTableBase.h
*	�ļ���ʶ��
*	ժ	  Ҫ��  ���Ʊ���
*	
*	��ǰ�汾��
*	��	  �ߣ�	����
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
#include "DrawTableBase.h"
#include "ConfigInterface.h"


class CDrawRStockTitle : public CDrawTableBase
{
public:
	CDrawRStockTitle(IHsUserDataCfg* pUserDefCfg, IHsColor* pColor, IHsFont* pFont);
	virtual ~CDrawRStockTitle();
	
	void ReadFile(); // �������ļ�

	virtual int	 DrawSelf(CDC* pDC, CPoint pos, int nItemID, CRect &rcItem);
	virtual CString GetTipText(int nCellID, int nItemID);
	virtual	BOOL GetRequestData(IRequestCollector* pReqCollector,StockUserInfo* pStockInfo, BOOL bForce = FALSE);

private:
	COLORREF m_clrBK;        // ����ɫ
	COLORREF m_clrCode;      // ��Ʊ������ɫ
	COLORREF m_clrCodeName;  // ��Ʊ������ɫ
	CFont* m_pFontCode;      // ��������
	CFont* m_pFontCodeName;  // ��������

	IHsUserDataCfg*   m_iUserDefCfg;    // �û����ݽӿڣ���Ʊ��ǣ�
	IHsColor*         m_pColor;         // ��ɫ����
	IHsFont*          m_pFont;          // ��������
	CImageList*       m_pImageList;     // ��Ʊ���ͼ��
};

