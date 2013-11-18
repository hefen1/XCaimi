/*************************************************************
*	Copyright (c)2010, �������ӹɷ����޹�˾
*	All rights reserved.
*
*	�ļ����ƣ�	FormulaCalc.h
*	�ļ���ʶ��	
*	ժ	  Ҫ��	ѡ��,����,Ԥ��(��),�������(�������,���Ű��,ǿ�ȷ���,
				ָ������)�ȵļ������
*
*	��ǰ�汾��	1.0
*	��	  �ߣ�	
*	�������ڣ�	
*
*	�޸�˵��:	
***************************************************************/

#if !defined(AFX_HSCALCULATE_H__5909144D_1313_477B_B0C9_55B772666225__INCLUDED_)
#define AFX_HSCALCULATE_H__5909144D_1313_477B_B0C9_55B772666225__INCLUDED_

#include "PubStruct.h"
#include "FormulaManPublic.h"
#include "DataSourceDefineEx.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define PERIOD_TYPE_DAY_ANY		0x00A0		//������������
#define MINUTE_ANY				0x00B0		//������������
#define HS_USR_DEF_BEGIN 1
// �ṹAskDataEx�ͽṹAskDataǰ�沿�ֱ�����ͬ
struct AskDataEx
{
	unsigned short 		m_nType;	     // ��������
	char				m_nIndex;     	 // �������������������ݰ�һ��
	char				m_cOperator;   	 // ����(Ŀǰ��ʹ��)
	long				m_lKey;		 	 // һ����ʶ��ͨ��Ϊ���ھ��
	HSPrivateKey	    m_nPrivateKey;	 // ������ʶ

	short   			m_nSize;         // ����֤ȯ������С����ʱ��
	short 				m_nAlignment;    // Ϊ��4�ֽڶ������ӵ��ֶ�

	ReqDayData          m_pCode[1];

	AskDataEx()
	{
		memset(this,0,sizeof(AskDataEx));
	}
};


#define  SUPPORT_OLD
//#include "..\comm\sscomm3.h"

struct SelStockCondition;
struct InportDataParam;
class CSystemFun;
class CMapVariabile;
class CHSDataSource;

/*************************************************************
*	�� �� �ƣ�	CFormulaCalc
*	�� �� ����	ѡ��,����,Ԥ��(��),�������(�������,���Ű��,ǿ�ȷ���,
				ָ������)�ȵļ��������
***************************************************************/
class BCGCONTROLBARDLLEXPORT CFormulaCalc  
{
public:
	CFormulaCalc();
	virtual ~CFormulaCalc();

public:
	// �Զ�ɾ������(CFormulaCalc������)
	struct _Auto_Delete_Data
	{
		~_Auto_Delete_Data();
	};
	friend _Auto_Delete_Data;

	// �Զ�������ʱʹ�õ��м��������(������������ص���Ϣ)
	struct FormulaVarColData
	{
		int      m_nID;		// ��ID
		CString  m_strName; // ����
		FormulaVarColData(int nID,CString strName)
		{
			m_nID = nID;
			m_strName = strName;
		}
	};

	// ָ�����򷵻أ������
	struct FormulaIndicatorSortResult
	{
		// ��ǰ��Ʊ
		void*	m_pStock;
		// �Զ�������Ϣ
		CArray<FormulaVarColData*,FormulaVarColData*>	m_ayColName;

		// ���㷵��һ�е�����--����(KEYֵΪ�Զ����е�ID)
		CMap<int,int,HSDouble*,HSDouble*>	m_ayValue;
		// ���㷵��һ�е�����--�ַ�(KEYֵΪ�Զ����е�ID)
		CMap<int, int, CString*, CString*>	m_ayValueEx;

		FormulaIndicatorSortResult()
		{
			m_pStock = NULL;
		}
		~FormulaIndicatorSortResult()
		{
			Free();
		}

		// �õ���nCol�е�����Ϣ
		CFormulaCalc::FormulaVarColData* GetColName(int nCol);
		// ��һ����nColΪ��ID, ��strNameΪ��������Ϣ
		int AddColName(int nCol,CString strName);
		// ���˽�пռ�(��������Ϣ)
		void	  Free();
		// �������
		void	  EmptyData();
		// �õ���IDΪnCol���е�����,������в���,�򷵻�NULL
		HSDouble* GetData(int nCol);
		CString*  GetDataEx(int nCol);
		// ������IDΪnCol���е�����
		void	  SetData(int nCol,HSDouble* pRefData);
		void	  SetData(int nCol, CString& strData);
	};

