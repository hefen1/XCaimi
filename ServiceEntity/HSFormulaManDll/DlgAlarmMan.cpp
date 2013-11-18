///////////////////////////////////////////////////////////////////////////////
// 文件名:			DlgAlarmMan.cpp
// 创建者：				
// 创建时间：		2002-4-25
// 内容描述：		"预警系统"功能主模块
///////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DlgAlarmMan.h"
#include "DlgAlarmParamSet.h"
#include "DlgAlarmSet.h"
#include "DlgAlarmAnalysis.h"

#include "Express.h"
#include "SystemFun.h"
#include "FormulaCalc.h"

extern HWND			 g_hParentWnd;
extern CSystemFun* g_pSystemFun; 
BOOL CDlgAlarmMan::m_bAlarmStateFlag = FALSE;

IMPLEMENT_DYNAMIC(CDlgAlarmMan, CDialog)
CDlgAlarmMan::CDlgAlarmMan(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgAlarmMan::IDD, pParent)
{
	m_strAlarmFileName =  CGeneralHelper::GetSpecifyPath(CGeneralHelper::SETTING_PATH)  + _T("AlarmCondition.dat");
	m_strAlarmInfoFileName =  CGeneralHelper::GetSpecifyPath(CGeneralHelper::SETTING_PATH)  + _T("AlarmInfo.dat");
	m_wStyle = HS_ALARM_STYLE_ALARMINFO_SAVE | HS_ALARM_STYLE_SOUNDFILE_SHARE;
	Create(IDD, pParent);
}

CDlgAlarmMan::~CDlgAlarmMan()
{
	KillTimer((UINT)this);
	WriteAlarmInfo();

	m_ayAlarmData.RemoveAll();

	for( int i = 0; i < m_CalculateData.GetSize(); i++ )
	{
		delete m_CalculateData.GetAt(i);
	}
	m_CalculateData.RemoveAll();
	m_itemList.DeleteImageList();
}

void CDlgAlarmMan::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CON_LIST, m_ListCondition);
	DDX_Control(pDX, IDC_ALARM_LIST, m_ListAlarm);
}


BEGIN_MESSAGE_MAP(CDlgAlarmMan, CDialog)
	ON_BN_CLICKED(IDC_ADD_CON, OnBnClicked1014)
	ON_BN_CLICKED(IDD_WRAN_ANALYSE, OnBnClickedWranAnalyse)
	ON_BN_CLICKED(IDC_CHAGE_CON, OnBnClickedChageCon)
	ON_NOTIFY(NM_CLICK, IDC_CON_LIST, OnNMClickConList)
	ON_BN_CLICKED(IDC_DEL_CON, OnBnClickedDelCon)
	ON_BN_CLICKED(IDC_DEL_ALARM, OnBnClickedDelAlarm)
	ON_BN_CLICKED(IDC_BEGIN_ALARM, OnBnClickedBeginAlarm)
	ON_BN_CLICKED(IDC_HIDE, OnBnClickedHide)
	ON_BN_CLICKED(IDC_COPY, OnBnClickedCopy)
	ON_NOTIFY(NM_CLICK, IDC_ALARM_LIST, OnNMClickAlarmList)
	ON_REGISTERED_MESSAGE(HX_USER_COMPILEDATA, OnFramMsgPress)
	ON_WM_TIMER()
	ON_NOTIFY(NM_RCLICK, IDC_CON_LIST, OnNMRclickConList)
	ON_COMMAND(ID_ALARM1_BEGIN_ALL, OnAlarm1BeginAll)
	ON_COMMAND(ID_ALARM1_END_ALL, OnAlarm1EndAll)
	ON_COMMAND(ID_ALARM1_BEGIN_CUR, OnAlarm1BeginCur)
	ON_COMMAND(ID_ALARM1_END_CUR, OnAlarm1EndCur)
	ON_COMMAND(ID_ALARM1_EXIT, OnAlarm1Exit)
	ON_COMMAND(ID_ALARM1_CHG_CON, OnAlarm1ChgCon)
	ON_COMMAND(ID_ALARM1_ADD_CON, OnAlarm1AddCon)
	ON_COMMAND(ID_ALARM1_DEL_CON, OnAlarm1DelCon)
	ON_COMMAND(ID_ALARM1_BEGIN, OnAlarm1Begin)
	ON_COMMAND(ID_ALARM1_END, OnAlarm1End)
	ON_NOTIFY(NM_DBLCLK, IDC_CON_LIST, OnNMDblclkConList)
	ON_COMMAND(ID_ALARM2_EXIT, OnAlarm2Exit)
	ON_COMMAND(ID_ALARM2_DEL_ALL, OnAlarm2DelAll)
	ON_COMMAND(ID_ALARM2_DEL_CUR, OnAlarm2DelCur)
	ON_NOTIFY(NM_RCLICK, IDC_ALARM_LIST, OnNMRclickAlarmList)
	ON_WM_SHOWWINDOW()
	ON_WM_ACTIVATE()
	ON_NOTIFY(NM_DBLCLK, IDC_ALARM_LIST, OnNMDblclkAlarmList)
	ON_NOTIFY(NM_KILLFOCUS, IDC_CON_LIST, OnNMKillfocusConList)
	ON_NOTIFY(NM_SETFOCUS, IDC_CON_LIST, OnNMSetfocusConList)
	ON_NOTIFY(NM_KILLFOCUS, IDC_ALARM_LIST, OnNMKillfocusAlarmList)
	ON_NOTIFY(NM_SETFOCUS, IDC_ALARM_LIST, OnNMSetfocusAlarmList)
END_MESSAGE_MAP()


// CDlgAlarmMan message handlers

void CDlgAlarmMan::OnCancel()
{
	WriteAlarmData();
	//停止预警 
	//::SendMessage(g_hParentWnd, HX_USER_COMPILEDATA, 26, 1);
	CFormulaCalc::StopAlarm();

	(CWnd::FromHandle(g_hParentWnd))->SetFocus();
	ShowWindow(SW_HIDE);
}

void CDlgAlarmMan::OnBnClicked1014()
{
	SetConditionButton(FALSE);
	SetAlarmButton(FALSE);
	AddCon();
}

void CDlgAlarmMan::OnBnClickedWranAnalyse()
{
	SetConditionButton(FALSE);
	SetAlarmButton(FALSE);
	CDlgAlarmAnalysis WranAnalyseDlg((void*)&m_DisplayData, CWnd::FromHandle(g_hParentWnd));
	WranAnalyseDlg.DoModal();
}

