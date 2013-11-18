// DlgFormulaBrowser.cpp : implementation file
//

#include "stdafx.h"
#include "dlgstrategytest.h"

#include "DlgFormulaMan.h"
#include "DlgFormulaFind.h"
#include "DlgNotesAndEidolon.h"
#include "CompileEditView.h"

#include "dlgCondition.h"

#include "dlgCondition.h"
#include ".\dlgstrategytest.h"
#include "PubStruct.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//extern void     AddPeriodNameToWnd(CComboBox* pBox);
//extern int		GetPeriodNumByName(CString strPeriodName);

/////////////////////////////////////////////////////////////////////////////
// CDlgStrategyTest dialog

extern HWND g_hParentWnd;

CDlgStrategyTest::CDlgStrategyTest(CWnd* pParent /*=NULL*/,HSOpenExpressUseData* pData /*= NULL*/)
: CDialog(CDlgStrategyTest::IDD, pParent)
{
	m_pData = pData;
	m_hParent = g_hParentWnd;
	m_pStrategyHis = NULL;
}


void CDlgStrategyTest::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgStrategyTest)
	DDX_Control(pDX, IDC_TAB1, m_wndTab);
	//}}AFX_DATA_MAP

	DDX_Control(pDX, IDD_PERIOD_STATIC1, m_wndPeriodStc);
	DDX_Control(pDX, IDD_PERIOD1, m_wndPeriod);

	DDX_Control(pDX, IDD_CONNOTES1, m_wndConNotes);
	DDX_Control(pDX, IDD_CON1, m_wndCon);
	DDX_Control(pDX, IDC_STATIC_CON1, m_wndStaticCon);

	DDX_Control(pDX, IDD_COMPARE1, m_wndCompare);
	DDX_Control(pDX, IDD_DATA21, m_wndData2);
	DDX_Control(pDX, IDD_AND_OR1, m_wndAndOr);
	DDX_Control(pDX, IDD_DATA11, m_wndData1);
	DDX_Control(pDX, IDC_MIDDLE1, m_wndMiddle);

	DDX_Control(pDX, IDC_EDIT1, m_wndCode);
	DDX_Control(pDX, IDC_EDIT2, m_wndMoney);
	DDX_Control(pDX, IDC_COMBOPER, m_wndPeri);
	DDX_Control(pDX, IDC_EDIT3, m_wndDay);
	DDX_Control(pDX, IDC_DATETIMEPICKER3, m_wndDayStart);
	DDX_Control(pDX, IDC_DATETIMEPICKER2, m_wndTimeStart);
	DDX_Control(pDX, IDC_DATETIMEPICKER4, m_wndDayEnd);
	DDX_Control(pDX, IDC_DATETIMEPICKER5, m_wndTimeEnd);
	DDX_Control(pDX, IDC_STATIC_UNIT, m_wndStaticUnit);
	DDX_Control(pDX, IDC_STRATEGY_BUYTYPE, m_wndBuyType);
	DDX_Control(pDX, IDC_STRATEGY_BUYCOUNT, m_wndBuyCount);
	DDX_Control(pDX, IDC_STRATEGY_GAIN, m_wndGainNum);
	DDX_Control(pDX, IDC_STRATEGY_DEFICIT, m_wndDeficitNum);
	DDX_Control(pDX, IDC_STRATEGY_FEES, m_wndFees);
}


