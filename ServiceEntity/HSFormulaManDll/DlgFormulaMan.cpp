// DlgFormulaMan.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "DlgFormulaMan.h"

#include "DlgFormulaEdit.h"
#include "CompileEditView.h"

#include "DlgNotesAndEidolon.h"
#include "DlgFormulaImportExport.h"


#include "DlgFormulaBrowser.h"
#include "DlgCondition.h"
#include "DlgConSelStock.h"
#include "DlgTailorSelStock.h"
#include "DlgIndicatorSelStock.h"
#include "DlgIntelligentSelStock.h"
#include "SheetExchMain.h"

#include "DlgBlockAnalysis.h"
#include "DlgUniteAnalysis.h"
#include "DlgAlarmMan.h"
#include "DlgAddStockToAlarm.h"

#include "DlgExchOptimize.h"

#include "DlgFormulaFind.h" //by HS
#include "DlgStrategyMain.h"
/*#include "DlgStrategyTest.h"*/
#include "DlgStrategyJY.h"
#include "DlgTestStrategy.h"
#include "DlgStrategyManage.h"
#include "RegMsg.h"
#include "GeneralHelper.h"
#include "ExpPublic.h"
#include "HSDataSource.h"
#include "WinnerApplication.h"
#include "HSBizDialog.h"
#include "QuoteComm/QuoteDefine.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#ifdef _AFXDLL
CArray<StockUserInfo*,StockUserInfo*> StockArea::ayStock;
#endif

#ifdef SUPPORT_2004_03_23_CHANGE
extern CString		  m_OldstrKey;
#endif

extern UINT		m_ShowQuickTradeKey;
/////////////////////////////////////////////////////////////////////////////

int     GetMultiDayParam() { return 65; }
BOOL    IsFirstInstance()  { return 0; }

#undef NON_SUPPORT_EXCHANGE

HWND g_hEditUpdateParentWnd;
HWND g_hParentWnd = NULL;
BOOL g_bIsExeFile = 0;

IDataSourceEx*   g_pDataSource = NULL;
CMapStringToOb *Formular_CfgTopMapOb = NULL;
CMapStringToOb *Formular_UserCfgMapOb = NULL;

CDlgFormulaFind *g_pDlgFormulaFind = NULL;  
void*	g_pSharedMapPosData = NULL;
static CMapVariabile* m_payDefTechAnalysis = NULL;
extern CDlgFormulaMan* g_pFormulaMan;
extern BOOL g_hShowCmd;
extern UINT HS_USER_STRATEGY;


IMPLEMENT_DYNAMIC(CDlgFormulaMan,CHSBaseDialog)



void SetActiveMain(CWnd* pRefWnd)
{
	CWnd* pWnd;
	if( g_pFormulaMan && 
		g_pFormulaMan != pRefWnd &&
		g_pFormulaMan->IsWindowVisible() )
	{
		pWnd = g_pFormulaMan;
	}
	else
	{
		pWnd = CWnd::FromHandle(g_hParentWnd);
	}
	if( pWnd != NULL )
	{
		::SetActiveWindow(pWnd->m_hWnd);
	}
}
BOOL GetStockByCode(CodeInfo* pInfo, StockUserInfo& stockInfo )
{
	if ( g_pDataSource )
		return g_pDataSource->HSDataSourceEx_GetStockUserInfo(pInfo, stockInfo);
	return FALSE;
}

int GetStockByMaket(unsigned short nMarket,  CArray<StockUserInfo>& stockArray) 
{
	if ( g_pDataSource )
		return g_pDataSource->HSDataSourceEx_GetMarketStocksInfo(nMarket, stockArray);
	return 0;
}


BOOL OpeDefTech(void* pExpress, BOOL bFind, void* pData, char cOperater)
{
	if(m_payDefTechAnalysis == NULL)
	{
		m_payDefTechAnalysis = new CMapVariabile;
	}

	if( cOperater == '+' ) // add
	{
		if( pExpress != NULL )
		{
			CExpression* pExp = ((CExpValue*)pExpress)->GetExp();
			if(pExp != NULL && pExp->IsExpressType(Tech) )
			{ 
				CString strName;
				strName.Format("%s", pExp->GetWholeName());
				CValue* valoare;
				pExp->AddStyle(HS_EXP_DEFAULT_TECH);

				if ( m_payDefTechAnalysis->Lookup(strName, valoare) )
				{
					return TRUE;
				}
				else
				{
					//-------2009.12.16 litao修改常用公式的排序未保存问题----------------------------------------------
					if (pExp->GetDefSortId() == -1)   //通过置为-1表示是系统设置界面新增
					{
						pExp->SetDefSortId(m_payDefTechAnalysis->GetCount()+1) ;
						m_payDefTechAnalysis->Add(strName, (CValue*)pExpress);
					}
					else if((pExp->GetDefSortId() < -1) || (pExp->GetDefSortId() == 0))  
						m_payDefTechAnalysis->Add(strName, (CValue*)pExpress);
					else
					{
						if (m_payDefTechAnalysis->GetCount() == 0)
							m_payDefTechAnalysis->Add(strName, (CValue*)pExpress);
						else
						{
							CValue*  tmpCValue;
							bool bInsert = FALSE;
							//找到对应位置加入
							for(int i=0;i< m_payDefTechAnalysis->GetCount();i++)
							{
								tmpCValue = m_payDefTechAnalysis->GetAt(i);
								if (((CExpression*)(CValue*)tmpCValue->GetExp())->GetDefSortId() > pExp->GetDefSortId())
								{
									m_payDefTechAnalysis->InsertAt(strName, (CValue*)pExpress,i);
									bInsert = TRUE;
									break;
								}
							}
							if (bInsert == FALSE)
								m_payDefTechAnalysis->Add(strName, (CValue*)pExpress);
						}
					}
					//-------------------------------------------------------------------------------------------------
					//---m_payDefTechAnalysis->Add(strName, (CValue*)pExpress);
				}
			}
		}
	}
	else if( cOperater == '-' ) // 删除
	{
		if( pExpress != NULL )
		{
			CExpression* pExp = ((CExpValue*)pExpress)->GetExp();
			if(pExp != NULL)
			{
				CString strName;
				strName.Format("%s", pExp->GetName());
				CValue* valoare;
				if ( m_payDefTechAnalysis->Lookup( strName, pExp->GetExpressType(), valoare ) )
				{
					pExp->RemoveStyle(HS_EXP_DEFAULT_TECH);
					m_payDefTechAnalysis->RemoveKey( (char*)LPCSTR(strName), pExp->GetExpressType() );
					m_payDefTechAnalysis->DeleteAt(valoare);
				}
			}
		}
	}
	else if( cOperater == 'd' )
	{
		m_payDefTechAnalysis->RemoveAll();
	}

	return TRUE;
}

#define USER_LIST	1001

CMapStringToPtr* g_pTableHeadData = NULL; // 表头

CMapVariabile*  CDlgFormulaMan::m_pExternExpression = NULL;     // 外部表达式表
CMapVariabile*  CDlgFormulaMan::m_pExternVariabile  = NULL;	 // 外部变量表
CMapVariabile*  CDlgFormulaMan::m_pExternFunction   = NULL;	 // 外部函数表

CDlgFormulaEdit* CDlgFormulaMan::m_pEdiUpdatetExp    = NULL;	 // 公式
CDlgNotesAndEidolon* CDlgFormulaMan::m_pDlgNotes    = NULL;     // 注释
CDlgNotesAndEidolon* CDlgFormulaMan::m_pDlgParam    = NULL;     // 参数

HSFunAddDefaultTechAnysis CDlgFormulaMan::m_pOpeDefTechFun = NULL;

UINT g_AllMsgData = RegisterWindowMessage(HS_ALL_MSG_DATA);

static CDlgCondition* g_pHSCondition = NULL;
static CDlgConSelStock* g_pConditionSelect = NULL;
static CDlgTailorSelStock* g_pTailorSelectStock = NULL;
static CDlgIndicatorSelStock* g_pTechSelect = NULL;
static CDlgIntelligentSelStock* g_pInteSelect = NULL;


static CDlgAlarmMan* g_pWranManagerDlg = NULL;
static CSheetExchMain* g_pExchUnion = NULL;

CImageList	CDlgFormulaMan::m_tabImages;
CImageList	CDlgFormulaMan::m_expressCheckImages;
CImageList	CDlgFormulaMan::m_expressImages;

CDlgFormulaMan::CDlgFormulaMan(CWnd* pParent /*=NULL*/,long* pThis /*= NULL*/)
: CHSBaseDialog(CDlgFormulaMan::IDD, pParent)
{
	
//	m_pIndividualInfo = NULL;
	//m_pKeyboard = NULL;
	m_wExternalExpress = 0;
	m_pColProt = (IHsColumnProperty*)CWinnerApplication::GetObject(OBJ_COLUMNPRO);
	m_pMainWnd = (IMainWnd*)CWinnerApplication::GetObject(OBJ_MAINFRAME);
}

CDlgFormulaMan::~CDlgFormulaMan()
{
	Delete();
}

void CDlgFormulaMan::DoDataExchange(CDataExchange* pDX)
{
	CHSBaseDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgFormulaMan)
	DDX_Control(pDX, IDC_TAB, m_wndTab);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgFormulaMan, CHSBaseDialog)
	//{{AFX_MSG_MAP(CDlgFormulaMan)
	ON_WM_DESTROY()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB, OnSelchangeTab)
	ON_BN_CLICKED(IDD_NEW, OnNew)
	ON_BN_CLICKED(IDD_UPDATE, OnUpdate)
	ON_BN_CLICKED(IDD_DELETE, OnDelete)
	ON_BN_CLICKED(IDC_NOTES, OnNotes)
	ON_BN_CLICKED(IDD_OUT, OnOut)
	ON_BN_CLICKED(IDD_IN, OnIn)
	ON_BN_CLICKED(IDD_TEMPIN, OnTempin)
	ON_BN_CLICKED(IDC_PARAM, OnParam)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BUTFIND, OnButfind)
	//}}AFX_MSG_MAP

	ON_REGISTERED_MESSAGE(g_AllMsgData,OnRun)
	ON_MESSAGE(HX_USER_LOADCUR, LoadCurExp)
	ON_REGISTERED_MESSAGE(HX_USER_DRAGDATA,OnUserDragData)

	ON_REGISTERED_MESSAGE(HX_USER_COMPILEDATA,OnExchangData)

	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgFormulaMan message handlers

