// DlgStrategyJY.cpp : 实现文件
//

#include "stdafx.h"
#include "DlgStrategyJY.h"
#include ".\dlgstrategyjy.h"
#include "DlgTestStrategy.h"
#include "DlgSaveTrig.h"
#include "PubStruct.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern UINT HS_USER_STRATEGY;
extern HWND g_hParentWnd;
// CDlgStrategyJY 对话框

IMPLEMENT_DYNAMIC(CDlgStrategyJY, CDialog)
CDlgStrategyJY::CDlgStrategyJY(CWnd* pParent /*=NULL*/,HSOpenExpressUseData* pData /*= NULL*/)
	: CDialog(CDlgStrategyJY::IDD, pParent)
{
	m_pData = pData;
	m_hParent = g_hParentWnd;
	m_pInfo = NULL;
	m_pMap = NULL;
	m_hTrade = NULL;
}

CDlgStrategyJY::~CDlgStrategyJY()
{
}

void CDlgStrategyJY::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TABHQ, m_wndTab);
	DDX_Control(pDX, IDC_EDITcode, m_wndCode);
	DDX_Control(pDX, IDC_COMBOcf, m_wndStartegyCF);
	DDX_Control(pDX, IDC_CHECKxd, m_wndCheck);
}


BEGIN_MESSAGE_MAP(CDlgStrategyJY, CDialog)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TABHQ, OnTcnSelchangeTabhq)
	ON_BN_CLICKED(IDC_BUTTONTEST, OnBnClickedButtontest)
	ON_BN_CLICKED(IDC_BUTTONSAVE, OnBnClickedButtonsave)
	ON_BN_CLICKED(IDC_BUTTONOK, OnBnClickedButtonok)
	ON_BN_CLICKED(IDC_BUTTONCANCEL, OnBnClickedButtoncancel)
	ON_EN_CHANGE(IDC_EDITcode, OnEnChangeEditcode)
	ON_REGISTERED_MESSAGE(HX_USER_DRAGDATA,OnUserDragData)
	ON_REGISTERED_MESSAGE(HS_USER_STRATEGY,OnStrategyTrade)
	ON_CBN_SELCHANGE(IDC_COMBOcf, OnCbnSelchangeCombocf)
	ON_BN_CLICKED(IDC_BUTTONSV, OnBnClickedButtonsv)
	ON_WM_CLOSE()
	ON_WM_PAINT()
END_MESSAGE_MAP()


// CDlgStrategyJY 消息处理程序
BOOL CDlgStrategyJY::OnInitDialog()
{
	CDialog::OnInitDialog();
	m_wndTab.InsertItem(0,"触发策略");
	m_wndTab.InsertItem(1,"交易策略");
	m_pStrategy=new CDlgStrategy(this,m_pData);
	m_pStrategy->Create(IDD_TAB_STRATEGY_HQ,this);

	CRect rc;
	m_wndTab.GetClientRect(&rc);
	rc.top += 100;
	rc.left += 12;
	rc.right += 9;
	rc.bottom += 18;
	::MoveWindow(m_pStrategy->m_hWnd,rc.left,rc.top,rc.Width(),rc.Height(),1);
	::ShowWindow(m_pStrategy->m_hWnd,TRUE);
	m_wndTab.SetCurSel(0);
	::SendMessage(g_hParentWnd,HS_USER_STRATEGY,6,(LPARAM)(this->m_hWnd));
	if (!LoadStrategy())
	{
		MessageBox("没取到触发策略！","提示");
		return FALSE;
	}	
	SetParam();
	return TRUE;
}
void CDlgStrategyJY::OnTcnSelchangeTabhq(NMHDR *pNMHDR, LRESULT *pResult)
{
	*pResult = 0;
	int nCurSel = m_wndTab.GetCurSel();
	switch(nCurSel)
	{
	case 0:
		::ShowWindow(m_pStrategy->m_hWnd,TRUE);
		this->GetDlgItem(IDC_BUTTONSAVE)->ShowWindow(TRUE);
		this->GetDlgItem(IDC_BUTTONSV)->ShowWindow(TRUE);
		this->GetDlgItem(IDC_STATICcf)->ShowWindow(TRUE);
		this->GetDlgItem(IDC_COMBOcf)->ShowWindow(TRUE);
		if (m_hTrade)
		{
			::ShowWindow(m_hTrade,FALSE);
		}
		break;
	case 1:
		::ShowWindow(m_pStrategy->m_hWnd,FALSE);
		this->GetDlgItem(IDC_BUTTONSAVE)->ShowWindow(FALSE);
		this->GetDlgItem(IDC_BUTTONSV)->ShowWindow(FALSE);
		this->GetDlgItem(IDC_COMBOcf)->ShowWindow(FALSE);
		this->GetDlgItem(IDC_STATICcf)->ShowWindow(FALSE);
		if (m_hTrade)
		{
			::ShowWindow(m_hTrade,TRUE);
		}
		break;
	}
}