void CDlgAlarmMan::Init()
{
	DWORD dWord =  0 ;//m_ListCondition.GetExStyle();	
	dWord |=  LVS_EX_FULLROWSELECT;
	dWord |=  LVS_EX_FLATSB;
	m_ListCondition.SetExtendedStyle(dWord);
	m_ListAlarm.SetExtendedStyle(dWord);

	m_itemList.Create(IDB_BITMAP_ALARM, 16, 3, RGB(255, 255, 255));

	m_ListCondition.SetImageList(&m_itemList, LVSIL_SMALL);
	m_ListAlarm.SetImageList(&m_itemList, LVSIL_SMALL);

	InitList();
	ReadAlarmData();
	ReadAlarmInfo();
	ReDrawCondition();
	ReDrawAlarm();

	SetConditionButton(FALSE);
	SetAlarmButton(FALSE);
}

void CDlgAlarmMan::InitList()
{
	LV_COLUMN	lvcol;
	lvcol.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
	lvcol.fmt = LVCFMT_CENTER;//居中
	lvcol.cx = 70;

	m_ListCondition.DeleteAllItems();
	m_ListAlarm.DeleteAllItems();
	while( m_ListCondition.DeleteColumn(0) != 0 );
	while( m_ListAlarm.DeleteColumn(0) != 0 );

	lvcol.iSubItem = 0;
	CString str;
	str = _T(HS_CONDITION_CON);
	lvcol.pszText = (char*)LPCTSTR(str);// HS_CONDITION_CON;
	m_ListCondition.InsertColumn(0, &lvcol);
	str = _T(HS_ALARM_CODE);
	lvcol.pszText = (char*)LPCTSTR(str);//HS_ALARM_CODE;
	m_ListAlarm.InsertColumn(0, &lvcol);

	lvcol.iSubItem = 1;
	str = _T(HS_CONDITION_TYPE);
	lvcol.pszText = (char*)LPCTSTR(str);//HS_CONDITION_TYPE;
	m_ListCondition.InsertColumn(1, &lvcol);
	str = _T(HS_CONDITION_CON);
	lvcol.pszText = (char*)LPCTSTR(str);// HS_CONDITION_CON;
	m_ListAlarm.InsertColumn(1, &lvcol);

	lvcol.iSubItem = 2;
	str = _T(HS_CONDITION_STK);
	lvcol.pszText = (char*)LPCTSTR(str);//HS_CONDITION_STK;
	m_ListCondition.InsertColumn(2, &lvcol);
	lvcol.cx = 140;
	str = _T(HS_ALARM_TIME);
	lvcol.pszText = (char*)LPCTSTR(str);//HS_ALARM_TIME;
	m_ListAlarm.InsertColumn(2, &lvcol);

	lvcol.cx = 70;
	lvcol.iSubItem = 3;
	str = _T(HS_CONDITION_NUM);
	lvcol.pszText = (char*)LPCTSTR(str);//HS_CONDITION_NUM;
	m_ListCondition.InsertColumn(3, &lvcol);
	str = _T(HS_ALARM_YK);
	lvcol.pszText = (char*)LPCTSTR(str);//HS_ALARM_YK;
	m_ListAlarm.InsertColumn(3, &lvcol);

	lvcol.iSubItem = 4;
	str = _T(HS_CONDITION_ALARM);
	lvcol.pszText = (char*)LPCTSTR(str);//HS_CONDITION_ALARM;
	lvcol.cx = 100;
	m_ListCondition.InsertColumn(4, &lvcol);
}

BOOL CDlgAlarmMan::OnInitDialog()
{
	CDialog::OnInitDialog();
	//预警主窗口
	//::SendMessage(g_hParentWnd, HX_USER_COMPILEDATA, 28, (LPARAM)this);
	Init();
	SetTimer((UINT)this, 500, NULL);

	ListView_SetItemState(m_ListCondition.m_hWnd, 0,
		LVIS_SELECTED|LVIS_FOCUSED,LVIS_SELECTED|LVIS_FOCUSED);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgAlarmMan::ReDrawCondition()
{
	LV_ITEM lvitem;
	CAdvancedAlarmData* pAlarm;
	int iEnd = m_ayAlarmData.GetSize();
	char	chTemp[250];

	BOOL bFalg = GetState();
	
	m_ListCondition.DeleteAllItems();

	for(int i = 0; i < iEnd; i++)
	{
		pAlarm = m_ayAlarmData.GetAt(i);
		lvitem.mask = LVIF_TEXT | LVIF_IMAGE | LVIF_PARAM;
		lvitem.iItem = i;
		if( pAlarm->m_uAlarmType & HS_ALARM_ENABLE )
		{
			lvitem.iImage = 0;
		}
		else
		{
			lvitem.iImage = 1;
		}
		lvitem.iSubItem = 0;
		strcpy(chTemp, pAlarm->m_strName);
		lvitem.pszText = chTemp;
		lvitem.lParam = i;
		m_ListCondition.InsertItem(&lvitem);//返回表项插入后的索引号

		lvitem.mask = LVIF_TEXT;
		lvitem.iSubItem = 1;
		if( pAlarm->m_SingleOrUnion == TRUE && pAlarm->m_pSingleSel != NULL )
		{
			sprintf(chTemp, "%s", pAlarm->m_pSingleSel->m_strExp);
		}
		else if( pAlarm->m_pUnionCondition != NULL )
		{
			sprintf(chTemp, _T("组合条件"));
		}
		else
		{
			sprintf(chTemp, _T("不可知的类型"));
		}

		lvitem.pszText = chTemp;
		m_ListCondition.SetItem(&lvitem);

		lvitem.iSubItem = 2;
		if( pAlarm->m_strAlarmBlock.IsEmpty() )
			sprintf(chTemp, _T("%d支股票"), pAlarm->m_ayStkList.GetSize());
		else
			sprintf(chTemp, "%s", pAlarm->m_strAlarmBlock);
		lvitem.pszText = chTemp;
		m_ListCondition.SetItem(&lvitem);

		lvitem.iSubItem = 3;
		sprintf(chTemp, "%d", pAlarm->m_nCount);
		lvitem.pszText = chTemp;
		m_ListCondition.SetItem(&lvitem);

		lvitem.iSubItem = 4;
		
		if( (pAlarm->m_uAlarmType & HS_ALARM_ON)  && bFalg )
		{
			strcpy(chTemp, _T("预警中..."));
		}
		else
		{
			strcpy(chTemp, _T("预警停止"));
		}
		lvitem.pszText = chTemp;
		m_ListCondition.SetItem(&lvitem);
	}
}

void CDlgAlarmMan::ReDrawAlarm(UINT uMode/* = -1*/)
{
	LV_ITEM lvitem;
	AlarmDisplayData* pData;
	char	chTemp[256];
	int		nBegin;
	CString strTime;

	switch( uMode )
	{
	case 0:			// 表示重画所有
		nBegin = 0;
		m_ListAlarm.DeleteAllItems();
		break;
	case -1:
	default:		// 表示重画追加的一部分
		nBegin = m_ListAlarm.GetItemCount();
		break;
	}

	for( int i = nBegin; i < m_DisplayData.GetSize(); i++ )
	{
		pData = m_DisplayData.GetAt(i);
		lvitem.mask = LVIF_TEXT | LVIF_IMAGE | LVIF_PARAM;
		lvitem.iItem = i;
		if( pData->m_wSytle & Alarm_Sytle_New )
		{
			lvitem.iImage = 3;
		}
		else
		{
			lvitem.iImage = 2;
		}
		lvitem.iSubItem = 0;
		memset( chTemp, 0, sizeof(char) * 256 );
		strncpy(chTemp, pData->m_Stock.m_cCode, sizeof(char) * 6);
		lvitem.pszText = chTemp;
		lvitem.lParam = i;
		m_ListAlarm.InsertItem(&lvitem);

		lvitem.mask = LVIF_TEXT;
		lvitem.iSubItem = 1;
		memset( chTemp, 0, sizeof(char) * 256 );
		sprintf(chTemp, "%s", pData->m_strName);
		lvitem.pszText = chTemp;
		m_ListAlarm.SetItem(&lvitem);

		lvitem.iSubItem = 2;
		memset( chTemp, 0, sizeof(char) * 256 );
		pData->GetStrTime(strTime);
		//pData->GetTime(&nMonth, &nDay, &nHour, &nMin);
		//sprintf(chTemp, "%2d-%2d %2d：%2d", nMonth, nDay, nHour, nMin);
		sprintf(chTemp, "%s", strTime);
		lvitem.pszText = chTemp;
		m_ListAlarm.SetItem(&lvitem);

		lvitem.iSubItem = 3;
		memset( chTemp, 0, sizeof(char) * 256 );
		sprintf(chTemp, "%d", pData->m_nTime);
		lvitem.pszText = chTemp;
		m_ListAlarm.SetItem(&lvitem);
	}
}

BOOL CDlgAlarmMan::ReadAlarmData()
{
	if( _access(m_strAlarmFileName, 0) == -1)
		return FALSE;
	CFile file;
	CAdvancedAlarmData *pAlarm;

	file.Open(m_strAlarmFileName, CFile::modeRead | CFile::shareDenyRead);
	CArchive ar(&file, CArchive::load);

	int nEnd;
	 
	ar >> nEnd;
	for( int i = 0; i < nEnd; i++ )
	{
		pAlarm = new CAdvancedAlarmData;
		pAlarm->Serialize(ar);
		m_ayAlarmData.Add(pAlarm);
	}
	ar.Close();
	file.Close();

	RemoveCalculateData();
	CreateCalculareData();
	return TRUE;
}

BOOL CDlgAlarmMan::WriteAlarmData()
{
	CFile file;
	CAdvancedAlarmData *pAlarm;

	file.Open(m_strAlarmFileName, CFile::modeWrite | CFile::modeCreate);
	CArchive ar(&file, CArchive::store);
	
	ar << m_ayAlarmData.GetSize();
	for( int i = 0; i < m_ayAlarmData.GetSize(); i++ )
	{
		pAlarm = m_ayAlarmData.GetAt(i);
		pAlarm->Serialize(ar);
	}
	ar.Close();
	file.Close();
	return TRUE;
}

void CDlgAlarmMan::OnBnClickedChageCon()
{
	ChageCon();
}

void CDlgAlarmMan::OnNMClickConList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pView = (LPNMLISTVIEW)pNMHDR;

	m_nCurSelCondition = pView->iItem;
	SetConditionButton(TRUE);
	SetAlarmButton(FALSE);
	*pResult = 0;
}