BOOL CDlgFormulaMan::OnInitDialog() 
{
	CHSBaseDialog::OnInitDialog();

	//数据引擎接口初始化
	g_pDataSource = (IDataSourceEx*)CWinnerApplication::GetObject(OBJ_HSDATAENGINE);

	this->SetWindowText(_T("公式管理"));

	static  BOOL bOne = FALSE;
	// image
	if( !bOne )
	{
		bOne = TRUE;

		m_expressCheckImages.Create (IDB_CHECKBOXIMG, 16, 0, RGB (255, 0, 0));

		// Init Data
		InitData();


		if( CExpression::m_expressImages != NULL )
		{
			m_expressImages.m_hImageList = CExpression::m_expressImages->m_hImageList;
		}
		else
		{
			m_expressImages.Create (IDD_ALLIMAGE, 16, 0, RGB (0, 128, 128));
		}
		//
		m_tabImages.Create (IDB_EXPRESS_TAB, 16, 0, RGB (255, 0, 0));

		// add tab
		m_wndTab.SetImageList ( &m_tabImages );

#ifdef NON_SUPPORT_EXCHANGE
		m_wndTab.InsertItem(0,CTreeCtrlFormula::g_strTech,0);
		m_wndTab.InsertItem(1,CTreeCtrlFormula::g_strCondition,2);
		m_wndTab.InsertItem(2,CTreeCtrlFormula::g_strColorKLine,3);	
#else
		m_wndTab.InsertItem(0,CTreeCtrlFormula::g_strTech,0);
#ifndef _METATRADER_STYLE_	
		m_wndTab.InsertItem(1,CTreeCtrlFormula::g_strExchange,1);
		m_wndTab.InsertItem(2,CTreeCtrlFormula::g_strCondition,2);
		m_wndTab.InsertItem(3,CTreeCtrlFormula::g_strColorKLine,3);	
		m_wndTab.InsertItem(4,CTreeCtrlFormula::g_strStrategy,4); //20090302 YJT 增加
#endif /*_METATRADER_STYLE_*/
#endif

		// tree
		CString strName;
		for(int nTab = 0; nTab < m_wndTab.GetItemCount(); nTab ++)
		{
#ifdef NON_SUPPORT_EXCHANGE
			strName = ( nTab == 0 ? CTreeCtrlFormula::g_strTech:
						nTab == 1 ? CTreeCtrlFormula::g_strCondition: 
									CTreeCtrlFormula::g_strColorKLine);
#else
			strName = ( nTab == 0 ? CTreeCtrlFormula::g_strTech:
						nTab == 1 ? CTreeCtrlFormula::g_strExchange:
						nTab == 2 ? CTreeCtrlFormula::g_strCondition: 
						nTab == 3 ?	CTreeCtrlFormula::g_strColorKLine:
									CTreeCtrlFormula::g_strStrategy); //20090302 YJT 增加
#endif

			CTreeCtrlFormula* pTree = new CTreeCtrlFormula(strName);
			pTree->SetType(CTreeCtrlFormula::Express);

			if ( !pTree->Create ( WS_CHILD|WS_BORDER|TVS_HASLINES|TVS_LINESATROOT|
				TVS_HASBUTTONS|TVS_SHOWSELALWAYS|TVS_EDITLABELS/*|TVS_CHECKBOXES*/, 
				CRect(0,0,0,0),&m_wndTab,USER_LIST+nTab ) )
			{
				TRACE0("Failed to create workspace view\n");
				return 0;
			}

			//HIMAGELIST h = TreeView_GetImageList(pTree->m_hWnd,TVSIL_STATE );

			pTree->ModifyStyleEx(0,WS_EX_CLIENTEDGE);
			pTree->SetImageList (&CDlgFormulaMan::m_expressImages, TVSIL_NORMAL);
			//pTree->SetImageList (&m_expressCheckImages, TVSIL_STATE);
			pTree->m_pFormulaMan = this;

			m_pAyTree.Add( pTree );		

			switch(nTab)
			{
#ifdef NON_SUPPORT_EXCHANGE
			case 0:
				pTree->FillExpress(m_pExternExpression,CTreeCtrlFormula::g_strTech,Tech,0);
				break;
			case 1:
				pTree->FillExpress(m_pExternExpression,CTreeCtrlFormula::g_strCondition,
					Condition,0);
				break;
			case 2:
				pTree->FillExpress(m_pExternExpression,CTreeCtrlFormula::g_strColorKLine,
					MoreKLine,0);
				break;
#else
			case 0:
				pTree->FillExpress(m_pExternExpression,CTreeCtrlFormula::g_strTech,Tech,0);
				break;
			case 1:
				pTree->FillExpress(m_pExternExpression,CTreeCtrlFormula::g_strExchange,
					Exchange,0);
				break;
			case 2:
				pTree->FillExpress(m_pExternExpression,CTreeCtrlFormula::g_strCondition,
					Condition,0);
				break;
			case 3:
				pTree->FillExpress(m_pExternExpression,CTreeCtrlFormula::g_strColorKLine,
					MoreKLine,0);
				break;
			case 4: //20090302 YJT 增加 Strategy
				pTree->FillExpress(m_pExternExpression,CTreeCtrlFormula::g_strStrategy,
					Strategy,0);
				break;
#endif
			default:
				break;
			}
      
			//pTree->ExpandGroup(NULL,TVE_EXPAND);
		}
	}
	else
	{
		m_wndTab.SetParent(this);
		CTreeCtrlFormula* pTree;
		for(int i = 0;i < m_pAyTree.GetSize(); i++)
		{
			pTree = m_pAyTree.GetAt(i);
			pTree->SetParent(this);
		}
	}

	CTreeCtrlFormula* pTree = GetCurTree();
	if(pTree != NULL)
	{
		pTree->SetFocus();
	}

	SetSize();

#ifdef Support_Hlt
	CWnd* pWnd = this->GetDlgItem(IDD_OUT);
	if( pWnd )
		pWnd->ShowWindow(SW_HIDE);
#endif

	return TRUE;  
}

void CDlgFormulaMan::SetTree(CTreeCtrlFormula* pTree)
{
	pTree->SetImageList (&CDlgFormulaMan::m_expressImages, TVSIL_NORMAL);
	pTree->SetImageList (&m_expressCheckImages, TVSIL_STATE);
	pTree->m_pFormulaMan = this;
}

void CDlgFormulaMan::OnDestroy() 
{
	Delete();

	CHSBaseDialog::OnDestroy();
	// TODO: Add your message handler code here
}

void CDlgFormulaMan::OnCancel() 
{
	if(g_bIsExeFile)
	{
		ShowWindow(SW_HIDE);
		Delete();
		CHSBaseDialog::OnCancel();
		//delete this;

		CWinApp* pApp = AfxGetApp();
		if(pApp != NULL)
		{
			pApp->PostThreadMessage(WM_QUIT,0,0);
		}
		else
		{
			exit(0);
		}
	}
	else
	{
		//ShowWindowEx(SW_MINIMIZE);
		ShowWindow(SW_HIDE);

		//Enable(TRUE);
		Save();

		SetActiveMain(this);

	}
#ifdef _METATRADER_STYLE_
	// Reworked by HS隐藏后焦点至ChildFrame
	if(g_hParentWnd)
	{
		::SetFocus(g_hParentWnd);
	}
#endif
}

void CDlgFormulaMan::OnOK() 
{
	/*
	if(g_bIsExeFile)
	{
	Delete();
	CHSBaseDialog::OnOK();		
	}
	else
	{
	ShowWindow(SW_HIDE);
	}
	*/
	// Reworked by HS隐藏后焦点至ChildFrame
	if(g_hParentWnd)
	{
		::SetFocus(g_hParentWnd);
	}
}

void CDlgFormulaMan::Delete()
{
	try
	{
		if ( g_pDlgFormulaFind != NULL && !IsBadReadPtr(g_pDlgFormulaFind,1))
		{
			g_pDlgFormulaFind->DestroyWindow();
			delete g_pDlgFormulaFind;
			g_pDlgFormulaFind = NULL;
		}
		if ( m_payDefTechAnalysis && !IsBadReadPtr(m_payDefTechAnalysis,1))
		{
			m_payDefTechAnalysis->RemoveAll();
			delete m_payDefTechAnalysis;
			m_payDefTechAnalysis = NULL;
		}
		if( g_pWranManagerDlg != NULL )
		{
			g_pWranManagerDlg->DestroyWindow();
			delete g_pWranManagerDlg;
			g_pWranManagerDlg = NULL;
		}

 		if( g_pExchUnion != NULL )  
 		{
 			g_pExchUnion->DestroyWindow();
 			delete g_pExchUnion;
 			g_pExchUnion = NULL;
 		}

		if( g_pHSCondition != NULL )
		{
			g_pHSCondition->DestroyWindow();
			g_pHSCondition->Delete();
			delete g_pHSCondition;
			g_pHSCondition = NULL;
		}

		if( g_pConditionSelect != NULL )
		{
			g_pConditionSelect->DestroyWindow();
			g_pConditionSelect->Delete();
			delete g_pConditionSelect;
			g_pConditionSelect = NULL;
		}

		if( g_pTailorSelectStock != NULL )
		{
			g_pTailorSelectStock->DestroyWindow();
			g_pTailorSelectStock->Delete();
			delete g_pTailorSelectStock;
			g_pTailorSelectStock = NULL;
		}

		if( g_pTechSelect != NULL )
		{
			g_pTechSelect->DestroyWindow();
			g_pTechSelect->Delete();
			delete g_pTechSelect;
			g_pTechSelect = NULL;
		}

		if( g_pInteSelect != NULL )
		{
			g_pInteSelect->DestroyWindow();
			g_pInteSelect->Delete();
			delete g_pInteSelect;
			g_pInteSelect = NULL;
		}

		if( m_pEdiUpdatetExp != NULL )
		{
			m_pEdiUpdatetExp->DestroyWindow();
			m_pEdiUpdatetExp->Delete();
			delete m_pEdiUpdatetExp;
			m_pEdiUpdatetExp = NULL;
		}
		CTreeCtrlFormula* pTree;
		for(int i = m_pAyTree.GetSize() - 1; i >= 0; i--)
		{
			pTree = m_pAyTree.GetAt(i);
			pTree->SetParent(NULL);
			pTree->DestroyWindow();
			delete pTree;
		}
		m_pAyTree.RemoveAll();


		if(m_pDlgNotes != NULL)
		{
			if( m_pDlgNotes->m_hWnd )
			{
				m_pDlgNotes->DestroyWindow();
			}
			delete m_pDlgNotes;
			m_pDlgNotes = NULL;
		}
		if(m_pDlgParam != NULL)
		{
			if( m_pDlgParam->m_hWnd )
			{
				m_pDlgParam->DestroyWindow();
			}
			delete m_pDlgParam;
			m_pDlgParam = NULL;
		}

		m_pExternExpression = DeleteVarsMap(m_pExternExpression,/*g_bIsExeFile*/TRUE);
		m_pExternVariabile  = DeleteVarsMap(m_pExternVariabile,/*g_bIsExeFile*/TRUE);
		m_pExternFunction   = DeleteVarsMap(m_pExternFunction,/*g_bIsExeFile*/TRUE);

		CTreeCtrlFormula* key = NULL;
		ExternParam* value = NULL;

		POSITION  pos;
		for( pos = m_ayExtern.GetStartPosition(); pos != NULL; )
		{
			m_ayExtern.GetNextAssoc( pos, key, value );
			
			_delObject(value);			
			if(key)
			{
				key->DestroyWindow();
				delete key;
				key = NULL;
			}
		}
		m_ayExtern.RemoveAll();

		if ( g_pTableHeadData && !IsBadReadPtr(g_pTableHeadData,1))
		{
			POSITION pos;
			CString key;
			CColPropIDIndex* pArray = NULL;
			for( pos = g_pTableHeadData->GetStartPosition(); pos != NULL; )
			{
				g_pTableHeadData->GetNextAssoc( pos, key, (void*&)pArray);
				if (pArray)
				{				
					pArray->RemoveAll();
					delete pArray;
				}
			}	
			g_pTableHeadData->RemoveAll();
			delete g_pTableHeadData;
			g_pTableHeadData = NULL;
		}
		if ( CExpression::m_expressImages )
			delete CExpression::m_expressImages;
		CExpression::m_expressImages = NULL;
		for ( int i=0; i<StockArea::ayStock.GetCount(); i++)
		{
			StockUserInfo* pStock = StockArea::ayStock.GetAt(i);
			if (pStock)
				delete pStock;
		}
		StockArea::ayStock.RemoveAll();
	}
	catch(...)
	{
	}
}

void CDlgFormulaMan::OnSelchangeTab(NMHDR* pNMHDR, LRESULT* pResult) 
{
	int nIndex = m_wndTab.GetCurSel( );
	CTreeCtrlFormula* pFirstTree = GetCurTree(0);
	CTreeCtrlFormula* pTree = GetCurTree(nIndex);
	if(pTree == NULL || pFirstTree == NULL)
	{
		*pResult = 0;
		return;		
	}
	SetSize();
	ShowTree(pTree,TRUE);

	if( pTree->GetCount() > 0)
	{
		*pResult = 0;
		return;
	}

	switch(nIndex)
	{
		case 0:
			pTree->FillExpress(m_pExternExpression,CTreeCtrlFormula::g_strTech,Tech,0);
			break;
		case 1:
			pTree->FillExpress(m_pExternExpression,CTreeCtrlFormula::g_strExchange,Exchange,0);
			break;
		case 2:
			pTree->FillExpress(m_pExternExpression,CTreeCtrlFormula::g_strCondition,Condition,0);
			break;
		case 3:
			pTree->FillExpress(m_pExternExpression,CTreeCtrlFormula::g_strColorKLine,MoreKLine,0);
			break;
		case 4:
			pTree->FillExpress(m_pExternExpression,CTreeCtrlFormula::g_strStrategy,Strategy,0); 
			break;
		default:
			break;
	}
	pTree->ExpandGroup(NULL,TVE_EXPAND);

	*pResult = 0;
}

