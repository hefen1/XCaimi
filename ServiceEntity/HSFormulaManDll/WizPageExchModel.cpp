// WizPageExchModel.cpp : implementation file
//

#include "stdafx.h"

#include "WizPageExchModel.h"
#include "SheetExchMain.h"
#include "DlgFormulaMan.h"
#include "WizPageExchSell.h"
#include "WinnerApplication.h"
#include "HSMessage.h"
// CWizPageExchModel dialog
extern HWND g_hParentWnd;
extern double EXCH_POUNDAGE;
extern BOOL GetStockByCode(CodeInfo* pInfo, StockUserInfo& stockInfo );
//IMPLEMENT_DYNAMIC(CWizPageExchModel, CDialog)
CWizPageExchModel::CWizPageExchModel(CWnd* pParent /*=NULL*/)
	: CWizPageExchBase(CWizPageExchModel::IDD, pParent)
	, m_nRadioIndex(0)
	, m_nMultiBuyIndex(0)
	, m_nOverBuyIndex(0)
	, m_nBuySortIndex(0)
	, m_lInvestMoney(0)
	, m_nSigOrAll(0)
	, m_nAllBuyOrLimitBuy(0)
	, m_nRandomBuyOrTechSort(0)
	, m_nBtosOrStob(0)
{
	m_pData = &m_TExchangeModel;
	m_ayCheck = new CArray<StockArea*,StockArea*>;
}

CWizPageExchModel::~CWizPageExchModel()
{
	if( m_ayCheck != NULL )
	{
		for ( int i=0;i<m_ayCheck->GetCount(); i++)
			delete m_ayCheck->GetAt(i);
		delete m_ayCheck;
		m_ayCheck = NULL;
	}
}

void CWizPageExchModel::DoDataExchange(CDataExchange* pDX)
{
	CWizPageExchBase::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIMITCOUNT, m_wndEditLimitCount);
	DDX_Control(pDX, IDC_MOSTCOUNT, m_wndEditLimitBuyCount);
	DDX_Control(pDX, IDC_SINGLETEST, m_wndRadioSOrM);
	DDX_Control(pDX, IDC_ALLBUY, m_wndRadioBuyAll);
	DDX_Control(pDX, IDC_RANDOM, m_wndRadioBuyRandom);
	DDX_Control(pDX, IDC_BTOS, m_wndRadioSToB);
	DDX_Control(pDX, IDD_INCLUDE, m_wndIncludeStock);
	DDX_Control(pDX, IDC_STK_LIST, m_ctlStockList);
	DDX_Control(pDX, IDC_STAITCEMTY, m_wndSortTech);
	DDX_Radio(pDX, IDC_SINGLETEST, m_nSigOrAll);
	DDX_Radio(pDX, IDC_ALLBUY, m_nAllBuyOrLimitBuy);
	DDX_Radio(pDX, IDC_RANDOM, m_nRandomBuyOrTechSort);
	DDX_Radio(pDX, IDC_BTOS, m_nBtosOrStob);
}


BEGIN_MESSAGE_MAP(CWizPageExchModel, CWizPageExchBase)
	ON_BN_CLICKED(IDC_HOLETEST, OnBnClickedHoletest)
	ON_BN_CLICKED(IDC_SINGLETEST, OnBnClickedSingletest)
	ON_BN_CLICKED(IDC_ALLBUY, OnBnClickedAllbuy)
	ON_BN_CLICKED(IDC_MOSTBUY, OnBnClickedMostbuy)
	ON_BN_CLICKED(IDC_RANDOM, OnBnClickedRandom)
	ON_BN_CLICKED(IDC_SORT, OnBnClickedSort)
	ON_BN_CLICKED(IDD_CHANGEIN, OnBnClickedChangein)

	ON_REGISTERED_MESSAGE(HX_USER_COMPILEDATA, OnExchangeData)
	ON_REGISTERED_MESSAGE(HX_USER_DRAGDATA,OnUserDragData)
	ON_MESSAGE(WM_KEYBOARD_NOTIFY, OnKeyBoadrMsg)
	ON_BN_CLICKED(IDB_ADD, OnBnClickedAdd)
	ON_BN_CLICKED(IDB_DELETE, OnBnClickedDelete)

	ON_NOTIFY(NM_SETFOCUS, IDC_STK_LIST, OnNMSetfocusStkList)
	ON_NOTIFY(NM_KILLFOCUS, IDC_STK_LIST, OnNMKillfocusStkList)
	ON_NOTIFY(NM_CLICK, IDC_STK_LIST, OnNMClickStkList)
