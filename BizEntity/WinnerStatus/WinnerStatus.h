// WinnerStatus.h : WinnerStatus DLL ����ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������
#include "..\..\Common\HSWinner.h"

// CWinnerStatusApp
// �йش���ʵ�ֵ���Ϣ������� WinnerStatus.cpp
//

class CWinnerStatusApp : public CWinApp
{
public:
	CWinnerStatusApp();
	IMainWnd *m_pMain;
// ��д
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
