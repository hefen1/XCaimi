/*************************************************************
*	Copyright (c)2010, �������ӹɷ����޹�˾
*	All rights reserved.
*
*	�ļ����ƣ�	ExchData.h
*	�ļ���ʶ��	
*	ժ	  Ҫ��	����ϵͳ�����Ϣ����
*
*	��ǰ�汾��	1.0
*	��	  �ߣ�	
*	�������ڣ�	2003-1-8
*
***************************************************************/

#ifndef _ExchData_H_
#define _ExchData_H_

#include "Express.h"
#include "ExpPublic.h"
#include "VarValue.h"
#include "hsds_comudata.h"
// ����ϵͳ
// ��������
#define EXCH_DATA_TYPE_Exp	1
#define EXCH_DATA_TYPE_Mode	2
#define EXCH_DATA_TYPE_Buy	3
#define EXCH_DATA_TYPE_Sell	4
#define EXCH_DATA_TYPE_Optimize	5

// ֪ͨ��Ϣ
// ����֪ͨ
#define EXCH_MSG_WP_CACL_TEMPO	7770
// ��ʼ
#define EXCH_MSG_WP_CACL_START	7771
// ֹͣ
#define EXCH_MSG_WP_CACL_STOP	7772
// ����ʧ��
#define EXCH_MSG_WP_CACL_FAIL	7773
// �������
#define EXCH_MSG_UP_CACL_SUCC	7774
// ��ѡ��ʼ
#define YouSel_MSG_WP_CACL_START 7775
// ��ѡֹͣ
#define YouSel_MSG_WP_CACL_STOP	7776
// ��ѡʧ��
#define YouSel_MSG_WP_CACL_FAIL 7777
// �������
#define YouSel_MSG_UP_CACL_SUCC	7778
// ��������BUY��ʽ
#define EXCH_MSG_WP_CALC_SET_BUY 7779

// ���뷽ʽ
#define BuyMethod_All_Buy			0	// ȫ������
#define BuyMethod_Portion_Buy		1	// ��������
#define	BuyMethod_Fixation_Money	2	// �̶��ʽ�����
#define	BuyMethod_Fixation_NUM		3	// �̶���Ʊ������

// �����źų���ʱ�Ĵ���ʽ
#define FollowBuyMethod_NO			0	// ��������
#define FollowBuyMethod_DL			1	// ��������
#define FollowBuyMethod_DJ			2	// �ݼ�����
#define FollowBuyMethod_DZ			3	// ��������

// �����������ƶ���
#define TDealFlow_DT_PingChang_Code	"ƽ��"
#define TDealFlow_DT_Buy_Code		"����"	
#define TDealFlow_DT_Sell_Code		"����"
#define TDealFlow_DT_ZhiShun_Code	"ֹ��"
#define TDealFlow_DT_ZhiYing_Code	"ֹӮ"
#define TDealFlow_DT_HuiLuo_Code	"����"
#define TDealFlow_DT_HenPang_Code	"����"
#define TDealFlow_DT_No_Code		"����"
#define TDealFlow_DT_QZPC_Code		"ƽ��"
// ��������ID����
#define TDealFlow_DT_PingChang		1	// ƽ��
#define TDealFlow_DT_Buy			100	// ����
#define TDealFlow_DT_Sell			3	// ����
#define TDealFlow_DT_ZhiShun		4	// ֹ��
#define TDealFlow_DT_ZhiYing		5	// ֹӮ
#define TDealFlow_DT_HuiLuo			6	// ����
#define TDealFlow_DT_HenPang		7	// ����
#define TDealFlow_DT_QZPC			8	// ǿ��ƽ��

// ����ϵͳ��������
#define TCaclReturn_NoData		100	// û����
#define TCaclReturn_VainParam	101 // ��������Ч
#define TCaclReturn_NoExperss	102 // û�й�ʽ��
#define TCaclReturn_CalFail		103 // ����ʧ��
#define TCaclReturn_MoneryFail	104 // �ڴ����ʧ��
#define TCaclReturn_TotalFail	105 // ͳ��ʧ��
#define TCaclReturn_Stop		106 // �û�ֹͣ
#define TCaclReturn_Succ		0   // �ɹ� 
#define TCaclReturn_Fail		1   // δ֪��ʧ��

