#include "..\common\tablecommondefine.h"
#include "..\common\hsstruct.h"
#include "..\common\quotestuctdefine.h"
#include "quotesort.h"
#include "hsgridtable.h"
#include "skinscrollbar.h"

#pragma once

extern int AnalyzeStr(CString strResource,const char* strDelimit,CStringArray& ayRet);

#define INIT_FIXED_HEAD			0x00010000
#define INIT_FIXED_DATA			0x00020000 // һ�㱨��(1~6)
#define INIT_SORTRPT_DATA		0x00040000 // ����(61~66)
#define INIT_FSDetail_DATA		0x00080000 // ��ʱ��ϸ��
#define INIT_PanHouDetail_DATA  0x00100000 // �̺���ϸ��
#define INIT_BigDetail_DATA		0x00200000 // ������ϸ��
#define INIT_FENJIA_DATA		0x00400000 // �ּ۱�


// for test
//	���Ҳ���������
enum FindType	
{
	none = 0x00, 
	SI_CODE = 0x01,	//	���չ�Ʊ������в���
	SI_NAME = 0x02,	//	���չ�Ʊ���ƽ��в���
	SI_PYJC = 0x04, //	����ƴ����ƽ��в���
	SI_FOREX= 0x10	//  �Ҳ������벻��000001��Ʊ
};


/*�� ���ڹ������е�*/
class CMapWordToPtrEx;
class CTableColume : public GV_ITEM
{
public:
	//������	�з��
	enum { NewColMask = 0x1,NewColFormat = 0x2 };
	CTableColume();
	CTableColume(WORD dID);
	~CTableColume();

	//��������
	CTableColume* operator=(CTableColume* pCol);


	BYTE	m_bDelete;					//����,��ע�����������, ȡֵ:NewColMask, NewColFormat
	WORD	m_dID;						//��ID
	TableColProperty* m_pProperty;		//������

	int m_nTempWidth;					// ��ʱ���

protected:
	//�ֶδ����ϣ��
	static CMapStringToPtr 		 m_mapList;
	static COLORREF*			m_colTest;	//for testing

public:
	//��������, ɾ��mask, nFormat�ڴ�
	void Delete();

	//��ȡ��m_dID�е�����ָ��, ʧ�ܷ���NULL
	TableColProperty* GetColProperty();
	//ȡ��������, ���ʧ��, ����:"��"
	CString				  GetName();
	//ȡ���������Լ����ָ��
	UINT				  GetRowMask();
	UINT				  GetRowFormat();
	//�����������Լ����	bRow  - �Ƿ�Ϊ��
	void SetRowMask(UINT nMask,BOOL bRow = TRUE);
	void SetRowFormat(UINT nFormatRef,BOOL bRow = TRUE);

	//ȡ��,�����п��	nWidth - �ַ����� * 2
	void SetColWidth(int nWidth);
	int  GetColWidth();

	//�·���һ���ֶη����ϣ��, ����ָ��
	static CMapWordToPtrEx*		GetMapColName();
	//���ֶδ����ϣ����,�����ֶη����ϣ��, ʧ�ܷ���NULL, CString strName - KEY
	static CMapWordToPtrEx*		GetMap(CString strName);
	//���ֶδ����ϣ����,�����ֶη����ϣ��, ʧ�ܷ���NULL, WORD key - KEY
	static CMapWordToPtrEx*		GetMap(WORD key);
	//�����ֶ�ID, ȡ�����������ϣ���KEY
	static CString				GetMapName(WORD key);
	//������ϣ��
	static void					InitColName();
	/********************************************************************************
	* �������� : �ֶ����Զ������ļ�
	* �������� : CWinApp* pApp - δʹ��,
	*			 WORD dNameID - �ֶ�ID,
	*			 CString& strName - ����ֵ,
	*			 CString strSection - ������
	*  ����ֵ  : BOOL
	* �������� : 
	* ��    �� : 
	* ������� : 
	* �޸ļ�¼ : 
	*******************************************************************************/
	static BOOL					GetColName(CWinApp* pApp,WORD dNameID,CString& strName,CString strSection);

	/********************************************************************************
	* �������� : �ֶ�����д�����ļ�
	* �������� : WORD wKey - �ֶ�ID, 
	*			 TableColProperty* pCol - ����ָ�� , 
	*			 LPCTSTR lpszSection ������,
	*			 LPCTSTR lpszFileName �ļ�
	*  ����ֵ  : void
	* �������� : 
	* ��    �� : 
	* ������� : 
	* �޸ļ�¼ : 
	*******************************************************************************/
	static void					WriteColName(WORD wKey, TableColProperty* pCol = NULL, LPCTSTR lpszSection = NULL,LPCTSTR lpszFileName = NULL);

