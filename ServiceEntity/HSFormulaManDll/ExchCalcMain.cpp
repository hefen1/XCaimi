#include "stdafx.h"
#include "ExchCalcMain.h"
#include "TreeCtrlFormula.h"
#include "DlgFormulaMan.h"
#include "Express.h"
#include "RegMsg.h"
#include "GeneralHelper.h"
#include <io.h>

#define EXCH_SETTING_INI	"exchsetting.cfg"
CExchCalcMain* CExchCalcMain::s_pThis = NULL;
CThreadStatusMutex CExchCalcMain::s_uTheradState;// = TCaclThread_State_Stoping;

extern HWND g_hParentWnd;
extern double EXCH_POUNDAGE;
extern BOOL GetStockByCode(CodeInfo* pInfo, StockUserInfo& stockInfo );
extern IDataSourceEx*   g_pDataSource = NULL;
// �õ���Ʊ��Ϣ
void GetStockInfoProc(CodeInfo sCode, StockUserInfo& stockInfo)
{
	if ( g_pDataSource )
		 g_pDataSource->HSDataSourceEx_GetStockUserInfo(&sCode, stockInfo);
}

//�õ�С��λ
//long GetStockPriceDecimal(CodeInfo* sType)
//{
//	if( !::IsWindow(g_hParentWnd) )
//		return 2;
//
//	return ::SendMessage(g_hParentWnd, HX_USER_COMPILEDATA, 
//		HS_COMPILE_DATA_WPARAM_GetStockPriceUnit, (LPARAM)sType);
//}
//
//�õ���Ʊ��λ
//long GetStockPriceUint(HSMarketDataType sType)
//{
//	if( !::IsWindow(g_hParentWnd) )
//		return 1;
//
//	return ::SendMessage(g_hParentWnd, HX_USER_COMPILEDATA, 68, (LPARAM)sType);
//}

/*************************************************************
*	�� �� �ƣ�	CThreadStatusMutex
*	�� �� ����	�߳�״̬
***************************************************************/
CThreadStatusMutex::CThreadStatusMutex()
{
	m_lStatus = TCaclThread_State_Stoping;
}

// ����ֵ
BOOL CThreadStatusMutex::SetVal(long lVal)
{
	if( m_mutex.Lock(3000) == FALSE )
	{
		//WRITE_LOG (__FILE__, __LINE__, "����ʧ��");
		return FALSE;
	}
	
	m_lStatus = lVal;

	m_mutex.Unlock();
	return TRUE;
}

// ������
BOOL CThreadStatusMutex::IsRun()
{
	if( m_mutex.Lock(3000) == FALSE )
	{
		//WRITE_LOG (__FILE__, __LINE__, "����ʧ��");
		return FALSE;
	}
	
	BOOL bRet = (m_lStatus == TCaclTherad_State_Start);

	m_mutex.Unlock();
	return bRet;
}

// ֹͣ
BOOL CThreadStatusMutex::IsStopping()
{
	if( m_mutex.Lock(3000) == FALSE )
	{
		//WRITE_LOG (__FILE__, __LINE__, "����ʧ��");
		return FALSE;
	}
	
	BOOL bRet = (m_lStatus == TCaclThread_State_Stoping);

	m_mutex.Unlock();
	return bRet;
}

CThreadStatusMutex& CThreadStatusMutex::operator=(long lVal)
{
	SetVal(lVal);
	return *this;
}

BOOL CThreadStatusMutex::operator==(long lVal)
{
	if( m_mutex.Lock(3000) == FALSE )
	{
		//WRITE_LOG (__FILE__, __LINE__, "����ʧ��");
		return FALSE;
	}
	
	BOOL bRet = (m_lStatus == lVal);

	m_mutex.Unlock();
	return bRet;
}

BOOL CThreadStatusMutex::operator!=(long lVal)
{
	if( m_mutex.Lock(3000) == FALSE )
	{
		//WRITE_LOG (__FILE__, __LINE__, "����ʧ��");
		return FALSE;
	}
	
	BOOL bRet = (m_lStatus != lVal);

	m_mutex.Unlock();
	return bRet;
}
/*************************************************************
*	�� �� �ƣ�	CExchCalcMain
*	�� �� ����	������������
***************************************************************/
CExchCalcMain::CExchCalcMain()
{
	CExchCalcMain::s_pThis = this;
	m_cIsApplySeting = 1;
	m_oImage.Create(IDB_EXCH_IMAGE, 16, 2, RGB(255, 255, 255));
	// �������� 2003-08-19 ����
	ReadSetting();
}

CExchCalcMain::~CExchCalcMain()
{
	s_pThis = NULL;
	// ��������
	WriteSetting();
	ClearData();
	if ( GetMode() )
		GetMode()->Clear();
}

/******************************************************************
*	�������ܣ�	��������
*	����������	���ޣ�
*	�� �� ֵ��	BOOL 
*	����������	
*	��    �ߣ�	
*	������ڣ�	
*	�޸�˵����	
*****************************************************************/
BOOL CExchCalcMain::ReadSetting()
{
	CString strFile =  CGeneralHelper::GetSpecifyPath(CGeneralHelper::SETTING_PATH) + EXCH_SETTING_INI;
	CFile fp;
	try
	{
		if( _access(strFile, 0) == -1 || fp.Open(strFile, CFile::modeReadWrite) != TRUE )
		{
			return FALSE;
		}
		CArchive ar(&fp, CArchive::load);

		TExchExp* pExp = GetExchExp();
		TExchModel* pModel = GetMode();
		TExchBuy* pBuy = GetBuy();
		TExchSell* pSell = GetSell();
		TExchOptimize* pOpt = GetOptimize();	

		if( pExp == NULL || pModel == NULL || pBuy == NULL || pSell == NULL 
			|| pOpt == NULL )
		{
			ar.Close();
			fp.Close();
			return FALSE;
		}

		pExp->Serialize(ar, CDlgFormulaMan::m_pExternExpression);
		pBuy->Serialize(ar);
		pSell->Serialize(ar);
		pModel->m_ProcGetStockUser = (GETSTOCKINFOPROC)GetStockInfoProc;
		pModel->Serialize(ar);
		pOpt->Serialize(ar);

		ar.Close();
		fp.Close();
		return TRUE;
	}
#ifdef VC_7_0
	// �쳣����
	catch(CMemoryException e) //���ڴ��쳣
	{
		TCHAR   szCause[255];
		e.GetErrorMessage(szCause, 255);
		CString strError;
		strError.Format("Read seting MemoryException: %s", szCause);
		// ������־
		//WRITE_LOG (__FILE__, __LINE__, _T(strError));
	}
	catch(CFileException e) // �ļ��쳣
	{
		TCHAR   szCause[255];
		e.GetErrorMessage(szCause, 255);
		CString strError;
		strError.Format("Read seting FileException: %s", szCause);
		// ������־
		//WRITE_LOG (__FILE__, __LINE__, _T(strError));
	}
	catch(CArchiveException e) // �����쳣
	{
		TCHAR   szCause[255];
		e.GetErrorMessage(szCause, 255);
		CString strError;
		strError.Format("Read seting ArchiveException: %s", szCause);
		// ������־
		//WRITE_LOG (__FILE__, __LINE__, _T(strError));
	}
#endif
	catch(...) // �����쳣
	{
		// ������־
		CString strError;
		strError.Format("Read seting other Exception: %s", ::strerror(::GetLastError()));
		//WRITE_LOG (__FILE__, __LINE__, _T(strError));
	}

	return FALSE;
}

/******************************************************************
*	�������ܣ�	д������
*	����������	���ޣ�
*	�� �� ֵ��	BOOL 
*	����������	
*	��    �ߣ�	
*	������ڣ�	2003��8��19��
*	�޸�˵����	
*****************************************************************/
BOOL CExchCalcMain::WriteSetting()
{
	CString strFile = CGeneralHelper::GetSpecifyPath(CGeneralHelper::FACE_PATH) + EXCH_SETTING_INI;
	CFile fp;
	try
	{
		if( fp.Open(strFile, CFile::modeReadWrite|CFile::modeCreate) != TRUE )
		{
			return FALSE;
		}
		CArchive ar(&fp, CArchive::store);

		TExchExp* pExp = GetExchExp();
		TExchModel* pModel = GetMode();
		TExchBuy* pBuy = GetBuy();
		TExchSell* pSell = GetSell();
		TExchOptimize* pOpt = GetOptimize();	

		if( pExp == NULL || pModel == NULL || pBuy == NULL || pSell == NULL 
			|| pOpt == NULL )
		{
			ar.Close();
			fp.Close();
			return FALSE;
		}

		pExp->Serialize(ar);
		pBuy->Serialize(ar); 
		pSell->Serialize(ar);
		pModel->Serialize(ar);
		pOpt->Serialize(ar);
		ar.Close();
		fp.Close();

		return TRUE;
	}
#ifdef VC_7_0
	// �쳣����
	catch(CMemoryException e) //���ڴ��쳣
	{
		TCHAR   szCause[255];
		e.GetErrorMessage(szCause, 255);
		CString strError;
		strError.Format("Exch : Write seting MemoryException: %s", szCause);
		// ������־
		//WRITE_LOG (__FILE__, __LINE__, _T(strError));
	}
	catch(CFileException e) // �ļ��쳣
	{
		TCHAR   szCause[255];
		e.GetErrorMessage(szCause, 255);
		CString strError;
		strError.Format("Exch : Write seting FileException: %s", szCause);
		// ������־
		//WRITE_LOG (__FILE__, __LINE__, _T(strError));
	}
	catch(CArchiveException e) // �����쳣
	{
		TCHAR   szCause[255];
		e.GetErrorMessage(szCause, 255);
		CString strError;
		strError.Format("Exch : Write seting ArchiveException: %s", szCause);
		// ������־
		//WRITE_LOG (__FILE__, __LINE__, _T(strError));
	}
#endif
	catch(...) // �����쳣
	{
		// ������־
		CString strError;
		strError.Format("Exch : Write seting other Exception: %s", ::strerror(::GetLastError()));
		//WRITE_LOG (__FILE__, __LINE__, _T(strError));
	}


	return FALSE;
}

LRESULT	CExchCalcMain::SendMsg(WPARAM wp, LPARAM lp)
{
	if( s_pThis == NULL )
		return TCaclReturn_Fail;

	return s_pThis->m_sMsg.SendMsg(wp, lp);
}

// ��ʼ���б�
BOOL CExchCalcMain::InitialOptimizeCtrl(CListCtrl* pCtrl, CImageList* pImage)
{
	if( pCtrl == NULL )
		return false;

	pCtrl->DeleteAllItems();
	while(pCtrl->DeleteColumn(0));

	if( pImage != NULL )
	{
		pCtrl->SetImageList(pImage, LVSIL_SMALL);
	}
	else
	{
		pCtrl->SetImageList(&m_oImage, LVSIL_SMALL);
	}

	pCtrl->SetExtendedStyle(pCtrl->GetExtendedStyle()|LVS_EX_FULLROWSELECT|LVS_EX_FLATSB);
	int i = 0;
	pCtrl->DeleteAllItems();
	while( pCtrl->DeleteColumn(0) && i++ < 300 );

	TCHAR _tagColName[][10] = {HS_EXCH_CACLRESULT_STOCK_HEADLE_PARAM,
		HS_EXCH_CACLRESULT_STOCK_HEADLE_PROFIT	,
		HS_EXCH_CACLRESULT_STOCK_HEADLE_YEAR	,
		HS_EXCH_CACLRESULT_STOCK_HEADLE_WIN	,	
		HS_EXCH_CACLRESULT_STOCK_HEADLE_DEALNUM,
		HS_EXCH_CACLRESULT_STOCK_HEADLE_SUCC	
	};

	int _tabColWidth[] = { 100, 60, 60, 60, 60, 60 };

	LV_COLUMN	lvcol;
	lvcol.mask = LVCF_FMT|LVCF_SUBITEM|LVCF_TEXT|LVCF_WIDTH;
	lvcol.fmt = LVCFMT_CENTER;//����

	for(i = 0; i < _countof(_tagColName); i ++)
	{
		lvcol.pszText = _tagColName[i];
		lvcol.iSubItem = i;
		lvcol.cx = _tabColWidth[i];
		pCtrl->InsertColumn(i,&lvcol);
		lvcol.fmt = LVCFMT_RIGHT;
	}

	return TRUE;
}

