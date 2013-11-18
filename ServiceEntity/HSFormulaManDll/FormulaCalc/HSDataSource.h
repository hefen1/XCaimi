/*************************************************************
*	Copyright (c)2010, ���ݺ�����Ϣ�������޹�˾
*	All rights reserved.
*
*	�ļ����ƣ�	HSDataSource.h
*	�ļ���ʶ��	
*	ժ	  Ҫ��	���ݶ���
*
*	��ǰ�汾��	1.0
*	��	  �ߣ�	
*	�������ڣ�	
*
***************************************************************/

#if !defined(_HXDATASOURCE_H_)
#define _HXDATASOURCE_H_

#include "hsds_comudata.h"
#include "FormulaStruct.h"
#include "ExpPublic.h"
#include "DataSourceDefineEx.h"

#define HS_HxDataSource_DynNews			0x00010000 // ��̬��Ϣ

#define HS_HxDataSource_SelectStock_Mark   0x00080000 // ѡ����ʾ


class CHSDisk;
#define FinanceData HisFinanceData
/*************************************************************
*	�� �� �ƣ�	CHSDataSource
*	�� �� ����	����Դ������
***************************************************************/
class  CHSDataSource : public CObject
{
	DECLARE_DYNCREATE(CHSDataSource)
public:
	// ����������ݵȽ��г�ʼ������
	CHSDataSource(AskData* pCode = NULL,
				  DWORD dwStyle = HS_HxDataSource_Left_ChuQuan		| 
								  HS_HxDataSource_Read_CaiWu_Data);
	// ��յ�����Դ
	virtual ~CHSDataSource();
	// ��յ�����Դ
	virtual void Free();

protected:
	DWORD	   m_dwStyle;		// ���ݴ����� ����߳�Ȩ,����Ȩ��
	AskData*   m_pAsk;			// ���������

public:
	HSDouble   m_nVolumeUnit;    // �ɽ�����λ
	HSDouble   m_nMoneyUnit;     // �ɽ��λ
	short	   m_nMoneyDecimal;  // �ɽ���С��λ

	HSDouble   m_nPriceUnit;	 // �۸�λ
	short	   m_nPriceDecimal;  // �۸�С��λ

	int		   m_nAnaPeriod;	 // ��������
	int        m_nSize;          // �������ݸ���
#ifdef SUPPORT_NETVALUE
	HSDouble	m_nNetVolueUint; // ��ֵ�����ʵ�λ��Ŀǰ��Ϊ1000)
#endif

public:
	// ������
	BOOL	IsStyle(long dwStyle)		{ return (m_dwStyle & dwStyle); }
	void	AddStyle(long dwStyle)		{ m_dwStyle |= dwStyle;         }
	void	RemoveStyle(long dwStyle)	{ m_dwStyle &= (~dwStyle);      }
	void    SetStyle(long dwStyle)		{ m_dwStyle = dwStyle;          }
	
public:
	// �����������Ϣ
	void	SetAsk(AskData* pAsk, int nAnaPeriod = 1); //20080826 YJT ����nAnaPeriod����

	// ��������
protected:
	// ���ݱ�(�翪��,����,���....�ȵ�����)
	CMapDataSource m_ayDayData;
	// ������
	int			   m_nDayCount;
	// �����߽ṹ�����ڴ����������
	long*		   m_lRefDay;

	// ��ǰ�����������
	CObject*	   m_pDataOwnerObj;
	IDataSourceEx* m_pDataSource;

	/******************************************************************
	*	�������ܣ�	��nkey��ֵ���ݼ��������б�,�����·���ռ��СΪlCurSize
	*	����������	StockDay* pDay	: [in] ����
	*				long lCurSize	: [in] �µĿռ䳤��
	*				int nOldSize	: [in] �ɵĿռ䳤��
	*				WORD nkey		: [in] Ҫ����������ļ�ֵ
	*	�� �� ֵ��	CDataSourceUseData* : �����������еĿռ�
	*	����������	
	*	��    �ߣ�
	*	������ڣ�	2003��4��16��
	*	�޸�˵����	
	*****************************************************************/
	CDataSourceUseData* GetDay(StockDay* pDay,long lCurSize,int nOldSize,WORD nkey,StockDay* pLastDay = NULL);

	// ��������
protected:
	// ���õ�����Դ
	CHSDataSource* m_pRefDataSource;
public:
	// �������õ�����Դ
	void SetRefDataSource(CHSDataSource* pRefDataSource) { m_pRefDataSource = pRefDataSource; }
	/******************************************************************
	*	�������ܣ�	���ļ��ж���strCode��Ʊ����,��strCodeΪ��ʱ,���������m_pAsk�е�һ֧��Ʊ������
	*	����������	CString strCode = "" : [in] ��Ʊ����
	*	�� �� ֵ��	int  :	<=0 ����ʧ��: ��Ʊ������(0), ���õ�����Դ����Ϣ���ڲ��Ϸ�(-1), ��������Ϸ�(-2),
							1 ����ɹ�
	*	����������	
	*	��    �ߣ�
	*	������ڣ�	
	*	�޸�˵����	
	*****************************************************************/
	int  ReadData(CString strCode = "");
	// �����б�m_ayDayData�Ƿ�Ϊ��
	BOOL IsEmpty();

