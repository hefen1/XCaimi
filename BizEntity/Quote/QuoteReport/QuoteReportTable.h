#pragma once

#include "quotestuctdefine.h"
#include "QuoteSort.h"
#include "DataSourceDefineEx.h"
#include "hsgridtable.h"
#include "skinscrollbar.h"
#include "..\..\..\HelperLib\GeneralHelper.h"
#include "QuoteLangMngInterFace.h"
#include "ConfigInterface.h"

#define INIT_FIXED_HEAD			0x00010000
#define INIT_FIXED_DATA			0x00020000 // һ�㱨��(1~6)
#define INIT_SORTRPT_DATA		0x00040000 // ����(61~66)
#define INIT_FSDetail_DATA		0x00080000 // ��ʱ��ϸ��
#define INIT_PanHouDetail_DATA  0x00100000 // �̺���ϸ��
#define INIT_BigDetail_DATA		0x00200000 // ������ϸ��
#define INIT_FENJIA_DATA		0x00400000 // �ּ۱�

#define DELAY_REQUEST_TIMESPAN		100		// ��ʱ����ʱ����
#define REFRESH_DISAPPEAR_TIMESPAN	100		// ˢ�¸�����Ԫ��䵭һ��ʱ��
#define REFRESH_DISAPPEAR_TIMES		20		// ˢ�¸�����Ԫ����ʧʱ�䣨TIMES * REFRESH_DISAPPEAR_TIMESPAN��

//////////////////////////////////////////////////////////////////////////����������
class CQuoteSort
{
public:
	CQuoteSort(void);
	~CQuoteSort(void);

	void Free();
	double GetData(int nRow,StockUserInfo* pStock = NULL);
	BOOL IsValid() { return (m_pData != NULL && m_nDataCount > 0 && m_bAscending != -1); }
	BOOL SetCWSortData();
	BOOL SetHQSortData();
	BOOL SetOtherSortData();

	HSSortData* Sort(int nBegin,int nEnd,
		CArray<StockUserInfo*,StockUserInfo*>* pStock,int nColumn,
		BOOL bAscending = TRUE);


	int									   m_nColumn;
	CArray<StockUserInfo*,StockUserInfo*>* m_pStock;

	HSSortData*		m_pData;
	int				m_nDataCount;

	CHSGridCtrl*	m_pGridCtrl;
	int				m_nFixedRow;

	BOOL			m_bAscending;

};

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////���鱨��
// CQuoteReportTable

class CQuoteReportTable : public CHSGridTable
{
	DECLARE_DYNAMIC(CQuoteReportTable)

public:
	CQuoteReportTable(IDataSourceEx* pDataSouce, long nDataSourceID, int nRows = 0, int nCols = 0, int nFixedRows = 0, int nFixedCols = 0);
	virtual ~CQuoteReportTable();


	//////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////////���ز���
	// �趨ָ���п� ע�⣺nWidthΪӢ�İ���ַ���,��Ҫͨ������õ�������
	virtual int SetColumnWidth(int nColID,int nWidth = -1);

	CString GetMenuName(int nType = 0, int nSubType = 0);
	CString GetMenuName(UINT nMenuID);

	virtual BOOL ShowRow(int row, BOOL bRefresh = FALSE);	// ��ʾָ����
	virtual BOOL ShowCol(int col, BOOL bRefresh = FALSE);	// ��ʾָ����

	virtual BOOL MoveRowTo(int nSrc,int nDes);		// �ƶ�ָ���е�Ŀ����
	virtual BOOL MoveColTo(int nSrc,int nDes);		// �ƶ�ָ���е�Ŀ����s

	virtual void OnFixedRowClick(CCellID& cell);	// ����̶��У�����
	virtual void SetSortColumn(int nCol);			// �趨������

	virtual void GetClientRect(LPRECT lpRect);

