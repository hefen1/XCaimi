// QuoteGeneralSort.h : QuoteGeneralSort DLL ����ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CQuoteSumGeneralSortApp
// �йش���ʵ�ֵ���Ϣ������� QuoteSumGeneralSort.cpp
//

class CQuoteGeneralSortApp : public CWinApp
{
public:
	CQuoteGeneralSortApp();

// ��д
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