void CDlgFormulaMan::InitData(HWND hParentWnd/* = NULL*/)
{
	if(hParentWnd)
		g_hParentWnd = hParentWnd;

	if( m_pExternExpression != NULL ||
		m_pExternVariabile  != NULL || 
		m_pExternFunction   != NULL )
		return;
	CMapWordToPtr *m_pmap = NULL;
	//策略交易
	if (g_hParentWnd && (m_pmap = (CMapWordToPtr*)::SendMessage(g_hParentWnd,HS_USER_STRATEGY,0,0)) != NULL)
	{
		ReadStrategy(m_pmap);
	}
	

	m_pOpeDefTechFun   = OpeDefTech;	  //缺省指标
	CHSDataSource::m_wndMsg = g_hParentWnd;
	CExpression::m_expressImages = new  CImageList;
	CExpression::m_expressImages->Create( IDD_ALLIMAGE, 16,0,RGB (0,128,128)) ;
	
	// 表头
	GetColumnProt();

	AllFinancialData::m_pMapStringToID = AllFinancialData::m_pMapStringToID;
	m_pExternExpression = new CMapVariabile;     // 外部表达式表
	m_pExternVariabile  = new CMapVariabile;	 // 外部变量表
	m_pExternFunction   = new CMapVariabile;	 // 外部函数表	
	

	::InitSysFunction(m_pExternFunction);

	CExpression::m_pExternExpression = m_pExternExpression;      // 外部表达式表
	CExpression::m_pExternVariabile  = m_pExternVariabile;		 // 外部变量表
	CExpression::m_pExternFunction   = m_pExternFunction;
}

CDlgFormulaEdit* CDlgFormulaMan::GetExpDlg(CExpression* pExpression/* = NULL*/)
{

#ifdef Support_Hlt
	if( pExpression )
	{
		if( pExpression->GetDescribe().Find("HLT-") == 0 )
		{
			//AfxMessageBox("您无权限修改。");
			return NULL;
		}
	}

	//AfxMessageBox("您无权限修改。");
	//return NULL;
#endif

	/*if( m_pEdiUpdatetExp != NULL )
	{
		m_pEdiUpdatetExp->Delete();
		delete m_pEdiUpdatetExp;
		m_pEdiUpdatetExp = NULL;
	}*/
	if( m_pEdiUpdatetExp == NULL )
	{
		m_pEdiUpdatetExp = new CDlgFormulaEdit(this);		
	}
	m_pEdiUpdatetExp->SetType(CDlgFormulaEdit::none);
	//m_pEdiUpdatetExp->SetExType(CDlgFormulaEdit::none);
	return m_pEdiUpdatetExp;
}

CDlgNotesAndEidolon* CDlgFormulaMan::GetDlgParam()
{
	if( m_pDlgParam == NULL )
	{
		m_pDlgParam = new CDlgNotesAndEidolon;
		m_pDlgParam->Create(this);
		m_pDlgParam->SetType(CDlgNotesAndEidolon::Eidolon);
	}
	return m_pDlgParam;
}

CDlgNotesAndEidolon* CDlgFormulaMan::GetDlgNotes()
{
	if( m_pDlgNotes == NULL )
	{
		m_pDlgNotes = new CDlgNotesAndEidolon;
		m_pDlgNotes->Create(this);
		m_pDlgNotes->SetType(CDlgNotesAndEidolon::Notes);
	}
	return m_pDlgNotes;
}

CDlgFormulaEdit* CDlgFormulaMan::NewExpress(WORD nMask,short nType /*= CDlgFormulaEdit::NewExp*/) 
{
	CDlgFormulaEdit* pDlg = GetExpDlg(NULL);
	if( pDlg != NULL )
	{		
		/*CTreeCtrlFormula* pTree = GetCurTree(nMask);
		if(pTree != NULL)
		{
			ShowTree(pTree,FALSE);
			pDlg->SetTitle(pTree->GetType(),"新建");
		}

		pDlg->m_nExpressType = pTree->GetType();*/

		pDlg->SetTitle(nMask,"新建");
		pDlg->m_nExpressType = nMask;

		pDlg->OnNewExpress();
		pDlg->SetType(nType);

		Enable(FALSE);

		ShowTree(GetCurTree(nMask),TRUE);
		ShowFomulaDlg(pDlg);

		return pDlg;
	}
	return NULL;
}

void CDlgFormulaMan::OnNew() 
{
	CDlgFormulaEdit* pDlg = GetExpDlg(NULL);
	if( pDlg != NULL )
	{
		CTreeCtrlFormula* pTree = GetCurTree();
		if(pTree != NULL)
		{
			pDlg->SetTitle(pTree->GetType(),_T("新建"));
		}

		pDlg->m_nExpressType = pTree->GetType();

		pDlg->OnNewExpress();

		Enable(FALSE);
		ShowFomulaDlg(pDlg); //by HS
			//pDlg->ShowWindowEx( SW_SHOWNORMAL );
	}
}

void CDlgFormulaMan::OnUpdate() 
{
	CTreeCtrlFormula* pTree = GetCurTree();
	if(pTree == NULL)
		return;
	HTREEITEM hItem = pTree->GetSelectedItem();
	if( hItem == NULL )
		return;

	CTreeGroup* pTreeGroup = (CTreeGroup*)pTree->GetItemData(hItem);
	if( pTreeGroup == NULL || 
		!pTreeGroup->IsStatus(hx_GroupItem) )
		return;

	CDlgFormulaEdit* pDlg = GetExpDlg((CExpression*)pTreeGroup->GetData());
	if( pDlg != NULL )
	{
		if(pDlg->SendMessage(HX_USER_LOADCUR,CCompileEditView::TreeMsg,(LPARAM)pTreeGroup->GetData()))
		{
			Enable(FALSE);
			//ShowFomulaDlg(pDlg,0,(LPARAM)pTreeGroup->GetData()); //by HS
			//	pDlg->ShowWindowEx(SW_SHOWNORMAL);
		}
	}
}

//begin by HS
void CDlgFormulaMan::ShowFomulaDlg(CDlgFormulaEdit* pDlg,WPARAM wParam /*= 0*/, LPARAM lParam /*= 0*/)
{
	pDlg->m_wndComBox.ResetContent();

	CTreeCtrlFormula* pCurTree = NULL;

	CExpression* pExpression = (CExpression*)lParam;
	if( pExpression != NULL )
	{
		pCurTree = this->GetCurTree(pExpression);
	}
	else
	{
		pCurTree = this->GetCurTree();
	}
	if( pCurTree == NULL )
		return;

	HTREEITEM hItem;//	   = pCurTree->GetSelectedItem();
	HTREEITEM hParentItem  = pCurTree->GetRootTree("");//hItem;
	HTREEITEM hCurGoupItem = pCurTree->NewGroup(_T("自定义"));// = hItem;

	// find group item
	if( pExpression != NULL )
	{
		HTREEITEM hFindItem;
		CTreeGroup* pTreeGroup = pCurTree->Find(pExpression);
		if( pTreeGroup != NULL )
		{
			hFindItem = pCurTree->GetParentItem(pTreeGroup->m_hTree);
			if( hFindItem != NULL )
			{
				hCurGoupItem = hFindItem;
			}
		}
	}

	//
	if( hParentItem != NULL )
	{
		hItem = pCurTree->GetChildItem( hParentItem );
		int nIndex;

		while( hItem != NULL )
		{
			nIndex = pDlg->m_wndComBox.AddString(pCurTree->GetItemText(hItem));
			pDlg->m_wndComBox.SetItemData( nIndex, (DWORD)hItem );

			if( hCurGoupItem == hItem )
			{
				pDlg->m_wndComBox.SetCurSel(nIndex);
			}

			hItem = pCurTree->GetNextSiblingItem( hItem );
		}
	}

	//pDlg->DoModal();

	pDlg->ShowWindow(SW_SHOWNORMAL);
}
///by HS end

void CDlgFormulaMan::OnDelete() 
{
	CTreeCtrlFormula* pTree = GetCurTree();
	if( pTree != NULL )
	{
		pTree->SendMessage(WM_COMMAND,IDM_DELEXPRESS);
	}
}

void CDlgFormulaMan::OnNotes() 
{
	CTreeCtrlFormula* pTree = GetCurTree();
	if( pTree == NULL )
		return;

	HTREEITEM hItem = pTree->GetSelectedItem();
	if( hItem == NULL )
		return;

	CTreeGroup* pTreeGroup = (CTreeGroup*)pTree->GetItemData(hItem);
	if( pTreeGroup == NULL || 
		!pTreeGroup->IsStatus(hx_GroupItem) )
		return;

	CExpression* pExpression = pTreeGroup->GetData();
	if(pExpression == NULL)
		return;

	CDlgNotesAndEidolon* pDlg = GetDlgNotes();
	if(pDlg != NULL)
	{
		pDlg->SetType(CDlgNotesAndEidolon::Notes);
		pDlg->m_pExpression = pExpression;
		pDlg->SetNotes(pExpression->GetNotes(),pExpression->GetName());
		pDlg->ShowWindow(SW_SHOWNORMAL);
	}
}

long CDlgFormulaMan::OnNotes(WPARAM wParam,LPARAM lParam) 
{
	HSOpenExpressUseData* pData = (HSOpenExpressUseData*)lParam;
	if( pData == NULL )
		return 0;
	CExpression* pExpression = (CExpression*)pData->m_pData;
	if( pExpression == NULL )
		return 0;

	CDlgNotesAndEidolon* pDlg = GetDlgNotes();
	if(pDlg != NULL)
	{
		pDlg->SetType(CDlgNotesAndEidolon::Notes);
		pDlg->m_pExpression = pExpression;
		pDlg->SetNotes(pExpression->GetNotes(),pExpression->GetName());
		pDlg->ShowWindow(SW_SHOWNORMAL);
	}
	return 1;
}

void CDlgFormulaMan::OnOut() 
{

	// TODO: Add your control notification handler code here
	CString fileName;
	int nResult = OpenExpressFile(FALSE,fileName);
	if( nResult == IDOK )
	{
		CDlgFormulaImportExport dlg(this);
		dlg.m_pFormulaMan      = this;
		dlg.m_ayExpress        = NULL;
		dlg.m_wExternalExpress = m_wExternalExpress;
		dlg.m_strFileName      = fileName;
		dlg.m_bInport		   = FALSE;

		if( dlg.DoModal() == IDOK )
		{

		}
	}
}

void CDlgFormulaMan::AutoSetupExpress(HSOpenExpressUseData* pData)
{
	if( pData == NULL )
		return;

	HSExpressAutoSetup* pAutoSetup = (HSExpressAutoSetup*)pData->m_pData;
	if( pAutoSetup == NULL )
		return;

	SetDefaultTech(NULL,'d');

	int      lLength = 0;
	CString  fileName = pAutoSetup->m_strFileName;
	if( fileName.IsEmpty() )
		return;

	char* pBuffer = GetInportBuffer(lLength,fileName);
	//int     nRet  = InportFxjExpress(pBuffer,lLength);

	//if(nRet == 2)
	if( pBuffer && lLength > 0 )
	{
		delete[] pBuffer;

		CDlgFormulaImportExport* dlg = NULL;
		dlg = new CDlgFormulaImportExport(this,(long*)&dlg);
		dlg->m_pFormulaMan      = this;
		dlg->m_ayExpress        = NULL;
		dlg->m_wExternalExpress = /*m_wExternalExpress |*/ EXTERN_EXPRESS_AUTO_SETUP;
		dlg->m_strFileName	    = fileName;
		dlg->Create(CDlgFormulaImportExport::IDD);
		dlg->AutoSetup();

		//delete dlg;
	}
}

void CDlgFormulaMan::OnIn() 
{

	SetDefaultTech(NULL,'d'); 

	int      lLength = 0;
	CString  fileName;
	char* pBuffer = GetInportBuffer(lLength,fileName);
	int     nRet  = InportFxjExpress(pBuffer,lLength);

	if(nRet == 2)
	{
		delete[] pBuffer;

		CDlgFormulaImportExport dlg(this);
		dlg.m_pFormulaMan      = this;
		dlg.m_ayExpress        = NULL;
		dlg.m_wExternalExpress = m_wExternalExpress;
		dlg.m_strFileName	   = fileName;
		if( dlg.DoModal() == IDOK )
		{
			CopyTree();
			// 浮动树 
			CopyToFloatTree();
		}
	}

	if( !nRet )
	{
		MessageBox(_T("不可识别的文件格式"),_T("提示"),MB_ICONINFORMATION);
	}
}

