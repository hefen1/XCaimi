// DlgExchParamSet.cpp : implementation file
//

#include "stdafx.h"
#include "DlgExchParamSet.h"


#define EXCH_PARAM_CALC_MAX_NUM 10000
// CDlgExchParamSet dialog

IMPLEMENT_DYNAMIC(CDlgExchParamSet, CDialog)
CDlgExchParamSet::CDlgExchParamSet(void* pData, CWnd* pParent /*=NULL*/)
	: CDialog(CDlgExchParamSet::IDD, pParent)
{
	SetOptimizeData((TExchOptimize*)pData);
	m_bHaveChange = FALSE;
}

CDlgExchParamSet::~CDlgExchParamSet()
{
}

void CDlgExchParamSet::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHECK1, m_wndCheckParam1);
	DDX_Control(pDX, IDC_CHECK2, m_wndCheckParam2);
	DDX_Control(pDX, IDC_CHECK3, m_wndCheckParam3);
	DDX_Control(pDX, IDC_CHECK4, m_wndCheckParam4);
}


BEGIN_MESSAGE_MAP(CDlgExchParamSet, CDialog)
	ON_CONTROL_RANGE(EN_CHANGE, IDC_EDIT_EXCH_PARAMSETBEGIN+11, IDC_EDIT_EXCH_PARAMSETBEGIN+43, OnChangeEdit)
	ON_BN_CLICKED(IDC_CHECK1, OnBnClickedCheck1)
	ON_BN_CLICKED(IDC_CHECK2, OnBnClickedCheck2)
	ON_BN_CLICKED(IDC_CHECK3, OnBnClickedCheck3)
	ON_BN_CLICKED(IDC_CHECK4, OnBnClickedCheck4)
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
	ON_BN_CLICKED(IDCANCEL2, OnBnClickedCancel2)
END_MESSAGE_MAP()


// CDlgExchParamSet message handlers

BOOL CDlgExchParamSet::OnInitDialog()
{
	CDialog::OnInitDialog();

	PlayData();

	return TRUE;
}

void CDlgExchParamSet::OnChangeEdit(UINT uID)
{
	if( !m_localData.IsValid() )
		return;
	m_bHaveChange = TRUE;

	int iLine = uID - IDC_EDIT_EXCH_PARAMSETBEGIN;
	iLine /= 10;
	if( iLine < 1 || iLine > GetMaxLine() )
		return;

	GetLineData(iLine-1);

	ShowTotal();
}

void CDlgExchParamSet::ShowTotal()
{
	CString str;
	str.Format(_T("总共要进行%d次计算"), m_localData.GetCaclUionCount());
	CWnd* pWnd = GetDlgItem(IDC_TOTAL);
	if( pWnd != NULL )
	{
		pWnd->SetWindowText(str);
	}
}

// 显示几行设计数据，最大四行
void CDlgExchParamSet::ShowLine(int nLineNum/* = 0*/)
{
	UINT uID;
	int iLineMax = GetMaxLine();

	if( nLineNum > 0 )
	{
		m_wndCheckParam1.ShowWindow(SW_SHOW);
	}
	else
	{
		m_wndCheckParam1.ShowWindow(SW_HIDE);
	}

	if( nLineNum > 1 )
	{
		m_wndCheckParam2.ShowWindow(SW_SHOW);
	}
	else
	{
		m_wndCheckParam2.ShowWindow(SW_HIDE);
	}

	if( nLineNum > 2 )
	{
		m_wndCheckParam3.ShowWindow(SW_SHOW);
	}
	else
	{
		m_wndCheckParam3.ShowWindow(SW_HIDE);
	}

	if( nLineNum > 3 )
	{
		m_wndCheckParam4.ShowWindow(SW_SHOW);
	}
	else
	{
		m_wndCheckParam4.ShowWindow(SW_HIDE);
	}

	for( int i = 0; i < iLineMax; i++ )
	{
		for( int j = 1; j < 4; j++ )
		{
			uID = IDC_EDIT_EXCH_PARAMSETBEGIN + (i+1)*10 + j;

			CWnd* pWnd = GetDlgItem(uID);
			if( pWnd != NULL )
			{
				if( i < nLineNum )
				{
					pWnd->ShowWindow(SW_SHOW);
				}
				else
				{
					pWnd->ShowWindow(SW_HIDE);
				}
			}

			uID = IDC_STATIC_EXCH_PARAMSETBEGIN + + (i+1)*10 + j;
			pWnd = GetDlgItem(uID);
			if( pWnd != NULL )
			{
				if( i < nLineNum )
				{
					pWnd->ShowWindow(SW_SHOW);
				}
				else
				{
					pWnd->ShowWindow(SW_HIDE);
				}
			}
		}
	}
}

