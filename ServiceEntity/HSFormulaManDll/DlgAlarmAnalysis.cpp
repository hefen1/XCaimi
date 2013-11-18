// DlgAlarmAnalysis.cpp : implementation file
//

#include "stdafx.h"
#include "DlgAlarmAnalysis.h"


// CDlgAlarmAnalysis dialog

IMPLEMENT_DYNAMIC(CDlgAlarmAnalysis, CDialog)
CDlgAlarmAnalysis::CDlgAlarmAnalysis(void *pData/* = NULL*/, CWnd* pParent /*=NULL*/, LPCSTR szAlarmName)
	: CDialog(CDlgAlarmAnalysis::IDD, pParent)
{
	m_pDisplayData = (CArray<AlarmDisplayData*, AlarmDisplayData*>*) pData;
	if( szAlarmName == NULL )
	{
		m_strCondition = HS_ALL_CONDITION;
	}
	else
	{
		m_strCondition.Format("%s", szAlarmName);
	}
	m_stkCode = HS_ALL_STOCK;
	m_nOldBeginTime = m_nOldEndTime = -1;
}

CDlgAlarmAnalysis::~CDlgAlarmAnalysis()
{
}

void CDlgAlarmAnalysis::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST, m_wndConditionList);
	DDX_Control(pDX, IDC_CON_LIST, m_wndAlarmCondition);
	DDX_Control(pDX, IDC_STK_LIST, m_wndListStk);
	DDX_Control(pDX, IDC_BEGIN_TIME, m_wndBeginTime);
	DDX_Control(pDX, IDC_END_TIME, m_wndEndTime);
}


BEGIN_MESSAGE_MAP(CDlgAlarmAnalysis, CDialog)
	ON_CBN_SELCHANGE(IDC_CON_LIST, OnCbnSelchangeConList)
	ON_CBN_SELCHANGE(IDC_STK_LIST, OnCbnSelchangeStkList)
	ON_BN_CLICKED(IDC_COPY, OnBnClickedCopy)
	ON_BN_CLICKED(IDC_ANALYSE, OnBnClickedAnalyse)
	ON_NOTIFY(DTN_CLOSEUP, IDC_BEGIN_TIME, OnDtnCloseupBeginTime)
	ON_BN_CLICKED(IDC_OUTPUT, OnBnClickedOutput)
	ON_NOTIFY(DTN_CLOSEUP, IDC_END_TIME, OnDtnCloseupEndTime)
END_MESSAGE_MAP()

void CDlgAlarmAnalysis::OnCancel()
{
	CDialog::OnCancel();
}

void CDlgAlarmAnalysis::InitList()
{
	LV_COLUMN	lvcol;
	lvcol.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
	lvcol.fmt = LVCFMT_CENTER;//居中
	lvcol.cx = 70;

	m_wndConditionList.DeleteAllItems();
	while( m_wndConditionList.DeleteColumn(0) != 0 );

	lvcol.iSubItem = 0;
	CString str;
	str = _T(HS_ALARM_ALALYSE_STK);
	lvcol.pszText = (char*)LPCTSTR(str);//HS_ALARM_ALALYSE_STK;
	m_wndConditionList.InsertColumn(0, &lvcol);

	lvcol.iSubItem = 1;
	str = _T(HS_ALARM_ALALYSE_NAME);
	lvcol.pszText = (char*)LPCTSTR(str);//HS_ALARM_ALALYSE_NAME;
	m_wndConditionList.InsertColumn(1, &lvcol);

	lvcol.iSubItem = 2;
	lvcol.cx = 140;
	str = _T(HS_ALARM_ALALYSE_TIME);
	lvcol.pszText = (char*)LPCTSTR(str);//HS_ALARM_ALALYSE_TIME;
	m_wndConditionList.InsertColumn(2, &lvcol);

	lvcol.iSubItem = 3;
	lvcol.cx = 70;
	str = _T(HS_ALARM_ALALYSE_DATA2);
	lvcol.pszText = (char*)LPCTSTR(str);//HS_ALARM_ALALYSE_DATA2;
	m_wndConditionList.InsertColumn(3, &lvcol);

	lvcol.iSubItem = 4;
	str = _T(HS_ALARM_ALALYSE_DATA3);
	lvcol.pszText = (char*)LPCTSTR(str);//HS_ALARM_ALALYSE_DATA3;
	m_wndConditionList.InsertColumn(4, &lvcol);

	lvcol.iSubItem = 5;
	str = _T(HS_ALARM_ALALYSE_DATA4);
	lvcol.pszText = (char*)LPCTSTR(str);//HS_ALARM_ALALYSE_DATA4;
	m_wndConditionList.InsertColumn(5, &lvcol);

	lvcol.iSubItem = 6;
	str = _T(HS_ALARM_ALALYSE_DESC);
	lvcol.pszText = (char*)LPCTSTR(str);//HS_ALARM_ALALYSE_DESC;
	lvcol.cx = 280;
	m_wndConditionList.InsertColumn(6, &lvcol);
}

