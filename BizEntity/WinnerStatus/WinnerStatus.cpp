// WinnerStatus.cpp : ���� DLL �ĳ�ʼ�����̡�
//

#include "stdafx.h"
#include "WinnerStatus.h"

#include "StatuBar.h"

#include "..\..\Common\HSWinner.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


//
//TODO: ����� DLL ����� MFC DLL �Ƕ�̬���ӵģ�
//		��Ӵ� DLL �������κε���
//		MFC �ĺ������뽫 AFX_MANAGE_STATE ����ӵ�
//		�ú�������ǰ�档
//
//		����:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// �˴�Ϊ��ͨ������
//		}
//
//		�˺������κ� MFC ����
//		������ÿ��������ʮ����Ҫ������ζ��
//		��������Ϊ�����еĵ�һ�����
//		���֣������������ж������������
//		������Ϊ���ǵĹ��캯���������� MFC
//		DLL ���á�
//
//		�й�������ϸ��Ϣ��
//		����� MFC ����˵�� 33 �� 58��
//

// CWinnerStatusApp

BEGIN_MESSAGE_MAP(CWinnerStatusApp, CWinApp)
END_MESSAGE_MAP()


// CWinnerStatusApp ����

CWinnerStatusApp::CWinnerStatusApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CWinnerStatusApp ����

CWinnerStatusApp theApp;


// CWinnerStatusApp ��ʼ��

BOOL CWinnerStatusApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}

int PASCAL InitEntity()
{	
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return TRUE;
}

//�ͷŲ��
int PASCAL UninitEntity()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return TRUE;
}

IBizWindow* PASCAL GetEntityInstance(const char* name, HWND  hOwner)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	if(name == NULL)
		return NULL;

	IBizWindow *pBizWnd = NULL;
	CWnd *pParentWnd = NULL;
	if(::IsWindow(hOwner))
		pParentWnd = CWnd::FromHandle(hOwner);
	if(_tcscmp(name,_T("WndStatusBar")) == 0)
	{
		CStatuBar* pStatusDlg  = new CStatuBar();
		pStatusDlg->Create(CStatuBar::IDD,pParentWnd);
		if (pStatusDlg == NULL || ::IsBadReadPtr(pStatusDlg, 1) || !::IsWindow(pStatusDlg->m_hWnd))
		{
			return NULL;
		}
		pStatusDlg->ShowWindow(SW_SHOW);
		pBizWnd = (IBizWindow*)pStatusDlg;
	}

	return pBizWnd;
}