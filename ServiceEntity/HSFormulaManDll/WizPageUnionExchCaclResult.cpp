#include "stdafx.h"
#include "WizPageUnionExchCaclResult.h"
#include "SheetExchMain.h"
#include "DlgExchParamSet.h"
#include "WizPageExchSell.h"
/*#include "..\comm\HSTextDefine.h"*/

int CALLBACK SortListCtrl(LPARAM lp1, LPARAM lp2, LPARAM lpdef)
{
	_tagExchSortParam* pData = (_tagExchSortParam*)lpdef;

	if( pData == NULL || pData->m_pData == NULL )
		return 0;

	return -pData->m_pData->CompareParamOfCol((UINT)lp1, (UINT)lp2, pData->m_colID);
}


CWizPageUnionExchCaclResult::CWizPageUnionExchCaclResult(CWnd* pParent /*=NULL*/)
	: CWizPageExchBase(CWizPageUnionExchCaclResult::IDD, pParent)
{
	m_pData = NULL;
}

CWizPageUnionExchCaclResult::~CWizPageUnionExchCaclResult()
{
}

void CWizPageUnionExchCaclResult::DoDataExchange(CDataExchange* pDX)
{
	CWizPageExchBase::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_wndList);
}


BEGIN_MESSAGE_MAP(CWizPageUnionExchCaclResult, CWizPageExchBase)
	ON_NOTIFY(NM_CLICK, IDC_LIST1, OnNMClickList1)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST1, OnNMDblclkList1)
	ON_NOTIFY(HDN_ITEMCLICK, IDC_LIST1, OnHdnItemclickList1)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST1, OnLvnColumnclickList1)
	ON_BN_CLICKED(IDB_RE_CACL, OnBnClickedReCacl)
END_MESSAGE_MAP()


// CWizPageExchResult message handlers

BOOL CWizPageUnionExchCaclResult::OnInitDialog()
{
	CWizPageExchBase::OnInitDialog();

	// TODO:  Add extra initialization here

	Initial();
	PlayData();
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CWizPageUnionExchCaclResult::OnSetActive()
{
	CSheetExchMain* pParent = (CSheetExchMain*)GetParent();
	if( pParent != NULL )
	{
		CString str(_T("系统评测-"));
		CExchCalcMain* pData = pParent->GetData();
		CExpression *pExp = pData->GetExp();
		if( pData != NULL && pExp != NULL )
		{
			
			str += pExp->GetName();
			str += "-";
		}
		str += _T("参数优化");
		pParent->SetWindowText(str);

		//pParent->SetFinishText("详情");
	}
	PlayData();
}

BOOL CWizPageUnionExchCaclResult::OnKillActive(int nPos /*= -1*/)
{
	if( nPos < 0 ) return TRUE;

	CSheetExchMain* pParent = (CSheetExchMain*)GetParent();
	if( pParent != NULL )
	{
		/*pParent->SetFinishText(_T("执行"));*/
	}
	return TRUE;
}

BOOL CWizPageUnionExchCaclResult::OpenDetailed()
{
	if( CWizPageExchBase::m_pMainTech == NULL || !IsWindowVisible() )
	{
		return false;
	}

	int iCurSel = m_wndList.GetSelectionMark();
	if( iCurSel < 0 || m_pMainTech->GetCaclResultCount() <= iCurSel )
		return false;

	UINT uID = (UINT)m_wndList.GetItemData(iCurSel);

	CSheetExchMain* pParent = (CSheetExchMain*)GetParent();
	if( pParent != NULL )
	{
		pParent->OpenTableWnd(uID);
	}
	else
	{
		MessageBox(_T("没有统计结果!"), FORMULAMAN_EXCH_PROMPT, MB_OK);
	}

	return true;
}

BOOL CWizPageUnionExchCaclResult::PlayData(CExchCalcMain* pData)
{
	if( CWizPageExchBase::m_pMainTech == NULL )
	{
		CWizPageExchBase::m_pMainTech = pData;
	}

	if( CWizPageExchBase::m_pMainTech == NULL )
		return false;

	CWizPageExchBase::m_pMainTech->ToOptimizeCtrl(&m_wndList);
	return true;
}

BOOL CWizPageUnionExchCaclResult::ClearCell()
{
	return m_wndList.DeleteAllItems();
}

BOOL CWizPageUnionExchCaclResult::Initial(CExchCalcMain* pData)
{
   	if( CWizPageExchBase::m_pMainTech == NULL )
	{
		CWizPageExchBase::m_pMainTech = pData;
	}

	if( CWizPageExchBase::m_pMainTech == NULL )
		return false; 

	CWizPageExchBase::m_pMainTech->InitialOptimizeCtrl(&m_wndList);
	return true;
}
void CWizPageUnionExchCaclResult::OnNMClickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	*pResult = 0;
}

void CWizPageUnionExchCaclResult::OnNMDblclkList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	OpenDetailed();
	*pResult = 0;
}

void CWizPageUnionExchCaclResult::OnHdnItemclickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMHEADER phdr = reinterpret_cast<LPNMHEADER>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;
}

void CWizPageUnionExchCaclResult::OnLvnColumnclickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: Add your control notification handler code here

	_tagExchSortParam sParam;
	sParam.m_colID = pNMLV->iSubItem;
	sParam.m_pData = CWizPageExchBase::m_pMainTech;
	m_wndList.SortItems(SortListCtrl, (DWORD)&sParam);
	*pResult = 0;
}

void CWizPageUnionExchCaclResult::OnBnClickedReCacl()
{
	if( m_pMainTech == NULL )
	{
		MessageBox(_T("内部数据设置错误!"), FORMULAMAN_EXCH_PROMPT, MB_OK);
		return ;
	}
	CDlgExchParamSet dlg(m_pMainTech->GetOptimize(), this);

	if( dlg.DoModal() != IDOK )
	{
		return;
	}

	if( dlg.m_bHaveChange == TRUE )
	{
		CSheetExchMain* pParent = (CSheetExchMain*)GetParent();
		if( pParent != NULL )
		{
			if( pParent->Start() != TCaclReturn_Succ )
			{
				MessageBox(_T("重新开始交易评测计算失败!"), FORMULAMAN_EXCH_PROMPT, MB_OK);
				return;
			}     
			this->ClearCell();
			this->PlayData();
		}
		else
		{
			MessageBox(_T("重新优化错误!"), FORMULAMAN_EXCH_PROMPT, MB_OK);
		}
	}
}