// ��ʾ�б�
BOOL CExchCalcMain::ToOptimizeCtrl(CListCtrl* pCtrl)
{
	if( pCtrl == NULL || CExchCalcMain::s_uTheradState != TCaclThread_State_Stoping )
		return false;

	TExchCaclResult* pItem;

	for( int i = 0; i < m_sCaclResult.GetSize(); i++ )
	{
		pItem = m_sCaclResult.GetAt(i);
		if( pItem != NULL )
		{
			pItem->ToListCtrl(pCtrl);
		}
	}
	return TRUE;
}

void CExchCalcMain::SetApplySeting(BOOL bSave)
{
	if( bSave )
	{
		m_cIsApplySeting = 1;
	}
	else
	{
		m_cIsApplySeting = 0;
	}
}

BOOL CExchCalcMain::GetApplySeting()
{
	return (m_cIsApplySeting == 1);
}

BOOL CExchCalcMain::SetData(TExchHead* pData)
{
	if( pData == NULL || GetApplySeting() == FALSE )
		return true;
	switch( pData->m_bType )
	{
	case EXCH_DATA_TYPE_Exp	:
		return SetExp(pData);
		break;
	case EXCH_DATA_TYPE_Mode:
		return SetMode(pData);
		break;
	case EXCH_DATA_TYPE_Buy	:
		return SetBuy(pData);
		break;
	case EXCH_DATA_TYPE_Sell:
		return SetSell(pData);
		break;
	case EXCH_DATA_TYPE_Optimize:
		return SetOptimize(pData);
		break;
	}
	return FALSE;
}

BOOL CExchCalcMain::SetExp(TExchHead* pData)
{
	if( pData == NULL || pData->m_bType != EXCH_DATA_TYPE_Exp )
		return false;

	TExchExp* pExp = (TExchExp*)pData;
	TExchExp* pDescExp = GetExchExp();
	if( pDescExp == NULL )
		return false;

	*pDescExp = *pExp;
	return TRUE;
}

BOOL CExchCalcMain::SetOptimize(CExpression* pPress, BOOL bValid)
{
	TExchOptimize* pOptimize = GetOptimize();
	if( pPress == NULL || pOptimize == NULL )
		return false;

	return pOptimize->ConvertFromExpress(pPress, bValid);
}

BOOL CExchCalcMain::SetExpress(CExpression* pPress)
{
	if( pPress == NULL ) return false;
	TExchExp* pDescExp = GetExchExp();

	if( pDescExp == NULL )
		return false;
	pDescExp->m_pMainTech = pPress;
	return TRUE;
}

BOOL CExchCalcMain::SetMode(TExchHead* pData)
{
	if( pData == NULL || pData->m_bType != EXCH_DATA_TYPE_Mode )
		return false;

	TExchModel* pExp = (TExchModel*)pData;
	TExchModel* pDescExp = GetMode();
	*pDescExp = *pExp;
	// �����ʽ���
	TDealFlowStock::m_dAllMoney = pDescExp->m_lAllInvestCount;
	TDealFlowStock::s_dLastMonery = pDescExp->m_lAllInvestCount;

	return TRUE;
}

BOOL CExchCalcMain::SetBuy(TExchHead* pData)
{
	if( pData == NULL || pData->m_bType != EXCH_DATA_TYPE_Buy )
		return false;

	TExchBuy* pExp = (TExchBuy*)pData;
	TExchBuy* pDescExp = GetBuy();
	*pDescExp = *pExp;

	// �����Ƿ�ǿ��ƽ�ּ���ͳ��
	TDealFlowStock::m_bLastSell = (BOOL)(pDescExp->m_bNoCountIn == 0);

	return TRUE;
}

BOOL CExchCalcMain::SetSell(TExchHead* pData)
{
	if(pData == NULL || pData->m_bType != EXCH_DATA_TYPE_Sell )
		return false;

	TExchSell* pExp = (TExchSell*)pData;
	TExchSell* pDescExp = GetSell();
	*pDescExp = *pExp;

	return TRUE;
}

BOOL CExchCalcMain::SetOptimize(TExchHead* pData)
{
	if( pData == NULL || pData->m_bType != EXCH_DATA_TYPE_Optimize )
		return false;

	TExchOptimize* pExp = (TExchOptimize*)pData;
	TExchOptimize* pDescExp = GetOptimize();
	*pDescExp = *pExp;

	return TRUE;
}

// ���ƹ�Ʊ�б�
void CExchCalcMain::CopyStock(CArray<StockUserInfo*,StockUserInfo*>& sStock)
{
	CArray<StockUserInfo*, StockUserInfo*>* pAyStock = GetStockList();
	if( pAyStock == NULL )
		return;
	pAyStock->Copy(sStock);
}

// ���б��еõ���nIdx֧��Ʊ
StockUserInfo* CExchCalcMain::GetStockAt(int nIdx)
{
	CArray<StockUserInfo*, StockUserInfo*>* pAyStock = GetStockList();
	if( pAyStock == NULL || nIdx >= pAyStock->GetSize() || nIdx < 0 )
		return NULL;
	return pAyStock->GetAt(nIdx);
}

// �õ���Ʊ��
long CExchCalcMain::GetStockNum()
{
	CArray<StockUserInfo*, StockUserInfo*>* pAyStock = GetStockList();
	return pAyStock->GetSize();
}

long CExchCalcMain::CaclValues(TCaclTemp& sCaclTemp)
{
	if( ::IsWindow(g_hParentWnd) )
	{
		return ::SendMessage(g_hParentWnd, HX_USER_COMPILEDATA,
								66, (LPARAM)&sCaclTemp);
	}
	else
	{
		return TCaclReturn_Fail;
	}
}

void CExchCalcMain::PreDeal(CodeInfo* pCode, StockDay* pDay)
{
	if( s_pThis == NULL || pDay == NULL )
		return;
	TDealFlowStock* pFlow = s_pThis->GetLastFlow(pCode);
	if( pFlow == NULL )
		return;
	long lCount = pFlow->GetCount();
	//if( pFlow->m_lLastBuy >= lCount )
	//	return;
	for( int i = lCount-1; i >= 0; i-- )
	{
		TDealFlowItem* pItem = pFlow->GetItem(i);
		if( pItem == NULL )
			continue;
		if( pItem->m_wDealType == TDealFlow_DT_Buy )
		{
			pItem->m_lCaclDays++;
			double dMax = (pDay->m_lMaxPrice * 1.0 / s_pThis->m_nPriceUnit - pItem->m_dDealPrice ) * pItem->m_lDealAmount;
			double dMin = (pDay->m_lMinPrice * 1.0 / s_pThis->m_nPriceUnit - pItem->m_dDealPrice ) * pItem->m_lDealAmount;
			pFlow->m_dMaxFDProfit = max(pFlow->m_dMaxFDProfit, dMax);
			pFlow->m_dMaxFDDificit = min(pFlow->m_dMaxFDDificit, dMin);
		}
		else
		{
			break;
		}
	}
}

long CExchCalcMain::CaclFail(CodeInfo* pCode)
{
	if( s_pThis == NULL )
		return TCaclReturn_Fail;
	TDealFlowStock* pFlow = s_pThis->GetFlowFormCode(pCode);
	if( pFlow == NULL )
		return TCaclReturn_Fail;
	
	return TCaclReturn_Succ;
}
// ���붯��(������)
BOOL CExchCalcMain::BeginBuyEndSell(CodeInfo* pCode, StockDay* pData, BOOL bBuy, int iCyc)
{
	if( s_pThis == NULL || pCode == NULL || pData == NULL || pData->m_lDate <= 19700101 || pData->m_lDate >= 22000101 )
		return FALSE;
	TDealFlowStock* pFlow = s_pThis->GetFlowFormCode(pCode);
	if( pFlow == NULL )
		return FALSE;

	if( bBuy )
	{
		pFlow->m_dBeginPrice = pData->m_lClosePrice * 1.0 / s_pThis->m_nPriceUnit;
	}
	else
	{
		pFlow->m_dEndPrice = pData->m_lClosePrice * 1.0 / s_pThis->m_nPriceUnit;
		pFlow->m_lTestCycNum = iCyc;
	}

	return TRUE;
}
// ���붯��
BOOL CExchCalcMain::BuyStock(CodeInfo* pCode, StockDay* pData, int iDay, BOOL bNewBuy)
{
	if( s_pThis == NULL || pCode == NULL || pData == NULL || pData[iDay].m_lDate <= 19700101 || pData[iDay].m_lDate >= 22000101 )
		return FALSE;

	TDealFlowStock* pFlow = s_pThis->GetFlowFormCode(pCode);
	if( pFlow == NULL )
		return FALSE;
	TExchBuy *pBuy = s_pThis->GetBuy();
	if( pBuy == NULL )
		return FALSE;

	TDealFlowItem* pPreItem = pFlow->GetLastItem();

	long lMaxNum = s_pThis->GetBuyStockMaxNum();
	double dLastMoney = s_pThis->GetOverplusMoney(pCode);
	double dJumpInMoney;
	long nStkNum = 0;
	double dClose = pData[iDay].m_lClosePrice  * 1.0 / s_pThis->m_nPriceUnit;
	if( dClose <= 0.0001 )
		dClose = 1;

	if( bNewBuy || pPreItem == NULL || pPreItem->m_wDealType != TDealFlow_DT_Buy )
	{
		switch( pBuy->m_nBuyMethod )
		{
		case BuyMethod_All_Buy: // ȫ������
			nStkNum = (long)(dLastMoney / dClose) / lMaxNum;
			dJumpInMoney = nStkNum * dClose;
			break;
		case BuyMethod_Portion_Buy:// ��������
			dJumpInMoney = dLastMoney * pBuy->m_fOneBuyCount / 100.0 / lMaxNum;
			nStkNum = (long)(dJumpInMoney / dClose);
			break;
		case BuyMethod_Fixation_Money:// �̶��ʽ�����
			dJumpInMoney = pBuy->m_fOneBuyCount > dLastMoney ? 0 : pBuy->m_fOneBuyCount / lMaxNum;
			nStkNum = (long)(dJumpInMoney / dClose);
			break;
		case BuyMethod_Fixation_NUM:// �̶���Ʊ������
			nStkNum = (pBuy->m_fOneBuyCount*dClose > dLastMoney) ? 0 : pBuy->m_fOneBuyCount / lMaxNum;
			dJumpInMoney = nStkNum * dClose;
			break;
		}
	}
	else
	{
		dJumpInMoney = pPreItem->m_dDealPrice * pPreItem->m_lDealAmount;
		switch( pBuy->m_nFollowBuyMethod )
		{
		case FollowBuyMethod_NO: // ��������
			dJumpInMoney = 0;
			break;
		case FollowBuyMethod_DL: // ��������
			dJumpInMoney = min(dJumpInMoney, dLastMoney);
			break;
		case FollowBuyMethod_DJ: // �ݼ�����
			dJumpInMoney *= (1 - pBuy->m_fAddOrSubCount/100);
			dJumpInMoney = min(dJumpInMoney, dLastMoney);
			break;
		case FollowBuyMethod_DZ: // ��������
			dJumpInMoney *= (1 + pBuy->m_fAddOrSubCount/100);
			dJumpInMoney = min(dJumpInMoney, dLastMoney);
			break;
		}
		nStkNum = (long)(dJumpInMoney / dClose);
	}

	// ��������
	nStkNum /= 100;
	nStkNum *= 100;
	dJumpInMoney = nStkNum*dClose;
	if( nStkNum > 0 )
	{
		TDealFlowItem* pItem = pFlow->Append();
		if( pItem == NULL )
			return FALSE;

		pItem->m_dDealProfit = 0;
		pItem->m_lCaclDays = 0;
		pItem->m_wDealType = TDealFlow_DT_Buy;
		pItem->m_lDealDate = pData[iDay].m_lDate;

		pItem->m_dDealPrice = dClose;
		pItem->m_lDealAmount = nStkNum;
		SetLastMonery(dJumpInMoney, CExchCalcMain::MoneryDel, pCode);
		pFlow->m_lLastStockNum += pItem->m_lDealAmount;
		pFlow->m_dJumpInMoney += dJumpInMoney;

		if( bNewBuy )
		{
			s_pThis->AddBuyStock(pCode);
		}
		
		return true;
	}

	return false;
}