void CDlgAlarmMan::OnBnClickedDelCon()
{
	DelCon();
	SetConditionButton(FALSE);
	SetAlarmButton(FALSE);
}

void CDlgAlarmMan::OnBnClickedDelAlarm()
{
	SetConditionButton(FALSE);
	SetAlarmButton(FALSE);
	DelAlarm(Del_Cur_Alarm);
}

void CDlgAlarmMan::OnBnClickedBeginAlarm()
{
	CString strTemp;

	CWnd *pWnd = GetDlgItem(IDC_BEGIN_ALARM);
	pWnd->GetWindowText(strTemp);
	if( strTemp.Compare(HS_BEGIN_ALARM_CODE) == 0 )
	{
		Start();
	}
	else
	{
		Stop();
	}
	SetConditionButton(FALSE);
	SetAlarmButton(FALSE);
}

void CDlgAlarmMan::OnBnClickedHide()
{
	CWnd* pWnd;
	CString strTemp;

	pWnd = GetDlgItem(IDC_HIDE);
	pWnd->GetWindowText(strTemp);
	if( strTemp.CompareNoCase( HS_SHOW_CONDITION ) == 0)
	{
		pWnd->SetWindowText( HS_HIDE_CONDITION );
		ShowWindowStyle();
	}
	else
	{
		pWnd->SetWindowText( HS_SHOW_CONDITION );
		ShowWindowStyle(FALSE);
	}
}

void CDlgAlarmMan::OnBnClickedCopy()
{
	if( m_nCurSelCondition >= 0 && m_nCurSelCondition < m_ayAlarmData.GetSize() && m_ListCondition.GetSelectedCount() )
	{
		CDlgAlarmParamSet dlg((void*)&m_ayAlarmData, m_nCurSelCondition, m_wStyle, this);
		if(dlg.DoModal() == IDOK )
		{
			if( dlg.m_bCheckAllSet )
			{
				// 重设所有
				m_wStyle |= HS_ALARM_STYLE_SOUNDFILE_SHARE;
				ReSetCalculateCyclyAndSoundFile();
			}
			else
			{
				// 只重设m_nCurSelCondition
				m_wStyle &= ~HS_ALARM_STYLE_SOUNDFILE_SHARE;
				ReSetCalculateCyclyAndSoundFile(m_nCurSelCondition);
			}

			if( dlg.m_wndSaveCheck )
			{
				m_wStyle |= HS_ALARM_STYLE_ALARMINFO_SAVE;
			}
			else
			{
				m_wStyle &= ~(WORD)HS_ALARM_STYLE_ALARMINFO_SAVE;
			}
		}
		m_ListCondition.SetFocus();
		ListView_SetItemState(m_ListCondition.m_hWnd, m_nCurSelCondition,
			LVIS_SELECTED|LVIS_FOCUSED,LVIS_SELECTED|LVIS_FOCUSED);
	}
	else
	{
		MessageBox(_T("请先选择一个预警条件!"), _T("提示..."),MB_ICONINFORMATION);
	}
}

void CDlgAlarmMan::OnNMClickAlarmList(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here
	NMLISTVIEW* pView = (NMLISTVIEW*)pNMHDR;
	m_nCurSelAlarm = pView->iItem;
	SetAlarmButton(TRUE);
	SetConditionButton(FALSE);
	*pResult = 0;
}