BEGIN_MESSAGE_MAP(CDlgStrategyTest, CDialog)
	//{{AFX_MSG_MAP(CDlgStrategyTest)
	ON_BN_CLICKED(IDC_BUTFIND1, OnButfind)
	ON_BN_CLICKED(IDC_NOTES1, OnNotes)
	ON_BN_CLICKED(IDOK21, OnOK)
	ON_BN_CLICKED(IDCANCEL31, OnCancel)
	ON_CBN_SELCHANGE(IDD_CON1, OnSelchangeCon)
	ON_CBN_SELCHANGE(IDD_COMPARE1, OnSelchangeCompare)
	ON_REGISTERED_MESSAGE(HX_USER_DRAGDATA,OnUserDragData)
	//}}AFX_MSG_MAP

	ON_MESSAGE(HX_USER_LOADCUR, LoadCurExp)

	ON_EN_CHANGE(IDC_EDIT1, OnEnChangeEdit1)
	ON_CBN_SELCHANGE(IDC_COMBOPER, OnCbnSelchangeComboper)
	ON_BN_CLICKED(IDC_CK_MAXGAIN, OnBnClickedCkMaxgain)
	ON_BN_CLICKED(IDC_CK_MAXDEFICIT, OnBnClickedCkMaxdeficit)
	ON_CBN_SELCHANGE(IDC_STRATEGY_BUYTYPE, OnCbnSelchangeStrategyBuytype)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgStrategyTest message handlers

void CDlgStrategyTest::OnCancel() 
{
	delete m_pStrategyHis->m_pCode;
	m_pStrategyHis->m_pCode = NULL;
	CDialog::OnCancel();
}

void CDlgStrategyTest::OnOK() 
{
	if( m_pData == NULL )
		goto _Exit;
	CDlgFormulaMan* pParentDlg  = (CDlgFormulaMan*)AfxGetMainWnd();
	if( pParentDlg == NULL )
		goto _Exit;

	CTreeCtrlFormula* pCurTree = m_wndTab.GetTree();
	if( !pCurTree )
		goto _Exit;
	HTREEITEM hItem = pCurTree->GetSelectedItem();
	if( hItem == NULL )
		goto _Exit;

	CTreeGroup* pTreeGroup = (CTreeGroup*)pCurTree->GetItemData(hItem);
	if( (pTreeGroup == NULL)			|| 
		(pTreeGroup->GetData() == NULL) ||
		!pTreeGroup->IsStatus(hx_GroupItem) )
		goto _Exit;

	if( m_pData->m_wType & hxShowReturnByExpressType )
	{
		if( (m_pData->m_wType & 0xF) <= MoreKLine )
		{
			m_pData->m_pData = pTreeGroup->GetData();
		}
	}
	else if(m_pData->m_wType & hxShowByExpressType)
	{
		if (!GetParam())
		{
			return;
		}
		CString csName = pTreeGroup->GetName();
		strcpy(m_pStrategyHis->m_sExpName,csName);//保存公式名
	}
	CDialog::OnOK();
	return;


_Exit:
	::MessageBox( m_hWnd,_T(" 请选择一个公式! "),_T(" 提示 "), MB_OK);
}

void CDlgStrategyTest::OnButfind() 
{
	// TODO: Add your control notification handler code here
	//by HS begin
	/*CDlgFormulaMan* pParentDlg  = (CDlgFormulaMan*)AfxGetMainWnd();
	if( pParentDlg == NULL )
	return;
	pParentDlg->Find(this);
	*/
	DWORD dwMask = 0xFFFF;
	if( m_pData != NULL )
	{
		dwMask = (m_pData->m_wType & 0xf);
	}
	CDlgFormulaMan* pParentDlg  = (CDlgFormulaMan*)AfxGetMainWnd();
	if(pParentDlg)
	{
		pParentDlg->Find(dwMask,this);
	}

	//by HS end 
}

