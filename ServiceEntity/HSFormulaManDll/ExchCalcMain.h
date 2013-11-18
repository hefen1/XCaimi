/*************************************************************
*	Copyright (c)2010, �������ӹɷ����޹�˾
*	All rights reserved.
*
*	�ļ����ƣ�	ExchCalcMain.h
*	�ļ���ʶ��	
*	ժ	  Ҫ��	����ϵͳ���Լ������ඨ��
*
*	��ǰ�汾��	1.0
*	��	  �ߣ�	
*	�������ڣ�	
*
*	�޸�˵����	
*
***************************************************************/
#include "PubStruct.h"
#include "ExchData.h"

#pragma once

#define Sell_Mode_
#include <afxcoll.h>

struct CThreadStatusMutex
{
	CMutex m_mutex;
	long   m_lStatus;

	CThreadStatusMutex();
	BOOL SetVal(long lVal); // ����ֵ
	BOOL IsRun(); // ������
	BOOL IsStopping(); // ֹͣ
	CThreadStatusMutex& operator=(long lVal);
	BOOL operator==(long lVal);
	BOOL operator!=(long lVal);
};

/*************************************************************
*	�� �� �ƣ�	CExchCalcMain
*	�� �� ����	����ϵͳ�����������(������ѡ������Ļ���)
***************************************************************/
class CExchCalcMain: public TExchSys
{
public:
	CExchCalcMain();
	~CExchCalcMain();

protected:
	// ֪ͨ��Ϣ����(����,��ϢID��֪ͨ���)
	TExchMsg	m_sMsg;

public:
	// ������Ϣ���ھ��
	void SetMsgWnd(HWND hWnd) { m_sMsg.m_hNotifyWnd = hWnd; }
	// ������Ϣ���ھ��
	void SetMsgWnd(CWnd* pWnd) { m_sMsg.m_hNotifyWnd = pWnd->GetSafeHwnd(); }
	// ������ϢID
	void SetMsg(UINT hMsg) { m_sMsg.m_hNotifyMsg = hMsg; }
	// ������Ϣ����
	void SetMsg(TExchMsg* pMsg){ if( pMsg == NULL ) return; memcpy(&m_sMsg, pMsg, sizeof(TExchMsg)); }
	// �õ���Ϣ���ھ��
	HWND GetMsgWnd() { return m_sMsg.m_hNotifyWnd; }
	// �õ���ϢID
	UINT GetMsgHead() { return m_sMsg.m_hNotifyMsg; }
	// �õ�֪ͨ���(�Ƿ���)
	BOOL IsNotify() { return m_sMsg.m_bNotify; }
	// ����Ϣ���ڷ�����Ϣ
	static LRESULT	SendMsg(WPARAM wp, LPARAM lp);

public:
	// �Ƿ���������(Ϊ1,��ʾ������������,��ʱ���ٱ���������ݵĸĶ�, ������Ҫ����)
	char	m_cIsApplySeting;
    // ����m_cIsApplySeting״̬
	void SetApplySeting(BOOL bSave);
	// �õ�m_cIsApplySeting״̬
	BOOL GetApplySeting();

public:
	// �Ӽ����ڴ��е�����
	// �õ���ǰ���⹫ʽָ��
	CExpression* GetExp(){return m_sExchExp.m_pMainTech;}
	// �õ���װ�˵�ǰ���⹫ʽ�Ķ���
	TExchExp* GetExchExp() { return &m_sExchExp; }
	// �õ������г�ģ������
	TExchModel* GetMode(){return &m_sTExchModel;}
	// �õ����뷽����������
	TExchBuy* GetBuy(){return &m_sTExchBuy;}
	// �õ�����������������
	TExchSell* GetSell() {return &m_sTExchSell;}
	// �õ�������ѡ��������
	TExchOptimize* GetOptimize() { return &m_sTExchOptimize; }
	
	/******************************************************************
	*	�������ܣ�	�õ���Ӧ��ͳ�ƽ������
	*	����������	UINT uParamID = -1 : [in] ���Ϊ-1,�򷵻����һ������������,
											  ����,���ؼ�������,����ID��ΪuParamID�ļ���������
	*	�� �� ֵ��	TCaclResult* : ���ص�ͳ�ƽ������, �Ҳ�����ΪNULL
	*	����������	
	*	��    �ߣ�	
	*	������ڣ�	2003��4��22��
	*	�޸�˵����	
	*****************************************************************/
	TCaclResult* GetOneResult(UINT uParamID = -1);

