// DlgSpeedTest.cpp : 实现文件
//

#include "stdafx.h"
#include "Login.h"
#include "DlgSpeedTest.h"
#include "DoLogin.h"
#include "..\HelperLib\GeneralHelper.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CDlgSpeedTest* g_pDlgSpeedTest;
// CDlgSpeedTest 对话框

IMPLEMENT_DYNAMIC(CDlgSpeedTest, CDialog)

CDlgSpeedTest::CDlgSpeedTest(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgSpeedTest::IDD, pParent)
{
	m_cType = CEV_Connect_HQ;
	m_bIsTesting = FALSE;
	m_nNotiCount = 0;

	m_tcInfo.m_Type = SSL;
	strncpy_s(m_tcInfo.m_ca, _T("hsca"), sizeof(m_tcInfo.m_ca));
	CString sCertFile =  CGeneralHelper::GetAppPath() + _T("Cert\\cert.pem");
	strncpy_s(m_tcInfo.m_cert, sCertFile, sizeof(m_tcInfo.m_cert));
	CString sCertKey =  CGeneralHelper::GetAppPath() + _T("Cert\\key.pem");
	strncpy_s(m_tcInfo.m_key, sCertKey, sizeof(m_tcInfo.m_key));
	strncpy_s(m_tcInfo.m_pwd, _T("111111"), sizeof(m_tcInfo.m_pwd));
}

CDlgSpeedTest::~CDlgSpeedTest()
{
}

void CDlgSpeedTest::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB_SPEET, m_cTabCtl);
	DDX_Control(pDX, IDC_LIST_TEST, m_cList);
	DDX_Control(pDX, IDC_BUTTON_TEST, m_cBtnTest);
}


BEGIN_MESSAGE_MAP(CDlgSpeedTest, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_TEST, &CDlgSpeedTest::OnBnClickedButtonTest)
	ON_BN_CLICKED(IDCANCEL, &CDlgSpeedTest::OnBnClickedCancel)
	ON_BN_CLICKED(IDOK, &CDlgSpeedTest::OnBnClickedOk)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_SPEET, &CDlgSpeedTest::OnTcnSelchangeTabSpeet)
END_MESSAGE_MAP()

BOOL WINAPI FNotify(SpeedTestResult *pVoid)
{
	if (g_pDlgSpeedTest && ::IsWindow(g_pDlgSpeedTest->GetSafeHwnd()))
	{
		return g_pDlgSpeedTest->Notify(pVoid);
	}
	return FALSE;
}
// CDlgSpeedTest 消息处理程序
BOOL CDlgSpeedTest::OnInitDialog()
{
	CDialog::OnInitDialog();
	m_cTabCtl.SetMinTabWidth(50);
	m_cTabCtl.InsertItem(0,_T("行情"));
	m_cTabCtl.InsertItem(1,_T("交易"));
	m_cList.SetExtendedStyle(m_cList.GetExtendedStyle() | LVS_EX_FULLROWSELECT);
	m_cList.InsertColumn(0,_T("站点名称"),LVCFMT_CENTER,100);
	m_cList.InsertColumn(1,_T("响应时间"),LVCFMT_LEFT,100);
	m_cList.InsertColumn(2,_T("在线率"),LVCFMT_LEFT,100);
	m_cList.InsertColumn(3,_T("综合"),LVCFMT_LEFT,100);
	Init();
	return TRUE;
}
void CDlgSpeedTest::Init()
{
	m_cList.DeleteAllItems();
	CArray<CommInfo*,CommInfo*> *pSite = g_pDoLogin->m_commData.GetSysServerInfo();
	CArray<CommInfo*,CommInfo*> *pSiteUsr = g_pDoLogin->m_commData.GetUsrServerInfo();
	int sel = m_cTabCtl.GetCurSel();
	switch (sel)
	{
	case 0:
		m_cType = CEV_Connect_HQ;
		break;
	case 1:
		m_cType = CEV_Connect_JY;
		break;
	}
	m_nNotiCount = 0;
	m_nTotalCount = 0;
 	if (pSite)
 	{
 		for (int i=0; i<pSite->GetCount(); i++)
 		{
 			CommInfo *pInfo = pSite->GetAt(i);
 			if (pInfo->m_cType == m_cType)
 			{
 				int n = m_cList.GetItemCount();
				m_nTotalCount++;
				m_cList.InsertItem(n,pInfo->m_strServerID);
				m_cList.SetItemText(n,1,"...");
				m_cList.SetItemText(n,2,"...");
				m_cList.SetItemText(n,3,"...");
 			}
 		}
 	}
	if (pSiteUsr)
	{
		for (int i=0; i<pSiteUsr->GetCount(); i++)
		{
			CommInfo *pInfo = pSiteUsr->GetAt(i);
			if (pInfo->m_cType == m_cType)
			{
				int n = m_cList.GetItemCount();
				m_nTotalCount++;
				m_cList.InsertItem(n,pInfo->m_strServerID);
				m_cList.SetItemText(n,1,"...");
				m_cList.SetItemText(n,2,"...");
				m_cList.SetItemText(n,3,"...");
			}
		}
	}
}
void CDlgSpeedTest::TestSpeed()
{
	m_ayTestResult.RemoveAll();
	TestSpeedCallBack fCallBack = FNotify;
	g_pDoLogin->m_pManager->TestSiteSpeed(m_cType, fCallBack, (void*)&m_tcInfo);
}
void CDlgSpeedTest::StopTest()
{
	g_pDoLogin->m_pManager->StopSiteTest(m_cType);
}
BOOL CDlgSpeedTest::Notify(SpeedTestResult *pVoid)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	SpeedTestResult *pRes = pVoid;
	CSingleLock lock(&m_cCritical);
	lock.Lock();
	m_ayTestResult.Add(*pRes);
	m_nNotiCount ++;
	if (m_nNotiCount >= m_nTotalCount)
	{
		m_cBtnTest.SetWindowText(_T("测速"));
		m_cBtnTest.EnableWindow(TRUE);
		m_cTabCtl.EnableWindow(TRUE);
		GetDlgItem(IDOK)->EnableWindow(TRUE);
		m_bIsTesting = FALSE;
	}
	Sort();
	lock.Unlock();
	return TRUE;
}
void CDlgSpeedTest::OnBnClickedButtonTest()
{
	// TODO: 在此添加控件通知处理程序代码
	int sel = m_cTabCtl.GetCurSel();
	switch (sel)
	{
	case 0:
		m_cType = CEV_Connect_HQ;
		break;
	case 1:
		m_cType = CEV_Connect_JY;
		break;
	}
	if (!m_bIsTesting)
	{
		m_nNotiCount = 0;
		TestSpeed();
		m_cBtnTest.SetWindowText(_T("测速中..."));
		m_cBtnTest.EnableWindow(FALSE);
		m_cTabCtl.EnableWindow(FALSE);
		GetDlgItem(IDOK)->EnableWindow(FALSE);
		m_bIsTesting = TRUE;
	}
}