	// �����Զ�������Ϣ(����������ص���Ϣ)
	struct FormulaBaseOtherColData
	{
		int		m_nID;					// ��id
		CString m_strName;				// ������
		FormulaGeneralParam*	m_pParam;	// ͨ�ò���

		COMPILEFUNCTION   m_pFun;		//��ȡ���ݵĺ���ָ�� COMPILEFUNCTION

		FormulaBaseOtherColData()
		{
			m_nID = -1;
			m_pFun = NULL;
			m_pParam = NULL;
		}

		~FormulaBaseOtherColData()
		{
			if( m_pParam != NULL )
				delete m_pParam;
			m_pParam = NULL;
		}

		FormulaBaseOtherColData& operator=(FormulaBaseOtherColData& sData)
		{
			m_nID = sData.m_nID;
			m_strName.Format("%s", sData.m_strName);
			m_pFun = sData.m_pFun;
			if( m_pParam != NULL )
			{
				delete m_pParam;
				m_pParam = NULL;
			}
			if( sData.m_pParam == NULL )
			{
				return *this;
			}
			m_pParam = new FormulaGeneralParam;
			for( int i = 0; i < sData.m_pParam->GetSize(); i++ )
			{
				LPNODELEMENT pItem = sData.m_pParam->GetParam(i);
				if( pItem == NULL )
				{
					continue;
				}
				m_pParam->Add(pItem);
			}
			return *this;
		}
		/******************************************************************
		*	�������ܣ�	�õ���ǰ�е�����,�������ݷ��뵽pBaseDataEx����
						�������Ű�����������б����������ʱ,���ô˺���
		*	����������	CFormulaCalc::FormulaIndicatorSortResult* pBaseDataEx : [out] �������
		*				int nPos : [in] Ҫ�󷵻ص����ݵ�offset(��Ϊ0�򷵻ص�ǰ���������еĵ�һ������)
		*	�� �� ֵ��	int : 1 �ɹ�, -1 ��������ȷ, ���� �����������Ĵ����
		*	����������	
		*	��    �ߣ�	
		*	������ڣ�	2003��4��22��
		*	�޸�˵����	
		*****************************************************************/
		int GetData(CFormulaCalc::FormulaIndicatorSortResult* pBaseDataEx,int nPos);

		/******************************************************************
		*	�������ܣ�	�õ���ǰ�е�����,�������ݷ��뵽pBaseDataEx����	
						���Ű�����ʱ,���ô˺���
		*	����������	CFormulaCalc::FormulaIndicatorSortResult* pBaseDataEx : [out] �������
		*				void* ayNowData : [in] �����������Ҫ�õ�����������
		*	�� �� ֵ��	int : 1 �ɹ�, -1 ��������ȷ, ���� �����������Ĵ���� 
		*	����������	
		*	��    �ߣ�	
		*	������ڣ�	2003��4��22��
		*	�޸�˵����	
		*****************************************************************/
		int GetData(CFormulaCalc::FormulaIndicatorSortResult* pBaseDataEx, void* ayNowData);
	};

	// �����Զ����е���Ϣ(����������ص���Ϣ)
	struct FormulaAddColParam
	{
		CArray<FormulaBaseOtherColData*,FormulaBaseOtherColData*>* payOther; // �����Զ�����
		CArray<SingleSelCondition*,SingleSelCondition*>*			   pExArray; // ָ���Զ�����
	};


	/******************************************************************
	*	�������ܣ�	��ayOther����Ϣ���뵽pBaseDataEx����,�����·�������ɵ�ID�������Զ�����
					���������ID������Ϣ���͸�Ŀ�괰��,�����б��ͷ�ĳ�ʼ������
	*	����������	CFormulaCalc::FormulaIndicatorSortResult* pBaseDataEx : [out] �����������Ϣ(ÿ�а���ΨһID)
	*				CArray<FormulaBaseOtherColData* 
	*				FormulaBaseOtherColData*>& ayOther : [in] ��ʼ������Ϣ,IDû�з����,�����������м�����ص���Ϣ
	*	�� �� ֵ��	int : 0 �������Ϸ�, 1 ����ɹ�
	*	����������	
	*	��    �ߣ�	
	*	������ڣ�	2003��4��22��
	*	�޸�˵����	
	*****************************************************************/
	static int AddBaseOtherColData( CFormulaCalc::FormulaIndicatorSortResult* pBaseDataEx,
						       	    CArray<FormulaBaseOtherColData*,FormulaBaseOtherColData*>& ayOther );