// �����߳�״̬
#define TCaclThread_State_Stoping	-1 // �Ѿ�ֹͣ 
#define TCaclThread_State_Stop		0  // Ҫ��ֹͣ
#define TCaclTherad_State_Start		1  // ��������

extern double EXCH_POUNDAGE;    // ��������(�Ժ����ɿ�����)

/*************************************************************
*	�� �� �ƣ�	TExchHead
*	�� �� ����	���ݹ���ͷ
***************************************************************/
struct TExchHead
{
	BYTE	m_bType;	// ��������
	virtual void Serialize(CArchive& ar);
};

/*************************************************************
*	�� �� �ƣ�	TExchMsg
*	�� �� ����	��Ϣ��������
***************************************************************/
struct TExchMsg
{
	HWND	m_hNotifyWnd; // ��Ϣ���մ���
	UINT	m_hNotifyMsg; // ��ϢID
	BOOL	m_bNotify;    // �Ƿ���

	TExchMsg();

	// �ж��Ƿ�����Ϣ
	BOOL IsNotify();   
	// �򴰿�m_hNotifyWnd����m_hNotifyMsg��Ϣ
	LRESULT SendMsg(WPARAM wp, LPARAM lp);
};

/*************************************************************
*	�� �� �ƣ�	TExchExp
*	�� �� ����	��ʽ����
***************************************************************/
struct TExchExp: public TExchHead
{
	CExpression*	m_pMainTech;	// ��ʽ��
	long			m_lCyc;			// ����
	BOOL		    m_bExRights;			// �Ƿ��Ȩ���� 

	TExchExp();
	TExchExp& operator=(TExchExp& sExp);
	// �õ���ʽ��������
	CString GetExpTypeName();
	// �õ���ʽ������ﴮ
	CString GetParamStr();
	void Serialize(CArchive& ar, CMapVariabile* pMapExpression = NULL);
};

// �õ���Ʊ�������ݵĺ���ָ������
/*typedef StockUserInfo* (*GETSTOCKINFOPROC)(CodeInfo& sCode);*/
typedef void (*GETSTOCKINFOPROC)(CodeInfo sCode, StockUserInfo&);
/*************************************************************
*	�� �� �ƣ�	TExchModel
*	�� �� ����	�г�����
***************************************************************/
struct TExchModel: public TExchHead
{
	BOOL	m_bSingle;			// ����Ʊ���Ƕ��Ʊ
	short   m_nHoleStockCount;	// ͬʱ���й�Ʊ��
	BOOL	m_bBuyAll;			// ȫ�����뻹�Ƕಿ������
	short   m_nOneBuyCount;		// һ����������
	BOOL	m_bBuyRandom;		// �������
	BOOL	m_bSToB;						// ��С����  ��m_bBuyRandom == 0
	SingleSelCondition	m_sSortReason;			// �������
	double    m_lAllInvestCount;			// �����ʽ�
	GETSTOCKINFOPROC m_ProcGetStockUser;	// ��Ʊ���ݻ�ú���ָ��
	CArray<StockUserInfo*, StockUserInfo*>	m_ayStkList; // ��Ʊ�б�
	
	TExchModel();
	~TExchModel();
	
	void Clear();
	void Copy( TExchModel &Model );
	TExchModel& operator=(TExchModel& Mode);
	void Serialize(CArchive& ar);
};

/*************************************************************
*	�� �� �ƣ�	TExchBuy
*	�� �� ����	�����������
***************************************************************/
struct TExchBuy: public TExchHead
{
	// ����ʱ�������
	long			m_lDateCount;		// ����ʱ��γ���
	static long		m_lBeginDate;		// ��ʼ����ʱ�� 
	static long		m_lEndDate;			// ��������ʱ��
	BOOL			m_bNoCountIn;		// ǿ��ƽ�ֱȼ�����Խ����1�����룬0������

	// �����������
	CString			m_strTechLeft;		// �Ƚ�ָ��һ
	FormulaCompareSubData m_SubData;			// ������ϵ(�������)
	short			m_nBuyMethod;		// �����Ʊ�ķ�ʽ��ȫ�����룬�������룬�̶��ʽ����룬�̶���Ʊ������
	float           m_fOneBuyCount;     // һ����������

	// �����ź�����
	short			m_nFollowBuyMethod;  // ���������ź�ʱ�����뷽ʽ ���������룬�������룬�ݼ����룬��������
	float           m_fAddOrSubCount;    // ������ݼ�����
	BOOL			m_bSkip;			 // ��������
	short			m_nInPeriod;		 // �����������ֵ�����

	TExchBuy();
	TExchBuy& operator=(TExchBuy& Buy);
	void Default(CExpression *pExp, CString strDefault);
	void Serialize(CArchive& ar);
};

/*************************************************************
*	�� �� �ƣ�	TExchSell
*	�� �� ����	������������
***************************************************************/
struct TExchSell: public TExchHead
{
	BOOL	m_bMaidPeriod;		//Ŀ������ƽ��
	BOOL	m_bWinPeriod;		//Ŀ������ֹӮ
	BOOL	m_bLosePeriod;		//Ŀ��ֹ��
	BOOL	m_bFallPeriod;		//����ƽ��
	BOOL	m_bUpPeriod;		//����ƽ��

	float	m_nMaidPeriod;		//Ŀ������ƽ��
	float	m_nWinPeriod;		//Ŀ������ֹӮ
	float	m_nLosePeriod;		//Ŀ��ֹ��
	short	m_nFallPeriod;		//����ƽ��
	short	m_nUpPeriod;		//����ƽ��
	short   m_nFallPeriodCount;		//����ƽ����ֵ
	short	m_nUpPeriodCount;		//����ƽ����ֵ

	TExchSell();
	TExchSell& operator=(TExchSell& sell);
	void Serialize(CArchive& ar);
};

/*************************************************************
*	�� �� �ƣ�	TExchParam
*	�� �� ����	��ʽ�Ĳ���(�Ż�ʱ��)
***************************************************************/
struct TExchParam
{
	UINT	m_uParamID; // ����ID
	long	m_lDecimal;	// С��λ
	long	m_lSize;	// m_pParam����
	double*	m_pParam;   // ��������

	TExchParam()
	{
		m_pParam = NULL;
		m_lSize = 0;
		m_lDecimal = 0;
	}

	~TExchParam()
	{
		Free();
	}

	TExchParam& operator=(TExchParam& sData)
	{
		m_uParamID = sData.m_uParamID;
		m_lDecimal = sData.m_lDecimal;

		if( New(sData.m_lSize) != NULL )
		{
			memcpy(m_pParam, sData.m_pParam, sizeof(double)*m_lSize);
		}

		return *this;
	}

	// �ռ���䲢���ط���Ŀռ�
	double* New(long lSize)
	{
		Free();

		m_pParam = new double[lSize];
		if( m_pParam != NULL )
		{
			m_lSize = lSize;
		}

		ZeroMemory(m_pParam, sizeof(double)*m_lSize);

		return m_pParam;
	}

	// ���
	void Free()
	{
		if( m_pParam != NULL && m_lSize > 0 )
			delete[m_lSize] m_pParam;
		m_pParam = NULL;
		m_lSize = 0;
	}

	// �ռ��Ƿ���Ч
	BOOL IsValid()
	{
		if( m_pParam != NULL && m_lSize > 0
			&& ::AfxIsValidAddress(m_pParam, sizeof(double)*m_lSize, TRUE) )
			return TRUE;

		return FALSE;
	}

	// ת�����ִ�,��m_pParam={0.1, 1.2, 0.23} --> "(0.1,1.2,0.23)"
	CString ToStr()
	{
		if( !IsValid() )
			return CString("�޲���");

		CString str("("), strTemp;
		for( int i = 0 ; i < m_lSize; i++ )
		{
			strTemp.Format("%.2f", m_pParam[i]);
			strTemp.TrimRight('0');
			strTemp.TrimRight('.');
			if( i != 0 ) str += ",";
			str += strTemp;
		}

		str += ")";
		return str;
	}

