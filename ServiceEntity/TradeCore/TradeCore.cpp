// TradeCore.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "HSWinner.h"
#include "TradeCore.h"
#include "TradeAccountMng.h"
#include "TradeStockWithDraw.h"
#include "TradeStockEntrust.h"
#include "QueryFund.h"
#include "TradeLogin.h"
#include "QueryStockAccount.h"
#include "TradeDataBase.h"
#include "QueryBase.h"
#include "QueryStockEntrust.h"
#include "QueryStockDeal.h"
#include "QueryStockHold.h"
#include "ReqHeart.h"
#include "QueryStockMaxBuy.h"
#include "QueryStockMaxSell.h"
#include "TradeAction.h"
#include "WinnerApplication.h"
#include "TradeFundEntrust.h"
#include "QueryFundInfo.h"
#include "QueryFundHold.h"
#include "QueryFundEntrust.h"
#include "QueryBank.h"
#include "BankStockTransfer.h"
#include "ChangePassword.h"
#include "TradeSetStockCostPrice.h"
#include "QueryUserInfo.h"
#include "ModifyUserInfo.h"
#include "QueryStockFundInfo.h"
#include "TradeStockZJHK.h"
#include "TradeStockZJHQ.h"
#include "QueryFundStockAccount.h"
#include "QueryFundCompany.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#pragma   data_seg( "._tradecore_data") 
ITradeCoreObj*	pTradeCoreObj = NULL; 
#pragma   data_seg() 

//
//TODO: If this DLL is dynamically linked against the MFC DLLs,
//		any functions exported from this DLL which call into
//		MFC must have the AFX_MANAGE_STATE macro added at the
//		very beginning of the function.
//
//		For example:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// normal function body here
//		}
//
//		It is very important that this macro appear in each
//		function, prior to any calls into MFC.  This means that
//		it must appear as the first statement within the 
//		function, even before any object variable declarations
//		as their constructors may generate calls into the MFC
//		DLL.
//
//		Please see MFC Technical Notes 33 and 58 for additional
//		details.
//

// CTradeCoreApp

BEGIN_MESSAGE_MAP(CTradeCoreApp, CWinApp)
END_MESSAGE_MAP()


// CTradeCoreApp construction

CTradeCoreApp::CTradeCoreApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only CTradeCoreApp object

CTradeCoreApp theApp;


// CTradeCoreApp initialization

BOOL CTradeCoreApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}


ITradeCoreObj* PASCAL GetServiceObject(const char* szName,const char* szPrefix)
{
	return pTradeCoreObj;
}

int PASCAL InitEntity()
{
	if (!pTradeCoreObj)
	{
		pTradeCoreObj = new CTradeCoreObj;
	}
	return TRUE;
}

int PASCAL UninitEntity()
{
	pTradeCoreObj->Release();
	pTradeCoreObj = NULL;
	return TRUE;
}

int PASCAL OnFrameNotify(int nFlag)
{
	if (SN_BEFORE_FRAME_CLOSE == nFlag)
	{
		pTradeCoreObj->FreeCache();
	}
	return TRUE;
}
//////////////////////////////////////////////////////////////////////////
 CTradeCoreObj::CTradeCoreObj()
{
	m_AccountMng = new CAccountMng;
	m_pTradeConfigObj = (ITradeConfigObj*)CWinnerApplication::GetObject(OBJ_TRADECONFIG);
	m_pWndTrading = NULL;
	m_pTradeTcpView = NULL;
}

CTradeCoreObj::~CTradeCoreObj()
{
	/*if (m_pWndTrading != NULL && m_pWndTrading->m_hWnd != NULL && ::IsWindow(m_pWndTrading->m_hWnd))
	{
		m_pWndTrading->SendMessage(WM_CLOSE);
	}
	if (m_pWndTrading != NULL )
	{
		int nCount = 0;
		while(m_pWndTrading->m_hWnd != NULL && ::IsWindow(m_pWndTrading->m_hWnd) && nCount ++ < 1000)
		{
			Sleep(10);
		}
		delete m_pWndTrading;
		m_pWndTrading = NULL;
	}*/
}

void CTradeCoreObj::Release()
{
	m_AccountMng->Release();
	delete this;
}