int CDlgFormulaMan::OpenExpressFile(BOOL bOpenFileDialog,CString& strFileName)
{
	CFileDialog dlgFile(bOpenFileDialog);

	CString title = (bOpenFileDialog?_T("引入公式"):_T("输出公式"));

	CString strFilter;

	strFilter += _T(_T("公式类型1 (*.EXP)"));
	strFilter += (TCHAR)'\0';   // next string please
	strFilter += _T("*.EXP");
	strFilter += (TCHAR)'\0';

	strFilter +=_T( _T("公式类型2 (*.FNC)"));
	strFilter += (TCHAR)'\0';   // next string please
	strFilter += _T("*.FNC");
	strFilter += (TCHAR)'\0';

	strFilter += _T(_T("公式类型3 (*.DTA)"));
	strFilter += (TCHAR)'\0';   // next string please
	strFilter += _T("*.DTA");
	strFilter += (TCHAR)'\0';

	strFilter += _T(_T("所有文件 (*.*)"));
	strFilter += (TCHAR)'\0';
	strFilter += _T("*.*");
	strFilter += (TCHAR)'\0';   // last string
	dlgFile.m_ofn.nMaxCustFilter++;

	dlgFile.m_ofn.lpstrFilter = strFilter;
	dlgFile.m_ofn.lpstrTitle  = title;
	dlgFile.m_ofn.lpstrFile	  = strFileName.GetBuffer(_MAX_PATH);
	dlgFile.m_ofn.lpstrDefExt = _T("EXP");

	int nResult = dlgFile.DoModal();
	strFileName.ReleaseBuffer();
	return nResult;
}

char* CDlgFormulaMan::GetInportBuffer(int& lLength,CString& fileName)
{
	int nResult = IDOK;
	if( fileName.IsEmpty() )
	{
		nResult = OpenExpressFile(TRUE,fileName);
	}
	if( nResult == IDOK )
	{
		CFile fp;
		//CFileException Exception;
		nResult = fp.Open(fileName, CFile::modeReadWrite | CFile::shareDenyNone | CFile::typeBinary,NULL);

		if( !nResult ) 
			return NULL;

		lLength = fp.GetLength();
		if(lLength <= 0)
		{
			fp.Close();
			return NULL;
		}

		char* pBuffer = new char[lLength];
		if(pBuffer == NULL)
		{
			fp.Close();
			return NULL;
		}
		fp.Read( pBuffer,sizeof(char)*lLength );
		fp.Close();

		if( IsZipExpressData(pBuffer) )
		{
			SaveTransZipData ZipData;
			memcpy(&ZipData,pBuffer,sizeof(ZipData));
			lLength -= sizeof(ZipData);
			ZipData.m_cData = new char[lLength];
			memcpy(ZipData.m_cData,&pBuffer[sizeof(ZipData)],lLength);
			char* punzipData = unzipData(&ZipData);
			ZipData.Free();

			return punzipData;
		}

		return pBuffer;
	}

	return NULL;
}

void CDlgFormulaMan::OnTempin() 
{
	m_wExternalExpress |= HS_EXP_TEMPIN;

	OnIn();
}

int CDlgFormulaMan::InportFxjExpress(char* pBuffer,long lLength)
{
	if(pBuffer != NULL)
	{
		CArray<FxjExpress*,FxjExpress*>* ayExpress = NULL;

		CExpression::HSFormulaFileHead  head;
		char* pCurBuffer = pBuffer;
		head.m_lCount	 = *(long*)pCurBuffer;
		pCurBuffer	    += sizeof(long);
		head.m_lVersion  = *(long*)pCurBuffer;
		if( head.IsValid() )
			return 2;

		{
			// 头数据
			FxjHead* pHead = (FxjHead*)pBuffer;
			if( pHead->m_nCount <= 0 )
			{
				delete[] pBuffer;
				return FALSE;
			}

			int  i,j;

			// 公式位置数据
			FxjPerhaps* pPerhaps = (FxjPerhaps*)pHead->m_pNextData;
			for( i = 0; i < pHead->m_nCount-1; i++ )
			{
				pPerhaps = (FxjPerhaps*)pPerhaps->m_pNextData;
			}
			char*	  pData		= (char*)pHead->m_pNextData;
			char*	  pNextData = pPerhaps->m_pNextData;

			long* pPosition = (long*)pHead->m_pNextData;
			long  lExpressLen;

			ayExpress = new CArray<FxjExpress*,FxjExpress*>;

			int nRet;
			for( j = 0; (j < pHead->m_nCount && pData && pPosition) ; j++ )
			{
				if(j == 0)
				{
					lExpressLen = 0;
				}
				else
				{
					lExpressLen = *pPosition - sizeof(FxjHead) + 1 + 4;
					pPosition++;

					pNextData = (char*)( &pData[ lExpressLen ] );
				}

				nRet = GetFxjExpress( pNextData,lExpressLen,*pPosition,*ayExpress,lLength );
				if( nRet < 0 )
				{
					nRet = ayExpress->GetSize()-1;
					delete ayExpress->GetAt(nRet);
					ayExpress->RemoveAt(nRet);
				}
			}
		}

		CDlgFormulaImportExport dlg(this);
		dlg.m_pFormulaMan      = this;
		dlg.m_ayExpress        = ayExpress;
		dlg.m_wExternalExpress = m_wExternalExpress;
		if( dlg.DoModal() == IDOK )
		{
			CopyTree();
		}

		delete[] pBuffer;
	}

	return TRUE;
}

int CDlgFormulaMan::GetFxjExpress(char* pBuffer,long lCurPos,long lNextPos,
								  CArray<FxjExpress*,FxjExpress*>& ayExpress,int lLength )
{
	int  nOff,nNext;
	BOOL bOneTime;

	char* pNextData = pBuffer;
	char* pOldNextData;

	FxjParam* pParam;
	FxjBody*  pBody;
	FxjName*  pName;

	FxjExpress*  pFxjExpress;

	int lCurLen = 0;
	int lSubLen;

	int lExpressLen = lCurPos - sizeof(FxjHead) + 1 + 4;
	lSubLen = lNextPos - lCurPos;

	pNextData	 = pBuffer;
	pOldNextData = pNextData;

	// 周期,图方式,公式名
	pName = (FxjName*)pNextData;
	if( (*pName).m_nChart == 1 )
		return 0;

	lCurLen += pName->GetLen();
	if( !IsValid(lCurLen,lSubLen,lLength) )
		return -1;

	pFxjExpress = new FxjExpress;
	ayExpress.Add(pFxjExpress);
	pFxjExpress->m_pName = pName;

	// 参数
	pNextData = (char*)pName->m_pNextData;
	if( pName->m_nParamCount > 0 )
	{
		for(int i = 0; i < pName->m_nParamCount && i < 4; i++ )
		{
			pParam = (FxjParam*)pNextData;

			lCurLen += pParam->GetLen();
			if( !IsValid(lCurLen,lSubLen,lLength) )
				return -1;

			pFxjExpress->m_ayParam->Add(pParam); //
			pNextData = pParam->m_pNextData;
		}
	}

	pOldNextData = pNextData;

	if( (*pName).m_nChart != 1 ) // 交易公式
	{
		// 坐标
		pBody = (FxjBody*)pNextData;
		lCurLen += pBody->GetLen();

		nNext = 1;

		lCurLen++;
		if( !IsValid(lCurLen,lSubLen,lLength) )
			return -1;

		FxjBody* pPreBody;	

		while( 1 )
		{
			nOff = pBody->m_Coodi.GetOffset();
			if( nOff )
			{
				pBody = (FxjBody*)&pNextData[nNext];
			}
			else
			{
				pPreBody = (FxjBody*)&pNextData[nNext];
				nOff	 = pPreBody->m_Coodi.GetOffset();
				if( nOff )
				{
					if( !pBody->m_Coodi.GetOffset() )
					{
						break;
					}
				}
				else
				{
					pBody = pPreBody;
				}
			}

			lCurLen++;
			if( !IsValid(lCurLen,lSubLen,lLength) )
				return -1;

			nNext++;
		}

		//pNextData = (char*)pBody->m_pNextData;
	}

	FxjSerialString* pSerialString = NULL;
	FxjEidolon* pEidolon = NULL;
	bOneTime = FALSE;

_NeedCheck_:
	{
		// 公式描述
		pSerialString = (FxjSerialString*)pBody->m_pNextData;
		pFxjExpress->m_pDepiction = pSerialString;
		if( pSerialString == NULL )
			return -1;
		if( !IsValid(lCurLen+pSerialString->m_lLength,lSubLen,lLength) )
			return -1;
		lCurLen += pSerialString->GetLen();
		if( !IsValid(lCurLen,lSubLen,lLength) )
			return -1;

		// 公式内容完全加密时为空
		pSerialString = (FxjSerialString*)pSerialString->GetNextData(lLength);
		pFxjExpress->m_pBody = pSerialString;
		if( pSerialString == NULL )
			return -1;
		if( !IsValid(lCurLen+pSerialString->m_lLength,lSubLen,lLength) )
			return -1;
		lCurLen += pSerialString->GetLen();
		if( !IsValid(lCurLen,lSubLen,lLength) )
			return -1;

		// 公式注释
		pSerialString = (FxjSerialString*)pSerialString->GetNextData(lLength);
		pFxjExpress->m_pNotes = pSerialString;
		if( pSerialString == NULL )
			return -1;
		if( !IsValid(lCurLen+pSerialString->m_lLength,lSubLen,lLength) )
			return -1;
		lCurLen += pSerialString->GetLen();
		if( !IsValid(lCurLen,lSubLen,lLength) )
			return -1;

		if(pSerialString != NULL) // ??
		{
			// 参数精灵
			pEidolon = (FxjEidolon*)pSerialString->GetNextData(lLength);
			pFxjExpress->m_pEidolon = pEidolon;

			lCurLen += pEidolon->GetLen();
			if( !IsValid(lCurLen,lSubLen,lLength) )
			{
				pFxjExpress->m_pEidolon = NULL;
			}

			if( !pEidolon->IsValid() )
			{
				if( !bOneTime )
				{
					pBody = (FxjBody*)&pNextData[nNext-5];
					bOneTime = TRUE;
					goto _NeedCheck_;
				}
			}
		}
	}

	if( !IsValid(lCurLen+pEidolon->m_sEidolon.m_lLength,lSubLen,lLength) )
	{
		return -1;
	}

	pNextData = (char*)pEidolon->GetNextData(lLength);

	lCurLen += FxjExpressEnd::GetLen();
	if( !IsValid(lCurLen,lSubLen,lLength) )
	{
		return -1;
	}

	// 一个公式结尾
	FxjExpressEnd* pExpressEnd = (FxjExpressEnd*)pNextData;
	if( pExpressEnd != NULL && !pExpressEnd->IsEnd() )
	{
		pSerialString = (FxjSerialString*)pNextData;
		pFxjExpress->m_pType = pSerialString;
	}

	return 1;
}

BOOL CDlgFormulaMan::IsValid(int nCurPos,int nSubCount,int nMainCount)
{
	return (nCurPos < nSubCount && nCurPos < nMainCount);
}

long CDlgFormulaMan::LoadCurExp(WPARAM wParam /*= 0*/, LPARAM lParam /*= 0*/)
{
	if( lParam == 0 )
	{
		EnableItem(FALSE);
		return 0;
	}

	EnableItem(TRUE);

	if( wParam == CCompileEditView::UpdateExpress )
	{
		CDlgFormulaEdit* pDlg = GetExpDlg((CExpression*)lParam);
		if( pDlg != NULL )
		{
			if(pDlg->SendMessage(HX_USER_LOADCUR,wParam,lParam))
			{
				Enable(FALSE);
			}
		}
		return 0;
	}

	CExpression* pExpression = (CExpression*)lParam;
	if( pExpression == NULL )
		return 0;

	CDlgNotesAndEidolon* pDlgNotes = m_pDlgNotes;
	CDlgNotesAndEidolon* pDlgParam = m_pDlgParam;

	if( pDlgNotes != NULL && pDlgNotes->IsWindowVisible() )
		pDlgNotes->ShowWindow(SW_HIDE);
	if( pDlgParam != NULL && pDlgParam->IsWindowVisible() )
		pDlgParam->ShowWindow(SW_HIDE);

	return 0;
}

void  CDlgFormulaMan::SetNotes(CString strNotes,CString strCaption)
{
	CDlgNotesAndEidolon* pDlg =	GetDlgNotes();
	if( pDlg != NULL )
	{
		pDlg->SetNotes(strNotes, strCaption);
	}
}

