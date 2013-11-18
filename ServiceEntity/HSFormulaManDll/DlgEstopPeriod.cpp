// DlgEstopPeriod.cpp : implementation file
//

#include "stdafx.h"
#include "DlgEstopPeriod.h"
#include "ExpPublic.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgEstopPeriod dialog


CDlgEstopPeriod::CDlgEstopPeriod(CWnd* pParent /*=NULL*/)
//	: CHSBaseDialog(CDlgEstopPeriod::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgEstopPeriod)
	//}}AFX_DATA_INIT
	Empty();

	Create(CDlgEstopPeriod::IDD,pParent);

}

void CDlgEstopPeriod::Empty()
{
	m_bDay = FALSE;
	m_bFifteenMinute = FALSE;
	m_bFiveMinute = FALSE;
	m_bMonth = FALSE;
	m_bMoreDay = FALSE;
	m_bOneDeal = FALSE;
	m_bOneMinute = FALSE;
	m_bSixtyMinute = FALSE;
	m_bThirtyMinute = FALSE;
	m_bWeek = FALSE;

	if(::IsWindow(m_oDefaultPeriod.m_hWnd))
	{
		CString str;
		str = _T(_T("日线"));
		m_oDefaultPeriod.SelectString(0,(char*)LPCTSTR(str));
	}
}

void CDlgEstopPeriod::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgEstopPeriod)
	DDX_Control(pDX, IDC_DEFAULTPERIOD, m_oDefaultPeriod);
	DDX_Check(pDX, IDC_PERIOD_TYPE_DAY, m_bDay);
	DDX_Check(pDX, IDC_FIFTEENMINUTE, m_bFifteenMinute);
	DDX_Check(pDX, IDC_FIVEMINUTE, m_bFiveMinute);
	DDX_Check(pDX, IDC_MONTH, m_bMonth);
	DDX_Check(pDX, IDC_MOREDAY, m_bMoreDay);
	DDX_Check(pDX, IDC_ONEDEAL, m_bOneDeal);
	DDX_Check(pDX, IDC_ONEMINUTE, m_bOneMinute);
	DDX_Check(pDX, IDC_SIXTYMINUTE, m_bSixtyMinute);
	DDX_Check(pDX, IDC_THIRTYMINUTE, m_bThirtyMinute);
	DDX_Check(pDX, IDC_WEEK, m_bWeek);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgEstopPeriod, CDialog)
	//{{AFX_MSG_MAP(CDlgEstopPeriod)
	//}}AFX_MSG_MAP
	
	ON_BN_CLICKED(IDC_ONEDEAL, OneDeal)
	ON_BN_CLICKED(IDC_ONEMINUTE, OneMinute)
	ON_BN_CLICKED(IDC_FIVEMINUTE, FiveMinute)
	ON_BN_CLICKED(IDC_FIFTEENMINUTE, FifteenMinute)
	ON_BN_CLICKED(IDC_THIRTYMINUTE, ThiryMinute)
	ON_BN_CLICKED(IDC_SIXTYMINUTE, SixtyMinute)
	ON_BN_CLICKED(IDC_PERIOD_TYPE_DAY, Day)
	ON_BN_CLICKED(IDC_WEEK, Week)
	ON_BN_CLICKED(IDC_MONTH, Month)
	ON_BN_CLICKED(IDC_MOREDAY, MoreDay)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgEstopPeriod message handlers

void CDlgEstopPeriod::OnOK() 
{
	
	// TODO: Add extra validation here
	this->UpdateData();
	//begin by HS 020401
	if(m_oDefaultPeriod.GetCurSel() == CB_ERR )
	{
		MessageBox(_T("请选择默认周期，谢谢！"),_T("禁用周期"),MB_ICONINFORMATION);
		return;
	}
	//end
	ShowWindow(SW_HIDE);
	//CHSBaseDialog::OnOK();
}

void CDlgEstopPeriod::OnCancel() 
{
	// TODO: Add extra cleanup here
	ShowWindow(SW_HIDE);
	//CHSBaseDialog::OnCancel();
}

BOOL CDlgEstopPeriod::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	CString str;
	str = _T(_T("日线"));
	m_oDefaultPeriod.SelectString(0,(char*)LPCTSTR(str));

	// TODO: Add extra initialization here	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