	/******************************************************************
	*	�������ܣ�	��ayOther����Ϣ���뵽pBaseDataEx����,�������·���ID
					���������ID������Ϣ���͸�Ŀ�괰��,�����б��ͷ�ĳ�ʼ������
	*	����������	CFormulaCalc::FormulaIndicatorSortResult* pBaseDataEx : [out] �����������Ϣ(ÿ����ʵҲ����ΨһID)
	*				CArray<FormulaBaseOtherColData*
	*				FormulaBaseOtherColData*>& ayOther : [in] ��ʼ������Ϣ, ���������м�����ص���Ϣ
	*	�� �� ֵ��	int : 0 �������Ϸ�, 1 ����ɹ�
	*	����������	
	*	��    �ߣ�	
	*	������ڣ�	2003��4��22��
	*	�޸�˵����	
	*****************************************************************/
	static int AddBaseOtherColDataEx( CFormulaCalc::FormulaIndicatorSortResult* pBaseDataEx,
						       	    CArray<FormulaBaseOtherColData*,FormulaBaseOtherColData*>& ayOther );


	

	/*************************************************************
	*	�� �� �ƣ�	CalcRetData
	*	�� �� ����	ѡ������м�������ݽṹ
	***************************************************************/
	struct CalcRetData
	{
		SingleSelCondition*  pUnionData;		// ����ѡ������

		CLineProperty* pCalDataLeft;	// ��ߵļ�����
		CLineProperty* pCalDataRight;	// �ұߵļ�����
		CLineProperty* pCalDataMidlle;	// �м�ļ�����

		CExpValue* m_pLeft;				// ��ߵĹ�ʽ
		CExpValue* m_pMiddle;			// �м�Ĺ�ʽ
		CExpValue* m_pRight;			// �ұߵĹ�ʽ

		// �Ƿ��Ѿ���ʼ����ʽ(���ΪTRUE���ʾm_pLeft�����ҵ�)
		BOOL       m_bInitLeftRight;	
		// ָ������
		FormulaIndicatorSortResult* m_pHSTechSortResult; 
		// ���������
		CArray<CLineProperty*,CLineProperty*> m_ayData;	

		// ���������
		AskDataEx ask;	
		// �ɽ�����λ
		int     nChengJiaoDiv;	
		// ������������
		int		nAnaPeriod;				
		// ���������أ�һ��0�Ǽ���ʧ�ܣ�����0ֵ��ʾ�����
		int		nResult;	
		// ѡ�ɽ��
		CArray<SuccResult*,SuccResult*>		  m_aySucc;			
		// �ɹ���Ʊ �����ݻ����Բ�ʹ��
		CArray<StockUserInfo*,StockUserInfo*> m_aySuccStock;


		// ����ask����nChengJiaoDiv��nAnaPeriod, ����nPeriod����
		void    SetAsk(int nPeriod = -1);
		void    SetCurCode(CodeInfo* pCode);
		// �õ�������������(һ���൱������)
		int				   GetSize() { return m_ayData.GetSize(); } 
		// ɾ������m_ayData
		void			   DeleteData();  
		// ����m_ayData��С,ע��,ֻ��nCount<OldnCount(ԭ����С)ʱ,���ܴ���ɹ�
		void			   SetDataSize(int nCount);
		// �õ���nPosistion��������
		CLineProperty*     GetDataAt(int nPosistion);
		// ��ÿ�����еļ������ĳ�������ΪnNumber
		void			   SetValueNumber(int nNumber);
		// ��һ��ѡ�ɳɹ�����
		void AddSucc(long lDate,long nPos) { m_aySucc.Add(new SuccResult(lDate,nPos) ); }
		void AddSucc(SuccResult* pSucc)    { m_aySucc.Add(pSucc); }
		// ɾ�����гɹ�����
		void RemoveAllSucc()
		{
			for(int i = 0; i < m_aySucc.GetSize(); i++)
			{
				delete m_aySucc.GetAt(i);
			}
			m_aySucc.RemoveAll();
		}
		
