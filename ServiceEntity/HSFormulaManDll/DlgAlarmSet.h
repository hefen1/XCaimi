/*************************************************************
*	Copyright (c)2010, �������ӹɷ����޹�˾
*	All rights reserved.
*
*	�ļ����ƣ�	DlgAlarmSet.h
*	�ļ���ʶ��	Ԥ��ϵͳ"��������ģ��
*	ժ	  Ҫ��	
*
*	��ǰ�汾��	1.0
*	��	  �ߣ�	
*	�������ڣ�
*
*	�޸�˵����	
*
***************************************************************/
#pragma once
#include "afxcmn.h"
#include "TreeCtrlFormula.h"
#include "resource.h"
#include "ExpTabCtrl.h"
#include "AdvancedAlarmData.h"
#include "DlgAlarmMan.h"
#include <afxwin.h>
#include "EditEidolon.h"

#define HS_STOCK_CODE	"��Ʊ����"
#define HS_STOCK_NAME	"��Ʊ����"
// CDlgAlarmSet dialog

class CDlgAlarmSet : public CDialog
{
	DECLARE_DYNAMIC(CDlgAlarmSet)

public:
	CDlgAlarmSet(CWnd* pParent = NULL, CAdvancedAlarmData* pAlarmData = NULL, StockUserInfo* pStockData = NULL, HWND hAlarmWnd = 0);   // standard constructor
	virtual ~CDlgAlarmSet();

// Dialog Data
	enum { IDD = IDD_ALARM_SET };

	CExpTabCtrl	m_wndTab;															//ָ����
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
	virtual void OnCancel();
	virtual BOOL OnInitDialog();	
	long LoadCurExp(WPARAM wParam = 0, LPARAM lParam = 0);							//���빫ʽ--��Ϣ��Ӧ


public:
	
	// Ԥ������ָ��
	HWND m_hAlarmWnd;
	// ���޸Ļ����½�
	BOOL m_bChageOrBuild;
	// Ĭ�ϼ���Ĺ�Ʊ
	StockUserInfo* m_pAddStock;
	// ����Ĺ�Ʊ������
	int m_nAddStockIndex;
	// �ǲ�ʹ�ð��
	CButton m_ctlBlockSel;
	// ����
	CButton m_ctlDelay;
	// �Ի���
	CButton m_ctlBox;
	// ����
	CButton m_ctlContinue;
	// ���
	CButton m_ctlSell;
	// ����
	CButton m_ctlJG;
	// �г����а��
	CComboBox m_ctlBlockCom;
	// ��ǰ�����
	CString m_ctlStrBlockList;

	// ��Ʊ�б�
	CListCtrl	m_ctlStockList;														
	// ��ǰ��Ʊ�б�����
	int			m_nStockIndex;														

	// �����ڴ����Ԥ�����ݽṹָ��
	CAdvancedAlarmData* m_pAlarmData;													

	// ��ʽ��������
	CEditEidolon m_EditEidolon;														

	//�������
	UnionSelCondition*		m_pUnion;														
	//��ָ��
	SingleSelCondition*	m_pSingleSel;													
	//�ǵ�ָ�꣺true �����������false;
	BOOL			m_SingOrUnion;													

	//��Ʊ�б�
	CArray<StockUserInfo*, StockUserInfo*>	m_ayStkList;							
	//��ǰ�����
	CString									m_CurSBlockName;	

	// Ԥ����
	CString m_strConditionName;														

	//��������
	CComboBox m_ctlPeried;	

	// ������ʾ�б����ͼ��
	CImageList m_itemList;

public:

	// ��ʼ��
	void			Init(void);														
	//��ʼ������б�����а����--����Ϣ
	void			InitBlockName();												
	//��ʼ��ָ������ѡ��
	void			InitTreeSel();													
	//��ʼ������--���빦�����������趨
	void			InitFace();														
	//��ʼ����Ʊ�б�ͷ
	void			InitStkList();													

	//����Ʊ�б�����
	void			DrawStkList();													

	//��/����Ʊ
	BOOL			AddStock(StockUserInfo* pData);									
	void			DelStock(int nIndex);											

	//����OK��ť״̬
	void			SetOkEnable();													

	//�������Ϲ�����
	BOOL			SaveLoad(UINT nMode,CString strFileName = "",DWORD dStyle = 0);	

	//�õ���ʽ����
	CString			GetDescribe(UnionSelCondition* pHSUnion);	

	// �õ������
	void			GetBlockBoxName();

	//����/�õ�����--��ѡ��״̬
	void			SetType();														
	WORD			GetType();														

	//�õ����а����--��Ϣ��Ӧ
	long			OnGetBlockName(WPARAM wp,LPARAM lp);							

	//ɾ���ڴ�ռ�
	void			Free();	

	// ���ͼӹ�Ʊ��Ϣ
	BOOL			SendAddStockMsg(MSG *pMsg);
	// ���͵õ�������й�Ʊ��Ϣ
	BOOL			SendBlockInfoMsg(CString strBlockName);
	// ����ɾ������״̬
	BOOL			SetDelStockButton(BOOL bFlag);
	
	
protected:
	//���ü�/����Ʊ��ť״̬
	void	SetEnableAddDelButtons(BOOL Flag);										

public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedBlokcSel();
	afx_msg void OnBnClickedDesc();
	afx_msg void OnBnClickedDelStk();
	afx_msg void OnNMClickStkList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnCbnKillfocusBlockList();
	afx_msg void OnCbnSetfocusBlockList();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedSet();
	LRESULT		 OnUserDragData(WPARAM wParam, LPARAM lParam);
	//�ı�����
	afx_msg void OnEnChangeName();													
	//�����Ʊ
	afx_msg void OnBnClickedAddStk();	
	afx_msg void OnCbnSelchangeBlockList();
	afx_msg void OnNMSetfocusStkList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMKillfocusStkList(NMHDR *pNMHDR, LRESULT *pResult);
};
#pragma once
