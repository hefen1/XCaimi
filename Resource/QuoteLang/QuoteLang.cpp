// QuoteLang.cpp : ���� DLL �ĳ�ʼ�����̡�
//

#include "stdafx.h"
#include "QuoteLang.h"
#include "quotelangmng.h"

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

// CQuoteLangApp

BEGIN_MESSAGE_MAP(CQuoteLangApp, CWinApp)
END_MESSAGE_MAP()


// CQuoteLangApp ����

CQuoteLangApp::CQuoteLangApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CQuoteLangApp ����

CQuoteLangApp theApp;

CQuoteLang g_QuoteLang;

// CQuoteLangApp ��ʼ��

BOOL CQuoteLangApp::InitInstance()
{
	CWinApp::InitInstance();

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
IQuoteLang* PASCAL GetServiceObject(const char* szName,const char* szPrefix)
{
	if(szName == NULL)
		return NULL;

	if(_tcscmp(szName,_T("CQuoteLang")) == 0)
	{
		return &g_QuoteLang;
	}
	return NULL;
}
