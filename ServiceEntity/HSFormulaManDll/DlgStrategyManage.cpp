// DlgStrategyManage.cpp : 实现文件
//

#include "stdafx.h"
#include "DlgStrategyManage.h"
#include ".\dlgstrategymanage.h"
#include "PubStruct.h"
extern UINT HS_USER_STRATEGY;
extern HWND g_hParentWnd;
// CDlgStrategyManage 对话框

IMPLEMENT_DYNAMIC(CDlgStrategyManage, CDialog)
CDlgStrategyManage::CDlgStrategyManage(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgStrategyManage::IDD, pParent)
{
	m_pMap = NULL;
	m_hTrade = NULL;
}

CDlgStrategyManage::~CDlgStrategyManage()
{
}

void CDlgStrategyManage::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TABTrig, m_wndTab);
	DDX_Control(pDX, IDC_EDITNAME, m_wndName);
	DDX_Control(pDX, IDC_LISTTRIG, m_wndList);
}


BEGIN_MESSAGE_MAP(CDlgStrategyManage, CDialog)
	ON_BN_CLICKED(IDC_BUTTONADD, OnBnClickedButtonadd)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TABTrig, OnTcnSelchangeTabtrig)
	ON_BN_CLICKED(IDC_BUTTONCANCEL, OnBnClickedButtoncancel)
	ON_BN_CLICKED(IDC_BUTTONDELETE, OnBnClickedButtondelete)
	ON_BN_CLICKED(IDC_BUTTONMODIFY, OnBnClickedButtonmodify)
	ON_NOTIFY(NM_CLICK, IDC_LISTTRIG, OnNMClickListtrig)
	ON_BN_CLICKED(IDC_BUTTONSAVE, OnBnClickedButtonsave)
	ON_BN_CLICKED(IDC_BUTTONCC, OnBnClickedButtoncc)
	ON_REGISTERED_MESSAGE(HS_USER_STRATEGY,OnStrategyTrade)
	ON_WM_CLOSE()
	ON_WM_PAINT()
END_MESSAGE_MAP()


// CDlgStrategyManage 消息处理程序
BOOL CDlgStrategyManage::OnInitDialog()
{
	CDialog::OnInitDialog();
	m_wndList.InsertColumn(0,"名称",0,282);
	m_wndList.SetExtendedStyle(m_wndList.GetExtendedStyle() | LVS_EX_FULLROWSELECT);
	m_wndName.SetLimitText(32);
	HSOpenExpressUseData data(hxShowByExpressType|Strategy);
	CString strName = _T("策略交易");
	data.m_strName  = &strName;
	data.m_hParent  = this->m_hWnd;
	m_pStrategy = new CDlgStrategy(this,&data);
	m_pStrategy->Create(IDD_TAB_STRATEGY_HQ,this);
	m_wndTab.InsertItem(0,"触发策略");
	m_wndTab.InsertItem(1,"交易策略");
	CRect rc;
	m_wndTab.GetClientRect(rc);
	rc.top += 33;
	rc.left += 10;
	rc.bottom += 9;
	rc.top += 122;

	::MoveWindow(m_pStrategy->m_hWnd,rc.left,rc.top,rc.Width(),rc.Height(),1);
	::SendMessage(g_hParentWnd,HS_USER_STRATEGY,5,(LPARAM)(this->m_hWnd));//发送交易策略管理窗口
	ShowTab(TRUE);
	LoadStrategy();
	SetRDOnly(TRUE);
	StrategyTrig trig;
	m_pStrategy->SetTrigData(&trig);
	return TRUE;
}

void CDlgStrategyManage::Show(BOOL bshow/* = TRUE*/)
{
	this->GetDlgItem(IDC_STATICNAME)->ShowWindow(bshow);
	this->GetDlgItem(IDC_EDITNAME)->ShowWindow(bshow);
	this->GetDlgItem(IDC_LISTTRIG)->ShowWindow(bshow);
	this->GetDlgItem(IDC_BUTTONADD)->ShowWindow(bshow);
	this->GetDlgItem(IDC_BUTTONMODIFY)->ShowWindow(bshow);
	this->GetDlgItem(IDC_BUTTONDELETE)->ShowWindow(bshow);
	this->GetDlgItem(IDC_BUTTONSAVE)->ShowWindow(bshow);
	this->GetDlgItem(IDC_BUTTONCC)->ShowWindow(bshow);
}
void CDlgStrategyManage::OnBnClickedButtonadd()
{
	m_bAdd = TRUE;
	SetRDOnly(FALSE);
	m_wndName.SetWindowText(NULL);
	this->GetDlgItem(IDC_BUTTONADD)->EnableWindow(FALSE);
	this->GetDlgItem(IDC_BUTTONMODIFY)->EnableWindow(FALSE);
	this->GetDlgItem(IDC_BUTTONDELETE)->EnableWindow(FALSE);
	this->GetDlgItem(IDC_BUTTONSAVE)->EnableWindow(TRUE);
	this->GetDlgItem(IDC_BUTTONCC)->EnableWindow(TRUE);
	StrategyTrig trig;
	m_pStrategy->SetTrigData(&trig);
}

