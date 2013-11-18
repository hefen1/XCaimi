// DlgSetStockAndTime.cpp : implementation file
//

#include "stdafx.h"
#include "DlgSetStockAndTime.h"
#include "PubStruct.h"
#include "Express.h"

#include <MMSystem.h>
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgSetStockAndTime dialog


extern HWND g_hParentWnd;

CDlgSetStockAndTime::CDlgSetStockAndTime(CWnd* pParent /*=NULL*/,
				CArray<StockArea*,StockArea*>* ayCheck /*= NULL*/,
				SelStockCondition* curSelStockCondition /*= NULL*/)
	: CDialog(CDlgSetStockAndTime::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgSetStockAndTime)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_ayCheck = ayCheck;
	m_curSelStockCondition = curSelStockCondition;
}


void CDlgSetStockAndTime::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgSetStockAndTime)
	DDX_Control(pDX, IDD_BEGINDATE, m_wndBeginDate);
	DDX_Control(pDX, IDD_ENDDATE, m_wndEndDate);
	DDX_Control(pDX, IDD_INCLUDE, m_wndIncludeStock);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgSetStockAndTime, CDialog)
	//{{AFX_MSG_MAP(CDlgSetStockAndTime)
	ON_BN_CLICKED(IDD_CHANGEIN, OnChangein)
	ON_BN_CLICKED(IDC_HISDATECHECK, OnHisdatecheck)
	ON_BN_CLICKED(IDC_UNIONCHECK, OnUnioncheck)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_CHECK_CHUQUAN, OnCheckChuquan)

	ON_REGISTERED_MESSAGE(HX_USER_COMPILEDATA, OnExchangeData)

END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgSetStockAndTime message handlers

void CDlgSetStockAndTime::OnCancel() 
{
	OnOK();
}

void CDlgSetStockAndTime::OnOK() 
{
	// TODO: Add extra validation here
	
	if( m_curSelStockCondition != NULL )
	{
		// 日期范围
		
		// 计算终止日期
		CString str;
		CTime timeEnd;
		if (m_curSelStockCondition->m_dStyle & SelectStock_HisDate)
		{
			m_wndEndDate.GetTime( timeEnd );
			str.Format("%04d%02d%02d",timeEnd.GetYear(),timeEnd.GetMonth(),timeEnd.GetDay());
		}
		else//yulx add 20091207
		{
			SYSTEMTIME   tm;
			GetLocalTime(&tm);
			str.Format("%04d%02d%02d",tm.wYear,tm.wMonth,tm.wDay);
		}
		
		m_curSelStockCondition->m_lEndDate = atol(str);   

		// 计算日期长度
		if( m_curSelStockCondition->m_dStyle & SelectStock_HisDate )
		{
			CTime timeBegin;
			m_wndBeginDate.GetTime( timeBegin );
			CTimeSpan nSpan = timeEnd - timeBegin;

			if( (nSpan.GetDays() + 1) <= 0 )
			{
				//AfxMessageBox(CDlgSetStockAndTime_Begin_End_Date_Error);
				MessageBox(CDlgSetStockAndTime_Begin_End_Date_Error,_T(" 提示 "), MB_OK);
				m_wndBeginDate.SetFocus();
				return;
			}

			str.Format("%04d%02d%02d",timeBegin.GetYear(),timeBegin.GetMonth(),timeBegin.GetDay());
			m_curSelStockCondition->m_lDateCount = atol(str);//nSpan.GetDays();
		}
		else
		{
			m_curSelStockCondition->m_lDateCount = 0;
		}
	}

	// save - begin
	CWinApp* pApp = AfxGetApp();

	// 时间
	CString str;
	if( m_curSelStockCondition )
	{
		str.Format("%i;%i",m_curSelStockCondition->m_lDateCount,m_curSelStockCondition->m_lEndDate);
	}
	pApp->WriteProfileString(CDlgSetStockAndTime_Section,
		CDlgSetStockAndTime_Time,str);

	// 股票范围
	str = "";
	CString strTemp;
	StockArea* pCheckData;
	for( int i = 0; m_ayCheck && i < m_ayCheck->GetSize(); i++ )
	{
		pCheckData = m_ayCheck->GetAt(i);
		strTemp.Format("%i,%s",pCheckData->lData,pCheckData->strText);

		str += strTemp;
		str += ";";
	}
	pApp->WriteProfileString(CDlgSetStockAndTime_Section,
		CDlgSetStockAndTime_Stock,str);

	// 选项
	if( m_curSelStockCondition )
	{
		pApp->WriteProfileInt(CDlgSetStockAndTime_Section,
			CDlgSetStockAndTime_Option,m_curSelStockCondition->m_dStyle);
	}
	// save - end

	CDialog::OnOK();
}