	/******************************************************************
	*	�������ܣ�	�õ���Ӧ�ĳɽ���ϸ����
	*	����������	UINT uParamID = -1 : [in] ���Ϊ-1,�򷵻����һ���ɽ���ϸ����,
											  ����,���ؼ�������,����ID��ΪuParamID�ĳɽ���ϸ����
	*	�� �� ֵ��	TDealFlow* : ���صĳɽ���ϸ����,�Ҳ�����ΪNULL
	*	����������	
	*	��    �ߣ�	
	*	������ڣ�	2003��4��22��
	*	�޸�˵����	
	*****************************************************************/
	TDealFlow* GetDealFlow(UINT uParamID = -1);

	/******************************************************************
	*	�������ܣ�	�õ���Ӧ�Ĳ�����������
	*	����������	UINT uParamID = -1 : [in] ���Ϊ-1,�򷵻����һ��������������,
											  ����,���ؼ�������,����ID��ΪuParamID�Ĳ�����������
	*	�� �� ֵ��	TExchParam*  : ���صĲ�����������,�Ҳ�����ΪNULL
	*	����������	
	*	��    �ߣ�	
	*	������ڣ�	2003��4��22��
	*	�޸�˵����	
	*****************************************************************/
	TExchParam*  GetCurParam(UINT uParamID = -1);

	/******************************************************************
	*	�������ܣ�	�õ�����IDΪuParamID�ļ�������m_sCaclResult�����е�λ��
	*	����������	UINT uParamID : [in] ���Ϊ-1 ����m_sCaclResult���һ��������
										 ����, ���ؼ�����������,����ID��ΪuParamID�ļ�������m_sCaclResult������
	*	�� �� ֵ��	int : ���ص�����(>=0), �Ҳ���(-1)
	*	����������	
	*	��    �ߣ�	
	*	������ڣ�	2003��4��22��
	*	�޸�˵����	
	*****************************************************************/
	int	GetCaclResult(UINT uParamID);
	// �õ�����������(m_sCaclResult)�ĳ���
	int GetCaclResultCount();

	/******************************************************************
	*	�������ܣ�	�õ���Ӧ�ļ���������(�����˳ɽ���ϸ,ͳ�ƽ��,��������)
	*	����������	UINT uParamID = -1 : [in] ���Ϊ-1,�򷵻����һ������������,
											  ����,���ؼ�����������,����ID��ΪuParamID�ļ���������
	*	�� �� ֵ��	TExchCaclResult* : ���صļ���������
	*	����������	
	*	��    �ߣ�	
	*	������ڣ�	2003��4��22��
	*	�޸�˵����	
	*****************************************************************/
	TExchCaclResult* GetCaclResultItem(UINT uParamID = -1);

	/******************************************************************
	*	�������ܣ�	�õ�ͳ�ƽ����,������uObjID�����ֵ�ļ�����, ������ѡ��ʹ��
	*	����������	UINT uObjID = HS_EXCH_CACLRESULT_STOCK_ID_PROFIT : [in] Ҫ�Ƚϵ�ͳ��������
	*	�� �� ֵ��	TExchCaclResult* : ������������������nObjID��ֵ���ļ�����
	*	����������	
	*	��    �ߣ�	
	*	������ڣ�	2003��4��22��
	*	�޸�˵����	??? �Ƚ����ֵ����,Ӧ�����ڵ�����,���忪��ʱ�ٸ�(HS)
	*****************************************************************/
	TExchCaclResult* GetObjResultItem(UINT uObjID = HS_EXCH_CACLRESULT_STOCK_ID_PROFIT);

	// �Ӳ����г�ģ�������еõ���Ʊ�б�
	CArray<StockUserInfo*, StockUserInfo*>* GetStockList()
	{
		TExchModel* pModel = GetMode();
		return &pModel->m_ayStkList;
	}
	
	/******************************************************************
	*	�������ܣ�	���һ֧�����Ʊ�Ƿ���ʣ����
	*	����������	���ޣ�
	*	�� �� ֵ��	BOOL : ��(TRUE), ��(FALSE)
	*	����������	GetOverplusMoney() �õ�������ɽ��
	*	��    �ߣ�	
	*	������ڣ�	2003��4��22��
	*	�޸�˵����	
	*****************************************************************/
	BOOL IsHaveMoney() { return (GetOverplusMoney() > 0);}

