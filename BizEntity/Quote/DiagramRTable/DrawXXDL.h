/*************************************************************
*	Copyright(c) 2010, ���ݺ����ɷ����޹�˾
*	All rights reserved.
*
*	�ļ����ƣ�	CDrawXXDL.h
*	�ļ���ʶ��
*	ժ	  Ҫ��  ��Ϣ����ͼԪ
*	
*	��ǰ�汾��
*	��	  �ߣ�	����
*	������ڣ�  2011-3-21
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
#include "DHtmlShowDlg.h"

#define XXDL_ROW_HEIGHT   20   // �и�
class CDrawXXDL :
	public CDrawTableBase
{
public:
	CDrawXXDL(IHsColor* pColor, IHsFont* pFont, IHsInfo* pInfo);
	~CDrawXXDL(void);

	void    CreateSelf();
	void    DelAllInfoIndex();           // ɾ������������Ϣ
	CString GetTitleString(InfoIndex* pInfoIndex); // ����Ϣ��������ȡ�����ı�
	void    RequestText(int nAyIndex);      // ��ȡ��Ϣ������Ѷ����
//interface
	virtual BOOL    DrawSelf(CDC* pDC, CPoint pos, int nItemID, CRect& rcItem);	
	virtual	BOOL	GetRequestData(IRequestCollector* pReqCollector,StockUserInfo* pStockInfo, BOOL bForce = FALSE);
	virtual int		OnUpdateData(DataHead* pData);
	virtual BOOL    MouseDBDown(UINT nFlags, CPoint point, hsRefMouseMsg* pData = NULL);
private:
	IHsColor*  m_pColor;         // ��ɫ�ӿ�ָ��
	IHsFont*   m_pFont;          // ����ӿ�ָ��
	IHsInfo*   m_pInfo;          // ��Ѷ�ӿ�ָ��
 
	CString    m_strInfoPath;    // ��ѯ·��
	int        m_nActiveID;      // ˫��ѡ�еĵ�Ԫ��
	long       m_lTextIndexCount;// ��Ϣ����������������
	BOOL       m_bRequst;        // �Ƿ��з�������

	CArray<InfoIndex*>  m_ayInfoIndex; // ��ѯ��������
	CDHtmlShowDlg*      m_pDlgShowHtml;// html�Ի���
};
