// WizPageExchSell.cpp : implementation file
//

#include "stdafx.h"

#include "WizPageExchSell.h"
#include "SheetExchMain.h"


// CWizPageExchSell dialog

IMPLEMENT_DYNAMIC(CWizPageExchSell, CWizPageExchBase)

CWizPageExchSell::CWizPageExchSell(CWnd* pParent /*=NULL*/)
	: CWizPageExchBase(CWizPageExchSell::IDD, pParent)
{
	m_pData = &m_sExchSell;
}

CWizPageExchSell::~CWizPageExchSell()
{
}

void CWizPageExchSell::DoDataExchange(CDataExchange* pDX)
{
	CWizPageExchBase::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUYCOUNT, m_wndEditSellPeriod);
	DDX_Control(pDX, IDC_LRCOUNT, m_wndEditWin);
	DDX_Control(pDX, IDC_LOSECOUNT, m_wndEditLose);
	DDX_Control(pDX, IDC_DOWNCOUNT, m_wndEditFallDay);
	DDX_Control(pDX, IDC_DOWNBL, m_wndEditFallRation);
	DDX_Control(pDX, IDC_HPCOUNT, m_wndEditUpDay);
	DDX_Control(pDX, IDC_HPBL, m_wndEditUpRation);
	DDX_Control(pDX, IDC_BUY, m_wndChkBuy);
	DDX_Control(pDX, IDC_BUY1, m_wndChkBuy1);
	DDX_Control(pDX, IDC_DOWN, m_wndChkDown);
	DDX_Control(pDX, IDC_LOSE, m_wndChkLose);
	DDX_Control(pDX, IDC_HP, m_wndChkHp);
}


BEGIN_MESSAGE_MAP(CWizPageExchSell, CWizPageExchBase)
END_MESSAGE_MAP()


// CWizPageExchSell message handlers