void CDlgSpeedTest::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	StopTest();
	OnCancel();
}

void CDlgSpeedTest::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	int sel = m_cList.GetSelectionMark();
	if (sel != -1)
	{
		CString str = m_cList.GetItemText(sel,0);
		g_pDoLogin->m_commData.SetDefault(m_cType,str.GetBuffer());
		OnOK();
	}
}

void CDlgSpeedTest::Sort()
{
	CMap<DWORD,DWORD,SpeedTestResult,SpeedTestResult> mapTmp;
	CArray<SpeedTestResult,SpeedTestResult> ayTmp;
	for (int i=0; i<m_ayTestResult.GetCount(); i++)
	{
		SpeedTestResult res = m_ayTestResult.GetAt(i);
		if (!res.m_bError)
		{
			mapTmp.SetAt(res.m_result,res);
		}
		else
			ayTmp.Add(res);
	}
	m_ayTestResult.RemoveAll();
	POSITION pos = mapTmp.GetStartPosition();
	DWORD key;
	SpeedTestResult val;
	while (pos)
	{
		mapTmp.GetNextAssoc(pos,key,val);
		m_ayTestResult.Add(val);
	}
	for (int j=0; j<ayTmp.GetCount(); j++)
	{
		m_ayTestResult.Add(ayTmp.GetAt(j));
	}

	for(int i=0; i<m_cList.GetItemCount(); i++)
	{
		CString str;
		str = m_cList.GetItemText(i,0);
		if (FindRes(str))
		{
			m_cList.DeleteItem(i);
			i--;
		}
	}
	ShowRes();
}

BOOL CDlgSpeedTest::FindRes(CString name)
{
	for (int i=0; i<m_ayTestResult.GetCount(); i++)
	{
		SpeedTestResult res = m_ayTestResult.GetAt(i);
		if (!name.Compare(res.m_strServerID))
		{
			return TRUE;
		}
	}
	return FALSE;
}

void CDlgSpeedTest::ShowRes()
{
	for (int i=0; i<m_ayTestResult.GetCount(); i++)
	{
		SpeedTestResult res = m_ayTestResult.GetAt(i);
		//inner_trade_test是内部初始化交易连接使用的 不应该显示给用户
		if(strncmp(res.m_strServerID,"inner_trade_test",sizeof("inner_trade_test")) == 0) 
			continue;

		m_cList.InsertItem(i,res.m_strServerID);
		if (!res.m_bError)
		{
			CString str;
			str.Format("%d",res.m_timeSpan);
			m_cList.SetItemText(i,1,str);
			str.Format("%f",res.m_onlineRadio);
			m_cList.SetItemText(i,2,str);
			str.Format("%d",res.m_result);
			m_cList.SetItemText(i,3,str);
		}
		else
		{
			m_cList.SetItemText(i,1,"——");
			m_cList.SetItemText(i,2,"——");
			m_cList.SetItemText(i,3,res.m_strError);
		}
	}
}
void CDlgSpeedTest::OnTcnSelchangeTabSpeet(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
	Init();
}