	/******************************************************************
	*	�������ܣ�	�õ���Ӧ��Ʊ��������һ֧��Ʊʣ����
	*	����������	CodeInfo* pCode = NULL : [in] ��Ʊ����, ΪNULLʱ��ʾ���һ֧
	*	�� �� ֵ��	double : ʣ��������
	*	����������	
	*	��    �ߣ�	
	*	������ڣ�	2003��4��22��
	*	�޸�˵����	
	*****************************************************************/
	double GetOverplusMoney(CodeInfo* pCode = NULL);

	/******************************************************************
	*	�������ܣ�	�õ���Ӧ��Ʊ��������һ֧��Ʊʣ���Ʊ��-
	*	����������	CodeInfo* pCode = NULL : [in] ��Ʊ����, ΪNULLʱ��ʾ���һ֧
	*	�� �� ֵ��	long : ʣ���Ʊ��-
	*	����������	
	*	��    �ߣ�	
	*	������ڣ�	2003��4��22��
	*	�޸�˵����	
	*****************************************************************/
	long GetLastStockNum(CodeInfo* pCode = NULL);
	// �õ����һ�����������Ŀǰ��������-
	long GetLastCyc(CodeInfo* pCode = NULL);
	// �õ����һ������ĳɽ���
	double GetLastPrice(CodeInfo* pCode = NULL);
	// �õ����һ֧��Ʊ�ĳ�Ч��ϸ
	TDealFlowStock* GetLastFlow(CodeInfo* pCode = NULL);
	// �õ�ָ����Ʊ����ɽ���ϸ
	TDealFlowStock* GetFlowFormCode(CodeInfo *pCode);

	// �ӽ��汣�����ݵ������ڴ���-
	// ����:����ɹ�(TRUE), ����ʧ��(FALSE)
	BOOL SetData(TExchHead* pData);
	// ���湫ʽ
	BOOL SetExp(TExchHead* pData);
	// ���湫ʽ
	BOOL SetExpress(CExpression* pPress);
	// �����г�ģ��
	BOOL SetMode(TExchHead* pData);
	// ������������
	BOOL SetBuy(TExchHead* pData);
	// ������������
	BOOL SetSell(TExchHead* pData);
	// ��������Ż�ѡ������
	BOOL SetOptimize(TExchHead* pData);

	/******************************************************************
	*	�������ܣ�	����ʽpPress�еĲ����б��ʼ������������ѡ��������
	*	����������	CExpression* pPress : [in] ��ʼ���Ĺ�ʽ
	*				BOOL bValid = FALSE : [in] ����ѡ��Ĭ���Ƿ���Ч
	*	�� �� ֵ��	BOOL : ����ɹ�(TRUE), ʧ��(FALSE)
	*	����������	
	*	��    �ߣ�	
	*	������ڣ�	2003��4��22��
	*	�޸�˵����	
	*****************************************************************/
	BOOL SetOptimize(CExpression* pPress, BOOL bValid = FALSE);

	// ���ƹ�Ʊ�б������г�ģ��������
	void CopyStock(CArray<StockUserInfo*,StockUserInfo*>& sStock);
	// ���б��еõ���nIdx֧��Ʊ
	StockUserInfo* GetStockAt(int nIdx);
	// �õ���Ʊ��
	long GetStockNum();	

	/******************************************************************
	*	�������ܣ�	��һ����������������������,���������������ݿռ��ָ��(��ѡʱ��)
	*	����������	int nIdx : [in] Ԥ������ļ�������Ӧ�Ĳ��������еĲ���ID
	*				TExchParam *pPrvParam = NULL : [in] ǰһ���������Ĳ�������
	*				int lDecimal = 0 : [in] С��λ
	*	�� �� ֵ��	TExchCaclResult* : �������ɹ�,�򷵻ؼ�������ݿռ�ָ��,����ΪNULL
	*	����������	
	*	��    �ߣ�	
	*	������ڣ�	2003��4��24��
	*	�޸�˵����	
	*****************************************************************/
	TExchCaclResult* GetNewCaclRetultItem(int nIdx, TExchParam *pPrvParam = NULL, int lDecimal = 0);

	/******************************************************************
	*	�������ܣ�	����һ�����������������(m_sCaclResult)��
	*	����������	TExchCaclResult* pItem : [in] Ԥ����ļ���������
	*	�� �� ֵ��	BOOL : ����ɹ�(TRUE), ʧ��(FALSE)
	*	����������	
	*	��    �ߣ�	
	*	������ڣ�	2003��4��24��
	*	�޸�˵����	
	*****************************************************************/
	BOOL AddCaclResultItem(TExchCaclResult* pItem);