void CDlgStrategyJY::OnBnClickedButtontest()//测试
{
	if (!m_pInfo)
	{
		return;
	}
	CString code;
	m_wndCode.GetWindowText(code);
	strncpy(m_pInfo->m_cCode,code,6);
	StrategyTrig *Trig = new StrategyTrig;
	CString cs;
	m_wndStartegyCF.GetWindowText(cs);
	if(m_pStrategy->GetTrigData(Trig))
	{
		void *temp = CDlgStrategy::GetStrategyTrig(m_pMap,cs.GetBuffer());
		if (temp)
		{
			if (!m_pStrategy->IsEqual((StrategyTrig*)temp ,Trig))
			{
				cs += "(自定义)";
			}
			/*strncpy(m_pInfo->m_cTrigName,cs.GetBuffer(),32);*/
		}
		if (cs.IsEmpty())
		{
			cs = "自定义";
		}
		strncpy(m_pInfo->m_cTrigName,cs,32);
	
		m_pInfo->m_bFuQuan = Trig->m_bFuQuan ;
		m_pInfo->m_lEndTime = Trig->m_lEndTime;
		m_pInfo->m_lStartTime = Trig->m_lStartTime;
		m_pInfo->m_nGap = Trig->m_nGap;
		m_pInfo->m_nMaxBuyTimes = Trig->m_nMaxBuyTimes;
		m_pInfo->m_nMaxSellTimes = Trig->m_nMaxSellTimes;
		m_pInfo->m_nPeriodNumber = Trig->m_nPeriodNumber;
		m_pInfo->m_nPeriodType = Trig->m_nPeriodType;
		m_pInfo->m_nLastTrigTime = (long)this->m_hWnd;//存放窗口句柄
		if(!m_pInfo->m_pData)
			m_pInfo->m_pData = new CLineProperty;
		m_pInfo->m_pData->m_pCurVal = Trig->m_pExpValue;
		if(Trig->m_pExpValue->GetExp())
			strncpy(m_pInfo->m_sExpName,Trig->m_pExpValue->GetExp()->GetName(),32);
	}
	else
	{
		return;
	}
	//////交易策略
	::SendMessage(g_hParentWnd,HS_USER_STRATEGY,2,(LPARAM)m_pInfo);//测试
}

void CDlgStrategyJY::OnBnClickedButtonsave()//保存策略
{
	StrategyTrig *Trig = new StrategyTrig;
	if (!m_pStrategy->GetTrigData(Trig))
	{
		delete Trig;
		return;
	}
	CString triname;
	m_wndStartegyCF.GetWindowText(triname);
	CDlgSaveTrig dlg(this);
	dlg.m_pMap = m_pMap;
	dlg.m_sName = triname;
	if(dlg.DoModal() == IDOK)
	{
		strncpy(Trig->m_cName,dlg.m_sName.GetBuffer(),32);
		strncpy(m_pInfo->m_cTrigName,Trig->m_cName,32);
		//strncpy(Trig->m_cExplain,dlg.m_sExp.GetBuffer(),64);
		WORD id = CDlgStrategy::GetStrategyTrigKey(m_pMap,dlg.m_sName.GetBuffer());
		if (id)
		{
			CDlgStrategy::DelStrategyTrig(m_pMap,id);
		}
		CDlgStrategy::AddStrategyTrig(m_pMap,Trig);
		MessageBox("保存策略成功！","提示");
		LoadStrategy();
		m_wndStartegyCF.SelectString(-1,dlg.m_sName);
		OnCbnSelchangeCombocf();
	}
}