	//ɾ���ֶη����ϣ��, ��д�����ļ�	strName - ����KEY
	static void					DelMapColName(CString strName);
	//�����ֶ��Ƿ����	key - ID
	static BOOL			  	    Find(WORD key);
	//ͨ���ֶ�ID���Ҹ��ֶ����Ի�������, ʧ�ܷ���NULL,"��"
	static TableColProperty*	GetByKey(WORD key);
	static CString				GetName(WORD key);
	//ͨ���ֶ�IDȡ���������������ָ��
	static UINT				GetRowMask(WORD key);
	static UINT				GetRowFormat(WORD key);
	//ͨ���ֶ�IDȡ���������������ָ��
	static UINT				GetColMask(WORD key);
	static UINT				GetColFormat(WORD key);

	// �����ֶδ����ϣ��ָ��
	static CMapStringToPtr*		GetList() { return &m_mapList; }
	//����ֶη����ϣ��		CString strName - KEY ,CMapWordToPtrEx* pValue - �����ϣ��ָ��
	static void					AddToList(CString strName,CMapWordToPtrEx* pValue);
	//ɾ���ֶδ����ϣ�� �ֶ�����д�����ļ�
	static void					DelList();
	//ɾ��һ�ֶ� д�����ļ� δʹ��
	static void					DelColumn(WORD key);

	//ͨ���ֶ�IDȡ�������	
	static CString				GetNameByColID(WORD dID);		//����
	static COLORREF				GetBkClrByColID(WORD dID);		//����ɫ
	static COLORREF				GetFgClrByColID(WORD dID);		//ǰ��ɫ
	static UINT					GetStyleByColID(WORD dID);		//���
	static UINT					GetDataTypeByColID(WORD dID);	//��������
};

class CMapWordToPtrEx : public CMapWordToPtr
{
protected:
	/*���ͷ��	��16λ,�������	����:MPEXSϵ�� ��λ���	*
	*	8 - 15λ,����,	����:MPEXTϵ�� �Ƚ�ֵ
	*	0 - 7λ,����*/
	DWORD m_dwStyle;
public:
	CMapWordToPtrEx()
	{
		m_dwStyle = MAKELONG( 0, MPEXS_RWCONFIG);		
	}
public:
	//���ù�ϣ��
	void SetAt(WORD key, void* newValue,CWinApp* pApp,CString& strName,CString& strSection)
	{
		TableColProperty* pCol = (TableColProperty*)newValue;

		if(pCol != NULL)
		{
			if( IsStyle(MPEXS_RWCONFIG) && 
				CTableColume::GetColName(pApp,key,strName,strSection) )
			{
				CStringArray ayRet;
				if(AnalyzeStr(strName,";",ayRet) >= 7)
				{
					if( !IsStyle(MPEXS_COLNAME_NORWCFG) )
					{
						pCol->m_strName.Format("%s",ayRet[0]);
					}
					pCol->m_nRowMask   = atol(ayRet[1]);
					pCol->m_nRowFormat = atol(ayRet[2]);
					pCol->m_nColMask   = atol(ayRet[3]);
					pCol->m_nColFormat = atol(ayRet[4]);
					pCol->m_TextColor  = atol(ayRet[5]);
					pCol->m_cWidth     = atol(ayRet[6]);
					pCol->m_cConfigWidth = pCol->m_cWidth;
				}
				else
				{
					pCol->m_strName.LoadString(atoi(pCol->m_strName));
				}
			}
			else
			{
				pCol->m_strName.LoadString(atoi(pCol->m_strName));
			}
		}
		CMapWordToPtr::SetAt(key,newValue);
	}
	void SetAt(WORD key, void* newValue)
	{
		void* pValue;
		if( CMapWordToPtr::Lookup(key,pValue) ) // �Ѿ�����
		{
			delete (CTableColume*)pValue;
			RemoveKey(key);
			return;
		}
		CMapWordToPtr::SetAt(key,newValue);
	}
public:
	//�жϷ��
	BOOL IsStyle(WORD wStyle){return wStyle & HIWORD(m_dwStyle);}
	//���÷��
	void ModifyStyle(WORD wAddStyle, WORD wRemoveStyle)
	{
		m_dwStyle |= MAKELONG(0, wAddStyle);
		m_dwStyle &= ~ MAKELONG(0, wRemoveStyle);
	}
public:
	//ȡ�û�����������
	BYTE GetType(){ return HIBYTE( LOWORD(m_dwStyle) );}
	void SetType(BYTE tType)
	{
		m_dwStyle &= 0xFFFF00FF;
		m_dwStyle |= MAKELONG(MAKEWORD(0, tType), 0);
	} 
public:
	//����
	BYTE GetReserve(){ return LOBYTE( LOWORD(m_dwStyle) );}
	void SetReserve(BYTE tType)
	{
		m_dwStyle &= 0xFFFFFF00;
		m_dwStyle |= MAKELONG(MAKEWORD(tType, 0), 0);
	} 

};