	// С��
	int m_nDecimal;
	// ��λ
	long m_nPriceUnit;

	// �����߳����
public:
	// �߳�״̬TCaclThread_State_Stopingֹͣ, TCaclTherad_State_Start������-
	static CThreadStatusMutex s_uTheradState;
	// ָ��ǰ��������(ָ���Լ�,һ�������)
	static CExchCalcMain* s_pThis;

	/******************************************************************
	*	�������ܣ�	�����߳�������
	*	����������	LPVOID pData : [in] ΪNULL���ǵ��ñ�������CExchCalcMain����ָ��
	*	�� �� ֵ��	long : ���㷵����Ϣ
								TCaclReturn_NoData		û����
								TCaclReturn_VainParam	��������Ч
								TCaclReturn_NoExperss	û�й�ʽ��
								TCaclReturn_CalFail		����ʧ��
								TCaclReturn_MoneryFail	�ڴ����ʧ��
								TCaclReturn_TotalFail	ͳ��ʧ��
								TCaclReturn_Stop		�û�ֹͣ
								TCaclReturn_Succ		�ɹ� 
								TCaclReturn_Fail		δ֪��ʧ��
	*	����������	SingleStockTest(), EntireMarketTest()
	*	��    �ߣ�	
	*	������ڣ�	2003��4��24��
	*	�޸�˵����	
	*****************************************************************/
	static long WINAPI CaclMain(LPVOID pData);

	/******************************************************************
	*	�������ܣ�	����Ʊ�г�ģ�Ͳ���
	*	����������	int iCurParamIdx : [in] ��ǰ������ȫ�ּ�������е�������(���������,��ʹ�ò�����ѡʱΪ0)
	*				int iCount		 : [in] �ܹ�Ҫ����Ĵ���(��ʹ�ò�����ѡʱΪ1)
	*	�� �� ֵ��	long : ���㷵����Ϣ
								TCaclReturn_NoData		û����
								TCaclReturn_VainParam	��������Ч
								TCaclReturn_NoExperss	û�й�ʽ��
								TCaclReturn_CalFail		����ʧ��
								TCaclReturn_MoneryFail	�ڴ����ʧ��
								TCaclReturn_TotalFail	ͳ��ʧ��
								TCaclReturn_Stop		�û�ֹͣ
								TCaclReturn_Succ		�ɹ� 
								TCaclReturn_Fail		δ֪��ʧ��
	*	����������	
	*	��    �ߣ�	
	*	������ڣ�	2003��4��24��
	*	�޸�˵����	
	*****************************************************************/
	static long SingleStockTest(int iCurParamIdx, int iCount);
	
	/******************************************************************
	*	�������ܣ�	ȫ�г�ģ�Ͳ��� ??? ���ٸĽ�,�㷨�л����в��׵�֮��HS
	*	����������	int iCurParamIdx : [in] ��ǰ������ȫ�ּ�������е�������(���������,��ʹ�ò�����ѡʱΪ0)
	*				int iCount		 : [in] �ܹ�Ҫ����Ĵ���(��ʹ�ò�����ѡʱΪ1)
	*	�� �� ֵ��	long : ���㷵����Ϣ
								TCaclReturn_NoData		û����
								TCaclReturn_VainParam	��������Ч
								TCaclReturn_NoExperss	û�й�ʽ��
								TCaclReturn_CalFail		����ʧ��
								TCaclReturn_MoneryFail	�ڴ����ʧ��
								TCaclReturn_TotalFail	ͳ��ʧ��
								TCaclReturn_Stop		�û�ֹͣ
								TCaclReturn_Succ		�ɹ� 
								TCaclReturn_Fail		δ֪��ʧ��
	*	����������	
	*	��    �ߣ�	
	*	������ڣ�	2003��4��24��
	*	�޸�˵����	
	*****************************************************************/
	static long EntireMarketTest(int iCurParamIdx, int iCount);

	/******************************************************************
	*	�������ܣ�	����ֹͣ����
	*	����������	���ޣ�
	*	�� �� ֵ��	long : TCaclReturn_Stop �û�ֹͣ
	*	����������	
	*	��    �ߣ�	
	*	������ڣ�	2003��4��24��
	*	�޸�˵����	
	*****************************************************************/
	static long CaclStop();

