// KeyBoard.h : KeyBoard DLL ����ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CKeyBoardApp
// �йش���ʵ�ֵ���Ϣ������� KeyBoard.cpp
//

class CKeyBoardApp : public CWinApp
{
public:
	CKeyBoardApp();

// ��д
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