// ��������
BOOL CExchCalcMain::SellStock(CodeInfo* pCode, StockDay* pData, int iDay, int nMode)
{
	if( s_pThis == NULL || pData[iDay].m_lDate <= 19700101 || pData[iDay].m_lDate >= 22000101)
		return FALSE;
	if( pData[iDay].m_lDate <= 0 )
		return FALSE;

	TDealFlow* pFlow = s_pThis->GetDealFlow();
	if( pFlow == NULL )
		return FALSE;

	TDealFlowStock* pStockItem = pFlow->GetFlowFormCode(pCode);
	if( pStockItem == NULL || pStockItem->m_lLastStockNum <= 0 )
		return FALSE;

	TExchSell* pSell = s_pThis->GetSell();  
	if( pSell == NULL )
		return FALSE;

	long lCount = pStockItem->GetCount();
	
	TDealFlowItem* pItem = pStockItem->Append();
	if( pItem == NULL )
		return FALSE;

	double dClose = pData[iDay].m_lClosePrice * 1.0 / s_pThis->m_nPriceUnit;
	double dGetMoney = dClose * pStockItem->m_lLastStockNum;
	pItem->m_wDealType = nMode;
	pItem->m_dDealPrice = dClose;
	pItem->m_lDealAmount = pStockItem->m_lLastStockNum;
	pItem->m_lDealDate = pData[iDay].m_lDate;
	pItem->m_dDealProfit = dGetMoney*( 1 - EXCH_POUNDAGE) - pStockItem->m_dJumpInMoney*(1+EXCH_POUNDAGE);

	long lValidCount = 0, lCycTotal = 0;
	for( ; lCount >= 0; lCount-- )
	{
		TDealFlowItem* pPreItem = pStockItem->GetItem(lCount);
		if( pPreItem != NULL && pPreItem->m_wDealType == TDealFlow_DT_Buy )
		{
			lValidCount++;
			lCycTotal += pPreItem->m_lCaclDays;
		}
		
	}
	if( lValidCount == 0 )
		lValidCount = 1;
	pItem->m_lCaclDays = lCycTotal / lValidCount;

	pStockItem->m_lLastBuy = pStockItem->GetCount();
	pStockItem->m_lLastStockNum = 0;
	SetLastMonery(dGetMoney, CExchCalcMain::MoneryAdd, pCode);
	pStockItem->m_dJumpInMoney = 0;

	s_pThis->DelBuyStock(pCode);

	return TRUE;
}

void CExchCalcMain::SetLastMonery(double dMonery, int nMask, CodeInfo* pCode/* = NULL*/)
{
	if( s_pThis == NULL )
		return;
	TDealFlowStock *pStock = s_pThis->GetLastFlow(pCode);
	TExchModel* pModel = s_pThis->GetMode();
	if( pStock == NULL || pModel == NULL )
		return;

	double* pData = NULL;
	double* pDateEx = NULL;
	if( pModel->m_bSingle )
	{
		pData = &pStock->m_lLastMoney;
	}
	else
	{
		pData = &pStock->s_dLastMonery;
		pDateEx = &pStock->m_lLastMoney;
	}

	if( pData == NULL )
		return;

	switch( nMask )
	{
	default:
	case MoneryAdd:
		dMonery *= (1-EXCH_POUNDAGE);
		*pData += dMonery;
		if( pDateEx ) *pDateEx += dMonery;
		break;
	case MoneryDel:
		dMonery *= (1+EXCH_POUNDAGE);
		*pData -= dMonery;
		if( pDateEx ) *pDateEx -= dMonery;
		break;
	case MonerySet:
		*pData = dMonery;
		if( pDateEx ) *pDateEx = dMonery;
		break;
	}
}

long CExchCalcMain::Fail(UINT msg)
{
	int iStop = 100;
	CExchCalcMain::s_uTheradState = TCaclThread_State_Stoping;
	while( iStop-- > 0 && CExchCalcMain::s_uTheradState != TCaclThread_State_Stoping )
	{
		CExchCalcMain::s_uTheradState = TCaclThread_State_Stoping;
		Sleep(200);
	}
	if( s_pThis != NULL )
	{
		s_pThis->ClearData();
		//s_pThis->ResetDefParam();
	}
	SendMsg(EXCH_MSG_WP_CACL_FAIL, (LPARAM)msg);
	return msg;
}



long CExchCalcMain::CaclStop()
{
	//int iStop = 100;
	//while( iStop-- > 0 && CExchCalcMain::s_uTheradState != TCaclThread_State_Stoping )
	//{
	//	CExchCalcMain::s_uTheradState = TCaclThread_State_Stoping;
	//	Sleep(3);
	//}
	if( s_pThis != NULL )
		s_pThis->ClearData();
	SendMsg(EXCH_MSG_WP_CACL_STOP, 0);
	return TCaclReturn_Stop;
}

// ����Ʊ����
long CExchCalcMain::SingleStockTest(int iCurParamIdx, int iCount)
{
	if( s_pThis == NULL )
		return TCaclReturn_NoData;

	// ������������
	TExchBuy* pBuy		= s_pThis->GetBuy();
	// ��ʽ����
	CExpression *pExp	= s_pThis->GetExp();
	// ��ʽ����������
	TExchExp* pSelExp	= s_pThis->GetExchExp();
	// ����ģ���������
	TExchModel* pModel	= s_pThis->GetMode();
	if( pBuy == NULL || pExp == NULL || pModel == NULL )
	{
		//WRITE_LOG (__FILE__, __LINE__, "�������ݲ���ȷ");
		return TCaclReturn_NoData;
	}

	if( iCount <= 0 )
		iCount = 1;
	// �Դ������
	double dPressBase = 1.0 * iCurParamIdx / iCount * 100;
	// һ��֧�괦���ܽ���
	double dPressUint = 1.0 / iCount * 100;
	// �����м����
	TCaclTemp sCaclTemp;
	// ָ�����ĸĽ�
	sCaclTemp.lCountDate = pBuy->m_lDateCount;
	sCaclTemp.lBeginDate = pBuy->m_lBeginDate;
	sCaclTemp.nAnaPeriod = pSelExp->m_lCyc;
	sCaclTemp.bExRights  = pSelExp->m_bExRights;
	sCaclTemp.New();
	if( sCaclTemp.pRetCacl == NULL )
	{
		return TCaclReturn_MoneryFail;
	}
	sCaclTemp.lEndDate = pBuy->m_lEndDate;
	sCaclTemp.pPress = pExp;
	if( pBuy->m_strTechLeft.Compare(CTreeCtrlFormula::g_strFirstTechLine) != 0 )
	{
		sCaclTemp.szTechMain = pBuy->m_strTechLeft;
	}
	else
	{
		sCaclTemp.szTechMain.Empty();
	}
	sCaclTemp.szTechSideLeft = pBuy->m_SubData.m_dData1;
	sCaclTemp.szTechSideRitht = pBuy->m_SubData.m_dData2;

	// ��Ч������
	int iValidNum = 0;
	// ��Ʊ��
	int iStockNum = s_pThis->GetStockNum();
	// һ֧��Ʊ��ռ���ȳ�
	float fSectorization = dPressUint / iStockNum;
	// ������ȼ�ǰһ����
	int iTempo, iPreTempo = -1;
	// �ź���
	long lTotalSignal = 0;
	// ǰһ����״̬
	long lPreState = 0; // 0 ����, 1 ����

	// ��Ʊ
	for( int j = 0; j < iStockNum; j++ )
	{
		if( CExchCalcMain::s_uTheradState == TCaclThread_State_Stop )
			return TCaclReturn_Stop;//CaclStop();

		StockUserInfo* pStock = s_pThis->GetStockAt(j);	
		if( pStock == NULL )
			continue;

		// ��ʼ���ɽ�����
		s_pThis->GetFlowFormCode(&pStock->m_ciStockCode);

		sCaclTemp.pCode = &pStock->m_ciStockCode;
		
		long lRet = CaclValues(sCaclTemp);

		if( CExchCalcMain::s_uTheradState == TCaclThread_State_Stop )
			return TCaclReturn_Stop;//CaclStop();

		if(  lRet != TCaclReturn_Succ || sCaclTemp.pRetCacl->m_lDescLen <= 0 )
		{
			// ������, ����ֵ
			CaclFail(&pStock->m_ciStockCode);
			continue;
		}
		// �й�Ʊ(����)
		BOOL bHaveStock = FALSE;
		int iMode;

		// С��
		s_pThis->m_nDecimal = pStock->m_nDecimal;
		// ��λ
		s_pThis->m_nPriceUnit = pStock->m_nPriceUnit;

		BeginBuyEndSell(&pStock->m_ciStockCode, sCaclTemp.pRetDiskData, TRUE);
		BeginBuyEndSell(&pStock->m_ciStockCode, sCaclTemp.pRetDiskData + sCaclTemp.pRetCacl->m_lDescLen - 1, FALSE, sCaclTemp.pRetCacl->m_lDescLen);

		BOOL bContinue;

		for( int day = 0; day < sCaclTemp.pRetCacl->m_lDescLen; day++ )
		{
			if( CExchCalcMain::s_uTheradState == TCaclThread_State_Stop )
				return TCaclReturn_Stop;//CaclStop();

			PreDeal(&pStock->m_ciStockCode, &sCaclTemp.pRetDiskData[day]);

			bContinue = TRUE;
			// ����
			if( IsBuy(&sCaclTemp, day) )
			{
				if( CExchCalcMain::s_uTheradState == TCaclThread_State_Stop )
					return TCaclReturn_Stop;//CaclStop();

				// �ź���
				lTotalSignal++;

				// �����ƱԤ����
				PreBuyStock(&pStock->m_ciStockCode, sCaclTemp.pRetDiskData, day, sCaclTemp.pRetCacl->m_lDescLen);

				// �õ�ʣ���Ǯ����һ�ֹ�Ʊʱ
				double dOverplusMoney = s_pThis->GetOverplusMoney();
				if( dOverplusMoney >= (sCaclTemp.pRetDiskData[day].m_lClosePrice  * 100.0 / s_pThis->m_nPriceUnit) )	
				{
					if( lPreState <= 0 )
					{
						lPreState = 1;
						BuyStock(&pStock->m_ciStockCode, sCaclTemp.pRetDiskData, day, TRUE);
						bHaveStock = TRUE;
						bContinue = FALSE;
					}
					else
					{
						lPreState++;
						// ����������������
						if( !pBuy->m_bSkip || (pBuy->m_bSkip  && lPreState >= pBuy->m_nInPeriod) )
						{
							BuyStock(&pStock->m_ciStockCode, sCaclTemp.pRetDiskData, day, FALSE);
							bHaveStock = TRUE;
							bContinue = FALSE;
						}
					}
				}
			}

			// ���� 
			if( 0 < (iMode = IsSell(&sCaclTemp, day))  )
			{
				if( CExchCalcMain::s_uTheradState == TCaclThread_State_Stop )
					return TCaclReturn_Stop;//CaclStop();

				lPreState = 0;
				lTotalSignal++;

				if( bHaveStock /*&& bContinue*/ )
				{
					SellStock(&pStock->m_ciStockCode, sCaclTemp.pRetDiskData, day, iMode);
					bHaveStock = FALSE;
				}
			}
			// ���ʹ������
			iTempo = dPressBase + (j*dPressUint/iStockNum) + (long)(day*fSectorization/sCaclTemp.pRetCacl->m_lLeftLen);
			if( iTempo != iPreTempo )
			{
				SendMsg(EXCH_MSG_WP_CACL_TEMPO, (LPARAM)iTempo);
				iPreTempo = iTempo;
			}
		}

		if( CExchCalcMain::s_uTheradState == TCaclThread_State_Stop )
			return TCaclReturn_Stop;//CaclStop();
		// ǿ��ƽ��
		if( bHaveStock )
		{
			SellStock(&pStock->m_ciStockCode, sCaclTemp.pRetDiskData, sCaclTemp.pRetCacl->m_lDescLen-1, TDealFlow_DT_QZPC);
		}

		// �����ź�����
		TDealFlowStock* pFlow = s_pThis->GetFlowFormCode(&pStock->m_ciStockCode);
		if( pFlow != NULL )
			pFlow->m_lSignalNum = lTotalSignal;

		// ��Ч��Ʊ(��ȷ����)��
		iValidNum++;

		// ���ʹ������
		iTempo = dPressBase + (j+1)*dPressUint/iStockNum;
		if( iTempo != iPreTempo )
		{
			SendMsg(EXCH_MSG_WP_CACL_TEMPO, (LPARAM)iTempo);
			iPreTempo = iTempo;
		}
	}
	// ͳ��
	if( Total() )
	{
		return TCaclReturn_Succ;
	}
	if( CExchCalcMain::s_uTheradState == TCaclThread_State_Stop )
		return TCaclReturn_Stop;//CaclStop();

	return TCaclReturn_TotalFail;
}

