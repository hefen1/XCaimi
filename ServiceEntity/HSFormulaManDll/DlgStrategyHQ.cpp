// DlgStrategyParamConf.cpp : 实现文件
#include "stdafx.h"
#include "DlgStrategyHQ.h"
#include "DlgFormulaMan.h"
#include "DlgFormulaFind.h"
#include "DlgFormulaEdit.h"
#include "DlgNotesAndEidolon.h"
#include "CompileEditView.h"
#include "dlgCondition.h"
#include "DlgStrategyMain.h"
#include ".\dlgstrategyhq.h"
#include "ExpPublic.h"
#include "VarValue.h"
#include "PubStruct.h"
// CDlgStrategyHQ 对话框

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#ifndef SHORT_MAX
#define SHORT_MAX  32767
#endif

extern HWND g_hParentWnd;

IMPLEMENT_DYNAMIC(CDlgStrategyHQ, CDialog)
CDlgStrategyHQ::CDlgStrategyHQ(CWnd* pParent /*=NULL*/,HSOpenExpressUseData* pData /*= NULL*/)
: CDialog(CDlgStrategyHQ::IDD, pParent)
{
	m_pData = pData;
	m_hParent = g_hParentWnd;
}

CDlgStrategyHQ::~CDlgStrategyHQ()
{
}

void CDlgStrategyHQ::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB, m_wndTab);

	//DDX_Control(pDX, IDD_PERIOD_STATIC, m_wndPeriodStc);
	//DDX_Control(pDX, IDD_PERIOD, m_wndPeriod);

	DDX_Control(pDX, IDD_CONNOTES, m_wndConNotes);
	DDX_Control(pDX, IDD_CON, m_wndCon);
	DDX_Control(pDX, IDC_STATIC_CON, m_wndStaticCon);

	DDX_Control(pDX, IDD_COMPARE, m_wndCompare);
	DDX_Control(pDX, IDD_DATA2, m_wndData2);
	DDX_Control(pDX, IDD_AND_OR, m_wndAndOr);
	DDX_Control(pDX, IDD_DATA1, m_wndData1);
	DDX_Control(pDX, IDC_MIDDLE, m_wndMiddle);
	DDX_Control(pDX, IDC_EDIT1, m_wndCode);
	DDX_Control(pDX, IDC_COMBO1, m_wndCycle);
	DDX_Control(pDX, IDC_EDIT3, m_wndDay);
	DDX_Control(pDX, IDC_EDIT4, m_wndBuyTimes);
	DDX_Control(pDX, IDC_EDIT6, m_wndSellTimes);
	DDX_Control(pDX, IDC_EDIT5, m_wndTriggerTime);
	DDX_Control(pDX, IDC_EDIT7, m_wndTrigerNum);
	DDX_Control(pDX, IDC_EDITGAP, m_wndGap);
	DDX_Control(pDX, IDC_CHECKFUQ, m_wndCheck);
	DDX_Control(pDX, IDC_CHECK1, m_wndXD);
}


BEGIN_MESSAGE_MAP(CDlgStrategyHQ, CDialog)
	ON_MESSAGE(HX_USER_LOADCUR, LoadCurExp)
 	ON_EN_CHANGE(IDC_EDIT1, OnEnChangeEdit1)
	ON_REGISTERED_MESSAGE(HX_USER_DRAGDATA,OnUserDragData)
	ON_CBN_SELCHANGE(IDC_COMBO1, OnCbnSelchangeCombo1)
END_MESSAGE_MAP()

// CDlgStrategyHQ 消息处理程序
void CDlgStrategyHQ::OnCancel() 
{
	DestroyWindow();
}