END_MESSAGE_MAP()


// CWizPageExchModel message handlers

BOOL CWizPageExchModel::OnInitDialog()
{
	CWizPageExchBase::OnInitDialog();

	// TODO:  Add extra initialization here

	int i = m_nBuySortIndex;

	//m_wndEditLimitCount.EnableWindow(FALSE);
	//m_wndEditLimitBuyCount.EnableWindow(FALSE);
	m_nRadioIndex = 0;
	m_nBuySortIndex = 0;
	m_nMultiBuyIndex = 0;
	m_nOverBuyIndex = 0;
	//m_wndRadioSOrM.SetCheck(1);
	//CWnd* pWnd = this->GetDlgItem(IDC_ALLBUY);
	//pWnd->EnableWindow(FALSE);
	//pWnd = this->GetDlgItem(IDC_MOSTBUY);
	//pWnd->EnableWindow(FALSE);
	//pWnd = this->GetDlgItem(IDC_RANDOM);
	//pWnd->EnableWindow(FALSE);
	//pWnd = this->GetDlgItem(IDC_SORT);
	//pWnd->EnableWindow(FALSE);
	//pWnd = this->GetDlgItem(IDC_BTOS);
	//pWnd->EnableWindow(FALSE);
	//pWnd = this->GetDlgItem(IDC_STOB);
	//pWnd->EnableWindow(FALSE);
	//m_wndIncludeStock.SetWindowText("");
	if( m_pMainTech )
	{
		if( m_pMainTech->GetStockList() )
			m_ayStkList.Copy(*m_pMainTech->GetStockList());
	}

	// 加入默认股票
	if( m_ayStkList.GetSize() <= 0 )
	{
		CodeInfo s000001;
		memcpy(s000001.m_cCode, "000001", sizeof(s000001.m_cCode));
		s000001.m_cCodeType = SZ_Bourse|KIND_STOCKA;
		StockUserInfo* pStock = new StockUserInfo;
		GetStockByCode(&s000001,*pStock);
		if( pStock != NULL )
		{
			m_ayStkList.Add(pStock);
		}
	}

	InitStkList();
	PlayData();
	ShowSingl(!m_TExchangeModel.m_bSingle);

	SetDelStockButton(FALSE);
	SetEnableAddDelButtons(TRUE);

	m_pKeyBoard = (IKeyboard*)CWinnerApplication::GetObject(OBJ_KEYBOARD);
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CWizPageExchModel::OnBnClickedHoletest()
{
	// TODO: Add your control notification handler code here
	m_nRadioIndex = 1;
	m_nBuySortIndex = 0;
	m_nMultiBuyIndex = 0;
	m_nOverBuyIndex = 0;

	ShowSingl(SW_SHOW);
	//m_wndRadioBuyRandom.EnableWindow(TRUE);
	//m_wndEditLimitBuyCount.EnableWindow(FALSE);
	//m_wndEditLimitCount.EnableWindow(TRUE);

	//CWnd* pWnd = this->GetDlgItem(IDC_ALLBUY);
	//pWnd->EnableWindow(TRUE);
	//pWnd = this->GetDlgItem(IDC_MOSTBUY);
	//pWnd->EnableWindow(TRUE);
	//pWnd = this->GetDlgItem(IDC_RANDOM);
	//pWnd->EnableWindow(FALSE);
	//pWnd = this->GetDlgItem(IDC_SORT);
	//pWnd->EnableWindow(FALSE);
	//pWnd = this->GetDlgItem(IDC_BTOS);
	//pWnd->EnableWindow(FALSE);
	//pWnd = this->GetDlgItem(IDC_STOB);
	//pWnd->EnableWindow(FALSE);

	//m_wndRadioBuyAll.SetCheck(1);
}

void CWizPageExchModel::OnBnClickedSingletest()
{
	// TODO: Add your control notification handler code here
	m_nRadioIndex = 0;
	m_nBuySortIndex = 0;
	m_nMultiBuyIndex = 0;
	m_nOverBuyIndex = 0;

	ShowSingl(SW_HIDE);
	//m_wndRadioBuyRandom.EnableWindow(FALSE);
	//m_wndEditLimitBuyCount.EnableWindow(FALSE);
	//m_wndEditLimitCount.EnableWindow(FALSE);

	//CWnd* pWnd = this->GetDlgItem(IDC_ALLBUY);
	//pWnd->EnableWindow(FALSE);
	//
	//pWnd = this->GetDlgItem(IDC_MOSTBUY);
	//pWnd->EnableWindow(FALSE);
	//pWnd = this->GetDlgItem(IDC_RANDOM);
	//pWnd->EnableWindow(FALSE);
	//pWnd = this->GetDlgItem(IDC_SORT);
	//pWnd->EnableWindow(FALSE);
	//pWnd = this->GetDlgItem(IDC_BTOS);
	//pWnd->EnableWindow(FALSE);
	//pWnd = this->GetDlgItem(IDC_STOB);
	//pWnd->EnableWindow(FALSE);

	//m_wndRadioBuyAll.SetCheck(0);
}

void CWizPageExchModel::OnBnClickedAllbuy()
{
	// TODO: Add your control notification handler code here
	m_nMultiBuyIndex = 1;

	ShowAllBuy(SW_HIDE);

	//CWnd* pWnd  = this->GetDlgItem(IDC_RANDOM);
	//pWnd->EnableWindow(FALSE);
	//pWnd = this->GetDlgItem(IDC_SORT);
	//pWnd->EnableWindow(FALSE);
	//pWnd = this->GetDlgItem(IDC_BTOS);
	//pWnd->EnableWindow(FALSE);
	//pWnd = this->GetDlgItem(IDC_STOB);
	//pWnd->EnableWindow(FALSE);
	//m_wndEditLimitBuyCount.EnableWindow(FALSE);
}

void CWizPageExchModel::OnBnClickedMostbuy()
{
	ShowAllBuy(SW_SHOW);
	// TODO: Add your control notification handler code here
	//CWnd* pWnd = this->GetDlgItem(IDC_RANDOM);
	//pWnd->EnableWindow(TRUE);
	//pWnd = this->GetDlgItem(IDC_SORT);
	//pWnd->EnableWindow(TRUE);
	//pWnd = this->GetDlgItem(IDC_BTOS);
	//pWnd->EnableWindow(FALSE);
	//pWnd = this->GetDlgItem(IDC_STOB);
	//pWnd->EnableWindow(FALSE);
	//m_wndEditLimitBuyCount.EnableWindow(TRUE);
	//m_wndRadioBuyRandom.SetCheck(1);
}

void CWizPageExchModel::OnBnClickedRandom()
{
	ShowRandom(SW_HIDE);
}

void CWizPageExchModel::OnBnClickedSort()
{
	// 打开公式管理
	CDlgFormulaMan *pwndForm = (CDlgFormulaMan*)AfxGetMainWnd();
	if( pwndForm == NULL )
		return;

	HSOpenExpressUseData data(hxShowReturnByExpressType|Tech);
	data.m_strName = new CString(_T("选择指标!"));
	pwndForm->Open(&data);

	if( data.m_pData != NULL )
	{
		CExpression* pExpression = (CExpression*)data.m_pData;
		m_TExchangeModel.m_sSortReason.m_strExp = pExpression->GetName();
		CArray<CFormulaDrawParam*, CFormulaDrawParam*> pAyParam;
		CString strParam;
		pExpression->GetDraw(strParam, pAyParam);
		if( pAyParam.GetSize() > 0 )
		{
			CFormulaDrawParam* pParam = pAyParam.GetAt(0);
			if( pParam )
			{
				m_TExchangeModel.m_sSortReason.m_strCondition = pParam->m_strName;
			}
			else
			{
				m_TExchangeModel.m_sSortReason.m_strCondition = CTreeCtrlFormula::g_strFirstTechLine;
			}
		}
	}
	else
	{
		m_nRandomBuyOrTechSort = 0;
		m_wndRadioBuyRandom.UpdateData(FALSE);
		m_wndRadioSToB.UpdateData(FALSE);
	}

	if( data.m_strName )
	{
		delete data.m_strName;
		data.m_strName = NULL;
	}
	ShowRandom(SW_SHOW);
}

void CWizPageExchModel::OnBnClickedChangein()
{
	// TODO: Add your control notification handler code here
	::SendMessage(g_hParentWnd,HX_USER_COMPILEDATA,2,(LPARAM)(this->GetParent()));	
}

void CWizPageExchModel::SetIncludeStockList(CArray<StockArea*,StockArea*>* pCheck,BOOL bAdd /*= FALSE*/)
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

LRESULT CWizPageExchModel::OnExchangeData(WPARAM wp,LPARAM lp)
{
	if( m_ayCheck == NULL )
	{
		SetIncludeStockList( ((CArray<StockArea*,StockArea*>*)lp),TRUE );
		return 0;
	}

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
void CWizPageExchModel::OnBnClickedAdd()
{
	// TODO: Add your control notification handler code here
	SetDelStockButton(FALSE);
	MSG pMsg;
	pMsg.message = WM_CHAR;
	pMsg.lParam = 0;
	pMsg.wParam = 0;
	SendAddStockMsg(&pMsg);
}

void CWizPageExchModel::OnBnClickedDelete()
{
	if( m_ayStkList.GetSize() > m_nStockIndex &&  m_nStockIndex > -1 )
	{
		DelStock(m_nStockIndex);
		SetDelStockButton(FALSE);
	}
}

void CWizPageExchModel::DelStock(int nIndex)
{
	m_ayStkList.RemoveAt(nIndex);
	DrawStkList();
	if( m_ctlStockList.GetItemCount() )
	{
		m_ctlStockList.SetFocus();
		ListView_SetItemState(m_ctlStockList.m_hWnd, ( m_ctlStockList.GetItemCount() > m_nStockIndex ) ? (m_ctlStockList.GetItemCount() - 1) : m_nStockIndex,
			LVIS_SELECTED|LVIS_FOCUSED,LVIS_SELECTED|LVIS_FOCUSED);
		SetDelStockButton(TRUE);
	}
}

void CWizPageExchModel::DrawStkList()
{
	StockUserInfo* pStock;
	LV_ITEM lvitem;
	char chTemp[9];

	m_ctlStockList.DeleteAllItems();
	for(int i = 0; i < m_ayStkList.GetSize(); i++ )
	{
		pStock = m_ayStkList.GetAt(i);
		if( pStock == NULL )
			continue;
		lvitem.mask = LVIF_TEXT | LVIF_IMAGE | LVIF_PARAM;
		lvitem.iItem = i;
		if( m_nAddStockIndex == i )
		{
			lvitem.iImage = 3;
		}
		else
		{
			lvitem.iImage = 2;
		}
		lvitem.iSubItem = 0;
		strncpy(chTemp, pStock->m_ciStockCode.m_cCode, sizeof(char) * 6);
		chTemp[6] = 0;
		lvitem.pszText = chTemp;
		lvitem.lParam = i;
		m_ctlStockList.InsertItem(&lvitem);//返回表项插入后的索引号

		lvitem.mask = LVIF_TEXT;
		lvitem.iSubItem = 1;
		strncpy(chTemp, pStock->m_cStockName, sizeof(char) * 8 );
		chTemp[8] = 0;
		lvitem.pszText = chTemp;
		m_ctlStockList.SetItem(&lvitem);
	}
}

void CWizPageExchModel::InitStkList()
{
	LV_COLUMN	lvcol;
	lvcol.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
	lvcol.fmt = LVCFMT_CENTER;//居中
	lvcol.cx = 70;

	m_ctlStockList.DeleteAllItems();
	while(m_ctlStockList.DeleteColumn(0) != 0);

	lvcol.iSubItem = 0;
	lvcol.pszText = HS_STOCK_CODE;
	m_ctlStockList.InsertColumn(0, &lvcol);

	lvcol.iSubItem = 1;
	lvcol.pszText = HS_STOCK_NAME;
	m_ctlStockList.InsertColumn(1, &lvcol);
}

BOOL CWizPageExchModel::SetDelStockButton(BOOL bFlag)
{
	CWnd* pAddWnd = GetDlgItem(IDB_ADD);
	CWnd* pWnd = GetDlgItem(IDB_DELETE);

	if( m_ctlStockList.GetSelectedCount() > 0 && pAddWnd->IsWindowEnabled() )
	{
		pWnd->EnableWindow(bFlag);
	}
	else
	{
		pWnd->EnableWindow(FALSE);
	}
	return bFlag;
}

void CWizPageExchModel::OnNMSetfocusStkList(NMHDR *pNMHDR, LRESULT *pResult)
{
	SetDelStockButton(TRUE);
	*pResult = 0;
}

void CWizPageExchModel::OnNMKillfocusStkList(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here
	//SetDelStockButton(FALSE);
	*pResult = 0;
}

BOOL CWizPageExchModel::SendAddStockMsg(MSG *pMsg)
{
	AlarmMsgData srtData;
	srtData.pWnd = this->GetParent();
	srtData.pMsg = pMsg;
	CRect rect;
	GetWindowRect(rect);
	HSShowKeyboardInfo KeyInfo;
	KeyInfo.m_cStyle = HSShowKeyboardInfo::right | HSShowKeyboardInfo::bottom | HSShowKeyboardInfo::stock;
	KeyInfo.m_oRect.CopyRect(rect);
	if ( m_pKeyBoard )
	{
		m_pKeyBoard->ShowKeyboard('\'',&KeyInfo,m_hWnd);
	}
	return TRUE;
	//return (BOOL)::SendMessage(g_hParentWnd, HX_USER_COMPILEDATA, 23, (LPARAM)&srtData);
}

LRESULT CWizPageExchModel::OnUserDragData(WPARAM wParam, LPARAM lParam)
{
	HSCurKeyInfo* pData = (HSCurKeyInfo*)lParam;
	if(pData == NULL)
		return -1;

	StockUserInfo* pStock = (StockUserInfo*)::SendMessage(g_hParentWnd, HX_USER_COMPILEDATA, 19, (LPARAM)&pData->m_cCode);

	if( pStock == NULL )
		return -1;
	AddStock(pStock);

	return 0;
}

BOOL CWizPageExchModel::AddStock(StockUserInfo* pData)
{
	StockUserInfo* pTemp;
	for(int i = 0; i < m_ayStkList.GetSize(); i++ )
	{
		pTemp = m_ayStkList.GetAt(i);
		if( memcmp(pTemp->m_ciStockCode.m_cCode, pData->m_ciStockCode.m_cCode, sizeof(char) * 6) == 0 )
		{
			MessageBox(_T("已包含股票") + pData->m_ciStockCode.GetCode() + "."
				, _T("提示..."),MB_OK);
			return FALSE;
		}
	}
	m_ayStkList.Add(pData); 
	DrawStkList();

	m_nStockIndex = m_ayStkList.GetCount() - 1;

	if( m_ctlStockList.GetItemCount() )
	{
		m_ctlStockList.SetFocus();
		ListView_SetItemState(m_ctlStockList.m_hWnd, m_ctlStockList.GetItemCount() - 1,
			LVIS_SELECTED|LVIS_FOCUSED,LVIS_SELECTED|LVIS_FOCUSED);
		SetDelStockButton(TRUE);
	}
	return TRUE;
}

BOOL CWizPageExchModel::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class

	if( pMsg && (pMsg->message == WM_KEYDOWN || pMsg->message == WM_CHAR) )
	{
		CWnd* pParent = GetParent();
		if( pParent && pMsg->wParam == VK_RETURN )
		{
			pParent->SendMessage(WM_KEYDOWN,VK_RETURN,0);
			return TRUE;
		}

		CWnd* pWnd = CWnd::FromHandle(pMsg->hwnd);
		if( pWnd != NULL )
		{
			UINT uID = pWnd->GetDlgCtrlID();
			if( uID != IDC_LIMITCOUNT && uID != IDC_MOSTCOUNT
				&& uID != IDC_MONEYC && SendAddStockMsg(pMsg))
			{
				return TRUE;
			}
		}
	}
	return CWizPageExchBase::PreTranslateMessage(pMsg);
}

void CWizPageExchModel::SetEnableAddDelButtons(BOOL Flag)
{
	//设置加/减股票按钮为flag状态
	CWnd *pWnd = GetDlgItem(IDB_ADD);
	pWnd->EnableWindow(Flag);
	SetDelStockButton(Flag);

	//设置板块列表为~Flag状态
	pWnd = GetDlgItem(IDB_DELETE);
	pWnd->EnableWindow( Flag == FALSE ? TRUE : FALSE );

}

void CWizPageExchModel::OnNMClickStkList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pView = (LPNMLISTVIEW)pNMHDR;
	m_nStockIndex = pView->iItem;
	SetDelStockButton(TRUE);
	*pResult = 0;
}