short CDlgEstopPeriod::GetEstopPeriod()   // 禁用周期
{
	UpdateData();

	short nEstopPeriod = 0;
	if(m_bDay)			 nEstopPeriod |= HX_PERIOD_PERIOD_TYPE_DAY;
	if(m_bFifteenMinute) nEstopPeriod |= HX_PERIOD_FIFTEENMINUTE;
	if(m_bFiveMinute)	 nEstopPeriod |= HX_PERIOD_FIVEMINUTE;
	if(m_bMonth)	     nEstopPeriod |= HX_PERIOD_MONTH;
	if(m_bMoreDay)	     nEstopPeriod |= HX_PERIOD_MOREDAY;
	if(m_bOneDeal)	     nEstopPeriod |= HX_PERIOD_ONEDEAL;
	if(m_bOneMinute)     nEstopPeriod |= HX_PERIOD_ONEMINUTE;
	if(m_bSixtyMinute)   nEstopPeriod |= HX_PERIOD_SIXTYMINUTE;
	if(m_bThirtyMinute)  nEstopPeriod |= HX_PERIOD_THIRTYMINUTE;
	if(m_bWeek)			 nEstopPeriod |= HX_PERIOD_WEEK;

	return nEstopPeriod;
}

/*改动前的程序
short CDlgEstopPeriod::GetDefaultPeriod() // 缺省周期
{

	UpdateData();

	int nIndex = m_oDefaultPeriod.GetCurSel();
	if(nIndex == -1)
		return 0;
/*
		分笔成交  0
		1分钟线   1
		5分钟线   2
		15分钟线  3
		30分钟线  4
		60分钟线  5
		日线      6
		周线      7
		月线      8
		多日线    9
*/		
/*
	if(nIndex == 6)	return HX_PERIOD_PERIOD_TYPE_DAY;
	if(nIndex == 3) return HX_PERIOD_FIFTEENMINUTE;
	if(nIndex == 2)	return HX_PERIOD_FIVEMINUTE;
	if(nIndex == 8)	return HX_PERIOD_MONTH;
	if(nIndex == 9)	return HX_PERIOD_MOREDAY;
	if(nIndex == 0)	return HX_PERIOD_ONEDEAL;
	if(nIndex == 1) return HX_PERIOD_ONEMINUTE;
	if(nIndex == 5) return HX_PERIOD_SIXTYMINUTE;
	if(nIndex == 4) return HX_PERIOD_THIRTYMINUTE;
	if(nIndex == 7)	return HX_PERIOD_WEEK;

	return 0;
}
*/

//begin by HS 020401
short CDlgEstopPeriod::GetDefaultPeriod() // 缺省周期
{

	UpdateData();

	int nIndex = m_oDefaultPeriod.GetCurSel();
	CString strSelect ;
	m_oDefaultPeriod.GetLBText(nIndex,strSelect);
	strSelect.TrimLeft();
	strSelect.TrimRight();

/*
		分笔成交  STRING_PERIOD_ONEDEAL      
		1分钟线   STRING_PERIOD_ONEMINUTE    
		5分钟线   STRING_PERIOD_FIVEMINUTE   
		15分钟线  STRING_PERIOD_FIFTEENMINUTE
		30分钟线  STRING_PERIOD_THIRTYMINUTE 
		60分钟线  STRING_PERIOD_SIXTYMINUTE  
		日线      STRING_PERIOD_PERIOD_TYPE_DAY          
		周线      STRING_PERIOD_WEEK         
		月线      STRING_PERIOD_MONTH        
		多日线    STRING_PERIOD_MOREDAY      
*/		

	if(strSelect == STRING_PERIOD_ONEDEAL      )	return HX_PERIOD_ONEDEAL      ;
	if(strSelect == STRING_PERIOD_ONEMINUTE    )    return HX_PERIOD_ONEMINUTE    ;
	if(strSelect == STRING_PERIOD_FIVEMINUTE   )	return HX_PERIOD_FIVEMINUTE   ;
	if(strSelect == STRING_PERIOD_FIFTEENMINUTE)	return HX_PERIOD_FIFTEENMINUTE;
	if(strSelect == STRING_PERIOD_THIRTYMINUTE )	return HX_PERIOD_THIRTYMINUTE ;
	if(strSelect == STRING_PERIOD_SIXTYMINUTE  )	return HX_PERIOD_SIXTYMINUTE  ;
	if(strSelect == STRING_PERIOD_PERIOD_TYPE_DAY          )    return HX_PERIOD_PERIOD_TYPE_DAY          ;
	if(strSelect == STRING_PERIOD_WEEK         )    return HX_PERIOD_WEEK         ;
	if(strSelect == STRING_PERIOD_MONTH        )    return HX_PERIOD_MONTH        ;
	if(strSelect == STRING_PERIOD_MOREDAY      )	return HX_PERIOD_MOREDAY      ;

	return 0;
}