void CDlgStrategyManage::ShowTab(BOOL bshow /*= TRUE*/)
{
	m_wndTab.ShowWindow(bshow);
	if (bshow)
	{
		::ShowWindow(m_pStrategy->m_hWnd,TRUE);
		if (m_hTrade)
		{
			::ShowWindow(m_hTrade,FALSE);
		}
		m_wndTab.SetCurSel(0);
	}
	else
	{
		::ShowWindow(m_pStrategy->m_hWnd,FALSE);
		if (m_hTrade)
		{
			::ShowWindow(m_hTrade,FALSE);
		}
	}
}

void CDlgStrategyManage::OnTcnSelchangeTabtrig(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
	int nCurSel = m_wndTab.GetCurSel();
	switch(nCurSel)
	{
	case 0:
		::ShowWindow(m_pStrategy->m_hWnd,TRUE);
		Show(TRUE);
		if (m_hTrade)
		{
			::ShowWindow(m_hTrade,FALSE);
		}
		break;
	case 1:
		::ShowWindow(m_pStrategy->m_hWnd,FALSE);
		Show(FALSE);
		if (m_hTrade)
		{
			::ShowWindow(m_hTrade,TRUE);
		}
		break;
	}
}


void CDlgStrategyManage::OnBnClickedButtoncancel()
{
	m_pStrategy->SaveAllTrig(m_pMap);
	CloseTradeWin();
	OnCancel();
}

void CDlgStrategyManage::OnBnClickedButtondelete()
{
	CString cs;
	m_wndName.GetWindowText(cs);
	if (cs.IsEmpty())
	{
		MessageBox("请选择要删除的策略！","提示");
		return;
	}
	if (MessageBox("确定要删除此策略吗？","提示",MB_ICONQUESTION | MB_YESNO) == IDYES)
	{
		WORD id = CDlgStrategy::GetStrategyTrigKey(m_pMap,cs.GetBuffer());
		if (id)
		{
			CDlgStrategy::DelStrategyTrig(m_pMap,id);
			LoadStrategy();
		}
	}
}

void CDlgStrategyManage::OnBnClickedButtonmodify()
{
	CString cs;
	m_wndName.GetWindowText(cs);
	if (cs.IsEmpty())
	{
		MessageBox("请选择要改的策略！","提示");
		return;
	}
	m_bAdd = FALSE;
	SetRDOnly(FALSE);
	m_wndName.SetReadOnly(TRUE);
	this->GetDlgItem(IDC_BUTTONADD)->EnableWindow(FALSE);
	this->GetDlgItem(IDC_BUTTONMODIFY)->EnableWindow(FALSE);
	this->GetDlgItem(IDC_BUTTONDELETE)->EnableWindow(FALSE);
	this->GetDlgItem(IDC_BUTTONSAVE)->EnableWindow(TRUE);
	this->GetDlgItem(IDC_BUTTONCC)->EnableWindow(TRUE);
}

BOOL CDlgStrategyManage::LoadStrategy()
{
	m_pMap = (CMapWordToPtr*)::SendMessage(g_hParentWnd,HS_USER_STRATEGY,0,0);
	if (!m_pMap)
	{
		return FALSE;
	}
	POSITION pos = m_pMap->GetStartPosition();
	WORD key;
	void *val;
	int n;
	if (m_wndList.GetItemCount()>0)
	{
		m_wndList.DeleteAllItems();
	}
	while(pos)
	{
		n = m_wndList.GetItemCount();
		m_pMap->GetNextAssoc(pos,key,val);
		char name[33]={0};
		//char explain[65]={0};
		strncpy(name,((StrategyTrig*)val)->m_cName,32);
		//strncpy(explain,((StrategyTrig*)val)->m_cExplain,64);
		m_wndList.InsertItem(n,name);
		//m_wndList.SetItemText(n,1,explain);
	}
	return TRUE;
}

void CDlgStrategyManage::OnNMClickListtrig(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
	int sel = m_wndList.GetSelectionMark();
	if (sel != -1)
	{
		m_wndName.SetWindowText(m_wndList.GetItemText(sel,0));
		POSITION pos = m_pMap->GetStartPosition();
		WORD key;
		void *val;
		while(pos)
		{
			m_pMap->GetNextAssoc(pos,key,val);
			if (!strcmp(((StrategyTrig*)val)->m_cName, m_wndList.GetItemText(sel,0).GetBuffer()))
			{
				m_pStrategy->SetTrigData((StrategyTrig*)val);
				break;
			}
		}
		
	}
	else
	{
		m_wndName.SetWindowText(NULL);
	}
}