		CalcRetData()
		{
			pUnionData = NULL;

			pCalDataLeft   = NULL;
			pCalDataRight  = NULL;
			pCalDataMidlle = NULL;

			m_pLeft = NULL;
			m_pMiddle = NULL;
			m_pRight = NULL;
			m_bInitLeftRight = FALSE;

			//
			nChengJiaoDiv = 0; // �ɽ�����λ
			nAnaPeriod = 0;
			memset(&ask,0,sizeof(ask));

			m_pHSTechSortResult = NULL;

			//
			nResult = 0;
		}

		~CalcRetData()
		{
			if( m_pHSTechSortResult )
			{
				delete m_pHSTechSortResult;
				m_pHSTechSortResult = NULL;
			}

			DeleteData();
			RemoveAllSucc();
		}
	};

	// ѡ�ɼ����м�ṹ
	struct UnionCalc
	{
		// ����������
		long	m_lDateCount;
		// �����������
		long	m_lEndDate;
		// ������֮���ǽ�����TRUE�����ǲ�����FALSE���Ĺ�ϵ
		BOOL	bAndOr;
		// ���ɵ�����ѡ�ɼ������ݼ��������
		CArray<CalcRetData*,CalcRetData*> subArray;
		// ����Ʊѡ�����ս����K��ѡ�е㣩
		CArray<SuccResult*,SuccResult*>   m_aySucc;

		// ��m_aySucc��һ���ɹ�
		void AddSucc(long lDate,long nPos) { m_aySucc.Add(new SuccResult(lDate,nPos) ); }
		void AddSucc(SuccResult* pSucc)    { m_aySucc.Add(pSucc); }
		// ɾ��m_aySucc���гɹ�������
		void RemoveAllSucc()
		{
			for(int i = 0; i < m_aySucc.GetSize(); i++)
			{
				delete m_aySucc.GetAt(i);
			}
			m_aySucc.RemoveAll();
		}

		UnionCalc()
		{
			bAndOr = 0;
		}

		~UnionCalc()
		{
			RemoveAllSucc();
		}
	};

public:
	/******************************************************************
	*	�������ܣ�	�ɹ�ʽ���Ƶõ���ʽ�ڲ���ͼ���ʽ��
	*	����������	CString strName : [in] ��ʽ���� 
	*				int nMask = Tech : [in] ��ʽ����
	*	�� �� ֵ��	CMapVariabile* : ���صĹ�ʽ�ڲ���ͼ��
	*	����������	
	*	��    �ߣ�	
	*	������ڣ�	2003��5��14��
	*	�޸�˵����	
	*****************************************************************/
	static CMapVariabile*  GetExpDraw(CString strName,int nMask = Tech);

	/******************************************************************
	*	�������ܣ�	�ɹ�ʽ���Ƶõ����ʽ(��ʽ)
	*	����������	CString strName : [in] ��ʽ����
	*				int nMask = Tech : [in] ��ʽ����
	*	�� �� ֵ��	CExpression* : ��ʽָ��
	*	����������	
	*	��    �ߣ�	
	*	������ڣ�	2003��5��14��
	*	�޸�˵����	
	*****************************************************************/
	static CExpression*    GetExpress(CString strName,int nMask = Tech);

	/******************************************************************
	*	�������ܣ�	��ʼ��retData����,������ָ����strDrawName
	*	����������	CFormulaCalc::CalcRetData& retData : [in/out] ��������
	*				CString strDrawName : [in] ָ��������
	*				CString strName : [in] ��ʽ����
	*				int nMask		: [in] ��ʽ����
	*				BOOL bTechLine  : [in] ���ù�ʽ����Դ
											0x01 ����ΪCFormulaCalc::m_pHxDataSource
											0x02 ����Ϊ��ʽĬ������Դ
	*				CFormulaCalc::FormulaIndicatorSortResult* pHSTechSortResult = NULL : [in] Ŀǰû��
	*	�� �� ֵ��	CExpValue* : �õ�������ΪstrDrawName��ָ���߹�ʽ����ָ��
	*	����������	
	*	��    �ߣ�	
	*	������ڣ�	2003��5��14��
	*	�޸�˵����	
	*****************************************************************/
	static CExpValue* GetDrawLine(CFormulaCalc::CalcRetData& retData,
		CString strDrawName,CString strName,
		int nMask /*= Tech*/,BOOL bTechLine = FALSE,
		CFormulaCalc::FormulaIndicatorSortResult* pHSTechSortResult = NULL);

