/*******************************************************
  源程序名称:FundRiskTestDlg.h
  软件著作权:恒生电子股份有限公司
  系统名称:  投资赢家金融理财终端V1.0
  模块名称:  交易
  功能说明:  基金风险测试
  作    者:  shenglq
  开发日期:  20110425
*********************************************************/
#pragma once
#include "TradeFund.h"
#include "TradeDialogBase.h"
#include "afxwin.h"
typedef CTypedPtrArray<CPtrArray, CButton*> CButtonList;
typedef CTypedPtrArray<CPtrArray, IFundRiskQuestionItem*> CQuestionItemList;


class CMyGroupBox : public CButton
{
public:
	CMyGroupBox();
	virtual~CMyGroupBox();
public:
	void SetRadioHeight(int nHeight){m_nRadioHeight = nHeight;}
	int GetRadioCount(){return m_RadioList.GetCount();}
	void OnSize();
	void SetCode(CString sCode){m_sCode = sCode;}
	CString GetCode(){return m_sCode;}
	void NewRadio(CString sCaption, UINT nID);
	void AddItem(IFundRiskQuestionItem* pItem){m_QuestionItemList.Add(pItem);}
	void AdjustRadio();
	void AdjustGroupWidth(int nWidth);
	int GetScore();
protected:
	CButtonList m_RadioList;
	CQuestionItemList m_QuestionItemList; 
	CString m_sCode;
	int m_nRadioHeight;
};

typedef CTypedPtrArray<CPtrArray, CMyGroupBox*> CMyGroupBoxList;

class CFundRiskTestDlg : public CTradeDialogBase
{
	DECLARE_DYNAMIC(CFundRiskTestDlg)
public:
	CFundRiskTestDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CFundRiskTestDlg();
	// 对话框数据
	enum { IDD = IDD_DIALOG_FUNDRISKTEST};
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInit();
private:
	IFundRiskQuestionList* m_pQuestionList;
	CMyGroupBoxList m_MyGroupBoxList;
	int m_nCurrIndex;
	CString m_sCurrCode;
	INodeTreeData* m_pFundAccRiskList;
	void AdjustButton();
	DECLARE_MESSAGE_MAP()
public:
	CButton m_btNext;
	CButton m_btOK;
	afx_msg void OnBnClickedBtNext();
	afx_msg void OnBnClickedBtOk();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	CButton m_btBack;
	afx_msg void OnBnClickedBtBack();
};