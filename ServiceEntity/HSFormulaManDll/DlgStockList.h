/*************************************************************
*	Copyright (c)2010, �������ӹɷ����޹�˾
*	All rights reserved.
*
*	�ļ����ƣ�	DlgStockList.h
*	�ļ���ʶ��	
*	ժ	  Ҫ��	��Ʊ�б���
*
*	��ǰ�汾��	1.0
*	��	  �ߣ�	
*	�������ڣ�	2003��4��29��
*
*	�޸�˵����	
*
***************************************************************/
#pragma once
#include "afxcmn.h"
#include "resource.h"

// CDlgStockList dialog

class CDlgStockList : public CDialog
{
	DECLARE_DYNAMIC(CDlgStockList)

public:
	CDlgStockList(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgStockList();

// Dialog Data
	enum { IDD = IDD_STOCKLIST };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

protected:
	// ��Ʊ��������
	CArray<StockUserInfo*, StockUserInfo*> m_ayStock;
	// �̶��Ĺ�Ʊ
	StockUserInfo* m_pFixeStock;
	// ��Ʊ�б�
	CHSListCtrlBase m_wndList;

	// ��һ����Ʊ����Ʊ����
	BOOL AddStock(StockUserInfo* pStock);
	// �ӹ�Ʊ����ɾ��һ֧��Ʊ
	BOOL DelStock(int nIdx);
	// ����ƱpStock�Ƿ��ڹ�Ʊ���鼰�̶���Ʊ�д���
	BOOL IsHave(StockUserInfo* pStock);
	// ��ʾ��Ʊ��Ϣ
	BOOL Display();
	// ��ʼ����Ʊ��ʾ�б�ؼ�
	BOOL InitCtrl();
	// ���̾�����Ϣ����
	LRESULT OnUserDragData(WPARAM wParam, LPARAM lParam);
	// ����̾��鷢�ӹ�Ʊ��Ϣ
	BOOL SendAddStockMsg(MSG *pMsg);

public:
	/******************************************************************
	*	�������ܣ�	����ԭʼ��Ʊ��Ϣ
	*	����������	StockUserInfo* pFixeStock : [in] �̶���Ʊ��
	*				CArray<StockUserInfo*     
	*				StockUserInfo*> &ayStock  : [in] ��Ʊ����
	*	�� �� ֵ��	void 
	*	����������	
	*	��    �ߣ�	
	*	������ڣ�	2003��4��29��
	*	�޸�˵����	
	*****************************************************************/
	void SetStock(StockUserInfo* pFixeStock, CArray<StockUserInfo*, StockUserInfo*> &ayStock);

	/******************************************************************
	*	�������ܣ�	�����ĺ�Ĺ�Ʊ��Ϣ��ֵ��ayStock
	*	����������	CArray<StockUserInfo*
	*				StockUserInfo*> &ayStock : [out] �õ����ĺ�Ĺ�Ʊ��Ϣ
	*	�� �� ֵ��	void 
	*	����������	
	*	��    �ߣ�	
	*	������ڣ�	2003��4��29��
	*	�޸�˵����	
	*****************************************************************/
	void CopyStockTo(CArray<StockUserInfo*, StockUserInfo*> &ayStock);
	// "����"��ť����
	afx_msg void OnBnClickedAdd();
	// "ɾ��"��ť����
	afx_msg void OnBnClickedDel();
	// "ȷ��"��ť����
	afx_msg void OnBnClickedOk4();

	// ��ϢԤ������,��Ҫ�ǽس�KEY��CHAR��Ϣ,���䷢�͸����̾���
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	// �Ի����ʼ��
	virtual BOOL OnInitDialog();
};