void CDlgStrategyJY::OnBnClickedButtonok()
{
	if (GetParam())
	{
		if(IsStrategyChg())
		{
			if (MessageBox("触发策略已修改，是否保存？","提示",MB_YESNO) == IDYES)
			{
				OnBnClickedButtonsv();
			}
		}
		if(IsTradeValid())
		{
			m_pStrategy->SaveAllTrig(m_pMap);
			CloseTradeWin();
			OnOK();
		}
	}
}

void CDlgStrategyJY::OnBnClickedButtoncancel()
{
// 	if(IsStrategyChg(FALSE))
// 	{
// 		if (MessageBox("触发策略已修改，是否保存？","提示",MB_YESNO) == IDYES)
// 		{
// 			OnBnClickedButtonsv();
// 		}
// 	}
	if (m_pInfo->m_nID == 0)//新增
	{
		m_pInfo->m_nID = -1;
	}
	m_pStrategy->SaveAllTrig(m_pMap);
	CloseTradeWin();
	OnCancel();
}

BOOL  CDlgStrategyJY::GetParam()
{
	if (!m_pInfo)
	{
		return FALSE;
	}
	CString cs;
	m_wndCode.GetWindowText(cs);
	if (cs.IsEmpty()/* || cs.GetLength()!=6*/)
	{
		MessageBox("请填写股票代码！","提示");
		m_wndCode.SetFocus();
		return FALSE;
	}
	strncpy(m_pInfo->m_cCode,cs,6);
	if (m_wndCheck.GetCheck())
	{
		m_pInfo->m_bXD = 1;
	}
	else
		m_pInfo->m_bXD = 0;
	m_wndStartegyCF.GetWindowText(cs);
	m_pInfo->m_nBuyTimes = 0;
	m_pInfo->m_nSellTimes = 0;
	StrategyTrig *Trig = new StrategyTrig;
	if(m_pStrategy->GetTrigData(Trig))
	{
		void *temp = CDlgStrategy::GetStrategyTrig(m_pMap,cs.GetBuffer());
		if (temp)
		{
			if (!m_pStrategy->IsEqual((StrategyTrig*)temp ,Trig))
			{
				cs += "(自定义)";
			}
			strncpy(m_pInfo->m_cTrigName,cs.GetBuffer(),32);
		}
		if (cs.IsEmpty())
		{
			cs = "(自定义)";
		}
		strncpy(m_pInfo->m_cTrigName,cs.GetBuffer(),32);
				
		m_pInfo->m_bFuQuan = Trig->m_bFuQuan ;
		m_pInfo->m_lEndTime = Trig->m_lEndTime;
		m_pInfo->m_lStartTime = Trig->m_lStartTime;
		m_pInfo->m_nGap = Trig->m_nGap;
		m_pInfo->m_nMaxBuyTimes = Trig->m_nMaxBuyTimes;
		m_pInfo->m_nMaxSellTimes = Trig->m_nMaxSellTimes;
		m_pInfo->m_nPeriodNumber = Trig->m_nPeriodNumber;
		m_pInfo->m_nPeriodType = Trig->m_nPeriodType;
		m_pInfo->m_nSize = 1;
		if(!m_pInfo->m_pData)
			m_pInfo->m_pData = new CLineProperty;
		m_pInfo->m_pData->m_pCurVal = Trig->m_pExpValue;
		if(Trig->m_pExpValue && Trig->m_pExpValue->GetExp())
			strncpy(m_pInfo->m_sExpName,Trig->m_pExpValue->GetExp()->GetName(),32);
	}
	else
	{
		return FALSE;
	}
	return TRUE;
}
void CDlgStrategyJY::SetParam()
{
	if (!m_pInfo)
	{
		return;
	}
	char code[7];
	strncpy(code,m_pInfo->m_cCode,6);
	code[6]=0;
	m_sCode = code;
	m_wndCode.SetWindowText(code);
	if (m_pInfo->m_bXD)
	{
		m_wndCheck.SetCheck(TRUE);
	}
	else
		m_wndCheck.SetCheck(FALSE);
	char cname[33]={0};
	strncpy(cname,m_pInfo->m_cTrigName,32);
	CString str = cname, name = cname;
	if (!CDlgStrategy::GetStrategyTrig(m_pMap,name.GetBuffer()) && name.Find("(自定义)",0) != -1)
	{
		str = name.Left(name.Find("(自定义)",0));
	}
	if (!str.IsEmpty())
	{
		m_wndStartegyCF.SelectString(-1,str);
		CString cs;
		m_wndStartegyCF.GetWindowText(cs);
		if (cs != str)//策略已被删除
		{
			m_wndStartegyCF.SetCurSel(-1);
		}
	}
	ResetTrig();
	if(name.IsEmpty())
	{
		m_wndStartegyCF.SetCurSel(0);
		OnCbnSelchangeCombocf();
	}
}