BOOL CDlgFormulaMan::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class

	CTreeCtrlFormula* pTree = GetCurTree();

	if(pMsg->message >= WM_KEYFIRST && pMsg->message <= WM_KEYLAST)
	{
		switch(pMsg->wParam)
		{
			///begin////修改“11、	从主菜单系统工具打开公式管理后，
			//////弹出的窗口ESC无效“ＢＵＧ,加上ShowWindowEx(SW_HIDE);
		case VK_ESCAPE:
			{
				//qinhn 20091023 Add 防止按Esc键退出公式查找，修改对话框的时候，整个公式管理对话框也会关闭的情况
				if (CDlgFormulaFind::bEscMsgOfDlgFind||CDlgFormulaEdit::bEscMsgOfDlgEdit ) 
				{
					CDlgFormulaFind::bEscMsgOfDlgFind = FALSE;
					CDlgFormulaEdit::bEscMsgOfDlgEdit = FALSE;
					return TRUE;
				}
				CDlgFormulaFind::bEscMsgOfDlgFind = FALSE;
				CDlgFormulaEdit::bEscMsgOfDlgEdit = FALSE;
				//qinhn 20091023 Add End
				ShowWindow(SW_HIDE);
				SetActiveMain(this);

				return FALSE;
			}
			///end////
		case VK_RETURN:
			{

				CWnd* pWnd = this->GetFocus();
				if( pTree != NULL && (pTree == pWnd || pTree->IsChild(pWnd)) )
				{
					if(pTree->IsChild(pWnd))
					{
						if( pWnd->IsKindOf(RUNTIME_CLASS(CEdit)) )
							pWnd->ShowWindow(SW_HIDE);
					}
					else
					{
						pTree->SendMessage(pMsg->message,pMsg->wParam,pMsg->lParam);
						return 1;
					}
					return 0;
				}
			}
			break;
		}
	}

	if( pTree != NULL /*&& pMsg->hwnd == pTree->m_hWnd*/ )
	{
		pTree->PreTranslateMessage(pMsg);
	}

	return CHSBaseDialog::PreTranslateMessage(pMsg);
}

BOOL CDlgFormulaMan::Key(UINT nKey)
{
	int nIndex = m_wndTab.GetCurSel();

	if( IsCTRLpressed() )
	{
		if(nKey == VK_LEFT)
		{
			nIndex--;
			if(nIndex < 0)
			{
				nIndex = m_wndTab.GetItemCount()-1;
			}
		}
		else if(nKey == VK_RIGHT)
		{
			nIndex++;
			if(nIndex >= m_wndTab.GetItemCount())
			{
				nIndex = 0;
			}
		}
		m_wndTab.SetCurSel(nIndex);
		LRESULT pResult;
		OnSelchangeTab(NULL, &pResult);
		return TRUE;
	}
	return FALSE;
}

void CDlgFormulaMan::OnParam() 
{
	CTreeCtrlFormula* pTree = GetCurTree();
	if(pTree == NULL)
		return;

	HTREEITEM hItem = pTree->GetSelectedItem();
	if( hItem == NULL )
		return;

	CTreeGroup* pTreeGroup = (CTreeGroup*)pTree->GetItemData(hItem);
	if( pTreeGroup == NULL || 
		!pTreeGroup->IsStatus(hx_GroupItem) )
		return;

	CExpression* pExpression = pTreeGroup->GetData();
	if(pExpression == NULL)
		return;

	CDlgNotesAndEidolon* pDlg = GetDlgParam();
	if(pDlg != NULL)
	{		
		pDlg->m_pExpression = pExpression;
		CString strText;
		strText.Format("%s",pExpression->GetEidolon());
		if(!strText.IsEmpty())
		{
			pDlg->SetEidolon(strText,pExpression->GetParamVar(),pExpression->GetName());
		}
		else
		{
			CStringArray* pArray = pExpression->GetDefEidolon();
			if(pArray->GetSize() <= 1)
			{
				pArray = NULL;
			}
			pDlg->SetEidolon(pArray,pExpression->GetParamVar(),pExpression->GetName());
		}					
		pDlg->ShowWindow(SW_SHOWNORMAL);
	}	
}

long CDlgFormulaMan::OnParam(WPARAM wParam,LPARAM lParam)
{
	HSOpenExpressUseData* pData = (HSOpenExpressUseData*)lParam;
	if( pData == NULL )
		return 0;
	CExpression* pExpression = (CExpression*)pData->m_pData;
	if( pExpression == NULL )
		return 0;

	CDlgNotesAndEidolon* pDlg = GetDlgParam();
	if(pDlg != NULL)
	{		
		pDlg->m_pExpression = pExpression;
		CString strText;
		strText.Format("%s",pExpression->GetEidolon());
		if(!strText.IsEmpty())
		{
			pDlg->SetEidolon(strText,pExpression->GetParamVar(),pExpression->GetName());
		}
		else
		{
			CStringArray* pArray = pExpression->GetDefEidolon();
			if(pArray->GetSize() <= 1)
			{
				pArray = NULL;
			}
			pDlg->SetEidolon(pArray,pExpression->GetParamVar(),pExpression->GetName());
		}					
		pDlg->ShowWindow(SW_SHOWNORMAL);
	}
	return 1;


}

void CDlgFormulaMan::EnableItem(BOOL bEnable)
{
	CWnd* pWnd;

	pWnd = GetDlgItem(IDD_UPDATE);
	pWnd->EnableWindow(bEnable);

	pWnd = GetDlgItem(IDD_DELETE);
	pWnd->EnableWindow(bEnable);

	pWnd = GetDlgItem(IDC_NOTES);
	pWnd->EnableWindow(bEnable);

	pWnd = GetDlgItem(IDC_PARAM);
	pWnd->EnableWindow(bEnable);
}

void CDlgFormulaMan::Enable(BOOL bEnable,HSOpenExpressUseData* pData /*= NULL*/)
{
	int nType = -1;
	if(pData != NULL)
	{
		nType = ((*pData).m_wType & 0x000f);
	}

	CWnd* pWnd = GetDlgItem(IDD_NEW);
	pWnd->EnableWindow(bEnable);

	CTreeCtrlFormula* pTree = GetCurTree();
	if(bEnable && pTree != NULL)
	{
		CTreeCtrlFormula* pTree = GetCurTree();
		if(pTree != NULL)
		{
			pTree->SetFocus();
			pTree->Invalidate(FALSE);
		}

		pWnd = GetDlgItem(IDD_UPDATE);
		pWnd->EnableWindow(bEnable);

		pWnd = GetDlgItem(IDD_DELETE);
		pWnd->EnableWindow(bEnable);

		pWnd = GetDlgItem(IDC_NOTES);
		pWnd->EnableWindow(bEnable);

		pWnd = GetDlgItem(IDC_PARAM);
		pWnd->EnableWindow(bEnable);
		/*
		HTREEITEM hItem = pTree->GetSelectedItem();
		CTreeGroup* pTreeGroup = (CTreeGroup*)pTree->GetItemData(hItem);
		if( pTreeGroup == NULL || 
		!pTreeGroup->IsStatus(hx_GroupItem) )
		{
		SendMessage(HX_USER_LOADCUR,0,NULL);
		}
		*/
	}
	else
	{
		pWnd = GetDlgItem(IDD_UPDATE);
		pWnd->EnableWindow(bEnable);

		pWnd = GetDlgItem(IDD_DELETE);
		pWnd->EnableWindow(bEnable);

		pWnd = GetDlgItem(IDC_NOTES);
		pWnd->EnableWindow(bEnable);

		pWnd = GetDlgItem(IDC_PARAM);
		pWnd->EnableWindow(bEnable);
	}

	m_wndTab.EnableWindow(bEnable);

	for(int nTab = 0; nTab < m_wndTab.GetItemCount(); nTab ++)
	{
		pWnd = m_wndTab.GetDlgItem(USER_LIST+nTab);
		if( nType == Tech && nTab == 0 )
		{
			m_wndTab.SetCurSel(nTab);
			pWnd->EnableWindow(TRUE);
		}
		else
		{
			pWnd->EnableWindow(bEnable);
		}
	}

	pWnd = GetDlgItem(IDD_IN);
	pWnd->EnableWindow(bEnable);

	pWnd = GetDlgItem(IDD_OUT);
	pWnd->EnableWindow(bEnable);

	pWnd = GetDlgItem(IDD_TEMPIN);
	pWnd->EnableWindow(bEnable);

	pWnd = GetDlgItem(IDCANCEL);
	pWnd->EnableWindow(TRUE);

	////
	CDlgNotesAndEidolon* pDlgNotes = GetDlgNotes();
	CDlgNotesAndEidolon* pDlgParam = GetDlgParam();
	if( pDlgNotes != NULL && pDlgNotes->IsWindowVisible() )
		pDlgNotes->ShowWindow(SW_HIDE);
	if( pDlgParam != NULL && pDlgParam->IsWindowVisible() )
		pDlgParam->ShowWindow(SW_HIDE);

}

CTreeCtrlFormula* CDlgFormulaMan::GetCurTree(int nIndex /*= -1*/)
{
	if( !::IsWindow(m_wndTab.m_hWnd) )
		return NULL;

	if(nIndex == -1)
	{
		nIndex = m_wndTab.GetCurSel();
	}

	if( nIndex >= 0 && nIndex < m_pAyTree.GetSize() )
		return m_pAyTree.GetAt(nIndex);

	return NULL;
}

CTreeCtrlFormula* CDlgFormulaMan::GetCurTree(WORD nMask)
{
	CTreeCtrlFormula* pTree;
	for(int nIndex = 0 ; nIndex < m_pAyTree.GetSize();nIndex++ )
	{
		pTree = m_pAyTree.GetAt(nIndex);
		if( pTree->GetType() == nMask )
		{
			return pTree;
		}
	}

	return NULL;
}

CTreeCtrlFormula* CDlgFormulaMan::GetCurTree(CExpression* pExpresie)
{
	if( pExpresie == NULL )
		return NULL;

	return GetCurTree( (WORD)pExpresie->GetExpressType() );
}

CTreeCtrlFormula* CDlgFormulaMan::GetCurTree(CString strName)
{
	if( !::IsWindow(m_wndTab.m_hWnd) )
		return NULL;

	CTreeCtrlFormula* pTree;
	for( int nIndex = 0 ;nIndex < m_pAyTree.GetSize() ;nIndex++)
	{
		pTree = m_pAyTree.GetAt(nIndex);
		if( pTree->IsTree(strName) )
			return pTree;
	}

	return NULL;
}

void CDlgFormulaMan::ShowTree(CTreeCtrlFormula* pCurTree,BOOL bShow /*= TRUE*/)
{
	for(int i = 0; i < m_pAyTree.GetSize(); i++)
	{
		CTreeCtrlFormula* pTree = m_pAyTree.GetAt(i);
		if( pCurTree != pTree )
		{
			if( pTree->IsWindowVisible() )
			{
				pTree->ShowWindow(SW_HIDE);
			}
		}
		else if( pCurTree == pTree )
		{
			m_wndTab.SetCurSel(i);
		}
	}

	if( bShow )
	{
		if( pCurTree != NULL && 
			!pCurTree->IsWindowVisible() )
		{
			pCurTree->ShowWindow(SW_SHOW);
		}
	}
}

void CDlgFormulaMan::OnSize(UINT nType, int cx, int cy) 
{
	CHSBaseDialog::OnSize(nType, cx, cy);

	if( abs(cx) + abs(cy) > 0 ) 
	{
		SetSize();
	}
}

