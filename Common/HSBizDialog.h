#pragma once
#include <afxdialogex.h>
#include ".\HSWinner.h"
#include ".\HSMessage.h"
#include ".\KeyboardInterface.h"
#include ".\hsstructnew.h"


class CLinkageParam: public ILinkageParam
{
private: 
	CMapStringToString m_valueMap;
public:
	UINT     m_nMsg;     //消息号
	BOOL     m_bPost;   //是否是post消息
	LONG     m_lFrom;   //来自的布局
	BOOL     m_bSendToSameLayout;

	CLinkageParam(BOOL bSendToSameLayout = FALSE);

	void AddParam(CString strFieldName,CString strValue);

	virtual LONG GetFrom() { return m_lFrom; }
	virtual BOOL IsSendToSameLayout() { return m_bSendToSameLayout; }
	virtual ILinkageParam* Clone();
	virtual CString GetStringField(CString strFieldName);
	virtual int GetIntegerField(CString strFieldName);
	virtual double GetDoubleField(CString strFieldName);
	virtual POSITION GetStartPosition();
	virtual void GetNextAssoc(POSITION& rNextPosition,CString& strParamName,CString& strValue);
	virtual void Release();
};

// CHSBizDialog 对话框
class CHSBizDialog : public CDialogEx,IBizWindow
{
	DECLARE_DYNAMIC(CHSBizDialog)

public:
	CHSBizDialog(UINT nID,CWnd* pParent = NULL,BOOL bFreeSelf = TRUE);   // 标准构造函数
	virtual ~CHSBizDialog();

	static HHOOK g_hkkMsgBox;
	static INT HSMessageBox(HWND hwnd,   CString strText,   CString strCaption, UINT uType);
//IBizWindow接口实现
	virtual HWND WINAPI GetHandle();
	virtual BOOL WINAPI Init(IMainWnd* pMain,ILayoutPanel* pLayoutPanel,IHSParam* pParam);
	virtual ILayoutPanel* WINAPI GetLayoutPanel();
	ILayoutWindow* GetLayoutWindow();
	IMainWnd* GetMain() { return m_pMain;}

//插件窗口功能实现
	//以sendmessage方式发送关联消息
	int SendLinkageMsg(UINT nMsg,CLinkageParam* pLinkageParam);
	//以postmessage方式发送关联消息
	int PostLinkageMsg(UINT nMsg,CLinkageParam* pLinkageParam);
	//注册需要响应的linkage msessage
	BOOL RegisterLinkageMsg(UINT nMsg);
	//取消注册需要响应的linkage msessage
	BOOL UnregisterLinkageMsg(UINT nMsg);
	//注册热键
	BOOL RegisterHotKey(UINT id,UINT fsModifiers,UINT vk);
	//取消注册热键
	BOOL UnregisterHotKey(UINT id,UINT fsModifiers,UINT vk);
	//插件窗口是否显示
	BOOL IsShow();
	//激活插件窗口自身
	BOOL ActiveSelf();
	//打开键盘精灵
	void OpenKeyBoard(int nCode,HSShowKeyboardInfo* pKeyInfo=NULL);
	void OpenKeyBoard(CString strCode,HSShowKeyboardInfo* pKeyInfo=NULL);
protected:
	virtual void PostNcDestroy();
	virtual void OnOK();
	virtual void OnCancel();
	
   //插件窗口通知回调
	//初始化: 窗口创建完成后调用
	virtual BOOL OnInit();
	//窗口显示/隐藏(包括tab页切换)时调用 
	virtual void OnShow(BOOL bShow);
	//热键响应
	virtual void OnHotKey(UINT nId);
	//焦点窗口
	virtual void OnWindowFocused();
	//响应关联事件: 收到关联消息时调用
	virtual void OnLinkageMsg(CLinkageParam* pParam);	
	//插件窗口所在布局状态改变
	virtual void OnLayoutStateChange();
	virtual void OnNoOperatTimeout();
	virtual void OnParamChanged(IHSParam* pParam);
	//键盘精灵代码通知
	virtual BOOL OnOpenKeyboard(int nCode);
	virtual void OnKeyBoardCodeNotify(StockUserInfo* pStkInfo);
	virtual BOOL OnKeyBoardNotify(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
	afx_msg LRESULT OnBizWindowNotify(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnLinkageNotify(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnKeyboardNotify(WPARAM wParam, LPARAM lParam);

protected:
	BOOL             m_bFreeSelf; //是否自释放
	BOOL             m_bAutoKeyboard;
	ILayoutPanel*    m_pLayoutPanel;
	IHSParam*        m_pParam;
	IMainWnd*        m_pMain;
	IKeyboard*       m_pKeyboard;

protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
private:
	BOOL IsValidateKeyboardCode(int nCode);
};