// DlgTestStrategy.cpp : 实现文件
//

#include "stdafx.h"
#include "DlgTestStrategy.h"
#include ".\dlgteststrategy.h"
#include "..\..\BizEntity\Quote\QuoteComm\pageid.h"
#include "PubStruct.h"
#include "DlgSaveTrig.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern UINT HS_USER_STRATEGY;
extern HWND g_hParentWnd;
// CDlgTestStrategy 对话框

IMPLEMENT_DYNAMIC(CDlgTestStrategy, CDialog)
CDlgTestStrategy::CDlgTestStrategy(CWnd* pParent /*=NULL*/,HSOpenExpressUseData* pData /*= NULL*/)
	: CDialog(CDlgTestStrategy::IDD, pParent)
{
	m_pData = pData;
	m_hParent = g_hParentWnd;
	m_pMap = NULL;
	m_hTrade = NULL;
}

CDlgTestStrategy::~CDlgTestStrategy()
{
}

void CDlgTestStrategy::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDITcode, m_wndCode);
	DDX_Control(pDX, IDC_EDITMONEY, m_wndMoney);
	DDX_Control(pDX, IDC_STRATEGYfee, m_wndFee);
	DDX_Control(pDX, IDC_DATETIMEPICKERDAYStart, m_wndDayStart);
	DDX_Control(pDX, IDC_DATETIMEPICKERTIMESTART, m_wndTimeStart);
	DDX_Control(pDX, IDC_DATETIMEPICKERDAYEND, m_wndDayEnd);
	DDX_Control(pDX, IDC_DATETIMEPICKERTIMEEND, m_wndTimeEnd);
	DDX_Control(pDX, IDC_COMBOcf, m_wndStategyTrig);
	DDX_Control(pDX, IDC_TABHQ, m_wndTab);
}


BEGIN_MESSAGE_MAP(CDlgTestStrategy, CDialog)
	ON_BN_CLICKED(IDC_BUTTONADD, OnBnClickedButtonadd)
	ON_BN_CLICKED(IDC_BUTTONSAVE, OnBnClickedButtonsave)
	ON_BN_CLICKED(IDC_BUTTONOK, OnBnClickedButtonok)
	ON_BN_CLICKED(IDC_BUTTONCANCEL, OnBnClickedButtoncancel)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TABHQ, OnTcnSelchangeTabhq)
	ON_EN_CHANGE(IDC_EDITcode, OnEnChangeEditCode)
	ON_REGISTERED_MESSAGE(HX_USER_DRAGDATA,OnUserDragData)
	ON_REGISTERED_MESSAGE(HS_USER_STRATEGY,OnStrategyTrade)
	ON_CBN_SELCHANGE(IDC_COMBOcf, OnCbnSelchangeCombocf)
	ON_BN_CLICKED(IDC_BUTTONSV, OnBnClickedButtonsv)
	ON_WM_CLOSE()
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BUTTONOPT, OnBnClickedButtonOPT)
	ON_BN_CLICKED(IDC_BUTTONBYDAY, OnBnClickedButtonbyday)
END_MESSAGE_MAP()


// CDlgTestStrategy 消息处理程序

