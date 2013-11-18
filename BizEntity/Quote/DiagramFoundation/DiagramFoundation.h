// DiagramFoundation.h : DiagramFoundation DLL ����ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������
#include "..\..\..\Common\ConfigInterface.h"
#include "..\..\..\Common\DataSourceDefineEx.h"


// CDiagramFoundationApp
// �йش���ʵ�ֵ���Ϣ������� DiagramFoundation.cpp
//

class CDiagramFoundationApp : public CWinApp
{
public:
	CDiagramFoundationApp();

// ��д
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()

public:
	IHsColor* m_iColor;
	IHsFont*  m_iFont;
	IDataSourceEx* m_pDataSource;
};
