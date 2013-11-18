/*************************************************************
*	Copyright(c) 2010, ���ݺ����ɷ����޹�˾
*	All rights reserved.
*
*	�ļ����ƣ�	CDrawQuoteReport.h
*	�ļ���ʶ��
*	ժ	  Ҫ��  ��ѯͼԪ
*	
*	��ǰ�汾��
*	��	  �ߣ�	����
*	������ڣ�  2011-3-17
*
*	��	  ע��
*	�޸ļ�¼��
*	��	  �ߣ�
*	�޸����ڣ�
*	�޸����ݣ�
*
****************************************************************/
#pragma once
#include "DrawTableBase.h"
#include "ConfigInterface.h"
#include "DrawQuoteReportStruct.h"
#include "DrawQuoteTabCtrl.h"
#include "QuoteLangMngInterFace.h"

#define QUOTEREPORT_ROW_HEIGHT       22  // �и�
#define DELAY_REQUEST_TIMESPAN      100  // �����ӳ�
#define REFRESH_DISAPPEAR_TIMESPAN	200	 // ˢ�¸�����Ԫ��䵭һ��ʱ��
#define QUOTEREPORT_REFRESH_TIMES     5  // ˢ�´���

#define QUOTEREPORT_SCROLL_VERT_SIZE   20  // ��ֱ������
#define QUOTEREPORT_SCROLL_HORT_WIDTH 400  // ˮƽ���������
#define QUOTEREPORT_TAB_HEIGHT         20  // ��ǩҳ�߶� 