// ȫ�г�����
long CExchCalcMain::EntireMarketTest(int iCurParamIdx, int iCount)
{
	if( s_pThis == NULL )
		return TCaclReturn_NoData;

	// ������������
	TExchBuy* pBuy		= s_pThis->GetBuy();
	// ��ʽ����
	CExpression *pExp	= s_pThis->GetExp();
	// ��ʽ����������
	TExchExp* pSelExp	= s_pThis->GetExchExp();
	// �����г�ģ��
	TExchModel* pModel	= s_pThis->GetMode();
	if( pBuy == NULL || pExp == NULL || pModel == NULL )
	{
		return TCaclReturn_NoData;
	}

	if( iCount <= 0 )
		iCount = 1;
	// �Դ������
	double dPressBase = 1.0 * iCurParamIdx / iCount * 100;
	// һ��֧�괦���ܽ���
	double dPressUint = 1.0 / iCount * 100;

	const double dOneStop = 0.3;

	// �����м����
	TCaclTemp sCaclTemp;
	// ָ�����ĸĽ�
	sCaclTemp.lCountDate = pBuy->m_lDateCount;
	sCaclTemp.lBeginDate = pBuy->m_lBeginDate;
	sCaclTemp.nAnaPeriod = pSelExp->m_lCyc;
	sCaclTemp.bExRights  = pSelExp->m_bExRights;
	sCaclTemp.New();
	if( sCaclTemp.pRetCacl == NULL )
	{
		return TCaclReturn_MoneryFail;
	}
	sCaclTemp.lEndDate = pBuy->m_lEndDate;
	sCaclTemp.pPress = pExp;
	if( pBuy->m_strTechLeft.Compare(CTreeCtrlFormula::g_strFirstTechLine) != 0 )
	{
		sCaclTemp.szTechMain = pBuy->m_strTechLeft;
	}
	else
	{
		sCaclTemp.szTechMain.Empty();
	}
	sCaclTemp.szTechSideLeft = pBuy->m_SubData.m_dData1;
	sCaclTemp.szTechSideRitht = pBuy->m_SubData.m_dData2;
	// ��Ч�����Ʊ��
	int iValidNum = 0;
	// ��Ʊ��
	int iStockNum = s_pThis->GetStockNum();
	// ������ȼ�ǰһ����
	int iTempo = 0, iPreTempo = -1;
	// �ź�ͳ��
	long* lTotalSignal;
	long* lBuySignal;
	// ǰһ����״̬
	long lPreState = 0; // 0 ����, 1 ����

	CArray<TCaclTemp*, TCaclTemp*> ayCaclTemp;
	TCaclTemp* pItem;
	// ���ָ�곤��
	long	lMaxTechLen = -1;
	// ��Ʊ,���㵥������ָ������
	int j;
	for(  j = 0; j < iStockNum; j++ )
	{
		if( CExchCalcMain::s_uTheradState == TCaclThread_State_Stop )
			return TCaclReturn_Stop;//CaclStop();

		StockUserInfo* pStock = s_pThis->GetStockAt(j);	
		if( pStock == NULL )
			continue;

		sCaclTemp.pCode = &pStock->m_ciStockCode;

		long lRet = CaclValues(sCaclTemp);
		
		if(  lRet != TCaclReturn_Succ && sCaclTemp.pRetCacl->m_lDescLen <= 0 )
		{
			// ��ʼ����, ����ֵ
			CaclFail(&pStock->m_ciStockCode);
			continue;
		}
		lMaxTechLen = max(sCaclTemp.pRetCacl->m_lDescLen, lMaxTechLen);
  		pItem = new TCaclTemp(sCaclTemp);
  		if( pItem != NULL )
  		{
  			ayCaclTemp.Add(pItem);
  		}
		
		iTempo = dPressBase + j*dPressUint/iStockNum*dOneStop;
		if( iPreTempo != iTempo )
		{
			SendMsg(EXCH_MSG_WP_CACL_TEMPO, (LPARAM)iTempo);
			iPreTempo = iTempo;
		}
	}// ָ�����ݼ������

	if( lMaxTechLen <= 0 )
	{
		return TCaclReturn_NoData;
	}
	
	// ��ʼ��������
	long layCaclTempCount = ayCaclTemp.GetSize();
	// ��ָ�����������ѭ��
	char*	lpsBufOff;
	char*	lpsHaveStock;

	lpsBufOff = new char[layCaclTempCount];
	lpsHaveStock = new char[layCaclTempCount];
	lTotalSignal = new long[layCaclTempCount];
	lBuySignal = new long[layCaclTempCount];

	if( lpsBufOff == NULL || lpsHaveStock == NULL 
		|| lTotalSignal == NULL || lBuySignal == NULL )
	{
		if( lpsBufOff != NULL )
		{
			delete[] lpsBufOff;
		}
		if( lpsHaveStock != NULL )
		{
			delete[] lpsHaveStock;
		}
		if( lTotalSignal != NULL )
		{
			delete[] lTotalSignal;
		}
		if( lBuySignal != NULL )
		{
			delete[] lBuySignal;
		}
		return TCaclReturn_MoneryFail;
	}

	ZeroMemory(lpsHaveStock, sizeof(char)*layCaclTempCount);
	ZeroMemory(lpsBufOff, sizeof(char)*layCaclTempCount);
	ZeroMemory(lTotalSignal, sizeof(long)*layCaclTempCount);
	ZeroMemory(lBuySignal, sizeof(long)*layCaclTempCount);

	int iMode;
	int iDay, iItem;
	for( iDay = 0; iDay < lMaxTechLen; iDay++ )
	{
		// ��ָ�������(����Ʊ)ѭ��
		for(  iItem = 0; iItem < layCaclTempCount; iItem++ )
		{
			if( CExchCalcMain::s_uTheradState == TCaclThread_State_Stop )
				return TCaclReturn_Stop;//CaclStop();

			pItem = ayCaclTemp.GetAt(iItem);
			if( pItem == NULL || pItem->pRetCacl->m_lDescLen <= iDay)
				continue;
			// С��
			int index = -1;
			s_pThis->m_nDecimal = GetStockPriceDecimal(pItem->pCode->GetCode(), index);
			// ��λ
			s_pThis->m_nPriceUnit = GetStockPriceUnit(pItem->pCode->GetCode(), index);

			BeginBuyEndSell(pItem->pCode, pItem->pRetDiskData, TRUE);
			BeginBuyEndSell(pItem->pCode, pItem->pRetDiskData + pItem->pRetCacl->m_lDescLen - 1, FALSE, pItem->pRetCacl->m_lDescLen);
			PreDeal(pItem->pCode, &pItem->pRetDiskData[iDay]);

			lpsBufOff[iItem] = 0;
			// ����
			if( 0 < (iMode = IsSell(pItem, iDay)) )
			{
				lTotalSignal[iItem]++;
				lpsBufOff[iItem] = iMode;
			}

			// ����
			if( IsBuy(pItem, iDay) )
			{
				lTotalSignal[iItem]++;
				lBuySignal[iItem]++;
				// �����ƱԤ����
				PreBuyStock(pItem->pCode, pItem->pRetDiskData, iDay, pItem->lCountDate);

				// �õ�ʣ���Ǯ����һ֧��Ʊʱ
				double dOverplusMoney = s_pThis->GetOverplusMoney();
				if( dOverplusMoney >= (pItem->pRetDiskData[iDay].m_lClosePrice  * 1.0 / s_pThis->m_nPriceUnit) )	
				{
					lpsBufOff[iItem] = TDealFlow_DT_Buy;
				}
			}
		}

		// ����
		long lCurBuyNum;
		lCurBuyNum = 0;
	
		for( iItem = 0; iItem < layCaclTempCount; iItem++ )
		{
			if( CExchCalcMain::s_uTheradState == TCaclThread_State_Stop )
				return TCaclReturn_Stop;//CaclStop();

			pItem = ayCaclTemp.GetAt(iItem);
			if( pItem == NULL || pItem->pRetCacl->m_lDescLen <= iDay)
				continue;
			// �п����ź�
			if( lpsBufOff[iItem] == TDealFlow_DT_Buy && CheckEntireIsBuy(pModel, pItem->pCode, lCurBuyNum) )
			{
				if( lpsHaveStock[iItem] != 0 )
				{
					// ����������������
					if( !pBuy->m_bSkip || (pBuy->m_bSkip  && ((int)lpsHaveStock[iItem]) >= pBuy->m_nInPeriod) )
					{
						BuyStock(pItem->pCode, pItem->pRetDiskData, iDay, FALSE);
						lCurBuyNum ++;
						lpsHaveStock[iItem] += 1;
					}
				}
				else
				{
					if( BuyStock(pItem->pCode, pItem->pRetDiskData, iDay, TRUE) )
					{
						lpsHaveStock[iItem] = 1;
					}
					lCurBuyNum++;
				}
			}
			// �п����ź�
			else if( lpsBufOff[iItem] != 0 && lpsHaveStock[iItem] != 0 )
			{
				SellStock(pItem->pCode, pItem->pRetDiskData, iDay, (int)lpsBufOff[iItem]);
				lpsHaveStock[iItem] = 0;
			}
			// �������
		}

		// ���ʹ������
		iTempo = dPressBase + j*dPressUint/lMaxTechLen*(1-dOneStop);
		if( iTempo != iPreTempo )
		{
			SendMsg(EXCH_MSG_WP_CACL_TEMPO, (LPARAM)iTempo);
			iPreTempo = iTempo;
		}
	}// ������������

	// �����ź�����
	TDealFlow* pFlow = s_pThis->GetDealFlow();
	if( pFlow == NULL )
		return FALSE;
	for( iDay = 0; iDay < layCaclTempCount; iDay++ )
	{
		pItem = ayCaclTemp.GetAt(iDay);
		if( pItem == NULL )
			continue;

		TDealFlowStock* pStockItem = pFlow->GetFlowFormCode(pItem->pCode);
		if( pStockItem == NULL )
			continue;

		pStockItem->m_lSignalNum = lTotalSignal[iDay];
		//pStockItem->m_lBuySignalNum = lBuySignal[iDay];
	}

	// ǿ��ƽ��
	for( int iPC = 0; iPC < layCaclTempCount; iPC++ )
	{
		if( lpsHaveStock[iPC] != 0 )
		{
			TCaclTemp* pItem = ayCaclTemp.GetAt(iPC);
			if( pItem == NULL )
				continue;
			// С��
			int index = -1;
			s_pThis->m_nDecimal = GetStockPriceDecimal(pItem->pCode->GetCode(), index); //GetStockPriceDecimal(pItem->pCode);
			// ��λ
			s_pThis->m_nPriceUnit = GetStockPriceUnit(pItem->pCode->GetCode(), index);

			SellStock(pItem->pCode, pItem->pRetDiskData, pItem->pRetCacl->m_lDescLen-1, TDealFlow_DT_QZPC);
		}
	}
	
	// ��ʼ��ռ����������
	for( j = 0; j < ayCaclTemp.GetSize(); j++ )
	{
		pItem = ayCaclTemp.GetAt(j);
		if( pItem != NULL )
		{
			pItem->Free(TRUE);
			delete pItem;
		}
	}
	if( lpsHaveStock != NULL )
		delete[] lpsHaveStock;
	if( lpsBufOff != NULL )
		delete[] lpsBufOff;
	if( lTotalSignal != NULL )
		delete[] lTotalSignal;
	if( lBuySignal != NULL )
		delete[] lBuySignal;
	// ��ռ���������ݽ���

	// ͳ��
	if( Total() )
		return TCaclReturn_Succ;
	return TCaclReturn_TotalFail;
}