BOOL CWizPageExchModel::OnKillActive(int nPos /*= -1*/)
{
	if(nPos <= 0) return TRUE;

	BOOL breturn;
	breturn =  SetData();
	return breturn;
}

void CWizPageExchModel::AddStockList(CArray<StockUserInfo*, StockUserInfo*>* pDesc, 
				  CArray<StockUserInfo*, StockUserInfo*>* pSour)
{
	if( pDesc == NULL || pSour == NULL )
		return;
	int lEnd = pSour->GetSize();
	int jEnd = pDesc->GetSize();
	BOOL bHave;
	for( int i = 0; i < lEnd; i++ )
	{
		StockUserInfo* pStock = pSour->GetAt(i);
		bHave = FALSE;
		for( int j = 0; j < jEnd; j++ )
		{
			StockUserInfo* pStockDesc = pDesc->GetAt(j);
			if( pStockDesc == pStock )
			{
				bHave = TRUE;
				break;
			}
		}
		if( bHave == FALSE )
		{
			pDesc->Add(pStock);
		}
	}
}

void CWizPageExchModel::OnSetActive()
{
	CSheetExchMain* pParent = (CSheetExchMain*)GetParent();
	if( pParent != NULL )
	{
		CString str(_T("系统评测-"));
		CExchCalcMain* pData = pParent->GetData();
		if( pData != NULL )
		{
			CExpression *pExp = pData->GetExp();
			if( pExp != NULL )
			{
				str += pExp->GetName();
				str += "-";
			}
		}
		str += _T("市场模型设置");
		pParent->SetWindowText(str);
	}
}

