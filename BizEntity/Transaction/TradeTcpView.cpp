// EntrustList.cpp : 实现文件
//

#include "stdafx.h"
#include "Transaction.h"
#include "TradeTcpView.h"
#include "..\..\Common\WinnerApplication.h"
#include "TradeFunc.h"
#include "PackEntry.h"
#include "..\..\ServiceEntity\TradeCore\Ar1Packet.h"
#include <windowsx.h>

CTradePackData::CTradePackData()
{
	m_pBuf = NULL;
	m_nLen = 0;
	m_nThreadId = 0;
	m_nTickCount = 0;
	m_nConnectHandle = 0;
	m_nAction = 0;
	m_nType = 0;
}
CTradePackData::~CTradePackData()
{
	if (m_pBuf)
	{
		delete[] m_pBuf;
	}
}

IMPLEMENT_DYNAMIC(CTcpView, CHSBizDialog)

CTcpView::CTcpView(CWnd* pParent /*=NULL*/)
	: CHSBizDialog(CTcpView::IDD, pParent)
{
	m_pTradeCoreObj = (ITradeCoreObj*) CWinnerApplication::GetObject(OBJ_TRADECORE);
	m_nLastIndex = -1;
}

CTcpView::~CTcpView()
{
	m_pTradeCoreObj->SetTradeTcpView();
	for (int i = 0; i < m_TradePackDataList.GetCount(); i++)
	{
		CTradePackData* pData = m_TradePackDataList.GetAt(i);
		delete pData;
	}
	m_TradePackDataList.RemoveAll();
}

void CTcpView::DoDataExchange(CDataExchange* pDX)
{
	CHSBizDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE_DATA, m_trData);
	DDX_Control(pDX, IDC_ENTRUST, m_lsEntrust);
	DDX_Control(pDX, IDC_SPLITTER, m_stSplitter);
}


BEGIN_MESSAGE_MAP(CTcpView, CHSBizDialog)
	ON_WM_SIZE()
	ON_MESSAGE(WM_TRADE_CALLBACK, OnTradeCallBack)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_ENTRUST, &CTcpView::OnLvnItemchangedEntrust)
END_MESSAGE_MAP()


// CEntrustList 消息处理程序

BOOL CTcpView::OnInitDialog()
{
	CHSBizDialog::OnInitDialog();

	FillTestData();
	//CRect rc;
	//CWnd* pWnd;
	//pWnd = GetDlgItem(IDC_SPLITTER);
	//pWnd->GetWindowRect(rc);
	//ScreenToClient(rc);
	//m_stSplitter.Create(WS_CHILD | WS_VISIBLE, rc, this, IDC_SPLITTER);
	//m_stSplitter.SetRange(50, 50, -1);
	return TRUE;
}

void CTcpView::OnSize(UINT nType, int cx, int cy)
{
	__super::OnSize(nType, cx, cy);
	CRect rcClient;
	GetClientRect(rcClient);
	int nHeight = rcClient.Height();
	int nWidth = rcClient.Width();
	CRect rect;
	CListCtrl* pListCtrl = (CListCtrl*)GetDlgItem(IDC_ENTRUST);
	if (pListCtrl
		&& pListCtrl->GetSafeHwnd()
		&& pListCtrl->IsWindowEnabled())
	{
		pListCtrl->GetClientRect(rect);
		int nLeftWidth = rect.Width();
		pListCtrl->MoveWindow(rect.left, rect.top, nLeftWidth, nHeight);
		if(((GetWindowStyle(pListCtrl->m_hWnd)) & WS_VSCROLL) == WS_VSCROLL)
		{
			//MessageBox("aa");
		}
		//pListCtrl->GetScrollBarCtrl()
		/*m_stSplitter.GetClientRect(rect);
		m_stSplitter.MoveWindow(rect.left + nLeftWidth, rect.top, rect.Width(), nHeight);*/
		nLeftWidth =+ rect.Width();
		m_trData.GetClientRect(rect);
		m_trData.MoveWindow(nLeftWidth, rect.top, nWidth - nLeftWidth, nHeight);
	}	
}


