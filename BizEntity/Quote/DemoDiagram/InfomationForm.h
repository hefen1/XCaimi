#pragma once

#include "Resource.h"

// CInfomationForm �Ի���

class CInfomationForm : public CDialog
{
	DECLARE_DYNAMIC(CInfomationForm)

public:
	CInfomationForm(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CInfomationForm();

// �Ի�������
	enum { IDD = IDD_FORMVIEW };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
};