void CDlgStrategyTest::OnNotes() 
{
	CDlgFormulaMan* pParentDlg  = (CDlgFormulaMan*)AfxGetMainWnd();
	if( pParentDlg == NULL )
	{
		goto OnNotes_End;
	}

	CTreeCtrlFormula* pCurTree = m_wndTab.GetTree();
	if( !pCurTree )
	{
		goto OnNotes_End;
	}
	HTREEITEM hItem = pCurTree->GetSelectedItem();
	if( hItem == NULL )
	{
		goto OnNotes_End;
	}

	CTreeGroup* pTreeGroup = (CTreeGroup*)pCurTree->GetItemData(hItem);
	if( (pTreeGroup == NULL)			|| 
		(pTreeGroup->GetData() == NULL) ||
		!pTreeGroup->IsStatus(hx_GroupItem) )
	{
		goto OnNotes_End;
	}

	CExpression* pExpression = (CExpression*)pTreeGroup->GetData();
	if( pExpression == NULL )
	{		
		goto OnNotes_End;
	}

	CDlgNotesAndEidolon* pDlgNotes = pParentDlg->GetDlgNotes();
	if( pDlgNotes != NULL )
	{
		pDlgNotes->m_pExpression = pExpression;
		pDlgNotes->SetNotes(pExpression->GetNotes(), pExpression->GetName());
		pDlgNotes->ShowWindow(SW_SHOW);
	}
	return;

OnNotes_End:
	::MessageBox( m_hWnd,_T(" 请选择一个公式! "),_T(" 提示 "), MB_OK);
}

