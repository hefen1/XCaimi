#pragma once
#include "HSBizDialog.h"
#include "DiagramLayer.h"
#include "DiagramView.h"


#define  DiagramHolderLoggerName  _T("DiagramHolder")

// CDlgDiagramHolder �Ի���
class CDlgDiagramHolder : public CHSBizDialog,public IDataSourceSink
{
	DECLARE_DYNAMIC(CDlgDiagramHolder)

public:
	CDlgDiagramHolder(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgDiagramHolder();

// �Ի�������
	enum { IDD = IDD_DIAGRAMHOLDER };

protected:
	CDiagramView*       m_pDiagramView;
	IDataSourceEx*		m_pDataSource;				// ��������ӿ�ָ��
	long				m_nDataSourceID;			// ��������ID
	CString             m_strFixedLayout;           
	BOOL                m_bFixedMode;

	virtual BOOL OnInit();
	virtual void OnExternalChangeContent(IHSParam* pParam);
	virtual BOOL OnOpenKeyboard(int nCode);
	virtual BOOL OnKeyBoardNotify(WPARAM wParam, LPARAM lParam);
	virtual void OnKeyBoardCodeNotify(StockUserInfo* pStkInfo);
	virtual void OnLinkageMsg(CLinkageParam* pParam);

	void OnMsgNotifyDiagramLayout( CLinkageParam* pParam );
	void OnMsgCodeChanged( CLinkageParam* pParam );
	void OnMsgCodePageUpDown( CLinkageParam* pParam );
	DECLARE_MESSAGE_MAP()
public:
	BOOL IsFixedMode() { return m_bFixedMode; }
	BOOL HSDataSourceSink_OnCommNotify(void* pData);
	BOOL HSDataSourceSink_OnRecvData(void* pData, int nLen);

	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
};