BOOL CWizPageExchModel::SetData()
{
	m_TExchangeModel.m_ayStkList.Copy( m_ayStkList );
	m_TExchangeModel.m_bSingle = m_wndRadioSOrM.GetCheck();
	m_TExchangeModel.m_bBuyAll = m_wndRadioBuyAll.GetCheck();
	m_TExchangeModel.m_bBuyRandom = m_wndRadioBuyRandom.GetCheck();
	m_TExchangeModel.m_bSToB   = m_wndRadioSToB.GetCheck();

	CString str;
	long    tem;
	CWnd* pWnd = this->GetDlgItem(IDC_LIMITCOUNT);
	pWnd->GetWindowText(str);
	tem = atol(str);
	m_TExchangeModel.m_nHoleStockCount = tem;

	pWnd = this->GetDlgItem(IDC_MOSTCOUNT);
	pWnd->GetWindowText(str);
	tem = atol(str);
	m_TExchangeModel.m_nOneBuyCount = tem;

	pWnd = this->GetDlgItem(IDC_MONEYC);
	pWnd->GetWindowText(str);
	tem = atol(str);
	m_TExchangeModel.m_lAllInvestCount = tem;

	pWnd = GetDlgItem(IDC_MONEYC2);
	if( pWnd )
	{
		pWnd->GetWindowText(str);
		EXCH_POUNDAGE = atof(str);
	}
	// 股票
	m_TExchangeModel.m_ayStkList.Copy( m_ayStkList );
	CArray<StockUserInfo*,StockUserInfo*>* pAyStock = NULL;
// 	if( g_hParentWnd )
// 	{
// 		pAyStock = (CArray<StockUserInfo*,StockUserInfo*>*)::SendMessage(g_hParentWnd, HX_USER_COMPILEDATA, 67, (LPARAM)(m_ayCheck));	
// 	}
	if( pAyStock != NULL )
	{
		AddStockList(&m_TExchangeModel.m_ayStkList, pAyStock);
	}

	if( m_TExchangeModel.m_ayStkList.IsEmpty() )
	{
		MessageBox(_T("股票不能为空"), FORMULAMAN_EXCH_PROMPT, MB_OK);
		return FALSE;
	}

	return SaveOption();
}

