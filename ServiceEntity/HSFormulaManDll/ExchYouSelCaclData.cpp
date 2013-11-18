#include "stdafx.h"
#include "exchyouselcacldata.h"
#include "SheetExchMain.h"
/*#include "DlgStockList.h"*/

//CExchCalcMain* CExchYouSelCaclData::m_pCaclMain = NULL;
UINT CExchYouSelCaclData::s_uTheradStateEx = TCaclThread_State_Stoping;
extern HWND			 g_hParentWnd;

CExchYouSelCaclData::CExchYouSelCaclData(void)
{
	m_lTestObj = 0;
	m_lTestCycle = 0;
	m_bUseAllLocalData = TRUE;
	m_lBeginDate = 0;
	m_lCount = 0;
	m_nCurCaclIdx = -1;
	m_pPageStock = NULL;
}

CExchYouSelCaclData::~CExchYouSelCaclData(void)
{
}

void CExchYouSelCaclData::CopyExch(CTreeCtrlFormula* pTree)
{
	if( pTree == NULL )
		return;

	m_ayListCtrl.RemoveAll();
	CArray<CTreeGroup*,CTreeGroup*>* pAyItem = pTree->GetTreeGroup();
	if( pAyItem == NULL )
		return;

	CTreeGroup* pItem = NULL;
	for( int i = 0; i < pAyItem->GetSize(); i++ )
	{
		pItem = pAyItem->GetAt(i);
		if( pItem == NULL || pItem->GetData() == NULL )
		{
			continue;
		}
		m_ayListCtrl.Add(pItem->GetData());
	}
}

// 指标或交易系统公式管理
void CExchYouSelCaclData::ClearExpression() 
{
	m_ayExpress.RemoveAll();
}

void CExchYouSelCaclData::AddExpression(CExpression* pExpression)
{
	m_ayExpress.Add(pExpression);
}

BOOL CExchYouSelCaclData::SetSelectAll(CListCtrl *pCtrl/* = NULL*/, BOOL bSelCheck/* = TRUE*/)
{
	if( pCtrl == NULL )
	{
		pCtrl = GetListCtrl();
		if( pCtrl == NULL )
			return false;
	}
	else
	{
		SetListCtrl(pCtrl);
	}

	for(int i = 0; i < pCtrl->GetItemCount(); i++ )
	{
		pCtrl->SetCheck(i, bSelCheck);
	}

	return TRUE;
}

BOOL CExchYouSelCaclData::SetExpressionFromCtrl(CListCtrl *pCtrl/* = NULL*/)
{
	if( pCtrl == NULL )
	{
		pCtrl = GetListCtrl();
		if( pCtrl == NULL )
			return false;
	}
	else
	{
		SetListCtrl(pCtrl);
	}

	ClearExpression();
	for(int i = 0; i < pCtrl->GetItemCount(); i++ )
	{
		if( pCtrl->GetCheck(i) )
		{
			LVITEM lvItem;
			//char	chTemp[MAX_PATH];
			//ZeroMemory(chTemp, sizeof(chTemp));
			lvItem.iItem = i;
			//lvItem.iSubItem = 0;
			//lvItem.pszText = chTemp;
			lvItem.mask = LVIF_PARAM;//IF_TEXT;
			pCtrl->GetItem(&lvItem);
			CExpression* pExp = (CExpression*)lvItem.lParam;
			if( pExp )
			{
				AddExpression(pExp);
			}
		}
	}

	return TRUE;
}

void CExchYouSelCaclData::DelExpression(int nIdx)
{
	m_ayExpress.RemoveAt(nIdx);
}

CArray<CExpression*, CExpression*>* CExchYouSelCaclData::GetAyExpression()
{
	return &m_ayExpress;
}

CMapStringToPtr* CExchYouSelCaclData::GetMapResult()
{
	return &m_mapCaclResult;
}

// 优化结果数据管理
void CExchYouSelCaclData::ClearResult()
{
	POSITION pos = m_mapCaclResult.GetStartPosition();
	while( pos != NULL )
	{
		void* rValue;
		CString sKey;

		m_mapCaclResult.GetNextAssoc(pos, sKey, rValue);

		if( rValue != NULL )
		{
			delete (TExchCaclResult*)rValue;
			rValue = NULL;
		}
	}

	m_mapCaclResult.RemoveAll();
}