BOOL CWizPageExchSell::OnInitDialog()
{
	CWizPageExchBase::OnInitDialog();

	// TODO:  Add extra initialization here

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CWizPageExchSell::OnKillActive(int nPos /*= -1*/)
{
	if(nPos < 0) return TRUE;
	CString str;

	BOOL breturn;
	if(!InValidTest())
	{
		return FALSE;
	}

	breturn =  SetData();
	SaveOption();

	return breturn;
}

BOOL CWizPageExchSell::SetData()
{
	m_sExchSell.m_bMaidPeriod = m_wndChkBuy.GetCheck();
	m_sExchSell.m_bWinPeriod  = m_wndChkBuy1.GetCheck();
	m_sExchSell.m_bLosePeriod = m_wndChkLose.GetCheck();
	m_sExchSell.m_bFallPeriod = m_wndChkDown.GetCheck();
	m_sExchSell.m_bUpPeriod = m_wndChkHp.GetCheck();

	CString str;
	int     tem;
	float   ftem;

	m_wndEditSellPeriod.GetWindowText(str);
	tem = atoi(str);
	m_sExchSell.m_nMaidPeriod = (short)tem;

	m_wndEditWin.GetWindowText(str);
	tem = atoi(str);
	m_sExchSell.m_nWinPeriod = (short)tem;

	m_wndEditLose.GetWindowText(str);
	tem = atoi(str);
	m_sExchSell.m_nLosePeriod = (short)tem;

	m_wndEditFallDay.GetWindowText(str);
	tem = atoi(str);
	m_sExchSell.m_nFallPeriodCount = (short)tem;
	m_wndEditFallRation.GetWindowText(str);
	ftem = atof(str);
	m_sExchSell.m_nFallPeriod = (short)ftem;

	m_wndEditUpDay.GetWindowText(str);
	tem = atoi(str);
	m_sExchSell.m_nUpPeriodCount = (short)tem;
	m_wndEditUpRation.GetWindowText(str);
	ftem = atof(str);
	m_sExchSell.m_nUpPeriod = ftem;

	return SaveOption();
}

void CWizPageExchSell::PlayData()
{
	m_wndChkBuy.SetCheck((int)m_sExchSell.m_bMaidPeriod);
	m_wndChkBuy1.SetCheck((int)m_sExchSell.m_bWinPeriod);
	m_wndChkLose.SetCheck((int)m_sExchSell.m_bLosePeriod);
	m_wndChkDown.SetCheck((int)m_sExchSell.m_bFallPeriod);
	m_wndChkHp.SetCheck((int)m_sExchSell.m_bUpPeriod);

	CString str;
	str.Format("%.f", m_sExchSell.m_nMaidPeriod);
	m_wndEditSellPeriod.SetWindowText(str);
	str.Format("%d", m_sExchSell.m_nFallPeriod);
	m_wndEditFallRation.SetWindowText(str);
	str.Format("%d", m_sExchSell.m_nFallPeriodCount);
	m_wndEditFallDay.SetWindowText(str);

	str.Format("%.f", m_sExchSell.m_nWinPeriod);
	m_wndEditWin.SetWindowText(str);
	str.Format("%.f", m_sExchSell.m_nLosePeriod);
	m_wndEditLose.SetWindowText(str);

	str.Format("%d", m_sExchSell.m_nUpPeriodCount);
	m_wndEditUpDay.SetWindowText(str);
	str.Format("%d", m_sExchSell.m_nUpPeriod);
	m_wndEditUpRation.SetWindowText(str);
}

BOOL CWizPageExchSell::InValidTest()
{
	CString str;

	if( m_wndChkBuy.GetCheck() )
	{
		m_wndEditSellPeriod.GetWindowText(str);
		if( str.IsEmpty() )
		{
			MessageBox(_T("平仓周期不能为空"), FORMULAMAN_EXCH_PROMPT, MB_OK);
			return FALSE;
		}

		int  tem= atoi(str);
		if( tem <= 0 )
		{
			MessageBox(_T("平仓周期必须大于0"), FORMULAMAN_EXCH_PROMPT, MB_OK);
			return FALSE;
		}
	}
	if( m_wndChkBuy1.GetCheck() )
	{
		this->m_wndEditWin.GetWindowText(str);
		if( str.IsEmpty() )
		{
			MessageBox(_T("目标利润止赢不能为空"), FORMULAMAN_EXCH_PROMPT, MB_OK);
			return FALSE;
		}
		float  tem= atof(str);
		if( tem <= 0 )
		{
			MessageBox(_T("目标利润止赢必须大于0"), FORMULAMAN_EXCH_PROMPT, MB_OK);
			return FALSE;
		}
	}
	if( m_wndChkLose.GetCheck() )
	{
		this->m_wndEditLose.GetWindowText(str);
		if( str.IsEmpty() )
		{
			MessageBox(_T("损失止损不能为空"), FORMULAMAN_EXCH_PROMPT, MB_OK);
			return FALSE;
		}
		float  tem= atof(str);
		if( tem <= 0 )
		{
			MessageBox(_T("最大损失止损必须大于0"), FORMULAMAN_EXCH_PROMPT, MB_OK);
			return FALSE;
		}
	}
	if((!m_wndChkBuy.GetCheck()) && (!m_wndChkBuy1.GetCheck()) && (!m_wndChkLose.GetCheck())
		&& (!m_wndChkDown.GetCheck()) && (!m_wndChkHp.GetCheck()))
	{
		MessageBox(_T("平仓条件不能为空"), FORMULAMAN_EXCH_PROMPT, MB_OK);
			return FALSE;
	}
	
	return TRUE;
}

void CWizPageExchSell::OnSetActive()
{
	CSheetExchMain* pParent = (CSheetExchMain*)GetParent();
	if( pParent != NULL )
	{
		CString str(_T("系统评测-"));
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
		str += _T("平仓设置");
		pParent->SetWindowText(str);
	}

	PlayData();
}

/******************************************************************
*	函数功能：	初始化设置数据
*	函数参数：	TExchHead* pData : [in] 初值数据
*	返 回 值：	BOOL : 成功（TRUE），失败（FALSE）
*	调用依赖：	重载父类函数
*	作    者：
*	完成日期：
*	修改说明：	
*****************************************************************/
BOOL CWizPageExchSell::InitialData(TExchHead* pData)
{
	m_sExchSell = *((TExchSell*)pData);

	return TRUE;
}