	virtual void OnResizeColWidth(int nCol);	// ��Ӧ�����п�
	//////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////�У�ͷ������
	/********************************************************************************
	* �������� : ������ͷ�����Ƴ�ʼ����ͷ
	* �������� : CString strGroupName : ��ͷ������
	*			 BOOL bRefreshData : �Ƿ�ͬʱˢ������
	*  ����ֵ  : BOOL : �ɹ�ΪTRUE,ʧ��ΪFALSE
	* �������� : 
	* ��    �� : lxqi
	* ������� : 2010-08-04
	* �޸ļ�¼ : 
	*******************************************************************************/
	virtual BOOL InitColGroup(CString strGroupName, BOOL bRefreshData = FALSE);

	/********************************************************************************
	* �������� : ɾ��ָ����Χ����ͷ
	* �������� : int nBegin : ��ɾ���������ʼֵ
	*			 int nEnd : ��ɾ���������ֵֹ
	*  ����ֵ  : int ��ǰ������ʣ����ͷ����
	* �������� : 
	* ��    �� : lxqi
	* ������� : 2010-08-04
	* �޸ļ�¼ : 
	*******************************************************************************/
	virtual int DelColHead(int nBegin = -1,int nEnd = -1);

	/********************************************************************************
	* �������� : ������ID�������֮��Ķ�Ӧ��
	* �������� : 
	*  ����ֵ  :
	* �������� : 
	* ��    �� : lxqi
	* ������� : 2010-08-04
	* �޸ļ�¼ : 
	*******************************************************************************/
	void ResetCorrespondMap();

	/********************************************************************************
	* �������� : ������ID��ȡ��ͷ���
	* �������� : UINT uiID : ��ID
	*  ����ֵ  : int ��ͷ���
	* �������� : 
	* ��    �� : lxqi
	* ������� : 
	* �޸ļ�¼ : 
	*******************************************************************************/
	virtual int GetColIndexByID(UINT uiID);

	/********************************************************************************
	* �������� : ������ͷ��Ż�ȡ��ID
	* �������� : int nPos : ��ͷ���
	*  ����ֵ  : UINT ��ID
	* �������� : 
	* ��    �� : lxqi
	* ������� : 
	* �޸ļ�¼ : 
	*******************************************************************************/
	virtual UINT GetColHeadID(int nPos = 0);

	/********************************************************************************
	* �������� : �����
	* �������� : UINT uiID : ��ID
	* �������� : 
	* ��    �� : lxqi
	* ������� : 2010-08-09
	* �޸ļ�¼ : 
	*******************************************************************************/
	virtual void AddColumn(UINT nID);


	/********************************************************************************
	* �������� : ��ȡ��ͷ����ָ��
	* �������� : int nType : ��ʽ��1.ϵͳ���õ���ͷ���� 2.���⴦�����ͷ���� 3.ϵͳ���õĶ�̬������壩
	*  ����ֵ  : 
	* �������� : ������ͷ���ʱ���á�����ͷʱ����
	* ��    �� : lxqi
	* ������� : 2010-08-09
	* �޸ļ�¼ : 
	*******************************************************************************/
	virtual CFont* GetColHeadFontPtr(int nType);

	/********************************************************************************
	* �������� : ���м������������
	* �������� : int nID : ��ID
	*			 int nColPos : �����
	*  ����ֵ  : BOOL : TRUE ֧������
	*					FALSE ��֧������
	* �������� : 
	* ��    �� : ������
	* ������� : 
	* �޸ļ�¼ : 
	*******************************************************************************/
	void AddColToMap(int nID,int nColPos);

	void SaveCurDispColHead();

	static BOOL IsFixedColumn(int nID);
	//////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////�������
	/********************************************************************************
	* �������� : ����ID����
	* �������� : UINT uiID : ��ID
	*			 BOOL bAscending : ����ʽ��TRUE ����FALSE ����
	*  ����ֵ  : 
	* �������� : 
	* ��    �� : lxqi
	* ������� : 2010-08-09
	* �޸ļ�¼ : 
	*******************************************************************************/
	virtual void Sort(int nID, BOOL bAscending, BOOL bForce = FALSE);