TExchCaclResult* CExchYouSelCaclData::GetResult(CExpression* pExpression, BOOL bNew)
{
	if( pExpression == NULL )
		return NULL;

	void *pVoid = NULL;
	if( m_mapCaclResult.Lookup(pExpression->GetName(), pVoid) )
	{
		return (TExchCaclResult*)pVoid;
	}
	else if( bNew )
	{
		TExchCaclResult* pInfo = new TExchCaclResult;
		if( pInfo )
		{
			if( AddResult(pExpression, pInfo) )
			{
				pVoid = pInfo;
			}
			else
			{
				delete pInfo;
			}
		}
	}

	return (TExchCaclResult*)pVoid;
}

int CExchYouSelCaclData::GetExpressIdx(CExpression* pExp)
{
	if( pExp == NULL )
		return -1;

	for( int i = 0; i < m_ayListCtrl.GetSize(); i++ )
	{
		CExpression* pItem = m_ayListCtrl.GetAt(i);
		if( pItem )
		{
			if( pExp == pItem || pItem->GetName().CompareNoCase(pExp->GetName()) == 0 )
			{
				return i;
			}
		}
	}

	return -2;
}

BOOL CExchYouSelCaclData::IsHave(CExpression* pExpression)
{
	if( pExpression == NULL )
		return false;

	void *pVoid;
	if( m_mapCaclResult.Lookup(pExpression->GetName(), pVoid) )
	{
		return (BOOL)pVoid;
	}

	return false;
}

BOOL CExchYouSelCaclData::AddResult(CExpression* pExpression, TExchCaclResult* pResult)
{
	if( pExpression == NULL || pResult == NULL )
		return false;
	
	if( !IsHave(pExpression) )
	{
		TExchCaclResult* pNew = new TExchCaclResult;
		if( pNew == NULL )
			return false;

		*pNew = *pResult;
		m_mapCaclResult.SetAt(pExpression->GetName(), pNew);
		return true;
	}

	return false;
}

BOOL CExchYouSelCaclData::DelResult(CExpression* pExpression)
{
	if( pExpression == NULL )
		return false;

	void *pVoid;
	if( m_mapCaclResult.Lookup(pExpression->GetName(), pVoid) )
	{
		delete (TExchCaclResult*)pVoid;
		m_mapCaclResult.RemoveKey(pExpression->GetName());
	}

	return true;
}

void CExchYouSelCaclData::InitCtrl(CListCtrl *pCtrl)
{
	if( pCtrl == NULL )
		return;

	SetListCtrl(pCtrl);
	pCtrl->DeleteAllItems();
	while(pCtrl->DeleteColumn(0));
	pCtrl->SetExtendedStyle(LVS_EX_CHECKBOXES | LVS_EX_FULLROWSELECT | pCtrl->GetExtendedStyle());

	CHAR _tagColName[][10] = {"交易名称",HS_EXCH_CACLRESULT_STOCK_HEADLE_PARAM	
		,HS_EXCH_CACLRESULT_STOCK_HEADLE_PROFIT	
		,HS_EXCH_CACLRESULT_STOCK_HEADLE_YEAR	
		,HS_EXCH_CACLRESULT_STOCK_HEADLE_WIN	
		,HS_EXCH_CACLRESULT_STOCK_HEADLE_DEALNUM
		,HS_EXCH_CACLRESULT_STOCK_HEADLE_SUCC};

	int _tabColWidth[] = { 80, 80, 80, 80, 80, 80, 80 };

	LV_COLUMN	lvcol;
	lvcol.mask=LVCF_FMT|LVCF_SUBITEM|LVCF_TEXT|LVCF_WIDTH;
	lvcol.fmt=LVCFMT_CENTER;//居中

	CString str;
	for(int i = 0; i < _countof(_tagColName); i ++)
	{
		str = _T(_tagColName[i]);

		lvcol.pszText = (char*)LPCTSTR(str);//_tagColName[i];
		lvcol.iSubItem = i;
		lvcol.cx = _tabColWidth[i];
		pCtrl->InsertColumn(i,&lvcol);
		lvcol.fmt=LVCFMT_RIGHT;
	}
}