void CDlgAlarmAnalysis::ReDrawListData(CString strName/* = HS_ALL_CONDITION*/, 
										CString strCode/* = HS_ALL_STOCK*/,
										int nBeginTime/* = -1*/, 
										int nEndTime/* = -1*/)
{
	if( m_pDisplayData == NULL )
		return;

	LV_ITEM lvitem;
	AlarmDisplayData* pData;
	CString strTemp;
	char	chTemp[1024];

	m_wndConditionList.DeleteAllItems();
	m_ayDiaplayIndex.RemoveAll();

	for( int i = 0; i < m_pDisplayData->GetSize(); i++ )
	{
		pData = m_pDisplayData->GetAt(i);

		if( strName.CompareNoCase(HS_ALL_CONDITION) != 0 && 
			pData->m_strName.CompareNoCase(strName) != 0 )
			continue;
		if( strCode.CompareNoCase(HS_ALL_STOCK) != 0 &&
			strCode.CompareNoCase(pData->m_Stock.GetCode()) != 0 )
			continue;
		if( nBeginTime != -1 && pData->m_nTime < nBeginTime )
			continue;
		if( nEndTime != -1 && pData->m_nTime > nEndTime )
			continue;

		// 加入显示索引，供输出用
		m_ayDiaplayIndex.Add(i);

		lvitem.mask = LVIF_TEXT | LVIF_IMAGE | LVIF_PARAM;
		lvitem.iItem = m_ayDiaplayIndex.GetSize() - 1;
		lvitem.iSubItem = 0;
		memset( chTemp, 0, sizeof(char) * 1024 );
		strncpy(chTemp, pData->m_Stock.m_cCode, sizeof(char) * 6);
		lvitem.pszText = chTemp;
		lvitem.lParam = i;
		m_wndConditionList.InsertItem(&lvitem);

		lvitem.mask = LVIF_TEXT;
		lvitem.iSubItem = 1;
		memset( chTemp, 0, sizeof(char) * 1024 );
		sprintf(chTemp, "%s", pData->m_strName);
		lvitem.pszText = chTemp;
		m_wndConditionList.SetItem(&lvitem);

		lvitem.mask = LVIF_TEXT;
		lvitem.iSubItem = 2;
		memset( chTemp, 0, sizeof(char) * 1024 );
		pData->GetStrTime(strTemp);
		sprintf(chTemp, "%s", strTemp);
		lvitem.pszText = chTemp;
		m_wndConditionList.SetItem(&lvitem);

		lvitem.iSubItem = 3;
		memset( chTemp, 0, sizeof(char) * 1024 );
		sprintf(chTemp, "%s", pData->m_strDesc);
		for( int j = 0; j < pData->m_strDesc.GetLength(); j++ )
		{
			if( chTemp[j] == '\n' )
				chTemp[j] = ';';
			else if( chTemp[j] == '\r' )
				chTemp[j] = ' ';
		}
		lvitem.pszText = chTemp;
		m_wndConditionList.SetItem(&lvitem);
	}
}