	// ����ʽ�еĲ���ת��m_pParam��
	BOOL ConvertFromExpress(CExpression* pExpress)
	{
		if( pExpress == NULL )
			return FALSE;

		CMapVariabile* pMapVal = pExpress->GetParamVar();
		if( pMapVal == NULL )
			return FALSE;

		if( !New(pMapVal->GetSize() ) )
			return FALSE;

		for( int i = 0; i < m_lSize; i++ )
		{
			CNumericValue* pItem = (CNumericValue*)pMapVal->GetAt(i);

			if( pItem != NULL )
			{
				m_pParam[i] = pItem->GetValue();
			}
		}

		return TRUE;
	}

	// ��m_pParam�������õ���ʽpExpress��
	BOOL ConvertToExpress(CExpression* pExpress)
	{
		if( pExpress == NULL )
			return FALSE;

		CMapVariabile* pMapVal = pExpress->GetParamVar();
		if( pMapVal == NULL )
			return FALSE;

		if( pMapVal->GetSize() != m_lSize )
			return FALSE;

		for( int i = 0; i < m_lSize; i++ )
		{
			CNumericValue* pItem = (CNumericValue*)pMapVal->GetAt(i);

			if( pItem != NULL )
			{
				pItem->SetValue(m_pParam[i]);
			}
		}

		return TRUE;
	}
};

/*************************************************************
*	�� �� �ƣ�	TExchOptimizeItem
*	�� �� ����	�����Ż�ѡ��
***************************************************************/
struct TExchOptimizeItem
{
	TCHAR  m_szName[64]; // ������
	double m_dMin;		 // ��Сֵ	
	double m_dMax;		 // ���ֵ
	double m_dAdd;		 // ���Բ���	

	TExchOptimizeItem();
	~TExchOptimizeItem();
	TExchOptimizeItem& operator=(const TExchOptimizeItem& sItem);
	long GetNum();
	// �ڵ���N������
	double GetParam(int nIdx);
	CString GetName() {  return CString(m_szName); }
	void Serialize(CArchive& ar);
	// �������������ת����CNumericValue
	void ConvertToNumVal(CNumericValue& sValue);
	// ��CNumericValueת����������
	void ConvertFromNumVal(CNumericValue* sValue, CMapVariabile* sMapVal, BOOL bValid = FALSE);
};

/*************************************************************
*	�� �� �ƣ�	TExchOptimize
*	�� �� ����	�����Ż���������
***************************************************************/
struct TExchOptimize: public TExchHead
{
	// ��Ч����
	int lValidLen;
	// ��������С
	int lBufLen;
	// ���ݻ�����
	TExchOptimizeItem* pItem;
	// ��Ӧ���Ƿ�μ��Ż�����ΪlBufLen
	char* pIsValid;

	TExchOptimize();
	~TExchOptimize();

	TExchOptimize(const TExchOptimize& sData);
	TExchOptimize& operator=(const TExchOptimize& sData);
	TExchOptimizeItem* operator[](int iIdx);
	BOOL IsValid();
	BOOL IsValidItem(int nIdx);
	BOOL New(int lLen);
	void Free();
	void Serialize(CArchive& ar);
	// �õ���Ч�μӼ���Ĳ�����
	int  GetValidItemCount(); 
	// �õ��ܹ�Ҫ����Ĵ���
	long GetCaclUionCount();
	// �õ���N���������
	BOOL GetCaclUionItem(long nIdx, TExchParam& sRet);
	// �õ���N����Ч�Ĳ���
	TExchOptimizeItem* GetValidItem(long nIdx);
	// �õ���N����Ч������ȫ������
	int ValidIdxToGlobalIdx(long nIdx);
	// ת����CMapVariabile
	void ConvertToMapNumVal(CMapVariabile& sMapVal);
	// ��CMapVariabileת��
	BOOL ConvertFromMapNumVal(CMapVariabile* sMapVal, BOOL bValid = FALSE);
	// �ӹ�ʽ����ת��
	BOOL ConvertFromExpress(CExpression* pExpress, BOOL bValid = FALSE);
};