void CDlgStrategyManage::SetRDOnly(BOOL ro/*=TRUE*/)
{
	m_wndList.EnableWindow(ro);
	m_wndName.SetReadOnly(ro);
	m_pStrategy->SetRDOnly(ro);
}
void CDlgStrategyManage::OnBnClickedButtonsave()
{
	CString csName,csExp;
	m_wndName.GetWindowText(csName);
	if (csName.IsEmpty())
	{
		MessageBox("请填写名称！","提示");
		m_wndName.SetFocus();
		return;
	}
	//m_wndExplain.GetWindowText(csExp);
	StrategyTrig* Trig = new StrategyTrig;
	if(m_pStrategy->GetTrigData(Trig))
	{
		strncpy(Trig->m_cName,csName,32);
		//strncpy(Trig->m_cExplain,csExp,64);
		void* pTemp = NULL;
		if (m_bAdd)//添加
		{
			if (CDlgStrategy::GetStrategyTrig(m_pMap,Trig->m_cName))
			{
				MessageBox("已有该策略，请换个名称！","提示");
				m_wndName.SetFocus();
				return;
			}
			WORD nTemp = 1;
			while(m_pMap->Lookup(nTemp,pTemp))
			{
				nTemp++;
			}
			m_pMap->SetAt(nTemp,Trig);
		}
		else//修改
		{
			WORD key;
			POSITION pos = m_pMap->GetStartPosition();
			while (pos)
			{
				m_pMap->GetNextAssoc(pos,key,pTemp);
				if (strcmp(((StrategyTrig*)pTemp)->m_cName, Trig->m_cName) == 0)
				{
					m_pMap->SetAt(key,Trig);
					break;
				}
			}
		}
		SetRDOnly(TRUE);
		this->GetDlgItem(IDC_BUTTONADD)->EnableWindow(TRUE);
		this->GetDlgItem(IDC_BUTTONMODIFY)->EnableWindow(TRUE);
		this->GetDlgItem(IDC_BUTTONDELETE)->EnableWindow(TRUE);
		this->GetDlgItem(IDC_BUTTONSAVE)->EnableWindow(FALSE);
		this->GetDlgItem(IDC_BUTTONCC)->EnableWindow(FALSE);
		LoadStrategy();
	}
}

void CDlgStrategyManage::OnBnClickedButtoncc()
{
	m_wndName.SetWindowText(NULL);
	SetRDOnly(TRUE);
	this->GetDlgItem(IDC_BUTTONADD)->EnableWindow(TRUE);
	this->GetDlgItem(IDC_BUTTONMODIFY)->EnableWindow(TRUE);
	this->GetDlgItem(IDC_BUTTONDELETE)->EnableWindow(TRUE);
	this->GetDlgItem(IDC_BUTTONSAVE)->EnableWindow(FALSE);
	this->GetDlgItem(IDC_BUTTONCC)->EnableWindow(FALSE);
	StrategyTrig trig;
	m_pStrategy->SetTrigData(&trig);
}

LRESULT CDlgStrategyManage::OnStrategyTrade(WPARAM wp, LPARAM lp)
{
	if (lp == NULL)
	{
		return 0;
	}
	m_hTrade = (HWND)lp;

	CRect rc;
	m_wndTab.GetClientRect(&rc);
	rc.top += 33;
	rc.left += 10;
	rc.bottom += 9;
	::MoveWindow(m_hTrade,rc.left,rc.top,rc.Width(),rc.Height(),1);
	::ShowWindow(m_hTrade,FALSE);
	return 0;
}
void CDlgStrategyManage::CloseTradeWin()
{
	::SendMessage(g_hParentWnd,HS_USER_STRATEGY,4,(LPARAM)(m_hTrade));
}
void CDlgStrategyManage::OnClose()
{
	m_pStrategy->SaveAllTrig(m_pMap);
	CloseTradeWin();
	CDialog::OnClose();
}
BOOL CDlgStrategyManage::PreTranslateMessage(MSG* pMsg) 
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
			case IDC_BUTTONMODIFY:
				OnBnClickedButtonmodify();
				break;
			case IDC_BUTTONDELETE:
				OnBnClickedButtondelete();
				break;
			case IDC_BUTTONSAVE:
				OnBnClickedButtonsave();
				break;
			case IDC_BUTTONCC:
				OnBnClickedButtoncc();
				break;
			case IDC_BUTTONCANCEL:
				OnBnClickedButtoncancel();
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

void CDlgStrategyManage::OnPaint()
{
	CDialog::OnPaint();
	int nCurSel = m_wndTab.GetCurSel();
	switch(nCurSel)
	{
	case 0:
		::ShowWindow(m_pStrategy->m_hWnd,TRUE);
		Show(TRUE);
		if (m_hTrade)
		{
			::ShowWindow(m_hTrade,FALSE);
		}
		break;
	case 1:
		::ShowWindow(m_pStrategy->m_hWnd,FALSE);
		Show(FALSE);
		if (m_hTrade)
		{
			::ShowWindow(m_hTrade,TRUE);
		}
		break;
	}
}