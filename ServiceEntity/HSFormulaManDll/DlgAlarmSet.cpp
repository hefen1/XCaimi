///////////////////////////////////////////////////////////////////////////////
// 文件名:			DlgAlarmSet.cpp
// 创建者：				
// 创建时间：		2002-4-25
// 内容描述：		"预警系统"条件设置模块
///////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DlgAlarmSet.h"
#include "DlgFormulaMan.h"
#include "AdvancedAlarmData.h"
#include "CompileEditView.h"
#include "DlgFormulaEdit.h"
#include "DlgAlarmParamSet.h"




// CDlgAlarmSet dialog
extern HWND			 g_hParentWnd;
extern BOOL GetStockByCode(CodeInfo* pInfo, StockUserInfo& stockInfo );
extern int GetStockByMaket(unsigned short nMarket,  CArray<StockUserInfo>& stockArray); 
IMPLEMENT_DYNAMIC(CDlgAlarmSet, CDialog)
CDlgAlarmSet::CDlgAlarmSet(CWnd* pParent /*=NULL*/, 
							   CAdvancedAlarmData* pAlarmData/* =NULL*/, 
							   StockUserInfo* pStockData/* = NULL*/,
							   HWND	hAlarmWnd)
	: CDialog(CDlgAlarmSet::IDD, pParent)
	, m_strConditionName(_T(""))
	, m_ctlStrBlockList(_T(""))
{
	ASSERT( pAlarmData != NULL );

	m_pAlarmData = pAlarmData;
	m_pUnion = NULL;
	m_pSingleSel = NULL;
	m_SingOrUnion = m_pAlarmData->m_SingleOrUnion;

	if( m_SingOrUnion == FALSE )
	{
		m_pUnion = new UnionSelCondition;
		if( m_pAlarmData->m_pUnionCondition != NULL )
			m_pUnion->Copy(m_pAlarmData->m_pUnionCondition);
	}
	else
	{
		m_pSingleSel = new SingleSelCondition;
		if( m_pAlarmData->m_pSingleSel != NULL )
			m_pSingleSel->Copy(m_pAlarmData->m_pSingleSel);
	}
	m_itemList.Create(IDB_BITMAP_ALARM, 16, 3, RGB(255, 255, 255));
	m_pAddStock = pStockData;
	m_hAlarmWnd = hAlarmWnd;
}

CDlgAlarmSet::~CDlgAlarmSet()
{
	m_itemList.DeleteImageList();
}

void CDlgAlarmSet::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB, m_wndTab);
	DDX_Control(pDX, IDC_STK_LIST, m_ctlStockList);
	DDX_Text(pDX, IDC_NAME, m_strConditionName);
	DDX_Control(pDX, IDC_PERIED, m_ctlPeried);
	DDX_Control(pDX, IDC_BLOKC_SEL, m_ctlBlockSel);
	DDX_Control(pDX, IDC_DELAY_SEL, m_ctlDelay);
	DDX_Control(pDX, IDC_DLG_SEL, m_ctlBox);
	DDX_Control(pDX, IDC_WARNING, m_ctlContinue);
	DDX_Control(pDX, IDC_SELL_SEL, m_ctlSell);
	DDX_Control(pDX, IDC_J_STK_SEL, m_ctlJG);
	DDX_Control(pDX, IDC_BLOCK_LIST, m_ctlBlockCom);
}


BEGIN_MESSAGE_MAP(CDlgAlarmSet, CDialog)
	ON_BN_CLICKED(IDC_ADD_STK, OnBnClickedAddStk)
	ON_REGISTERED_MESSAGE(HX_USER_DRAGDATA,OnUserDragData)
	ON_EN_CHANGE(IDC_NAME, OnEnChangeName)
	ON_MESSAGE(HX_USER_LOADCUR, LoadCurExp)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BLOKC_SEL, OnBnClickedBlokcSel)
	ON_BN_CLICKED(IDC_DESC, OnBnClickedDesc)
	ON_BN_CLICKED(IDC_DEL_STK, OnBnClickedDelStk)
	ON_NOTIFY(NM_CLICK, IDC_STK_LIST, OnNMClickStkList)
	ON_REGISTERED_MESSAGE(HX_USER_COMPILEDATA, OnGetBlockName)
	ON_CBN_KILLFOCUS(IDC_BLOCK_LIST, OnCbnKillfocusBlockList)
	ON_CBN_SETFOCUS(IDC_BLOCK_LIST, OnCbnSetfocusBlockList)
	ON_BN_CLICKED(ID_SET, OnBnClickedSet)
	ON_CBN_SELCHANGE(IDC_BLOCK_LIST, OnCbnSelchangeBlockList)
	ON_NOTIFY(NM_SETFOCUS, IDC_STK_LIST, OnNMSetfocusStkList)
	ON_NOTIFY(NM_KILLFOCUS, IDC_STK_LIST, OnNMKillfocusStkList)