void CDlgAlarmAnalysis::ReDrawListBox()
{
	m_wndAlarmCondition.ResetContent();
	m_wndListStk.ResetContent();
	m_wndAlarmCondition.AddString(HS_ALL_CONDITION);
	m_wndListStk.AddString(HS_ALL_STOCK);

	int nEnd = m_pDisplayData->GetSize();
	AlarmDisplayData* pAlarmData;
	CString strTemp;

	for( int i = 0; i < nEnd; i++ )
	{
		pAlarmData = m_pDisplayData->GetAt(i);
		if( LB_ERR == m_wndAlarmCondition.FindString(0, pAlarmData->m_strName) )
			m_wndAlarmCondition.AddString(pAlarmData->m_strName);
		strTemp = pAlarmData->m_Stock.GetCode();
		if( LB_ERR == m_wndListStk.FindString(0, strTemp) )
			m_wndListStk.AddString(strTemp);
	}
	m_wndAlarmCondition.SelectString(0, HS_ALL_CONDITION);
	m_wndListStk.SelectString(0, HS_ALL_STOCK);
}

BOOL CDlgAlarmAnalysis::OnInitDialog()
{
	CDialog::OnInitDialog();

	const CTime tMin(1978,1,1,0,0,0);
	const CTime tMax(2038,1,1,0,0,0);

	m_wndBeginTime.SetFormat(_T("yyyy年MM月dd日 dddd"));
	m_wndBeginTime.SetRange(&tMin, &tMax);

	m_wndEndTime.SetFormat(_T("yyyy年MM月dd日 dddd"));
	m_wndEndTime.SetRange(&tMin, &tMax);

	InitList();
	ReDrawListData();
	ReDrawListBox();

	GetSetTime(&m_nOldBeginTime, &m_nOldEndTime);

	return TRUE;  // return TRUE unless you set the focus to a control
}

void CDlgAlarmAnalysis::OnCbnSelchangeConList()
{
	GetBoxText(&m_wndAlarmCondition, m_strCondition);
	ReDrawListData(m_strCondition, m_stkCode);
}

void CDlgAlarmAnalysis::OnCbnSelchangeStkList()
{
	GetBoxText(&m_wndListStk, m_stkCode);
	ReDrawListData(m_strCondition, m_stkCode);
}

void CDlgAlarmAnalysis::OnBnClickedCopy()
{
}

void CDlgAlarmAnalysis::OnBnClickedAnalyse()
{
}

void CDlgAlarmAnalysis::OnDtnCloseupBeginTime(NMHDR *pNMHDR, LRESULT *pResult)
{
	int nBegin, nEnd;
	GetSetTime(&nBegin, &nEnd);
	if( m_nOldEndTime < 0 || m_nOldBeginTime < 0 )
	{
		m_nOldBeginTime = nBegin;
		m_nOldEndTime = nEnd;
	}

	if( nBegin > nEnd )
	{
		SetTime(m_nOldBeginTime, m_nOldEndTime);
	}
	else
	{
		m_nOldBeginTime = nBegin;
		m_nOldEndTime = nEnd;
	}

	ReDrawListData(m_strCondition, m_stkCode, m_nOldBeginTime, m_nOldEndTime);
	*pResult = 0;
}

BOOL CDlgAlarmAnalysis::GetBoxText(CComboBox* pt, CString& strTemp)
{
	if( pt == NULL || pt->GetCurSel() < 0 || pt->GetCurSel() >= pt->GetCount() )
		return FALSE;

	pt->GetLBText(pt->GetCurSel(), strTemp);
	return TRUE;
}