	// ����ת��
public:	
	// �������б��еõ���ֵΪnKey����������
	HSDouble*			GetData(WORD nkey);
	// �õ����ݵ���������,��ʵ����GetData()�����,����ĳ���
	int					GetDaySize() { return m_nDayCount; }

	/******************************************************************
	*	�������ܣ�	�����������ݵ������б�m_ayDayData��
	*	����������	StockDay* pDay		: [in] ��������
	*				long lCurSize		: [in] �������ݳ���
	*				int nVolumeUnit		: [in] �ɽ�����λ
	*				int nMoneyUnit		: [in] �ɽ��λ
	*				short nMoneyDecimal	: [in] �ɽ���С��λ
	*				int nPriceUnit		: [in] �۸�λ
	*				short nPriceDecimal	: [in] �۸�С��λ
	*				WORD nKey = 0		: [in] Ϊ0ʱ,��ʾ�������е�K��,����,ֻ����һ����ӦnKey��ֵ������
	*	�� �� ֵ��	BOOL : �ɹ�(TRUE), ʧ��(FALSE)
	*	����������	
	*	��    �ߣ�
	*	������ڣ�	2003��4��16��
	*	�޸�˵����	
	*****************************************************************/
	BOOL				SetDay(StockDay* pDay,long lCurSize,
							   int nVolumeUnit,
							   int nMoneyUnit,short nMoneyDecimal,
							   int nPriceUnit,short nPriceDecimal,
							   WORD nKey = 0,
							   StockDay* pLastDay = NULL);

	/******************************************************************
	*	�������ܣ�	����������������ͬ,ֻ�ǵ�pDataOwnerObjΪ��ǰ��m_pDataOwnerObjʱ,����������, Ŀǰ�˺���û�в���
	*	����������	CObject* pDataOwnerObj : [in] ������
	*				StockDay* pDay		: [in] ��������	
	*				long lCurSize		: [in] �������ݳ���
	*				int nVolumeUnit		: [in] �ɽ�����λ
	*				int nMoneyUnit		: [in] �ɽ��λ
	*				short nMoneyDecimal	: [in] �ɽ���С��λ
	*				int nPriceUnit		: [in] �۸�λ
	*				short nPriceDecimal	: [in] �۸�С��λ
	*	�� �� ֵ��	BOOL : �ɹ�(TRUE), ʧ��(FALSE)
	*	����������	
	*	��    �ߣ�
	*	������ڣ�	2003��4��16��
	*	�޸�˵����	
	*****************************************************************/
	BOOL				SetDay(CObject* pDataOwnerObj,StockDay* pDay,long lCurSize,
							   int nVolumeUnit,
							   int nMoneyUnit,short nMoneyDecimal,
							   int nPriceUnit,short nPriceDecimal);

	// ��Ȩ�������
protected:
	// ��ǰ��Ʊ�ĳ�Ȩ����
	CArray<ChuQuanData*,ChuQuanData*> m_ayChuQuanData;
public:
	// ���ó�Ȩ����(���ļ��ж���)
	void SetChuQuanData(CodeInfo* pCode,CArray<ChuQuanData*,ChuQuanData*>* pAyChuQuanData=NULL);  // 

	void SetPriceUnit(long lPrice = 1);  //���ü۳���


	/******************************************************************
	*	�������ܣ�	��Ȩ���ݼ���
	*	����������	ChuQuanData* pData	: [in] ��Ȩ����
	*				long& lData			: [int/out] Ҫ��Ȩ������
	*				int& nPriceUnit		: [in] �۸�����
	*	�� �� ֵ��	void 
	*	����������	
	*	��    �ߣ�
	*	������ڣ�	2003��4��16��
	*	�޸�˵����	
	*****************************************************************/
	void CalcChuQuan(ChuQuanData* pData,long& lData,int& nPriceUnit);

	/******************************************************************
	*	�������ܣ�	��Ȩ���ݼ���
	*	����������	ChuQuanData* pData: [in] ��Ȩ����
	*				double& dwData    : [in/out] Ҫ��Ȩ������
	*	�� �� ֵ��	void 
	*	����������	
	*	��    �ߣ�
	*	������ڣ�	2003��4��16��
	*	�޸�˵����	
	*****************************************************************/
	void CalcChuQuan(ChuQuanData* pData,double& dwData);