BOOL CDlgAlarmMan::Start(int nIndex/* = -1*/)
{
	if( nIndex < -1 && nIndex >= m_CalculateData.GetSize() )
		return FALSE;

	CWnd *pWnd = GetDlgItem(IDC_BEGIN_ALARM);
	pWnd->SetWindowText(_T(HS_END_ALARM_CODE));

	if( nIndex == -1 )
	{
		if( m_CalculateData.GetSize() > 0 )
		{
			ReInitCalData();
			//::SendMessage(g_hParentWnd, HX_USER_COMPILEDATA, 25, (LPARAM)&m_CalculateData);
			StartAlarm();
		}
	}
	else
	{
	}
	m_bAlarmStateFlag = TRUE;
	ReDrawCondition();
	SetButtonState(FALSE);
	return TRUE;
}

BOOL CDlgAlarmMan::Stop(int nIndex/* = -1*/)
{
	if( nIndex < -1 && nIndex >= m_CalculateData.GetSize() )
		return FALSE;

	CWnd *pWnd = GetDlgItem(IDC_BEGIN_ALARM);
	pWnd->SetWindowText(_T(HS_BEGIN_ALARM_CODE));

	if( nIndex == -1 )
	{
		CFormulaCalc::StopAlarm();
		//停止预警声音
		//::SendMessage(g_hParentWnd, HX_USER_COMPILEDATA, 29, 0);
	}
	else
	{
	}
	m_bAlarmStateFlag = FALSE;
	ReSetState(FALSE, -1, TRUE);
	SetButtonState(TRUE);
	return TRUE;
}

BOOL CDlgAlarmMan::RemoveCalculateData(int nIndex/* = -1*/)
{
	if( nIndex != -1 && nIndex >= m_CalculateData.GetSize() )
		return FALSE;

	if( nIndex == -1 )
	{
		for(int i = 0; i < m_CalculateData.GetSize(); i++ )
		{
			delete m_CalculateData.GetAt(i);
		}
		m_CalculateData.RemoveAll();
	}
	else
	{
		delete m_CalculateData.GetAt(nIndex);
		m_CalculateData.RemoveAt(nIndex);
	}
	return  TRUE;
}

BOOL CDlgAlarmMan::CreateCalculareData(int nIndex/* == -1*/)
{
	if( nIndex != -1 && nIndex >= m_ayAlarmData.GetSize() )
		return FALSE;
	
	CString str;
	CAdvancedAlarmData* pAlarmData;
	AlarmCalcData* pCalData;
	SelStockCondition* pSelStockCondition;

	CTime timeDest = CTime::GetCurrentTime();
	str.Format("%04d%02d%02d",timeDest.GetYear(),timeDest.GetMonth(),timeDest.GetDay());
	if( nIndex == -1 )
	{
		for(int i = 0; i < m_ayAlarmData.GetSize(); i++ )
		{
			pCalData = new AlarmCalcData;
			pSelStockCondition = new SelStockCondition;

			m_CalculateData.Add( pCalData ); 
			pCalData->m_pCondition = pSelStockCondition;
			pAlarmData = m_ayAlarmData.GetAt(i);
			if( pAlarmData->m_uAlarmType & HS_ALARM_ENABLE )
			{
				pCalData->m_bFlag = TRUE;
				pAlarmData->m_uAlarmType |= HS_ALARM_ON;
			}
			else
			{
				pCalData->m_bFlag = FALSE;
				pAlarmData->m_uAlarmType &= ~HS_ALARM_ON;
			}
			pCalData->m_ayStkList.Copy(pAlarmData->m_ayStkList);
			pCalData->m_nCycle = pAlarmData->m_nCycle;
			pCalData->m_strSoundFileName = pAlarmData->m_strSoundFileName;

			pSelStockCondition->m_lEndDate = atol(str);  
			pSelStockCondition->m_dStyle = AlarmSystem_Calc;
//			pSelStockCondition->m_ayCheck = m_pHSayCheck;
			pSelStockCondition->m_sSingleSel = pAlarmData->m_pSingleSel;
			pSelStockCondition->m_pHSUnion = pAlarmData->m_pUnionCondition;
			pSelStockCondition->m_strBlock = NULL;
			pSelStockCondition->m_pData = NULL;
		}
	}
	else
	{
	}
	return TRUE;
}

long CDlgAlarmMan::OnFramMsgPress(WPARAM wp,LPARAM lp)
{
	// 接收从付窗口发过来的预警信息并记录
	if( wp == Alarm_Save )
	{
		if( lp == NULL )
			return -1;

		AlarmDisplayData *pAlarmData;
		AlarmDisplayData *pData = ( AlarmDisplayData* )lp;
		CAdvancedAlarmData* pHSAlarmData = m_ayAlarmData.GetAt(pData->m_nIndex);

		if( pHSAlarmData == NULL )
			return -1;
		pAlarmData = new AlarmDisplayData(pData);
		m_DisplayData.Add( pAlarmData );

		pData->m_strName = pHSAlarmData->m_strName;
		pAlarmData->m_strName = pHSAlarmData->m_strName;
		ReDrawAlarm();

		pHSAlarmData->m_nCount++;

		if( !(pHSAlarmData->m_uAlarmType & HS_NoBreak) )
		{
			pHSAlarmData->m_uAlarmType &= ~HS_ALARM_ON;
		}
		ReDrawCondition();

		WORD wStyle = pHSAlarmData->m_uAlarmType;
		if( pData->m_strDesc.IsEmpty() )
		{
			pData->m_strDesc.Format("%s", GetDescribe(pHSAlarmData->m_pUnionCondition));
		}
		return (long)wStyle;
	}
	// 新建条件,并加入股票
	else if( wp == Add_Stock_To_New_Alarm )
	{
		if( lp == NULL )
			return -1;
		StockUserInfo* pData = (StockUserInfo *)lp;
		AddCon(pData);
	}
	// 在条件中加入股票
	else if( wp == Add_Stock_To_Cur_Alarm )
	{
		if( lp == NULL )
			return -1;

		StockUserInfo* pData = (StockUserInfo *)lp;
		CAdvancedAlarmData* pAlarmData = m_ayAlarmData.GetAt(m_nCurSelCondition);
		if( AddStock(&pAlarmData->m_ayStkList, pData) )
		{
			BOOL bOffFlg = GetState();
			if( bOffFlg )
			{
				//停止预警
				CFormulaCalc::StopAlarm();
			}

			AlarmCalcData* pCalData = m_CalculateData.GetAt(m_nCurSelCondition);
			pCalData->m_ayStkList.Add(pData);
			
			if( bOffFlg )// 如果开着, 则重启预警
			{
				StartAlarm();
			}
			return 0;
		}
		else
		{
			return 1;
		}
		//ChageCon(pData);
	}
	// 得到当前所选条件索引
	else if( wp == Add_Stock_To_Alarm_CurSel )
	{
		m_nCurSelCondition = (int)lp;
	}
	// 加入预警条件时,名字是否重复
	else if( wp == Check_Alarm_Name )
	{
		if( lp == NULL )
			return -1;
		CString* pData = (CString*)lp;

		if( pData->IsEmpty() )
			return -1;

		CAdvancedAlarmData* pAlarmData;
		for(int i = 0; i < m_ayAlarmData.GetSize(); i++ )
		{
			pAlarmData = m_ayAlarmData.GetAt(i);
			if(pData->CompareNoCase(pAlarmData->m_strName) == 0 )
			{
				return 1;
			}
		}
		return 0;
	}
	// 显示窗口
	else if( wp == Show_Alarm_Window )
	{
		if( lp == 0 )
		{
			ShowWindow(SW_SHOWNORMAL);
		}
	}
	// 开始预警
	else if( wp == Begin_Alarm_Msg )
	{
		OnAlarm1Begin();
	}
	// 显示预警分析
	else if( wp == Show_Alarm_Analyse )
	{
		if( lp == NULL )
			return 0;

		CString* szTemp = (CString*)lp;
		CDlgAlarmAnalysis WranAnalyseDlg((void*)&m_DisplayData, CWnd::FromHandle(g_hParentWnd), szTemp->GetBuffer());
		szTemp->ReleaseBuffer();
		WranAnalyseDlg.DoModal();
	}
	// 打开某个条件
	else if( wp == Flag_Alarm_Bebin ) 
	{
		if( lp == NULL )
			return 0;

		CString strTemp;
		strTemp.Format("%s", *((CString*)lp));

		int nCurSel = GetIdxByName(strTemp);
		if( nCurSel == -1 )
			return 0;

		return ReSetState(TRUE, nCurSel);
	}
	// 停止某个条件
	else if( wp == Flag_Alarm_End )
	{
		if( lp == NULL )
			return 0;

		CString strTemp;
		strTemp.Format("%s", *((CString*)lp));

		int nCurSel = GetIdxByName(strTemp);
		if( nCurSel == -1 )
			return 0;

		return ReSetState(FALSE, nCurSel);
	}


	return 1;
}

