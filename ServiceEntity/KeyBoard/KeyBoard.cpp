// KeyBoard.cpp : ���� DLL �ĳ�ʼ�����̡�
//

#include "stdafx.h"
#include "KeyBoard.h"
#include "KeyBoardImp.h"

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

// CKeyBoardApp

BEGIN_MESSAGE_MAP(CKeyBoardApp, CWinApp)
END_MESSAGE_MAP()


// CKeyBoardApp ����

CKeyBoardApp::CKeyBoardApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CKeyBoardApp ����

CKeyBoardApp theApp;
CKeyBoard* g_pCKeyBoard=NULL;

// CKeyBoardApp ��ʼ��

BOOL CKeyBoardApp::InitInstance()
{
	/*AfxOleInit();*/
	CWinApp::InitInstance();

	return TRUE;
}


int PASCAL InitEntity()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	if ( g_pCKeyBoard == NULL)
	{
		g_pCKeyBoard = new CKeyBoard;
// 		g_pCKeyBoard->CreateKeyboard();
// 		g_pCKeyBoard->InitKeyboard();
	}
	return 1;
}

//�ͷŲ��
int PASCAL UninitEntity()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	if ( g_pCKeyBoard )
		delete g_pCKeyBoard;
	g_pCKeyBoard = NULL;
	return 1;
}

IKeyboard* PASCAL GetServiceObject(const char* szName,const char* szPrefix)
{
	if(_tcscmp(szName,_T("KeyBoard")) == 0)
	{
		if ( g_pCKeyBoard == NULL )
			g_pCKeyBoard = new CKeyBoard;
		return g_pCKeyBoard;
	}
	return NULL;
}	