// ��Ʊ����(����ԭ�����Ǹ���vpt,�����������
struct CodeInfoEx
{
	HSMarketDataType	m_cCodeType;//֤ȯ����
	char				m_cCode[6]; //֤ȯ����
	CodeInfoEx& operator=(CodeInfo& sCode)
	{
		memcpy(this, &sCode, sizeof(CodeInfoEx));
		return *this;
	}
};

/*************************************************************
*	�� �� �ƣ�	TCaclOneStockResult
*	�� �� ����	һ֧��Ʊ���
***************************************************************/
struct TCaclOneStockResult
{
	union
	{
		LONGLONG	m_llData;
		CodeInfoEx	m_sCode;					// ��Ʊ����(��֧��Ʊʱʹ��)
		long		m_lStockNum;				// ��Ʊ֧��(�ۺ�ͳ��ʱʹ��)
	};
	double		m_dNetProfit;					// ������
	double		m_dNPPercent;					// ��������NP=Net Profit
	double		m_dTotalProfit;					// ��ӯ��
	double		m_dTotalDificit;				// �ܿ���
	long		m_lDealNum;						// ���״���
	double		m_dWinPercent;					// ʤ��
	double		m_lYearDealNum;					// ������״���
	long		m_lProfitDealNum;				// ӯ�����״���
	long		m_lDificitDealNum;				// �����״���
	double		m_dMaxSingleProfit;				// ��󵥴�ӯ��
	double		m_dMaxSingleDificit;			// ��󵥴ο���

	long		m_lMaxContinuProfitNum;			// �������ӯ������
	long		m_lMaxContinuDificitNum;		// �������������� 
	double		m_dDealAverageCyc;				// ����ƽ��������
	double		m_dProfiltAverageCyc;			// ӯ��ƽ����������
	double		m_dDiticitAverageCyc;			// ����ƽ����������

	double		m_dMaxFloatProfit;				// ��󸡶�ӯ��
	double		m_dMaxFloatDificit;				// ��󸡶�����

	double		m_dAllJumpIn;					// ��Ͷ��
	double		m_dValidJumpIn;					// ��ЧͶ��
	double		m_dYearReturnPercent;			// ��ر���
	double		m_dYearValidReturnPercent;		// ����Ч�ر���
	double		m_dSimpleBuyHaveReturnPercent;	// ��������лر�
	double		m_dBuyHaveYearPercent;			// ���������ر���
	double		m_dAllDealAmount;				// �ܽ��׶�
	double		m_dAllDealCost;					// �ܽ��׷�
	long		m_lBeginDate;
	long		m_lEndDate;						// ����ʱ��
	long		m_lTestCycNum;					// ����������
	double		m_dAverageCW;					// ƽ����λ
	double		m_dMaxCW;						// ����λ
	double		m_dAverageCCL;					// ƽ���ֲ���
	double		m_dMaxCCL;						// ���ֲ���
	double		m_dSucessPercent;				// �ɹ���
	double		m_dSignalNum;					// �ź�����
	double		m_d5DayProfitProbability;		// ���ջ�������
	double		m_d10DayProfitProbability;		// ʮ��������
	double		m_d20DayProfitProbability;		// ��ʮ��������
	double		m_dAimCycProfitProbability;		// Ŀ�����ڻ�������

	TCaclOneStockResult();

	TCaclOneStockResult& operator=(TCaclOneStockResult& sData);
	TCaclOneStockResult& operator+=(TCaclOneStockResult& sData);
	void ToListCtrl(CListCtrl *pCtrl);
	void Serialize(CArchive& ar);
};			

// ���ɽ���ϸ
struct TDealFlowItem
{
	WORD		m_wDealType;	// ��������
	unsigned long		m_lDealDate;	// ����ʱ��
	double		m_dDealPrice;	// �ɽ���
	LONG		m_lDealAmount;	// �ɽ���

	double		m_dDealProfit;	// ����(������ʱ��Ч,������ʾ����,������ʾ����)

	// ��������Ϊ����ʱ������Ч
	LONG		m_lCaclDays;	// �ɽ��յ�Ŀǰʱ��(���ֵΪ�ɽ�����=����ʱ��-����ʱ��),��������ƽ��,����ƽ��,����ƽ����

