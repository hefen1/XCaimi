/*******************************************************
  Դ��������:TradeLang.cpp
  �������Ȩ:�������ӹɷ����޹�˾
  ϵͳ����:  Ͷ��Ӯ�ҽ�������ն�V1.0
  ģ������:  ������Դ
  ����˵��:  
  ��    ��:  shenglq
  ��������:  20100921
*********************************************************/
// TradeLang.cpp : ���� DLL �ĳ�ʼ�����̡�
//

#include "stdafx.h"
#include "TradeLang.h"
#include "TradeLangMng.h"

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

// CTradeLangApp

BEGIN_MESSAGE_MAP(CTradeLangApp, CWinApp)
END_MESSAGE_MAP()


// CTradeLangApp ����

CTradeLangApp::CTradeLangApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CTradeLangApp ����

CTradeLangApp theApp;

CTradeLang gTradeLang;


// CTradeLangApp ��ʼ��

BOOL CTradeLangApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}

ITradeLang* PASCAL GetServiceObject(const char* szName,const char* szPrefix)
{
	return &gTradeLang;
}

int PASCAL InitEntity()
{

	return TRUE;
}

int PASCAL UninitEntity()
{
	return TRUE;
}