BOOL CDlgTestStrategy::OnInitDialog()
{
	CDialog::OnInitDialog();
	m_wndTab.InsertItem(0,"触发策略");
	m_wndTab.InsertItem(1,"交易策略");

	m_pStrategy=new CDlgStrategy(this,m_pData);
	m_pStrategy->Create(IDD_TAB_STRATEGY_HQ,this);
	CRect rc;
	m_wndTab.GetClientRect(&rc);
	rc.top += 163;
	rc.left += 17;
	rc.bottom += 84;
	
	::MoveWindow(m_pStrategy->m_hWnd,rc.left,rc.top,rc.Width(),rc.Height(),1);
	::ShowWindow(m_pStrategy->m_hWnd,TRUE);
	m_wndTab.SetCurSel(0);
	::SendMessage(g_hParentWnd,HS_USER_STRATEGY,1,(LPARAM)(this->m_hWnd));
	if (!LoadStrategy())
	{
		MessageBox("没取到触发策略！","提示");
		return FALSE;
	}
	SetParam();
	((CButton*) GetDlgItem(IDC_RADIO01))->SetCheck(TRUE);
	return TRUE;
}
void CDlgTestStrategy::OnBnClickedButtonadd()//添加到监控
{
	if (!m_pStrategyHis)
	{
		return;
		//m_pStrategyHis = new StrategyHis;
	}
	CString cs;
	m_wndCode.GetWindowText(cs);
	if (cs.IsEmpty())
	{
		MessageBox("请填写代码！","提示");
		m_wndCode.SetFocus();
		return ;
	}
	if(!m_pStrategyHis->m_pCode)
		m_pStrategyHis->m_pCode = new CodeInfo;
	strncpy(m_pStrategyHis->m_pCode->m_cCode,cs,6);
	m_wndStategyTrig.GetWindowText(cs);
	StrategyTrig *Trig = new StrategyTrig;
	if (m_pStrategy->GetTrigData(Trig) && IsTradeValid())
	{
		void *temp = CDlgStrategy::GetStrategyTrig(m_pMap,cs.GetBuffer());
		if (temp)
		{
			if (!m_pStrategy->IsEqual((StrategyTrig*)temp ,Trig))
			{
				cs += "(自定义)";
			}
		}
		if (cs.IsEmpty())
		{
			cs = "(自定义)";
		}
		strncpy(m_pStrategyHis->m_cTrigName,cs,32);
		m_pStrategyHis->m_bFuQuan = Trig->m_bFuQuan ;
		m_pStrategyHis->m_lEffectEndTime = Trig->m_lEndTime;
		m_pStrategyHis->m_lEffectStartTime = Trig->m_lStartTime;
		m_pStrategyHis->m_nGap = Trig->m_nGap;
		m_pStrategyHis->m_nMaxBuyTimes = Trig->m_nMaxBuyTimes;
		m_pStrategyHis->m_nMaxSellTimes = Trig->m_nMaxSellTimes;
		m_pStrategyHis->m_nPeriodNumber = Trig->m_nPeriodNumber;
		m_pStrategyHis->m_nPeriodType = Trig->m_nPeriodType;
		if(!m_pStrategyHis->m_pData)
			m_pStrategyHis->m_pData = new CLineProperty;
		m_pStrategyHis->m_pData->m_pCurVal = Trig->m_pExpValue;
		if(Trig->m_pExpValue && Trig->m_pExpValue->GetExp())
			strncpy(m_pStrategyHis->m_sExpName,Trig->m_pExpValue->GetExp()->GetName(),32);
		//
	}
}

void CDlgTestStrategy::OnBnClickedButtonsave()//保存策略
{
	StrategyTrig *Trig = new StrategyTrig;
	if (!m_pStrategy->GetTrigData(Trig))
	{
		delete Trig;
		return;
	}
	CString triname;
	m_wndStategyTrig.GetWindowText(triname);
	CDlgSaveTrig dlg(this);
	dlg.m_pMap = m_pMap;
	dlg.m_sName = triname;
	if(dlg.DoModal() == IDOK)
	{
		strncpy(Trig->m_cName,dlg.m_sName.GetBuffer(),32);
		strncpy(m_pStrategyHis->m_cTrigName,Trig->m_cName,32);
		//strncpy(Trig->m_cExplain,dlg.m_sExp.GetBuffer(),64);
		WORD id = CDlgStrategy::GetStrategyTrigKey(m_pMap,dlg.m_sName.GetBuffer());
		if (id)
		{
			CDlgStrategy::DelStrategyTrig(m_pMap,id);
		}
		CDlgStrategy::AddStrategyTrig(m_pMap,Trig);
		MessageBox("保存策略成功！","提示");
		LoadStrategy();
		m_wndStategyTrig.SelectString(-1,dlg.m_sName);
		OnCbnSelchangeCombocf();
	}
}

