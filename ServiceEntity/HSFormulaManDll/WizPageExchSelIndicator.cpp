// WizPageExchSelIndicator.cpp : implementation file
//

#include "stdafx.h"

#include "WizPageExchSelIndicator.h"
#include "DlgFormulaMan.h"
#include "SheetExchMain.h"
#include "pageid.h"
#include "WizPageExchSell.h"

// CWizPageExchSelIndicator dialog
extern void AddPeriodNameToWnd(CComboBox* pBox);
extern int GetPeriodNumByName(CString strPeriodName);
extern CString GetPeriodStr(WORD dPeriod,int* nPos);
//IMPLEMENT_DYNAMIC(CWizPageExchSelIndicator, CDialog)
CWizPageExchSelIndicator::CWizPageExchSelIndicator(CWnd* pParent /*=NULL*/)
	: CWizPageExchBase(CWizPageExchSelIndicator::IDD, pParent)
{
	m_pData = &m_sExp;
}

CWizPageExchSelIndicator::~CWizPageExchSelIndicator()
{
}

void CWizPageExchSelIndicator::DoDataExchange(CDataExchange* pDX)
{

	CWizPageExchBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CWizPageExchSelIndicator)
	DDX_Control(pDX, IDC_TAB, m_wndTab);
	DDX_Control(pDX, IDC_PERIOD, m_wndPeriod);
	DDX_Control(pDX, IDC_CHECK_EXRIGHT, m_ctlExRight);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CWizPageExchSelIndicator, CWizPageExchBase)
	ON_MESSAGE(HX_USER_LOADCUR, LoadCurExp)
	ON_BN_CLICKED(IDB_BUTTON2, OnBnClickedButton2)
	ON_REGISTERED_MESSAGE(HX_USER_DRAGDATA, OnUserDragData)
END_MESSAGE_MAP()


// CWizPageExchSelIndicator message handlers

LRESULT CWizPageExchSelIndicator::OnUserDragData(WPARAM wParam, LPARAM lParam)
{
	if(wParam == HSCurKeyInfo::UpdateExpressData )
	{
		CTreeCtrlFormula* pCurTree = m_wndTab.GetTree();
		if( pCurTree == NULL )  return 0;

		CExpression* pExpress = (CExpression*)lParam;
		if( pExpress == NULL ) return 0;

		pCurTree->Show(pExpress,0);
		LoadCurExp(HX_EXPRESSSENTENCE, (LPARAM)pExpress);

		return 1;
	}
	return 0;
}

BOOL CWizPageExchSelIndicator::OnInitDialog()
{
	CWizPageExchBase::OnInitDialog();

	int nCurSel;
	AddPeriodNameToWnd(&m_wndPeriod);
	GetPeriodStr(m_sExp.m_lCyc, &nCurSel);
	if( nCurSel < 0 )
	{
		GetPeriodStr(AnalisysForDay, &nCurSel);
	}
	m_wndPeriod.SetCurSel(nCurSel);

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
// 		CWnd* pParent = this->GetParent();
// 		pParent = pParent->GetParent();
		CDlgFormulaMan* pDlg = (CDlgFormulaMan*)(this->GetParent())->GetParent();

		pCurTree->ModifyStyleEx(0,WS_EX_CLIENTEDGE);
		pCurTree->SetImageList(&CDlgFormulaMan::m_expressImages, TVSIL_NORMAL);
		pCurTree->m_pFormulaMan = pDlg;

		CTreeCtrlFormula* pTree = pDlg->GetCurTree( (WORD)Tech );
		pCurTree->CopyTree(pTree,CTreeCtrlFormula::CopyDelete);
		pTree = pDlg->GetCurTree( (WORD)Exchange );
		pCurTree->CopyTree(pTree,CTreeCtrlFormula::CopyNone);
		pTree = pDlg->GetCurTree( (WORD)Condition );
		pCurTree->CopyTree(pTree,CTreeCtrlFormula::CopyNone);

		// ��ʽ��ʾ��ʼ���������¼��� 2003-08-20
		if( m_sExp.m_pMainTech != NULL )
		{
			pCurTree->Show(m_sExp.m_pMainTech,0);
			LoadCurExp(HX_EXPRESSSENTENCE, (LPARAM)m_sExp.m_pMainTech);
		}
		// �����¼������
	}

	// ��Ȩ���㣬�����¼��� 2003-08-20
	m_ctlExRight.SetCheck((int)m_sExp.m_bExRights);
	// �����¼������

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