void CDlgSetStockAndTime::OnChangein() 
{
	// TODO: Add your control notification handler code here
	::SendMessage(g_hParentWnd,HX_USER_COMPILEDATA,2,(LPARAM)this);	
}

BOOL CDlgSetStockAndTime::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	//设定时间控件的范围以及显示格式
	const CTime tMin(1978,1,1,0,0,0);
	const CTime tMax(2038,1,1,0,0,0);
	m_wndBeginDate.SetFormat(_T("yyyy年MM月dd日 dddd"));
	m_wndEndDate.SetFormat(_T("yyyy年MM月dd日 dddd"));
	m_wndEndDate.SetRange(&tMin, &tMax);

	if( m_curSelStockCondition != NULL )
	{
		// 日期范围
		try{
			// 终止日期
			if( m_curSelStockCondition->m_lEndDate != 0 )
			{
				CTime timeDest(m_curSelStockCondition->m_lEndDate/10000,
					m_curSelStockCondition->m_lEndDate%10000/100,
					m_curSelStockCondition->m_lEndDate%10000%100,0,0,0);
				m_wndEndDate.SetTime( &timeDest );
			}
			else
			{
				SYSTEMTIME   tm;
				GetLocalTime(&tm);
				m_wndEndDate.SetTime( &tm );
			}
			// 开始日期
			if( m_curSelStockCondition->m_lDateCount != 0 )
			{
				CTime timeDest(m_curSelStockCondition->m_lDateCount/10000,
					m_curSelStockCondition->m_lDateCount%10000/100,
					m_curSelStockCondition->m_lDateCount%10000%100,0,0,0);
				m_wndBeginDate.SetTime( &timeDest );
			}
			else
			{
// 				CTime timeDest(m_curSelStockCondition->m_lEndDate/10000,
// 					m_curSelStockCondition->m_lEndDate%10000/100,
// 					m_curSelStockCondition->m_lEndDate%10000%100,0,0,0);
				SYSTEMTIME   tm;
				GetLocalTime(&tm);
				m_wndBeginDate.SetTime( &tm );
			}
		}
		catch(...)
		{
			OutputDebugStr("时间设置错误！");
		}
		/*CTime timeEnd;
		m_wndEndDate.GetTime( timeEnd );
		timeEnd -= CTimeSpan( m_curSelStockCondition->m_lDateCount, 0, 0, 0 );
		m_wndBeginDate.SetTime( &timeEnd );*/

		CButton* pButton = (CButton*)this->GetDlgItem(IDC_HISDATECHECK);
		if( pButton != NULL )
		{
			pButton->SetCheck( (m_curSelStockCondition->m_dStyle & SelectStock_HisDate)?1:0 );

			m_wndBeginDate.EnableWindow((m_curSelStockCondition->m_dStyle & SelectStock_HisDate)?1:0);
			m_wndEndDate.EnableWindow((m_curSelStockCondition->m_dStyle & SelectStock_HisDate)?1:0);

		}
		pButton = (CButton*)this->GetDlgItem(IDC_UNIONCHECK);
		if( pButton != NULL )
		{
			pButton->SetCheck( (m_curSelStockCondition->m_dStyle & SelectStock_InK_Disp_Mark)?1:0 );
		}

		pButton = (CButton*)this->GetDlgItem(IDC_CHECK_CHUQUAN);
		if( pButton != NULL )
		{
			pButton->SetCheck( 1/*(m_curSelStockCondition->m_dStyle & SelectStock_ChuQuan)?1:0*/ );
		}		
	}

	// 缺省选股范围	
	if( m_ayCheck != NULL && m_ayCheck->GetSize() > 0 )
	{
		SetIncludeStockList(m_ayCheck);
	}
	else
	{
		m_wndIncludeStock.SetWindowText(_T("上海Ａ股,深圳Ａ股"));
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CDlgSetStockAndTime::ShowWindow( int nCmdShow )
{

	if( !::IsWindow(this->m_hWnd) )
		return 0;

	if( nCmdShow == SW_HIDE || nCmdShow == SW_MINIMIZE )
		return CDialog::ShowWindow(nCmdShow);


	if( IsIconic() )
	{
		return CDialog::ShowWindow(SW_RESTORE);
	}

	if(  IsZoomed() ) // max
		return CDialog::ShowWindow(nCmdShow);

	if( ::AnimateWindow(m_hWnd,100,AW_CENTER | AW_ACTIVATE) )
	{
		this->Invalidate();
	}
	else
	{
		return CDialog::ShowWindow(nCmdShow);
	}

	return 1;
}
LRESULT CDlgSetStockAndTime::OnExchangeData(WPARAM wp,LPARAM lp)
{
	if( m_ayCheck == NULL )
		return 0;

	if( wp == 1 ) // 股票列表
	{
		for(int i = 0; i < m_ayCheck->GetSize(); i++)
		{
			delete m_ayCheck->GetAt(i);
		}
		m_ayCheck->RemoveAll();
		
		SetIncludeStockList( ((CArray<StockArea*,StockArea*>*)lp),TRUE );

	}
	else if (wp == 3) // 返回当前选中股票
	{
		return (long)m_ayCheck;
	}
	return 0;
}

void CDlgSetStockAndTime::SetIncludeStockList(CArray<StockArea*,StockArea*>* pCheck,BOOL bAdd /*= FALSE*/)
{
	if( pCheck == NULL )
	{
		return;
	}

	CString strInclude;	
	StockArea* pData;
	StockArea* pCurData;
	int nCount = pCheck->GetSize();

	for(int i = 0; i < nCount; i++)
	{
		pCurData = pCheck->GetAt(i);
		
		if( bAdd )
		{
			pData = new StockArea;
			pData->strText.Format("%s",pCurData->strText);
			pData->lData = pCurData->lData;
			m_ayCheck->Add(pData);
			strInclude += pData->strText;
		}
		else
		{
			strInclude += pCurData->strText;
		}
				
		if( i != nCount - 1 )
		{
			strInclude += ",";
		}				
	}
	m_wndIncludeStock.SetWindowText(strInclude);
}

void CDlgSetStockAndTime::OnCheckChuquan() 
{
	// TODO: Add your control notification handler code here
	CButton* pButton = (CButton*)this->GetDlgItem(IDC_CHECK_CHUQUAN);
	if( pButton != NULL )
	{
		if( pButton->GetCheck() ) 
		{
			m_curSelStockCondition->m_dStyle |= SelectStock_ChuQuan;
		}
		else
		{
			m_curSelStockCondition->m_dStyle &= (~SelectStock_ChuQuan);
		}
	}
}

void CDlgSetStockAndTime::OnHisdatecheck() 
{
	// TODO: Add your control notification handler code here
	CButton* pButton = (CButton*)this->GetDlgItem(IDC_HISDATECHECK);
	if( pButton != NULL )
	{
		if( pButton->GetCheck() ) 
		{
			m_curSelStockCondition->m_dStyle |= SelectStock_HisDate;
		}
		else
		{
			m_curSelStockCondition->m_dStyle &= (~SelectStock_HisDate);
		}

		m_wndBeginDate.EnableWindow((m_curSelStockCondition->m_dStyle & SelectStock_HisDate)?1:0);
		m_wndEndDate.EnableWindow((m_curSelStockCondition->m_dStyle & SelectStock_HisDate)?1:0);
	}
}

void CDlgSetStockAndTime::OnUnioncheck() 
{
	// TODO: Add your control notification handler code here
	CButton* pButton = (CButton*)this->GetDlgItem(IDC_UNIONCHECK);
	if( pButton != NULL )
	{
		if( pButton->GetCheck() ) 
		{
			m_curSelStockCondition->m_dStyle |= SelectStock_InK_Disp_Mark;
		}
		else
		{
			m_curSelStockCondition->m_dStyle &= (~SelectStock_InK_Disp_Mark);
		}
	}
}