void CDlgStrategyHQ::OnOK() 
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

	if(m_pData->m_wType & hxShowByExpressType)
	{
		CString csCode,csCycle,csBuyTimes,csSellTimes,csTriggertime,csTriggerNum,csGap;
		int buyTimes,sellTimes,trigTime,trigNum,gap, perType,perNum;
		m_wndCode.GetWindowText(csCode);//股票代码
		m_wndCycle.GetWindowText(csCycle);
		m_wndBuyTimes.GetWindowText(csBuyTimes);
		m_wndSellTimes.GetWindowText(csSellTimes);
		m_wndTriggerTime.GetWindowText(csTriggertime);
		m_wndTrigerNum.GetWindowText(csTriggerNum);
		m_wndGap.GetWindowText(csGap);
		if(csCode.IsEmpty())
		{
			::MessageBox( m_hWnd,_T(" 请填写代码! "),_T(" 提示 "), MB_OK);
			m_wndCode.SetFocus();
			return ;
		}
		if(csCycle.IsEmpty() || csCycle == "多日线")
		{
			if (csCycle.IsEmpty())
			{
				::MessageBox( m_hWnd,_T(" 请选择分析周期! "),_T(" 提示 "), MB_OK);
				m_wndCycle.SetFocus();
				return ;
			}
			CString cs;
			m_wndDay.GetWindowText(cs);
			if (cs.IsEmpty() || atoi(cs) <= 0)
			{
				::MessageBox( m_hWnd,_T(" 请填写多日天数! "),_T(" 提示 "), MB_OK);
				m_wndDay.SetFocus();
				return ;
			}
			perType = AnalisysForMoreDay;
			perNum = atoi(cs);
		}
		else
		{
			if (csCycle == "1分钟")
			{
				perType = AnalisysFor1;
				perNum = 0;
			}
			else if (csCycle == "5分钟")
			{
				perType = AnalisysFor5;
			}
			else if (csCycle == "15分钟")
			{
				perType = AnalisysFor15;
			}
			else if (csCycle == "30分钟")
			{
				perType = AnalisysFor30;
			}
			else if (csCycle == "60分钟")
			{
				perType = AnalisysFor60;
			}
			else if (csCycle == "日线")
			{
				perType = AnalisysForDay;
			}
			else if (csCycle == "周线")
			{
				perType = AnalisysForWeek;
			}
			else if (csCycle == "月线")
			{
				perType = AnalisysForMonth;
			}
			perNum = 0;
		}
		if(csBuyTimes.IsEmpty())
		{
			::MessageBox( m_hWnd,_T(" 请填写买入次数! "),_T(" 提示 "), MB_OK);
			m_wndBuyTimes.SetFocus();
			return ;
		}
		if(csSellTimes.IsEmpty())
		{
			::MessageBox( m_hWnd,_T(" 请填写卖出次数! "),_T(" 提示 "), MB_OK);
			m_wndSellTimes.SetFocus();
			return ;
		}
		if(csTriggertime.IsEmpty())
		{
			::MessageBox( m_hWnd,_T(" 请填写持续多少秒触发! "),_T(" 提示 "), MB_OK);
			m_wndTriggerTime.SetFocus();
			return ;
		}
		if(csTriggerNum.IsEmpty())
		{
			::MessageBox( m_hWnd,_T(" 请填写几次达标后触发! "),_T(" 提示 "), MB_OK);
			m_wndTrigerNum.SetFocus();
			return ;
		}
		strcpy(m_pStrategy->m_cCode,csCode);
		buyTimes = atoi(csBuyTimes);
		if(buyTimes < -1 || buyTimes > SHORT_MAX)//
		{
			::MessageBox( m_hWnd,_T(" 请正确填写买入次数! "),_T(" 提示 "), MB_OK);
			m_wndBuyTimes.SetFocus();
			return ;
		}
		sellTimes = atoi(csSellTimes);
		if( sellTimes < -1 || sellTimes > SHORT_MAX)//
		{
			::MessageBox( m_hWnd,_T(" 请正确填写卖出次数! "),_T(" 提示 "), MB_OK);
			m_wndSellTimes.SetFocus();
			return ;
		}	
		trigTime = atoi(csTriggertime);
		if (trigTime < -1 || trigTime > SHORT_MAX)//
		{
			::MessageBox( m_hWnd,_T(" 请正确填写持续多少秒触发! "),_T(" 提示 "), MB_OK);
			m_wndTriggerTime.SetFocus();
			return ;
		}	
		trigNum = atoi(csTriggerNum);
		if (trigNum < -1 || trigNum > SHORT_MAX)//
		{
			::MessageBox( m_hWnd,_T(" 请正确填写几次达标后触发! "),_T(" 提示 "), MB_OK);
			m_wndTrigerNum.SetFocus();
			return ;
		}	
		gap = atoi(csGap);
		if( gap < -1 || gap > SHORT_MAX)//
		{
			::MessageBox( m_hWnd,_T(" 请正确填写连续委托间隔! "),_T(" 提示 "), MB_OK);
			m_wndGap.SetFocus();
			return ;	
		}
		m_pStrategy->m_nPeriodType = perType;
		m_pStrategy->m_nPeriodNumber = perNum;
		m_pStrategy->m_nMaxBuyTimes = buyTimes;
		m_pStrategy->m_nMaxSellTimes = sellTimes;
		m_pStrategy->m_nTrigTimes = trigTime;
		m_pStrategy->m_nMaxTrigTimes = trigNum;
		m_pStrategy->m_nGap = gap;
		if(m_pStrategy->m_nGap == 0)//默认为30秒
		{
			m_pStrategy->m_nGap = 30;
		}
		if (m_wndCheck.GetCheck())
		{
			m_pStrategy->m_bFuQuan = 0;
		}
		else
		{
			m_pStrategy->m_bFuQuan = 1;
		}
		if (m_wndXD.GetCheck())
		{
			m_pStrategy->m_bXD = 1;
		}
		else
		{
			m_pStrategy->m_bXD = 0;
		}
		if (m_pStrategy->m_pData == NULL)
		{
			m_pStrategy->m_pData = new CLineProperty;
		}
		if (!m_pStrategy->m_pData->m_pCurVal)
		{
			m_pStrategy->m_pData->m_pCurVal = new CExpValue;
		}
		//只保存公式名和变量
		CExpression *pNew;
		if (!((CExpValue*)(m_pStrategy->m_pData->m_pCurVal))->GetExp())
		{
			pNew = new CExpression;
		}
 		else
			pNew = ((CExpValue*)(m_pStrategy->m_pData->m_pCurVal))->GetExp();
		CExpression *pTreeExp = pTreeGroup->GetData();
		if (pTreeExp)//复制参数值
		{
			CMapVariabile *pMv = pTreeExp->GetParamVar();
			//----------------------------------------------
			if (pMv)
			{
				CMapVariabile *pMap = new CMapVariabile;
				CString key;
				CValue *cvalue;
				POSITION pos;
				pos = pMv->GetStartPosition();
				while(pos)
				{
					pMv->GetNextAssoc(pos,key,cvalue);
					CNumericValue *pnum = new CNumericValue;
					double db = cvalue->GetValue();
					pnum->SetValue(db);
					pnum->SetStyle(cvalue->GetStyle());
					pnum->SetMaxValue(((CNumericValue*)cvalue)->GetMaxValue());
					pnum->SetMinValue(((CNumericValue*)cvalue)->GetMinValue());
					pnum->SetTestStepValue(((CNumericValue*)cvalue)->GetTestStepValue());
					pnum->SetDescribe(((CNumericValue*)cvalue)->GetDescribe());
					pMap->SetAt(key,pnum);
				}
				//===============================================================
				pNew->SetParamVar(pMap);//存储变量值
			}
			memcpy(m_pStrategy->m_sExpName,pTreeExp->GetName(),32);//存储公式名
		}
		((CExpValue*)(m_pStrategy->m_pData->m_pCurVal))->SetValue(pNew);
	}
	CWnd* pwnd = GetParent();
	if (pwnd && pwnd->IsKindOf(RUNTIME_CLASS(CDlgStrategyMain)))
	{
		CDlgStrategyMain *pMain = (CDlgStrategyMain*)pwnd;
		pMain->m_bHQOK = TRUE;
	}
	OnCancel();
	return ;

