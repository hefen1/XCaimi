// DlgExchOptimize.cpp : implementation file
//
#include "stdafx.h"
#include "DlgExchOptimize.h"
#include "DlgFormulaMan.h"

#define HS_CDlgExchOptimize_Begin _T("��ʼ")
#define HS_CDlgExchOptimize_End	_T("ֹͣ")
// CDlgExchOptimize dialog

IMPLEMENT_DYNAMIC(CDlgExchOptimize, CDialog)
CDlgExchOptimize::CDlgExchOptimize(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgExchOptimize::IDD, pParent)
{
}

CDlgExchOptimize::~CDlgExchOptimize()
{
}

void CDlgExchOptimize::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST, m_wndList);
	DDX_Control(pDX, IDC_CACLPROGRESS, m_wndProgress);
	DDX_Control(pDX, IDC_COMBO_OBJ, m_wndTestObj);
	DDX_Control(pDX, IDC_COMBO_CYCLE, m_wndTestCyc);
	DDX_Control(pDX, IDC_BEGINDATE, m_wndBeginDate);
	DDX_Control(pDX, IDC_ENDDATE, m_wndEndDate);
}


BEGIN_MESSAGE_MAP(CDlgExchOptimize, CDialog)
	ON_REGISTERED_MESSAGE(HX_USER_COMPILEDATA, OnExchangeData)
	ON_BN_CLICKED(IDC_RADIO_DEFAULT, OnBnClickedRadioDefault)
	ON_BN_CLICKED(IDC_RADIO_COMM, OnBnClickedRadioComm)
	ON_BN_CLICKED(IDC_CHECK_ALLSEL, OnBnClickedCheckAllsel)
	ON_BN_CLICKED(IDC_CHECK_ALLLOCTLDATA, OnBnClickedCheckAllloctldata)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()


LRESULT CDlgExchOptimize::OnExchangeData(WPARAM wp,LPARAM lp)
{
	GetYouSelCaclData()->OnExchangeData(wp, lp);

	switch( wp )
	{
		// ����֪ͨ
	case EXCH_MSG_WP_CACL_TEMPO	:
		break;
		// ��ʼ
	case EXCH_MSG_WP_CACL_START	: // ���õ�ǰ���㽹��
		break;
		// ֹͣ// ��ѡֹͣ
	case EXCH_MSG_WP_CACL_STOP	:
	case YouSel_MSG_WP_CACL_STOP: // ֹͣ����
		break;
		// ����ʧ��
	case EXCH_MSG_WP_CACL_FAIL	: // ������һ����
		break;
		// �������
	case EXCH_MSG_UP_CACL_SUCC	: // ����һ������
		break;
		// ��ѡ��ʼ
	case YouSel_MSG_WP_CACL_START : // ����״̬
		break;
		// ��ѡʧ��
	case YouSel_MSG_WP_CACL_FAIL : // ʧ�ܴ���
		break;
		// �������
	case YouSel_MSG_UP_CACL_SUCC : // ��ɴ���
		{
			CWnd* pWnd = GetDlgItem(IDC_STATIC_NOTE);
			if( pWnd ) 
			{
				pWnd->SetWindowText(_T("�����ѡ!"));
				pWnd->ShowWindow(SW_SHOW);
			}
			m_wndProgress.ShowWindow(SW_HIDE);
		}
		break;
	}

	return 0;
}

void CDlgExchOptimize::FailDispose(LPARAM lp)
{
	CString strRet = GetYouSelCaclData()->ReturnMsgPress(lp);

	if( strRet.IsEmpty() == FALSE )
	{
		MessageBox(strRet, _T("��ʾ"), MB_OK);
	}
}

// ����ָ��Ի��е�ָ��
void CDlgExchOptimize::SetExpression()
{
	GetYouSelCaclData()->SetExpressionFromCtrl();
}


void CDlgExchOptimize::OnBnClickedRadioDefault()
{
	SetDefTact();
}

void CDlgExchOptimize::OnBnClickedRadioComm()
{
	SetCommTact();
}

void CDlgExchOptimize::OnBnClickedCheckAllsel()
{
	CButton* pBut = (CButton*)GetDlgItem(IDC_CHECK_ALLSEL);
	if( pBut )
	{
		GetYouSelCaclData()->SetSelectAll(&m_wndList, pBut->GetCheck());
	}
}

