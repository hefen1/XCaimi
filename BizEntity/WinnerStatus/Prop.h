#pragma once


#include "afxcmn.h"
#include <afxbutton.h>

// CProp �Ի���

class CProp : public CPropertyPage
{
	DECLARE_DYNAMIC(CProp)

public:
	CProp();
	virtual ~CProp();

// �Ի�������
	enum { IDD = IDD_PROP1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
//�¼�
	afx_msg void OnPaint();
	afx_msg void OnBnClickedBkclr();
	afx_msg void OnBnClickedFontclr();
	afx_msg void OnStnClickedFontkind();
	virtual BOOL OnInitDialog();
	afx_msg void OnNMCustomdrawSlider1(NMHDR *pNMHDR, LRESULT *pResult);
	virtual BOOL DestroyWindow();
//��������
public:
	void OnDrawLabel();
//��������
public:
	COLORREF m_bkClr;    //����������ɫ
	COLORREF m_fontClr;  //����������ɫ
	CFont    m_fontKind; //������������
	int      m_nSpeed;   //�����ٶ�
	int      m_clickWay; //��굥��ѡ����ʽ
private:

	CMFCButton m_BtnFontClr;	
	CMFCButton m_BtnBkClr;
	CSliderCtrl m_ctrlSlider;
	
};

