#pragma once
#include "resource.h"
#include "afxdtctl.h"
#include "afxwin.h"

// CDlgJYStrategy �Ի���

class CDlgJYStrategy : public CDialog
{
	DECLARE_DYNAMIC(CDlgJYStrategy)

public:
	CDlgJYStrategy(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgJYStrategy();

// �Ի�������
	enum { IDD = 0 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
};