END_MESSAGE_MAP()


void CDlgAlarmSet::OnCancel()
{
	Free();

	CDialog::OnCancel();
}

// 初始化
void CDlgAlarmSet::Init(void)
{
	m_ctlStockList.SetImageList(&m_itemList, LVSIL_SMALL);
	InitFace();				//界面
	InitTreeSel();			//列出当前选择
	InitStkList();			//股票列表表头设置
	SetType();				//设置按钮

	//设置板块名/股票数组
	if( !m_pAlarmData->m_strAlarmBlock.IsEmpty() )
	{
		m_CurSBlockName.Format("%s", m_pAlarmData->m_strAlarmBlock );
		m_ctlStrBlockList = m_CurSBlockName;
		m_ctlBlockCom.EnableWindow();
		SetEnableAddDelButtons(FALSE);
	}
	m_pAlarmData->CopyStkTo(&m_ayStkList);

	if( m_pAddStock != NULL )
	{
		if( AddStock(m_pAddStock) )
			m_nAddStockIndex = m_ayStkList.GetSize() - 1;
	}
	else 
	{
		m_nAddStockIndex = -1;
	}


	//设置预警名
	CWnd *pWnd = GetDlgItem(IDC_NAME);
	pWnd->SetWindowText(m_pAlarmData->m_strName);
	
	//设置OK按钮状态
	SetOkEnable();
	DrawStkList();
	SetDelStockButton(FALSE);
}

void CDlgAlarmSet::InitFace()
{
	DWORD dWord = 0;
	dWord |=  LVS_EX_FULLROWSELECT;
	dWord |=  LVS_EX_FLATSB;
	m_ctlStockList.SetExtendedStyle(dWord);
	
	CTreeCtrlFormula* pCurTree = m_wndTab.GetTree();
	if( pCurTree )
	{
		CDlgFormulaMan* pDlg = (CDlgFormulaMan*)AfxGetMainWnd();
		
		if( pDlg != NULL)
		{
			pCurTree->ModifyStyleEx(0,WS_EX_CLIENTEDGE);
			pCurTree->SetImageList(&CDlgFormulaMan::m_expressImages, TVSIL_NORMAL);
			pCurTree->m_pFormulaMan = pDlg;
			CTreeCtrlFormula* pTree = pDlg->GetCurTree( (WORD)Condition );
			pCurTree->CopyTree(pTree,CTreeCtrlFormula::CopyDelete);
			pTree = pDlg->GetCurTree( (WORD)Exchange );
			pCurTree->CopyTree(pTree);

			HTREEITEM hRootItem = pCurTree->InsertItem(CTreeCtrlFormula::g_strUnionCondition,
				CExpression::HS_Exp_Root, CExpression::HS_Exp_Root);
			CTreeGroup* pTreeGroup = new CTreeGroup(hx_OtherData,(DWORD)Union); 
			pCurTree->SetItemData(hRootItem, (DWORD)pTreeGroup);
			
			SaveLoad(CArchive::load);
			//pTree = pDlg->GetCurTree( (WORD)JiBenMian );
			//pCurTree->CopyTree(pTree);
		}
	}

	m_EditEidolon.CreateEx(WS_EX_CLIENTEDGE,NULL,NULL,WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER,
		CRect(0,0,0,0),this,1001);
	m_EditEidolon.SetFont( GetFont() );
	CRect rect;
	CWnd *pWnd = GetDlgItem(IDC_PARAM);
	pWnd->GetWindowRect(&rect);
	ScreenToClient(&rect);
	m_EditEidolon.MoveWindow(rect);
}

void CDlgAlarmSet::InitStkList()
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
	DrawStkList();
}