	/********************************************************************************
	* �������� : �Ƿ�֧������
	* �������� : int nCol : �����
	*			 WORD nColID : ��ID
	*  ����ֵ  : BOOL : TRUE ֧������
	*					FALSE ��֧������
	* �������� : 
	* ��    �� : ������
	* ������� : 
	* �޸ļ�¼ : 
	*******************************************************************************/
	virtual BOOL IsSupportSort(int nCol);
	virtual BOOL IsSupportSort(WORD nColID);

	/********************************************************************************
	* �������� : ����Ĭ������ʽ
	* �������� : int nCol : �����
	*			 WORD nColID : ��ID
	*  ����ֵ  : BOOL : TRUE ��������
	*					FALSE ��������
	* �������� : 
	* ��    �� : ������
	* ������� : 
	* �޸ļ�¼ : 
	*******************************************************************************/
	virtual BOOL GetDefaultSortType(int nCol);
	virtual BOOL GetDefaultSortType(WORD nColID);

	/********************************************************************************
	* �������� : ����ҳ����Ϣ��ȡ��������
	* �������� : IN const PageViewInfo* pvInfo : ҳ������
	*			 IN short nID : δ��
	*			 OUT int& nCount : �����Ʊ����
	*			 OUT HSMarketDataType& cCodeType : �����г�����
	*  ����ֵ  : BOOL : TRUE ��ȷ��ȡ
	*					FALSE û�л�ȡ
	* �������� : 
	* ��    �� : ������
	* ������� : 
	* �޸ļ�¼ : 
	*******************************************************************************/
	virtual BOOL GetSortInfoByPageInfo(IN const PageViewInfo* pvInfo, IN short nID, OUT int& nCount,OUT HSMarketDataType& cCodeType);

	/********************************************************************************
	* �������� : ȡ������
	* �������� : 
	*  ����ֵ  : int : 0
	* �������� : 
	* ��    �� : ������
	* ������� : 
	* �޸ļ�¼ : 
	*******************************************************************************/
	virtual int CancelSort();

	//////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////��Ʊ�����б����
	/********************************************************************************
	* �������� : ��ȡָ��λ�ù�Ʊ����
	* �������� : int nPos : λ���������
	*  ����ֵ  : 
	* �������� : 
	* ��    �� : lxqi
	* ������� : 2010-08-09
	* �޸ļ�¼ : 
	*******************************************************************************/
	virtual StockUserInfo* GetStock( int nPos );
	/********************************************************************************
	* �������� : ���õ�ǰ��Ʊ���루����ǰ����գ�
	* �������� : CArray<StockUserInfo*,StockUserInfo*>& payStock : ��Ʊ������������
	*			 StockUserInfo* pStock : ��Ʊ�����һ����Աָ��
	*			 int nTotal : ��Ʊ����
	*  ����ֵ  : 
	* �������� : 
	* ��    �� : lxqi
	* ������� : 2010-08-09
	* �޸ļ�¼ : 
	*******************************************************************************/
	virtual void SetStock(CArray<StockUserInfo*,StockUserInfo*>& payStock);
	virtual void SetStock(StockUserInfo* pStock,int nTotal);
	
	/********************************************************************************
	* �������� : ��յ�ǰ��Ʊ����
	* �������� : int nType : ��շ�ʽ
	*  ����ֵ  : 
	* �������� : 
	* ��    �� : lxqi
	* ������� : 2010-08-09
	* �޸ļ�¼ : 
	*******************************************************************************/
	void EmptyStock(int nType = 0);