void CDlgAlarmAnalysis::OnBnClickedOutput()
{
	CFileDialog dlg(FALSE, "txt", "alarmamalyse.txt", 
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, 
		_T("文本文件(*.txt)|*.txt|所有文件|*.*||"), this);
	if( dlg.DoModal() == IDOK )
	{
		CString strFileName;
		strFileName = dlg.GetPathName();
		CFile file;
		int nCount;
		file.Open(strFileName, CFile::modeWrite | CFile::modeCreate);
		AlarmDisplayData* pData;
		strFileName.Format("%s\t%s\t%s\t%s\t%s\t%s\t%s\r\n",
							HS_ALARM_ALALYSE_STK,
							HS_ALARM_ALALYSE_NAME,	
							HS_ALARM_ALALYSE_TIME,	
							HS_ALARM_ALALYSE_DATA2,	
							HS_ALARM_ALALYSE_DATA3,	
							HS_ALARM_ALALYSE_DATA4,
							HS_ALARM_ALALYSE_DESC);

		file.Write(strFileName, strFileName.GetLength());

		CString strTime;

		for( int i = 0; i < m_ayDiaplayIndex.GetSize(); i++ )
		{
			pData = m_pDisplayData->GetAt(m_ayDiaplayIndex.GetAt(i));
			nCount = pData->m_ayOtherData.GetSize();
			pData->GetStrTime(strTime);
			switch( nCount )
			{
			case 0 :
			default:
				{
					strFileName.Format("%s\t\t%s\t%s\t\t\t\t\t\t\t\t%s\r\n", 
						pData->m_Stock.GetCode(), pData->m_strName, strTime, 
						pData->m_strDesc);
				}
				break;
			case 1 :
				{
					strFileName.Format("%s\t\t%s\t%s\t\t%.2f\t\t\t\t\t\t%s\r\n", 
						pData->m_Stock.GetCode(), pData->m_strName, strTime, 
						pData->m_ayOtherData.GetAt(0), 
						pData->m_strDesc);
				}
				break;
			case 2 :
				{
					strFileName.Format("%s\t\t%s\t%s\t\t%.2f\t\t%.2f\t\t\t\t%s\r\n", 
						pData->m_Stock.GetCode(), pData->m_strName,  strTime,
						pData->m_ayOtherData.GetAt(0), 
						pData->m_ayOtherData.GetAt(1), 
						pData->m_strDesc);
				}
				break;
			case 3 :
				{
					strFileName.Format("%s\t\t%s\t%s\t\t%.2f\t\t%.2f\t\t%.2f\t\t%s\r\n", 
						pData->m_Stock.GetCode(), pData->m_strName, strTime, 
						pData->m_ayOtherData.GetAt(0), 
						pData->m_ayOtherData.GetAt(1), 
						pData->m_ayOtherData.GetAt(2), 
						pData->m_strDesc);
				}
				break;
			}
			file.Write(strFileName, strFileName.GetLength());
		}
		file.Close();
	}
}

void CDlgAlarmAnalysis::GetSetTime(int* nBegin, int* nEnd)
{
	CTime cTime;
	m_wndBeginTime.GetTime(cTime);
	*nBegin = 1000000 * cTime.GetMonth() + 10000 * cTime.GetDay() + 100 * 0 + 0;
	m_wndEndTime.GetTime(cTime);
	*nEnd = 1000000 * cTime.GetMonth() + 10000 * cTime.GetDay() + 100 * 23 + 59;
}

void CDlgAlarmAnalysis::SetTime(int nBegin, int nEnd)
{
	
	CTime cTime;
	cTime = CTime::GetCurrentTime();
	int nYear = cTime.GetYear();
	int nMon = nBegin / 1000000;
	int nDay = nBegin / 10000 % 100;
	int nHour = nBegin / 100 % 100;
	int nMin = nBegin % 100;

	CTime cTemp1(nYear, nMon, nDay, nHour, nMin, 0);
	m_wndBeginTime.SetTime(&cTemp1);

	nMon = nEnd / 1000000;
	nDay = nEnd / 10000 % 100;
	nHour = nEnd / 100 % 100;
	nMin = nEnd % 100;
	
	CTime cTemp2(nYear, nMon, nDay, nHour, nMin, 0);
	m_wndEndTime.SetTime(&cTemp2);
}

void CDlgAlarmAnalysis::OnDtnCloseupEndTime(NMHDR *pNMHDR, LRESULT *pResult)
{
	int nBegin, nEnd;
	GetSetTime(&nBegin, &nEnd);
	CTime curTime;
	curTime = CTime::GetCurrentTime();
	int nCurTime = 1000000 * curTime.GetMonth() + 10000 * curTime.GetDay() + 100 * 23 + 59;

	if( m_nOldEndTime < 0 || m_nOldBeginTime < 0 )
	{
		m_nOldBeginTime = nBegin;
		m_nOldEndTime = nEnd;
	}

	if( nBegin >= nEnd || nCurTime < nEnd )
	{
		SetTime(m_nOldBeginTime, m_nOldEndTime);
	}
	else
	{
		m_nOldBeginTime = nBegin;
		m_nOldEndTime = nEnd;
	}

	ReDrawListData(m_strCondition, m_stkCode, m_nOldBeginTime, m_nOldEndTime);
	*pResult = 0;
}