BOOL CDlgAlarmSet::OnInitDialog()
{
	CDialog::OnInitDialog();

	Init();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgAlarmSet::OnBnClickedAddStk()
{
	SetDelStockButton(FALSE);
	MSG pMsg;
	pMsg.message = WM_CHAR;
	pMsg.lParam = 0;
	pMsg.wParam = 0;
	SendAddStockMsg(&pMsg);
}

LRESULT CDlgAlarmSet::OnUserDragData(WPARAM wParam, LPARAM lParam)
{
	HSCurKeyInfo* pData = (HSCurKeyInfo*)lParam;
	if(pData == NULL)
		return -1;

	//根据代码获取股票信息  
	StockUserInfo* pStock = new StockUserInfo;
	GetStockByCode(&pData->m_cCode, *pStock);
	if( pStock == NULL )
		return -1;
	AddStock(pStock);

	return 0;
}

BOOL CDlgAlarmSet::AddStock(StockUserInfo* pData)
{
	StockUserInfo* pTemp;
	for(int i = 0; i < m_ayStkList.GetSize(); i++ )
	{
		pTemp = m_ayStkList.GetAt(i);
		if( memcmp(pTemp->m_ciStockCode.m_cCode, pData->m_ciStockCode.m_cCode, sizeof(char) * 6) == 0 )
		{
			MessageBox(_T("已包含股票") + pData->m_ciStockCode.GetCode() + ".", _T("提示..."),MB_ICONINFORMATION);
			return FALSE;
		}
	}
	m_ayStkList.Add(pData); 
	DrawStkList();

	if( m_ctlStockList.GetItemCount() )
	{
		m_ctlStockList.SetFocus();
		ListView_SetItemState(m_ctlStockList.m_hWnd, m_ctlStockList.GetItemCount() - 1,
			LVIS_SELECTED|LVIS_FOCUSED,LVIS_SELECTED|LVIS_FOCUSED);
		SetDelStockButton(TRUE);
	}
	SetOkEnable();
	return TRUE;
}

void CDlgAlarmSet::DelStock(int nIndex)
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
	SetOkEnable();
}

void CDlgAlarmSet::SetOkEnable()
{
	
	CWnd *pWnd = GetDlgItem(IDC_NAME);
	pWnd->GetWindowText(m_strConditionName);

	pWnd = GetDlgItem(IDOK);

	if( m_strConditionName.IsEmpty()|| (m_ayStkList.GetSize() <= 0 && m_CurSBlockName.IsEmpty()) )
	{
		pWnd->EnableWindow(FALSE);
	}
	else
	{
		pWnd->EnableWindow();
	}
}

void CDlgAlarmSet::OnEnChangeName()
{
	SetDelStockButton(FALSE);
	SetOkEnable();
}

long CDlgAlarmSet::LoadCurExp(WPARAM wParam /*= 0*/, LPARAM lParam /*= 0*/)
{	
	if( wParam == 7 )
		return 0;
	CDlgFormulaMan* pParentDlg  = (CDlgFormulaMan*)AfxGetMainWnd();
	CWnd *pWnd = GetDlgItem(IDC_NAME);

	if( wParam == HX_UNION_EXPRESS )
	{
		if( lParam )
		{
			if( m_pUnion == NULL )
				m_pUnion = new UnionSelCondition;
			m_pUnion->Copy((UnionSelCondition*)lParam);
			m_SingOrUnion = FALSE;
			CString strTemp = GetDescribe(m_pUnion);
			m_EditEidolon.SetText(strTemp, NULL);
			m_EditEidolon.Invalidate();
			pWnd->SetWindowText(m_pUnion->m_strName + _T("系统"));
		}
	}
	else if( wParam == HX_EXPRESSSENTENCE )
	{
		if( lParam )
		{
			CExpression* pExpression = (CExpression*)lParam;

			if( m_pSingleSel == NULL )
				m_pSingleSel = new SingleSelCondition;
			m_pSingleSel->m_nType = pExpression->GetExpressType();
			m_pSingleSel->m_strExp.Format("%s",pExpression->GetName());
			switch( m_pSingleSel->m_nType )
			{
			case Condition:
				{
					m_pSingleSel->m_SubData.m_cType = FormulaCompareSubData::Condition;
				}
				break;
			case Exchange:
				{
					m_pSingleSel->m_SubData.m_cType = FormulaCompareSubData::Exchange;
				}
				break;
			}
			m_SingOrUnion = TRUE;

			if( pExpression == NULL )
				return 0;

			if( m_EditEidolon.m_hWnd != NULL )
			{		
				CString strText;
				strText.Format("%s",pExpression->GetEidolon());
				if( !strText.IsEmpty() )
				{
					m_EditEidolon.SetText(strText,pExpression->GetParamVar());
				}
				else
				{
					CStringArray* pArray = pExpression->GetDefEidolon();
					if(pArray->GetSize() <= 1)
					{
						pArray = NULL;
					}
					m_EditEidolon.SetText(pArray,pExpression->GetParamVar());
				}
				m_EditEidolon.Invalidate();
			}
			pWnd->SetWindowText(pExpression->GetName() + _T("系统"));
		}
	}
	m_ctlPeried.SetCurSel(6);
	return 0;
}