void CDlgExchOptimize::OnBnClickedCheckAllloctldata()
{
	CButton* pBut = (CButton*)GetDlgItem(IDC_CHECK_ALLLOCTLDATA);
	if( pBut )
	{
		CWnd* pWnd = GetDlgItem(IDC_BEGINDATE);
		if( pWnd )
		{
			pWnd->EnableWindow(!pBut->GetCheck());
		}
		pWnd = GetDlgItem(IDC_ENDDATE);
		if( pWnd )
		{
			pWnd->EnableWindow(!pBut->GetCheck());
		}
	}
}

void CDlgExchOptimize::OnBnClickedOk()
{
	CWnd* pWnd = GetDlgItem(IDOK);
	if( pWnd )
	{
		CString strTxt;
		pWnd->GetWindowText(strTxt);
		if( strTxt.Compare(HS_CDlgExchOptimize_Begin) == 0 )
		{
			pWnd->SetWindowText(HS_CDlgExchOptimize_End);
			pWnd = GetDlgItem(IDC_STATIC_NOTE);
			if( pWnd ) pWnd->ShowWindow(SW_HIDE);
			m_wndProgress.ShowWindow(SW_SHOW);
			GetYouSelCaclData()->SetExpressionFromCtrl();
			GetYouSelCaclData()->StartEx();
		}
		else if( strTxt.Compare(HS_CDlgExchOptimize_End) == 0 )
		{
			pWnd->SetWindowText(HS_CDlgExchOptimize_Begin);
			if( pWnd ) pWnd->ShowWindow(SW_SHOW);
			m_wndProgress.ShowWindow(SW_HIDE);
			GetYouSelCaclData()->StopEx();
		}
	}
}

BOOL CDlgExchOptimize::OnInitDialog()
{
	CDialog::OnInitDialog();

	CWnd* pWnd = GetDlgItem(IDOK);
	if( pWnd )
	{
		pWnd->SetWindowText(HS_CDlgExchOptimize_Begin);
	}

	// ���ý�����
	m_wndProgress.ShowWindow(SW_HIDE);
	GetYouSelCaclData()->SetProgress(&m_wndProgress);
	// ��ʾ����ϵͳ�б�
	GetYouSelCaclData()->InitCtrl(&m_wndList);
	CDlgFormulaMan* pDlg = (CDlgFormulaMan*)GetParent();
	CTreeCtrlFormula* pTree = pDlg->GetCurTree( (WORD)Exchange );
	GetYouSelCaclData()->CopyExch(pTree);
	GetYouSelCaclData()->Display();

	// ����ʱ���
	CButton* pBut = (CButton*)GetDlgItem(IDC_CHECK_ALLLOCTLDATA);
	if( pBut )
	{
		pBut->SetCheck(1);
		OnBnClickedCheckAllloctldata();
	}

	// ��ʼ������Ŀ��
	m_wndTestObj.AddString(HS_EXCH_CACLRESULT_STOCK_HEADLE_PROFIT		);
	m_wndTestObj.AddString(HS_EXCH_CACLRESULT_STOCK_HEADLE_YEAR		);
	m_wndTestObj.AddString(HS_EXCH_CACLRESULT_STOCK_HEADLE_WIN		);
	m_wndTestObj.AddString(HS_EXCH_CACLRESULT_STOCK_HEADLE_DEALNUM	);
	m_wndTestObj.AddString(HS_EXCH_CACLRESULT_STOCK_HEADLE_SUCC	);	
	m_wndTestObj.SetCurSel(HS_EXCH_CACLRESULT_STOCK_ID_PROFIT-1);

	// ��ʼ����������
	m_wndTestCyc.SetCurSel(0);

	// ��ʼ������ʱ���
	const CTime tMin(1978,1,1,0,0,0);
	const CTime tMax(2038,1,1,0,0,0);
	struct tm *stm;
	time_t	lTime;
	time(&lTime);
	stm = localtime(&lTime);
	CTime	sTiem(stm->tm_year+1900-1, stm->tm_mon+1, stm->tm_mday, 0, 0, 0);
	m_wndBeginDate.SetFormat(_T("yyyy��MM��dd�� dddd"));
	m_wndBeginDate.SetRange(&tMin, &tMax);
	m_wndBeginDate.SetTime(&sTiem);
	m_wndEndDate.SetFormat(_T("yyyy��MM��dd�� dddd"));
	m_wndEndDate.SetRange(&tMin, &tMax);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