//////////////////////////////////////////////////////////////////////////
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
struct InitTableData
{
	DWORD m_dStyle;
	int   m_nRowCount;
	int   m_nColCount;
	int   m_nFixRowCount;
	int   m_nFixColCount;
	void* m_pData;

	CString m_strGroupName;

	InitTableData()
	{
		m_dStyle = INIT_FIXED_HEAD;
		m_nRowCount = 1;
		m_nColCount = 3;
		m_nFixRowCount = 1;
		m_nFixColCount = 3;
		m_pData = NULL;
	}
	BOOL IsStyle(DWORD dStyle) {return (m_dStyle & dStyle);}
};

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
// CQuoteReportTable

class CQuoteReportTable : public CHSGridTable
{
	DECLARE_DYNAMIC(CQuoteReportTable)

public:
	CQuoteReportTable(int nRows = 0, int nCols = 0, int nFixedRows = 0, int nFixedCols = 0);
	virtual ~CQuoteReportTable();


	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////���ز���
	// �趨ָ���п� ע�⣺nWidthΪӢ�İ���ַ���,��Ҫͨ������õ�������
	virtual int SetColumnWidth(int nColID,int nWidth = -1);

	CString GetMenuName(int nType = 0, int nSubType = 0);

	virtual BOOL ShowRow(int row, BOOL bRefresh = FALSE);	// ��ʾָ����
	virtual BOOL ShowCol(int col, BOOL bRefresh = FALSE);	// ��ʾָ����

	virtual BOOL MoveRowTo(int nSrc,int nDes);		// �ƶ�ָ���е�Ŀ����
	virtual BOOL MoveColTo(int nSrc,int nDes);		// �ƶ�ָ���е�Ŀ����


	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////�У�ͷ������
	/********************************************************************************
	* �������� : ��ȡĬ����ͷ������
	* �������� : CString strFileName : ���Ĭ����ͷ�����Ƶ������ļ�
	*  ����ֵ  : CString ��ͷ������
	* �������� : 
	* ��    �� : lxqi
	* ������� : 2010-08-04
	* �޸ļ�¼ : 
	*******************************************************************************/
	CString	GetDefultColGroup(CString strFileName = _T(""));

	/********************************************************************************
	* �������� : ������ͷģ�����ڵ��ļ���
	* �������� : int nType : ��ͷģ������
	*  ����ֵ  : CString ��ͷģ�����������ļ���
	* �������� : 
	* ��    �� : lxqi
	* ������� : 2010-08-09
	* �޸ļ�¼ : 
	*******************************************************************************/
	static CString GetColHeadTemplFileName(int nType = 0);

	/********************************************************************************
	* �������� : ��ȡ�����ļ��е���ͷģ��
	* �������� : CString strTemplName : ��ͷģ������
	*			 int nType : ģ������
	*			 CArray<CStringArray*,CStringArray*>* ayColTemplList : ģ���б�
	*  ����ֵ  : int ģ���б������
	* �������� : 
	* ��    �� : lxqi
	* ������� : 2010-08-09
	* �޸ļ�¼ : 
	*******************************************************************************/
	static int LoadColHeadTemplate(CString strTemplName, int nType = 0,
		CArray<CStringArray*,CStringArray*>* ayColTemplList = NULL);

	/********************************************************************************
	* �������� : ������ͷ�����Ƴ�ʼ����ͷ
	* �������� : CString strGroupName : ��ͷ������
	*  ����ֵ  : 
	* �������� : 
	* ��    �� : lxqi
	* ������� : 2010-08-04
	* �޸ļ�¼ : 
	*******************************************************************************/
	virtual void InitColGroup(CString strGroupName);

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
	* ������� : 2010-08-04
	* �޸ļ�¼ : 
	*******************************************************************************/
	virtual int GetColIndexByID(UINT uiID);