void CTcpView::FillTestData()
{
	m_trData.ModifyStyle(0, TVS_LINESATROOT | TVS_HASBUTTONS
		| TVS_SHOWSELALWAYS | TVS_HASLINES );
	//CListCtrl* pListCtrl = (CListCtrl*)GetDlgItem(IDC_ENTRUST);
	m_lsEntrust.SetExtendedStyle(m_lsEntrust.GetExtendedStyle() | LVS_EX_FULLROWSELECT);
	m_lsEntrust.InsertColumn(0,_T("TickCount"),LVCFMT_LEFT,80);
	m_lsEntrust.InsertColumn(1,_T("线程号"),LVCFMT_LEFT,80);
	m_lsEntrust.InsertColumn(2,_T("连接句柄"),LVCFMT_LEFT,80);
	m_lsEntrust.InsertColumn(3,_T("功能号"),LVCFMT_LEFT,60);
	m_lsEntrust.InsertColumn(4,_T("类型"),LVCFMT_LEFT,40);
	m_lsEntrust.InsertColumn(5,_T("说明"),LVCFMT_LEFT,80);

	/*CString strText;
	int nColumnCount = pListCtrl->GetHeaderCtrl()->GetItemCount();
	for (int i=0;i < 50;i++)
	{
		strText.Format(TEXT("item %d"), i);

		pListCtrl->InsertItem(i, strText);

		for (int j=1;j < nColumnCount;j++)
		{
			strText.Format(TEXT("sub-item %d %d"), i, j);
			pListCtrl->SetItemText(i, j, strText);
		}
	}*/
}

BOOL CTcpView::OnInit()
{
	//RegisterHotKey(1,0,VK_F4);
	//RegisterHotKey(2,MOD_CONTROL,'A');
	//RegisterHotKey(3,MOD_ALT,'A');
	//RegisterLinkageMsg(1000);
	m_pTradeCoreObj->SetTradeTcpView(this);
	return TRUE;
}

void CTcpView::OnLinkageMsg(CLinkageParam* pParam)
{
	if(pParam->m_nMsg = 1000)
	{
// 		CString str(_T("[CEntrustList::OnLinkageMsg]m_strParam="));
// 		str += pParam->m_strParam;
// 		MessageBox(str);

		//CWinnerApplication::FormatLog(DEBUG_LOG_LEVEL,_T("winner"),
		//	_T("[CEntrustList::OnLinkageMsg] name1=%d,name2=%d,name3=%s"),
		//	pParam->GetIntegerField(_T("name1")),
		//	pParam->GetIntegerField(_T("name2")),
		//	pParam->GetStringField(_T("name3")) );
	}
}

void CTcpView::OnHotKey(UINT nId)
{
	//if( nId == 1)
	//{
	//	ActiveSelf();
	//}
	//else if (nId == 2)
	//{
	//	if( IsShow() )
	//	{
	//		CString strMsg;
	//		strMsg.Format("(CEntrustList::OnHotKey)handle=%x: ctrl + a is pressed.",this->m_hWnd);
	//		OutputDebugString(strMsg + "\n");
	//		MessageBox(strMsg);
	//		this->SetFocus();
	//	}	
	//}
	//else if (nId == 3)
	//{
	//	if( IsShow() )
	//	{
	//		CString strMsg;
	//		strMsg.Format("(CEntrustList::OnHotKey)handle=%x: alt + a is pressed.",this->m_hWnd);
	//		OutputDebugString(strMsg + "\n");
	//		MessageBox(strMsg);
	//		this->SetFocus();
	//	}	
	//}
}

void CTcpView::OnShow(BOOL bShow)
{

}