CString CDlgAlarmMan::GetDescribe(UnionSelCondition* pHSUnion)
{
	if( pHSUnion == NULL ) return "";

	UnionSelConditionSub* pSelSub;
	SingleSelCondition*   pUnionData;
	int i,j;

	CString strRet,strTmp;
	for( i = 0; i < pHSUnion->m_aySub->GetSize(); i++ )
	{
		pSelSub = pHSUnion->m_aySub->GetAt(i);
		
		strTmp.Format("%i.",i+1);
		strRet += strTmp;
		strRet += pSelSub->m_strName;
		strRet += "\r\n";

		for( j = 0; j < pSelSub->m_aySub->GetSize(); j++ )
		{
			pUnionData = pSelSub->m_aySub->GetAt(j);

			CString* strCurRet = NULL;
			switch( pUnionData->m_nType )      // 公式类型
			{
			case Tech:
				strCurRet = &CTreeCtrlFormula::g_strTech;
				break;
			case Condition:
				strCurRet = &CTreeCtrlFormula::g_strCondition;
				break;
			case Exchange:
				strCurRet = &CTreeCtrlFormula::g_strExchange;
				break;
			case MoreKLine:
				strCurRet = &CTreeCtrlFormula::g_strColorKLine;
				break;
			case Strategy: //20090302 YJT 增加 Strategy
				strCurRet = &CTreeCtrlFormula::g_strStrategy;
				break;
			}
			
			strTmp.Format("%i.%i",i+1,j+1);
			strRet += strTmp;
			strRet += pUnionData->GetDescribe(strCurRet);
			strRet += "\r\n";
		}
	}
	return strRet;
}

void CDlgAlarmMan::OnTimer(UINT nIDEvent)
{
	if( nIDEvent == (UINT)this && m_CalculateData.GetSize() > 0 && this->IsWindowVisible() )
	{
		CString strText;
		AlarmCalcData *pData = m_CalculateData.GetAt(0);
		if( pData != NULL )
		{
			strText.Format(_T("预警管理 - 共进行%d次计算"), pData->m_nCalculateCount);
			SetWindowText(strText);
		}
	}
	CDialog::OnTimer(nIDEvent);
}

// 跟据条件名得到条件索引
int CDlgAlarmMan::GetIdxByName(CString strName)
{
	CAdvancedAlarmData* pTempData;

	for(int i = 0; i < m_ayAlarmData.GetSize(); i++ )
	{
		pTempData = m_ayAlarmData.GetAt(i);
		if( pTempData->m_strName.CompareNoCase(strName) == 0 )
		{
			return i;
		}
	}

	return -1;
}

BOOL CDlgAlarmMan::ReadAlarmInfo()
{
	if( _access(m_strAlarmInfoFileName, 0) == -1)
		return FALSE;
	CFile file(m_strAlarmInfoFileName, CFile::modeRead);
	CArchive ar(&file, CArchive::load);
	AlarmDisplayData* pData;
	int	nCount;

	// 清除原来
	int i;
	for(  i = 0; i < m_DisplayData.GetSize(); i++ )
	{
		delete m_DisplayData.GetAt(i);
	}
	m_DisplayData.RemoveAll();

	ar >> m_wStyle;
	if( m_wStyle & HS_ALARM_STYLE_ALARMINFO_SAVE )
	{
		ar >> nCount;
		for( i = 0; i < nCount; i++ )
		{
			pData = new AlarmDisplayData;
			pData->Serialize(ar);
			m_DisplayData.Add(pData);
		}
	}
	ar.Close();
	file.Close();
	ReDrawAlarm(0);// 重画所有
	return TRUE;
}

BOOL CDlgAlarmMan::WriteAlarmInfo()
{
	CFile file(m_strAlarmInfoFileName, CFile::modeWrite | CFile::modeCreate);
	CArchive ar(&file, CArchive::store);

	ar << m_wStyle;
	if( m_wStyle & HS_ALARM_STYLE_ALARMINFO_SAVE )
	{
		AlarmDisplayData* pData;

		ar << m_DisplayData.GetSize();
		for( int i = 0; i < m_DisplayData.GetSize(); i++ )
		{
			pData = m_DisplayData.GetAt(i);
			pData->Serialize(ar);
		}
		ar.Close();
		file.Close();
		return FALSE;
	}
	else
	{
		ar.Close();
		file.Close();
		return FALSE;
	}
}