	/******************************************************************
	*	�������ܣ�	��K�����ݽ��г�Ȩ����
	*	����������	StockDay& pDay		: [in/out] K������
	*				DWORD dwStyle = 0	: [in] ����
	*	�� �� ֵ��	BOOL : �ɹ�(TRUE),ʧ��(FALSE)
	*	����������	
	*	��    �ߣ�
	*	������ڣ�	2003��4��16��
	*	�޸�˵����	
	*****************************************************************/
	BOOL CalcChuQuan(StockDay& pDay,DWORD dwStyle = 0);
	// ������������(�����֧�ֳ�Ȩ,������)
	void CalcChuQuan(StockDay* pDay,long lCurSize,void* pData = NULL);
	// �õ���Ȩ��������ָ��
	CArray<ChuQuanData*,ChuQuanData*>* GetChuQuan() { return &m_ayChuQuanData; }

	// �����������
protected:
	// �������ݹ������
	CHSFinanceData  m_HSFinaceData;
	//CArray<CurrentFinanceData> 
public:
	// ���زƹ������
	CHSFinanceData* GetFinaceData() { return &m_HSFinaceData; }
	// �õ�ĳһ��Ĳ�������
	HSDouble		GetFinaceDataByID(UINT nID);
	// ���ļ��ж���pCode��Ʊ�Ĳ������ݵ��ڴ�m_HSFinaceData��
	void		    SetFinaceData(CodeInfo* pCode);
	// �õ���ǰ��Ʊ����ͨ��,����ʽʹ��
	HSDouble        CAPITAL();
	// ���ݲ����������ڲ������������е�λ�õõ�����������
	HSDouble        GetFinaceDataByPos(int nPos);

	// ʱʱ�������
public:
	// ����ʽʹ��

	/******************************************************************
	*	�������ܣ�	�õ���Ӧ��Ʊ��Ӧ����������� 
	*	����������	int nPos			: [in] �������ݵ�ID,����3:����,4����,5���....
	*				char cType = 0		: [in] Ŀǰ����
	*				char* pCode = NULL	: [in] ��Ʊ����
	*	�� �� ֵ��	HSDouble  : ������Ӧ��Ʊ��Ӧ����������� 
	*	����������	
	*	��    �ߣ�
	*	������ڣ�	2003��4��16��
	*	�޸�˵����	
	*****************************************************************/
	HSDouble           GetNowDataByCode(int nPos,char cType = 0,char* pCode = NULL);

	// ѡ�����
protected:
	// ѡ�ɽ��
	CArray<SuccResult*,SuccResult*>* m_pSelStockSuccResult;
public:
	// ����ѡ�ɽ��
	CArray<SuccResult*,SuccResult*>* SetSeleckStockData(CodeInfo* pCode = NULL);

	// ͨѶ��������Ѷ����
public:
	// ��Ϣ����
	static HWND m_wndMsg;
public:
	/******************************************************************
	*	�������ܣ�	����Ϣ���ڷ�HX_USER_COMPILEDATA��Ϣ
	*	����������	 void* pBuffer : [IN] ������Ϣʱ�ĵڶ�������
	*				WPARAM wParam  : [IN] ��Ϣ����ID, ������Ϣʱ�ĵ�һ������
	*	�� �� ֵ��	long : ��Ϣ������
	*	����������	
	*	��    �ߣ�
	*	������ڣ�	2003��4��16��
	*	�޸�˵����	
	*****************************************************************/
	long SendDataToWnd( void* pBuffer, WPARAM wParam );

	// �����������ݲ���
protected:
	AllFinancialData m_sAllFinancialData;
public:
	BOOL ReadFinancial(CString strCode,AllFinancialData* pAllFinancialData = NULL);

	HSDouble PROFFIN(CString strID,HSCurDataInfoStatus* pRefDataStatus,
		/*int nOffset,*/
		int Begin,int End,CString code,
		HSCurDataInfoStatus* pCurDataStatus, int& nError);
	HSDouble PROFFINON(CString strID,
		int Begin,int End,
		HSCurDataInfoStatus* pRefDataStatus,
		int nOffset,
		CString code,
		HSCurDataInfoStatus* pCurDataStatus, int& nError);
	HSDouble BlockCalc(CString strID,
		CString strBlockName,
		int Begin,int End,
		HSCurDataInfoStatus* pRefDataStatus,
		int nOffset,
		HSCurDataInfoStatus* pCurDataStatus, int& nError,
	    int nOperator = 0);
	
	int BlockCalc(CString strBlockName,
		int Begin,int End,
		AllFinancialData* pRetAllFinancialData,	
	    int nOperator = 0);
public:
	// �������, Ŀǰ���������Ʊ�Ͱ��Ĳ�������
	void EmptyData();
};

typedef CHSDataSource* lpHxDataSource;

#endif
