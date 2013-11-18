// DiagramCombAnalysis.h : DiagramCombAnalysis DLL ����ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������
#include "CombComm/CombCache.h"

// CDiagramCombAnalysisApp
// �йش���ʵ�ֵ���Ϣ������� DiagramCombAnalysis.cpp
//

class CDiagramCombAnalysisApp : public CWinApp
{
public:
	CDiagramCombAnalysisApp();

// ��д
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};

ICombCacheSet* GetCombCacheSet();