	/********************************************************************************
	* �������� : ���ݴ��������
	* �������� : CodeInfo* pCode : ��Ʊ������Ϣ
	*			 OUT int& nPos : �ҵ��������
	*  ����ֵ  : StockUserInfo* : ��Ʊ��Ϣ
	* �������� : 
	* ��    �� : ������
	* ������� : 
	* �޸ļ�¼ : 
	*******************************************************************************/
	StockUserInfo* FindRowByCode(CodeInfo* pCode, OUT int& nPos);



	//////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////ҳ�棨���ԣ�����
	/********************************************************************************
	* �������� : �ı䵱ǰҳ��
	* �������� : PageViewInfo* pvInfo : ҳ������
	*			 BOOL bForceRefresh : ��ҳ��������ͬʱ�Ƿ�ǿ��ˢ��ҳ��������Ϣ
	*  ����ֵ  : 
	* �������� : 
	* ��    �� : lxqi
	* ������� : 2010-08-09
	* �޸ļ�¼ : 
	*******************************************************************************/
	virtual int ChangeContent(PageViewInfo* pvInfo, StockUserInfo* pStock = NULL, int nTotal = 0,CString strColGroup = _T(""), BOOL bForceRefresh = FALSE, BOOL bRequestData = TRUE);

	/********************************************************************************
	* �������� : �趨��ǰҳ����Ϣ
	* �������� : PageViewInfo* pvInfo : ҳ������
	*			 CString strColGroupName : ��ͷ����
	*			 BOOL bRequest : �Ƿ�ͬʱ��������
	*  ����ֵ  : 
	* �������� : 
	* ��    �� : lxqi
	* ������� : 2010-08-09
	* �޸ļ�¼ : 
	*******************************************************************************/
	virtual void SetPageViewInfo(PageViewInfo* pvInfo, CString strColGroupName = _T(""), BOOL bRequest = FALSE);

	/////////////////////////////////////////////////////////////////////////////////////////////������ʾ��ز���
	/********************************************************************************
	* �������� : ��ȡˢ������
	* �������� : 
	*  ����ֵ  : RefreshDispTextProp* : ˢ������
	* �������� : 
	* ��    �� : ������
	* ������� : 
	* �޸ļ�¼ : 
	*******************************************************************************/
	virtual RefreshDispTextProp* GetRefreshDispTextProp();

	/********************************************************************************
	* �������� : ��������ˢ�µĵ�Ԫ��
	* �������� : 
	*  ����ֵ  : int : ������ɵĵ�Ԫ������
	* �������� : 
	* ��    �� : ������
	* ������� : 
	* �޸ļ�¼ : 
	*******************************************************************************/
	int DelutionHilightCells();

	/********************************************************************************
	* �������� : ֹͣ���ж�ʱ��
	* �������� : 
	*  ����ֵ  : 
	* �������� : ��������ᵼ����������ˢ�µ�����ʱ����
	* ��    �� : ������
	* ������� : 
	* �޸ļ�¼ : 
	*******************************************************************************/
	void KillAllTimer();

	//////////////////////////////////////////////////////////////////////////////////////////�б����ݲ���
	/********************************************************************************
	* �������� : ���ݵ�ǰ��Ϣ���³�ʼ������������
	* �������� : 
	*  ����ֵ  : 
	* �������� : 
	* ��    �� : lxqi
	* ������� : 2010-08-09
	* �޸ļ�¼ : 
	*******************************************************************************/
	virtual void InitRowData();

	/********************************************************************************
	* �������� : ��ʼ���̶���Ԫ������
	* �������� : int nFirst : ��ʼ��
	*			 int nCount : ������
	*  ����ֵ  : 
	* �������� : 
	* ��    �� : ������
	* ������� : 
	* �޸ļ�¼ : 
	*******************************************************************************/
	virtual void InitFixedDataItem(int nFirst,int nCount);

