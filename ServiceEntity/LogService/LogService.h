// LogService.h : LogService DLL ����ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CLogServiceApp
// �йش���ʵ�ֵ���Ϣ������� LogService.cpp
//

class CLogServiceApp : public CWinApp
{
public:
	CLogServiceApp();

// ��д
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
