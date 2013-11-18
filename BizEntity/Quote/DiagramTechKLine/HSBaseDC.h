/*******************************************************************************
 * Copyright (c)2003, ���ݺ�����Ϣ�������޹�˾
 * All rights reserved.
 *
 * �ļ����ƣ�HSBaseDC.h
 * �ļ���ʶ����ͼ�豸��װ��
 * ժ    Ҫ����ͼ�豸��װ��,�������������ô�ӡ����ɫ
 *
 * ��ǰ�汾��2.0
 * ��    �ߣ�����ʯ
 * ������ڣ�2003��05��09�� ������ 
 *
 * ȡ���汾��
 * ԭ �� �ߣ�����ʯ
 * ������ڣ�2003��05��09�� ������ 
 * ��	 ע��
 *******************************************************************************/
// HSBaseDC.h: interface for the CHSBaseDC class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_YLSBASEDC_H__81E4965B_0145_4D5B_90D1_277D327CDB99__INCLUDED_)
#define AFX_YLSBASEDC_H__81E4965B_0145_4D5B_90D1_277D327CDB99__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CHSBaseDC : public CDC
{
public:
	CHSBaseDC();
    CHSBaseDC(CDC* pDC);
    ~CHSBaseDC();

public:
    CHSBaseDC* operator->() { return this; }
    operator CHSBaseDC*()   { return this; }

protected:
	static COLORREF m_clPrintBKColor;	//��ӡ������ɫ
	static COLORREF m_clPrintTextColor;	//��ӡ�ı���ɫ
	static CPen     m_pPrintPen;		//��ӡ����
	static CBrush	m_pPrintBrush;		//��ӡ��ˢ

protected:
	char	m_bSelf;		//��ͼ�豸ָ���Ƿ�Ϊ�ڲ�������
	/*************************************************************
	* �����¼���
	* ʱ�䣺  2003��9��12��
	* ������  Ϊ��֧�ֲ�ӡ
	*************************************************************/
	// �Ƿ�֧�ֲ�ɫ��ӡ
	BOOL	m_bSupportMulticolor; 
	/**********�����¼������*************/

public:
	//��ʼ������,���ô�ӡ�����뻭ˢ
	static BOOL Init();

public:
	//�����ı���ɫ�뱳����ɫ
	virtual COLORREF SetTextColor(COLORREF crColor);
	virtual COLORREF SetBkColor(COLORREF crColor);

public:
	//�豸ѡ�����
	virtual CGdiObject* SelectStockObject(int nIndex);
	CPen*			    SelectObject(CPen* pPen);
	CBrush*			    SelectObject(CBrush* pBrush);
	virtual CFont*	    SelectObject(CFont* pFont);
	CBitmap*		    SelectObject(CBitmap* pBitmap);
	int					SelectObject(CRgn* pRgn);
	CGdiObject*			SelectObject(CGdiObject* pObject);
	HGDIOBJ				SelectObject(HGDIOBJ);
	//������
	void FillRect(LPCRECT lpRect, CBrush* pBrush);
	void FillSolidRect(LPCRECT lpRect, COLORREF clr);
	void FillSolidRect(int x, int y, int cx, int cy, COLORREF clr);
	BOOL FloodFill(int x, int y, COLORREF crColor);
	BOOL FillRgn(CRgn* pRgn, CBrush* pBrush);


	/*************************************************************
	* �����¼���
	* ʱ�䣺  2003��9��12��
	* ������  Ϊ��֧�ֲ�ӡ
	*************************************************************/
	// �Ƿ�Ϊ�ڰ״�ӡ
	BOOL IsMonochromePrint() { return m_bPrinting && !m_bSupportMulticolor; }
	/**********�����¼������*************/
};

#endif // !defined(AFX_YLSBASEDC_H__81E4965B_0145_4D5B_90D1_277D327CDB99__INCLUDED_)
