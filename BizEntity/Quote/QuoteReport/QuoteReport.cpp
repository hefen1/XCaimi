// QuoteReport.cpp : ���� DLL �ĳ�ʼ�����̡�
//

#include "stdafx.h"
#include "QuoteReport.h"
#include "QuoteReportDlg.h"
#include "WinnerApplication.h"
#include "BlockAdscriptionDlg.h"

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

// CQuoteReportApp

BEGIN_MESSAGE_MAP(CQuoteReportApp, CWinApp)
END_MESSAGE_MAP()


// CQuoteReportApp ����

CQuoteReportApp::CQuoteReportApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CQuoteReportApp ����

CQuoteReportApp theApp;


// CQuoteReportApp ��ʼ��

BOOL CQuoteReportApp::InitInstance()
{
	CWinApp::InitInstance();

	HINSTANCE hRes = (HINSTANCE)CWinnerApplication::GetModuleHandle(_T("QuoteLang.dll"));
	if (hRes)
	{
		CWinnerApplication::FormatLog(INFO_LOG_LEVEL,QuoteReportLoggerName,"���鱨��ģ��-������Դ���Ϊ��%x",hRes);
		AfxSetResourceHandle(hRes);
	}
	return TRUE;
}

int PASCAL InitEntity()
{
	
	return TRUE;
}

//�ͷŲ��
int PASCAL UninitEntity()
{
	return TRUE;
}

//
IBizWindow* PASCAL GetEntityInstance(const char* name, HWND  hOwner)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	if(name == NULL)
		return NULL;

	IBizWindow* pWnd = NULL;
	CWnd *pParentWnd = NULL;
	if(::IsWindow(hOwner))
		pParentWnd = CWnd::FromHandle(hOwner);
	if(_tcscmp(name,_T("WndQuoteReport")) == 0)
	{
		CQuoteReportDlg *pDlg = new CQuoteReportDlg(pParentWnd);
		pDlg->Create(CQuoteReportDlg::IDD,pParentWnd);
		pDlg->ShowWindow(SW_SHOW);
		pWnd = (IBizWindow*)pDlg;
	}
	else if (_tcscmp(name, _T("WndBlockAdscription")) == 0)
	{
		CBlockAdscriptionDlg* pDLg= new CBlockAdscriptionDlg(pParentWnd);
		pDLg->Create(CBlockAdscriptionDlg::IDD, pParentWnd);
		pDLg->ShowWindow(SW_SHOW);
		pWnd = (IBizWindow*)pDLg;
	}
	return pWnd;
}
