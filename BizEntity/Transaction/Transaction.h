// Transaction.h : Transaction DLL ����ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// ������
#include "..\..\Common\HSWinner.h"


// CTransactionApp
// �йش���ʵ�ֵ���Ϣ������� Transaction.cpp
//

class CTransactionApp : public CWinApp
{
public:
	CTransactionApp();
// ��д
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