void CTcpView::CallBackData(char* pBuf, UINT nLen, long nConnectHandle, int nAction,int nType)
{
	CTradePackData* pTradePackData = new CTradePackData();
	pTradePackData->m_nTickCount = MyGetTickCount();
	pTradePackData->m_nThreadId = GetCurrentThreadId();
	pTradePackData->m_pBuf = pBuf;
	pTradePackData->m_nLen = nLen;
	pTradePackData->m_nConnectHandle = nConnectHandle;
	pTradePackData->m_nAction = nAction;
	pTradePackData->m_nType = nType;
	m_TradePackDataList.Add(pTradePackData);
	this->PostMessage(WM_TRADE_CALLBACK, 0, (LPARAM)pTradePackData);
}

LRESULT CTcpView::OnTradeCallBack(WPARAM wParam, LPARAM lParam)
{
	CTradePackData* pTradePackData = (CTradePackData*)lParam;
	int nCount = m_lsEntrust.GetItemCount();
	CString strText;
	strText.Format(TEXT("%d"), pTradePackData->m_nTickCount);
	m_lsEntrust.InsertItem(nCount, strText);
	strText.Format(TEXT("%d"), pTradePackData->m_nThreadId);
	m_lsEntrust.SetItemText(nCount, 1, strText);	
	strText.Format(TEXT("%d"), pTradePackData->m_nConnectHandle);
	m_lsEntrust.SetItemText(nCount, 2, strText);	
	strText.Format(TEXT("%d"), pTradePackData->m_nAction);
	m_lsEntrust.SetItemText(nCount, 3, strText);	
	strText.Format(TEXT("%d"), pTradePackData->m_nType);
	m_lsEntrust.SetItemText(nCount, 4, strText);	
	return 1;
}

LRESULT CTcpView::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	/*if (message == WM_NOTIFY)
	{
		if (wParam == IDC_SPLITTER)
		{	
			SPC_NMHDR* pHdr = (SPC_NMHDR*) lParam;
			DoResize(pHdr->delta);
		}
	}*/
	return __super::DefWindowProc(message, wParam, lParam);
}