void CDlgStrategyJY::OnEnChangeEditcode()
{
	CString strTemp;
	m_wndCode.GetWindowText(strTemp);
	if (strTemp.GetLength() >= 1 && strTemp != m_sCode)
	{
		strTemp.MakeUpper();
		m_wndCode.SetWindowText(NULL);
		AlarmMsgData *pAl = new AlarmMsgData;
		pAl->pMsg = new MSG;
		memset(pAl->pMsg,0,sizeof(MSG));
		pAl->pMsg->hwnd = this->m_hParent;
		pAl->pWnd = this->GetDlgItem(IDC_EDITcode);
		pAl->pMsg->wParam = (WPARAM)strTemp.GetAt(0);
		pAl->pMsg->message = WM_KEYDOWN;
		::SendMessage(g_hParentWnd,HX_USER_COMPILEDATA,(WPARAM)23,(LPARAM)pAl);
	}
}

LRESULT CDlgStrategyJY::OnUserDragData(WPARAM wParam, LPARAM lParam)
{
	HSCurKeyInfo* pData = (HSCurKeyInfo*)lParam;
	if(pData == NULL)
		return 0;
	if (!m_pInfo)
	{
		m_pInfo = new StrategyInfo;
	}
	m_pInfo->m_cCodeType = pData->m_cCode.m_cCodeType;
	m_sCode = pData->GetCode()->GetCode();
	m_wndCode.SetWindowText(m_sCode);
	char code[7] = {0};
	strncpy(code,pData->m_cCode.m_cCode,6);
	char *lp = new char[sizeof(HWND)+7];
	ZeroMemory(lp,sizeof(HWND)+7);
	memcpy(lp,(void*)&m_hTrade,sizeof(HWND));
	memcpy(lp+sizeof(HWND),code,7);
	::SendMessage(g_hParentWnd,HS_USER_STRATEGY,7,(LPARAM)lp);//向交易发代码
	return 1;
}