	TDealFlowItem();
	TDealFlowItem& operator++();
	TDealFlowItem& operator=(TDealFlowItem& sData);
	CString GetDealType();
	void ToListCtrl(CListCtrl* pCtrl, CString& strName);
	void Serialize(CArchive& ar);
};

// һ֧��Ʊ�ɽ���ϸ
struct TDealFlowStock
{
	static long		m_lLastBuy;		// ��ǰ���һ����Ч������
	static double	m_dAllMoney;	// ��ʼ�ʽ�
	static BOOL		m_bLastSell;	// �����ƽ���Ƿ����ͳ��
	static double	s_dLastMonery;	// ȫ�г�����ģ��ʱ��

	CodeInfo	m_sCode;		// ����
	CString		m_strName;		// ����
	long		m_lLastStockNum; // ���һ�ʽ��׺���ʣ�µĹ�Ʊ
	double		m_lLastMoney;	// ���һ�ʽ��׺���ʣ�½�Ǯ
								// ���Ƿ����0,���п�Ͷ����(����Ͷ��ʱʹ��)
								// ���û��,��ֻҪ�����Ƿ��к�������������ƽ��
								// �����ͬʱ�����Ƿ��п�����ʱ��
	CArray<TDealFlowItem*, TDealFlowItem*> m_ayItem;	// �ڱ���ϸ

	double	m_dBeginPrice;		// ��ʼʱ�۸�
	double	m_dEndPrice;		// ����ʱ�۸�	
	long	m_lTestCycNum;		// ��������

	long	m_lBuySignalNum;	// �����ź���
	long	m_lWinDays;			// ����ɹ�����
	long	m_lWin5Day;			// ����ʤ��
	long	m_lWin10Day;		// ʮ��ʤ��
	long	m_lWin20Day;		// ��ʮ��ʤ��
	long	m_lWinTabCyc;		// Ŀ������ʤ��

	double		m_dJumpInMoney; // Ͷ�뵥�ν��׽��
	long		m_lSignalNum;	// �ź�����
	double		m_dMaxFDProfit; // ��󸡶�ӯ��
	double		m_dMaxFDDificit; // ��󸡶�����

	TDealFlowStock();
	TDealFlowStock(TDealFlowStock* pData);
	TDealFlowStock(CodeInfo& sCode, CString& strName, double dLastMoney);

	~TDealFlowStock();

	/******************************************************************
	*	�������ܣ�	���ݳɽ���ϸ,����ͳ��,�����������pItem, ���pItemΪ��,��Newһ��
	*	����������	TCaclOneStockResult* pItem = NULL : [in] ͳ�ƽ�����
	*				BOOL bSingle = TRUE				  : [in] TRUE: ����Ʊ, FALSE : ȫ�г� 
	*	�� �� ֵ��	TCaclOneStockResult* : ͳ�ƽ��
	*	����������	
	*	��    �ߣ�	
	*	������ڣ�	2003��4��18��
	*	�޸�˵����	
	*****************************************************************/
	TCaclOneStockResult* TotalTo(TCaclOneStockResult* pItem = NULL, BOOL bSingle = TRUE);
	// �õ��ɽ�����
	long GetCount();
	// ��һ���ɽ���ϸ�����,���pItemΪNULL,��Newһ��ռ�,Ȼ�󷵻�
	TDealFlowItem* Append(TDealFlowItem* pItem = NULL);
	// �õ�����һ���ɽ���ϸ
	TDealFlowItem* GetLastItem();
	// �õ���iIdx�ĳɽ���ϸ
	TDealFlowItem* GetItem(int iIdx);
	// �õ����һ�ʳɽ��ļ۸�
	double GetLastPrice();
	// �õ����һ�ʳɽ�������������
	LONG GetLastCaclDays();
	// �Ƿ���ʣ���Ǯ
	BOOL IsHaveMoney();
	// �Ƿ���ʣ��Ĺ�Ʊ
	BOOL IsHaveStock();
	// ��ճɽ���ϸռ�ÿռ�
	void Free(void);
	TDealFlowStock& operator=(TDealFlowStock& sData);
	// ���ɽ���ϸ��pCtrl����ʾ����
	void ToListCtrl(CListCtrl *pCtrl);
	// �������
	void Clear();
	void Serialize(CArchive& ar);
};

