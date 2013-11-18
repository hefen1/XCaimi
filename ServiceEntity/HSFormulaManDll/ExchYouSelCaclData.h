/*************************************************************
*	Copyright (c)2010, �������ӹɷ����޹�˾
*	All rights reserved.
*
*	�ļ����ƣ�	ExchYouSelCaclData.h
*	�ļ���ʶ��	������ѡ������
*	ժ	  Ҫ��	Ŀǰû�в���,��ʱ��д
*
*	��ǰ�汾��	1.0
*	��	  �ߣ�	
*	�������ڣ�
*
*	�޸�˵����	
*
***************************************************************/
#pragma once
#include "ExchCalcMain.h"
#include "TreeCtrlFormula.h"
#include "DlgSingleExchCalcResult.h"


class CExchYouSelCaclData: public CExchCalcMain
{
public:
	CExchYouSelCaclData(void);
	~CExchYouSelCaclData(void);

protected:
	CArray<CExpression*, CExpression*>		m_ayListCtrl;
	CArray<CExpression*, CExpression*>		m_ayExpress;	// �μӲ��ԵĹ�ʽ�б�
	CMapStringToPtr	m_mapCaclResult;	// ���Ų��Խ��

	CArray<StockUserInfo*,StockUserInfo*> 	m_ayStock;	// �μӲ��ԵĹ�Ʊ
	StockUserInfo*	m_pPageStock;
	int	m_lTestObj;		// ����Ŀ��
	int m_lTestCycle;	// ��������

	BOOL m_bUseAllLocalData;	// ʹ�ñ���ȫ������
	int m_lBeginDate;			// ����ʹ�ñ���ָ��ʱ��ε�����
	int m_lCount;

public:
	// ��ʾ���ݹ���
	void CopyExch(CTreeCtrlFormula* pTree);

	// ָ�����ϵͳ��ʽ����
	void ClearExpression();
	void AddExpression(CExpression* pExpression);
	void DelExpression(int nIdx);
	CArray<CExpression*, CExpression*>* GetAyExpression();
	CMapStringToPtr* GetMapResult();
	int GetExpressIdx(CExpression* pExp);
	int ConvertToGlobe(int nIdx);
	BOOL SetExpressionFromCtrl(CListCtrl *pCtrl = NULL);
	BOOL SetSelectAll(CListCtrl *pCtrl = NULL, BOOL bSelCheck = TRUE);

	// �Ż�������ݹ���
	void ClearResult();
	BOOL AddResult(CExpression* pExpression, TExchCaclResult* pResult);
	BOOL DelResult(CExpression* pExpression);
	TExchCaclResult* GetResult(CExpression* pExpression, BOOL bNew = FALSE);
	BOOL IsHave(CExpression* pExpression);
	CArray<StockUserInfo*,StockUserInfo*>* GetAyStock();

	// ��ʾ���
	void InitCtrl(CListCtrl *pCtrl);
	int  Display(CListCtrl *pCtrl = NULL);
	int  Display(CListCtrl *pCtrl, CExpression* pExpression, int nIdx = -1);
	
	// �����������
	void SetDate(BOOL bUseAll, int nBegin = 0, int nCount = 0);
	inline int  GetDayBegin() { return m_lBeginDate; }
	inline int  GetDayCount() { return m_lCount; }
	inline void SetTestObj(int nObj) { m_lTestObj = nObj; }
	inline int GetTestObj() { return m_lTestObj; }
	inline void SetTestCycle(int nCycle) { m_lTestCycle = nCycle; }
	inline int GetTestCycle() { return m_lTestCycle; }
	// ��Ʊ����
	void AddStock(StockUserInfo* pInfo);
	void DelStock(StockUserInfo* pInfo);
	StockUserInfo* GetStock(int nIdx);
	// -1�����û��
	int IsHave(StockUserInfo* pInfo);

	CString ReturnMsgPress(LPARAM lpID);

	// ������
	static UINT s_uTheradStateEx;

	LRESULT StartEx();
	LRESULT StopEx();

	LRESULT OnExchangeData(WPARAM wp,LPARAM lp);

protected:
	CListCtrl*	m_pwndExchList;
	CProgressCtrl* m_pwndProgress;
public:
	inline void SetListCtrl(CListCtrl*	pwndExchList) { m_pwndExchList = pwndExchList; }
	inline CListCtrl* GetListCtrl() { return m_pwndExchList; }
	inline void SetProgress(CProgressCtrl* pwndProgress) { m_pwndProgress = pwndProgress; }
	inline CProgressCtrl* GetProgress() { return m_pwndProgress; }

protected:
	int m_nCurCaclIdx; // ��ǰ�����ָ������
	LRESULT CaclNext(int nIdx = -1); // ������һ�� ���û�У�������ѡ����ź�
	// ��ǰ�������
	long			m_lCurProgress;
	// ÿָ֧�������еĽ���
	double			m_dProgressUint;

public:
	// ����Ĭ�ϲ��Բ���
	void	SetDefTact();
	// �����Զ�����Բ���
	void	SetCommTact();

	// �������
protected:
	CDlgSingleExchCalcResult*	m_dlgTable;
public:
	BOOL OpenTableWnd(UINT uID = 0);
	BOOL CreageTable(CWnd* pParent);
	BOOL DescTable();
};