BOOL CDlgStrategyJY::LoadStrategy()
{
	m_pMap = (CMapWordToPtr*)::SendMessage(g_hParentWnd,HS_USER_STRATEGY,0,0);
	if (!m_pMap)
	{
		return FALSE;
	}
	m_wndStartegyCF.ResetContent();
	POSITION pos = m_pMap->GetStartPosition();
	WORD key;
	void *val;
	while(pos)
	{
		m_pMap->GetNextAssoc(pos,key,val);
		char name[33]={0};
		strncpy(name,((StrategyTrig*)val)->m_cName,32);
		m_wndStartegyCF.AddString(name);
	}
	return TRUE;
}
void CDlgStrategyJY::OnCbnSelchangeCombocf()
{
	CString name;
	m_wndStartegyCF.GetWindowText(name);
	if (name.IsEmpty())
	{
		return;
	}
	POSITION pos = m_pMap->GetStartPosition();
	WORD key;
	void *val;
	while(pos)
	{
		m_pMap->GetNextAssoc(pos,key,val);
		if (strncmp(((StrategyTrig*)val)->m_cName,name,32) == 0)
		{
			m_pStrategy->SetTrigData((StrategyTrig*)val);
			break;
		}
	}
}

void CDlgStrategyJY::ResetTrig()
{
	if (!m_pInfo)
	{
		return;
	}
	StrategyTrig *Trig = new StrategyTrig;
	Trig->m_bFuQuan = m_pInfo->m_bFuQuan;
	Trig->m_lEndTime = m_pInfo->m_lEndTime;
	Trig->m_lStartTime = m_pInfo->m_lStartTime;
	Trig->m_nGap = m_pInfo->m_nGap;
	Trig->m_nMaxBuyTimes = m_pInfo->m_nMaxBuyTimes;
	Trig->m_nMaxSellTimes = m_pInfo->m_nMaxSellTimes;
	Trig->m_nPeriodNumber = m_pInfo->m_nPeriodNumber;
	Trig->m_nPeriodType = m_pInfo->m_nPeriodType;
	if (m_pInfo->m_pData && m_pInfo->m_pData->m_pCurVal && ((CExpValue*)(m_pInfo->m_pData->m_pCurVal))->GetExp())
	{
		Trig->m_pExpValue = new CExpValue;
		CExpression *pex = new CExpression;
		char name[33]={0};
		strncpy(name,m_pInfo->m_sExpName,32);
		pex->SetName(name);
		pex->SetParamVar(((CExpValue*)(m_pInfo->m_pData->m_pCurVal))->GetExp()->GetParamVar());
		Trig->m_pExpValue->SetValue(pex);
	}
	m_pStrategy->SetTrigData(Trig);
}
LRESULT CDlgStrategyJY::OnStrategyTrade(WPARAM wp, LPARAM lp)
{
	if (lp == NULL)
	{
		return 0;
	}
	m_hTrade = (HWND)lp;
	
	CRect rc;
	m_wndTab.GetClientRect(&rc);
	rc.top += 100;
	rc.left += 12;
	rc.right += 9;
	rc.bottom += 20;
	rc.top -= 15;
	rc.bottom += 20;
	::MoveWindow(m_hTrade,rc.left,rc.top,rc.Width(),rc.Height(),1);
	::ShowWindow(m_hTrade,FALSE);
	return 0;
}
void CDlgStrategyJY::OnBnClickedButtonsv()
{
	CString name;
	m_wndStartegyCF.GetWindowText(name);
	if (name.IsEmpty())
	{
		OnBnClickedButtonsave();
		return;
	}
	StrategyTrig *Trig = new StrategyTrig;
	if (!m_pStrategy->GetTrigData(Trig))
	{
		delete Trig;
		return;
	}
	WORD id = CDlgStrategy::GetStrategyTrigKey(m_pMap,name.GetBuffer());
	if (id)
	{
// 		StrategyTrig *pt = (StrategyTrig*)CDlgStrategy::GetStrategyTrig(m_pMap,name.GetBuffer());
// 		strncpy(Trig->m_cExplain,pt->m_cExplain,64);
		CDlgStrategy::DelStrategyTrig(m_pMap,id);
	}
	strncpy(Trig->m_cName,name.GetBuffer(),32);
	strncpy(m_pInfo->m_cTrigName,Trig->m_cName,32);
	CDlgStrategy::AddStrategyTrig(m_pMap,Trig);
	MessageBox("保存成功！","提示");
	LoadStrategy();
	m_wndStartegyCF.SelectString(-1,name);
	OnCbnSelchangeCombocf();
}

