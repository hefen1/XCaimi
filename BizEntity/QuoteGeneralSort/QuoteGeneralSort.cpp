// QuoteSumGeneralSort.cpp : ���� DLL �ĳ�ʼ�����̡�
//

#include "stdafx.h"
#include "QuoteGeneralSort.h"
#include "QuoteGeneralSortDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
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

// CQuoteSumGeneralSortApp

BEGIN_MESSAGE_MAP(CQuoteGeneralSortApp, CWinApp)
END_MESSAGE_MAP()


// CQuoteSumGeneralSortApp ����

CQuoteGeneralSortApp::CQuoteGeneralSortApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CQuoteSumGeneralSortApp ����

CQuoteGeneralSortApp theApp;


// CQuoteSumGeneralSortApp ��ʼ��

BOOL CQuoteGeneralSortApp::InitInstance()
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

//
IBizWindow* PASCAL GetEntityInstance(const char* name, HWND  hOwner)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	if(name == NULL)
		return NULL;

	IBizWindow *pQuoteGeneralSortWnd = NULL;
	CWnd *pParentWnd = NULL;
	if(::IsWindow(hOwner))
		pParentWnd = CWnd::FromHandle(hOwner);
	if(_tcscmp(name,_T("WndQuoteGeneralSort")) == 0)
	{
		QuoteGeneralSortDlg *pDlg = new QuoteGeneralSortDlg();
		//pDlg->CreateStatic(pParentWnd,3,3);
		pDlg->Create(QuoteGeneralSortDlg::IDD,pParentWnd);
	/*	pDlg->ShowWindow(SW_SHOW);*/
		pQuoteGeneralSortWnd = (IBizWindow*)pDlg;
	}
	return pQuoteGeneralSortWnd;
}
