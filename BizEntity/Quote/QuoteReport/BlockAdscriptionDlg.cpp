#include "StdAfx.h"
#include "BlockAdscriptionDlg.h"
#include "WinnerApplication.h"
#include "HSMessage.h"
#include "QuoteComm\QuoteDefine.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CBlockAdscriptionDlg::CBlockAdscriptionDlg(CWnd* pParent /* = NULL */)
: CHSBizDialog(CBlockAdscriptionDlg::IDD,pParent)
{
	m_bFreeSelf = FALSE;
	m_iQuoteLang = (IQuoteLang*)CWinnerApplication::GetObject(OBJ_QUOTELANG);
	m_iBlock = (IBlockManager*)CWinnerApplication::GetObject(OBJ_BLOCKMANAGER);
	m_pDataSource = (IDataSourceEx*)CWinnerApplication::GetObject(OBJ_HSDATAENGINE);
	m_stockInfo.SetEmpty();
}

CBlockAdscriptionDlg::~CBlockAdscriptionDlg(void)
{
}


IMPLEMENT_DYNAMIC(CBlockAdscriptionDlg, CHSBizDialog)

void CBlockAdscriptionDlg::DoDataExchange(CDataExchange* pDX)
{
	CHSBizDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_BLOCK_LIST,m_wndBlockList);
	DDX_Control(pDX, IDC_REFRESH, m_wndRefresh);
}


long CBlockAdscriptionDlg::Reset()
{
	m_wndBlockList.DeleteAllItems();
	CString strBlock;
	CStringArray ayBlock;
	m_iBlock->GetStockAdscription(&m_stockInfo.m_ciStockCode, ayBlock);
	for (int i=0; i<ayBlock.GetCount(); i++)
	{
		strBlock = ayBlock.GetAt(i);
		int nCount = m_iBlock->GetStockListCount(strBlock);
		m_wndBlockList.InsertItem(i,strBlock);
		strBlock.Format("%d",nCount);
		m_wndBlockList.SetItemText(i,1,strBlock);
	}
	//////////////////////////////////////////////////////////////////////////
	// 更改标题
	// 暂无
	//

	return ayBlock.GetCount();
}

BEGIN_MESSAGE_MAP(CBlockAdscriptionDlg, CHSBizDialog)
	ON_BN_CLICKED(IDC_REFRESH, &CBlockAdscriptionDlg::OnBnClickedRefresh)
	ON_NOTIFY(NM_DBLCLK, IDC_BLOCK_LIST, &CBlockAdscriptionDlg::OnNMDblclkBlockList)
END_MESSAGE_MAP()

void CBlockAdscriptionDlg::OnLinkageMsg(CLinkageParam* pParam)
{
	if (pParam->m_nMsg == QUOTE_MESSAGE_BLOCK_ADSCRIPTION)
	{
		CString strStockCode = pParam->GetStringField(PARAM_BLOCK_ADCRIPTION_CODE);
		int     strStockType = pParam->GetIntegerField(PARAM_BLOCK_ADCRIPTION_CODETYPE);

		CodeInfo codeInfo(strStockType, strStockCode);
		SetStockInfo(&codeInfo);
	}
}

void CBlockAdscriptionDlg::SetStockInfo(CodeInfo* pCodeInfo)
{
	if (pCodeInfo)
	{
		CArray<CodeInfo> ayCodeInfo;
		ayCodeInfo.Add(*pCodeInfo);
		CArray<StockUserInfo> ayStockInfo;
		m_pDataSource->HSDataSourceEx_GetStocksUserInfo(ayCodeInfo, ayStockInfo);
		if (ayStockInfo.GetCount() == 1)
		{
			m_stockInfo.Copy(&ayStockInfo.GetAt(0));
		}
		//m_codeInfo.Copy(pStock);
		//m_strTitle.Format("%s (%s) %s",pStock->GetName(),pStock->GetCode(),m_iQuoteLang->LoadStr(IDS_BLOCK_ADSCRIPTION));
	}
	else
		m_stockInfo.SetEmpty();

	Reset();
}

BOOL CBlockAdscriptionDlg::OnInitDialog()
{
	CHSBizDialog::OnInitDialog();

	CRect rcList;
	CString strBlock = m_iQuoteLang->LoadStr(IDS_BLOCK_ADSCRIPTION);
	m_wndBlockList.GetClientRect(rcList);
	m_wndBlockList.SetExtendedStyle(m_wndBlockList.GetExtendedStyle()|LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES|LVS_EX_UNDERLINEHOT | LVS_EX_ONECLICKACTIVATE);
	m_wndBlockList.InsertColumn(0,strBlock,LVCFMT_LEFT,rcList.Width()-50);
	m_wndBlockList.InsertColumn(1,m_iQuoteLang->LoadStr(IDS_BLOCK_COUNT),LVCFMT_CENTER,40);
//	Reset();
	return TRUE;
}

BOOL CBlockAdscriptionDlg::OnInit()
{
	return RegisterLinkageMsg(QUOTE_MESSAGE_BLOCK_ADSCRIPTION);
}

void CBlockAdscriptionDlg::OnBnClickedRefresh()
{
	Reset();
}

void CBlockAdscriptionDlg::OnNMDblclkBlockList(NMHDR *pNMHDR, LRESULT *pResult)
{
	POSITION pos = m_wndBlockList.GetFirstSelectedItemPosition();
	if(NULL == pos)
		return;

	int listIndex = (int)m_wndBlockList.GetNextSelectedItem(pos);
	CString strText = m_wndBlockList.GetItemText(listIndex, 0);
	if (!strText.IsEmpty())
	{
		CLinkageParam param;
		CString strMsgID("");
		strMsgID.Format("%d", DiagramMsg_BlockChange);
		param.AddParam(PARAM_DIAGRAMMSGID, strMsgID);         // 添加消息号
		param.AddParam(PARAM_DIAGRAM_LAYOUT_NAME, Layout_QuoteReport);// 添加消息号
		param.AddParam(Param_BlockChange_BlockName, strText); // 板块名称
		this->SendLinkageMsg(QUOTE_MESSAGE_NOTIFY_DIAGRAM_LAYOUT, &param);
	}
}