void CWizPageExchModel::PlayData()
{
	m_nSigOrAll = 1 - (int)m_TExchangeModel.m_bSingle;
	m_nAllBuyOrLimitBuy = 1 - (int)m_TExchangeModel.m_bBuyAll;
	m_nRandomBuyOrTechSort = 1 - (int)m_TExchangeModel.m_bBuyRandom;
	m_nBtosOrStob = 1 - (int)m_TExchangeModel.m_bSToB;

	UpdateData(FALSE);

	CString str;
	

	CWnd* pWnd = this->GetDlgItem(IDC_LIMITCOUNT);
	str.Format("%d", m_TExchangeModel.m_nHoleStockCount);
	pWnd->SetWindowText(str);

	pWnd = this->GetDlgItem(IDC_MOSTCOUNT);
	str.Format("%d", m_TExchangeModel.m_nOneBuyCount);
	pWnd->SetWindowText(str);

	pWnd = this->GetDlgItem(IDC_MONEYC);
	str.Format("%.f", m_TExchangeModel.m_lAllInvestCount);
	pWnd->SetWindowText(str);

	if( m_ayCheck != NULL )
	{
		str.Empty();

		for( int i=0; i<m_ayCheck->GetSize(); i++ )
		{
			StockArea* pCheck = m_ayCheck->GetAt(i);
			if( pCheck != NULL )
			{
				str += pCheck->strText;
				str += ",";
			}
		}
		m_wndIncludeStock.SetWindowText(str);
	}
	else
	{
		m_wndIncludeStock.SetWindowText("");
	}

	if( m_TExchangeModel.m_bBuyRandom == FALSE && !m_TExchangeModel.m_sSortReason.m_strExp.IsEmpty())
	{
		str = _T("按指标");
		str += m_TExchangeModel.m_sSortReason.m_strExp;
		str += _T("[日线]");
		m_wndSortTech.SetWindowText(str);
	}
	else
	{
		m_wndSortTech.SetWindowText("");
	}

	pWnd = GetDlgItem(IDC_MONEYC2);
	if( pWnd )
	{
		str.Format("%.4f", EXCH_POUNDAGE);
		pWnd->SetWindowText(str);
	}
	DrawStkList();
}