IAccountMng* CTradeCoreObj::GetAccountMng()
{
	if (!m_pWndTrading)
	{
		m_pWndTrading = new CWndTrading();
		if (m_pWndTrading)
		{
			if (m_pWndTrading->m_hWnd == NULL || !::IsWindow(m_pWndTrading->m_hWnd))
			{
				BOOL bCreated = m_pWndTrading->Create(NULL, _T("TradingWnd"), WS_OVERLAPPED | WS_EX_TOOLWINDOW, CRect(0, 0, 0, 0), NULL, 0);
				if(bCreated == FALSE)
				{
					OutputDebugString("Create WndTrading window failed.\n");
				}
			}
		}
	}	
	return m_AccountMng;
}

ITrading* CTradeCoreObj::NewTradingByAction(const int iAction)
{
	ITrading* pTmp = NULL;
	switch(iAction)
	{
	case TRDADE_LOGIN://��½
		{
			pTmp = new CReqLogin;
			break;
		}
	case TRADE_STOCK_QUERYMAXSELL://������
	case TRADE_STOCK_QUERYMERGESPLIT://�ɺϲ��������
	case TRADE_RZRQ_QUERYRQAMOUNT:
		{
			pTmp = new CReqQueryStockMaxSell;
			break;
		}
	case TRADE_STOCK_QUERYMAXBUY://������
	case TRADE_RZRQ_QUERYMAXBUY://
	case TRADE_RZRQ_QUERYMAXSELL:
		{
			pTmp = new CReqQueryStockMaxBuy;
			break;
		}
	case TRADE_STOCK_ENTRUST://֤ȯί��
	case TRADE_STOCK_SBENTRUST://����ί��
	case TRADE_STOCK_FUNDMERGE://�����̺�ҵ��ϲ�
	case TRADE_STOCK_FUNDSPLIT://�����̺�ҵ����
	case TRADE_STOCK_FUNDTRANS://�����̺�ҵ��ת��
	case TRADE_RZRQ_ENTRUST://������ȯί��
	case TRADE_STOCK_9835://ETF�����ֽ��Ϲ�
	case TRADE_STOCK_9837://ETF���¹ɷݻ���
		{
			pTmp = new CReqStockEntrust;
			break;
		}
	case TRADE_STOCK_WITHDRAW://����
	case TRADE_STOCK_SBWITHDRAW://���峷��
	case TRADE_STOCK_FUNDWITHDRAW://�����̺�ҵ�񳷵�
		{
			pTmp = new CReqStockWithDraw;
			break;
		}
	case TRADE_QUERYFUND://��ѯ�ʽ�
	case TRADE_RZRQ_QUERYDEBITTOTAL://������ȯ��ծ����
		{
			pTmp = new CReqQueryFund;
			break;
		}
	case TRADE_STOCK_QUERYACCOUNT://��ѯ�ɶ��˺�
		{
			pTmp = new CReqQueryStockAccount;
			break;
		}
	case TRADE_STOCK_QUERYENTRUST://֤ȯ����ί��
	case TRADE_STOCK_303://�ɳ�ί��
	case TRADE_STOCK_QUERYSBENTRUST://���嵱��ί��
	case TRADE_STOCK_QUERYFUNDENTRUST://�����̺�ҵ����ί��
		{
			pTmp = new CReqQueryStockEntrust;
			break;
		}
	case TRADE_STOCK_QUERYDEAL://֤ȯ���ճɽ�
		{
			pTmp = new CReqQueryStockDeal;
			break;
		}
	case TRADE_STOCK_QUERYHOLD://֤ȯ�ֲ�
		{
			pTmp = new CReqQueryStockHold;
			break;
		}
	case TRADE_FUND_QUERYFUNDINFO://��ѯ������Ϣ
		{
			pTmp = new CReqQueryFundInfo;
			break;
		}
	case TRADE_FUND_SUBSCRIBE://�����Ϲ�
	case TRADE_FUND_ALLOT://�����깺
	case TRADE_FUND_REDEEM://�������
	case TRADE_FUND_WITHDRAW://���𳷵�
	case TRADE_FUND_SETDIVIDEND://���÷ֺ췽ʽ
	case TRADE_FUND_TRANS://����ת��
	case TRADE_FUND_SETRISKLEVEL://���÷��յȼ�
	case TRADE_FUND_OPEN://���𿪻�
	case TRADE_FUND_HOPEALLOT://����ԤԼ�깺
	case TRADE_FUND_HOPEREDEEM://����ԤԼ���
	case TRADE_FUND_7434://����Ͷ����
	case TRADE_FUND_7432://����Ͷ
		{
			pTmp = new CReqFundEntrust;
			break;
		}
	case TRADE_FUND_QUERYHOLD://��ѯ����ֲ�
		{
			pTmp = new CReqQueryFundHold;
			break;
		}
	case TRADE_FUND_QUERYENTRUST://��ѯ������ί��
	case TRADE_FUND_7431://��ѯ����Ͷ
		{
			pTmp = new CReqQueryFundEntrust;
			break;
		}
	case TRADE_QUERYBANK://��ѯ������Ϣ
		{
			pTmp = new CReqQueryBank;
			break;
		}
	case TRADE_BANKTRANSFER://��֤ת��
	case TRADE_QUERYBANKBALANCE://��ѯ�������
		{
			pTmp = new CReqBankStockTransfer;
			break;
		}
	case TRADE_CHANGEPASSWORD://�޸�����
		{
			pTmp = new CReqChangePassword;
			break;
		}
	case TRADE_STOCK_SETCOSTPRICE://���óɱ���
		{
			pTmp = new CReqSetStockCostPrice;
			break;;
		}
	case TRADE_QUERYUSERINFO://��ѯ�û���Ϣ
		{
			pTmp = new CReqQueryUserInfo;
			break;
		}
	case TRADE_MODIFYUSERINFO://�޸��û���Ϣ
		{
			pTmp = new CReqModifyUserInfo;
			break;
		}
	case TRADE_STOCK_QUERYFUNDINFO://��ѯ�����̺�ҵ����Ϣ
		{
			pTmp = new CReqQueryStockFundInfo;
			break;
		}
	case TRADE_RZRQ_ZJHK://ֱ�ӻ���
		{
			pTmp = new CReqStockZJHK;
			break;
		}
	case TRADE_RZRQ_ZJHQ://ֱ�ӻ�ȯ
		{
			pTmp = new CReqStockZJHQ;
			break;
		}
	case TRADE_FUND_QUERYSTOCKACCOUNT://�����˻���ѯ
		{
			pTmp = new CReqQueryFundStockAccount;
			break;
		}
	case TRADE_FUND_QUERYFUNDCOMPANY://����˾��ѯ
		{
			pTmp = new CReqQueryFundCompany;
			break;
		}
	default:
		pTmp = new CReqQueryBase;
	}
	pTmp->SetAction(iAction);
	return pTmp;
}


