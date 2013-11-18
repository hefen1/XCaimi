#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "resource.h"
#include "NetEngineInterface.h"

// CDlgSrvSet �Ի���

class CDlgSrvSet : public CDialog
{
	DECLARE_DYNAMIC(CDlgSrvSet)

public:
	CDlgSrvSet(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgSrvSet();

// �Ի�������
	enum { IDD = IDD_DIALOG_SRV_SET };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
protected:
	BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
public:	
	CEdit m_cSrvName;
	CEdit m_cSrvPort;
	CEdit m_cSrvAddr;
	CommInfo *m_pConn;
	Server_type m_type;
	void SetConn(CommInfo *pConn){ m_pConn = pConn;}
	BOOL CheckSrv();
};