	/********************************************************************************
	* �������� : �趨ָ���̶���Ԫ������
	* �������� : int nRow : ��
	*			 int nCol : ��
	*			 CGridCellBase* pCell : ��Ԫ��ָ��
	*			 StockUserInfo* pStock : ��Ʊ��Ϣ
	*			 int nNumber : ���
	*			 BOOL bRefresh : �Ƿ�ˢ��
	*			 CDC* pDC : �豸DC
	*  ����ֵ  : 
	* �������� : 
	* ��    �� : ������
	* ������� : 
	* �޸ļ�¼ : 
	*******************************************************************************/
	virtual void SetFixedData(int nRow,int nCol,CGridCellBase* pCell, StockUserInfo* pStock = NULL,
		int nNumber = -1, BOOL bRefresh = TRUE, CDC* pDC = NULL);

	/********************************************************************************
	* �������� : ��ʼ�������
	* �������� : int nBeginRow : ��ʼ��
	*			 int nEndRow : ������
	*  ����ֵ  : 
	* �������� : 
	* ��    �� : ������
	* ������� : 
	* �޸ļ�¼ : 
	*******************************************************************************/
	virtual void InitNumber(int nBeginRow = -1, int nEndRow = -1);

	//////////////////////////////////////////////////////////////////////////////////////////����������
	/********************************************************************************
	* �������� : �趨������ˮƽ�����������ϵͳĬ��ˮƽ������
	* �������� : CSkinScrollBar* pNewHorScrollBar : ˮƽ������ָ��
	*  ����ֵ  : BOOL : �Ƿ�ɹ�
	* �������� : 
	* ��    �� : ������
	* ������� : 
	* �޸ļ�¼ : 
	*******************************************************************************/
	BOOL SetHorScrollBarRef(CSkinScrollBar* pNewHorScrollBar = NULL);
	/********************************************************************************
	* �������� : �趨��������ֱ�����������ϵͳĬ�ϴ�ֱ������
	* �������� : CSkinScrollBar* pNewVerScrollBar : ��ֱ������ָ��
	*  ����ֵ  : BOOL : �Ƿ�ɹ�
	* �������� : 
	* ��    �� : ������
	* ������� : 
	* �޸ļ�¼ : 
	*******************************************************************************/
	BOOL SetVerScrollBarRef(CSkinScrollBar* pNewVerScrollBar = NULL);

	/********************************************************************************
	* �������� : ���������
	* �������� : 
	*  ����ֵ  : 
	* �������� : ÿ�ε�������ȸ߶ȡ�����ɾ�����ȿ���Ӱ�쵽���������ȵĲ��������
	* ��    �� : ������
	* ������� : 
	* �޸ļ�¼ : 
	*******************************************************************************/
	virtual void ResetScrollBars();

	/********************************************************************************
	* �������� : ��ȡָ���������Ĺ���λ��
	* �������� : int nBar : ��������ʽ��ˮƽ��ֱ��
	*			 BOOL bGetTrackPos : �Ƿ��ȡ�϶�ʱ��λ��
	*  ����ֵ  : int : ��ǰ������λ��
	* �������� : 
	* ��    �� : ������
	* ������� : 
	* �޸ļ�¼ : 
	*******************************************************************************/
	virtual int  GetScrollPos32(int nBar, BOOL bGetTrackPos = FALSE);

	/********************************************************************************
	* �������� : �趨��������ֱ�����������ϵͳĬ�ϴ�ֱ������
	* �������� : CSkinScrollBar* pNewVerScrollBar : ��ֱ������ָ��
	*  ����ֵ  : BOOL : �Ƿ�ɹ�
	* �������� : 
	* ��    �� : ������
	* ������� : 
	* �޸ļ�¼ : 
	*******************************************************************************/
	virtual BOOL SetScrollPos32(int nBar, int nPos, BOOL bRedraw = TRUE);