// 设置一行数据
void CDlgExchParamSet::SetLineData(int nLineIdx, CString strParamName, double dMin, double dMax, double dAdd)
{
	CButton *pBut = NULL;
	switch( nLineIdx )
	{
	case 0:
		pBut = &m_wndCheckParam1;
		break;
	case 1:
		pBut = &m_wndCheckParam2;
		break;
	case 2:
		pBut = &m_wndCheckParam3;
		break;
	case 3:
		pBut = &m_wndCheckParam4;
		break;
	case 4:
		break;
	}

	if( pBut == NULL )
		return;

	pBut->SetWindowText(strParamName);

	UINT uID = IDC_EDIT_EXCH_PARAMSETBEGIN + (nLineIdx+1)*10 + 1;
	CWnd* pWnd = GetDlgItem(uID++);
	CString str;
	if( pWnd != NULL )
	{
		str.Format("%.2f", dMin);
		str.TrimRight('0');
		str.TrimRight('.');
		pWnd->SetWindowText(str);
	}
	pWnd = GetDlgItem(uID++);
	if( pWnd != NULL )
	{
		str.Format("%.2f", dMax);
		str.TrimRight('0');
		str.TrimRight('.');
		pWnd->SetWindowText(str);
	}
	pWnd = GetDlgItem(uID++);
	if( pWnd != NULL )
	{
		if( fabs(dAdd) <= 0.001 )
		{
			dAdd = (long)(0.5 + fabs(dMax - dMin)/20);
		}
		str.Format("%.2f", dAdd);
		str.TrimRight('0');
		str.TrimRight('.');
		pWnd->SetWindowText(str);
	}
}

// 得到一行数据
BOOL CDlgExchParamSet::GetLineData(int nLineIdx, TExchOptimizeItem* pItem)
{
	if( nLineIdx >= GetMaxLine() )
		return FALSE;

	if( pItem == NULL )
	{
		if( m_localData.lValidLen <= nLineIdx 
			|| !m_localData.IsValid() )
			return false;

		pItem = &m_localData.pItem[nLineIdx];
	}

	if( pItem == NULL )
		return false;

	UINT uID = IDC_EDIT_EXCH_PARAMSETBEGIN + (nLineIdx+1)*10 + 1;
	CWnd* pWnd = GetDlgItem(uID++);
	CString str;
	if( pWnd != NULL )
	{
		pWnd->GetWindowText(str);
		pItem->m_dMin = atof(str);
	}
	pWnd = GetDlgItem(uID++);
	if( pWnd != NULL )
	{
		pWnd->GetWindowText(str);
		pItem->m_dMax = atof(str);
	}
	pWnd = GetDlgItem(uID++);
	if( pWnd != NULL )
	{
		pWnd->GetWindowText(str);
		pItem->m_dAdd = atof(str);
	}

	return TRUE;
}

// 使一行有效或无效
void CDlgExchParamSet::EnableLine(int nLineIdx, BOOL bValid /*= TRUE*/)
{
	if( nLineIdx >= GetMaxLine()
		|| !m_localData.IsValid() || nLineIdx >= m_localData.lValidLen )
		return;
	UINT uID = IDC_EDIT_EXCH_PARAMSETBEGIN + (nLineIdx+1)*10 + 1;
	UINT uStaticID = IDC_STATIC_EXCH_PARAMSETBEGIN + (nLineIdx+1)*10 + 1;
	CWnd* pWnd;

	for( int i = 0; i < 3; i++ )
	{
		pWnd = GetDlgItem(uID++);
		if( pWnd != NULL )
		{
			pWnd->EnableWindow(bValid);
		}
		pWnd = GetDlgItem(uStaticID++);
		if( pWnd != NULL )
		{
			pWnd->EnableWindow(bValid);
		}
	}

	m_localData.pIsValid[nLineIdx] = (bValid) ? 'y' : 'n';

	ShowTotal();
}