void CDlgFormulaMan::SetSize()
{
	CWnd* pHorWnd = GetDlgItem(IDD_OUT);
	CWnd* pVerWnd = GetDlgItem(IDC_BUTFIND);

	if(pVerWnd == NULL)
		return;

	CRect rcClient,rect,rcItem,rcHor,rcVer;

	GetClientRect(rcClient);

	int nBorderMargin = 10;

	pHorWnd->GetClientRect(rcHor);
	pVerWnd->GetClientRect(rcVer);

	rect = rcClient;
	rect.top    = nBorderMargin;
	rect.left   = nBorderMargin;
	rect.bottom = rect.top + rcClient.Height()  - rcHor.Height() - nBorderMargin*3;
	rect.right  = rect.left + rcClient.Width()  - rcVer.Width()  - nBorderMargin*3;

	m_wndTab.MoveWindow(rect);

	// ver
	rcItem.left   = rect.right + nBorderMargin;
	rcItem.right  = rcItem.left + rcVer.Width();
	rcItem.bottom = rect.bottom - rcVer.Height()*5 - nBorderMargin*5/2;
	rcItem.top    = rcItem.bottom - rcVer.Height();
	pVerWnd->MoveWindow(rcItem);

	pVerWnd = GetDlgItem(IDD_NEW);
	rcItem.OffsetRect(0,rcVer.Height()+nBorderMargin/2);
	pVerWnd->MoveWindow(rcItem);

	pVerWnd = GetDlgItem(IDD_UPDATE);
	rcItem.OffsetRect(0,rcVer.Height()+nBorderMargin/2);
	pVerWnd->MoveWindow(rcItem);

	pVerWnd = GetDlgItem(IDD_DELETE);
	rcItem.OffsetRect(0,rcVer.Height()+nBorderMargin/2);
	pVerWnd->MoveWindow(rcItem);

	pVerWnd = GetDlgItem(IDC_NOTES);
	rcItem.OffsetRect(0,rcVer.Height()+nBorderMargin/2);
	pVerWnd->MoveWindow(rcItem);

	pVerWnd = GetDlgItem(IDC_PARAM);
	rcItem.OffsetRect(0,rcVer.Height()+nBorderMargin/2);
	pVerWnd->MoveWindow(rcItem);

	// hor
	rcItem.right  = rect.right - rcHor.Width()*3 - nBorderMargin*3/2;
	rcItem.left   = rcItem.right - rcHor.Width();
	rcItem.top    = rect.bottom + nBorderMargin;
	rcItem.bottom = rcItem.top + rcHor.Height();
	pHorWnd->MoveWindow(rcItem);

	pHorWnd = GetDlgItem(IDD_IN);
	rcItem.OffsetRect(rcHor.Width()+nBorderMargin/2,0);
	pHorWnd->MoveWindow(rcItem);

	pHorWnd = GetDlgItem(IDD_TEMPIN);
	rcItem.OffsetRect(rcHor.Width()+nBorderMargin/2,0);
	pHorWnd->MoveWindow(rcItem);

	pHorWnd = GetDlgItem(IDCANCEL);
	rcItem.OffsetRect(rcHor.Width()+nBorderMargin/2,0);
	pHorWnd->MoveWindow(rcItem);

	//
	CTreeCtrlFormula* pTree = GetCurTree();
	if( pTree != NULL )
	{
		m_wndTab.GetItemRect(0,rcItem);
		m_wndTab.GetClientRect(rect);
		rect.top = rcItem.bottom + 1;
		pTree->MoveWindow(rect,FALSE);
		ShowTree(pTree,TRUE);
	}

	//this->Invalidate(FALSE);
}

void CDlgFormulaMan::CreateIndividualInfo()
{
	CopyTree();
}


CTreeCtrlFormula* CDlgFormulaMan::GetIndividualInfo(void* pData /*= NULL*/)
{
	//if(m_wndFavorate != NULL )
	//	return m_wndFavorate;

	HSOpenExpressUseData* ppData = (HSOpenExpressUseData*)pData;
	if( ppData == NULL ) return NULL; 

	CWnd* pWnd = (CWnd*)ppData->m_pData;
	if( pWnd == NULL ) return NULL;

	CTreeCtrlFormula* wndFavorate = new CTreeCtrlFormula;

	CRect rect;
	pWnd->GetClientRect(rect);

	if( !wndFavorate->Create ( WS_CHILD/*|WS_BORDER*/|TVS_HASLINES|/*TVS_LINESATROOT|*/
		TVS_HASBUTTONS|TVS_SHOWSELALWAYS|WS_VISIBLE , 
		rect,pWnd,1001 ) )
	{
		TRACE0("Failed to create workspace view\n");
		delete wndFavorate;
		wndFavorate = NULL;
		return 0;
	}

	ExternParam* pExternParam = new ExternParam; 
	/*::HSMapAdd(m_ayExtern,wndFavorate,pExternParam);*/
	m_ayExtern.SetAt(wndFavorate, pExternParam);

	//wndFavorate->ModifyStyleEx(0,WS_EX_CLIENTEDGE);
	wndFavorate->SetImageList (&CDlgFormulaMan::m_expressImages, TVSIL_NORMAL);
	wndFavorate->m_pFormulaMan = this;

	wndFavorate->SetType(CTreeCtrlFormula::AlignWin);

	// 由类型填充数据

	WORD nType = (ppData->m_wType & 0xF); // 公式类别

	int nCopy = CTreeCtrlFormula::CopyNone;
	if( ppData->m_wType & hxShowByExpressType )
	{
		if( (ppData->m_wType & 0xF0) == hxShowByExpressType_AddTech )
		{
			nCopy = CTreeCtrlFormula::CopyNotGroup | CTreeCtrlFormula::CopyNone | HS_EXP_MAINCHART;
		}
	}

	if( HS_EXP_OUTTYPE(ppData->m_dStyle) )
	{
		nCopy &= ~CTreeCtrlFormula::CopyNotGroup; 
		nCopy |= CTreeCtrlFormula::CopyDelete;
		//nCopy |= CTreeCtrlFormula::CopyNoRoot;
		nCopy |= CTreeCtrlFormula::CopyMustHasChildren;
	}

	if( nType != 0 )
	{
		CTreeCtrlFormula* pTree;
		for( int i = 0; i < 4; i++ )
		{
			switch(i)
			{
			case 0:
				nType = ((ppData->m_wType & 0xF) & Tech);
				break;
			case 1:
				nType = ((ppData->m_wType & 0xF) & Condition);
				break;
			case 2:
				nType = ((ppData->m_wType & 0xF) & Exchange);
				break;
			case 3:
				nType = ((ppData->m_wType & 0xF) & MoreKLine);
				break;
			case 4: //20090302 YJT 增加 Strategy
				nType = ((ppData->m_wType & 0xFF) & Strategy);
				break;
			}

			pTree = GetCurTree( nType );
			wndFavorate->CopyTree(pTree,nCopy,ppData->m_dStyle);		
		}
		
		pExternParam->m_nCopy	= nCopy;
		pExternParam->m_dwStyle = ppData->m_dStyle;
		pExternParam->m_dwType	=  ppData->m_wType ;
	}
	else
	{
		CopyTree(wndFavorate);
		pExternParam->m_nCopy = nCopy;
		pExternParam->m_dwStyle = 0;
	}

	if(nCopy & CTreeCtrlFormula::CopyMustHasChildren)
	{
		wndFavorate->DelAllNoChildren();
	}

	return wndFavorate;
}
void CDlgFormulaMan::Save()
{
	for(int i = 0; i < m_pAyTree.GetSize(); i++)
	{
		CTreeCtrlFormula* pTree = m_pAyTree.GetAt(i);
		if( pTree != NULL )
		{
			pTree->Save(pTree->GetType(),NULL);
		}
	}
}

long CDlgFormulaMan::Open(StrategyInfo* pData)
{
	HSOpenExpressUseData data(hxShowByExpressType|Strategy);
	CString strName = _T("策略交易参数");
	data.m_strName  = &strName;
	data.m_hParent  = this->m_hWnd;
	CDlgStrategyJY dlg(this,&data);//CDlgStrategyMain dlg(this,&data);
	dlg.m_pInfo = pData;
	if( data.m_hParent != NULL )
		dlg.m_hParent = data.m_hParent;
	dlg.DoModal();
	return 0;
}

long CDlgFormulaMan::Open(StrategyHis * pData)
{
	HSOpenExpressUseData *data = new HSOpenExpressUseData(hxShowByExpressType|Strategy);
	CString strName = _T("策略交易可靠性测试");
	data->m_strName  = &strName;
	HWND hwnd;
	CWnd *pwnd = NULL;
	if(pData->m_lHolding > 0)
	{
		pwnd = FromHandle((HWND)pData->m_lHolding);
		hwnd = (HWND)pData->m_lHolding;
	}
	if (!pwnd)
	{
		hwnd = this->m_hWnd;
		pwnd = this;
	}
	data->m_hParent  = hwnd;//this->m_hWnd;
	CDlgTestStrategy *dlg = new CDlgTestStrategy(pwnd,data);//(this,data);
 	dlg->m_pStrategyHis = pData;
 	if( data->m_hParent != NULL )
 		dlg->m_hParent = data->m_hParent;
	//dlg->Create(IDD_STRATEGY_TEST);
	//dlg->ShowWindow(SW_SHOW);
	dlg->DoModal();
	return 0;
}
BOOL CDlgFormulaMan::ReadStrategy(CMapWordToPtr *& pmap)
{
	CFile file;
	CString path;
//	path = CHSTools::HSGetCurrentExeBasePath() + "StrategyTrade\\";
	path =  CGeneralHelper::GetAppPath() + "StrategyTrade\\";
	if (_access(path, 0) != 0)
	{
		_mkdir(path);
	}
	path += "StrategyTrig.dat";
	if (!file.Open(path,CFile::modeRead))
	{
		return FALSE;
	}
	CArchive ar(&file,CArchive::load);
	int n;
	ar>>n;
	for(int k=0; k<n; k++)
	{
		StrategyTrig *Trig = new StrategyTrig;
		CString name;
		ar>>name;
		/*ar>>explain;*/
		strncpy(Trig->m_cName,name.GetBuffer(),32);
		ar>>Trig->m_bFuQuan;
		ar>>Trig->m_lStartTime;
		ar>>Trig->m_lEndTime;
		ar>>Trig->m_nGap;
		ar>>Trig->m_nMaxBuyTimes;
		ar>>Trig->m_nMaxSellTimes;
		ar>>Trig->m_nPeriodType;
		ar>>Trig->m_nPeriodNumber;
		ar>>Trig->m_nReserve;
		if (!Trig->m_pExpValue)
		{
			Trig->m_pExpValue = new CExpValue;
		}
		SerializeExp(ar,Trig->m_pExpValue);	
		pmap->SetAt(k+1,Trig);
	}
	return TRUE;
}

void CDlgFormulaMan::SerializeExp(CArchive & ar,CExpValue *& exp)
{
	CExpression *pex = exp->GetExp();
	if (!pex)
	{
		pex = new CExpression;
	}
	if (ar.IsStoring())
	{
		CString sname = pex->GetName();
		ar<<sname;
		if(pex->GetParamVar() != NULL)
		{
			CArray<class CValue*,class CValue*>* pArray = pex->GetParamVar()->GetObjArray();
			if( pArray != NULL )
			{
				ar << pArray->GetSize();

				CValue* pCurVal;	
				CString strName;
				for(int nObj = 0; nObj < pArray->GetSize(); nObj++)
				{
					pCurVal = pArray->GetAt(nObj);
					strName = pex->GetParamVar()->GetObjectName(pCurVal);
					ar << strName;
					pCurVal->Serialize(ar);
				}
			}
			else
			{
				ar << 0;
			}
		}
		else
		{
			ar << 0;
		}

	}
	else
	{
		CString sname;
		int nCountVar;
		ar >> sname;
		pex->SetName(sname);
		ar >> nCountVar;
		for(short i = 0; i < nCountVar;i++)
		{
			CString sName;
			CNumericValue *pVal = new CNumericValue();
			ar >> sName;
			pVal->Serialize(ar);
			pex->AddParamer(sName,pVal);
		}
		exp->SetValue(pex);
	}
}