	////////////////////////////////////////////////////////////////////////////////////////////////////////////////����������
	/********************************************************************************
	* �������� : ��������
	* �������� : PageViewInfo* pPageInfo : ҳ������
	*  ����ֵ  : 
	* �������� : ��ʱ��ʹ�ã����幦��ֱ�ӵ��þ��幦������
	* ��    �� : ������
	* ������� : 
	* �޸ļ�¼ : 
	*******************************************************************************/
	virtual void RequestData( PageViewInfo* pPageInfo = NULL );
	
	/********************************************************************************
	* �������� : �����Ʊ�����б�
	* �������� : PageViewInfo* pPageInfo : ҳ������
	*  ����ֵ  : 
	* �������� : 
	* ��    �� : ������
	* ������� : 
	* �޸ļ�¼ : 
	*******************************************************************************/
	virtual void RequestStockUserInfo(PageViewInfo* pPageInfo = NULL);
	
	/********************************************************************************
	* �������� : ����ʵʱ���ݣ�Ĭ�ϲ�����ʾ����ǰҳ�����ݣ�
	* �������� : int nFirst : ��ʼ�������
	*			 int nCount : �������
	*			 BOOL bNeedAutoPush : �Ƿ�ͬʱ��������
	*  ����ֵ  : 
	* �������� : 
	* ��    �� : ������
	* ������� : 
	* �޸ļ�¼ : 
	*******************************************************************************/
	virtual void RequestRealTimeData(int nFirst = -1, int nCount = -1, BOOL bNeedAutoPush = TRUE);
	
	/********************************************************************************
	* �������� : �����������ݣ�Ĭ�ϲ�����ʾ����ǰҳ���������ݣ�
	* �������� : PageViewInfo* pPageInfo : ҳ������
	*			 BOOL bNeedAutoPush : �Ƿ�ͬʱ��������
	*  ����ֵ  : 
	* �������� : 
	* ��    �� : ������
	* ������� : 
	* �޸ļ�¼ : 
	*******************************************************************************/
	virtual void RequestSortData(PageViewInfo* pPageInfo = NULL, BOOL bNeedAutoPush = TRUE);

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////����Ӧ����
	/********************************************************************************
	* �������� : ʵʱ���ݴ���
	* �������� : DataHead* pHead : Ӧ���ͷ�ṹ
	*			 const CommRealTimeData_Ext* pnowData : ʵʱ����
	*			 int nSize : ���ݸ���
	*  ����ֵ  : 
	* �������� : 
	* ��    �� : ������
	* ������� : 
	* �޸ļ�¼ : 
	*******************************************************************************/
	virtual void HandleNowData(DataHead* pHead,const CommRealTimeData_Ext* pnowData, int nSize);
	/********************************************************************************
	* �������� : �������ݴ���
	* �������� : DataHead* pHead : Ӧ���ͷ�ṹ
	*			 const CommRealTimeData_Ext* pnowData : ʵʱ����
	*			 int nSize : ���ݸ���
	*  ����ֵ  : 
	* �������� : 
	* ��    �� : ������
	* ������� : 
	* �޸ļ�¼ : 
	*******************************************************************************/
	virtual void HandleAutoPushData(DataHead* pHead, const CommRealTimeData_Ext* pnowData, int nSize);
	/********************************************************************************
	* �������� : �������ݴ���
	* �������� : DataHead* pHead : Ӧ���ͷ�ṹ
	*			 AnsReportData* pReportData : ��������
	*  ����ֵ  : 
	* �������� : 
	* ��    �� : ������
	* ������� : 
	* �޸ļ�¼ : 
	*******************************************************************************/
	virtual void HandleSortData(DataHead* pHead,const AnsReportData* pReportData);