IReqHeart* CTradeCoreObj::NewReqHeart()
{
	IReqHeart* pTmp = new CReqHeart;
	pTmp->SetAction(TRADE_HEART);
	return pTmp;
}

void CTradeCoreObj::FreeCache()
{
	m_AccountMng->FreeCacheAll();
	if (m_pWndTrading != NULL && m_pWndTrading->m_hWnd != NULL && ::IsWindow(m_pWndTrading->m_hWnd))
	{
		m_pWndTrading->SendMessage(WM_CLOSE);
	}
	if (m_pWndTrading != NULL )
	{
		int nCount = 0;
		delete m_pWndTrading;
		m_pWndTrading = NULL;
	}
}

ITradeTable* CTradeCoreObj::NewColumsByAtionEx(const int iAction)
{
	if (m_pTradeConfigObj)
	{
		return m_pTradeConfigObj->GetTradeTableData(iAction);
	}
	else
		return NULL;

}

void CTradeCoreObj::ThreadSync(ITrading* pTrading)
{
	m_pWndTrading->SendMessage(WM_TRADE_THREAD_ASYNC, 0, (LPARAM)pTrading);
}

void CTradeCoreObj::AccountStatus(IAccount* pAccount)
{
	
	m_pWndTrading->SendMessage(WM_ACCOUNT_STATUS, 0, (LPARAM)pAccount);
}

void CTradeCoreObj::SetTradeTcpView(ITradeTcpView* pTradeTcpView )
{
	m_pTradeTcpView = pTradeTcpView;
}

ITradeTcpView* CTradeCoreObj::GetTradeTcpView()
{
	return m_pTradeTcpView;
}