// �ɽ���ϸ
#define HS_EXCH_DEALFLOW_HANDLE_TIME		"�ɽ�����"
#define HS_EXCH_DEALFLOW_HANDLE_STK		"��Ʊ����"
#define HS_EXCH_DEALFLOW_HANDLE_TYPE		"��������"
#define HS_EXCH_DEALFLOW_HANDLE_PRICE		"�ɽ���"
#define HS_EXCH_DEALFLOW_HANDLE_AMOUNT		"�ɽ���"
#define HS_EXCH_DEALFLOW_HANDLE_PROFIT		"����(Ԫ)"
struct TDealFlow
{
	CArray<TDealFlowStock*, TDealFlowStock*> m_ayData; // ���й�Ʊ�ĳɽ���ϸ(�μӼ���)

	~TDealFlow();

	// �õ����һ֧��Ʊ�ĳɽ���ϸ
	TDealFlowStock* GetLastFlow();
	// �õ���iIdx֧��Ʊ�ĳɽ���ϸ
	TDealFlowStock* GetItem(int iIdx);
	// �õ���ƱpCode�ĳɽ���ϸ
	TDealFlowStock* GetFlowFormCode(CodeInfo* pCode);
	// ��һ֧��Ʊ�ĳɽ���ϸ
	void AddStockItem(TDealFlowStock* pItem);
	// �õ���Ʊ��
	int GetCount();
	// ���
	void Free(void);
	TDealFlow& operator=(TDealFlow& sData);
	// ����nIdx��Ʊ��������ʾ��pCtrl��
	void ToListCtrl(CListCtrl *pCtrl, int nIdx);
	// ��ʼ��pCtrl,ɾ��ԭ������,����IMAGE,������
	void Initital(CListCtrl* pCtrl, CImageList *pImage);
	void Serialize(CArchive& ar);
};

// ������
#define HS_EXCH_CACLRESULT_STOCK_HEADLE_STKNAME	"��Ʊ��"
#define HS_EXCH_CACLRESULT_STOCK_HEADLE_STKCODE	"��Ʊ����"
#define HS_EXCH_CACLRESULT_STOCK_HEADLE_PARAM		"����"
#define HS_EXCH_CACLRESULT_STOCK_HEADLE_PROFIT		"������"
#define HS_EXCH_CACLRESULT_STOCK_HEADLE_YEAR		"��ر�"
#define HS_EXCH_CACLRESULT_STOCK_HEADLE_WIN		"ʤ��"
#define HS_EXCH_CACLRESULT_STOCK_HEADLE_DEALNUM	"������"
#define HS_EXCH_CACLRESULT_STOCK_HEADLE_SUCC		"�ɹ���"

#define HS_EXCH_CACLRESULT_STOCK_ID_STKNAME	0
#define HS_EXCH_CACLRESULT_STOCK_ID_STKCODE	0
#define HS_EXCH_CACLRESULT_STOCK_ID_PARAM		0
#define HS_EXCH_CACLRESULT_STOCK_ID_PROFIT		1
#define HS_EXCH_CACLRESULT_STOCK_ID_YEAR		2
#define HS_EXCH_CACLRESULT_STOCK_ID_WIN		3
#define HS_EXCH_CACLRESULT_STOCK_ID_DEALNUM	4
#define HS_EXCH_CACLRESULT_STOCK_ID_SUCC		5

/*************************************************************
*	�� �� �ƣ�	TCaclResult
*	�� �� ����	������
***************************************************************/
struct TCaclResult
{
	TCaclOneStockResult	m_sTotal;	// ���й�Ʊ�Ľ��ͳ��
	CArray<TCaclOneStockResult*, TCaclOneStockResult*> m_ayAllStock; // ��֧��Ʊ�Ľ��ͳ��

