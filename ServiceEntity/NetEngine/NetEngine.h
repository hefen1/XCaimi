// NetEngine.h : NetEngine DLL ����ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������
#include "NetEngineJYAPI.h"
// CNetEngineApp
// �йش���ʵ�ֵ���Ϣ������� NetEngine.cpp
//

class CNetEngineApp : public CWinApp
{
public:
	CNetEngineApp();

// ��д
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()

};