class CDrawQuoteReport :
	public CDrawTableBase,
	public IDrawQuoteTabNotify
{
public:
	CDrawQuoteReport(void);
	~CDrawQuoteReport(void);

	BOOL CreateSelf(); // �������鱨�۱��
    long GetMarketNameByType(const PageInfo* pvInfo,CStringArray& strMarketNames);
	BOOL SetPageInfo(PageInfo* pInfo,CString strColGroupName = _T("")); // ��Ҫ���ڸı䵱ǰҳ��Ʊ����
	BOOL SetPageViewInfo(PageInfo* pInfo, CString strColGroupName = "", BOOL bRequest = FALSE);    // ���õ�ǰҳ������
	
	BOOL InitColGroup(CString strGroupName, BOOL bRefreshData = FALSE ); // ��ʼ����
	void InitRowData(); // ��ʼ����
	void InitFixedDataItem(int nFirst,int nCount);      // ��ʼ���̶���
	void SetFixedData(int nRow, StockUserInfo* pStock); // ����ĳһ�еĹ̶�����

	void RequestStockUserInfo(PageInfo* pPageInfo = NULL); // �����Ʊ����

	BOOL AddColumn(UINT nID, UINT nWidth = 0); // ����һ��
	static BOOL IsFixedColumn(UINT nID);   // ͨ����ID�ж� �Ƿ�Ϊ�̶���
	int  GetColIndexByID(UINT nID);        // ͨ����ID��ȡ��λ��(δ�ƶ�)
	UINT GetColIDByIndex(int nCol);        // ͨ��(δ�ƶ�)��λ�û�ȡ��ID

	// �ı䵱ǰҳ��
	int ChangeContent(PageInfo* pvInfo, CArray<StockUserInfo>& ayStock, CString strColGroup = "", BOOL bForceRefresh = FALSE, BOOL bRequestData = TRUE);
	int ChangeContent(PageInfo* pvInfo, StockUserInfo* pStock = NULL, int nTotal = 0, CString strColGroup = "", BOOL bForceRefresh = FALSE, BOOL bRequestData = TRUE);

	void EmptyStock(); // ��մ������
	// ���ù�Ʊ�������
	void SetStock(CArray<StockUserInfo>& payStock);
	void SetStock(StockUserInfo* pStock,int nTotal);
	StockUserInfo* GetStock(int nPos); // ��ȡ�����й�Ʊ����
	StockUserInfo* FindRowByCode(CodeInfo* pCode, int& nPos); // ���ݴ��������

	void OnColGroupChange(UINT nID);    // �л���ʾ��ͷ��
	void OnColumnSetting();				// �趨��ʾ��ͷ����

	void UpdateRefreshMap();            // ����ˢ��map            
	//////////////////////////////////////////////////////////////////////////
	// ������������
	void KillAllTimer();  // ������ж�ʱ��
	void RequestRealTimeData(int nStart, int nTotal); // ����ʵʱ�����Լ�ͬʱ����������Ϣ
	void RequestSortData(PageInfo* pPageInfo = NULL, BOOL bNeedAutoPush = TRUE); // ������������
	static VOID CALLBACK RefreshProc(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime);                    // ������ʾ
	static VOID CALLBACK DelayRealTimeRequestTimerProc(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime ); // ʵʱ��������
	static VOID CALLBACK DelaySortRequestTimerProc(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime);      // ������������
	
	//////////////////////////////////////////////////////////////////////////
	// ������������
	void HandleNowData(DataHead* pHead, const CommRealTimeData_Ext* pnowData, int nSize); // ʵʱ���ݴ���
	void HandleNowData(DataHead* pHead, const CommRealTimeData* pnowData, int nSize);     // ʵʱ���ݴ���
	void HandleAutoPushData(DataHead* pHead, const CommRealTimeData_Ext* pnowData, int nSize); // �������ݴ���
	void HandleAutoPushData(DataHead* pHead, const CommRealTimeData* pnowData, int nSize);     // �������ݴ���
	void HandleSortData(DataHead* pHead, const AnsReportData* pReportData); // �������ݴ���
	void UpdateDataEntry(int nRow, StockUserInfo* pStock, HSStockRealTime_Ext* pData, StockOtherData* pOtherData); // ����������ں���
	void UpdateStockData(int nRow, StockUserInfo* pStock, HSStockRealTime_Ext* pData, StockOtherData* pOtherData); // ���¹�Ʊ
	void UpdateIndexData(int nRow, StockUserInfo* pStock, HSIndexRealTime* pData, StockOtherData* pOtherData);
	WORD GetColor(double dNewValue, double dPreValue); // �Ƚ�ǰ������ֵ �õ���ɫID	
    void UpdateCellText(int nCellID, int nItemID, const char* strText, WORD lClr = 0, WORD lFont = 0, DWORD lMark = 0, DWORD lDelMark = 0, BOOL bNeedRedraw = FALSE);
	//////////////////////////////////////////////////////////////////////////
	// �������
	void SetSortColumn(int nCol);		        // �趨������
	int  CancelSort();                          // ȡ������
	BOOL GetDefaultSortTypeByCol(int nCol);     // ��ȡĬ������״̬
	BOOL GetDefaultSortTypeByColID(UINT nColID);// ��ȡĬ������״̬
	BOOL IsSupportSortByCol(int nCol);          // ��ǰ���Ƿ�֧������
	BOOL IsSupportSortByColID(UINT nColID);     // ��ǰ���Ƿ�֧������
	BOOL IsSupportLocalSortByCol(int nCol);     // ��ǰ���Ƿ�֧�ֱ�������
	BOOL IsSupportLocalSortByColID(UINT nColID);// ��ǰ���Ƿ�֧�ֱ�������
	BOOL GetSortInfoByPageInfo(IN const PageInfo* pvInfo, IN short nID, OUT int& nCount,OUT HSMarketDataType& cCodeType); //����ҳ����Ϣ��ȡ��������
	BOOL Sort(UINT nColID, BOOL bAscending, BOOL bForce = FALSE);  // ����ĳ��
	//////////////////////////////////////////////////////////////////////////
	// ��ӱ�����
	void OnMarkCodeByIcon(UINT nID, int nRow);  // ���ͼ����
	void OnMarkCodeByString(int nRow);          // ������ֱ��
	CString GetMarkContent(CPoint& point);      // ��ȡ�û����������
	void OnCancelMarkCode(int nRow);            // ȡ�����
	//////////////////////////////////////////////////////////////////////////
	// ������
	void OnBlockAdscription(int nRow);          // �������
	void OnAddToSelfBlock(int nRow);            // ������ѡ���
	void OnBlockDelete(int nRow);               // ����ѡ���ɾ��
	//////////////////////////////////////////////////////////////////////////
	// ҳ���л�
	void GotoDealDetail(int nRow);              // �л���F1
	void GotoPriceBook(int nRow);               // �л���F2
	void GotoFinanceInfo(int nRow);             // �л�����������
	void GotoTechAnalysis(int nRow, int nMark = 1); // �л�����ʱ(nMark = 1)����K��(nMark = 2)
	//////////////////////////////////////////////////////////////////////////
	// ��Ӧ���̾�����Ϣ
	int  GetMarketType(LPARAM lparam, BOOL& bSort);   // ������̾���Ĳ����������г����ͣ������Ƿ�Ҫ�Ƿ�����
	void EnSureVisible(int nColID);                   // ȷ��������ʾ����
	//////////////////////////////////////////////////////////////////////////
// ��д����ĺ���
	virtual void    MoveCol(int nBegin, int nEnd); // �ڻ���Ļ����ϣ���Ҫʵ����ID����λ�õĸı�
	virtual BOOL    Scroll(int nDiff, BOOL bIsVert = TRUE, BOOL bReDraw = TRUE);     // �ڻ���Ļ����ϣ�ʵ�ַ��͹�Ʊ����
	virtual BOOL    SetPosition(int nPosition, BOOL bIsVert, BOOL bIsRedraw = TRUE); // �ڻ���Ļ����ϣ�ʵ�ַ��͹�Ʊ����
	virtual void    OnFixedRowClick(); // ���û��������̶���
	virtual BOOL    DrawSelf(CDC* pDC, CPoint pos, int nItemID, CRect& rcItem); // �Զ����ͼ
	virtual CString GetTipText(int nCellID, int nItemID);
//interface
	virtual BOOL    Draw(CDC* pDC);
	virtual BOOL    SetRect(CRect rc);
	virtual BOOL    GetRequestData(IRequestCollector* pReqCollector,StockUserInfo* pStockInfo, BOOL bForce = FALSE);
	virtual int	    OnUpdateData(DataHead* pData);
	virtual BOOL    MouseMove(UINT nFlags, CPoint point, hsRefMouseMsg* pData = NULL);
	virtual BOOL    MouseDown(UINT nFlags, CPoint point, hsRefMouseMsg* pData = NULL);
	virtual BOOL    MouseDBDown(UINT nFlags, CPoint point, hsRefMouseMsg* pData = NULL);
	virtual BOOL    RButtonDown(UINT nFlags,CPoint point, HWND hWndParent, hsRefMouseMsg* pData = NULL);
	virtual int	    OnKey(UINT nChar, UINT nRepCnt, UINT nFlags);
	virtual void    OnLayoutNotify(UINT nMsgId,IDrawBaseParam* pParam);
	virtual BOOL    OnKeyBoardNotify(WPARAM wParam, LPARAM lParam);
	//////////////////////////////////////////////////////////////////////////
	// ʵ�ֱ�ǩҳ�ص�����
	virtual void    OnActiveTabChange(CDrawQuoteTab* pActiveItem);
	virtual void    TabInvalidate(CRect* pRect, BOOL bErase = TRUE);
	virtual CWnd*   TabGetCWnd();
protected:
	IHsColumnProperty*		 m_iColProp;	    // ���������ýӿ�
	IHsTabCfg*               m_iTab;            // ��ǩҳ���ýӿ�
	IDataSourceEx*	         m_pDataSource;     // ��������ӿ�ָ��
	IBlockManager*			 m_iBlockManager;	// ������
	IHsColor*                m_pColor;          // ��ɫ���ýӿ�
	IHsFont*                 m_pFont;           // �������ýӿ�
	IHsUserDataCfg*          m_iUserDefCfg;     // �û����ݽӿڣ���Ʊ��ǣ�
	IQuoteLang*				 m_iQuoteLang;		// ������Դ�ӿ�
	
	CDrawQuoteTabCtrl*       m_pQuoteTabCtrl;   // ��ǩҳ

	PageInfo    	            m_pviInfo;		// ��ǰҳ����Ϣ
	CMap<UINT, UINT, int, int>  m_mapColumn;    // ��¼��ID��λ�õ�map key:��ID, value:��λ�� 
	CArray<StockUserInfo*,StockUserInfo*>	m_ayStock;	   	    // ��ǰ��ʾ�����й�Ʊ������Ϣ
	CMap<UINT,UINT,CString,CString>		    m_mapTempColGroup;	// ����ʱ����ͷ�����Ʋ˵���Ӧ��
	CMap<int,int,int,int>					m_mapRefreshCells;	// ����ˢ�µĵ�Ԫ���б�
//	CMapStringToPtr                         m_mapCodeInfo;      // ��¼��ǰҳ��CArray<CurrentFinanceData>              m_ayFinanceData;    // ��������

	CImageList*     m_pImageList;           // ��Ʊ���ͼ��

	BOOL			m_bNeedRequestSort;	    // �Ƿ���Ҫ�����������ݣ�ĳЩ�в���Ҫ�������������Ƶȣ�
	CString			m_strCurColGroup;		// ��ǰ��ͷ������
	CString			m_strPreColGroup;	    // ǰһ����ͷ������

	int             m_nCurActiveRow;        // ��ǰѡ����
	int				m_nDelayRealTimeRequestTimerID;		// ��ʱʵʱ��������ʱ��ID	
	int				m_nDelaySortRequestTimerID;			// ��ʱ��������ʱ��ID
	int				m_nRefreshTimerID;			        // ˢ�¶�ʱ��ID

	BOOL            m_bNeedRefresh;         // �Ƿ���Ҫˢ��
	int             m_nSortColumnID;        // ��������ID
	BOOL            m_bAscending;           // ��ǰ����ķ���

	DWORD  m_nTickCount; // for test
};