void CTcpView::DoResize(int delta)
{
	CSplitterControl::ChangeWidth(&m_lsEntrust, delta);
	CSplitterControl::ChangeWidth(&m_trData, -delta, CW_RIGHTALIGN);
	Invalidate();
	UpdateWindow();
}
void CTcpView::OnLvnItemchangedEntrust(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: Add your control notification handler code here
	POSITION pos = m_lsEntrust.GetFirstSelectedItemPosition();
	if (pos != NULL)
	{
		int nCurSel = m_lsEntrust.GetNextSelectedItem(pos);
		if (m_nLastIndex != nCurSel)
		{
			m_trData.DeleteAllItems();
			CTradePackData* pData = m_TradePackDataList.GetAt(nCurSel);
			CPacker Packer;
			if (Packer.ParseData(pData->m_pBuf, pData->m_nLen))
			{
				HTREEITEM hHeader = m_trData.InsertItem("Header");
				CString str;
				str.Format("phAction:%d", Packer.GetHeader(phAction));
				m_trData.InsertItem(str, hHeader);
				str.Format("phCellIndex:%d", Packer.GetHeader(phCellIndex));
				m_trData.InsertItem(str, hHeader);
				str.Format("phOnlineID:%d", Packer.GetHeader(phOnlineID));
				m_trData.InsertItem(str, hHeader);
				str.Format("phOnlineToken:%d", Packer.GetHeader(phOnlineToken));
				m_trData.InsertItem(str, hHeader);
				str.Format("phClientAddr:%d", Packer.GetHeader(phClientAddr));
				m_trData.InsertItem(str, hHeader);
				str.Format("phReturnCode:%d", Packer.GetHeader(phReturnCode));
				m_trData.InsertItem(str, hHeader);
				str.Format("phSenderID:%d", Packer.GetHeader(phSenderID));
				m_trData.InsertItem(str, hHeader);
				m_trData.Expand(hHeader, TVE_EXPAND);
				HTREEITEM hNVPack = m_trData.InsertItem("NVPack");
				for (int i = 0; i < Packer.GetNVCount(); i++)
				{
					int iType;
					CString sName;
					sName = Packer.GetNVNameByIndex(i, iType);
					if (ptNV == (EPacketType)iType)
					{
						str = sName + ":" + Packer.GetStr(sName);
						m_trData.InsertItem(str, hNVPack);
					}					
				}
				m_trData.Expand(hNVPack, TVE_EXPAND);
				HTREEITEM hArPack = m_trData.InsertItem("ArPack");
				int iDataLen = 0;	
				CString sName = "";
				if (0 == pData->m_nType)
				{
					sName = Packer.GetStr(S_BIN_REQ);
					if (sName.IsEmpty())
					{
						sName = S_BIN_DATA;
					}
				}
				else
				{
					sName = Packer.GetStr(S_BIN_ANS);
					if (sName.IsEmpty())
					{
						sName = S_BIN_DATA_ANS;
					}
				}
				
				char* pAr1Buf = NULL;
				pAr1Buf = Packer.GetData(sName.MakeLower(), iDataLen);
				if (iDataLen > 0)
				{
					CAr1UnPacker Ar1UnPacker;
					if (Ar1UnPacker.open(pAr1Buf, iDataLen))
					{
						HTREEITEM hArHeader = m_trData.InsertItem(sName, hArPack);
						AR1PacketHeader* pAR1PacketHeader = Ar1UnPacker.GetHeader();
						str.Format("HeaderVersion:%d", pAR1PacketHeader->HeaderVersion);
						m_trData.InsertItem(str, hArHeader);
						str.Format("PacketType:%d", pAR1PacketHeader->PacketType);
						m_trData.InsertItem(str, hArHeader);
						str.Format("Branch:%d", pAR1PacketHeader->Branch);
						m_trData.InsertItem(str, hArHeader);
						str.Format("FunctionNo:%d", pAR1PacketHeader->FunctionNo);
						m_trData.InsertItem(str, hArHeader);
						str.Format("SenderID:%d", pAR1PacketHeader->SenderID);
						m_trData.InsertItem(str, hArHeader);
						str.Format("AddressLevel:%d", pAR1PacketHeader->AddressLevel);
						m_trData.InsertItem(str, hArHeader);
						str.Format("DataLength:%d", pAR1PacketHeader->DataLength);
						m_trData.InsertItem(str, hArHeader);
						str.Format("ReturnCode:%d", pAR1PacketHeader->ReturnCode);
						m_trData.InsertItem(str, hArHeader);
						m_trData.Expand(hArHeader, TVE_EXPAND);
						HTREEITEM hSections = m_trData.InsertItem("Sections:0", hArHeader);
						str.Format("RecordCount:%d;FieldCount:%d", Ar1UnPacker.GetRowCount(), Ar1UnPacker.GetColCount());
						m_trData.InsertItem(str, hSections);
						m_trData.Expand(hSections, TVE_EXPAND);
						while(!Ar1UnPacker.isEof())
						{
							HTREEITEM hRecord = m_trData.InsertItem("Record", hSections);
							CMapStringToString* lpAr1Item = Ar1UnPacker.GetItem();
							if (lpAr1Item)
							{
								POSITION ipos = lpAr1Item->GetStartPosition();
								while(ipos)
								{
									CString sKey = "";
									CString sValue = "";
									lpAr1Item->GetNextAssoc(ipos,sKey,sValue);
									str.Format("%s:%s", sKey, sValue);
									m_trData.InsertItem(str, hRecord);
								}
							}		
							Ar1UnPacker.Next();
							m_trData.Expand(hRecord, TVE_EXPAND);
						}						
					}
					delete[] pAr1Buf;
				}
				m_trData.Expand(hArPack, TVE_EXPAND);
			}					
			m_nLastIndex = nCurSel;
		}
	}
	*pResult = 0;
}
