// DiagramHolder.h : DiagramHolder DLL ����ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������
#include "DiagramFactory.h"


// CDiagramHolderApp
// �йش���ʵ�ֵ���Ϣ������� DiagramHolder.cpp
//

class CDiagramHolderApp : public CWinApp
{
public:
	CDiagramHolderApp();

// ��д
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};

CDiagramFactory* GetDiagramfactory();