_Exit:
	::MessageBox( m_hWnd,_T(" 请选择一个公式! "),_T(" 提示 "), MB_OK);
	return ;
}


BOOL CDlgStrategyHQ::OnInitDialog() 
{
	CDialog::OnInitDialog();
	m_bSetVar = FALSE;
// 	m_wndPeriod.ResetContent();
// 	AddPeriodNameToWnd(&m_wndPeriod);

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

			if (m_pStrategy->m_pData && m_pStrategy->m_pData->m_pCurVal)
			{ 
				CString cs;
				char c[7] = {0};
				strncpy(c,m_pStrategy->m_cCode,6);
				m_wndCode.SetWindowText(c);//股票代码
				//--------------------------------------------
				switch(m_pStrategy->m_nPeriodType)
				{
				case AnalisysFor1:
					cs = "1分钟";
					break;
				case AnalisysFor5:
					cs = "5分钟";
					break;
				case AnalisysFor15:
					cs = "15分钟";
					break;
				case AnalisysFor30:
					cs = "30分钟";
					break;
				case AnalisysFor60:
					cs = "60分钟";
					break;
				case AnalisysForDay:
					cs = "日线";
					break;
				case AnalisysForWeek:
					cs = "周线";
					break;
				case AnalisysForMonth:
					cs = "月线";
					break;
				default:
					cs = "多日线";
					break;
				}
				if (cs == "多日线" || cs.IsEmpty())
				{
					m_wndDay.EnableWindow(TRUE);
				}
				m_wndCycle.SelectString(-1,cs);
				//--------------------------------------------------
				cs.Format("%d",m_pStrategy->m_nMaxBuyTimes);
				m_wndBuyTimes.SetWindowText(cs);
				cs.Format("%d",m_pStrategy->m_nMaxSellTimes);
				m_wndSellTimes.SetWindowText(cs);
				cs.Format("%d",m_pStrategy->m_nTrigTimes);
				m_wndTriggerTime.SetWindowText(cs);
				cs.Format("%d",m_pStrategy->m_nMaxTrigTimes);
				m_wndTrigerNum.SetWindowText(cs);
				cs.Format("%d",m_pStrategy->m_nPeriodNumber);
				m_wndDay.SetWindowText(cs);
				cs.Format("%d",m_pStrategy->m_nGap);
				m_wndGap.SetWindowText(cs);
				if (m_pStrategy->m_bFuQuan == 0)
				{
					m_wndCheck.SetCheck(TRUE);	
				}
				if (m_pStrategy->m_bXD == 1)
				{
					m_wndXD.SetCheck(TRUE);
				}
				CArray<CTreeGroup*,CTreeGroup*>* pTreeGp = pCurTree->GetTreeGroup();
				int sel = 0;
				for (; sel<pTreeGp->GetCount(); sel++)
				{
					CTreeGroup *pGp = pTreeGp->GetAt(sel);
					CString csName = pGp->GetName();
					CString cExpName = m_pStrategy->m_sExpName;
					if (pGp && csName == cExpName)
					{
						break;
					}
				}

				if (sel < pTreeGp->GetCount())
				{
					CTreeGroup *ptg = pTreeGp->GetAt(sel);					
					
					m_bSetVar = TRUE;
					pCurTree->Show(ptg->GetData(),0);
			
				}
				else
				{
					pCurTree->Show(NULL,0);
				}
			}
			else
				pCurTree->Show(NULL,0);
		}

	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