// 显示结果
int  CExchYouSelCaclData::Display(CListCtrl *pCtrl)
{ 
	if( pCtrl == NULL )
	{
		pCtrl = GetListCtrl();
		if( pCtrl == NULL )
			return TCaclReturn_Fail;
	}
	else
	{
		SetListCtrl(pCtrl);
	}

	pCtrl->DeleteAllItems();
	for( int i = 0; i < m_ayListCtrl.GetSize(); i++ )
	{
		CExpression* pItem = m_ayListCtrl.GetAt(i);
		Display(pCtrl, pItem);
	}
	return TCaclReturn_Succ; 
}

int  CExchYouSelCaclData::Display(CListCtrl *pCtrl, CExpression* pExpression, int nIdx)
{ 
	if( pExpression == NULL )
		return TCaclReturn_Fail;

	if( pCtrl == NULL )
	{
		pCtrl = GetListCtrl();
		if( pCtrl == NULL )
			return TCaclReturn_Fail;
	}
	else
	{
		SetListCtrl(pCtrl);
	}

	LVITEM lvItem;
	char chText[MAX_PATH];
	ZeroMemory(chText, sizeof(chText));
	ZeroMemory(&lvItem, sizeof(lvItem));

	lvItem.mask = LVIF_STATE|LVIF_TEXT|LVIF_PARAM;
	lvItem.iSubItem = 0;
	lvItem.pszText = chText;
	lvItem.lParam = (LPARAM)pExpression;
	sprintf(chText, "%s", pExpression->GetName());
	if( nIdx == -1 )
	{
		lvItem.iItem = pCtrl->GetItemCount();
		pCtrl->InsertItem(&lvItem);
	}
	else
	{
		lvItem.iItem = nIdx;
	}

	TExchCaclResult* pResult = GetResult(pExpression);
	if( pResult != NULL )
	{
		for(int i = 1; i < HS_EXCH_CACLRESULT_STOCK_ID_SUCC+1; i++ )
		{
			lvItem.iSubItem = i;
			if( HS_EXCH_CACLRESULT_STOCK_ID_PARAM == i )
			{
				// 最优参数
				sprintf(chText, "%s", pResult->GetCurParam()->ToStr());
				pCtrl->SetItem(&lvItem);
			}
			else
			{
				sprintf(chText, "%.02f", pResult->GetIDData(i));
				pCtrl->SetItem(&lvItem);
			}
		}
	}

	return TCaclReturn_Succ; 
}

CArray<StockUserInfo*,StockUserInfo*>* CExchYouSelCaclData::GetAyStock()
{
	return &m_ayStock;
}

// 计算参数管理
void CExchYouSelCaclData::SetDate(BOOL bUseAll, int nBegin/* = 0*/, int nCount/* = 0*/)
{
	m_bUseAllLocalData = bUseAll;
	if( !bUseAll )
	{
		m_lBeginDate = nBegin;
		m_lCount = nCount;
	}
}

// 股票操作
void CExchYouSelCaclData::AddStock(StockUserInfo* pInfo) 
{ 
	if( pInfo && IsHave(pInfo) < 0 ) 
		m_ayStock.Add(pInfo); 
}

void CExchYouSelCaclData::DelStock(StockUserInfo* pInfo)
{
	int iIdx = IsHave(pInfo);
	if( iIdx < 0 )
		return;

	m_ayStock.RemoveAt(iIdx);
}

StockUserInfo* CExchYouSelCaclData::GetStock(int nIdx)
{
	if( nIdx < 0 && nIdx >= m_ayStock.GetSize() )
		return NULL;

	return m_ayStock.GetAt(nIdx);
}

// -1则表求没有
int CExchYouSelCaclData::IsHave(StockUserInfo* pInfo)
{
	if( pInfo == NULL )
		return -1;
	for( int i = 0; i < m_ayStock.GetSize(); i++ )
	{
		StockUserInfo* pItem = m_ayStock.GetAt(i);
		if( pItem == NULL )
			continue;
		if( pItem == pInfo )
			return i;

		if( memcmp(&pItem->m_ciStockCode, &pInfo->m_ciStockCode, sizeof(pItem->m_ciStockCode)) == 0 )
			return i;
	}

	return -1;
}