void CDlgAlarmSet::OnBnClickedOk()
{
	//名称设定
	CEdit *pWnd = (CEdit*)GetDlgItem(IDC_NAME);
	pWnd->GetWindowText( m_pAlarmData->m_strName );
	
	if( m_bChageOrBuild == FALSE && ::IsWindow(m_hAlarmWnd) && (BOOL)::SendMessage(m_hAlarmWnd, HX_USER_COMPILEDATA, Check_Alarm_Name, (LPARAM)&m_pAlarmData->m_strName) )
	{
		MessageBox(_T("预警名重复,请从新输入!"), _T("提示..."),MB_ICONINFORMATION);
		pWnd->SetSel(0, m_pAlarmData->m_strName.GetLength());
		pWnd->SetFocus();
		return;
	}

	m_pAlarmData->m_uAlarmType = GetType();
	
	//条件设定
	m_pAlarmData->m_SingleOrUnion = m_SingOrUnion;
	if( m_SingOrUnion == FALSE )
	{
		if( m_pUnion != NULL )
			m_pAlarmData->SetUnionCondition(m_pUnion);
		if( m_pSingleSel != NULL )
			delete m_pSingleSel;
		m_pSingleSel = NULL;
	}
	else
	{
		if( m_pSingleSel != NULL )
		{
			m_pSingleSel->m_nPeriod = m_ctlPeried.GetCurSel();//yulx add
			m_pAlarmData->SetCondition(m_pSingleSel);
		}
		if( m_pUnion != NULL )
			delete m_pUnion;
		m_pUnion = NULL;
	}

	//股票设定
	m_pAlarmData->m_strAlarmBlock.Format("%s", m_CurSBlockName);
	m_pAlarmData->CopyStk(&m_ayStkList);

	Free();
	OnOK();
}

void CDlgAlarmSet::OnBnClickedCancel()
{
	if( m_pSingleSel != NULL )
		delete m_pSingleSel;
	m_pSingleSel = NULL;
	if( m_pUnion != NULL )
		delete m_pUnion;
	m_pUnion = NULL;
	OnCancel();
}

void CDlgAlarmSet::OnBnClickedBlokcSel()
{
	m_ayStkList.RemoveAll();
	if( m_ctlBlockSel.GetCheck() )
	{
		GetBlockBoxName();
		SetEnableAddDelButtons(FALSE);
		if( !m_ctlStrBlockList.IsEmpty() )
			SendBlockInfoMsg( m_ctlStrBlockList );
	}
	else
	{
		m_ctlStrBlockList.Empty();
		m_CurSBlockName.Empty();
		SetEnableAddDelButtons(TRUE);
		DrawStkList();
	}
	SetOkEnable();
	SetDelStockButton(FALSE);
}

void CDlgAlarmSet::GetBlockBoxName()
{
	int nCurSel = m_ctlBlockCom.GetCurSel();
	if( nCurSel >= 0 && nCurSel < m_ctlBlockCom.GetCount() )
	{
		m_ctlBlockCom.GetLBText(nCurSel, m_ctlStrBlockList);
	}
	else
	{
		m_ctlStrBlockList.Empty();
	}
	m_CurSBlockName = m_ctlStrBlockList;
}

void CDlgAlarmSet::SetEnableAddDelButtons(BOOL Flag)
{
	//设置加/减股票按钮为flag状态
	CWnd *pWnd = GetDlgItem(IDC_ADD_STK);
	pWnd->EnableWindow(Flag);
	SetDelStockButton(Flag);

	//设置板块列表为~Flag状态
	pWnd = GetDlgItem(IDC_BLOCK_LIST);
	pWnd->EnableWindow( Flag == FALSE ? TRUE : FALSE );

	//如果板块列表有效且列表项不大于0于，初始化板块列表
	if( Flag == FALSE &&  m_ctlBlockCom.GetCount() <= 0 )
			InitBlockName();

	if( Flag == FALSE )
	{
		m_ctlBlockCom.SetCurSel( m_ctlBlockCom.FindString(0, m_ctlStrBlockList) );
		m_ctlBlockCom.SetFocus();
	}
	else
	{
		m_CurSBlockName.Empty();
	}
}