// m_nRandomBuyOrTechSort
void CWizPageExchModel::ShowRandom(int nCmd)
{
	UpdateData();
	if( nCmd == SW_SHOW )
	{
		if( m_nRandomBuyOrTechSort == 0 )
		{
			nCmd = SW_HIDE;
		}
	}
	m_wndRadioSToB.ShowWindow(nCmd);
	CWnd *pWnd = GetDlgItem(IDC_STOB);
	if( pWnd )
		pWnd->ShowWindow(nCmd);
	m_wndSortTech.ShowWindow(nCmd);
	if( m_nRandomBuyOrTechSort == 1 && !m_TExchangeModel.m_sSortReason.m_strExp.IsEmpty() )
	{
		CString str;

		str = _T("按指标");
		str += m_TExchangeModel.m_sSortReason.m_strExp;
		str += _T("[日线]");
		m_wndSortTech.SetWindowText(str);
	}
	else
	{
		m_wndSortTech.SetWindowText("");
	}
}

// m_nAllBuyOrLimitBuy
void CWizPageExchModel::ShowAllBuy(int nCmd)
{
	UpdateData();
	// 吕连新改，不设买入顺序设置
	return;

	if( nCmd == SW_SHOW )
	{
		if( m_nAllBuyOrLimitBuy == 0 )
		{
			nCmd = SW_HIDE;
		}
	}
	CWnd* pWnd;
	pWnd = GetDlgItem(IDC_RANDOM);
	if( pWnd )
		pWnd->ShowWindow(nCmd);
	pWnd = GetDlgItem(IDC_SORT);
	if( pWnd )
		pWnd->ShowWindow(nCmd);
	pWnd = GetDlgItem(IDC_STATICOVERLOAD);
	if( pWnd )
		pWnd->ShowWindow(nCmd);
	pWnd = GetDlgItem(IDC_MOSTCOUNT);
	if( pWnd )
		pWnd->ShowWindow(nCmd);
	pWnd = GetDlgItem(IDC_STATICGP);
	if( pWnd )
		pWnd->ShowWindow(nCmd);

	ShowRandom(nCmd);
}

