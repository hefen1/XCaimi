// DiagramRTable.cpp : ���� DLL �ĳ�ʼ�����̡�
//

#include "stdafx.h"
#include "DiagramRTable.h"
#include "DrawRStockTitle.h"
#include "DrawRStockForm.h"
#include "DrawRTick.h"
#include "DrawRFinance.h"
#include "WinnerApplication.h"
#include "DrawTrendDetail.h"
#include "DrawExitRight.h"
#include "DrawFenPrice.h"
#include "DrawInfo.h"
#include "DrawXXDL.h"
#include "DrawQuoteReport.h"
#include "DrawRValue.h"
#include "DrawRLevelForm.h"
#include "DrawTradeStockForm.h"


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

// CDiagramRTableApp

BEGIN_MESSAGE_MAP(CDiagramRTableApp, CWinApp)
END_MESSAGE_MAP()


// CDiagramRTableApp ����

CDiagramRTableApp::CDiagramRTableApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CDiagramRTableApp ����

CDiagramRTableApp theRTableApp;



// CDiagramRTableApp ��ʼ��

BOOL CDiagramRTableApp::InitInstance()
{
	CWinApp::InitInstance();
	return TRUE;
}

void CDiagramRTableApp::CreateInterface()
{
	m_iInfo = (IHsInfo*)CWinnerApplication::GetObject(OBJ_INFO);
	m_iColor = (IHsColor*)CWinnerApplication::GetObject(OBJ_COLORCFG);
	m_iFont = (IHsFont*)CWinnerApplication::GetObject(OBJ_FONTCFG);
	m_pDataSource = (IDataSourceEx*)CWinnerApplication::GetObject(OBJ_HSDATAENGINE);
	m_iUserDefCfg = (IHsUserDataCfg*)CWinnerApplication::GetObject(OBJ_USERDATA);
}

extern "C" int WINAPI InitEntity()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	theRTableApp.CreateInterface();
	return 0;
}

extern "C" int WINAPI UninitEntity()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return 0;
}

//
extern "C" IDrawBase* WINAPI GetDiagramInstance(const char* name)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	if(strnicmp(name, "DiagramRStockTitle", strlen(name)) == 0)
	{
		CDrawRStockTitle* pRStockTitle = new CDrawRStockTitle(theRTableApp.m_iUserDefCfg, theRTableApp.m_iColor, theRTableApp.m_iFont);
		return pRStockTitle;
	}
	else if(strnicmp(name, "DiagramRStockForm", strlen(name)) == 0)
	{
		CDrawRStockForm* pRStockForm = new CDrawRStockForm(theRTableApp.m_pDataSource);
		return pRStockForm;
	}
	else if(strnicmp(name, "DiagramRTick", strlen(name)) == 0)
	{
		CDrawRTick* pRTick = new CDrawRTick(theRTableApp.m_pDataSource);
		return pRTick;
	}
	else if (strnicmp(name, "DiagramRFinance", strlen(name)) == 0)
	{
		CDrawRFinance* pFinance = new CDrawRFinance(theRTableApp.m_pDataSource);
		return pFinance;
	}
	else if (strnicmp(name, "DiagramTrendDetail", strlen(name)) == 0)
	{
		CDrawTrendDetail* pTrendDetail = new CDrawTrendDetail(theRTableApp.m_pDataSource);
		return pTrendDetail;
	}
	else if (strnicmp(name, "DiagramExitRight", strlen(name)) == 0)
	{
		CDrawExitRight* pDraw = new CDrawExitRight(theRTableApp.m_pDataSource);
		return pDraw;
	}
	else if (strnicmp(name, "DiagramFenPrice", strlen(name)) == 0)
	{
		CDrawFenPrice* pFenPrice = new CDrawFenPrice(theRTableApp.m_pDataSource, theRTableApp.m_iColor, theRTableApp.m_iFont);
		return pFenPrice;
	}
	else if (strnicmp(name, "DiagramInfo", strlen(name)) == 0)
	{
		CDrawInfo* pInfo = new CDrawInfo(theRTableApp.m_iColor, theRTableApp.m_iInfo);
		return pInfo;
	}
	else if (strnicmp(name, "DiagramXXDL", strlen(name)) == 0)
	{
		CDrawXXDL* pXXDL = new CDrawXXDL(theRTableApp.m_iColor, theRTableApp.m_iFont, theRTableApp.m_iInfo);
		return pXXDL;
	}
	else if(strnicmp(name, "DiagramQuoteReport", strlen(name)) == 0)
	{
		CDrawQuoteReport* pQuoteReport = new CDrawQuoteReport;
		return pQuoteReport;
	}
	else if (strnicmp(name, "DiagramValue", strlen(name)) == 0)
	{
		CDrawRValue* pValue = new CDrawRValue(theRTableApp.m_pDataSource);
		return pValue;
	}
	else if (strnicmp(name, "DiagramRLevelForm", strlen(name)) == 0)
	{
		CDrawRLevelForm* pLevelFrom = new CDrawRLevelForm(theRTableApp.m_pDataSource);
		return pLevelFrom;
	}
	else if (strnicmp(name, "DiagramTradeStockForm", strlen(name)) == 0)
	{
		CDrawTradeStockForm* pTradeForm = new CDrawTradeStockForm(theRTableApp.m_pDataSource);
		return pTradeForm;
	}

	return NULL;
}
