/*************************************************************
*	Copyright(c) 2010, ���ݺ����ɷ����޹�˾
*	All rights reserved.
*
*	�ļ����ƣ�	CDlgInfoView.h
*	�ļ���ʶ��
*	ժ	  Ҫ��  ��ʾ��Ϣ�������ݶԻ���
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
#include "Resource.h"
#include "HSHtmlView.h"

// CDlgInfoView �Ի���

class CDlgInfoView : public CDialog
{
	DECLARE_DYNAMIC(CDlgInfoView)

public:
	CDlgInfoView(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgInfoView();

// �Ի�������
	enum { IDD = IDD_SHOWINFO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
public:
	void  SetUrlString(CString str);   // ����Ҫ�򿪵�Url��ַ

private:
	CHSHtmlView*   m_pInfoView;
	CString        m_strUrl;
};