long CDlgStrategyHQ::LoadCurExp(WPARAM wParam /*= 0*/, LPARAM lParam /*= 0*/)
{	
	CDlgFormulaMan* pParentDlg  = (CDlgFormulaMan*)AfxGetMainWnd();
	if( pParentDlg == NULL )
		return 0;

	EnableItem(FALSE);

	if( wParam == CCompileEditView::UpdateExpress )
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
		CString csName = pExpression->GetName();
		CString cExpName = m_pStrategy->m_sExpName;
		if (m_bSetVar && csName == cExpName && m_pStrategy->m_pData->m_pCurVal)
		{
			CExpression *pex = ((CExpValue*)(m_pStrategy->m_pData->m_pCurVal))->GetExp();
			if (pex)//设置参数
			{
				CMapVariabile *pMap = pExpression->GetParamVar();
				CMapVariabile *pMv = pex->GetParamVar();
				//--------------------------------------------------
				if (pMv && pMap)
				{
					CString key;
					CValue *cvalue;
					CValue *pval;
					POSITION pos;
					pos = pMv->GetStartPosition();
					while(pos)
					{
						pMv->GetNextAssoc(pos,key,cvalue);
						if (pMap->Lookup(key,pval))
						{
							((CNumericValue*)pval)->SetValueOnly(cvalue->GetValue());
						}
					}
					//=====================================================================
				}
			}
		}
		CString cs = pExpression->GetName();
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

void CDlgStrategyHQ::PostNcDestroy()
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
	CDialog::PostNcDestroy();
	delete this;
}

void CDlgStrategyHQ::EnableItem(BOOL bEnable)
{
	//m_wndPeriodStc.ShowWindow(SW_HIDE);
	//m_wndPeriod.ShowWindow(SW_HIDE);

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

BOOL CDlgStrategyHQ::PreTranslateMessage(MSG* pMsg)
{
	if(pMsg->message == WM_KEYDOWN) 
	{ 
		if(pMsg->wParam == VK_RETURN ) 
		{ 
			UINT nID = GetFocus()->GetDlgCtrlID(); 
			switch( nID) 
			{ 
			case IDC_EDITGAP:
				{
					CWnd* pwnd = GetParent();
					if (pwnd && pwnd->IsKindOf(RUNTIME_CLASS(CDlgStrategyMain)))
					{
						CDlgStrategyMain *pMain = (CDlgStrategyMain*)pwnd;
						pMain->OnBnClickedOk();
					}
					break;
				}
			default: 
				NextDlgCtrl(); 
			} 
			return TRUE;
		}
		else if (pMsg->wParam == VK_TAB)
		{
			NextDlgCtrl();
			return TRUE;
		}	
	} 
	return FALSE;
}

void CDlgStrategyHQ::OnEnChangeEdit1()
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

LRESULT CDlgStrategyHQ::OnUserDragData(WPARAM wParam, LPARAM lParam)
{
	HSCurKeyInfo* pData = (HSCurKeyInfo*)lParam;
	if(pData == NULL)
		return 0;
	m_pStrategy->m_cCodeType = pData->m_cCode.m_cCodeType;
	m_wndCode.SetWindowText(pData->m_cCode.m_cCode);
	return 1;
}


void CDlgStrategyHQ::OnCbnSelchangeCombo1()
{
	// TODO: 在此添加控件通知处理程序代码
	CString cs;
	m_wndCycle.GetWindowText(cs);
	if (cs == "多日线" || cs.IsEmpty())
	{
		m_wndDay.EnableWindow(TRUE);
	}
	else
	{
		m_wndDay.EnableWindow(FALSE);
	}
}
