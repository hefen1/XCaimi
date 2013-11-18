/*******************************************************
  Դ��������:ModifyUserInfoDlg.h
  �������Ȩ:�������ӹɷ����޹�˾
  ϵͳ����:  Ͷ��Ӯ�ҽ�������ն�V1.0
  ģ������:  ����
  ����˵��:  �޸��û���Ϣ���ڻ���
  ��    ��:  shenglq
  ��������:  20110315
*********************************************************/
#pragma once
#include "TradeCommAction.h"
#include "TradeDialogBase.h"
#include "afxwin.h"

class CModifyUserInfoDlg : public CTradeDialogBase
{
	DECLARE_DYNAMIC(CModifyUserInfoDlg)
public:
	CModifyUserInfoDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CModifyUserInfoDlg();
	virtual void SetSize();
	// �Ի�������
	enum { IDD = IDD_DIALOG_MODIFYUSERINFO};
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
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