void CExchCalcMain::AddBuyStock(CodeInfo* pInfo)
{
	if( pInfo == NULL  || m_sTExchModel.m_bSingle )
		return;
	CString str(pInfo->m_cCode, sizeof(pInfo->m_cCode));
	CString strType("H");
	m_ayCurStock.SetAt(str, strType);	
}

void CExchCalcMain::DelBuyStock(CodeInfo* pInfo)
{
	if( pInfo == NULL || m_sTExchModel.m_bSingle )
		return;
	CString str(pInfo->m_cCode, sizeof(pInfo->m_cCode));
	m_ayCurStock.RemoveKey(str);
}

long CExchCalcMain::GetBuyStockNum()
{
	return m_ayCurStock.GetSize();
}

double CExchCalcMain::GetBuyStockMaxMonery()
{
	if( m_sTExchModel.m_bSingle )
		return hx_DefMinValues;

	return (GetOverplusMoney() / m_sTExchModel.m_nOneBuyCount);
}

long CExchCalcMain::GetBuyStockMaxNum()
{
	if( !m_sTExchModel.m_bSingle )
	{
		if( m_sTExchModel.m_ayStkList.GetSize() <= 0 )
		{
			return m_sTExchModel.m_nOneBuyCount;
		}
		
		return min(m_sTExchModel.m_nOneBuyCount, m_sTExchModel.m_ayStkList.GetSize());
	}

	return 1; // m_sTExchModel.m_nOneBuyCount; 2003-09-02
}

BOOL CExchCalcMain::IsHaveBuyStock(CodeInfo* pInfo)
{
	if( pInfo == NULL || m_sTExchModel.m_bSingle )
		return FALSE;
	CString str(pInfo->m_cCode, sizeof(pInfo->m_cCode));
	CString strKey;
	return m_ayCurStock.Lookup(str, strKey);
}

BOOL CExchCalcMain::CheckEntireIsBuy(TExchModel* pModel, CodeInfo* pInfo, int nCurNum)
{
	if( s_pThis == NULL || pModel == NULL || pInfo == NULL )
		return FALSE;

	if( pModel->m_bBuyAll == FALSE && pModel->m_nOneBuyCount <= nCurNum )
	{
		return FALSE;
	}

	if( s_pThis->GetBuyStockNum() < pModel->m_nHoleStockCount )
	{
		return TRUE;
	}

	return s_pThis->IsHaveBuyStock(pInfo);
}

TExchCaclResult* CExchCalcMain::GetNewCaclRetultItem(int nIdx, TExchParam *pPrvParam/* = NULL*/, int lDecimal/* = 0*/)
{
	TExchOptimize* pOptimize = GetOptimize();
	CExpression* pExpress = GetExp();
	if( pOptimize == NULL || pExpress == NULL )
		return NULL;

	if( nIdx != -1 && pOptimize->IsValid() == FALSE )
	{
		return NULL;
	}

	TExchCaclResult* pCaclItem = new TExchCaclResult;
	if( pCaclItem == NULL )
	{
		return NULL;
	}

	if( pPrvParam != NULL )
	{
		pCaclItem->m_sCurParam = *pPrvParam;
		pCaclItem->m_sCurParam.m_uParamID = nIdx;

		AddCaclResultItem(pCaclItem);
		return pCaclItem;
	}

	pCaclItem->m_sCurParam.m_uParamID = (nIdx == -1 ? 0 : nIdx);
	pCaclItem->m_sCurParam.m_lDecimal = lDecimal;


	if( NULL == pCaclItem->m_sCurParam.New(pExpress->GetParamVarCount()) )
	{
		delete pCaclItem;
		pCaclItem = NULL;
	}
	else
	{
		CMapVariabile* pParamVar = pExpress->GetParamVar();
		
		if( pParamVar != NULL )
		{
			for( int i = 0 ; i < pParamVar->GetSize(); i++ )
			{
				CNumericValue* pNumVal = (CNumericValue*)pParamVar->GetAt(i);
				if( pNumVal != NULL )
				{
					if( pOptimize->IsValidItem(i) )
					{
						pCaclItem->m_sCurParam.m_pParam[i] = pNumVal->GetMinValue();
					}
					else
					{
						pCaclItem->m_sCurParam.m_pParam[i] = pNumVal->GetValue();
					}
				}
			}
		}
		AddCaclResultItem(pCaclItem);
	}

	return pCaclItem;
}

BOOL CExchCalcMain::AddCaclResultItem(TExchCaclResult* pItem)
{
	if( pItem == NULL )
		return FALSE;

	m_sCaclResult.Add(pItem);

	return TRUE;
}

BOOL CExchCalcMain::InitialExpressParam(TExchParam& sParam)
{
	if( s_pThis == NULL )
		return false;
	CExpression* pExpress = s_pThis->GetExp();
	if( pExpress == NULL )
		return false;

	CMapVariabile* pParamVar = pExpress->GetParamVar();
	if( pExpress == NULL )
		return false;

	if( pParamVar->GetSize() != sParam.m_lSize )
		return false;

	for( int i = 0 ; i < sParam.m_lSize; i++ )
	{
		CNumericValue* pNumVal = (CNumericValue*)pParamVar->GetAt(i);
		pNumVal->SetValue(sParam.m_pParam[i]);
	}

	return true;
}

CString CExchCalcMain::GetParamStr(BOOL bSignal, UINT uID /*= -1*/)
{
	CString strRet;
	TExchOptimize* pOptimize = GetOptimize();
	TExchExp* pExp;
	pExp = GetExchExp();
	if( pOptimize == NULL || pOptimize->GetValidItemCount() <= 0 )
	{
		if( pExp == NULL )
			return strRet;

		return pExp->GetParamStr();
	}

	// (-,-,256);
	if( bSignal )
	{
		CStringArray ayRet;
		strRet = pExp->GetParamStr();
		CGeneralHelper::AnalyseStr(strRet, ",", ayRet);
		int i;
		for(  i = 0; i < pOptimize->GetValidItemCount(); i++ )
		{
			int iGlobal = pOptimize->ValidIdxToGlobalIdx(i);
			if( iGlobal < ayRet.GetSize() )
			{
				strRet = ayRet.GetAt(iGlobal);
				if( iGlobal == 0 )
				{
					strRet = "(-";
				}
				else if( iGlobal == (ayRet.GetSize()-1) )
				{
					strRet = "-)";
				}
				else
				{
					strRet = "-";
				}

				ayRet.SetAt(i, strRet);
			}
		}

		strRet.Empty();
		for(i = 0; i < ayRet.GetSize(); i++ )
		{
			strRet += ayRet.GetAt(i);
			strRet += ',';
		}
		strRet.TrimRight(',');

		return strRet;
	}
	else
	{
		TExchCaclResult* pRet = GetCaclResultItem(uID);
		if( pRet == NULL )
		{
			return pExp->GetParamStr();
		}

		return pRet->GetCurParam()->ToStr();
	}
	
	return CString("");
}

long WINAPI CExchCalcMain::CaclMain(LPVOID pData)
{
	try
	{
		CExchCalcMain::s_uTheradState = TCaclTherad_State_Start;

		if( pData != NULL )
		{
			s_pThis = (CExchCalcMain*)pData;
		}
		if( s_pThis == NULL )
		{
			return Fail(TCaclReturn_NoData);
		}

		s_pThis->ClearData();

		TExchModel* pModel = s_pThis->GetMode();
		TExchOptimize* pOptimize = s_pThis->GetOptimize();
		CExpression* pExpress = s_pThis->GetExp();
		if( pModel == NULL || pOptimize == NULL || pExpress == NULL )
		{
			return Fail(TCaclReturn_NoData);
		}

		// ���Ϳ�ʼ����
		SendMsg(EXCH_MSG_WP_CACL_START, 0);

		long lRet;
		long lCaclCount = pOptimize->GetCaclUionCount(); // �Ż����Դ���
		TExchParam* pPrvExchParam = NULL;


		// ����Ʊ����
		if( pModel->m_bSingle )
		{
			if( lCaclCount > 0 )
			{
				for( int i = 0; i < lCaclCount; i++ )
				{
					if( CExchCalcMain::s_uTheradState == TCaclThread_State_Stop )
					{
						break;
					}
					// ��ʼ������
					TExchCaclResult* pCaclItem = s_pThis->GetNewCaclRetultItem(i, pPrvExchParam);
					if( pCaclItem == NULL )
					{
						return Fail(TCaclReturn_MoneryFail);
					}

					pPrvExchParam = &pCaclItem->m_sCurParam;

					if( pOptimize->GetCaclUionItem(i, pCaclItem->m_sCurParam) == FALSE 
						|| InitialExpressParam(pCaclItem->m_sCurParam) == FALSE
						)
					{
						//WRITE_LOG (__FILE__, __LINE__, "�����Ż�����");
						return Fail(TCaclReturn_Fail);
					}

					lRet = SingleStockTest(i, lCaclCount);
					if( lRet != TCaclReturn_Succ )
					{
						CString strtemp;
						strtemp.Format("����Ʊ�������-%d ", lRet);
						//WRITE_LOG (__FILE__, __LINE__, strtemp);
						return Fail(lRet);
					}
				}
			}
			else
			{
				// ��ʼ������
				TExchCaclResult* pCaclItem = s_pThis->GetNewCaclRetultItem(-1);
				if( pCaclItem == NULL )
				{
					return Fail(TCaclReturn_MoneryFail);
				}

				lRet = SingleStockTest(0, 1);
				if( lRet != TCaclReturn_Succ )
				{
					CString strtemp;
					strtemp.Format("����Ʊ�������-%d ", lRet);
					//WRITE_LOG (__FILE__, __LINE__, strtemp);
					return Fail(lRet);
				}
			}
		}
		// ȫ�г�����
		else
		{
			if( lCaclCount > 0 )
			{
				for( int i = 0; i < lCaclCount; i++ )
				{
					if( CExchCalcMain::s_uTheradState == TCaclThread_State_Stop )
					{
						break;
					}

					// ��ʼ��Ǯ
					TDealFlowStock::s_dLastMonery = TDealFlowStock::m_dAllMoney;
					// ��ʼ������
					TExchCaclResult* pCaclItem = s_pThis->GetNewCaclRetultItem(i);
					if( pCaclItem == NULL )
					{
						return Fail(TCaclReturn_MoneryFail);
					}
					if( pOptimize->GetCaclUionItem(i, pCaclItem->m_sCurParam) == FALSE 
						|| InitialExpressParam(pCaclItem->m_sCurParam) == FALSE
						)
					{
						CString strtemp;
						strtemp.Format("�����Ż�����");
						//WRITE_LOG (__FILE__, __LINE__, strtemp);
						return Fail(TCaclReturn_Fail);
					}

					lRet = EntireMarketTest(i, lCaclCount);
					if( lRet != TCaclReturn_Succ )
					{
						CString strtemp;
						strtemp.Format("����Ʊ�������-%d ", lRet);
						//WRITE_LOG (__FILE__, __LINE__, strtemp);
						return Fail(lRet);
					}
				}
			}
			else
			{
				// ��ʼ������
				TExchCaclResult* pCaclItem = s_pThis->GetNewCaclRetultItem(-1);
				if( pCaclItem == NULL )
				{
					return Fail(TCaclReturn_MoneryFail);
				}

				lRet = EntireMarketTest(0, 1);
				if( lRet != TCaclReturn_Succ )
				{
					CString strtemp;
					strtemp.Format("����Ʊ�������-%d ", lRet);
					//WRITE_LOG (__FILE__, __LINE__, strtemp);
					return Fail(lRet);
				}
			}
		}

		// ����״̬λ
		CExchCalcMain::s_uTheradState = TCaclThread_State_Stoping;
		// ���ͼ��������Ϣ
		SendMsg(EXCH_MSG_UP_CACL_SUCC, (LPARAM)0);
		return TCaclReturn_Succ;
	}
#ifdef VC_7_0
	// �쳣����
	catch(CMemoryException e) //���ڴ��쳣
	{
		TCHAR   szCause[255];
		e.GetErrorMessage(szCause, 255);
		CString strError;
		strError.Format("CaclMain() MemoryException: %s", szCause);
		// ������־
		//WRITE_LOG (__FILE__, __LINE__, _T(strError));
	}
	catch(CFileException e) // �ļ��쳣
	{
		TCHAR   szCause[255];
		e.GetErrorMessage(szCause, 255);
		CString strError;
		strError.Format("CaclMain() FileException: %s", szCause);
		// ������־
		//WRITE_LOG (__FILE__, __LINE__, _T(strError));
	}
	catch(CArchiveException e) // �����쳣
	{
		TCHAR   szCause[255];
		e.GetErrorMessage(szCause, 255);
		CString strError;
		strError.Format("CaclMain() ArchiveException: %s", szCause);
		// ������־
		//WRITE_LOG (__FILE__, __LINE__, _T(strError));
	}
#endif
	catch(...) // �����쳣
	{
		// ������־
		CString strError;
		strError.Format("CaclMain() other Exception: %s", ::strerror(::GetLastError()));
		//WRITE_LOG (__FILE__, __LINE__, _T(strError));
	}

	CExchCalcMain::s_uTheradState = TCaclThread_State_Stoping;
	return Fail();
}

