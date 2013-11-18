// DiagramRTable.h : DiagramRTable DLL ����ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������
#include "ConfigInterface.h"
#include "DataSourceDefineEx.h"


// CDiagramRTableApp
// �йش���ʵ�ֵ���Ϣ������� DiagramRTable.cpp
//

class CDiagramRTableApp : public CWinApp
{
public:
	CDiagramRTableApp();

// ��д
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
public:
	void   CreateInterface();
public:
	IHsInfo*  m_iInfo;
	IHsColor* m_iColor;
	IHsFont*  m_iFont;
	IDataSourceEx*    m_pDataSource;
	IHsUserDataCfg*   m_iUserDefCfg;    // �û����ݽӿڣ���Ʊ��ǣ�
};
