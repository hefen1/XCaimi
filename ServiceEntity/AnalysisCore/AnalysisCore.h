// AnalysisCore.h : AnalysisCore DLL ����ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������
#include "CombCacheImp.h"


// CAnalysisCoreApp
// �йش���ʵ�ֵ���Ϣ������� AnalysisCore.cpp
//

class CAnalysisCoreApp : public CWinApp
{
public:
	CAnalysisCoreApp();

// ��д
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};

CCombCacheSet* GetCombCacheSet();