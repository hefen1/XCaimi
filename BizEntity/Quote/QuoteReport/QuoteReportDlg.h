#include "HSBizDialog.h"
#include "quotetabctrl.h"
#include "skinscrollbar.h"
#include "QuoteReportTable.h"
#include "DataSourceDefineEx.h"
#include "DataSourceSinkDefine.h"
#pragma once


#define QUOTETAB_SPLITERECT_WIDTH	3		// ��ǩҳ�϶������

#define QUOTEREPORT_HOTKEY_F6					1	// �ȼ�F6 ID
#define QUOTEREPORT_HOTKEY_ADD_TO_BLOCK			2	// �ȼ� ��ӵ���� ID
#define QUOTEREPORT_HOTKEY_ADD_TO_SELFBLOCK		3	// �ȼ� ��ӵ���ѡ�� ID
#define QUOTEREPORT_HOTKEY_BLOCK_ADSCRIPTION	4	// �ȼ� ������� ID
#define QUOTEREPORT_HOTKEY_MARK_STRING			5   // �ȼ� �������
#define QUOTEREPORT_HOTKEY_MARK_CANCEL			6	// �ȼ� ȡ�����
#define QUOTEREPORT_HOTKEY_MARK_ICON_BEGIN		20   // �ȼ� ���ͼ�� ��ʼ
#define QUOTEREPORT_HOTKEY_MARK_ICON_END		QUOTEREPORT_HOTKEY_MARK_ICON_BEGIN+MARK_COUNT  // �ȼ� ���ͼ�� ����
// CQuoteReportDlg �Ի���
/*CQuoteReportDlg class
********************************************************************************
<PRE>
������   : CQuoteReportDlg
����     : ���鱨��ģ������Ի�����
--------------------------------------------------------------------------------
��ע     : <ʹ�ø���ʱ��Ҫע������⣨����еĻ���> 
�����÷� : <��������ʹ�÷����ϸ��ӻ����⣬�������͵Ĵ�������> 
--------------------------------------------------------------------------------
����     : <xxx>, [yyy], [zzz] ...�����ߺͶ��ŷָ���޸����б�
</PRE>
*******************************************************************************/
class CQuoteReportDlg : public CHSBizDialog,public IDataSourceSink
{
	DECLARE_DYNAMIC(CQuoteReportDlg)

public:
	CQuoteReportDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CQuoteReportDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_QUOTE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

	CBrush				m_DlgBkBrush;
	HBITMAP				m_hBmpHScrollBar;		// ˮƽ������ͼƬ���
	HBITMAP				m_hBmpVScrollBar;		// ��ֱ������ͼƬ���

	CSkinScrollBar		m_wndHorzScrollWnd;		// ˮƽ������
	CSkinScrollBar		m_wndVertScrollWnd;		// ��ֱ������
	CQuoteTabCtrl*		m_pQuoteTabCtrl;		// ���۱�ǩҳ
	CQuoteReportTable*	m_pQuoteReport;			// ���鱨�۱�
	CRect				m_rcSplit;				// ��ǩ�������λ���϶���
	BOOL				m_bSplitMoved;			// ��¼�Ƿ��϶����ָ���

	IDataSourceEx*		m_pDataSource;				// ��������ӿ�ָ��
	long				m_nDataSourceID;			// ��������ID

	PageViewInfo		m_CurPageInfo;			// ��ǰҳ����Ϣ
public:
	virtual BOOL OnInitDialog();
	virtual BOOL OnInit();
	virtual void OnKeyBoardCodeNotify(StockUserInfo* pStkInfo);
	virtual void OnWindowFocused();

	virtual long GetMarketNameByType(const PageViewInfo* pvInfo,CStringArray& strMarketNames);
	BOOL	SetPageInfo(PageViewInfo* pInfo,CString strColGroupName = _T(""));
	void	SetSize();

	BOOL HSDataSourceSink_OnCommNotify(void* pData);
	BOOL HSDataSourceSink_OnRecvData(void* pData, int nLen);

protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//��Ϣ������
public:
	void			OnHotKey(UINT nId);
	void			OnQuoteReportMsg(UINT nMsgID);
	LRESULT			OnChangeTabItem(WPARAM wParam, LPARAM lParam);
	afx_msg void	OnSize(UINT nType, int cx, int cy);
	afx_msg HBRUSH	OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg BOOL	OnEraseBkgnd(CDC* pDC);
	afx_msg void	OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void	OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	virtual BOOL	PreTranslateMessage(MSG* pMsg);
	afx_msg void	OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void	OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void	OnPaint();
	afx_msg void	OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void	OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void	OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void	OnReportTableNotify(NMHDR* pNotifyStruct,LRESULT* result);

};