// 显示数据
BOOL CDlgExchParamSet::PlayData()
{
	if( m_localData.IsValid() != TRUE )
	{
		ShowLine(0);
		CWnd* pWnd = GetDlgItem(IDC_TOTAL);
		if( pWnd != NULL )
		{
			pWnd->SetWindowText(_T("无参数，无法进行优化设置!"));
		}
		return false;
	}

	ShowLine(m_localData.lValidLen);
	for(int i = 0; i < m_localData.lValidLen, i < GetMaxLine(); i++ )
	{
		TExchOptimizeItem* pItem;

		pItem = &m_localData.pItem[i];
		if( pItem == NULL )
			return false;

		SetLineData(i, pItem->GetName(), pItem->m_dMin, pItem->m_dMax, pItem->m_dAdd);
		EnableLine(i, m_localData.IsValidItem(i));
	}

	if( m_localData.IsValidItem(0) )
	{
		m_wndCheckParam1.SetCheck(1);
	}
	else
	{
		m_wndCheckParam1.SetCheck(0);
	}

	if( m_localData.IsValidItem(1) )
	{
		m_wndCheckParam2.SetCheck(1);
	}
	else
	{
		m_wndCheckParam2.SetCheck(0);
	}

	if( m_localData.IsValidItem(2) )
	{
		m_wndCheckParam3.SetCheck(1);
	}
	else
	{
		m_wndCheckParam3.SetCheck(0);
	}

	if( m_localData.IsValidItem(3) )
	{
		m_wndCheckParam4.SetCheck(1);
	}
	else
	{
		m_wndCheckParam4.SetCheck(0);
	}

	return true;
}

// 得到数据
BOOL CDlgExchParamSet::SaveData()
{
	if( m_localData.IsValid() != TRUE )
		return false;

	for(int i = 0; i < m_localData.lValidLen, i < GetMaxLine(); i++ )
	{
		TExchOptimizeItem* pItem;

		pItem = &m_localData.pItem[i];
		if( pItem == NULL || !GetLineData(i, pItem) )
			return false;
	}

	return true;
}

void CDlgExchParamSet::OnBnClickedCheck1()
{
	m_bHaveChange = TRUE;
	if( m_wndCheckParam1.GetCheck() )
	{
		EnableLine(0);
	}
	else
	{
		EnableLine(0, FALSE);
	}
}

void CDlgExchParamSet::OnBnClickedCheck2()
{
	m_bHaveChange = TRUE;
	if( m_wndCheckParam2.GetCheck() )
	{
		EnableLine(1);
	}
	else
	{
		EnableLine(1, FALSE);
	}
}

void CDlgExchParamSet::OnBnClickedCheck3()
{
	m_bHaveChange = TRUE;
	if( m_wndCheckParam3.GetCheck() )
	{
		EnableLine(2);
	}
	else
	{
		EnableLine(2, FALSE);
	}
}

void CDlgExchParamSet::OnBnClickedCheck4()
{
	m_bHaveChange = TRUE;
	if( m_wndCheckParam4.GetCheck() )
	{
		EnableLine(3);
	}
	else
	{
		EnableLine(3, FALSE);
	}
}

void CDlgExchParamSet::OnBnClickedCancel()
{
	OnCancel();
}

void CDlgExchParamSet::OnBnClickedCancel2()
{
	if( m_localData.GetCaclUionCount() >= EXCH_PARAM_CALC_MAX_NUM )
	{
		CString strTemp;
// 		strTemp.Format(FORMULAMAN_EXCH_OPTIMIZE_NUM, EXCH_PARAM_CALC_MAX_NUM);
// 		MessageBox(strTemp, FORMULAMAN_EXCH_PROMPT, MB_OK);
		return;
	}

	if( m_pData != NULL )
	{
		*m_pData = m_localData;
	}
	OnOK();
}