void CDlgAlarmMan::OnNMRclickConList(NMHDR *pNMHDR, LRESULT *pResult)
{
	*pResult = 0;

	NMLISTVIEW* pView = (NMLISTVIEW *)pNMHDR;

	m_nCurSelCondition = pView->iItem;
	if( pView->iItem >= m_ListCondition.GetItemCount() || pView->iItem < 0 )
	{
		return;
	}
	else
	{
		CMenu MainMenu;
		MainMenu.LoadMenu(IDR_ALARMMENU);
		CMenu* pSubMenu;
		pSubMenu = MainMenu.GetSubMenu(0);

		if( GetState() )//如果正在运行
		{
			pSubMenu->EnableMenuItem(ID_ALARM1_BEGIN, MF_DISABLED | MF_GRAYED);
			pSubMenu->EnableMenuItem(ID_ALARM1_BEGIN_ALL, MF_DISABLED | MF_GRAYED);

			pSubMenu->EnableMenuItem(ID_ALARM1_END_ALL, MF_DISABLED | MF_GRAYED);
			pSubMenu->EnableMenuItem(ID_ALARM1_BEGIN_CUR, MF_DISABLED | MF_GRAYED);
			pSubMenu->EnableMenuItem(ID_ALARM1_END_CUR, MF_DISABLED | MF_GRAYED);
			pSubMenu->EnableMenuItem(ID_ALARM1_CHG_CON, MF_DISABLED | MF_GRAYED);
			pSubMenu->EnableMenuItem(ID_ALARM1_ADD_CON, MF_DISABLED | MF_GRAYED);
			pSubMenu->EnableMenuItem(ID_ALARM1_DEL_CON, MF_DISABLED | MF_GRAYED);
		}
		else
		{
			pSubMenu->EnableMenuItem(ID_ALARM1_END, MF_DISABLED | MF_GRAYED);
		}

		CPoint point;
		GetCursorPos(&point);
		pSubMenu->TrackPopupMenu(TPM_LEFTBUTTON|TPM_LEFTALIGN, point.x, point.y, this, NULL);
	}
	return;
}

BOOL CDlgAlarmMan::ReSetState(BOOL bFlag/* = TRUE*/, int nIndex/* = -1*/, BOOL bOnlyOnOff/* = FALSE*/)
{
	if( m_ayAlarmData.GetSize() <= nIndex || nIndex < -1 )
	{
		return FALSE;
	}

	BOOL bOffFlg = GetState();
	// 如果开着,则先关闭,做好处理后再重启预警
	if( bOffFlg )
	{
		CFormulaCalc::StopAlarm();
	}

	CAdvancedAlarmData* pAlarmData;
	AlarmCalcData* pCalData;

	if( nIndex == -1 )// 全部
	{
		for( int i = 0; i < m_ayAlarmData.GetSize(); i++ )
		{
			pAlarmData = m_ayAlarmData.GetAt(i);
			pCalData = m_CalculateData.GetAt(i);
			if( bFlag )
			{
				if( !bOnlyOnOff )
					pAlarmData->m_uAlarmType |= HS_ALARM_ENABLE;
				pAlarmData->m_uAlarmType |= HS_ALARM_ON;
			}
			else
			{
				if( !bOnlyOnOff )
					pAlarmData->m_uAlarmType &= ~(WORD)HS_ALARM_ENABLE;
				pAlarmData->m_uAlarmType &= ~(WORD)HS_ALARM_ON;
			}

			pCalData->m_bFlag = bFlag;
		}
	}
	else
	{
		pAlarmData = m_ayAlarmData.GetAt(nIndex);
		pCalData = m_CalculateData.GetAt(nIndex);
		if( bFlag )
		{
			if( !bOnlyOnOff )
				pAlarmData->m_uAlarmType |= HS_ALARM_ENABLE;
			pAlarmData->m_uAlarmType |= HS_ALARM_ON;
		}
		else
		{
			if( !bOnlyOnOff )
				pAlarmData->m_uAlarmType &= ~(WORD)HS_ALARM_ENABLE;
			pAlarmData->m_uAlarmType &= ~(WORD)HS_ALARM_ON;
		}

		pCalData->m_bFlag = bFlag;
	}

	if( bOffFlg )// 如果开着, 则重启预警
	{
		StartAlarm();
	}
	ReDrawCondition();
	return TRUE;
}

void CDlgAlarmMan::ShowWindowStyle(BOOL bFlag/* = TRUE*/)
{
	CWnd* pWnd;
	CRect rc1, rc2;

	pWnd = GetDlgItem(IDC_CON_LIST);
	pWnd->ShowWindow( bFlag );
	pWnd->GetWindowRect(&rc1);
	ScreenToClient(&rc1);

	pWnd = GetDlgItem(IDC_ADD_CON);
	pWnd->ShowWindow( bFlag );
	pWnd = GetDlgItem(IDC_DEL_CON);
	pWnd->ShowWindow( bFlag );
	pWnd = GetDlgItem(IDC_CHAGE_CON);
	pWnd->ShowWindow( bFlag );
	pWnd = GetDlgItem(IDC_ALARM_LIST);

	pWnd->GetWindowRect(&rc2);
	ScreenToClient(&rc2);
	if( bFlag == TRUE )
	{
		pWnd->SetWindowPos(0, rc1.left, rc1.bottom + 8, rc2.Width(), rc2.Height() - 8 - rc1.Height(), SWP_NOZORDER);
	}
	else
	{
		pWnd->SetWindowPos(0, rc1.left, rc1.top, rc2.Width(), rc2.Height() + 8 + rc1.Height(), SWP_NOZORDER);
	}
}

// 打开所有预警标志
void CDlgAlarmMan::OnAlarm1BeginAll()
{
	ReSetState();
}

// 关闭所有预警标志
void CDlgAlarmMan::OnAlarm1EndAll()
{
	ReSetState(FALSE);
}

// 打开当前预警标志
void CDlgAlarmMan::OnAlarm1BeginCur()
{
	ReSetState(TRUE, m_nCurSelCondition);
}

// 关闭当前预警标志
void CDlgAlarmMan::OnAlarm1EndCur()
{
	ReSetState(FALSE, m_nCurSelCondition);
}

void CDlgAlarmMan::OnAlarm1Exit()
{
	OnCancel();
}

void CDlgAlarmMan::OnAlarm1ChgCon()
{
	ChageCon();
}

void CDlgAlarmMan::OnAlarm1AddCon()
{
	AddCon();
}

void CDlgAlarmMan::OnAlarm1DelCon()
{
	DelCon();
}

void CDlgAlarmMan::AddCon(StockUserInfo* pData/* = NULL*/)
{
	CAdvancedAlarmData* pAlarm;
	pAlarm = new CAdvancedAlarmData;
	CDlgAlarmSet pWranSetDlg(this/*CWnd::FromHandle(g_hParentWnd)*/, pAlarm, pData, this->m_hWnd);
	pWranSetDlg.m_bChageOrBuild = FALSE;

	if( pWranSetDlg.DoModal() == IDOK )
	{
		BOOL bOffFlg = GetState();
		// 如果开着,则先关闭,做好处理后再重启预警
		if( bOffFlg )
		{
			
		}

		m_ayAlarmData.Add(pAlarm);
		RemoveCalculateData();
		CreateCalculareData();
		ReDrawCondition();
		if( this->IsWindowVisible() )
		{
			this->SetFocus();
			ListView_SetItemState(m_ListCondition.m_hWnd,m_ListCondition.GetItemCount() - 1,
				LVIS_SELECTED|LVIS_FOCUSED,LVIS_SELECTED|LVIS_FOCUSED);
		}
	}
	else
	{
		delete pAlarm;
		if( this->IsWindowVisible() )
		{
			this->SetFocus();
			int nIndex;
			if( m_nCurSelCondition == -1 || m_nCurSelCondition >= m_ListCondition.GetItemCount() )
				nIndex = 0;
			else
				nIndex = m_nCurSelCondition;

			if( m_ListCondition.GetItemCount() )
			{
				ListView_SetItemState(m_ListCondition.m_hWnd, nIndex,
					LVIS_SELECTED|LVIS_FOCUSED,LVIS_SELECTED|LVIS_FOCUSED);
			}
		}
	}
	SetConditionButton(TRUE);
}