long CWizPageExchSelIndicator::LoadCurExp(WPARAM wParam /*= 0*/, LPARAM lParam /*= 0*/)
{
	BOOL bNewExpression = FALSE;
	if( wParam == HX_EXPRESSSENTENCE )
	{
		//EnableItem(FALSE);

		CExpression* pExpression = (CExpression*)lParam;
		if( pExpression == NULL )
			return 0;

		// �����¼��� 2003-8-20 Ϊ�˱����������
		if( m_sExp.m_pMainTech != pExpression )
		{
			bNewExpression = TRUE;
			m_sExp.m_pMainTech = pExpression;
		}

		// �������뷽ʽ������
		CWnd* pParentWnd = GetParent();
		if( pParentWnd == NULL )
		{
			return 0;
		}
		::SendMessage(pParentWnd->GetSafeHwnd(), HX_USER_COMPILEDATA, EXCH_MSG_WP_CALC_SET_BUY, lParam);
		// �����¼��� 2003-8-20 �������

		// ע��/�����޸�
		CDlgFormulaMan* pParentDlg  = (CDlgFormulaMan*)pParentWnd->GetParent();
		if( pParentDlg == NULL )
			return 0;

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
				m_EditEidolon.SetText(strText,pExpression->GetParamVar());
			}
			else
			{
				CStringArray* pArray = pExpression->GetDefEidolon();
				if(pArray->GetSize() <= 1)
				{
					pArray = NULL;
				}
				m_EditEidolon.SetText(pArray,pExpression->GetParamVar());
			}
			m_EditEidolon.Invalidate();
		}
		CSheetExchMain* pParent = (CSheetExchMain*)GetParent();
		if( pParent != NULL && bNewExpression )
		{
			CExchCalcMain* pDataCacl = pParent->GetData();
			if( pDataCacl != NULL )
			{
				TExchBuy* pBuy = pDataCacl->GetBuy();
				if( pBuy != NULL )
				{
					pBuy->m_strTechLeft.Empty();
					pBuy->m_SubData.m_cType = FormulaCompareSubData::DaYu;
					pBuy->m_SubData.m_dData1 = _T("5");
					pBuy->m_SubData.m_dData2.Empty();
				}
			}
		}
	}
	else
	{
		m_EditEidolon.ShowWindow(SW_HIDE);
		m_sExp.m_pMainTech = NULL;
	}

	if( bNewExpression )
	{
		SaveOptionEx();
	}

	return 0;
}

LRESULT CWizPageExchSelIndicator::OnWizardNext()
{
	//if( m_sExp.m_pMainTech == NULL )
	//{
	//	MessageBox("��ѡ��ָ��!", FORMULAMAN_EXCH_PROMPT);
	//	return -1;
	//}
	//m_sExp.m_bExRights = m_ctlExRight.GetCheck();
	/*return CWizPageExchBase::OnWizardNext();*/
	return 0;
}

BOOL CWizPageExchSelIndicator::OnKillActive(int nPos /*= -1*/)
{
	if(nPos <= 0) return TRUE;

	if( m_sExp.m_pMainTech == NULL )
	{
		MessageBox(_T("��ѡ��ָ��!"), FORMULAMAN_EXCH_PROMPT, MB_OK);
		return FALSE;
	}
	m_sExp.m_bExRights = m_ctlExRight.GetCheck();
	CString strCyc;
	m_wndPeriod.GetLBText(m_wndPeriod.GetCurSel(), strCyc);
	m_sExp.m_lCyc = GetPeriodNumByName(strCyc);

	return SaveOption();
}

void CWizPageExchSelIndicator::OnSetActive()
{
	CSheetExchMain* pParent = (CSheetExchMain*)GetParent();
	if( pParent != NULL )
	{
		CString str(_T("ϵͳ����-"));
		CExchCalcMain* pData = pParent->GetData();
		if( pData != NULL )
		{
			CExpression *pExp = pData->GetExp();
			if( pExp != NULL )
			{
				str += pExp->GetName();
				str += "-";
			}
		}
		str += _T("ѡ�񷽷�");
		pParent->SetWindowText(str);
	}
}

void CWizPageExchSelIndicator::OnBnClickedButton2()
{
	DWORD dwMask = Tech | Condition | Exchange;
	CWnd* pParent = GetParent();
	if( pParent == NULL )
		return;

	CDlgFormulaMan* pParentDlg  = (CDlgFormulaMan*)pParent->GetParent();
	if( pParentDlg )
	{
		pParentDlg->Find(dwMask,this);
	}
}

/******************************************************************
*	�������ܣ�	��ʼ����������
*	����������	TExchHead* pData : [in] ��ֵ����
*	�� �� ֵ��	BOOL : �ɹ���TRUE����ʧ�ܣ�FALSE��
*	����������	���ظ��ຯ��
*	��    �ߣ�
*	������ڣ�
*	�޸�˵����	
*****************************************************************/
BOOL CWizPageExchSelIndicator::InitialData(TExchHead* pData)
{
	m_sExp = *((TExchExp*)pData);

	return TRUE;
}