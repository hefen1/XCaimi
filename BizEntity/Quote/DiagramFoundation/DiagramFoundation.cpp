// DiagramFoundation.cpp : ���� DLL �ĳ�ʼ�����̡�
//

#include "stdafx.h"
#include "DiagramFoundation.h"
#include "..\..\..\Common\WinnerApplication.h"

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

// CDiagramFoundationApp

BEGIN_MESSAGE_MAP(CDiagramFoundationApp, CWinApp)
END_MESSAGE_MAP()


// CDiagramFoundationApp ����

CDiagramFoundationApp::CDiagramFoundationApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CDiagramFoundationApp ����

CDiagramFoundationApp theFoundationApp;


// CDiagramFoundationApp ��ʼ��

BOOL CDiagramFoundationApp::InitInstance()
{
	CWinApp::InitInstance();
	
	m_iColor = (IHsColor*)CWinnerApplication::GetObject(OBJ_COLORCFG);
	m_iFont = (IHsFont*)CWinnerApplication::GetObject(OBJ_FONTCFG);
	m_pDataSource = (IDataSourceEx*)CWinnerApplication::GetObject(OBJ_HSDATAENGINE);
	return TRUE;
}
