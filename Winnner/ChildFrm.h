
// ChildFrm.h : CChildFrame 类的接口
//

#pragma once

class CChildFrame : public CMDIChildWndEx
{
	DECLARE_DYNCREATE(CChildFrame)
private:
	CString          m_strLayoutFile;   //窗口布局配置文件
	CString          m_strMenuFile;     //菜单树配置文件
	CWinnerView*     m_pWinnerView;
public:
	CChildFrame();
	CChildFrame(CMDIFrameWndEx* pPrentWnd);

	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	void SetLayoutInfo(CString strLayoutFileName,CString strMenuFileFname);
	CWinnerView* GetWinnerView() { return m_pWinnerView; }
public:
	virtual ~CChildFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
protected:
	virtual void PostNcDestroy();
public:
	afx_msg void OnClose();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	virtual void OnUpdateFrameMenu(BOOL bActive, CWnd* pActiveWnd, HMENU hMenuAlt);
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
};