void CDlgAlarmSet::DrawStkList()
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

BOOL CDlgAlarmSet::SaveLoad(UINT nMode,CString strFileName /*= ""*/,DWORD dStyle /*= 0*/)
{
	CFile fp;
	if(strFileName.IsEmpty())
	{
		strFileName =  CGeneralHelper::GetSpecifyPath(CGeneralHelper::SETTING_PATH) +"Union.exp";
	}

	BOOL bOpen = FALSE;
	if(nMode == CArchive::store) // 无存储
	{
		return 0;
	}
	else // 装入
	{
		CFileException fileEx;
		bOpen = fp.Open( strFileName, CFile::typeBinary | CFile::shareDenyNone | 
			CFile::modeReadWrite, &fileEx); 		
		if(!bOpen) 
		{
			return FALSE;
		}

		try
		{
			CArchive ar( &fp, nMode );

			CExpression::HSFormulaFileHead head;
			head.Serialize(ar);
			if( !head.IsValid() )
				return FALSE;

			CTreeCtrlFormula* pCurTree = m_wndTab.GetTree();
			if( pCurTree == NULL )
				return FALSE;

			HTREEITEM hRootItem = pCurTree->GetRootTree(CTreeCtrlFormula::g_strUnionCondition);
			if( hRootItem == NULL )
				return FALSE;

			if(head.m_lCount < 0)
			{
				return FALSE;
			}

			char cStatus;
			HTREEITEM hItem;
			UnionSelCondition* pHSUnion;
			for(int i = 0; i < head.m_lCount; i++)
			{
				ar >> cStatus;
				if( cStatus )
				{
					pHSUnion = new UnionSelCondition;
					pHSUnion->Serialize(ar);

					hItem = pCurTree->InsertItem( pHSUnion->m_strName,
						CExpression::HS_Exp_Union, CExpression::HS_Exp_Union,hRootItem);
					pCurTree->SetItemData(hItem,(DWORD)pHSUnion);
				}
			}

			fp.Close();
		}
#ifdef VC_7_0
		catch( CMemoryException memoryex)
		{
			return FALSE;
		}
		catch( CArchiveException archiveex )
		{
			return FALSE;
		}
		catch( CFileException fileex)
		{
			return FALSE;
		}
#endif
		catch(...)
		{
			return FALSE;
		}
	}

	return TRUE;
}

void CDlgAlarmSet::OnBnClickedDesc()
{
	SetDelStockButton(FALSE);
	CDlgFormulaMan* pParentDlg  = (CDlgFormulaMan*)AfxGetMainWnd();
	if( pParentDlg == NULL )
		return;
	CDlgNotesAndEidolon* pDlgNotes = pParentDlg->GetDlgNotes();
	if( m_SingOrUnion == FALSE )
	{
		CString strTemp = GetDescribe(m_pUnion);
		if( pDlgNotes != NULL )
		{
			pDlgNotes->SetNotes(strTemp, m_pUnion->m_strName);
			pDlgNotes->ShowWindow(SW_SHOW);
		}
		return;
	}
	else
	{
		if( pDlgNotes != NULL )
		{
			pDlgNotes->SetNotes(m_pSingleSel->GetDescribe(), m_pSingleSel->m_strExp);
			pDlgNotes->ShowWindow(SW_SHOW);
		}
		return;
	}
	//CTreeCtrlFormula* pCurTree = m_wndTab.GetTree();
	//if( !pCurTree )
	//	return;
	//HTREEITEM hItem = pCurTree->GetSelectedItem();
	//if( hItem == NULL )
	//	return;
	//
	//CTreeGroup* pTreeGroup = (CTreeGroup*)pCurTree->GetItemData(hItem);
	//if( (pTreeGroup == NULL)			|| 
	//	(pTreeGroup->GetData() == NULL) ||
	//	!pTreeGroup->IsStatus(hx_GroupItem) )
	//	return;

	//CExpression* pExpression = (CExpression*)pTreeGroup->GetData();
	//if( pExpression == NULL )
	//	return;

	//if( pDlgNotes != NULL )
	//{
	//	pDlgNotes->SetNotes(pExpression->GetNotes(), pExpression->GetName());
	//	pDlgNotes->ShowWindow(SW_SHOW);
	//}
}

