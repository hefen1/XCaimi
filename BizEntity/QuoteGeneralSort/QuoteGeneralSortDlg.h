/*************************************************************
*	Copyright(c) 2010, ���ݺ�����Ϣ�������޹�˾
*	All rights reserved.
*
*	�ļ����ƣ�	QuoteTableCtrlGeneralSort.h
*	�ļ���ʶ��
*	ժ	  Ҫ��	�����ۺ�������Ի���
*	
*	��ǰ�汾��
*	��	  �ߣ�	
*	������ڣ�
*
*	��	  ע��
*	�޸ļ�¼��
*	��	  �ߣ�
*	�޸����ڣ�
*	�޸����ݣ�
****************************************************************/
#ifndef QUOTEGeneralSortDLG
#define QUOTEGeneralSortDLG

#include "CHSStatic.h"
#include "QuoteGeneralSortTabCtrl.h"

#include "HSBizDialog.h"
#include "DataSourceDefine.h"
#include "QuoteSplitterWnd.h"

#define QuoteReportGeneralSortLoggerName  _T("QUOTEGENERALGeneralSort")
#define RequestGeneralGeneralSortTimerID   0x2000
#define RequestGeneralGeneralSortTimer		5000
typedef CArray<StockUserInfo*, StockUserInfo*> CAyStock;

class QuoteGeneralSortDlg: public CHSBizDialog,public IDataSourceSink
{
    DECLARE_DYNCREATE(QuoteGeneralSortDlg)

public:
	QuoteGeneralSortDlg(CWnd* pParent = NULL);
	~QuoteGeneralSortDlg();
	// �Ի�������
	enum { IDD = IDD_QUOTEGeneralSort_DIALOG};

public:

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧�� 
	BOOL	OnInitDialog();
	void	RequestData();
	void	Draw();
	void    SetSize();
	void	SetMulitStock();
	QuoteTableCtrlGeneralSort* GetWndByAskType(int nAskType,int& nWidIndex);
	void	DrawTitle(CDC* pDC,CRect rect);
	void    RemoveCArray();
	void    SetMarketType(HSMarketDataType type) { m_cGeneralSortRequestType = type;}
	void    SetTitleText(CString strText);
	//��Ϣ���ܽӿ�
	BOOL	HSDataSourceSink_OnCommNotify(void* pData);
	BOOL	HSDataSourceSink_OnRecvData(void* pData, int nLen);

	//�����Ʊ�б���Ϣ
	void RequestStockUserInfo();
	//�����Ʊ�б���Ϣ
	void HandleStockUserInfo(void* pData, int nlen);
	//��ӹ�Ʊ��Ϣ
	void AddStock(StockUserInfo* pStock,int nTotal);
	StockUserInfo* GetStockUserInfo(const char*, HSMarketDataType type);

	DECLARE_MESSAGE_MAP()

private:
	CArray<QuoteTableCtrlGeneralSort*,QuoteTableCtrlGeneralSort*> m_ayQuoteSort;
	IDataSource*	    m_pDataSource;				          // ��������ӿ�ָ��
	long				m_nDataSourceID;			          // ��������ID
	HSMarketDataType	m_cGeneralSortRequestType;             //80-89�г�����				 
	CStaticTitle		m_wndTitle;								//�����
	CQuoteButton		m_wndClose;
	CSize				m_szTitle;								//�����С
	int					m_nRowCount;							//��ͼ�ָ������
	int					m_nColCount;							//��ͼ�ָ������	
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnPaint();


	afx_msg void OnBnClickedButtonClose();
};

#endif