	/********************************************************************************
	* �������� : ������ID��ȡ��ͷ����ָ��
	* �������� : [IN] UINT uiID : ��ID
	*			 [OUT] int& nPos : ��ͷ���
	*  ����ֵ  : CTableColume* ��ͷָ��
	* �������� : 
	* ��    �� : lxqi
	* ������� : 2010-08-04
	* �޸ļ�¼ : 
	*******************************************************************************/
	virtual CTableColume* GetColByID(UINT uiID);
	virtual CTableColume* GetColumn(UINT nID,int& nPos);

	/********************************************************************************
	* �������� : ��������Ż�ȡ��ͷ����ָ��
	* �������� : [IN] int nPos : �����
	*  ����ֵ  : CTableColume* ��ͷָ��
	* �������� : 
	* ��    �� : lxqi
	* ������� : 2010-08-11
	* �޸ļ�¼ : 
	*******************************************************************************/
	virtual CTableColume* GetHead(int nPos);

	/********************************************************************************
	* �������� : �����
	* �������� : UINT uiID : ��ID
	*			 InitTableData* pInitData : 
	* �������� : 
	* ��    �� : lxqi
	* ������� : 2010-08-09
	* �޸ļ�¼ : 
	*******************************************************************************/
	virtual void AddColumn(UINT nID);

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
	virtual void Sort(int nID, BOOL bAscending);

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

	void ResetMap();
	void AddColToMap(int nID,int nColPos);


	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////��Ʊ�����б����
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
	virtual void SetStock(CArray<StockUserInfo*,StockUserInfo*>& payStock,int nTotal);
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





	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////ҳ�棨���ԣ�����
	/********************************************************************************
	* �������� : �ı䵱ǰҳ��
	* �������� : PageViewInfo* pvInfo : ҳ������
	*  ����ֵ  : 
	* �������� : 
	* ��    �� : lxqi
	* ������� : 2010-08-09
	* �޸ļ�¼ : 
	*******************************************************************************/
	virtual int ChangeContent(PageViewInfo* pvInfo);

	/********************************************************************************
	* �������� : �趨��ǰҳ����Ϣ
	* �������� : PageViewInfo* pvInfo : ҳ������
	*  ����ֵ  : 
	* �������� : 
	* ��    �� : lxqi
	* ������� : 2010-08-09
	* �޸ļ�¼ : 
	*******************************************************************************/
	virtual void SetPageViewInfo(PageViewInfo* pvInfo);

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

	virtual void InitFixedDataItem(int nFirst,int nCount);

	virtual void SetFixedData(int nRow,int nCol,CGridCellBase* pCell, StockUserInfo* pStock = NULL,
		int nNumber = -1, BOOL bRefresh = TRUE, CDC* pDC = NULL);

	virtual void GetClientRect(LPRECT lpRect);

	//////////////////////////////////////////////////////////////////////////����������
	BOOL SetHorScrollBarRef(CSkinScrollBar* pNewHorScrollBar = NULL);	// �趨������ˮƽ�����������ϵͳĬ��ˮƽ������
	BOOL SetVerScrollBarRef(CSkinScrollBar* pNewVerScrollBar = NULL);	// �趨��������ֱ�����������ϵͳĬ�ϴ�ֱ������

	virtual void ResetScrollBars();
	virtual int  GetScrollPos32(int nBar, BOOL bGetTrackPos = FALSE);
	virtual BOOL SetScrollPos32(int nBar, int nPos, BOOL bRedraw = TRUE);

	// ��������
	//StockUserInfo* pStock,
	//	long nUnit,
	//	short nDecimal,
	//	long  lClose,
	//	HSStockRealTime_Ext* pData,
	//	StockOtherData* pOtherData,
	//	RefreshDispTextProp* pYlsRefreshDispText = NULL,
	//	short nType = YLS_DATA_COLUMN_POS);
	//virtual void UpdateStdData(
	//	int nPos,
	//	StockUserInfo* pStock,
	//	long nUnit,
	//	short nDecimal,
	//	long  lClose,
	//	HSStockRealTime* pData,
	//	StockOtherData* pOtherData,
	//	RefreshDispTextProp* pYlsRefreshDispText = NULL,
	//	short nType = YLS_DATA_COLUMN_POS);