	/********************************************************************************
	* �������� : ���ݸ������
	* �������� : int nPos : ������
	*			 StockUserInfo* pStock : ��Ʊ��Ϣ����
	*			 long nUnit : ÿ�ֹ���
	*			 short nDecimal : С��λ��
	*			 long  lClose : ���ռ�
	*			 HSStockRealTime_Ext* pData : ʵʱ����
	*			 StockOtherData* pOtherData : ��������
	*			 RefreshDispTextProp* pRefreshDispTextProp : ��������
	*			 int nType : ������ʽ
	*  ����ֵ  : 
	* �������� : 
	* ��    �� : ������
	* ������� : 
	* �޸ļ�¼ : 
	*******************************************************************************/
	virtual void UpdateDataEntry(
		int nPos,
		StockUserInfo* pStock,
		long nUnit,
		short nDecimal,
		long  lClose,
		HSStockRealTime_Ext* pData,
		StockOtherData* pOtherData,
		RefreshDispTextProp* pRefreshDispTextProp = NULL,
		int nType = MAKELONG(0,LTOA_MASK_DEFAULT));

	// ���¹�Ʊ��
	/********************************************************************************
	* �������� : ���ݸ������
	* �������� : int nPos : ������
	*			 StockUserInfo* pStock : ��Ʊ��Ϣ����
	*			 long nUnit : ÿ�ֹ���
	*			 short nDecimal : С��λ��
	*			 long  lClose : ���ռ�
	*			 HSStockRealTime_Ext* pData : ʵʱ����
	*			 StockOtherData* pOtherData : ��������
	*			 RefreshDispTextProp* pRefreshDispTextProp : ��������
	*			 int nType : ������ʽ
	*  ����ֵ  : 
	* �������� : 
	* ��    �� : ������
	* ������� : 
	* �޸ļ�¼ : 
	*******************************************************************************/
 	virtual void UpdateStockData(
 		int nPos,
 		StockUserInfo* pStock,
 		long nUnit,
 		short nDecimal,
 		long  lClose,
 		HSStockRealTime_Ext* pData,
 		StockOtherData* pOtherData,
 		RefreshDispTextProp* pRefreshDispTextProp = NULL,
 		int nType = 0);
// 
// 	// ����ָ����
// 	virtual void UpdateIndexData(
// 		int nPos,
// 		StockUserInfo* pStock,
// 		long nUnit,
// 		short nDecimal,
// 		long  lClose,
// 		HSIndexRealTime* pData,
// 		StockOtherData* pOtherData,
// 		RefreshDispTextProp* pRefreshDispTextProp = NULL,
// 		short nType = 0);
// 	// ����ָ����
// 	virtual void UpdateIndexData(
// 		int nPos,
// 		StockUserInfo* pStock,
// 		long nUnit,
// 		short nDecimal,
// 		long  lClose,
// 		HSStockRealTime_Ext* pData,
// 		StockOtherData* pOtherData,
// 		RefreshDispTextProp* pRefreshDispTextProp = NULL,
// 		short nType = 0);
// 	
// 	// �����ڻ���
// 	virtual void UpdateFuturesData(
// 		int nPos,
// 		StockUserInfo* pStock,
// 		long nUnit,
// 		short nDecimal,
// 		long  lClose,
// 		HSQHRealTime* pData,
// 		StockOtherData* pOtherData,
// 		RefreshDispTextProp* pRefreshDispTextProp = NULL,
// 		short nType = 0);
// 
// 	// ���������
// 	virtual void UpdateForeignData(
// 		int nPos,
// 		StockUserInfo* pStock,
// 		long nUnit,
// 		short nDecimal,
// 		long  lClose,
// 		HSStockRealTime* pForeignData,
// 		StockOtherData* pOtherData,
// 		RefreshDispTextProp* pRefreshDispTextProp = NULL,
// 		short nType = 0);
// 
// 	// ���¸۹���
// 	virtual void UpdateHKData(
// 		int nPos,
// 		StockUserInfo* pStock,
// 		long nUnit,
// 		short nDecimal,
// 		long  lClose,
// 		HSStockRealTime* pHKData,
// 		StockOtherData* pOtherData,
// 		RefreshDispTextProp* pRefreshDispTextProp = NULL,
// 		short nType = 0);
protected:
	PageViewInfo	m_pviInfo;					// ��ǰҳ����Ϣ
	IDataSourceEx*	m_pDataSource;				// ��������ӿ�ָ��
	long			m_nDataSourceID;			// ��������ID