CString CExchYouSelCaclData::ReturnMsgPress(LPARAM lpID) 
{
	switch( lpID )
	{
	case TCaclReturn_NoData		:
		return _T(_T("计算失败:没有用于计算的数据!"));
	case TCaclReturn_VainParam	:
		return _T(_T("计算失败:传入无效的参数!"));
	case TCaclReturn_NoExperss	:
		return _T(_T("计算失败:没有指定公式或公式不正确!"));
	case TCaclReturn_CalFail	:
		return _T(_T("计算失败:单支股票的计算不成功!"));
	case TCaclReturn_MoneryFail	:
		return _T(_T("计算失败:系统内存不足或其它内存错误!"));
	case TCaclReturn_TotalFail	:
		return _T(_T("计算失败:对各股票评测结果的统计不成功!"));
	case TCaclReturn_Stop		:
		return _T(_T("提示:计算过程被停止!"));
	case TCaclReturn_Fail		:
		return _T(_T("计算失败:未知的原因!"));
	}

	return "";
}

// 不用开新线程
LRESULT CExchYouSelCaclData::StartEx()
{
	if( CExchYouSelCaclData::StopEx() != TCaclReturn_Succ )
		return TCaclReturn_Fail;

	CExchYouSelCaclData::s_uTheradStateEx = TCaclTherad_State_Start;
	m_lCurProgress = 0;	
	m_dProgressUint = 100.0 / (m_ayExpress.GetSize() == 0 ? 1 : m_ayExpress.GetSize());
	CaclNext();

	return TCaclReturn_Succ;    
}

int CExchYouSelCaclData::ConvertToGlobe(int nIdx)
{
	if( nIdx < 0 || m_ayExpress.GetSize() >= nIdx )
		return -1;

	CExpression* pExp = m_ayExpress.GetAt(nIdx);
	if( pExp == NULL )
		return -1;

	CExpression* pItem;
	for( int i = 0; i < m_ayListCtrl.GetSize(); i++ )
	{
		pItem = m_ayListCtrl.GetAt(i);
		if( pItem != NULL )
		{
			if( pItem == pExp || pExp->GetName().CompareNoCase(pItem->GetName()) == 0 )
				return i;
		}
	}

	return -1;
}

// 不用开新线程
LRESULT CExchYouSelCaclData::StopEx()
{
	if( TCaclTherad_State_Start != CExchYouSelCaclData::s_uTheradStateEx )
	{
		return TCaclReturn_Succ;
	}

	if( Stop() != TCaclReturn_Succ )
		return TCaclReturn_Fail;

	int iStop = 100;
	while( iStop > 0 )
	{
		iStop--;
		CExchYouSelCaclData::s_uTheradStateEx =	TCaclThread_State_Stop;
		if( TCaclTherad_State_Start != CExchYouSelCaclData::s_uTheradStateEx )
			break;
		Sleep(10);
	}

	return TCaclReturn_Succ;
}

LRESULT CExchYouSelCaclData::OnExchangeData(WPARAM wp,LPARAM lp)
{
	switch( wp )
	{
		// 一个指标进度通知
	case EXCH_MSG_WP_CACL_TEMPO	:
		m_lCurProgress += (long)(m_dProgressUint * lp * 1.0 / 100.0);
		if( GetProgress() )
		{
			GetProgress()->SetPos(m_lCurProgress);
		}
		break;
		// 一个指标开始
	case EXCH_MSG_WP_CACL_START	: // 设置当前计算焦点
		{
			int iCur = ConvertToGlobe(m_nCurCaclIdx);
			if( iCur < 0 )
				return TCaclReturn_Fail;
		}
		break; 
		// 一个指标停止// 优选停止
	case EXCH_MSG_WP_CACL_STOP	:
	case YouSel_MSG_WP_CACL_STOP: 
		ClearData();
		// 停止处理

		break;
		// 一个指标失败
	case EXCH_MSG_WP_CACL_FAIL	: // 进行下一计算
		{
			ClearData();
			// 进行下一个指标计算
			CaclNext(m_nCurCaclIdx);
		}
		break;
		// 一个指标完成
	case EXCH_MSG_UP_CACL_SUCC	: // 设置一行数据 在换调 用中显示更新
		{
			CExpression* pExp = GetExp();
			if( pExp == NULL )
				return TCaclReturn_Fail;
			AddResult(pExp, GetObjResultItem((UINT)GetTestObj()));
			ClearData();
			int iIdx = GetExpressIdx(pExp);
			if( iIdx < 0 )
				return TCaclReturn_Fail;
			Display(NULL,pExp, iIdx);
			// 进行下一个指标计算
			CaclNext(m_nCurCaclIdx);
		}
		break;
		// 优选开始
	case YouSel_MSG_WP_CACL_START : // 设置状态
		break;
		// 优选失败
	case YouSel_MSG_WP_CACL_FAIL : // 失败处理
		break;
		// 优先完成
	case YouSel_MSG_UP_CACL_SUCC : // 完成处理
		break;
	}

	return 0;
}