	// ���m_UnionArray��m_ayFreeList����
	static void DeleteAll();
	// ���Ԥ�������������
	static void AlarmDeleteAll();
	// ����Ԥ���������
	static BOOL AlarmCreateAll();

	/******************************************************************
	*	�������ܣ�	ѡ�ɼ���(ͨ������ָ��������������)
	*	����������	SingleSelCondition*  pUnionData : [in/out] ��������
	*				CalcRetData& retData : [out] ��������
	*				int nBegin : [in] ��ʼ��
	*				int nEnd   : [in] ������
	*	�� �� ֵ��	int : 0ʧ�ܣ� ������ɹ�
	*	����������	UnionCal,Condition,MoreKLine,Exchange,DaYu,XiaoYu,DengYu,JieYu,ShangChuan,XiaoPo,WeiChi,BiaoChi
	*	��    �ߣ�	
	*	������ڣ�	2003��5��14��
	*	�޸�˵����	
	*****************************************************************/
	static int SingleCal( SingleSelCondition*  pUnionData,CalcRetData& retData,int nBegin,int nEnd);

	static int UnionCal( SelStockCondition* pUnionSubData,CalcRetData& retData,int nBegin,int nEnd);

	/******************************************************************
	*	�������ܣ�	��������ָ���ߣ���������������retData��
	*	����������	CString& strName  : [in] ��ʽ��
	*				int& nMask		  : [in] ��ʽ����
	*				CString& strLine1 : [in] ָ����1����
	*				CString& strLine2 : [in] ָ����2����
	*				CString& strLine3 : [in] ָ����3����
	*				CFormulaCalc::CalcRetData& retData : [in/out] ���������ݼ�����������
	*				int& nBegin : [in] ���㿪ʼ��
	*				int& nEnd   : [in] ���������
	*				FormulaIndicatorSortResult* pHSTechSortResult = NULL : [in] ������
	*	�� �� ֵ��	int : ʧ�ܣ�-1���� ����ɹ�
	*	����������	
	*	��    �ߣ�	
	*	������ڣ�	2003��5��14��
	*	�޸�˵����	
	*****************************************************************/
	static int CalcAll( CString& strName,int& nMask,
						CString& strLine1,CString& strLine2,CString& strLine3,
						CFormulaCalc::CalcRetData& retData,
						int& nBegin,int& nEnd,
						FormulaIndicatorSortResult* pHSTechSortResult = NULL);


	/******************************************************************
	*	�������ܣ�	����ѡ�ɼ���
	*	����������	SingleSelCondition*  pUnionData : [in] ����������
	*				CalcRetData& retData	  : [in/out] ����������ͬʱ���������(ѡ�ɳɹ���retData.m_aySucc.GetSize()>0)
	*				int nBegin	: [in] ���㿪ʼ��
	*				int nEnd	: [in] ���������
	*	�� �� ֵ��	int : 0ʧ�ܣ� ������ɹ�
	*	����������	
	*	��    �ߣ�	
	*	������ڣ�	2003��5��14��
	*	�޸�˵����	
	*****************************************************************/
	static int Condition(SingleSelCondition*  pUnionData,
							 CalcRetData& retData,
							 int nBegin,int nEnd);
	//	����ϵͳѡ�ɼ���
	static int	Exchange(SingleSelCondition*  pUnionData,
							 CalcRetData& retData,
							 int nBegin,int nEnd);	
	//	���K�� ѡ�ɼ���
	static int MoreKLine(SingleSelCondition*  pUnionData,
							 CalcRetData& retData,
							 int nBegin,int nEnd);	

	//  ����ѡ�ɼ���
	static int DaYu(SingleSelCondition*  pUnionData,
					CalcRetData& retData,
					int nBegin,int nEnd);		
	//	С��ѡ�ɼ���
	static int XiaoYu(SingleSelCondition*  pUnionData,
					CalcRetData& retData,
					int nBegin,int nEnd);		
	//	����ѡ�ɼ���
	static int DengYu(SingleSelCondition*  pUnionData,
					CalcRetData& retData,
					int nBegin,int nEnd);		
	//	����ѡ�ɼ���
	static int JieYu(SingleSelCondition*  pUnionData,
					CalcRetData& retData,
					int nBegin,int nEnd);		
	//	�ϴ�ѡ�ɼ���
	static int ShangChuan(SingleSelCondition*  pUnionData,
					CalcRetData& retData,
					int nBegin,int nEnd);		
	//	����ѡ�ɼ���
	static int XiaoPo(SingleSelCondition*  pUnionData,
					CalcRetData& retData,
					int nBegin,int nEnd);		
	//	ά��ѡ�ɼ���
	static int WeiChi(SingleSelCondition*  pUnionData,
					CalcRetData& retData,
					int nBegin,int nEnd);		
	//	����ѡ�ɼ��� 
	static int BiaoChi(SingleSelCondition*  pUnionData,
					CalcRetData& retData,
					int nBegin,int nEnd);	