	/******************************************************************
	*	�������ܣ�	ʧ�ܴ���,����Ϣ���ڷ���Ϣ,��������֪ͨ��Ŀ�괰��,��������Ӧ����
	*	����������	UINT msg = TCaclReturn_Fail : [in] ������ID
														TCaclReturn_NoData		û����
														TCaclReturn_VainParam	��������Ч
														TCaclReturn_NoExperss	û�й�ʽ��
														TCaclReturn_CalFail		����ʧ��
														TCaclReturn_MoneryFail	�ڴ����ʧ��
														TCaclReturn_TotalFail	ͳ��ʧ��
														TCaclReturn_Stop		�û�ֹͣ
														TCaclReturn_Succ		�ɹ� 
														TCaclReturn_Fail		δ֪��ʧ��
	*	�� �� ֵ��	long : �����msg����
	*	����������	
	*	��    �ߣ�	
	*	������ڣ�	2003��4��24��
	*	�޸�˵����	
	*****************************************************************/
	static long Fail(UINT msg = TCaclReturn_Fail);

	/******************************************************************
	*	�������ܣ�	����ʧ�ܴ���,���ɽ���ϸ�����м�һ���յ���ϸ
	*	����������	CodeInfo* pCode : [in] Ҫ����Ĺ�Ʊ
	*	�� �� ֵ��	long : �ɹ�(TCaclReturn_Succ), ʧ��(TCaclReturn_Fail)
	*	����������	
	*	��    �ߣ�	
	*	������ڣ�	2003��4��24��
	*	�޸�˵����	
	*****************************************************************/
	static long CaclFail(CodeInfo* pCode);
	// -

	/******************************************************************
	*	�������ܣ�	����һ֧��Ʊ����ʽָ��(Ҳ������)�ļ�����
	*	����������	TCaclTemp& sCaclTemp : [in/out] ����������õ������ݲ���ż�����
	*	�� �� ֵ��	long : ����ɹ�(TCaclReturn_Succ), ����ʧ��
	*	����������	����Ϣ��������Ϣ��childframe,��Ȼ���ù�ʽ����õ�����
	*	��    �ߣ�	
	*	������ڣ�	2003��4��24��
	*	�޸�˵����	
	*****************************************************************/
	static long CaclValues(TCaclTemp& sCaclTemp);

	/******************************************************************
	*	�������ܣ�	str�Ƿ�Ϊָ����(��һ���Ƿ�Ϊ����,Ϊ��������ָ����)
	*	����������	CString& str : [in] Ҫ�жϵ��ִ�
	*	�� �� ֵ��	BOOL : ��ָ����(TRUE), ����(FALSE)
	*	����������	
	*	��    �ߣ�	
	*	������ڣ�	2003��4��24��
	*	�޸�˵����	
	*****************************************************************/
	static BOOL IsTechLine(CString& str);

	/******************************************************************
	*	�������ܣ�	��pRet�еõ�����
	*	����������	TCaclResultTemp* pRet : [in] ָ�����������
	*				FormulaCompareSubData* pSub : [in] �����ȽϽṹ����
	*				int nTechNum : [in] �ڼ�������ָ��
										0 ��pRet->m_lLeftReqult����pSub->m_dData1������
										1 ��pRet->m_lRightReqult����pSub->m_dData2������
	*				int iIdx : [in] ��pRet->m_lLeftReqult��pRet->m_lRightRequltȡ����ʱ������
	*	�� �� ֵ��	double : ȡ�õ�����, ����Ϊ0
	*	����������	
	*	��    �ߣ�	
	*	������ڣ�	2003��4��24��
	*	�޸�˵����	
	*****************************************************************/
	static double GetTechData(TCaclResultTemp* pRet, FormulaCompareSubData* pSub, int nTechNum, int iIdx);

	/******************************************************************
	*	�������ܣ�	Ԥ����(�ɽ���ϸ��,�ɽ����ڴ���,��󸡶�ӯ������󸡶�����Ĵ���)
	*	����������	CodeInfo* pCode : [in] Ҫ����Ĵ���
	*				StockDay* pDay  : [in] ��������
	*	�� �� ֵ��	(��) 
	*	����������	
	*	��    �ߣ�	
	*	������ڣ�	2003��4��24��
	*	�޸�˵����	
	*****************************************************************/
	static void PreDeal(CodeInfo* pCode, StockDay* pDay);

	/******************************************************************
	*	�������ܣ�	iDay�������Ƿ�������������
	*	����������	TCaclTemp* pData : [in] ָ�������
	*				int iDay		 : [in] ������
	*	�� �� ֵ��	BOOL : ������(TRUE), ��������(FALSE)
	*	����������	
	*	��    �ߣ�	
	*	������ڣ�	2003��4��24��
	*	�޸�˵����	
	*****************************************************************/
	static BOOL IsBuy(TCaclTemp* pData, int iDay);