LRESULT CExchYouSelCaclData::CaclNext(int nIdx/* = -1*/)
{
	nIdx++;
	int nExpSize = m_ayExpress.GetSize();
	// 发送开始优选消息
	if( nIdx == 0 && nExpSize > 0)
	{
		SendMsg(YouSel_MSG_WP_CACL_START, 0);
	}
	// 如果是最后一个，则发送完成优选消息
	else if( nExpSize <= nIdx ) 
	{
		SendMsg(YouSel_MSG_UP_CACL_SUCC, 0);
		return TCaclReturn_Succ;
	}

	// 停止计算
	if( CExchYouSelCaclData::s_uTheradStateEx != TCaclTherad_State_Start )
	{
		CExchYouSelCaclData::s_uTheradStateEx = TCaclThread_State_Stoping;
		SendMsg(YouSel_MSG_WP_CACL_STOP, 0);
		return TCaclReturn_Succ;
	}

	// 得到下一个公式
	CExpression* pExp = m_ayExpress.GetAt(nIdx);
	while( pExp == NULL )
	{
		if( ++nIdx < nExpSize )
		{
			pExp = m_ayExpress.GetAt(++nIdx);
		}
		else
		{
			SendMsg(YouSel_MSG_UP_CACL_SUCC, 0);
			return TCaclReturn_Succ;
		}
	}

	m_nCurCaclIdx = nIdx;

	// 设置公式
	SetExpress(pExp);
	// 设置参数
	SetOptimize(pExp, TRUE);
	if( m_pPageStock )
	{
		m_ayStock.Add(m_pPageStock);
	}
	// 设置股票
	CopyStock(m_ayStock);
	if( m_pPageStock )
	{
		m_ayStock.RemoveAt(m_ayStock.GetSize()-1);
	}

	// 开始计算
	return Start();
}

// 设置默认测试策略
void CExchYouSelCaclData::SetDefTact()
{
	// 清除所有非原来股票
	if( m_pPageStock == NULL )
	{
		if( m_ayStock.GetSize() > 0 )
		{
			m_pPageStock = m_ayStock.GetAt(0);
		}
		else
		{
			if( ::IsWindow(g_hParentWnd) )
				m_pPageStock = (StockUserInfo*)::SendMessage(g_hParentWnd, HX_USER_COMPILEDATA, 71, 0);
		}
	}

	m_ayStock.RemoveAll();
}

// 设置自定义测试策略 // 加股票
void CExchYouSelCaclData::SetCommTact()
{
// 	CDlgStockList dlg(CWnd::FromHandle(GetMsgWnd()));
// 	dlg.SetStock(m_pPageStock, m_ayStock);
// 	if( dlg.DoModal() == IDOK )
// 	{
// 		dlg.CopyStockTo(m_ayStock);
// 	}
}

BOOL CExchYouSelCaclData::OpenTableWnd(UINT uID/* = 0*/)
{
	// 打开报表
	if( m_dlgTable == NULL )
	{
		CWnd* pWnd = CWnd::FromHandle(GetMsgWnd());
		CreageTable(pWnd);
	}
	m_dlgTable->ShowData(uID);

	return TRUE;
}

BOOL CExchYouSelCaclData::CreageTable(CWnd* pParent)
{
	DescTable();

	// 新建报表
	if( m_dlgTable == NULL )
	{
		m_dlgTable = new CDlgSingleExchCalcResult(pParent);
		m_dlgTable->Initial(this);
		m_dlgTable->CenterWindow(pParent);
	}

	return true;
}

BOOL CExchYouSelCaclData::DescTable()
{
	if( m_dlgTable != NULL )
	{
		if( ::IsWindow(m_dlgTable->m_hWnd) )
		{
			m_dlgTable->DestroyWindow();
		}
		delete m_dlgTable;
	}
	m_dlgTable = NULL;

	return TRUE;
}