BOOL CExchCalcMain::PreBuyStock(CodeInfo* pCode, StockDay* pData, int iDay, int iDayLen)
{
	if( s_pThis == NULL || pCode == NULL || pData == NULL )
		return FALSE;

	TDealFlowStock* pFlow = s_pThis->GetFlowFormCode(pCode);
	if( pFlow == NULL )
		return FALSE;

	pFlow->m_lBuySignalNum ++;
	if( iDay + 5 < iDayLen )
	{
		if( pData[iDay].m_lClosePrice <  pData[iDay + 5].m_lClosePrice )
			pFlow->m_lWin5Day++;
	}

	if( iDay + 10 < iDayLen )
	{
		if( pData[iDay].m_lClosePrice <  pData[iDay + 10].m_lClosePrice )
			pFlow->m_lWin10Day++;
	}

	if( iDay + 20 < iDayLen )
	{
		if( pData[iDay].m_lClosePrice <  pData[iDay + 20].m_lClosePrice )
			pFlow->m_lWin20Day++;
	}

	TExchSell* pSell = s_pThis->GetSell();
	if( pSell != NULL && iDay + pSell->m_nMaidPeriod < iDayLen )
	{
		double dClose = pData[iDay + (long)(pSell->m_nMaidPeriod)].m_lClosePrice;
		if( pData[iDay].m_lClosePrice <  dClose )
		{
			pFlow->m_lWinTabCyc++;
		}

		double dZF;
		if( dClose == 0 )
		{
			dZF = 0;
		}
		else
		{
			dZF = (dClose - pData[iDay].m_lClosePrice) * 100.0 / dClose;
		}
		if( pData[iDay].m_lClosePrice <  pData[iDay + (long)(pSell->m_nMaidPeriod)].m_lClosePrice 
			&&  dZF > pSell->m_nWinPeriod )
		{
			pFlow->m_lWinDays++;
		}
	}
	return TRUE;
}

BOOL CExchCalcMain::Total()
{
	if( s_pThis == NULL )
		return FALSE;

	try
	{
		TCaclResult *pResult = s_pThis->GetOneResult();
		TDealFlow	*pFlow = s_pThis->GetDealFlow();
		if( pResult == NULL || pFlow == NULL )
			return false;

		TDealFlowStock* pItem;
		int iCount = pFlow->GetCount();
		BOOL bSingle = s_pThis->GetMode()->m_bSingle;
		for( int i = 0; i < iCount; i++ )
		{
			pItem = pFlow->GetItem(i);
			if( pItem == NULL )
				continue;
			TCaclOneStockResult *pOneResult = pItem->TotalTo(NULL, bSingle);
			if( pOneResult != NULL )
			{
				pResult->Add(pOneResult);
			}
		}
		return pResult->Total(bSingle);
	}
#ifdef VC_7_0
	// �쳣����
	catch(CMemoryException e) //���ڴ��쳣
	{
		TCHAR   szCause[255];
		e.GetErrorMessage(szCause, 255);
		CString strError;
		strError.Format(" MemoryException: %s", szCause);
		// ������־
		//WRITE_LOG (__FILE__, __LINE__, _T(strError));
	}
#endif
	catch(...) // �����쳣
	{
		// ������־
		CString strError;
		strError.Format(" other Exception: %s", ::strerror(::GetLastError()));
		//WRITE_LOG (__FILE__, __LINE__, _T(strError));
	}
	
	return FALSE;
}

TDealFlowStock* CExchCalcMain::GetLastFlow(CodeInfo* pCode/* = NULL*/)
{
	TDealFlow* pDealFlow = GetDealFlow();
	if( pDealFlow == NULL )
		return NULL;

	TDealFlowStock* pFlowStock = NULL;
	if( pCode == NULL )
	{
		pFlowStock = pDealFlow->GetLastFlow();
	}
	else
	{
		pFlowStock = pDealFlow->GetFlowFormCode(pCode);
	}

	return pFlowStock;
}

double CExchCalcMain::GetOverplusMoney(CodeInfo* pCode)
{
	TExchModel* pMode = GetMode();
	if( pMode != NULL )
	{
		if( pMode->m_bSingle == FALSE )
			return TDealFlowStock::s_dLastMonery*(1-EXCH_POUNDAGE);
	}
	TDealFlowStock* pFlow;
	if( pCode == NULL )
	{
		pFlow = GetLastFlow();
	}
	else
	{
		pFlow = GetFlowFormCode(pCode);
	}
	if( pFlow == NULL )
		return 0;

	return (pFlow->m_lLastMoney*(1-EXCH_POUNDAGE));
}

// �õ����һ֧��Ʊ��ʣ���Ʊ��
long CExchCalcMain::GetLastStockNum(CodeInfo* pCode/* = NULL*/)
{
	TDealFlowStock* pFlow;
	if( pCode == NULL )
	{
		pFlow = GetLastFlow();
	}
	else
	{
		pFlow = GetFlowFormCode(pCode);
	}
	if( pFlow == NULL )
		return 0;

	return pFlow->m_lLastStockNum;
}

long CExchCalcMain::GetLastCyc(CodeInfo* pCode/* = NULL*/)
{
	TDealFlowStock* pFlow = GetLastFlow(pCode);
	if( pFlow == NULL )
		return 0;
	return pFlow->GetLastCaclDays();
}

double CExchCalcMain::GetLastPrice(CodeInfo* pCode/* = NULL*/)
{
	TDealFlowStock* pFlow = GetLastFlow(pCode);
	if( pFlow == NULL )
		return 0;
	return pFlow->GetLastPrice();
}

TDealFlowStock* CExchCalcMain::GetFlowFormCode(CodeInfo *pCode)
{
	try
	{
		TDealFlow* pFlow = GetDealFlow();
		if( pFlow == NULL )
			return NULL;

		TDealFlowStock* pItem = pFlow->GetFlowFormCode(pCode);
		if( pItem == NULL )
		{
			TExchModel* pMode = GetMode();
			const StockUserInfo* pInfo = (StockUserInfo*)((::IsWindow(g_hParentWnd) == FALSE)?NULL : ::SendMessage(g_hParentWnd, HX_USER_COMPILEDATA, 29, (LPARAM)pCode));
			if( pInfo != NULL )
			{
				CString strName(pInfo->m_cStockName, sizeof(pInfo->m_cStockName));
				pItem = new TDealFlowStock(*pCode, strName, pMode->m_lAllInvestCount);
			}
			else
			{
				CString strName(pCode->m_cCode, sizeof(pCode->m_cCode));
				pItem = new TDealFlowStock(*pCode, strName, pMode->m_lAllInvestCount);
			}

			pItem->m_lLastBuy = 0;
			if( pItem != NULL )
				pFlow->AddStockItem(pItem);
		}

		return pItem;
	}
#ifdef VC_7_0
	// �쳣����
	catch(CMemoryException e) //���ڴ��쳣
	{
		TCHAR   szCause[255];
		e.GetErrorMessage(szCause, 255);
		CString strError;
		strError.Format("CExchCalcMain::GetFlowFormCode MemoryException: %s", szCause);
		// ������־
		//WRITE_LOG (__FILE__, __LINE__, _T(strError));
	}
#endif
	catch(...) // �����쳣
	{
		// ������־
		CString strError;
		strError.Format("CExchCalcMain::GetFlowFormCode other Exception: %s", ::strerror(::GetLastError()));
		//WRITE_LOG (__FILE__, __LINE__, _T(strError));
	}

	return NULL;
}

// �Ƿ�Ϊָ����
BOOL CExchCalcMain::IsTechLine(CString& str)
{
	str.TrimLeft();
	char chTemp = str.GetAt(0); 
	if( ::isdigit(chTemp) > 0 )
		return FALSE;
	return TRUE;
}

// �õ��Ƚ�����
double CExchCalcMain::GetTechData(TCaclResultTemp* pRet, FormulaCompareSubData* pSub, int nTechNum, int iIdx)
{
	if( pRet == NULL || pSub == NULL )
		return 0;
	if( nTechNum == 0 )
	{
		if( CExchCalcMain::IsTechLine(pSub->m_dData1) )
		{
			return pRet->m_lLeftReqult[iIdx];
		}
		else
		{
			return atof(pSub->m_dData1);
		}
	}

	if( nTechNum == 1 )
	{
		if( CExchCalcMain::IsTechLine(pSub->m_dData2) )
		{
			return pRet->m_lRightReqult[iIdx];
		}
		else
		{
			return atof(pSub->m_dData2);
		}
	}

	return 0;
}

