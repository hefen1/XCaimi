// DlgAlarmParamSet.cpp : implementation file
//

#include "stdafx.h"
#include "DlgAlarmParamSet.h"


// CDlgAlarmParamSet dialog

IMPLEMENT_DYNAMIC(CDlgAlarmParamSet, CDialog)
CDlgAlarmParamSet::CDlgAlarmParamSet(void* pData/* = NULL*/, int nMode/* = 0*/, WORD wStyle /* = 0*/, CWnd* pParent /*=NULL*/)
	: CDialog(CDlgAlarmParamSet::IDD, pParent)
	, m_bCheckAllSet(FALSE)
	, m_wndSaveCheck(FALSE)
{
	if( nMode == -1 )
	{
		m_pSingulAlarmData = (CAdvancedAlarmData*)pData;
		m_pAyAlarmData = NULL;
	}
	else
	{
		m_pSingulAlarmData = NULL;
		m_pAyAlarmData = (CArray<CAdvancedAlarmData*, CAdvancedAlarmData*>*)pData;
	}
	m_nCurSelAlarmData = nMode;
	m_wndSaveCheck = wStyle & HS_ALARM_STYLE_ALARMINFO_SAVE;
	m_bCheckAllSet = wStyle & HS_ALARM_STYLE_SOUNDFILE_SHARE;
}

CDlgAlarmParamSet::~CDlgAlarmParamSet()
{
}

void CDlgAlarmParamSet::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_CHECK1, m_bCheckAllSet);
	DDX_Check(pDX, IDC_CHECK2, m_wndSaveCheck);
}


BEGIN_MESSAGE_MAP(CDlgAlarmParamSet, CDialog)
	ON_BN_CLICKED(IDC_CHECK1, OnBnClickedCheck1)
	ON_BN_CLICKED(IDC_BUTTON1, OnBnClickedButton1)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN1, OnDeltaposSpin1)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDC_CHECK2, OnBnClickedCheck2)
END_MESSAGE_MAP()

void CDlgAlarmParamSet::Init()
{
	if( m_pSingulAlarmData )
	{
		m_nCycle = m_pSingulAlarmData->m_nCycle;
		m_strSoundFileName = m_pSingulAlarmData->m_strSoundFileName;
		m_bCheck = FALSE;
	}

	if( m_pAyAlarmData )
	{
		if( m_pAyAlarmData->GetSize() <= m_nCurSelAlarmData )
		{
			return;
		}
		CAdvancedAlarmData* pData = m_pAyAlarmData->GetAt(m_nCurSelAlarmData);
		m_nCycle = pData->m_nCycle;
		m_strSoundFileName = pData->m_strSoundFileName;
		m_bCheck = TRUE;
	}
	
	// 显示或隐藏--声音文件共享
	CWnd* pWnd = GetDlgItem( IDC_CHECK1 );
	pWnd->EnableWindow( m_bCheck );
	if( m_bCheck )
	{
		((CButton*)pWnd)->SetCheck(m_bCheckAllSet);
	}

	pWnd = GetDlgItem(IDC_CHECK2 );
	pWnd->EnableWindow( m_bCheck );
	if( m_bCheck )
	{
		((CButton*)pWnd)->SetCheck(m_wndSaveCheck);
	}

	pWnd = GetDlgItem( IDC_EDIT1 );
	pWnd->SetWindowText( m_strSoundFileName );

	CString strTemp;
	strTemp.Format("%d", m_nCycle);
	pWnd = GetDlgItem( IDC_EDIT2 );
	pWnd->SetWindowText( strTemp );
}

void CDlgAlarmParamSet::OnBnClickedCheck1()
{
}

void CDlgAlarmParamSet::OnBnClickedButton1()
{
	CFileDialog dlg(TRUE, "wav", NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T("声音文件(*.wav)|*.wav|所有文件|*.*||"));
	if( dlg.DoModal() == IDOK )
	{
		m_strSoundFileName = dlg.GetFileName();
		CWnd* pWnd = GetDlgItem( IDC_EDIT1 );
		pWnd->SetWindowText( m_strSoundFileName );
	}
}

void CDlgAlarmParamSet::OnDeltaposSpin1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);

	m_nCycle -= pNMUpDown->iDelta;

	if( m_nCycle > HS_MAX_CYCLE )
	{
		m_nCycle = HS_MIN_CYCLE;
	}

	if( m_nCycle < HS_MIN_CYCLE )
	{
		m_nCycle = HS_MAX_CYCLE;
	}
	CString strTemp;
	strTemp.Format("%d", m_nCycle);
	CWnd* pWnd = GetDlgItem( IDC_EDIT2 );
	pWnd->SetWindowText( strTemp );
	*pResult = 0;
}

void CDlgAlarmParamSet::OnBnClickedOk()
{
	CWnd* pWnd = GetDlgItem( IDC_EDIT1 );
	pWnd->GetWindowText( m_strSoundFileName );
	CString strTemp;
	pWnd = GetDlgItem( IDC_EDIT2 );
	pWnd->GetWindowText( strTemp );
	m_nCycle = atol( strTemp );
	
	CButton* pButtonWnd = (CButton*)GetDlgItem( IDC_CHECK1 );
	m_bCheckAllSet = pButtonWnd->GetCheck();

	pButtonWnd = (CButton*)GetDlgItem( IDC_CHECK2 );
	m_wndSaveCheck = pButtonWnd->GetCheck();


	if( pButtonWnd->IsWindowEnabled() && m_pAyAlarmData )
	{
		// 是否设置所有
		CAdvancedAlarmData* pData;
		if( m_bCheckAllSet )
		{
			for( int i = 0; i < m_pAyAlarmData->GetSize(); i++ )
			{
				pData = m_pAyAlarmData->GetAt(i);
				pData->m_nCycle = m_nCycle;
				pData->m_strSoundFileName = m_strSoundFileName;
			}
		}
		else
		{
			if( m_nCurSelAlarmData >= 0 && m_nCurSelAlarmData < m_pAyAlarmData->GetSize() )
			{
				pData = m_pAyAlarmData->GetAt( m_nCurSelAlarmData );
				pData->m_nCycle = m_nCycle;
				pData->m_strSoundFileName = m_strSoundFileName;
			}
		}
	}
	else if( m_pSingulAlarmData )
	{
		m_pSingulAlarmData->m_nCycle = m_nCycle;
		m_pSingulAlarmData->m_strSoundFileName = m_strSoundFileName;
	}

	OnOK();
}

BOOL CDlgAlarmParamSet::OnInitDialog() 
{
	CDialog::OnInitDialog();
	Init();
	return TRUE;
}
void CDlgAlarmParamSet::OnBnClickedCheck2()
{
}
