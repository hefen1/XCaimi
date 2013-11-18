/*******************************************************
  Դ��������:TradeQueryWhereDlg.h
  �������Ȩ:�������ӹɷ����޹�˾
  ϵͳ����:  Ͷ��Ӯ�ҽ�������ն�V1.0
  ģ������:  ����
  ����˵��:  ��ѯ�������ڻ���
  ��    ��:  shenglq
  ��������:  20100910
*********************************************************/
#pragma once
#include "TradeCommAction.h"
#include "TradeDialogBase.h"
#include "afxwin.h"

class CTradeQueryWhereDlg : public CTradeDialogBase
{
	DECLARE_DYNAMIC(CTradeQueryWhereDlg)
public:
	CTradeQueryWhereDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CTradeQueryWhereDlg();
	virtual void SetSize();

	// �Ի�������
	enum { IDD = IDD_DIALOG_QUERYWHERE};
	CDateTimeCtrl m_dtBeginDate;
	CDateTimeCtrl m_dtEndDate;
	CButton		  m_btQuery;
protected:
	BOOL m_bWithDrawShow;
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInit();
	virtual void OnShow(BOOL bShow);
	virtual BOOL Validate();
	virtual void OnLinkageMsg(CLinkageParam* pParam);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnBnClickedQuery();
	CStatic m_stCount;
	CButton m_btWithDraw;
	CStatic m_stBeginDate;
	CStatic m_stEndDate;
	afx_msg void OnBnClickedBtWithdraw();
};