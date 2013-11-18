/*******************************************************
  源程序名称:ModifyUserInfoDlg.h
  软件著作权:恒生电子股份有限公司
  系统名称:  投资赢家金融理财终端V1.0
  模块名称:  交易
  功能说明:  修改用户信息窗口基类
  作    者:  shenglq
  开发日期:  20110315
*********************************************************/
#pragma once
#include "TradeCommAction.h"
#include "TradeDialogBase.h"
#include "afxwin.h"

class CModifyUserInfoDlg : public CTradeDialogBase
{
	DECLARE_DYNAMIC(CModifyUserInfoDlg)
public:
	CModifyUserInfoDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CModifyUserInfoDlg();
	virtual void SetSize();
	// 对话框数据
	enum { IDD = IDD_DIALOG_MODIFYUSERINFO};
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInit();
	virtual void OnShow(BOOL bShow);
	virtual void OnWindowFocused();
	virtual BOOL Validate();
	virtual void LoadWnd();
	virtual void OnHotKey(UINT nId);
	virtual void OnOK();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	CEdit m_edClientName;
	CEdit m_edEMail;
	CEdit m_edIDKind;
	CEdit m_edIDNo;
	CEdit m_edIDAddress;
	CEdit m_edPhoneCode;
	CEdit m_edMobiletelePhone;
	CEdit m_edFax;
	CEdit m_edZipCode;
	CEdit m_edAddress;
	CButton m_btOK;
	afx_msg void OnBnClickedBtOk();
private:
	IUserInfoItem* m_UserInfo;
	INodeTreeData* m_IDKindList;
};