	/******************************************************************
	*	�������ܣ�	�ź�����ͳ��,���Ҽ������ʤ�ʵ�����(���ͳ��ʤ��ʱ��)
	*	����������	CodeInfo* pCode : [in] Ҫ����Ĺ�Ʊ
	*				StockDay* pData : [in] ��������
	*				int iDay		: [in] Ҫ������������
	*				int iDayLen		: [in] ��������pData�ĳ���
	*	�� �� ֵ��	BOOL : �ɹ�(TRUE), ʧ��(FALSE)
	*	����������	
	*	��    �ߣ�	
	*	������ڣ�	2003��4��24��
	*	�޸�˵����	
	*****************************************************************/
	static BOOL PreBuyStock(CodeInfo* pCode, StockDay* pData, int iDay, int iDayLen);

	/******************************************************************
	*	�������ܣ�	���붯�� 
	*	����������	CodeInfo* pCode : [in] Ҫ����Ĺ�Ʊ
	*				StockDay* pData	: [in] ��������
	*				int iDay		: [in] Ҫ���������
	*				BOOL bNewBuy	: [in] �Ƿ�Ϊ��һ�ε�����(Ϊ��������ʱΪFALSE,�������ΪTRUE)
	*	�� �� ֵ��	BOOL : �ɹ�(TRUE), ʧ��(FALSE)
	*	����������	
	*	��    �ߣ�	
	*	������ڣ�	2003��4��24��
	*	�޸�˵����	
	*****************************************************************/
	static BOOL BuyStock(CodeInfo* pCode, StockDay* pData, int iDay, BOOL bNewBuy);

	/******************************************************************
	*	�������ܣ�	��Ʊ��������ʱ��Ԥ����,���óɽ���ϸ�п�ʼ��,�����ۼ�����������
	*	����������	CodeInfo* pCode : [in] Ҫ����Ĺ�Ʊ
	*				StockDay* pData : [in] ��������
	*				BOOL bBuy		: [in] TRUE��һ�촦��(���ÿ�ʼ��), FALSE���һ�촦��(������)
	*				int iCyc = 0	: [in] ����������bBuyΪTRUE����Ч
	*	�� �� ֵ��	BOOL : �ɹ�(TRUE), ʧ��(FALSE)
	*	����������	
	*	��    �ߣ�	
	*	������ڣ�	2003��4��24��
	*	�޸�˵����	
	*****************************************************************/
	static BOOL BeginBuyEndSell(CodeInfo* pCode, StockDay* pData, BOOL bBuy , int iCyc = 0);

	/******************************************************************
	*	�������ܣ�	�Ƿ�������������,�����Ƿ��й�Ʊ����
	*	����������	TCaclTemp* pData : [in] ָ�����������
	*				int iDay		 : [in] Ҫ������������
	*	�� �� ֵ��	long :  -1	������������
							����Ϊ����������:
								TDealFlow_DT_PingChang		ƽ��
								TDealFlow_DT_Sell			����
								TDealFlow_DT_ZhiShun		ֹ��
								TDealFlow_DT_ZhiYing		ֹӮ
								TDealFlow_DT_HuiLuo			����
								TDealFlow_DT_HenPang		����
								TDealFlow_DT_QZPC			ǿ��ƽ��
	*	����������	
	*	��    �ߣ�	
	*	������ڣ�	2003��4��24��
	*	�޸�˵����	
	*****************************************************************/
	static long IsSell(TCaclTemp* pData, int iDay);

	/******************************************************************
	*	�������ܣ�	��������
	*	����������	CodeInfo* pCode : [in] Ҫ����Ĺ�Ʊ
	*				StockDay* pData : [in] ��������
	*				int iDay		: [in] Ҫ������������
	*				int nMode		: [in] ������ʽ(IsSell()�Ŀ��򷵻�)
	*	�� �� ֵ��	BOOL 
	*	����������	
	*	��    �ߣ�	
	*	������ڣ�	2003��4��24��
	*	�޸�˵����	
	*****************************************************************/
	static BOOL SellStock(CodeInfo* pCode, StockDay* pData, int iDay, int nMode);