CString CDlgAlarmSet::GetDescribe(UnionSelCondition* pHSUnion)
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

void CDlgAlarmSet::OnBnClickedDelStk()
{
	if( m_ayStkList.GetSize() > m_nStockIndex )
	{
		DelStock(m_nStockIndex);
		SetDelStockButton(FALSE);
	}
}

void CDlgAlarmSet::OnNMClickStkList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pView = (LPNMLISTVIEW)pNMHDR;
	m_nStockIndex = pView->iItem;
	SetDelStockButton(TRUE);
	*pResult = 0;
}

void CDlgAlarmSet::SetType()
{
	WORD wStyle = m_pAlarmData->m_uAlarmType;
	BOOL bFlag = HS_Block == (wStyle & HS_Block);
	
	if( bFlag == TRUE )
	{
		SetEnableAddDelButtons(FALSE);
	}
	m_ctlBlockSel.SetCheck( bFlag );
	m_ctlDelay.SetCheck( HS_Avi == (wStyle & HS_Avi) );
	m_ctlBox.SetCheck( HS_Box == (wStyle & HS_Box) );
	m_ctlContinue.SetCheck( HS_NoBreak == (wStyle & HS_NoBreak) );
//	m_ctlSell.SetCheck( HS_T0 == (wStyle & HS_T0) ）；

	if( HS_Jg == (wStyle & HS_Jg) )
	{
	}
	else
	{
	}
	
	if( HS_Ts == (wStyle & HS_Ts) )
	{
	}
	else
	{
	}
}

WORD CDlgAlarmSet::GetType()
{
	WORD wStyle = m_pAlarmData->m_uAlarmType;

	if( m_ctlBlockSel.GetCheck() )
	{
		wStyle |= HS_Block;
	}
	else
	{
		wStyle &= ~HS_Block;
	}

	if( m_ctlDelay.GetCheck() )
	{
		wStyle |= HS_Avi;
	}
	else
	{
		wStyle &= ~HS_Avi;
	}

	if( m_ctlBox.GetCheck() )
	{
		wStyle |= HS_Box;
	}
	else
	{
		wStyle &= ~HS_Box;
	}

	if( m_ctlContinue.GetCheck() )
	{
		wStyle |= HS_NoBreak;
	}
	else
	{
		wStyle &= ~HS_NoBreak;
	}
	return wStyle;
}

void CDlgAlarmSet::InitBlockName()
{
	//::SendMessage(g_hParentWnd, HX_USER_COMPILEDATA, 24, (LPARAM)this);
	//获取版块名 need to modify
}

long CDlgAlarmSet::OnGetBlockName(WPARAM wp,LPARAM lp)
{
	// 得到板块名
	if( wp == 0 )
	{
		ASSERT( lp != NULL );
		CStringArray strTemp;
		CStringArray *pData = (CStringArray *)lp;

		strTemp.Copy(*pData);
		m_ctlBlockCom.ResetContent();
		for( int i = 0; i < strTemp.GetSize(); i++ )
		{
			m_ctlBlockCom.AddString( strTemp.GetAt(i) );
		}
	}
	return 0;
}

void CDlgAlarmSet::InitTreeSel()
{
	CValue* valoare;
	CExpression* pExpress = NULL;
	CTreeCtrlFormula* pCurTree = m_wndTab.GetTree();

	if( m_SingOrUnion == TRUE )
	{
		if( CExpression::m_pExternExpression->Lookup( m_pSingleSel->m_strExp,
			m_pSingleSel->m_nType,valoare ) )
		{
			pExpress = valoare->GetExp();

			pCurTree->Show(pExpress,0);
			LoadCurExp(HX_EXPRESSSENTENCE, (LPARAM)pExpress);
		}
	}
	else
	{
	}
}

void CDlgAlarmSet::OnCbnKillfocusBlockList()
{
	//if( m_ctlBlockCom.IsWindowEnabled() )
	//{
	//	m_ctlBlockCom.GetLBText(m_ctlBlockCom.GetCurSel(), m_ctlStrBlockList);
	//	m_CurSBlockName.Format("%s", m_ctlStrBlockList);
	//}
}