// �Ƿ�������������
BOOL CExchCalcMain::IsBuy(TCaclTemp* pData, int iDay)
{
	try
	{
		if( pData == NULL || s_pThis == NULL 
			/*|| !s_pThis->IsHaveMoney()*/ 
			|| iDay >= pData->lCountDate 
			|| pData->pRetCacl->m_lDescReqult[iDay] == hx_DefMinValues )
			return FALSE;

		TExchBuy* pBuy = s_pThis->GetBuy();
		if( pBuy == NULL )
			return FALSE;

		double dTemp1, dTemp2;
		dTemp1 = CExchCalcMain::GetTechData(pData->pRetCacl, &pBuy->m_SubData, 0, iDay);
		dTemp2 = CExchCalcMain::GetTechData(pData->pRetCacl, &pBuy->m_SubData, 1, iDay);

		// ��������
		switch( pBuy->m_SubData.m_cType )
		{
		case FormulaCompareSubData::DaYu:			//  ����
			{
				if( dTemp1 == hx_DefMinValues )
					return FALSE;
				if( pData->pRetCacl->m_lDescReqult[iDay] > dTemp1 )
					return TRUE;
				return FALSE;
			}
			break;
		case FormulaCompareSubData::XiaoYu:		//	С��
			{
				if( dTemp1 == hx_DefMinValues )
					return FALSE;
				if( pData->pRetCacl->m_lDescReqult[iDay] < dTemp1 )
					return TRUE;
				return FALSE;
			}
			break;
		case FormulaCompareSubData::DengYu:		//	����
			{
				if( dTemp1 == hx_DefMinValues )
					return FALSE;
				if( fabs(pData->pRetCacl->m_lDescReqult[iDay].m_dData) - fabs(dTemp1) < 0.0001 )
					return TRUE;
				return FALSE;
			}
			break;
		case FormulaCompareSubData::JieYu:		//	����
			{
				if( dTemp1 == hx_DefMinValues )
					return FALSE;
				if( dTemp2 == hx_DefMinValues )
					return FALSE;
				if( pData->pRetCacl->m_lDescReqult[iDay] > dTemp1 && pData->pRetCacl->m_lLeftReqult[iDay] < dTemp2 )
					return TRUE;
				return FALSE;
			}
			break;
		case FormulaCompareSubData::ShangChuan:	//	�ϴ�
			{
				if( dTemp1 == hx_DefMinValues )
					return FALSE;
				if( iDay < 1 )
				{
					return FALSE;
				}
				double dPreData = pData->pRetCacl->m_lDescReqult[iDay-1];
				if( dPreData == hx_DefMinValues )
					return FALSE;
				if( pData->pRetCacl->m_lDescReqult[iDay] > dTemp1 && dPreData < dTemp1)
					return TRUE;
				return FALSE;
			}
			break;
		case FormulaCompareSubData::XiaoPo:		//	����
			{
				if( dTemp1 == hx_DefMinValues )
					return FALSE;
				if( iDay < 1 )
				{
					return FALSE;
				}
				double dPreData = pData->pRetCacl->m_lDescReqult[iDay-1];
				if( dPreData == hx_DefMinValues )
					return FALSE;
				if( pData->pRetCacl->m_lDescReqult[iDay] < dTemp1 && dPreData > dTemp1 )
					return TRUE;
				return FALSE;
			}
			break;
		case FormulaCompareSubData::WeiChi:		//	ά��dtemp1�ܺ�����
			{
				if( dTemp1 == hx_DefMinValues )
					return FALSE;
				if( dTemp2 == hx_DefMinValues )
					return FALSE;
				if( iDay < dTemp1 )
				{
					return FALSE;
				}

				if( pData->pRetCacl->m_lDescReqult[iDay] < dTemp2 )
					return false;
				for( long lLoop = 1; lLoop < (long)dTemp1; lLoop++ )
				{
					double dNext = pData->pRetCacl->m_lDescReqult[iDay-lLoop];
					if( dNext == hx_DefMinValues )
						return FALSE;
					if( dNext >= dTemp2 )
						return false;
				}
				return TRUE;
			}
			break;
		case FormulaCompareSubData::BiaoChi:		//	����dtemp1�ܺ�����
			{
				if( dTemp1 == hx_DefMinValues )
					return FALSE;
				if( dTemp2 == hx_DefMinValues )
					return FALSE;
				if( iDay < dTemp1 )
				{
					return FALSE;
				}

				if( pData->pRetCacl->m_lDescReqult[iDay] > dTemp2 )
					return false;
				for( long lLoop = 1; lLoop < (long)dTemp1; lLoop++ )
				{
					double dNext = pData->pRetCacl->m_lDescReqult[iDay-lLoop];
					if( dNext == hx_DefMinValues )
						return FALSE;
					if( dNext <= dTemp2 )
						return false;
				}
				return TRUE;
			}
			break;
		case FormulaCompareSubData::Condition: 	// ����ѡ��
		case FormulaCompareSubData::Exchange:  	// ����ϵͳ
		case FormulaCompareSubData::MoreKLine: 	// ���K��
			{
				if( pData->pRetCacl->m_lDescReqult[iDay] == hx_DefMinValues )
				{
					return FALSE;
				}
				BOOL bTemp = (BOOL)pData->pRetCacl->m_lDescReqult[iDay];
				return bTemp;
			}
			break;
		}
		return TRUE;
	}
#ifdef VC_7_0
	// �쳣����
	catch(CMemoryException e) //���ڴ��쳣
	{
		TCHAR   szCause[255];
		e.GetErrorMessage(szCause, 255);
		CString strError;
		strError.Format("CExchCalcMain::IsBuy MemoryException: %s", szCause);
		// ������־
		//WRITE_LOG (__FILE__, __LINE__, _T(strError));
	}
#endif
	catch(...) // �����쳣
	{
		// ������־
		CString strError;
		strError.Format("CExchCalcMain::IsBuy other Exception: %s", ::strerror(::GetLastError()));
		//WRITE_LOG (__FILE__, __LINE__, _T(strError));
	}
	return FALSE;
}

// �Ƿ�������������,�����Ƿ��й�Ʊ����
long CExchCalcMain::IsSell(TCaclTemp* pData, int iDay)
{
	try
	{
#ifdef _DEBUG
		if( pData->pRetDiskData[iDay].m_lDate == 20030625 )
		{
			long lLen = 0;
		}
#endif
		if( pData == NULL || s_pThis == NULL 
			|| iDay >= pData->lCountDate )
		{
			return -1;
		}

		TExchSell* pSell = s_pThis->GetSell();
		if( pSell == NULL )
		{
			return -1;
		}
		char cExpType = pData->pPress->GetExpressType();

		// ����ϵͳ
		if( cExpType == Exchange )
		{
			if( pData->pRetCacl->m_lLeftReqult[iDay].m_dData != hx_DefMinValues )
			{
				BOOL bTemp = (BOOL)pData->pRetCacl->m_lLeftReqult[iDay];
				if( bTemp )
				{
					return TDealFlow_DT_Sell;
				}
			}
		}

		// Ŀ������ƽ��
		if( pSell->m_bMaidPeriod )		
		{
			long lCyc = s_pThis->GetLastCyc(pData->pCode);
			if( lCyc >= pSell->m_nMaidPeriod )
			{
				return TDealFlow_DT_PingChang;
			}
		}

		// Ŀ������ֹӮ
		if( pSell->m_bWinPeriod )		
		{
			double dLastPrice = s_pThis->GetLastPrice(pData->pCode); 

			if( dLastPrice != -1 )
			{
				double dPrice = pData->pRetDiskData[iDay].m_lClosePrice  * 1.0 / s_pThis->m_nPriceUnit;
				if( ((dPrice - dLastPrice) / dPrice) > (0.01 * pSell->m_nWinPeriod) )
				{
					return TDealFlow_DT_ZhiYing;
				}
			}
		}

		// Ŀ��ֹ��
		if( pSell->m_bLosePeriod )		
		{
			double dLastPrice = s_pThis->GetLastPrice(pData->pCode); 
			if( dLastPrice != -1 )
			{
				double dPrice = pData->pRetDiskData[iDay].m_lClosePrice  * 1.0 / s_pThis->m_nPriceUnit;
				if( ((dPrice - dLastPrice) / dPrice) < (-0.01 * pSell->m_nLosePeriod) )
				{
					return TDealFlow_DT_ZhiShun;
				}
			}
		}

		// ����ƽ��
		long lCyc = s_pThis->GetLastCyc(pData->pCode);
		if( lCyc != -1 && pSell->m_bFallPeriod && (iDay-lCyc) >= 0 )		
		{
			if( lCyc <= pSell->m_nFallPeriodCount )
			{
				double dMaxPrice = -1;
				for( int i = 0; i < lCyc; i++ )
				{
					dMaxPrice = max(dMaxPrice, pData->pRetDiskData[iDay-i].m_lClosePrice);
				}
				if( dMaxPrice > 0 )
				{
					double dPrice = pData->pRetDiskData[iDay].m_lClosePrice;
					if( ((dPrice-dMaxPrice) / dMaxPrice) <= (-0.01 * pSell->m_nFallPeriod) )
					{
						return TDealFlow_DT_HuiLuo;
					}
				}
			}
		}

		// ����ƽ��
		if( lCyc != -1 && pSell->m_bUpPeriod )	
		{
			if( lCyc == pSell->m_nUpPeriodCount )
			{
				double dMaxPrice = -1;
				for( int i = 0; i < lCyc; i++ )
				{
					dMaxPrice = max(dMaxPrice, pData->pRetDiskData[iDay-i].m_lClosePrice);
				}
				if( dMaxPrice > 0 )
				{
					dMaxPrice /= s_pThis->m_nPriceUnit;
					double dLastPrice = s_pThis->GetLastPrice(pData->pCode); 
					if( fabs((dMaxPrice - dLastPrice) / dMaxPrice) < (0.01 * pSell->m_nUpPeriod) )
					{
						return TDealFlow_DT_HenPang;
					}
				}
			}
		}
		return -1;
	}
#ifdef VC_7_0
	// �쳣����
	catch(CMemoryException e) //���ڴ��쳣
	{
		TCHAR   szCause[255];
		e.GetErrorMessage(szCause, 255);
		CString strError;
		strError.Format(" MemoryException: %s", szCause);
		// ������־
		//WRITE_LOG (__FILE__, __LINE__, _T(strError));
	}
#endif
	catch(...) // �����쳣
	{
		// ������־
		CString strError;
		strError.Format(" other Exception: %s", ::strerror(::GetLastError()));
		//WRITE_LOG (__FILE__, __LINE__, _T(strError));
	}

	return -1;
}

BOOL CExchCalcMain::ClearData()
{
	try
	{
		for( int i = 0; i < m_sCaclResult.GetSize(); i++ )
		{
			delete m_sCaclResult.GetAt(i);
		}
		m_sCaclResult.RemoveAll();
		
		return TRUE;
	}
#ifdef VC_7_0
	// �쳣����
	catch(CMemoryException e) //���ڴ��쳣
	{
		TCHAR   szCause[255];
		e.GetErrorMessage(szCause, 255);
		CString strError;
		strError.Format("CExchCalcMain::ClearData MemoryException: %s", szCause);
		// ������־
		//WRITE_LOG (__FILE__, __LINE__, _T(strError));
	}
#endif
	catch(...) // �����쳣
	{
		// ������־
		CString strError;
		strError.Format("CExchCalcMain::ClearData other Exception: %s", ::strerror(::GetLastError()));
		//WRITE_LOG (__FILE__, __LINE__, _T(strError));
	}

	return FALSE;
}

// ֹͣ
long CExchCalcMain::Stop()
{
	if( CExchCalcMain::s_uTheradState != TCaclTherad_State_Start)
	{
		return TCaclReturn_Succ;
	}

	int iStop = 100;
	while( iStop > 0 )
	{
		iStop--;
		CExchCalcMain::s_uTheradState =	TCaclThread_State_Stop;
		Sleep(100);
		if( CExchCalcMain::s_uTheradState != TCaclTherad_State_Start)
			break;
	}

	return TCaclReturn_Succ;
}