void CDlgTestStrategy::OnBnClickedButtonok()
{
	if (GetParam())
	{
		m_pStrategyHis->m_bOpt = 0;
		m_pStrategyHis->m_bByDay = 0;
		if(IsStrategyChg())
		{
			if (MessageBox("触发策略已修改，是否保存？","提示",MB_YESNO) == IDYES)
			{
				OnBnClickedButtonsv();
			}
		}
		if (IsTradeValid())
		{
			m_pStrategy->SaveAllTrig(m_pMap);
			CloseTradeWin();
			OnOK();
		}	
	}
}

void CDlgTestStrategy::OnBnClickedButtoncancel()
{
// 	if(IsStrategyChg(FALSE))
// 	{
// 		if (MessageBox("触发策略已修改，是否保存？","提示",MB_YESNO) == IDYES)
// 		{
// 			OnBnClickedButtonsv();
// 		}
// 	}
	//m_pStrategyHis->m_pCode = NULL;
	m_pStrategy->SaveAllTrig(m_pMap);
	CloseTradeWin();
	OnCancel();
}

void CDlgTestStrategy::OnTcnSelchangeTabhq(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
	int nCurSel = m_wndTab.GetCurSel();
	switch(nCurSel)
	{
	case 0:
		this->GetDlgItem(IDC_COMBOcf)->ShowWindow(TRUE);
		this->GetDlgItem(IDC_STATICcf)->ShowWindow(TRUE);
		this->GetDlgItem(IDC_BUTTONSAVE)->ShowWindow(TRUE);
		this->GetDlgItem(IDC_BUTTONSV)->ShowWindow(TRUE);
		//m_pStrategy->ShowWindow(TRUE);
		::ShowWindow(m_pStrategy->m_hWnd,TRUE);
		if (m_hTrade)
		{
			::ShowWindow(m_hTrade,FALSE);
		}
		break;
	case 1:
		this->GetDlgItem(IDC_COMBOcf)->ShowWindow(FALSE);
		this->GetDlgItem(IDC_STATICcf)->ShowWindow(FALSE);
		this->GetDlgItem(IDC_BUTTONSAVE)->ShowWindow(FALSE);
		this->GetDlgItem(IDC_BUTTONSV)->ShowWindow(FALSE);
		//m_pStrategy->ShowWindow(FALSE);
		::ShowWindow(m_pStrategy->m_hWnd,FALSE);
		if (m_hTrade)
		{
			::ShowWindow(m_hTrade,TRUE);
		}
		break;
	}
}
BOOL CDlgTestStrategy::GetParam(BOOL isByDay /*= FALSE*/)
{
	if (!m_pStrategyHis)
	{
		return FALSE;
	}
	CString cs;
	m_wndCode.GetWindowText(cs);
	if (cs.IsEmpty())
	{
		MessageBox("请填写代码！","提示");
		m_wndCode.SetFocus();
		return FALSE;
	}
	if(!m_pStrategyHis->m_pCode)
		m_pStrategyHis->m_pCode = new CodeInfo;
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
	sel = GetCheckedRadioButton(IDC_RADIO01,IDC_RADIO03);
	switch(sel)
	{
	case IDC_RADIO02:
		mltp = 10000;
		break;
	case IDC_RADIO03:
		mltp = 100000000;
		break;
	default:
		mltp = 1;
		break;
	}
	m_pStrategyHis->m_nMoney *= mltp;
	StrategyTrig *Trig = new StrategyTrig;
	m_wndStategyTrig.GetWindowText(cs);
	if(m_pStrategy->GetTrigData(Trig,isByDay))
	{
		void *temp = CDlgStrategy::GetStrategyTrig(m_pMap,cs.GetBuffer());
		if (temp)
		{
			if (!m_pStrategy->IsEqual((StrategyTrig*)temp ,Trig))
			{
				cs += "(自定义)";
			}
		}
		if (cs.IsEmpty())
		{
			cs = "(自定义)";
		}
		strncpy(m_pStrategyHis->m_cTrigName,cs,32);
		m_pStrategyHis->m_bFuQuan = Trig->m_bFuQuan ;
		m_pStrategyHis->m_lEffectEndTime = Trig->m_lEndTime;
		m_pStrategyHis->m_lEffectStartTime = Trig->m_lStartTime;
		m_pStrategyHis->m_nGap = Trig->m_nGap;
		m_pStrategyHis->m_nMaxBuyTimes = Trig->m_nMaxBuyTimes;
		m_pStrategyHis->m_nMaxSellTimes = Trig->m_nMaxSellTimes;
		m_pStrategyHis->m_nPeriodNumber = Trig->m_nPeriodNumber;
		m_pStrategyHis->m_nPeriodType = Trig->m_nPeriodType;

		if(Trig->m_pExpValue && Trig->m_pExpValue->GetExp())//保存公式名和参数
		{
			m_pStrategyHis->m_mapVar.RemoveAll();
			strncpy(m_pStrategyHis->m_sExpName,Trig->m_pExpValue->GetExp()->GetName(),32);
			if (Trig->m_pExpValue->GetExp()->GetParamVar())
			{
				CMapVariabile *pMap = Trig->m_pExpValue->GetExp()->GetParamVar();
				POSITION pos;
				CString key;
				CValue *pval;
				pos = pMap->GetStartPosition();
				while (pos)
				{
					pMap->GetNextAssoc(pos,key,pval);
					m_pStrategyHis->m_mapVar.SetAt(key,pval->GetValue());
				}
			}
		}
	}
	else
	{
		return FALSE;
	}
	//add by lxqi 20090401
 	CString strTemp;
	m_wndFee.GetWindowText(strTemp);
	m_pStrategyHis->m_nFees = atof(strTemp);
	if (m_pStrategyHis->m_nFees < 0)
	{
		MessageBox("请正确填写手续费率！","提示");
		m_wndFee.SetFocus();
		return FALSE;
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
	return TRUE;
}


void  CDlgTestStrategy::SetParam()
{
	if (!m_pStrategyHis)
	{
		return;
	}
	if (m_pStrategyHis->m_pCode)
	{
		char code[7]={0};
		strncpy(code,m_pStrategyHis->m_pCode->m_cCode,6);
		m_wndCode.SetWindowText(code);
	}
	char cname[33]={0};
	strncpy(cname,m_pStrategyHis->m_cTrigName,32);
	CString str = cname, name = cname;
	if (!CDlgStrategy::GetStrategyTrig(m_pMap,name.GetBuffer()) && name.Find("(自定义)",0) != -1)
	{
		str = name.Left(name.Find("(自定义)",0));
	}
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
	delete time;
	if (!str.IsEmpty())
	{
		m_wndStategyTrig.SelectString(-1,str);
		CString cs;
		m_wndStategyTrig.GetWindowText(cs);
		if (cs != str)//策略已被删除
		{
			m_wndStategyTrig.SetCurSel(-1);
		}
	}
	ResetTrig();
	if (name.IsEmpty())
	{
		m_wndStategyTrig.SetCurSel(0);
		OnCbnSelchangeCombocf();
	}
}
void CDlgTestStrategy::OnEnChangeEditCode()
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
LRESULT CDlgTestStrategy::OnUserDragData(WPARAM wParam, LPARAM lParam)
{
	HSCurKeyInfo* pData = (HSCurKeyInfo*)lParam;
	if(pData == NULL)
		return 0;
	m_sCode = pData->GetCode()->GetCode();
	m_wndCode.SetWindowText(m_sCode);

	NextDlgCtrl(); 
	char code[7] = {0};
	strncpy(code,pData->m_cCode.m_cCode,6);
	char *lp = new char[sizeof(HWND)+7];
	ZeroMemory(lp,sizeof(HWND)+7);
	memcpy(lp,(void*)&m_hTrade,sizeof(HWND));
	memcpy(lp+sizeof(HWND),code,7);
	::SendMessage(g_hParentWnd,HS_USER_STRATEGY,7,(LPARAM)lp);//向交易发代码
	return 1;
}


void CDlgTestStrategy::OnCbnSelchangeCombocf()
{
	CString name;
	m_wndStategyTrig.GetWindowText(name);
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

BOOL CDlgTestStrategy::LoadStrategy()
{
	m_pMap = (CMapWordToPtr*)::SendMessage(g_hParentWnd,HS_USER_STRATEGY,0,0);
	if (!m_pMap)
	{
		return FALSE;
	}
	m_wndStategyTrig.ResetContent();
	POSITION pos = m_pMap->GetStartPosition();
	WORD key;
	void *val;
	while(pos)
	{
		m_pMap->GetNextAssoc(pos,key,val);
		char name[33]={0};
		strncpy(name,((StrategyTrig*)val)->m_cName,32);
		m_wndStategyTrig.AddString(name);
	}
	return TRUE;
}
LRESULT CDlgTestStrategy::OnStrategyTrade(WPARAM wp, LPARAM lp)
{
	if (lp == NULL)
	{
		return 0;
	}
	m_hTrade = (HWND)lp;

	CRect rc;
	m_wndTab.GetClientRect(&rc);
	rc.top += 162;
	rc.left += 17;
	rc.bottom += 84;
	rc.top -= 15;
	rc.bottom += 20;
	::MoveWindow(m_hTrade,rc.left,rc.top,rc.Width(),rc.Height(),1);
	::ShowWindow(m_hTrade,FALSE);
	return 0;
}

void CDlgTestStrategy::ResetTrig()
{
	if (!m_pStrategyHis)
	{
		return;
	}
	StrategyTrig *Trig = new StrategyTrig;
	Trig->m_bFuQuan = m_pStrategyHis->m_bFuQuan;
	Trig->m_lEndTime = m_pStrategyHis->m_lEffectEndTime;
	Trig->m_lStartTime = m_pStrategyHis->m_lEffectStartTime;
	Trig->m_nGap = m_pStrategyHis->m_nGap;
	Trig->m_nMaxBuyTimes = m_pStrategyHis->m_nMaxBuyTimes;
	Trig->m_nMaxSellTimes = m_pStrategyHis->m_nMaxSellTimes;
	Trig->m_nPeriodNumber = m_pStrategyHis->m_nPeriodNumber;
	Trig->m_nPeriodType = m_pStrategyHis->m_nPeriodType; 
	if (m_pStrategyHis->m_pData && m_pStrategyHis->m_pData->m_pCurVal && ((CExpValue*)(m_pStrategyHis->m_pData->m_pCurVal))->GetExp())
	{
		Trig->m_pExpValue = new CExpValue;
		CExpression *pex = new CExpression;
		char name[33]={0};
		strncpy(name,m_pStrategyHis->m_sExpName,32);
		pex->SetName(name);
		pex->SetParamVar(((CExpValue*)(m_pStrategyHis->m_pData->m_pCurVal))->GetExp()->GetParamVar());
		Trig->m_pExpValue->SetValue(pex);
	}
	m_pStrategy->SetTrigData(Trig);
}
BOOL CDlgTestStrategy::IsTradeValid()
{
	int n = ::SendMessage(g_hParentWnd,HS_USER_STRATEGY,3,(LPARAM)m_hTrade);
	return (n == 1);
}
void CDlgTestStrategy::OnBnClickedButtonsv()
{
	CString name;
	m_wndStategyTrig.GetWindowText(name);
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
	strncpy(Trig->m_cName,name.GetBuffer(),32);
	strncpy(m_pStrategyHis->m_cTrigName,Trig->m_cName,32);
	WORD id = CDlgStrategy::GetStrategyTrigKey(m_pMap,name.GetBuffer());
	if (id)
	{
// 		StrategyTrig *pt = (StrategyTrig*)CDlgStrategy::GetStrategyTrig(m_pMap,name.GetBuffer());
// 		strncpy(Trig->m_cExplain,pt->m_cExplain,64);
		CDlgStrategy::DelStrategyTrig(m_pMap,id);
	}
	CDlgStrategy::AddStrategyTrig(m_pMap,Trig);
	MessageBox("保存成功！","提示");
	LoadStrategy();
	m_wndStategyTrig.SelectString(-1,name);
	OnCbnSelchangeCombocf();
}

void CDlgTestStrategy::CloseTradeWin()
{
	::SendMessage(g_hParentWnd,HS_USER_STRATEGY,4,(LPARAM)(m_hTrade));
}
void CDlgTestStrategy::OnClose()
{
	if(IsStrategyChg(FALSE))
	{
		if (MessageBox("触发策略已修改，是否保存？","提示",MB_YESNO) == IDYES)
		{
			OnBnClickedButtonsv();
		}
	}
	//m_pStrategyHis->m_pCode = NULL;
	m_pStrategy->SaveAllTrig(m_pMap);
	CloseTradeWin();
	CDialog::OnClose();
}

BOOL CDlgTestStrategy::PreTranslateMessage(MSG* pMsg) 
{ 
	if(pMsg->message == WM_KEYDOWN) 
	{ 
		if(pMsg->wParam == VK_RETURN ) 
		{ 
			UINT nID = GetFocus()->GetDlgCtrlID(); 
			switch( nID) 
			{ 
			case IDC_BUTTONADD:
				OnBnClickedButtonadd();
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
			case IDC_BUTTONOPT:
				OnBnClickedButtonOPT();
				break;
			case IDC_BUTTONBYDAY:
				OnBnClickedButtonbyday();
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
BOOL CDlgTestStrategy::IsStrategyChg(BOOL NoName /*= TRUE*/)
{
	CString trigname;
	m_wndStategyTrig.GetWindowText(trigname);
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

void CDlgTestStrategy::OnPaint()
{
	CDialog::OnPaint();
	int nCurSel = m_wndTab.GetCurSel();
	switch(nCurSel)
	{
	case 0:
		this->GetDlgItem(IDC_COMBOcf)->ShowWindow(TRUE);
		this->GetDlgItem(IDC_STATICcf)->ShowWindow(TRUE);
		this->GetDlgItem(IDC_BUTTONSAVE)->ShowWindow(TRUE);
		this->GetDlgItem(IDC_BUTTONSV)->ShowWindow(TRUE);
		::ShowWindow(m_pStrategy->m_hWnd,TRUE);
		if (m_hTrade)
		{
			::ShowWindow(m_hTrade,FALSE);
		}
		break;
	case 1:
		this->GetDlgItem(IDC_COMBOcf)->ShowWindow(FALSE);
		this->GetDlgItem(IDC_STATICcf)->ShowWindow(FALSE);
		this->GetDlgItem(IDC_BUTTONSAVE)->ShowWindow(FALSE);
		this->GetDlgItem(IDC_BUTTONSV)->ShowWindow(FALSE);
		::ShowWindow(m_pStrategy->m_hWnd,FALSE);
		if (m_hTrade)
		{
			::ShowWindow(m_hTrade,TRUE);
		}
		break;
	}
}
void CDlgTestStrategy::OnBnClickedButtonOPT()
{
	// TODO: 在此添加控件通知处理程序代码
	if (GetParam())
	{
		m_pStrategyHis->m_bOpt = 1;
		if(IsStrategyChg())
		{
			if (MessageBox("触发策略已修改，是否保存？","提示",MB_YESNO) == IDYES)
			{
				OnBnClickedButtonsv();
			}
		}
		if (IsTradeValid())
		{
			m_pStrategy->SaveAllTrig(m_pMap);
			CloseTradeWin();
			OnOK();
		}	
	}
}

void CDlgTestStrategy::OnBnClickedButtonbyday()
{
	// TODO: 在此添加控件通知处理程序代码
	if (GetParam(TRUE))
	{
		m_pStrategyHis->m_bByDay = 1;
		if(IsStrategyChg())
		{
			if (MessageBox("触发策略已修改，是否保存？","提示",MB_YESNO) == IDYES)
			{
				OnBnClickedButtonsv();
			}
		}
		if (IsTradeValid())
		{
			m_pStrategy->SaveAllTrig(m_pMap);
			CloseTradeWin();
			OnOK();
		}	
	}
}
