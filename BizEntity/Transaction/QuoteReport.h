#pragma once

#include "../../Common/HSBizDialog.h"

// CQuoteReport �Ի���

class CQuoteReport : public CHSBizDialog
{
	DECLARE_DYNAMIC(CQuoteReport)

public:
	CQuoteReport(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CQuoteReport();

// �Ի�������
	enum { IDD = IDD_QUOTEREPORT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
};