void CDlgAlarmMan::DelCon()
{
	if( m_ayAlarmData.GetSize() > 0 && m_nCurSelCondition >= 0 && m_nCurSelCondition < m_ayAlarmData.GetSize() )
	{
		BOOL bOffFlg = GetState();
		// 如果开着,则先关闭,做好处理后再重启预警
		if( bOffFlg )
		{
			CFormulaCalc::StopAlarm();
		}
		delete m_ayAlarmData.GetAt(m_nCurSelCondition);
		m_ayAlarmData.RemoveAt(m_nCurSelCondition);
		RemoveCalculateData();
		CreateCalculareData();
		ReDrawCondition();
		if( bOffFlg )// 如果开着, 则重启预警
		{
			//::SendMessage(g_hParentWnd, HX_USER_COMPILEDATA, 25, (LPARAM)&m_CalculateData);
			StartAlarm();
		}
		if( m_nCurSelCondition < 0 || m_nCurSelCondition >= m_ListCondition.GetItemCount() )
			m_nCurSelCondition = 0;

		if( m_ListCondition.GetItemCount() )
		{
			m_ListCondition.SetFocus();
			ListView_SetItemState(m_ListCondition.m_hWnd, m_nCurSelCondition,
				LVIS_SELECTED|LVIS_FOCUSED,LVIS_SELECTED|LVIS_FOCUSED);
		}
	}
	SetAlarmButton(TRUE);
	SetConditionButton(TRUE);
}

void CDlgAlarmMan::ChageCon(StockUserInfo* pData/* = NULL*/)
{
	if( m_nCurSelCondition < 0 || m_nCurSelCondition >= m_ayAlarmData.GetSize() || m_bAlarmStateFlag)
		return;

	CAdvancedAlarmData* pAlarm;
	pAlarm = m_ayAlarmData.GetAt(m_nCurSelCondition);
	CDlgAlarmSet pWranSetDlg(this/*CWnd::FromHandle(g_hParentWnd)*/, pAlarm, pData, this->m_hWnd);
	pWranSetDlg.m_bChageOrBuild = TRUE;

	if( pWranSetDlg.DoModal() == IDOK )
	{
		BOOL bOffFlg = GetState();
		// 如果开着,则先关闭,做好处理后再重启预警
		if( bOffFlg )
		{
			CFormulaCalc::StopAlarm();
		}
		RemoveCalculateData();
		CreateCalculareData();
		ReDrawCondition();
		if( bOffFlg )// 如果开着, 则重启预警
		{
			StartAlarm();
		}
	}
	this->SetFocus();

	ListView_SetItemState(m_ListCondition.m_hWnd, m_nCurSelCondition,
		LVIS_SELECTED|LVIS_FOCUSED,LVIS_SELECTED|LVIS_FOCUSED);

	SetConditionButton(TRUE);
}

BOOL CDlgAlarmMan::GetState()
{
	return m_bAlarmStateFlag;
}

void CDlgAlarmMan::OnAlarm1Begin()
{
	ReSetState(TRUE, m_nCurSelCondition);
	Start();
}

void CDlgAlarmMan::OnAlarm1End()
{
	Stop();
}

BOOL CDlgAlarmMan::ReSetCalculateCyclyAndSoundFile(int nIndex/* = -1*/)
{
	CAdvancedAlarmData* pAlarmData;
	AlarmCalcData* pCalData;

	if( nIndex < -1 && nIndex >= m_ayAlarmData.GetSize() )
		return FALSE;

	if( nIndex == -1 )
	{
		for( int i = 0; i < m_ayAlarmData.GetSize(); i++ )
		{
			pAlarmData = m_ayAlarmData.GetAt(i);
			pCalData = m_CalculateData.GetAt(i);
			pCalData->m_nCycle = pAlarmData->m_nCycle;
			pCalData->m_strSoundFileName = pAlarmData->m_strSoundFileName;
		}
	}
	else
	{
		pAlarmData = m_ayAlarmData.GetAt(nIndex);
		pCalData = m_CalculateData.GetAt(nIndex);
		pCalData->m_nCycle = pAlarmData->m_nCycle;
		pCalData->m_strSoundFileName = pAlarmData->m_strSoundFileName;
	}
	return TRUE;
}

void CDlgAlarmMan::OnNMDblclkConList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pView = (LPNMLISTVIEW)(pNMHDR);
	m_nCurSelCondition = pView->iItem;
	ChageCon();
	*pResult = 0;
}

void CDlgAlarmMan::OnAlarm2Exit()
{
	OnCancel();
}

void CDlgAlarmMan::OnAlarm2DelAll()
{
	DelAlarm(Del_All_Alarm);
}

void CDlgAlarmMan::OnAlarm2DelCur()
{
	DelAlarm(Del_Cur_Alarm);
}

void CDlgAlarmMan::OnNMRclickAlarmList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pView = (LPNMLISTVIEW)pNMHDR;
	m_nCurSelAlarm = pView->iItem;
	//SetDisplayDataOld(m_nCurSelAlarm);
	//ReDrawAlarm(0);
	if( m_nCurSelAlarm < 0 || m_nCurSelAlarm >= m_DisplayData.GetSize() )
		return;
	CMenu MainMenu;
	MainMenu.LoadMenu(IDR_ALARMMENU);
	CMenu* pSubMenu;
	pSubMenu = MainMenu.GetSubMenu(1);
	CPoint point;
	GetCursorPos(&point);
	pSubMenu->TrackPopupMenu(TPM_LEFTBUTTON|TPM_LEFTALIGN, point.x, point.y, this, NULL);
	*pResult = 0;
}

void CDlgAlarmMan::DelAlarm(int nIndex/* = Del_Cur_Alarm */)
{
	if( m_DisplayData.GetSize() <= 0 )
		return;

	AlarmDisplayData* pData;
	if( nIndex == Del_Cur_Alarm )
	{
		if(  m_nCurSelAlarm >= 0 && m_nCurSelAlarm < m_DisplayData.GetSize() )
		{
			pData =  m_DisplayData.GetAt(m_nCurSelAlarm);
			pData->m_pData = NULL;
			delete pData;
			m_DisplayData.RemoveAt(m_nCurSelAlarm);
			m_ListAlarm.DeleteItem(m_nCurSelAlarm);
		}
	}
	else if( nIndex == Del_All_Alarm )
	{
		for( int i = 0; i < m_DisplayData.GetSize(); i++ )
		{
			pData =  m_DisplayData.GetAt(i);
			pData->m_pData = NULL;
			delete pData;
		}
		m_DisplayData.RemoveAll();
		m_ListAlarm.DeleteAllItems();
	}
}