	TCaclResult();
	~TCaclResult();
	// ��һ����Ʊ��ͳ��
	void Add(TCaclOneStockResult* pResult);
	// ͳ������(m_ayAllStock��m_sTotal)
	BOOL Total(BOOL bSingle = TRUE);
	// ���ռ�ÿռ�
	void Free(void);
	TCaclResult& operator=(TCaclResult& sData);
	// ��ͳ�ƽ����ʾ��pCtrl
	void ToListCtrl(CListCtrl* pCtrl);
	// ��ʼ��pCtrl,ɾ��ԭ������,����IMAGE,������
	void Initital(CListCtrl* pCtrl, CImageList *pImage = NULL);
	void Serialize(CArchive& ar);

	// ����ID�õ��ܵ�ͳ�ƽ���е�����
	double GetIDData(int nID);
};

/*************************************************************
*	�� �� �ƣ�	TCaclResultTemp
*	�� �� ����	ָ��������������м����ʹ��
***************************************************************/
struct TCaclResultTemp
{
	long	m_lDaysLen;				// ��������ǵ�������
	long	m_lBufLen;				// ��BUF����,Ӧ����ͬ

	long	m_lDescLen;				// Դ����������
	HSDouble *m_lDescReqult;		// Դ������

	long	m_lLeftLen;				// ��һ���Ƚ�ָ��������
	HSDouble *m_lLeftReqult;		// ��һ���Ƚ�ָ�������

	long	m_lRithtLen;			// �ڶ����Ƚ�ָ����Ч����
	HSDouble *m_lRightReqult;		// �ڶ����Ƚ�ָ�������



	TCaclResultTemp(TCaclResultTemp *pData = NULL);
	~TCaclResultTemp();

	BOOL Copy(TCaclResultTemp *pData);
	BOOL IsValid();
	void New(long lSize);
	void Del();
	enum { ToDesc = 0, ToLeft, ToRight, ToOther };
	BOOL Copy(HSDouble* pData, long lCount, int nMask = ToDesc);
};

// ��������̵Ĳ����ṹ
struct TCaclTemp
{
	CodeInfo	*pCode;				// ��Ʊ����
	CExpression	*pPress;			// ��ʽ��
	CString		szTechMain;			// ��ָ��
	CString		szTechSideLeft;			// �Ƚ�ָָ��(����)
	CString		szTechSideRitht;			// �Ƚ�ָָ��(����)
	long		lEndDate;			// ��������
	long		lBeginDate;			// ��ʼ����
	long		lCountDate;			// ������
	long		lBuffDiskData;		// pRetDiskData����
	long		nChengJiaoDiv;		// �ɽ�����λ
	long		nAnaPeriod;			// ���ڵ�λ

	StockDay	*pRetDiskData;		// ���ص��������ݿռ�,Щ�ռ䲻��ɾ��
	TCaclResultTemp	*pRetCacl;		// ���ص��������ָ��ļ�����

	BOOL		bExRights;			// �Ƿ��Ȩ���� 

	TCaclTemp();
	TCaclTemp(TCaclTemp& sTemp);
	~TCaclTemp();

	void New(int iDiskDataLen = 0);
	void Free(BOOL bAll = FALSE);
	void ClearData();
};

// ���һ���������õļ�����
struct TExchCaclResult
{
	// ��ǰ��ʽ�Ĳ���
	TExchParam		m_sCurParam;
	// ������(ͳ��m_sDealFlow����)
	TCaclResult		m_sCaclResult;
	// �ɽ���ϸ
	TDealFlow		m_sDealFlow;

	TExchParam*  GetCurParam()	{ return &m_sCurParam; }
	TCaclResult* GetResult()	{ return &m_sCaclResult; }
	TDealFlow*   GetDealFlow()	{ return &m_sDealFlow; }
	// �õ���ӦID������
	double GetIDData(int nID);
	TExchCaclResult& operator=(TExchCaclResult& sData)
	{
		m_sCurParam = sData.m_sCurParam;
		m_sCaclResult = sData.m_sCaclResult;
		m_sDealFlow = sData.m_sDealFlow;
		return *this;
	}
	// ��ʾ���б�
	void ToListCtrl(CListCtrl* pCtrl);
};

#endif