BOOL CDlgStrategyTest::OnInitDialog() 
{
	CDialog::OnInitDialog();
	m_bCKMaxGain = FALSE;
	m_bCKMaxDeficit = FALSE;
	m_wndPeriod.ResetContent();
	AddPeriodNameToWnd(&m_wndPeriod);
	//end 

	m_EditEidolon.CreateEx(WS_EX_CLIENTEDGE,NULL,NULL,WS_CHILD|WS_VISIBLE|WS_TABSTOP|WS_BORDER,
		CRect(0,0,0,0),this,1001);
	m_EditEidolon.SetFont( GetFont() );

	CRect rect;
	CWnd* pWnd = GetDlgItem(IDC_PARAM);
	pWnd->GetWindowRect(&rect);
	ScreenToClient(&rect);
	m_EditEidolon.MoveWindow(rect);

	CTreeCtrlFormula* pCurTree = m_wndTab.GetTree();
	if( pCurTree )
	{
		CDlgFormulaMan* pDlg = (CDlgFormulaMan*)AfxGetMainWnd();

		pCurTree->ModifyStyleEx(0,WS_EX_CLIENTEDGE);
		pCurTree->SetImageList(&CDlgFormulaMan::m_expressImages, TVSIL_NORMAL);
		pCurTree->m_pFormulaMan = pDlg;

		if( m_pData != NULL )
		{
			if( m_pData->m_strName != NULL )
			{
				this->SetWindowText(*m_pData->m_strName);
			}
			WORD nType = ((m_pData->m_wType & 0xFF) & Strategy);
			int nCopy = CTreeCtrlFormula::CopyNone;
			CTreeCtrlFormula* pTree;
			for( int i = 0; i < 5; i++ ) //20090302 YJT 修改
			{
				switch(i)
				{
				case 0:
					nType = ((m_pData->m_wType & 0xF) & Tech);
					break;
				case 1:
					nType = ((m_pData->m_wType & 0xF) & Condition);
					break;
				case 2:
					nType = ((m_pData->m_wType & 0xF) & Exchange);
					break;
				case 3:
					nType = ((m_pData->m_wType & 0xF) & MoreKLine);
					break;
				case 4:
					nType = ((m_pData->m_wType & 0xFF) & Strategy);
					break;
				}

				pTree = pDlg->GetCurTree( nType );
				pCurTree->CopyTree(pTree,nCopy,m_pData->m_dStyle);
			}

			if( m_pData->m_pData != NULL )
			{
				pCurTree->Show((CExpression*)m_pData->m_pData,0);
			}
			else
			{
				pCurTree->Show(NULL,0); // 显示第一个
			}
		}

	}

	//add by lxqi 20090401
	m_wndBuyType.SetCurSel(0);
	LPSYSTEMTIME time=new SYSTEMTIME;
	ZeroMemory(time,sizeof(SYSTEMTIME));
	time->wYear = 1990;
	time->wMonth = 1;
	time->wDay = 1;
	time->wHour = 9;
	time->wMinute = 30;
	m_wndTimeStart.SetTime(time);
	time->wHour = 15;
	time->wMinute = 0;
	m_wndTimeEnd.SetTime(time);
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

long CDlgStrategyTest::LoadCurExp(WPARAM wParam /*= 0*/, LPARAM lParam /*= 0*/)
{	
	CDlgFormulaMan* pParentDlg  = (CDlgFormulaMan*)AfxGetMainWnd();
	if( pParentDlg == NULL )
		return 0;

	EnableItem(FALSE);

	if( wParam == CTreeCtrlFormula::HSBrowser )
	{

	}
	else if( wParam == CCompileEditView::UpdateExpress )
	{
		CDlgFormulaEdit* pDlg = pParentDlg->GetExpDlg();
		if( pDlg != NULL )
		{
			if(pDlg->SendMessage(HX_USER_LOADCUR,wParam,lParam))
			{
				pParentDlg->Enable(FALSE);
				pDlg->ShowWindow(SW_SHOWNORMAL);
			}
		}
	}
	else if( wParam == HX_EXPRESSSENTENCE )
	{
		//EnableItem(FALSE);
		CExpression* pExpression = (CExpression*)lParam;
		if( pExpression == NULL )
			return 0;

		CWnd* pWnd = GetDlgItem(IDC_NOTES1);
		pWnd->ShowWindow(SW_SHOW);
		m_EditEidolon.ShowWindow(SW_SHOW);

		CDlgNotesAndEidolon* pDlgNotes = pParentDlg->GetDlgNotes();
		CDlgNotesAndEidolon* pDlgParam = pParentDlg->GetDlgParam();
		if( pDlgNotes != NULL && 
			pDlgNotes->IsWindowVisible() )
		{
			pDlgNotes->SetNotes(pExpression->GetNotes(), pExpression->GetName());
		}

		if( m_EditEidolon.m_hWnd != NULL )
		{		
			CString strText;
			strText.Format("%s",pExpression->GetEidolon());
			if( !strText.IsEmpty() )
			{
				m_EditEidolon.SetText(strText,pExpression->GetParamVar(),FALSE);
			}
			else
			{
				CStringArray* pArray = pExpression->GetDefEidolon();
				if(pArray->GetSize() <= 1)
				{
					pArray = NULL;
				}
				m_EditEidolon.SetText(pArray,pExpression->GetParamVar(),FALSE);
			}
			m_EditEidolon.Invalidate();
		}
	}
	return 0;
}

void CDlgStrategyTest::EnableItem(BOOL bEnable)
{
	CWnd* pWnd = GetDlgItem(IDC_NOTES1);
	pWnd->ShowWindow(SW_HIDE);

	m_wndPeriodStc.ShowWindow(SW_HIDE);
	m_wndPeriod.ShowWindow(SW_HIDE);

	m_wndConNotes.ShowWindow(SW_HIDE);
	m_wndCon.ShowWindow(SW_HIDE);
	m_wndStaticCon.ShowWindow(SW_HIDE);

	m_wndCompare.ShowWindow(SW_HIDE);
	m_wndData1.ShowWindow(SW_HIDE);
	m_wndAndOr.ShowWindow(SW_HIDE);
	m_wndData2.ShowWindow(SW_HIDE);
	m_wndMiddle.ShowWindow(SW_HIDE);

	m_EditEidolon.ShowWindow(SW_HIDE);
}


void CDlgStrategyTest::OnSelchangeCon() 
{
	// TODO: Add your control notification handler code here
	OnSelchangeCompare();
}

void CDlgStrategyTest::OnSelchangeCompare() 
{
	// TODO: Add your control notification handler code here
	int nIndex = m_wndCompare.GetCurSel();
	if( nIndex == -1 ) 
		goto __OnSelchangeCompare_;

	switch( nIndex )
	{
	case FormulaCompareSubData::DaYu:		//  大于
		{
			m_wndAndOr.ShowWindow(SW_HIDE);
			m_wndData2.ShowWindow(SW_HIDE);
			m_wndMiddle.ShowWindow(SW_HIDE);
		}
		break;
	case FormulaCompareSubData::XiaoYu:		//	小于
		{
			m_wndAndOr.ShowWindow(SW_HIDE);
			m_wndData2.ShowWindow(SW_HIDE);
			m_wndMiddle.ShowWindow(SW_HIDE);
		}
		break;
	case FormulaCompareSubData::DengYu:		//	等于
		{
			m_wndAndOr.ShowWindow(SW_HIDE);
			m_wndData2.ShowWindow(SW_HIDE);
			m_wndMiddle.ShowWindow(SW_HIDE);
		}
		break;
	case FormulaCompareSubData::JieYu:		//	介于
		{
			m_wndAndOr.ShowWindow(SW_SHOW);
			m_wndAndOr.SetWindowText(_T("与"));
			m_wndData2.ShowWindow(SW_SHOW);
			m_wndMiddle.ShowWindow(SW_SHOW);
		}
		break;
	case FormulaCompareSubData::ShangChuan: //	上穿
		{
			m_wndAndOr.ShowWindow(SW_HIDE);
			m_wndData2.ShowWindow(SW_HIDE);
			m_wndMiddle.ShowWindow(SW_HIDE);
		}
		break;
	case FormulaCompareSubData::XiaoPo:		//	下破
		{
			m_wndAndOr.ShowWindow(SW_HIDE);
			m_wndData2.ShowWindow(SW_HIDE);
			m_wndMiddle.ShowWindow(SW_HIDE);
		}
		break;
	case FormulaCompareSubData::WeiChi:		//	维持
		{
			m_wndAndOr.ShowWindow(SW_SHOW);
			m_wndAndOr.SetWindowText(_T("周期后上穿"));
			m_wndData2.ShowWindow(SW_SHOW);
			m_wndMiddle.ShowWindow(SW_HIDE);
		}
		break;
	case FormulaCompareSubData::BiaoChi:		//	保持
		{
			m_wndAndOr.ShowWindow(SW_SHOW);
			m_wndAndOr.SetWindowText(_T("周期后下破"));
			m_wndData2.ShowWindow(SW_SHOW);
			m_wndMiddle.ShowWindow(SW_HIDE);
		}
		break;
	}

__OnSelchangeCompare_:
	m_wndData1.ResetContent();
	m_wndData2.ResetContent();
}

void CDlgStrategyTest::PostNcDestroy()
{
	CTreeCtrlFormula* pCurTree = this->m_wndTab.GetTree();
	if( pCurTree )
	{
		HTREEITEM hRootItem = pCurTree->GetRootTree(CTreeCtrlFormula::g_strJiben);
		if(hRootItem)
		{
			CTreeGroup* pTreeGroup = (CTreeGroup*)(pCurTree->GetItemData(hRootItem));
			delete pTreeGroup;
		}

	}

}

LRESULT CDlgStrategyTest::OnUserDragData(WPARAM wParam, LPARAM lParam)
{
	if(wParam == HSCurKeyInfo::UpdateExpressData/*HX_USER_DRAGDATA-1*/)
	{
		CTreeCtrlFormula* pCurTree = m_wndTab.GetTree();
		if( pCurTree == NULL )  return 0;

		CExpression* pExpress = (CExpression*)lParam;
		if( pExpress == NULL ) return 0;

		pCurTree->Show(pExpress,0);
		LoadCurExp(HX_EXPRESSSENTENCE, (LPARAM)pExpress);

		return 1;
	}
	else
	{
		HSCurKeyInfo* pData = (HSCurKeyInfo*)lParam;
		if(pData == NULL)
			return 0;
		if (!m_pStrategyHis)
		{
			m_pStrategyHis = new StrategyHis;
		}
		if (!m_pStrategyHis->m_pCode)
		{
			m_pStrategyHis->m_pCode = new CodeInfo;
		}
		m_pStrategyHis->m_pCode->m_cCodeType = pData->m_cCode.m_cCodeType;
		m_wndCode.SetWindowText(pData->m_cCode.m_cCode);
		return 1;
	}
}
void CDlgStrategyTest::OnEnChangeEdit1()
{
	CString strTemp;
	m_wndCode.GetWindowText(strTemp);
	if (strTemp.GetLength() >= 1 && strTemp.GetLength() != 6)
	{
		strTemp.MakeUpper();
		m_wndCode.SetWindowText(NULL);
		AlarmMsgData *pAl = new AlarmMsgData;
		pAl->pMsg = new MSG;
		memset(pAl->pMsg,0,sizeof(MSG));
		pAl->pMsg->hwnd = this->m_hParent;
		pAl->pWnd = this->GetDlgItem(IDC_EDIT1);
		pAl->pMsg->wParam = (WPARAM)strTemp.GetAt(0);
		pAl->pMsg->message = WM_KEYDOWN;
		::SendMessage(g_hParentWnd,HX_USER_COMPILEDATA,(WPARAM)23,(LPARAM)pAl);
	}
}

void CDlgStrategyTest::OnCbnSelchangeComboper()
{
	CString cs;
	m_wndPeri.GetWindowText(cs);
	if (cs == "多日线" || cs.IsEmpty())
	{
		m_wndDay.EnableWindow(TRUE);
	}
	else
	{
		m_wndDay.EnableWindow(FALSE);
	}
}

BOOL CDlgStrategyTest::GetParam()
{
	CString cs;
	m_wndCode.GetWindowText(cs);
	if (cs.IsEmpty())
	{
		MessageBox("请填写代码！","提示");
		m_wndCode.SetFocus();
		return FALSE;
	}
	strncpy(m_pStrategyHis->m_pCode->m_cCode,cs,6);
	m_wndMoney.GetWindowText(cs);
	if (cs.IsEmpty())
	{
		MessageBox("请填写资金！","提示");
		m_wndMoney.SetFocus();
		return FALSE;
	}
	m_pStrategyHis->m_nMoney = atof(cs);
	if (m_pStrategyHis->m_nMoney < 0.000000000001)
	{
		MessageBox("请正确填写资金！","提示");
		m_wndMoney.SetFocus();
		return FALSE;
	}
	
	int sel, mltp;
	sel = GetCheckedRadioButton(IDC_RADIO11,IDC_RADIO31);
	switch(sel)
	{
	case IDC_RADIO11:
		mltp = 10000;
		break;
	case IDC_RADIO21:
		mltp = 100000000;
		break;
	default:
		mltp = 1;
		break;
	}
	m_pStrategyHis->m_nMoney *= mltp;
	m_wndPeri.GetWindowText(cs);
	if(cs.IsEmpty() || cs == "多日线")
	{
		if (cs.IsEmpty())
		{
			::MessageBox( m_hWnd,_T(" 请选择分析周期! "),_T(" 提示 "), MB_OK);
			m_wndPeri.SetFocus();
			return FALSE;
		}
		CString csDay;
		m_wndDay.GetWindowText(csDay);
		if (csDay.IsEmpty() || atoi(csDay) <= 0)
		{
			::MessageBox( m_hWnd,_T(" 请填写多日天数! "),_T(" 提示 "), MB_OK);
			m_wndDay.SetFocus();
			return FALSE;
		}
		m_pStrategyHis->m_nPeriodType = AnalisysForMoreDay;
		m_pStrategyHis->m_nPeriodNumber = atoi(csDay);
	}
	else
	{
		if (cs == "1分钟")
		{
			m_pStrategyHis->m_nPeriodType = AnalisysFor1;
			m_pStrategyHis->m_nPeriodNumber = 0;
		}
		else if (cs == "5分钟")
		{
			m_pStrategyHis->m_nPeriodType = AnalisysFor5;
			m_pStrategyHis->m_nPeriodNumber = 0;
		}
		else if (cs == "15分钟")
		{
			m_pStrategyHis->m_nPeriodType = AnalisysFor15;
			m_pStrategyHis->m_nPeriodNumber = 0;
		}
		else if (cs == "30分钟")
		{
			m_pStrategyHis->m_nPeriodType = AnalisysFor30;
			m_pStrategyHis->m_nPeriodNumber = 0;
		}
		else if (cs == "60分钟")
		{
			m_pStrategyHis->m_nPeriodType = AnalisysFor60;
			m_pStrategyHis->m_nPeriodNumber = 0;
		}
		else if (cs == "日线")
		{
			m_pStrategyHis->m_nPeriodType = AnalisysForDay;
			m_pStrategyHis->m_nPeriodNumber = 0;
		}
		else if (cs == "周线")
		{
			m_pStrategyHis->m_nPeriodType = AnalisysForWeek;
			m_pStrategyHis->m_nPeriodNumber = 1;
		}
		else if (cs == "月线")
		{
			m_pStrategyHis->m_nPeriodType = AnalisysForMonth;
			m_pStrategyHis->m_nPeriodNumber = 0;
		}

	}
	CTime time;
	int year, month, day, hour, minute, sec;
	m_wndDayStart.GetTime(time);
	year = time.GetYear();
	month = time.GetMonth();
	day = time.GetDay();
	m_wndTimeStart.GetTime(time);
	hour = time.GetHour();
	minute = time.GetMinute();
	sec = time.GetSecond();
	if (m_pStrategyHis->m_nPeriodType >= AnalisysForDay)//日线以上
	{
		m_pStrategyHis->m_nStartDate = day + month * 100 + year * 10000;
	}
	else
		m_pStrategyHis->m_nStartDate = minute + hour * 100 + day * 10000 + month * 1000000 + (year - 1990)%100 * 100000000;
	m_wndDayEnd.GetTime(time);
	year = time.GetYear();
	month = time.GetMonth();
	day = time.GetDay();
	m_wndTimeEnd.GetTime(time);
	hour = time.GetHour();
	minute = time.GetMinute();
	sec = time.GetSecond();
	if (m_pStrategyHis->m_nPeriodType >= AnalisysForDay)//日线以上
	{
		m_pStrategyHis->m_nEndDate = day + month * 100 + year * 10000;
	}
	else
		m_pStrategyHis->m_nEndDate = minute + hour * 100 + day * 10000 + month * 1000000 + (year - 1990)%100 * 100000000;
	if (m_pStrategyHis->m_nEndDate < m_pStrategyHis->m_nStartDate)
	{
		MessageBox("开始时间不能比结束时间小！","提示");
		return FALSE;
	}
	//add by lxqi 20090401
	CString strTemp;
	//获取买入类型
	m_pStrategyHis->m_nBuyType = m_wndBuyType.GetCurSel();

	//获取买入数量
	if (m_pStrategyHis->m_nBuyType != 0)
	{
		m_wndBuyCount.GetWindowText(strTemp);
		m_pStrategyHis->m_nBuyNum = atof(strTemp);
		if (m_pStrategyHis->m_nBuyType == 1)
		{
			if (m_pStrategyHis->m_nBuyNum > 100 || m_pStrategyHis->m_nBuyNum < 0.000000000001)
			{
				MessageBox("请正确填写每次买入资金！","提示");
				m_wndBuyCount.SetFocus();
				return FALSE;
			}
		}
		else if (m_pStrategyHis->m_nBuyType == 2)
		{
			if (m_pStrategyHis->m_nBuyNum < 0.000000000001)
			{
				MessageBox("请正确填写每次买入资金！","提示");
				m_wndBuyCount.SetFocus();
				return FALSE;
			}
			else if (m_pStrategyHis->m_nBuyNum > m_pStrategyHis->m_nMoney)
			{
				MessageBox("每次买入资金不能比总资金大！","提示");
				m_wndBuyCount.SetFocus();
				return FALSE;
			}
		}
		else if (m_pStrategyHis->m_nBuyType == 3)
		{
			long num = m_pStrategyHis->m_nBuyNum;
			if (num <= 0 || m_pStrategyHis->m_nBuyNum - num > 0.000000000001)
			{
				MessageBox("请正确填写每次买入股数！","提示");
				m_wndBuyCount.SetFocus();
				return FALSE;
			}
		}
		//m_pStrategyHis->m_nBuyNum = atol(strTemp.GetBuffer());
	}
	
	//获取盈利上限
	if (m_bCKMaxGain)
	{
		m_wndGainNum.GetWindowText(strTemp);
		m_pStrategyHis->m_nMaxGain = atof(strTemp);
		if (m_pStrategyHis->m_nMaxGain < 0)
		{
			MessageBox("请正确填写盈利上限！","提示");
			m_wndFees.SetFocus();
			return FALSE;
		}
		//m_pStrategyHis->m_nMaxGain = atoi(strTemp.GetBuffer());
	}
	else
	{
		m_pStrategyHis->m_nMaxGain = -1;
	}

	//获得亏损上限
	if (m_bCKMaxDeficit)
	{
		m_wndDeficitNum.GetWindowText(strTemp);
		m_pStrategyHis->m_nMaxDeficit = atof(strTemp);
		if (m_pStrategyHis->m_nMaxDeficit < 0)
		{
			MessageBox("请正确填写亏损上限！","提示");
			m_wndFees.SetFocus();
			return FALSE;
		}
		//m_pStrategyHis->m_nMaxDeficit = atoi(strTemp.GetBuffer());
	}
	else
	{
		m_pStrategyHis->m_nMaxDeficit = -1;
	}

	//获得手续费率
	m_wndFees.GetWindowText(strTemp);
	m_pStrategyHis->m_nFees = atof(strTemp);
	if (m_pStrategyHis->m_nFees < 0)
	{
		MessageBox("请正确填写手续费率！","提示");
		m_wndFees.SetFocus();
		return FALSE;
	}
	return TRUE;
}
void CDlgStrategyTest::OnBnClickedCkMaxgain()
{
	if (!m_bCKMaxGain)
	{
		m_bCKMaxGain = TRUE;
		m_wndGainNum.EnableWindow(TRUE);
	}
	else
	{
		m_bCKMaxGain = FALSE;
		m_wndGainNum.EnableWindow(FALSE);
	}
}

void CDlgStrategyTest::OnBnClickedCkMaxdeficit()
{
	if (!m_bCKMaxDeficit)
	{
		m_bCKMaxDeficit = TRUE;
		m_wndDeficitNum.EnableWindow(TRUE);
	} 
	else
	{
		m_bCKMaxDeficit = FALSE;
		m_wndDeficitNum.EnableWindow(FALSE);
	}
}

void CDlgStrategyTest::OnCbnSelchangeStrategyBuytype()
{
	int nBuyType = m_wndBuyType.GetCurSel();
	if (nBuyType == 0)			//全部资金买入
	{
		m_wndBuyCount.EnableWindow(FALSE);
		m_wndStaticUnit.SetWindowText("");
	} 
	else if (nBuyType == 1)		//部分资金买入(按比例)
	{
		m_wndBuyCount.EnableWindow(TRUE);
		m_wndStaticUnit.SetWindowText("%");
	} 
	else if (nBuyType == 2)		//固定资金买入
	{
		m_wndBuyCount.EnableWindow(TRUE);
		m_wndStaticUnit.SetWindowText("元");
	} 
	else if (nBuyType == 3)		//固定股数买入
	{
		m_wndBuyCount.EnableWindow(TRUE);
		m_wndStaticUnit.SetWindowText("手");
	}	
}