	/******************************************************************
	*	�������ܣ�	ͳ����������(�ӳɽ���ϸ��ͳ�Ƴ���֧��Ʊ�ļ�����,
								 �ٽ��Ӹ��ɵļ�������ͳ�Ƴ��������ԵĽ��)
	*	����������	���ޣ�
	*	�� �� ֵ��	BOOL : �ɹ�(TRUE), ʧ��(FALSE)
	*	����������	
	*	��    �ߣ�	
	*	������ڣ�	2003��4��24��
	*	�޸�˵����	
	*****************************************************************/
	static BOOL Total();
	// ���������m_sCaclResult����
	BOOL ClearData();

	/******************************************************************
	*	�������ܣ�	��sParamֵ���õ���ǰ���ԵĹ�ʽ��������
	*	����������	TExchParam& sParam : [in] Ҫ���õĲ���
	*	�� �� ֵ��	BOOL : �ɹ�(TRUE), ʧ��(FALSE) 
	*	����������	
	*	��    �ߣ�	
	*	������ڣ�	2003��4��24��
	*	�޸�˵����	
	*****************************************************************/
	static BOOL InitialExpressParam(TExchParam& sParam);

public:
	/******************************************************************
	*	�������ܣ�	ֹͣ���Լ���,Ҳ����ֹͣCaclMain()�߳�
	*	����������	���ޣ�
	*	�� �� ֵ��	long : �ɹ�(TCaclReturn_Succ), ʧ��(TCaclReturn_Fail) 
	*	����������	
	*	��    �ߣ�	
	*	������ڣ�	2003��4��24��
	*	�޸�˵����	
	*****************************************************************/
	long Stop();

	/******************************************************************
	*	�������ܣ�	��ʼ���Լ���,Ҳ��������CaclMain()�߳̽��м���
	*	����������	���ޣ�
	*	�� �� ֵ��	long : �ɹ�(TCaclReturn_Succ), ʧ��(TCaclReturn_Fail)
	*	����������	
	*	��    �ߣ�	
	*	������ڣ�	2003��4��24��
	*	�޸�˵����	
	*****************************************************************/
	long Start();

public:
	// �����������ļ����ļ�ͷ�ṹ
	struct _tagFileHead
	{
		char szFlags[2]; // �ļ�ͷ��־
		char cVer;		 // �汾��

		_tagFileHead()
		{
			szFlags[0] = 'h';
			szFlags[1] = 's';
			cVer = 1;
		}

		// ����ļ��Ƿ���ȷ
		BOOL Check()
		{
			if( szFlags[0] == 'h'
				&& szFlags[1] == 's'
				&& cVer == 1
				)
				return TRUE;
			return FALSE;
		}

		void Serialize(CArchive& ar)
		{
			if( ar.IsStoring() )
			{
				ar << szFlags[0];
				ar << szFlags[1];
				ar << cVer;
			}
			else
			{
				ar >> szFlags[0];
				ar >> szFlags[1];
				ar >> cVer;
			}
		}
	};

	// �ļ�ͷ
	_tagFileHead m_fileHead;
	// ������Լ����� ??? �д��Ľ�
	long Save(CWnd* pParent = NULL);
	// ������Լ��������� ??? �д��Ľ�
	long Open(CWnd* pParent = NULL);

public:
	// ��ǰ���й�Ʊ�б�
	CMapStringToString m_ayCurStock;
	// �����������
	long m_lLastBuyDay;

	// ����Ʊ���뵱ǰ���й�Ʊ�б�m_ayCurStock��
	void AddBuyStock(CodeInfo* pInfo);
	// ����Ʊ�ӵ�ǰ���й�Ʊ�б�m_ayCurStock��ɾ��
	void DelBuyStock(CodeInfo* pInfo);
	// �õ���ǰ���еĹ�Ʊ֧��
	long GetBuyStockNum();
	// ��ǰ��Ʊ�б����Ƿ��й�ƱpInfo
	BOOL IsHaveBuyStock(CodeInfo* pInfo);
	// �õ�һ�������Ʊ�����(û��)
	double GetBuyStockMaxMonery();
	// �õ�һ�������Ʊ��������
	long GetBuyStockMaxNum();


	/******************************************************************
	*	�������ܣ�	ȫ�г�����ʱ,�Ƿ������Ľ�һ���ж�(�����й�Ʊ֧����û�г���)
	*	����������	TExchModel* pModel : [in] �г�ģ������
	*				CodeInfo* pInfo    : [in] Ҫ����Ĺ�Ʊ
	*				int nCurNum		   : [in] Ҫ��������(û��???)
	*	�� �� ֵ��	BOOL : ����(TRUE), ������(FALSE)
	*	����������	
	*	��    �ߣ�	
	*	������ڣ�	2003��4��24��
	*	�޸�˵����	
	*****************************************************************/
	static BOOL CheckEntireIsBuy(TExchModel* pModel, CodeInfo* pInfo, int nCurNum);
	enum { MoneryAdd/*��*/, MoneryDel/*��*/, MonerySet/*��������*/ };