// ��ʼ
long CExchCalcMain::Start()
{
	if( CExchCalcMain::Stop() != TCaclReturn_Succ )
		return TCaclReturn_Fail;

	DWORD dwThreadID;
	CExchCalcMain::s_uTheradState = TCaclThread_State_Stop;
	HANDLE hThread = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)CaclMain, 
		(void *)this, NULL, &dwThreadID);
	if (hThread)
	{
		CloseHandle(hThread);
		hThread = NULL;

		int i = 10000;
		while ( i-- > 0 && CExchCalcMain::s_uTheradState != TCaclTherad_State_Start )
		{
			Sleep(20);
		}
		if( CExchCalcMain::s_uTheradState != TCaclTherad_State_Start )
			return TCaclReturn_Fail;
	}
	else 
	{
		return TCaclReturn_Fail;
	}

	return TCaclReturn_Succ;    
}

// ������
long CExchCalcMain::Save(CWnd* pParent/* = NULL*/)
{
	if( pParent == NULL )
	{
		pParent = CWnd::FromHandle(GetMsgWnd());
	}
	CFileDialog fileDlg(FALSE, "exh", "", OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, _T("�����ļ�(*.exh)|*.exh|�����ļ�(*.*)|*.*||"), pParent);
	if( fileDlg.DoModal() != IDOK )
		return TCaclReturn_Fail;

	CFile fp;
	if( fp.Open(fileDlg.GetFileName(), CFile::modeWrite|CFile::modeCreate) != 0 )
	{
		return TCaclReturn_Fail;
	}
	CArchive ar(&fp, CArchive::store);

	TExchExp* pExp = GetExchExp();
	TExchModel* pModel = GetMode();
	TExchBuy* pBuy = GetBuy();
	TExchSell* pSell = GetSell();
	TCaclResult* pResult = GetOneResult();
	TDealFlow* pFlow = GetDealFlow();
	TExchOptimize* pOpt = GetOptimize();	

	if( pExp == NULL || pModel == NULL || pBuy == NULL || pSell == NULL 
		|| pResult == NULL || pFlow == NULL || pOpt == NULL )
	{
		ar.Close();
		fp.Close();
		return TCaclReturn_Fail;
	}

	pExp->Serialize(ar);
	pModel->Serialize(ar);
	pBuy->Serialize(ar); 
	pSell->Serialize(ar); 
	pResult->Serialize(ar);
	pFlow->Serialize(ar);
	pOpt->Serialize(ar);

	ar.Close();
	fp.Close();

	return TCaclReturn_Succ;
}

// ��������
long CExchCalcMain::Open(CWnd* pParent/* = NULL*/)
{
	try
	{
		if( pParent == NULL )
		{
			pParent = CWnd::FromHandle(GetMsgWnd());
		}
		CFileDialog fileDlg(TRUE, "exh", "", OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, _T("�����ļ�(*.exh)|*.exh|�����ļ�(*.*)|*.*||"), pParent);
		if( fileDlg.DoModal() != IDOK )
			return TCaclReturn_Fail;

		CFile fp;
		if( fp.Open(fileDlg.GetFileName(), CFile::modeRead) != 0 )
		{
			return TCaclReturn_Fail;
		}
		CArchive ar(&fp, CArchive::load);

		TExchExp* pExp = GetExchExp();
		TExchModel* pModel = GetMode();
		TExchBuy* pBuy = GetBuy();
		TExchSell* pSell = GetSell();
		TCaclResult* pResult = GetOneResult();
		TDealFlow* pFlow = GetDealFlow();
		TExchOptimize* pOpt = GetOptimize();	

		if( pExp == NULL || pModel == NULL || pBuy == NULL || pSell == NULL 
			|| pResult == NULL || pFlow == NULL || pOpt == NULL )
		{
			ar.Close();
			fp.Close();
			return TCaclReturn_Fail;
		}

		pModel->m_ProcGetStockUser = (GETSTOCKINFOPROC)GetStockInfoProc;
		pExp->Serialize(ar, CDlgFormulaMan::m_pExternExpression);
		pModel->Serialize(ar);
		pBuy->Serialize(ar); 
		pSell->Serialize(ar); 
		pResult->Serialize(ar);
		pFlow->Serialize(ar);
		pOpt->Serialize(ar);

		ar.Close();
		fp.Close();

		return TCaclReturn_Succ;
	}
#ifdef VC_7_0
	// �쳣����
	catch(CMemoryException e) //���ڴ��쳣
	{
		TCHAR   szCause[255];
		e.GetErrorMessage(szCause, 255);
		CString strError;
		strError.Format("CExchCalcMain::Open MemoryException: %s", szCause);
		// ������־
		//WRITE_LOG (__FILE__, __LINE__, _T(strError));
	}
	catch(CFileException e) // �ļ��쳣
	{
		TCHAR   szCause[255];
		e.GetErrorMessage(szCause, 255);
		CString strError;
		strError.Format("CExchCalcMain::Open FileException: %s", szCause);
		// ������־
		//WRITE_LOG (__FILE__, __LINE__, _T(strError));
	}
	catch(CArchiveException e) // �����쳣
	{
		TCHAR   szCause[255];
		e.GetErrorMessage(szCause, 255);
		CString strError;
		strError.Format("CExchCalcMain::Open ArchiveException: %s", szCause);
		// ������־
		//WRITE_LOG (__FILE__, __LINE__, _T(strError));
	}
#endif
	catch(...) // �����쳣
	{
		// ������־
		CString strError;
		strError.Format("CExchCalcMain::Open other Exception: %s", ::strerror(::GetLastError()));
		//WRITE_LOG (__FILE__, __LINE__, _T(strError));
	}
	
	return TCaclReturn_Fail;
}

int CExchCalcMain::GetCaclResultCount()
{
	return m_sCaclResult.GetSize();
}

TExchCaclResult* CExchCalcMain::GetObjResultItem(UINT uObjID/* = HS_EXCH_CACLRESULT_STOCK_ID_PROFIT*/)
{
	int nFindID = -1;
	double dMaxRet = -1000000;
	int nCount = m_sCaclResult.GetSize();

	for( int i = 0; i < nCount; i++ )
	{
		TExchCaclResult* pItme = m_sCaclResult.GetAt(i);
		if( pItme != NULL )
		{
			double dTemp = pItme->GetIDData(uObjID);
			if( dMaxRet < dTemp )
			{
				nFindID = i;
				dMaxRet = dTemp;
			}
		}
	}

	if( nFindID == -1 )
		return NULL;

	return m_sCaclResult.GetAt(nFindID);
}

TExchCaclResult* CExchCalcMain::GetCaclResultItem(UINT uParamID/* = -1*/)
{
	if( uParamID == -1 )
	{
		if( m_sCaclResult.GetSize() > 0 )
		{
			TExchCaclResult* pItem = m_sCaclResult.GetAt(m_sCaclResult.GetSize() - 1 );
			return pItem;
		}
	}
	else
	{
		for( int i = 0; i < m_sCaclResult.GetSize(); i++ )
		{
			TExchCaclResult* pItem = m_sCaclResult.GetAt(i);
			if( pItem->GetCurParam()->m_uParamID == uParamID )
			{
				return pItem;
			}
		}
	}

	return NULL;
}

TCaclResult* CExchCalcMain::GetOneResult(UINT uParamID /*= -1*/) 
{ 
	if( uParamID == -1 )
	{
		if( m_sCaclResult.GetSize() > 0 )
		{
			TExchCaclResult* pItem = m_sCaclResult.GetAt(m_sCaclResult.GetSize() - 1 );
			return pItem->GetResult();
		}
	}
	else
	{
		for( int i = 0; i < m_sCaclResult.GetSize(); i++ )
		{
			TExchCaclResult* pItem = m_sCaclResult.GetAt(i);
			if( pItem->GetCurParam()->m_uParamID == uParamID )
			{
				return pItem->GetResult();
			}
		}
	}
	return NULL; 
}

TDealFlow* CExchCalcMain::GetDealFlow(UINT uParamID /*= -1*/) 
{ 
	if( uParamID == -1 )
	{
		if( m_sCaclResult.GetSize() > 0 )
		{
			TExchCaclResult* pItem = m_sCaclResult.GetAt(m_sCaclResult.GetSize() - 1 );
			return pItem->GetDealFlow();
		}
	}
	else
	{
		for( int i = 0; i < m_sCaclResult.GetSize(); i++ )
		{
			TExchCaclResult* pItem = m_sCaclResult.GetAt(i);
			if( pItem->GetCurParam()->m_uParamID == uParamID )
			{
				return pItem->GetDealFlow();
			}
		}
	}
	return NULL; 
}

TExchParam*  CExchCalcMain::GetCurParam(UINT uParamID/* = -1*/)
{
	if( uParamID == -1 )
	{
		if( m_sCaclResult.GetSize() > 0 )
		{
			TExchCaclResult* pItem = m_sCaclResult.GetAt(m_sCaclResult.GetSize() - 1 );
			return pItem->GetCurParam();
		}
	}
	else
	{
		for( int i = 0; i < m_sCaclResult.GetSize(); i++ )
		{
			TExchCaclResult* pItem = m_sCaclResult.GetAt(i);
			if( pItem->GetCurParam()->m_uParamID == uParamID )
			{
				return pItem->GetCurParam();
			}
		}
	}
	return NULL; 
}

int	CExchCalcMain::GetCaclResult(UINT uParamID)
{
	if( uParamID == -1 )
	{
		return m_sCaclResult.GetSize()-1;
	}
	else
	{
		for( int i = 0; i < m_sCaclResult.GetSize(); i++ )
		{
			TExchCaclResult* pItem = m_sCaclResult.GetAt(i);
			if( pItem->GetCurParam()->m_uParamID == uParamID )
			{
				return i;
			}
		}
	}

	return -1;
}

// �Ƚϴ�С�������Ż��ı������ʹ��
int CExchCalcMain::CompareParamOfCol(UINT uID1, UINT uID2, int ColID)
{
	TExchCaclResult *pItem1, *pItem2;

	pItem1 = this->GetCaclResultItem(uID1);
	if( pItem1 == NULL )
		return 1;

	pItem2 = this->GetCaclResultItem(uID2);
	if( pItem2 == NULL )
		return -1;

	double colData1 = pItem1->GetIDData(ColID);
	double colData2 = pItem2->GetIDData(ColID);

	if( colData1 > colData2 )
		return 1;
	if( colData1 < colData2 )
		return -1;

	return 0;
}

// ��pExp�Ĳ������ó�Ĭ�ϲ���m_sDefParam
BOOL CExchCalcMain::ResetDefParam(CExpression* pExp)
{
	if( pExp == NULL )
	{
		pExp = GetExp();
	}
	return m_sDefParam.ConvertToExpress(pExp);
}

// ��Ĭ�ϲ���m_sDefParam���ó�pExp�Ĳ���
BOOL CExchCalcMain::FromExpToDefParam(CExpression* pExp)
{
	if( pExp == NULL )
	{
		pExp = GetExp();
	}
	return m_sDefParam.ConvertFromExpress(pExp);
}
long CExchCalcMain::GetStockPriceDecimal(CString strCode, int& index)
{
	if ( index > 0 )
	{
		StockUserInfo* pStock = s_pThis->GetStockAt(index);	
		if (pStock)
			return pStock->m_nDecimal;
	}
	int iStockNum = s_pThis->GetStockNum();
	for ( int i=0; i<iStockNum; i++)
	{
		StockUserInfo* pStock = s_pThis->GetStockAt(i);	
		if( pStock == NULL )
			continue;
		if ( !pStock->GetCode().Compare(strCode) )
		{
			index = i;
			return pStock->m_nDecimal;
		}
	}
	return 0;
}
long CExchCalcMain::GetStockPriceUnit(CString strCode, int& index)
{
	if ( index > 0 )
	{
		StockUserInfo* pStock = s_pThis->GetStockAt(index);	
		if (pStock)
			return pStock->m_nPriceUnit;
	}
	int iStockNum = s_pThis->GetStockNum();
	for ( int i=0; i<iStockNum; i++)
	{
		StockUserInfo* pStock = s_pThis->GetStockAt(i);	
		if( pStock == NULL )
			continue;
		if ( !pStock->GetCode().Compare(strCode) )
		{
			index = i;
			return pStock->m_nPriceUnit;
		}
	}
	return 0;
}