	IHsColumnProperty*		m_iColProp;			// ���������ýӿ�
	IHsColor*               m_iColorProt;       // ��ɫ���ýӿ�
	IHsFont*                m_iFontProt;        // �������ýӿ�
	IHsUserDataCfg*         m_iUserDefCfg;      // �û����ݽӿڣ���Ʊ��ǣ�
	IBlockManager*			m_iBlockManager;	// ������
	IQuoteLang*				m_iQuoteLang;		// ������Դ�ӿ�

	BOOL			m_bNeedRequestSort;			// �Ƿ���Ҫ�����������ݣ�ĳЩ�в���Ҫ�������������Ƶȣ�
	CString			m_strCurColGroup;			// ��ǰ��ͷ������
	CString			m_strPreColGroup;			// ǰһ����ͷ������

	int				m_nCurActiveRow;			// ��ǰ������

	CSkinScrollBar* m_pHorScrollBar;			// ������ˮƽ������
	CSkinScrollBar* m_pVerScrollBar;			// ��������ֱ������

	CUIntArray		m_ayColInfo;	// ������ID
	CArray<StockUserInfo*,StockUserInfo*>	m_pStock;		// ��ǰ��ʾ�����й�Ʊ������Ϣ
	
	CMap<int,int,int,int>					m_mapColumn;	//��ID������Ŷ�Ӧ��
	BOOL			m_bResizeColWidth;			// �ֶ��������п�
public:
	int				m_nRefreshTimerID;			// ˢ�¶�ʱ��ID
	int				m_nDelayRealTimeRequestTimerID;		// ��ʱʵʱ��������ʱ��ID	
	int				m_nDelaySortRequestTimerID;			// ��ʱ��������ʱ��ID
	CMap<int,int,int,int>					m_mapRefreshCells;	// ����ˢ�µĵ�Ԫ���б�
	CMap<UINT,UINT,CString,CString>						m_mapTempColGroup;	// ����ʱ����ͷ�����Ʋ˵���Ӧ��
	DECLARE_MESSAGE_MAP()
public:
	/********************************************************************************
	* �������� : ��������Ӧ��
	* �������� : void* pData : Ӧ�������
	*			 int nLen : Ӧ�������
	*  ����ֵ  : 
	* �������� : 
	* ��    �� : ������
	* ������� : 
	* �޸ļ�¼ : 
	*******************************************************************************/
	BOOL OnRecvData(void* pData, int nLen);

	void OnColGroupChange(UINT nID);			// �л���ʾ��ͷ��
	void OnColumnSetting();				// �趨��ʾ��ͷ����
	void OnMarkCodeByString();			// �ַ�����Ǵ���
	void OnMarkCodeByIcon(UINT nID);	// ͼ���Ǵ���
	void OnCancelMarkCode();			// ȡ��������
	void OnAddToBlock();				// ��ӵ����
	void OnAddToSelfBlock();			// ��ӵ���ѡ�ɰ��
	void OnBlockAdscription();			// �������

	virtual void OnDraw(CDC* pDC);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	static VOID CALLBACK TimerProc( HWND hwnd,	UINT uMsg, UINT_PTR idEvent, DWORD dwTime );
	static VOID CALLBACK DelayRealTimeRequestTimerProc(HWND hwnd,	UINT uMsg, UINT_PTR idEvent, DWORD dwTime );
	static VOID CALLBACK DelaySortRequestTimerProc(HWND hwnd,	UINT uMsg, UINT_PTR idEvent, DWORD dwTime);
};