	/******************************************************************
	*	�������ܣ�	�������Ľ��
	*	����������	double dMonery			: [in] Ҫ�����Ľ����
	*				int nMask = MoneryAdd	: [in] ������ʽ(��,��������)
	*	�� �� ֵ��	void 
	*	����������	
	*	��    �ߣ�	
	*	������ڣ�	2003��4��24��
	*	�޸�˵����	
	*****************************************************************/
	static void SetLastMonery(double dMonery, int nMask = MoneryAdd, CodeInfo* pCode = NULL);

public:
	// �б�ͼ����Դ
	CImageList			m_oImage;
	// ��ʼ���б�(�����Ŀ,��ʼ���б��м�ͼ��)
	BOOL InitialOptimizeCtrl(CListCtrl* pCtrl, CImageList* pImage = NULL);

	/******************************************************************
	*	�������ܣ�	��ʾ���Խ���б�
	*	����������	CListCtrl* pCtrl : [in] Ŀ���б�ؼ�
	*	�� �� ֵ��	BOOL : ��������ȷ(FALSE), ����(TRUE)
	*	����������	
	*	��    �ߣ�	
	*	������ڣ�	2003��4��24��
	*	�޸�˵����	
	*****************************************************************/
	BOOL ToOptimizeCtrl(CListCtrl* pCtrl);

	/******************************************************************
	*	�������ܣ�	�õ���������IDΪuID���ģ�2,1,1)��ʽ�Ĳ����ִ�
	*	����������	BOOL bSignal  : [in] ����Ʊ(TRUE), ȫ�г�(FALSE)
	*				UINT uID = -1 : ����ID
	*	�� �� ֵ��	CString : �����ִ�
	*	����������	
	*	��    �ߣ�	
	*	������ڣ�	2003��4��24��
	*	�޸�˵����	
	*****************************************************************/
	CString GetParamStr(BOOL bSignal, UINT uID = -1);

	/******************************************************************
	*	�������ܣ�	�Ƚϴ�С�������Ż��ı������ʹ��
	*	����������	UINT uID1 : [in] �Ƚϵĵ�һ���������Ĳ������õĲ���ID
	*				UINT uID2 : [in] �Ƚϵĵڶ����������Ĳ������õĲ���ID
	*				int ColID : [in] Ҫ�Ƚϵļ��������ݵ�ID
	*	�� �� ֵ��	int : 0 ���
						 -1 ��һ��������С�ڵڶ��������� 
						  1	��һ�����������ڵڶ���������
	*	����������	
	*	��    �ߣ�	
	*	������ڣ�	2003��4��24��
	*	�޸�˵����	
	*****************************************************************/
	int CompareParamOfCol(UINT uID1, UINT uID2, int ColID);

	/******************************************************************
	*	�������ܣ�	��Ĭ�ϲ���m_sDefParam���õ�pExp�Ĳ���
	*	����������	CExpression* pExp = NULL : [in] Ҫ���õĹ�ʽ, Ϊ�������õ�ǰ���Թ�ʽ
	*	�� �� ֵ��	BOOL : �ɹ�(TRUE), ʧ��(FALSE)
	*	����������	
	*	��    �ߣ�	
	*	������ڣ�	2003��4��24��
	*	�޸�˵����	
	*****************************************************************/
	BOOL ResetDefParam(CExpression* pExp = NULL);

	/******************************************************************
	*	�������ܣ�	��pExp�Ĳ������õ�Ĭ�ϲ���m_sDefParam
	*	����������	CExpression* pExp = NULL : [in] Ҫ����Ĺ�ʽ,Ϊ����ָ��ǰ���Թ�ʽ
	*	�� �� ֵ��	BOOL : �ɹ�(TRUE), ʧ��(FALSE)
	*	����������	
	*	��    �ߣ�	
	*	������ڣ�	2003��4��24��
	*	�޸�˵����	
	*****************************************************************/
	BOOL FromExpToDefParam(CExpression* pExp = NULL);
	// �������� 2003-08-19 ����
	BOOL ReadSetting();
	// д������ 2003-08-19 ����
	BOOL WriteSetting();

	static long GetStockPriceDecimal(CString strCode, int& index);
	static long GetStockPriceUnit(CString strCode,int& index);
};

