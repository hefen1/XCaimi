// QuoteLang.h : QuoteLang DLL ����ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CQuoteLangApp
// �йش���ʵ�ֵ���Ϣ������� QuoteLang.cpp
//

class CQuoteLangApp : public CWinApp
{
public:
	CQuoteLangApp();

// ��д
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