void CDlgAlarmMan::ReInitCalData()
{
	AlarmCalcData* pCalData;
	CAdvancedAlarmData* pAlarmData;

	for( int i = 0; i < m_CalculateData.GetSize(); i++ )
	{
		pAlarmData = m_ayAlarmData.GetAt(i);
		pCalData = m_CalculateData.GetAt(i);
		if( pAlarmData->m_uAlarmType & HS_ALARM_ENABLE )
		{
			pCalData->m_bFlag = TRUE;
			pAlarmData->m_uAlarmType |= HS_ALARM_ON;
		}
		else
		{
			pCalData->m_bFlag = FALSE;
			pAlarmData->m_uAlarmType &= ~HS_ALARM_ON;
		}
	}
}

BOOL CDlgAlarmMan::SetButtonState(BOOL bFalg)
{
	CWnd* pWnd = GetDlgItem(IDC_COPY);
	pWnd->EnableWindow(bFalg);
	return TRUE;
}

void CDlgAlarmMan::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);
	if( bShow == SW_HIDE )
	{
		SetDisplayDataOld();
	}
	else 
	{
		ReDrawCondition();
		ReDrawAlarm(0);
		if( m_ListCondition.GetItemCount())
		{
			this->SetFocus();
			ListView_SetItemState(m_ListCondition.m_hWnd, 0,
				LVIS_SELECTED|LVIS_FOCUSED,LVIS_SELECTED|LVIS_FOCUSED);
		}
	}
}

int	CDlgAlarmMan::SetDisplayDataOld(int nIndex/* = -1*/)
{
	if( nIndex < -1 && nIndex >= m_DisplayData.GetSize() )
		return -1;

	AlarmDisplayData* pData;
	int	nCount = 0;
	if( nIndex == -1 )
	{
		for( int i = 0; i < m_DisplayData.GetSize(); i++ )
		{
			pData = m_DisplayData.GetAt(i);
			if( pData->m_wSytle & Alarm_Sytle_New )
			{
				pData->SetStyle(~Alarm_Sytle_New, FALSE);
				nCount++;
			}
		}
	}
	else
	{
		pData = m_DisplayData.GetAt(nIndex);
		if( pData->m_wSytle & Alarm_Sytle_New )
		{
			pData->SetStyle(~Alarm_Sytle_New, FALSE);
			nCount++;
		}
	}
	return nCount;
}

void CDlgAlarmMan::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	CDialog::OnActivate(nState, pWndOther, bMinimized);
}

BOOL CDlgAlarmMan::AddStock(CArray<StockUserInfo*, StockUserInfo*>* pAyStock, StockUserInfo* pData)
{
	if( pAyStock == NULL || pData == NULL )
		return FALSE;

	StockUserInfo* pTemp;

	for(int i = 0; i < pAyStock->GetSize(); i++ )
	{
		pTemp = pAyStock->GetAt(i);
		if( !memcmp(pTemp, pData, sizeof(*pTemp) ) )
		{
			return FALSE;
		}
	}
	pAyStock->Add(pData);

	return TRUE;
}
void CDlgAlarmMan::OnNMDblclkAlarmList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pView = (LPNMLISTVIEW)pNMHDR;
	m_nCurSelAlarm = pView->iItem;
	if( m_nCurSelAlarm < 0 || m_nCurSelAlarm >= m_DisplayData.GetSize() )
		return;

	SetConditionButton(FALSE);
	SetAlarmButton(TRUE);
	HSCurKeyInfo keyInfo;
	AlarmDisplayData* pData = m_DisplayData.GetAt(m_nCurSelAlarm);

	keyInfo.Copy(&pData->m_Stock);
	::SendMessage(g_hParentWnd,HX_USER_DRAGDATA,
							HX_USER_DRAGDATA,(LPARAM)&keyInfo);
	*pResult = 0;
}

void CDlgAlarmMan::OnNMKillfocusConList(NMHDR *pNMHDR, LRESULT *pResult)
{
	*pResult = 0;
}

void CDlgAlarmMan::OnNMSetfocusConList(NMHDR *pNMHDR, LRESULT *pResult)
{
	SetConditionButton(TRUE);
	SetAlarmButton(FALSE);
	*pResult = 0;
}

void CDlgAlarmMan::OnNMKillfocusAlarmList(NMHDR *pNMHDR, LRESULT *pResult)
{
	*pResult = 0;
}

void CDlgAlarmMan::OnNMSetfocusAlarmList(NMHDR *pNMHDR, LRESULT *pResult)
{
	SetAlarmButton(TRUE);
	SetConditionButton(FALSE);
	*pResult = 0;
}

void CDlgAlarmMan::SetAlarmButton(BOOL bFlag)
{
	CWnd* pWnd = GetDlgItem(IDC_DEL_ALARM);
	if(/* m_ListAlarm.GetItemCount() > 0 && */m_ListAlarm.GetSelectedCount() > 0 )
	{
		pWnd->EnableWindow(bFlag);
	}
	else
	{
		pWnd->EnableWindow(FALSE);
	}
}

void CDlgAlarmMan::SetConditionButton(BOOL bFlag)
{
	CWnd* pWnd;
	int nCount = m_ListCondition.GetSelectedCount();
	if( !GetState() && (nCount > 0) )
	{
		pWnd = GetDlgItem(IDC_DEL_CON);
		pWnd->EnableWindow(bFlag);
		pWnd = GetDlgItem(IDC_CHAGE_CON);
		pWnd->EnableWindow(bFlag);
	}
	else
	{
		pWnd = GetDlgItem(IDC_DEL_CON);
		pWnd->EnableWindow(FALSE);
		pWnd = GetDlgItem(IDC_CHAGE_CON);
		pWnd->EnableWindow(FALSE);
	}
}
void  CDlgAlarmMan::StartAlarm()
{
	CFormulaCalc::m_pAlarmCondition = &m_CalculateData;			              // 条件
	CFormulaCalc::m_pExternExpression = CExpression::m_pExternExpression;   // 外部表达式表
	CFormulaCalc::m_pExternVariabile = CExpression::m_pExternVariabile;	     // 外部变量表
	CFormulaCalc::m_pExternFunction = CExpression::m_pExternFunction;		 // 外部函数表
	CFormulaCalc::m_pSystemFun = g_pSystemFun;				                 // 函数指针

	CFormulaCalc::m_pAlarmWnd = this;
	CFormulaCalc::StartAlarm();
}