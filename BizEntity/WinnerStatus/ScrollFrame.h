#pragma once

#include "ScrollNewsBar.h"

// CScrollFrame �Ի���

class CScrollFrame : public CDialog
{
	DECLARE_DYNAMIC(CScrollFrame)

public:
	CScrollFrame(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CScrollFrame();

// �Ի�������
	enum { IDD = IDD_SCROLLFRAME };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
//�¼�
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI);
	afx_msg LRESULT OnNcHitTest(CPoint point);
	afx_msg void OnNcMouseMove(UINT nHitTest, CPoint point);
//���ú���
public:
	CRect getPosition();
//����
private:
	//CRect m_rectScrollFloat;       //������Ʋ��ǹ̶���״̬���� ��¼�ڴ��ڵ�λ�ã���С
public:
	CScrollNewsBar* m_pScrollWnd;  //��¼�����ָ��


};