	//// ���¹�Ʊ��
	//virtual void UpdateStockData(
	//	int nPos,
	//	StockUserInfo* pStock,
	//	long nUnit,
	//	short nDecimal,
	//	long  lClose,
	//	HSStockRealTime* pData,
	//	StockOtherData* pOtherData,
	//	RefreshDispTextProp* pYlsRefreshDispText = NULL,
	//	short nType = YLS_DATA_COLUMN_POS);
	//virtual void UpdateStockDataExt(
	//	int nPos,
	//	StockUserInfo* pStock,
	//	long nUnit,
	//	short nDecimal,
	//	long  lClose,
	//	HSStockRealTime_Ext* pData,
	//	StockOtherData* pOtherData,
	//	RefreshDispTextProp* pYlsRefreshDispText = NULL,
	//	short nType = YLS_DATA_COLUMN_POS);
	//// ����ָ����
	//virtual void UpdateIndexData(
	//	int nPos,
	//	StockUserInfo* pStock,
	//	long nUnit,
	//	short nDecimal,
	//	long  lClose,
	//	HSIndexRealTime* pData,
	//	StockOtherData* pOtherData,
	//	RefreshDispTextProp* pYlsRefreshDispText = NULL,
	//	short nType = YLS_DATA_COLUMN_POS);
	//// ����ָ����
	//virtual void UpdateIndexDataExt(
	//	int nPos,
	//	StockUserInfo* pStock,
	//	long nUnit,
	//	short nDecimal,
	//	long  lClose,
	//	HSStockRealTime_Ext* pData,
	//	StockOtherData* pOtherData,
	//	RefreshDispTextProp* pYlsRefreshDispText = NULL,
	//	short nType = YLS_DATA_COLUMN_POS);
	//// �����ڻ���
	//virtual void UpdateFuturesData(
	//	int nPos,
	//	StockUserInfo* pStock,
	//	long nUnit,
	//	short nDecimal,
	//	long  lClose,
	//	HSQHRealTime* pData,
	//	StockOtherData* pOtherData,
	//	RefreshDispTextProp* pYlsRefreshDispText = NULL,
	//	short nType = YLS_DATA_COLUMN_POS);

	//// ���¹�Ʊ��
	//virtual void UpdateForeignData(
	//	int nPos,
	//	StockUserInfo* pStock,
	//	long nUnit,
	//	short nDecimal,
	//	long  lClose,
	//	HSStockRealTime* pForeignData,
	//	StockOtherData* pOtherData,
	//	RefreshDispTextProp* pYlsRefreshDispText = NULL,
	//	short nType = YLS_DATA_COLUMN_POS);
	//// ���¹�Ʊ��
	//virtual void UpdateHKData(
	//	int nPos,
	//	StockUserInfo* pStock,
	//	long nUnit,
	//	short nDecimal,
	//	long  lClose,
	//	HSStockRealTime* pHKData,
	//	StockOtherData* pOtherData,
	//	RefreshDispTextProp* pYlsRefreshDispText = NULL,
	//	short nType = YLS_DATA_COLUMN_POS);
	////yangdl 2008.01.22 �����������
	//virtual void UpdateWHCenterData(
	//	int nPos,
	//	StockUserInfo* pStock,
	//	long nUnit,
	//	short nDecimal,
	//	long  lClose,
	//	HSWHCenterRealTime* pData,
	//	StockOtherData* pOtherData,
	//	RefreshDispTextProp* pYlsRefreshDispText = NULL,
	//	short nType = YLS_DATA_COLUMN_POS);

	PageViewInfo	m_pviInfo;					// ��ǰҳ����Ϣ

	int				m_nCurSortColumnID;			// ��ǰ�����������id
	BOOL			m_bAscending;				// �Ƿ���������
	CString			m_strCurColGroup;			// ��ǰ��ͷ������
	int				m_nColHeadHeigh;			// ��ǰ��ͷ�߶�

	int				m_nCurActiveRow;			// ��ǰ������

	CSkinScrollBar* m_pHorScrollBar;			// ������ˮƽ������
	CSkinScrollBar* m_pVerScrollBar;			// ��������ֱ������

	CArray<CTableColume*,CTableColume*>		m_ayColInfo;	// ����������
	CArray<StockUserInfo*,StockUserInfo*>	m_pStock;		// ��ǰ��ʾ�����й�Ʊ������Ϣ
	
	CMap<int,int,int,int>					m_mapColumn;	//��ID������Ŷ�Ӧ��
	DECLARE_MESSAGE_MAP()
public:
	virtual void OnDraw(CDC* pDC);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
};