long CDlgFormulaMan::StrategyManage()
{
	CDlgStrategyManage dlg(this);
	dlg.DoModal();
	return 0;
}
long CDlgFormulaMan::Open(HSOpenExpressUseData* pData)
{
	if( ((pData->m_wType & 0XF00) == hxShowByExpressType) || 
		((pData->m_wType & 0XF00) == hxShowReturnByExpressType) )
	{
		CDlgFormulaBrowser dlg(this,pData);
		if( pData->m_hParent != NULL )
			dlg.m_hParent = pData->m_hParent;
		dlg.DoModal();
	}
	else if( (pData->m_wType & 0XF00) == hxOpenExpressByConditionStockType )  // 智能选股
	{
		long lRet = 0;
		switch(pData->m_wType)
		{
		case hxOpenExpressByConditionStockType:  // 条件选股
			{			
				CDlgCondition dlg(this,pData);
			
				dlg.m_hMsgWnd = pData->m_hParent;
				dlg.m_lRefWnd = (long)(this->m_hWnd);
				dlg.DoModal();			
			}
			break;
		case hxOpenExchangeYouSel : // 交易优选
			{
				CDlgExchOptimize dlg(this);

				dlg.DoModal();
			}
			break;
		case hxOpenExpressByConditionDingZhi:  // 自动安装公式
			{
				AutoSetupExpress(pData);
			}
			break;
		case hxOpenExchangeSys:  // 交易系统
			{
				try
				{
 					CSheetExchMain dlg(this);
					
					dlg.DoModal();
				}
#ifdef VC_7_0
				// 异常处理
				catch(CMemoryException e) //　内存异常
				{
					TCHAR   szCause[255];
					e.GetErrorMessage(szCause, 255);
					CString strError;
					strError.Format("交易系统 MemoryException: %s", szCause);
					// 调试日志
					//WRITE_LOG (__FILE__, __LINE__, _T(strError));
				}
				catch(CFileException e) // 文件异常
				{
					TCHAR   szCause[255];
					e.GetErrorMessage(szCause, 255);
					CString strError;
					strError.Format("交易系统 FileException: %s", szCause);
					// 调试日志
					//WRITE_LOG (__FILE__, __LINE__, _T(strError));
				}
#endif
				catch(...) // 其它异常
				{
					// 调试日志
					CString strError;
					strError.Format("交易系统 other Exception: %s", ::strerror(::GetLastError()));
					//WRITE_LOG (__FILE__, __LINE__, _T(strError));
				}
			}
			break;
		case hxOpenExpressByConditionZhiBiao: // 指标选股打开
			{
				CDlgIndicatorSelStock dlg(this,pData,NULL);

				long** l = (long**)pData->m_pData;
				*l = (long*)&dlg;

				dlg.DoModal();

				*l = 0;
			}
			break;
		case hxOpenExpressByConditionZhiNeng:  // 智能选股
			{
				CDlgIntelligentSelStock dlg(this,pData,NULL);

				long** l = (long**)pData->m_pData;
				*l = (long*)&dlg;

				dlg.DoModal();

				*l = 0;
			}
			break;
		case hxOpenExpressByConditionChaJian:  // 插件选股
			{
			}
			break;
		}

		return lRet;
	}

	else if( (pData->m_wType & 0XF00) == hxOpenExpressBySortStockType )        // 指标排序
	{
		char cType = ((pData->m_wType & 0XF00) == hxOpenExpressByConditionStockType)?CDlgCondition::ConditionType:CDlgCondition::Sort;
		CDlgCondition dlg(this,pData,cType,NULL);

		long** l = (long**)pData->m_pData;
		*l = (long*)&dlg;

		dlg.DoModal();

		*l = 0;
	}
	else if( (pData->m_wType & 0XF00) == hxOpenBlockAnalysis )
	{
		static CDlgBlockAnalysis* pDlg = NULL;
		if( pDlg == NULL )
		{
			pDlg = new CDlgBlockAnalysis(this);
		/*	pDlg->SetThisParam((long*)&pDlg);*/
		}

		pDlg->ShowWindow(SW_SHOW);
	}
	else if( (pData->m_wType & 0XF00) == hxOpenUniteAnalysis )
	{
		CDlgUniteAnalysis* dlg = new CDlgUniteAnalysis(this);
		dlg->ShowWindow(SW_SHOW);
	}
	else if( (pData->m_wType & 0XF00) == hxOpenAlarmManagerDlg )	//打开预警dlg
	{
		if( g_pWranManagerDlg == NULL)
		{
			g_pWranManagerDlg = new CDlgAlarmMan(this);
		}
		g_pWranManagerDlg->ShowWindow(SW_SHOWNORMAL);
		return (long)g_pWranManagerDlg;
	}
	else if( (pData->m_wType & 0XF00) == hxOpenFindDataDlg ) //打开查找dlg
	{
		if (g_pDlgFormulaFind == NULL)
		{
			g_pDlgFormulaFind = (CDlgFormulaFind*)Find(/*0xFFFF,this*/);
		}
		else
		{
			g_pDlgFormulaFind->ShowWindow(SW_SHOW);
			g_pDlgFormulaFind->SetActiveWindow();
		}
	}
	else if( (pData->m_wType & 0XF000) == hxOpenAddStkToAlarm )
	{
		if( g_pWranManagerDlg == NULL)
		{
			g_pWranManagerDlg = new CDlgAlarmMan(this);
		}
		int nCount = (int)::SendMessage(g_hParentWnd, HX_USER_COMPILEDATA, 32, (LPARAM)-1);
		if( nCount > 0 )
		{
			CDlgAddStockToAlarm dlg(g_pWranManagerDlg,CWnd::FromHandle(g_hParentWnd));
			dlg.DoModal();
		}
		else
		{
			MessageBox(_T("没有加入！"),_T("提示"),MB_ICONINFORMATION);
		}
	}

// 	else if ( (pData->m_wType & 0XF000) == hsOpenStrategyTestSetting)
// 	{
// 		CDlgStrategyTest dlg(this,pData);
// 		if( pData->m_hParent != NULL )
// 			dlg.m_hParent = pData->m_hParent;
// 		dlg.DoModal();
// 	}
	return 0;
}

long CDlgFormulaMan::OnRun(WPARAM wParam, LPARAM lParam)
{
	CWinThread* pThread = AfxGetThread();
	if(pThread != NULL)
	{
		return pThread->Run();
	}
	return 0;
}

BOOL CDlgFormulaMan::SetDefaultTech(CExpression* pExpression,char Operator)
{
	if(m_pOpeDefTechFun == NULL) 
		return FALSE;

	if( pExpression == NULL )
	{
		(*m_pOpeDefTechFun)( NULL,TRUE,NULL,Operator );
	}
	else
	{
		CString strName = pExpression->GetName();

		CValue* pValoare;
		if ( m_pExternExpression->Lookup(strName,pExpression->GetExpressType(),pValoare) )
		{
			(*m_pOpeDefTechFun)( pValoare,TRUE,NULL,Operator );
		}
	}
	
	return TRUE;
}

void CDlgFormulaMan::CopyToFloatTree(int nCopy /*= CTreeCtrlFormula::CopyNone*/)
{
	CTreeCtrlFormula* key = NULL;
	ExternParam* value = NULL;

	POSITION  pos;
	for( pos = m_ayExtern.GetStartPosition(); pos != NULL; )
	{
		m_ayExtern.GetNextAssoc( pos, key, value );
		if( value && key)
		{
			WORD nType = (value->m_dwType & 0xF); // 公式类别
			if( nType != 0)
			{
				CTreeCtrlFormula* pTree;
				for( int i = 0; i < 4; i++ )
				{
					switch(i)
					{
					case 0:
						nType = ((value->m_dwType & 0xF) & Tech);
						break;
					case 1:
						nType = ((value->m_dwType & 0xF) & Condition);
						break;
					case 2:
						nType = ((value->m_dwType & 0xF) & Exchange);
						break;
					case 3:
						nType = ((value->m_dwType & 0xF) & MoreKLine);
						break;
					}

					pTree = GetCurTree( nType );
					key->CopyTree(pTree, value->m_nCopy, value->m_dwStyle);		
				}				
			}
			else
			{
				CopyTree(key, value->m_nCopy/*, value->m_dwStyle*/);
			}

			if(value->m_nCopy & CTreeCtrlFormula::CopyMustHasChildren)
			{
				key->DelAllNoChildren();
			}
		}
		
	}

}

void CDlgFormulaMan::CopyTree(CTreeCtrlFormula* pTree /*= NULL*/,
							  int nCopy /*= CTreeCtrlFormula::CopyNone*/)
{
	if(pTree == NULL)
	{
		return;
		//pTree = m_wndFavorate;
	}

	CTreeCtrlFormula* pCurTree;
	if(pTree != NULL )
	{
		for(int i = 0; i < m_pAyTree.GetSize(); i++)
		{
			pCurTree = m_pAyTree.GetAt(i);
			pTree->CopyTree( pCurTree,
				(nCopy != CTreeCtrlFormula::CopyNone?nCopy:((i == 0)?CTreeCtrlFormula::CopyDelete:CTreeCtrlFormula::CopyNone)) );
		}
	}
}

void CDlgFormulaMan::AddExpress(CMapVariabile* pExternExpression,WORD	wStyle,
								FxjExpress* pFxjExpress,CTreeCtrlFormula* pTree)
{
	if(pFxjExpress == NULL && pFxjExpress->m_pName == NULL)
		return;

	CString strName = pFxjExpress->m_pName->m_strName;

	CExpValue* pValue = NULL;
	CValue* valoare   = NULL;

	CString strWholeName = strName;
	if(pFxjExpress->m_pName->m_nChart == 4 || pFxjExpress->m_pName->m_nChart == 3)
	{
		strWholeName.Insert(0,'1');
	}
	else if(pFxjExpress->m_pName->m_nChart == 5)
	{
		strWholeName.Insert(0,'4');
	}
	else if(pFxjExpress->m_pName->m_nChart == 1)
	{
		strWholeName.Insert(0,'2');
	}
	else
	{
		strWholeName.Insert(0,'3');
	}

	if ( pExternExpression->Lookup(strWholeName,valoare) )
	{
		pValue = (CExpValue*)valoare;
	}
	if(pValue != NULL)
	{
		if( (wStyle & EXTERN_EXPRESS_OVR) )
		{
			pValue->Clean();
		}
		else
		{
			strName = GetIdentifierExpressName(strWholeName,strName,pExternExpression);
			pValue = NULL;
		}
	}

	if(pValue == NULL)
	{
		pValue = new CExpValue;
		(*pExternExpression)[strWholeName/*strName*/] = pValue;
	}

	CExpression* pExpression = new CExpression(NULL,NULL,NULL);
	pValue->SetValue(pExpression);
	pExpression->SetName(strName);
	pValue->Serialize(pFxjExpress);

	pExpression->AddStyle( (wStyle & HS_EXP_TEMPIN) );

	if( valoare != NULL && (wStyle & EXTERN_EXPRESS_OVR) )
	{
		return;
	}
	//
	CString strRoot;
	CString strSub;
	int		nType;
	if( pFxjExpress->m_pType == NULL )
	{
		strSub = CTreeCtrlFormula::g_strDefault;
	}
	else
	{
		strSub = pFxjExpress->m_pType->GetText();
	}

	switch(pFxjExpress->m_pName->m_nChart)
	{
	case 5:
		nType   = MoreKLine;
		strRoot = CTreeCtrlFormula::g_strColorKLine;
		break;
	case 4: //20090302 YJT 修改
		nType   = Strategy;
		strRoot = CTreeCtrlFormula::g_strStrategy;
		break;
	case 3:
		nType   = Tech;
		strRoot = CTreeCtrlFormula::g_strTech;
		break;
	case 0:
		nType   = Condition;
		strRoot = CTreeCtrlFormula::g_strCondition;
		break;
	case 1:
		nType   = Exchange;
		strRoot = CTreeCtrlFormula::g_strExchange;
	default:
		nType   = Tech;
		strRoot = CTreeCtrlFormula::g_strDefault;
		break;
	}
	if(pTree == NULL)
	{
		pTree = GetCurTree(strRoot);
		if(pTree == NULL)
		{
			pTree = GetCurTree(CTreeCtrlFormula::g_strTech);
		}
	}
	if(pTree)
	{
		pTree->InputItem(strRoot,nType,strSub,pValue);
	}
}