void CDlgEstopPeriod::PostNcDestroy()
{
	//delete this;
}


void CDlgEstopPeriod::OneDeal()
{
   DefaultPeriod();
}

void CDlgEstopPeriod::DefaultPeriod(void)
{
	CString strSelect ;
	if( m_oDefaultPeriod.GetCurSel() != CB_ERR )
	{
		int nIndex = m_oDefaultPeriod.GetCurSel();
		m_oDefaultPeriod.GetLBText(nIndex,strSelect);
		strSelect.TrimLeft();
		strSelect.TrimRight();
	}
	UpdateData();
	m_oDefaultPeriod.ResetContent();
//	for(int i = 0;i <= m_oDefaultPeriod.GetCount();i++)
//	{
//		m_oDefaultPeriod.DeleteString(i);
//	}
	if(!m_bOneDeal)		    m_oDefaultPeriod.AddString(STRING_PERIOD_ONEDEAL      );
	if(!m_bOneMinute)		m_oDefaultPeriod.AddString(STRING_PERIOD_ONEMINUTE    );
	if(!m_bFiveMinute)		m_oDefaultPeriod.AddString(STRING_PERIOD_FIVEMINUTE   );
	if(!m_bFifteenMinute)	m_oDefaultPeriod.AddString(STRING_PERIOD_FIFTEENMINUTE);
	if(!m_bThirtyMinute)	m_oDefaultPeriod.AddString(STRING_PERIOD_THIRTYMINUTE );
	if(!m_bSixtyMinute)	    m_oDefaultPeriod.AddString(STRING_PERIOD_SIXTYMINUTE  );
	if(!m_bDay)				m_oDefaultPeriod.AddString(STRING_PERIOD_PERIOD_TYPE_DAY          );
	if(!m_bWeek)			m_oDefaultPeriod.AddString(STRING_PERIOD_WEEK         );
	if(!m_bMonth)			m_oDefaultPeriod.AddString(STRING_PERIOD_MONTH        );
	if(!m_bMoreDay)			m_oDefaultPeriod.AddString(STRING_PERIOD_MOREDAY      ); 
	
	if(strSelect != "")
	{
		m_oDefaultPeriod.SelectString(0,strSelect);
	}
	else
	if(!m_bDay)
	{
	  m_oDefaultPeriod.SelectString(0,STRING_PERIOD_PERIOD_TYPE_DAY);
	}

}

void CDlgEstopPeriod::OneMinute()
{
	DefaultPeriod();
	// TODO: Add your control notification handler code here
}

void CDlgEstopPeriod::FiveMinute()
{
	DefaultPeriod();
	// TODO: Add your control notification handler code here
}

void CDlgEstopPeriod::FifteenMinute()
{
	DefaultPeriod();
	// TODO: Add your control notification handler code here
}

void CDlgEstopPeriod::ThiryMinute()
{
	DefaultPeriod();
	// TODO: Add your control notification handler code here
}

void CDlgEstopPeriod::SixtyMinute()
{
	DefaultPeriod();
	// TODO: Add your control notification handler code here
}

void CDlgEstopPeriod::Day()
{
	DefaultPeriod();
	// TODO: Add your control notification handler code here
}

void CDlgEstopPeriod::Week()
{
	DefaultPeriod();
	// TODO: Add your control notification handler code here
}

void CDlgEstopPeriod::Month()
{
	DefaultPeriod();
	// TODO: Add your control notification handler code here
}

void CDlgEstopPeriod::MoreDay()
{
	DefaultPeriod();
	// TODO: Add your control notification handler code here
}
//end