void CDlgAlarmSet::OnCbnSetfocusBlockList()
{
	if( m_ctlBlockCom.IsWindowEnabled() )
	{
		int i = 0;
		GetBlockBoxName();
		while( m_CurSBlockName.IsEmpty() && i < m_ctlBlockCom.GetCount() )
		{
			m_ctlBlockCom.SetCurSel(i++);
			m_ctlBlockCom.GetLBText(m_ctlBlockCom.GetCurSel(), m_ctlStrBlockList);
			m_CurSBlockName.Format("%s", m_ctlStrBlockList);
		}
		if( !m_CurSBlockName.IsEmpty() )
		{
			SendBlockInfoMsg(m_CurSBlockName);
			DrawStkList();
		}
	}
}

void CDlgAlarmSet::Free()
{
	if( m_pUnion != NULL && m_SingOrUnion == TRUE )
	{
		delete m_pUnion;
		m_pUnion = NULL;
	}

	if( m_pSingleSel != NULL && m_SingOrUnion == FALSE )
	{
		delete m_pSingleSel;
		m_pSingleSel = NULL;
	}
	
	m_ayStkList.RemoveAll();
}

BOOL CDlgAlarmSet::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class
	if( pMsg && (pMsg->message == WM_KEYDOWN || pMsg->message == WM_CHAR) )
	{
		CWnd* pWnd = this->GetFocus();
		int nID = pWnd->GetDlgCtrlID();
		CRect wndRc, IdRc;
		CWnd* pIdWnd;
		pIdWnd = GetDlgItem(IDC_PARAM);
		pIdWnd->GetWindowRect(&IdRc);
		pWnd->GetWindowRect(&wndRc);
		CPoint poing(wndRc.left, wndRc.top);

		if( !IdRc.PtInRect(poing) && 
			!m_ctlBlockSel.GetCheck() && 
			nID != IDC_NAME && 
			SendAddStockMsg(pMsg))
		{
			return TRUE;
		}
	}
	return CDialog::PreTranslateMessage(pMsg);
}

BOOL CDlgAlarmSet::SendAddStockMsg(MSG *pMsg)
{
	AlarmMsgData srtData;
	srtData.pWnd = this;
	srtData.pMsg = pMsg;
	//加入股票 need to modify
	//return (BOOL)::SendMessage(g_hParentWnd, HX_USER_COMPILEDATA, 23, (LPARAM)&srtData);
	return TRUE;
}

void CDlgAlarmSet::OnBnClickedSet()
{
	SetDelStockButton(FALSE);
	CDlgAlarmParamSet dlg((void*)m_pAlarmData);
	dlg.DoModal();
}

BOOL CDlgAlarmSet::SendBlockInfoMsg(CString strBlockName)
{
	BlockInfo BlockInfo;
	BlockInfo.strBlockName = strBlockName;
	//根据版块名获取股票列表  need to modify
	int nCount = (int)::SendMessage(g_hParentWnd, HX_USER_COMPILEDATA, 31, (LPARAM)&strBlockName);
	BlockInfo.ayStockList.SetSize( nCount );
	if((BOOL)::SendMessage(g_hParentWnd, HX_USER_COMPILEDATA, 30, (LPARAM)&BlockInfo) )
	{
		//m_ayStkList.Copy( BlockInfo.ayStockList );
		return TRUE;
	}
	else
	{
		MessageBox(_T("由于未知的原因,不能得到此板块股票信息,请另选!^_^"), _T("提示..."),MB_ICONINFORMATION);
		return FALSE;
	}
}

void CDlgAlarmSet::OnCbnSelchangeBlockList()
{
	GetBlockBoxName();
	if( !m_CurSBlockName.IsEmpty() )
		SendBlockInfoMsg(m_CurSBlockName);
	DrawStkList();
	SetDelStockButton(FALSE);
}

BOOL CDlgAlarmSet::SetDelStockButton(BOOL bFlag)
{
	CWnd* pAddWnd = GetDlgItem(IDC_ADD_STK);
	CWnd* pWnd = GetDlgItem(IDC_DEL_STK);

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

void CDlgAlarmSet::OnNMSetfocusStkList(NMHDR *pNMHDR, LRESULT *pResult)
{
	SetDelStockButton(TRUE);
	*pResult = 0;
}

void CDlgAlarmSet::OnNMKillfocusStkList(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here
	//SetDelStockButton(FALSE);
	*pResult = 0;
}