void CDlgFormulaMan::AddExpress(CTreeCtrlFormula::LoadItemData* pLoadData,WORD wStyle)
{
	if(pLoadData == NULL)
		return;

	CExpValue* pRefValue  = pLoadData->pExpValue;
	CExpression* pExpress = NULL;
	if(pRefValue != NULL)
	{
		pExpress = pRefValue->GetExp();
	}

	CExpValue* pValue = NULL;
	if(pExpress != NULL)
	{
		CString strName = pExpress->GetName();
		CValue* valoare = NULL;
		if ( m_pExternExpression->Lookup(pExpress->GetWholeName(),valoare) )
		{
			pValue = (CExpValue*)valoare;
		}
		if(pValue != NULL)
		{
			if( (wStyle & EXTERN_EXPRESS_OVR) )
			{
				pValue->Clean();
			}
			else //if( wStyle & EXTERN_EXPRESS_AUTO_NAME )
			{
				strName = GetIdentifierExpressName(pExpress->GetWholeName(),strName,m_pExternExpression);
				pValue = NULL;
			}
		}

		BOOL bAdd = FALSE;
		if(pValue == NULL)
		{
			pValue = new CExpValue;
			bAdd = TRUE;
		}

		CExpression* pExpression = new CExpression(NULL,NULL,NULL);
		pValue->SetValue(pExpression);
		pExpression->SetName(strName);
		pValue->Serialize(pExpress);

		SetDefaultTech(pExpression,'+');

		if( bAdd )
		{
			m_pExternExpression->AddExpress(strName,pValue);
		}

		if( valoare != NULL && (wStyle & EXTERN_EXPRESS_OVR) )
		{
			return;
		}
	}

	//
	CString strRoot;
	CString strSub;
	int		nType = Tech;
	if( pLoadData->strName.IsEmpty() )
	{
		strSub = CTreeCtrlFormula::g_strDefault;
	}
	else
	{
		strSub = pLoadData->strName;
	}

	switch(pLoadData->nExpressType)
	{
	case MoreKLine:
		nType   = MoreKLine;
		strRoot = CTreeCtrlFormula::g_strColorKLine;
		break;
	case Tech:
		nType   = Tech; //
		strRoot = CTreeCtrlFormula::g_strTech;
		break;
	case Condition:
		nType   = Condition; //
		strRoot = CTreeCtrlFormula::g_strCondition;
		break;
	case Exchange:
		nType   = Exchange; //
		strRoot = CTreeCtrlFormula::g_strExchange;
		break; //
	case Strategy: //20090302 YJT 增加 Strategy
		nType   = Strategy; //
		strRoot = CTreeCtrlFormula::g_strStrategy;
		break; //
	default:
		nType   = Tech;
		strRoot = CTreeCtrlFormula::g_strDefault;
		break;
	}
	CTreeCtrlFormula* pTree = GetCurTree(strRoot);
	if(pTree == NULL)
	{
		pTree = GetCurTree(CTreeCtrlFormula::g_strTech);
	}
	if(pTree)
	{
		pTree->InputItem(strRoot,nType,strSub,pValue);
	}
}
void  CDlgFormulaMan::GetColumnProt()
{
	if ( g_pTableHeadData == NULL)
		g_pTableHeadData = new CMapStringToPtr;
	if ( m_pColProt )
	{
		//报价行情字段
		CColPropIDIndex* pColIndex = new CColPropIDIndex;	
		if (m_pColProt->GetColumnGroupPropEx(*pColIndex,HQ_BASE_NAME))
			g_pTableHeadData->SetAt(HQ_BASE_NAME, (void*)pColIndex);
		
		//股本数据
		pColIndex = new CColPropIDIndex;	
		if (m_pColProt->GetColumnGroupPropEx(*pColIndex,GB_GUBEN_NAME))
			g_pTableHeadData->SetAt(GB_GUBEN_NAME, (void*)pColIndex);
		
		//财务数据
		pColIndex = new CColPropIDIndex;		
		if (m_pColProt->GetColumnGroupPropEx(*pColIndex,Cj_GUBEN_NAME))
			g_pTableHeadData->SetAt(Cj_GUBEN_NAME, (void*)pColIndex);
		
		//联动分析
		pColIndex = new CColPropIDIndex;
		if ( m_pColProt->GetColumnGroupPropEx(*pColIndex,LOCK_NAME)) 
			g_pTableHeadData->SetAt(LOCK_NAME, (void*)pColIndex);
	}
}
//by HS
void CDlgFormulaMan::OnButfind() 
{
	// TODO: Add your control notification handler code here
	//	new CDlgFormulaFind(CWnd::FromHandle(g_hParentWnd)); // HS replace
	//end

	//qinhn 20091023 replace "Find(0xFFFF,this);"by following code
	if (g_pDlgFormulaFind == NULL)
	{
		g_pDlgFormulaFind = (CDlgFormulaFind*)Find(0xFFFF,this);
	}
	else
	{
		g_pDlgFormulaFind->ShowWindow(SW_SHOW);
		g_pDlgFormulaFind->SetActiveWindow();
	}
	//qinhn 20091023 replace End

//	Find(0xFFFF,this);

}

BOOL CDlgFormulaMan::ShowWindow( int nCmdShow )
{

	if( !::IsWindow(this->m_hWnd) )
		return 0;

	if( nCmdShow == SW_HIDE || nCmdShow == SW_MINIMIZE )
		return CHSBaseDialog::ShowWindow(nCmdShow);


	if( IsIconic() )
	{
		return CHSBaseDialog::ShowWindow(SW_RESTORE);
	}

	if(  IsZoomed() ) // max
		return CHSBaseDialog::ShowWindow(nCmdShow);

	if( ::AnimateWindow(m_hWnd,100,AW_CENTER | AW_ACTIVATE) )
	{
		this->Invalidate();
	}
	else
	{
		return CHSBaseDialog::ShowWindow(nCmdShow);
	}

	return 1;
}

LRESULT CDlgFormulaMan::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	return CHSBaseDialog::WindowProc(message, wParam, lParam);
}

void CDlgFormulaMan::MyNcDestroy()
{
	//delete this;
}

long CDlgFormulaMan::Find(DWORD dwMask/* = 0xFFFF*/,CWnd* pWnd /*= NULL*/)
{
	if( pWnd == NULL )
		pWnd = CWnd::FromHandle(g_hParentWnd);

	CDlgFormulaFind* pDlg = new CDlgFormulaFind(dwMask,pWnd);
	return (long)pDlg;
	
}

LRESULT CDlgFormulaMan::OnUserDragData(WPARAM wParam, LPARAM lParam)
{
	if(wParam == HSCurKeyInfo::UpdateExpressData/*HX_USER_DRAGDATA-1*/)
	{
		CTreeCtrlFormula* pCurTree = NULL;

		CExpression* pExpression = (CExpression*)lParam;
		if( pExpression != NULL )
		{
			char cStyle = pExpression->GetExpressType();
			switch( cStyle )
			{
			case Tech :
				{
					this->m_wndTab.SetCurSel(0);
					break;
				}
			case Exchange :
				{
					this->m_wndTab.SetCurSel(1);
					break;
				}
			case Condition :
				{
					this->m_wndTab.SetCurSel(2);
					break;
				}
			case MoreKLine :
				{
					this->m_wndTab.SetCurSel(3);
					break;
				}
			case Strategy : //20090302 YJT 增加 Strategy
				{
					this->m_wndTab.SetCurSel(4);
					break;
				}
			default :
				{
					return 0;
				}
			}
			pCurTree = GetCurTree(m_wndTab.GetCurSel());
			if(pCurTree == NULL ) return 0;
			SetSize();
			ShowTree(pCurTree,TRUE);

			pCurTree = this->GetCurTree(pExpression);
		}

		if( pCurTree == NULL )  return 0;

		HTREEITEM hFindItem;
		CTreeGroup* pTreeGroup = pCurTree->Find(pExpression);
		if( pTreeGroup == NULL ) return 0;

		hFindItem = pTreeGroup->m_hTree;
		pCurTree->SelectItem(hFindItem);

		return 1;
	}
	else if(wParam == HSCurKeyInfo::UpdateDefExpress)
	{
		CExpression* pExpression = (CExpression*)lParam;
		/* 更新树对应条目状态 */
		CTreeCtrlFormula* pTree = GetCurTree(pExpression);
		if(pTree)
		{
			HTREEITEM hSub = NULL;
			CTreeGroup* pTreeGroup = pTree->Find(pExpression);
			if(pTreeGroup)
			{
				hSub = pTreeGroup->m_hTree;
			}

			if(hSub)
			{
				UINT nState = pTree->GetItemState(hSub, TVIF_STATE|TVIS_BOLD);
				if(pExpression->IsStyle(HS_EXP_DEFAULT_TECH))
				{
					nState |= TVIS_BOLD;
				}
				else
				{
					nState &= (~TVIS_BOLD);
				}			
				pTree->SetItemState(hSub, nState, TVIF_STATE|TVIS_BOLD);
			}
		}
		return 1;
	}
	else
	{
	}
	return 0;
}

LRESULT CDlgFormulaMan::OnExchangData(WPARAM wParam, LPARAM lParam)
{
	
	StockUserInfo stockinfo;
// 	//根据代码获取股票信息
// 	if (wParam == HS_GETSTOCK_BYCODE)
// 	{
// 		CodeInfo* code = (CodeInfo*)lParam;
// 		m_pDataSource->HSDataSourceEx_GetStockUserInfo(code, &stockinfo);
// 		return (LRESULT)stockinfo;
// 	}
// 	//获取市场股票列表
// 	else if ( HS_GETSTOCK_BYMAKET )
// 	{
// 		
// 	}
	return 0;
}


//// 加入 快速创建公式 
//long CDlgFormulaMan::SpeedCreateTech()
//{
//	CDlgFormulaEdit* pDlg = GetExpDlg();
//	if( pDlg != NULL )
//	{
//		pDlg->SpeedCreateExpress();
//		ShowFomulaDlg(pDlg);
//	}
//
//	return 1;
//}

// gbq add 20060209, 公式查找
CExpression* CDlgFormulaMan::FindExpression(CString strKey)
{
	strKey.MakeUpper();
	CMapVariabile* pVarsMap = CDlgFormulaMan::m_pExternExpression;
	if(pVarsMap != NULL)
	{
		POSITION pos = pVarsMap->GetStartPosition();

		CString sName;
		CValue *pVal;
		while (pos != NULL)
		{			
			pVarsMap->GetNextAssoc (pos, sName, pVal);

			if(pVal != NULL)
			{
				if( pVal->IsStyle(HX_EXPRESSSENTENCE) )
				{
					CString strRet;
					if( ((CExpValue*)pVal)->Find(strKey,strRet, 512 /*CExpression::FindInName*/) )
					{
						CExpression* pExpression = ((CExpValue*)pVal)->GetExp();
						if(pExpression )
							return pExpression;
					}
				}
			}
		}
	}
	return NULL;
}

CExpression* CDlgFormulaMan::ShowExpression(CString strName)
{
	CExpression *pExpression = FindExpression(strName);
	if( pExpression == NULL )
		return NULL;
	WORD dwMask = pExpression->GetExpressType();
	dwMask &= 0xf;
	CTreeCtrlFormula* pTree = GetCurTree(dwMask);
	//CTreeCtrlFormula* pTree = GetCurTree(pExpression);
	pTree->Show(pExpression, 1);
	ShowTree(pTree);

	return pExpression;
}
// gbq add end
BOOL CDlgFormulaMan::OnEraseBkgnd(CDC* pDC)
{
	//return TRUE;
	return CHSBaseDialog::OnEraseBkgnd(pDC);
}

void CDlgFormulaMan::SendLinkMsg( WPARAM wParam , LPARAM lParam )
{
	if ( m_pMainWnd )
	{
		if ( wParam == DiagramMsg_FormulaParamChanged )
		{
			ExpPropery* pExp = (ExpPropery*)lParam;
			if ( pExp )
			{
				CString strId,strStyle;
				strId.Format("%d", DiagramMsg_FormulaParamChanged);
				strStyle.Format("%d", pExp->m_bStyle);
				CLinkageParam param;
				param.m_bPost = FALSE;
				param.m_nMsg = QUOTE_MESSAGE_NOTIFY_DIAGRAM_LAYOUT;
				param.AddParam(PARAM_DIAGRAMMSGID, strId);         // 添加消息号
				param.AddParam(PARAM_DIAGRAM_LAYOUT_NAME, Layout_TechAnalysis);
				param.AddParam(Param_TechAnalyas_ExpressName, pExp->m_strName);
				param.AddParam(Param_TechAnalyas_ExpressType, (CString)pExp->m_dExpType);
				param.AddParam(Param_TechAnalyse_ExpressStyle,strStyle);
				m_pMainWnd->DispatchLinkageMsg(QUOTE_MESSAGE_NOTIFY_DIAGRAM_LAYOUT,(WPARAM)&param,0,FALSE);
			}
		}
		else if ( wParam == DiagramMsg_BlockChange)
		{
			CString strFilename = (CString)(char*)lParam;
			CString strId;
			strId.Format("%d", DiagramMsg_BlockChange);
			CLinkageParam param;
			param.m_bPost = FALSE;
			param.m_nMsg = QUOTE_MESSAGE_NOTIFY_DIAGRAM_LAYOUT;
			param.AddParam(PARAM_DIAGRAMMSGID, strId);         // 添加消息号
			param.AddParam(PARAM_DIAGRAM_LAYOUT_NAME, Layout_QuoteReport);
			param.AddParam(Param_BlockChange_BlockName, strFilename);

			m_pMainWnd->DispatchLinkageMsg(QUOTE_MESSAGE_NOTIFY_DIAGRAM_LAYOUT,(WPARAM)&param,0,FALSE);
		}
		
	}
}