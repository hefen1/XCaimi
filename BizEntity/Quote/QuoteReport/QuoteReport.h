// QuoteReport.h : QuoteReport DLL ����ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������
#include "..\..\..\Resource\QuoteLang\Resource.h"


#define QuoteReportLoggerName _T("quotereport")

// CQuoteReportApp
// �йش���ʵ�ֵ���Ϣ������� QuoteReport.cpp
//
#define IDD_DYNAMIC_CREATE 100


class CQuoteReportApp : public CWinApp
{
public:
	CQuoteReportApp();

// ��д
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