	static BOOL IsStoping() { return (m_nStopThread==0); }

public:
	// �߳�״̬ 
	// -2 ��������
	//  0 ��������
	//  2 Ҫ��ֹͣ����
	static int		m_nStopThread;
	// �߳̾��(ûʲô�� ??? )
	static HANDLE	m_hThread;

	// Ԥ�������߳�״̬ 
	// -2 ��������
	//  0 ��������
	//  2 Ҫ��ֹͣ����
	static int		m_nStopAlarmThread;
	// �߳̾��(ûʲô�� ??? )
	static HANDLE	m_hAlarmThread;

	// �Ҳ�����ʲô�ô����Ż�ʱ����ɾ��( ??? )
	static CArray<CExpValue*,CExpValue*> m_ayFreeList;
	// ѡ�ɼ������ݣ���������Ӧ�����ļ��㷵�أ�
	static CArray<UnionCalc*,UnionCalc*> m_UnionArray;
	// Ԥ���������ݣ���������Ӧ�����ļ��㷵�أ�
	static CArray<UnionCalc*,UnionCalc*> m_AlarmUnionArray;


	// ��ʼ������m_pHSDisk������Դ����m_pHxDataSource
	static BOOL InitObj();
	// ��ʼ��Ԥ������m_pAlarmDisk��Ԥ������Դ����m_pAlarmHxDataSource
	static BOOL InitAlarmObj();

public:
	// ����Դ�ӿ�
	static InportDataParam* m_pHxDataSource;
	// Ԥ������Դ�ӿ� 
	static InportDataParam* m_pAlarmHxDataSource;

	// �����߳�
public:
	// ��ʼ�����߳�
	static int  Start();
	// ֹͣ�����߳�
	static void Stop();
	// ��û�ã��Ż�ʱɾ�� ??? 
	static int  StartExchSys();

	// �����߳�
	static BOOL HSCreateThread(LPTHREAD_START_ROUTINE fnThreadFun);

	// ��ʼԤ�������߳�
	static int	StartAlarm();
	// ֹͣԤ�������߳�
	static void StopAlarm();

	/******************************************************************
	*	�������ܣ�	ѡ�ɼ��㣨������ѡ����)
	*	����������	LPVOID pParameter : [in] ������
	*	�� �� ֵ��	DWORD : ��Ϊ0
	*	����������	
	*	��    �ߣ�	
	*	������ڣ�	
	*	�޸�˵����	
	*****************************************************************/
	static DWORD WINAPI Calculate(LPVOID pParameter);
	// ָ���������
	static DWORD WINAPI TechSortCalculate(LPVOID pParameter);
	// ����������
	static DWORD WINAPI HSBlockCalculate(LPVOID pParameter);
	// ���Ű�鱨�����
	static DWORD WINAPI HSBlockReportCalulate(LPVOID pParameter);
	// ����ѡ�ɼ��� Ŀǰ����
	static DWORD WINAPI HSCalculate(LPVOID pParameter);
	// ���ã�����ʱɾ�� ???
	static DWORD WINAPI HSExchCalc(LPVOID pParameter);
	// ���ã�����ʱɾ�� ???
	static DWORD WINAPI HSPhaseSortCalculate(LPVOID pParameter);
	// �߼���Ԥ������
	static DWORD WINAPI AlarmCalculate(LPVOID pParameter);
	//��ȡ��������ӿ�ָ��
	static IDataSourceEx* GetDataSourcePtr();
	

	// �ⲿ����
	protected:
	static	IDataSourceEx* m_pDataSource;       //��������ӿ�
public:
	
	static CMapVariabile*  m_pExternExpression;      // �ⲿ���ʽ��
	static CMapVariabile*  m_pExternVariabile;		 // �ⲿ������
	static CMapVariabile*  m_pExternFunction;		 // �ⲿ������
	static CSystemFun*	   m_pSystemFun;			 // ����ָ��

	static CWnd* m_pMsgWnd;							 // ��Ϣ����
	static CWnd* m_pAlarmWnd;						 // Ԥ����Ϣ����

	static SelStockCondition* m_curSelStockCondition; // �ȹ�����
	static TExchSys*			 m_pExchSys;			 // û�ã�ȥ�� ??? 
	static CArray<AlarmCalcData*, AlarmCalcData*>* m_pAlarmCondition; // Ԥ�����������


public:
	/******************************************************************
	*	�������ܣ�	Ѱ����������pDay��lDate���ڵ�λ��
	*	����������	long lDate : [in] �������� ��20020901
	*				long lLen  : [in] ����
	*				StockDay* pDay : [in] �������� 
	*				int lSize : [in] pDay�������ݸ���
	*	�� �� ֵ��	int : -1 �Ҳ����� ����0 lDate��pDay�е�λ��
	*	����������	
	*	��    �ߣ�	
	*	������ڣ�	2003��5��14��
	*	�޸�˵����	
	*****************************************************************/
	static int FindDate(unsigned long lDate,long lLen,StockDay* pDay,int lSize);//--2010.11.12 litao����2012ʱ������
	static int FindDateExact(unsigned long lDate,long lLen,StockDay* pDay, int lSize, BOOL bIsBegin = TRUE);//--2010.11.12 litao����2012ʱ������

	/******************************************************************
	*	�������ܣ�	�������ļ�IntelligentSelect.ini��д������
	*	����������	CString strSection : [in] ����
	*				CString strKeyName : [in] Key��
	*	�� �� ֵ��	BOOL : �ɹ�(TRUE), ʧ�ܣ�FALSE��
	*	����������	
	*	��    �ߣ�	
	*	������ڣ�	2003��5��14��
	*	�޸�˵����	
	*****************************************************************/
	static BOOL SaveToIni(CString strSection ,CString strKeyName);

public:
	/******************************************************************
	*	�������ܣ�	ɾ�����гɹ�����m_mapSelectSuccInfo
	*	����������	BOOL bSelf = TRUE : [in] �ǲ�ʱɾ������
	*	�� �� ֵ��	int : ��Ϊ0
	*	����������	
	*	��    �ߣ�	
	*	������ڣ�	2003��5��14��
	*	�޸�˵����	
	*****************************************************************/
	static int  DelAllSuccData(BOOL bSelf = TRUE);

	/******************************************************************
	*	�������ܣ�	��ѡ�ɳɹ����ݵ�����
	*	����������	CodeInfo* pCode : [in] ѡ���Ĺ�Ʊ������Ϣ
	*				CArray<SuccResult* 
	*				SuccResult*>* pArray : [in] �����K�ߵ�λ������
	*	�� �� ֵ��	void 
	*	����������	
	*	��    �ߣ�	
	*	������ڣ�	2003��5��14��
	*	�޸�˵����	
	*****************************************************************/
	static void AddSuccDataToMap(CodeInfo* pCode,CArray<SuccResult*,SuccResult*>* pArray);
//	static CArray<SuccResult*,SuccResult*>* AddSuccDataToMap(CodeInfo* pCode);


	//add by lxqi 20090310	���Խ��׼���//////////////////////////////////////////////////////
	/******************************************************************
	*	�������ܣ�	���Խ��׼��㣬�ж���������
	*	����������	CLineProperty*& pRefData : [in] ���������ݣ�����ԭʼ���ݡ���ؽ���ͼ��㹫ʽ��
	*				HSCurDataInfoStatus* pCurDataStatus : [out] ��ż���������Ϊ���򲻴��
	*	�� �� ֵ��	int		: >0ʱΪ�����źţ�<0ʱΪ�����źţ�=0 Ϊ������������Ϣ
	*	����������	
	*	��    �ߣ�	������
	*	������ڣ�	2009��3��10��
	*	�޸�˵����	
	*****************************************************************/
	static int CalcStrategyTrade(CLineProperty*& pRefData,HSCurDataInfoStatus* pCurDataStatus = NULL,int nSize = 0);
	//end by lxqi 20090310

};

typedef int ( *fnHSCompareCondition)( SingleSelCondition*  pUnionData,
									   CFormulaCalc::CalcRetData& retData,
									   int nBegin,int nEnd );

#endif // !defined(AFX_HSCALCULATE_H__5909144D_1313_477B_B0C9_55B772666225__INCLUDED_)