// m_nSigOrAll
void CWizPageExchModel::ShowSingl(int nCmd)
{
	UpdateData();
	if( nCmd == SW_SHOW )
	{
		if( m_nSigOrAll == 0 )
		{
			nCmd = SW_HIDE;
		}
	}

	CWnd* pWnd;
	pWnd = GetDlgItem(IDC_LIMITCOUNT);
	if( pWnd )
	{
		pWnd->ShowWindow(nCmd);
	}
	pWnd = GetDlgItem(IDC_STATICLIMIT);
	if( pWnd )
	{
		pWnd->ShowWindow(nCmd);
	}
	pWnd = GetDlgItem(IDC_STATICCOUNT);
	if( pWnd )
	{
		pWnd->ShowWindow(nCmd);
	}
	pWnd = GetDlgItem(IDC_STATICMONEY);
	if( pWnd )
	{
		if( m_nSigOrAll )
			pWnd->SetWindowText(_T("全部股票共投入资金:"));
		else
			pWnd->SetWindowText(_T("每只股票投入资金:"));
	}
	//return;

	// 吕连新改，去除全市场的其它设置
	pWnd = GetDlgItem(ID_MULITBUY);
	if( pWnd )
		pWnd->ShowWindow(nCmd);
	pWnd = GetDlgItem(IDC_ALLBUY);
	if( pWnd )
		pWnd->ShowWindow(nCmd);
	pWnd = GetDlgItem(IDC_MOSTBUY);
	if( pWnd )
		pWnd->ShowWindow(nCmd);
	pWnd = GetDlgItem(IDC_STATICGP);
	if( pWnd )
		pWnd->ShowWindow(nCmd);
	pWnd = GetDlgItem(IDC_MOSTCOUNT);
	if( pWnd )
		pWnd->ShowWindow(nCmd);

	ShowAllBuy(nCmd);
}

/******************************************************************
*	函数功能：	初始化设置数据
*	函数参数：	TExchHead* pData : [in] 初值数据
*	返 回 值：	BOOL : 成功（TRUE），失败（FALSE）
*	调用依赖：	重载父类函数
*	作    者：
*	完成日期：
*	修改说明：	
*****************************************************************/
BOOL CWizPageExchModel::InitialData(TExchHead* pData)
{
	m_TExchangeModel = *((TExchModel*)pData);

	return TRUE;
}

LRESULT CWizPageExchModel::OnKeyBoadrMsg( WPARAM wParam, LPARAM lParam )
{
	StockUserInfo* pStock = (StockUserInfo*)lParam;
	if (pStock && !IsBadReadPtr(pStock,sizeof(StockUserInfo)))
	{
		StockUserInfo* pNewStock = new StockUserInfo;
		pNewStock->Copy(pStock);
		m_ayStkList.Add(pNewStock);
		DrawStkList();
	}
	return 0;
}