BOOL CDlgStrategyJY::IsTradeValid()
{
	int n = ::SendMessage(g_hParentWnd,HS_USER_STRATEGY,3,(LPARAM)m_hTrade);
	return (n == 1);
}
void CDlgStrategyJY::CloseTradeWin()
{
	::SendMessage(g_hParentWnd,HS_USER_STRATEGY,4,(LPARAM)(m_hTrade));
}
void CDlgStrategyJY::OnClose()
{
	if(IsStrategyChg(FALSE))
	{
		if (MessageBox("触发策略已修改，是否保存？","提示",MB_YESNO) == IDYES)
		{
			OnBnClickedButtonsv();
		}
	}
	if (m_pInfo->m_nID == 0)//新增
	{
		m_pInfo->m_nID = -1;
	}
	m_pStrategy->SaveAllTrig(m_pMap);
	CloseTradeWin();
	CDialog::OnClose();
}
BOOL CDlgStrategyJY::PreTranslateMessage(MSG* pMsg) 
{ 
	if(pMsg->message == WM_KEYDOWN) 
	{ 
		if(pMsg->wParam == VK_RETURN ) 
		{ 
			UINT nID = GetFocus()->GetDlgCtrlID(); 
			switch( nID) 
			{ 
			case IDC_BUTTONTEST:
				OnBnClickedButtontest();
				break;
			case IDC_BUTTONSAVE:
				OnBnClickedButtonsave();
				break;
			case IDC_BUTTONOK:
				OnBnClickedButtonok();
				break;
			case IDC_BUTTONCANCEL:
				OnBnClickedButtoncancel();
				break;
			case IDC_BUTTONSV:
				OnBnClickedButtonsv();
				break;
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
	return CDialog::PreTranslateMessage(pMsg);
} 
BOOL CDlgStrategyJY::IsStrategyChg(BOOL NoName/* = TRUE*/)
{
	CString trigname;
	m_wndStartegyCF.GetWindowText(trigname);
	if (trigname.IsEmpty())
	{
		if (NoName)
		{
			return TRUE;
		}
		else
		{
			return FALSE;
		}
	}

	StrategyTrig *Trig = new StrategyTrig;
	if (m_pStrategy->GetTrigData(Trig))
	{
		StrategyTrig *pt = (StrategyTrig*)CDlgStrategy::GetStrategyTrig(m_pMap,trigname.GetBuffer());
		if (pt && CDlgStrategy::IsEqual(pt,Trig))
		{
			return FALSE;
		}
	}
	else
	{
		return FALSE;
	}
	return TRUE;
}

void CDlgStrategyJY::OnPaint()
{
	CDialog::OnPaint();
	int nCurSel = m_wndTab.GetCurSel();
	switch(nCurSel)
	{
	case 0:
		::ShowWindow(m_pStrategy->m_hWnd,TRUE);
		this->GetDlgItem(IDC_BUTTONSAVE)->ShowWindow(TRUE);
		this->GetDlgItem(IDC_BUTTONSV)->ShowWindow(TRUE);
		this->GetDlgItem(IDC_STATICcf)->ShowWindow(TRUE);
		this->GetDlgItem(IDC_COMBOcf)->ShowWindow(TRUE);
		if (m_hTrade)
		{
			::ShowWindow(m_hTrade,FALSE);
		}
		break;
	case 1:
		::ShowWindow(m_pStrategy->m_hWnd,FALSE);
		this->GetDlgItem(IDC_BUTTONSAVE)->ShowWindow(FALSE);
		this->GetDlgItem(IDC_BUTTONSV)->ShowWindow(FALSE);
		this->GetDlgItem(IDC_COMBOcf)->ShowWindow(FALSE);
		this->GetDlgItem(IDC_STATICcf)->ShowWindow(FALSE);
		if (m_hTrade)
		{
			::ShowWindow(m_hTrade,TRUE);
		}
		break;
	}
}