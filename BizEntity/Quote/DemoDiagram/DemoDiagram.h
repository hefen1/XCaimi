// DemoDiagram.h : DemoDiagram DLL ����ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CDemoDiagramApp
// �йش���ʵ�ֵ���Ϣ������� DemoDiagram.cpp
//

class CDemoDiagramApp : public CWinApp
{
public:
	CDemoDiagramApp();

// ��д
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
