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
	UINT     m_nMsg;     //��Ϣ��
	BOOL     m_bPost;   //�Ƿ���post��Ϣ
	LONG     m_lFrom;   //���ԵĲ���
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

// CHSBizDialog �Ի���
class CHSBizDialog : public CDialogEx,IBizWindow
{
	DECLARE_DYNAMIC(CHSBizDialog)

public:
	CHSBizDialog(UINT nID,CWnd* pParent = NULL,BOOL bFreeSelf = TRUE);   // ��׼���캯��
	virtual ~CHSBizDialog();

	static HHOOK g_hkkMsgBox;
	static INT HSMessageBox(HWND hwnd,   CString strText,   CString strCaption, UINT uType);
//IBizWindow�ӿ�ʵ��
	virtual HWND WINAPI GetHandle();
	virtual BOOL WINAPI Init(IMainWnd* pMain,ILayoutPanel* pLayoutPanel,IHSParam* pParam);
	virtual ILayoutPanel* WINAPI GetLayoutPanel();
	ILayoutWindow* GetLayoutWindow();
	IMainWnd* GetMain() { return m_pMain;}

//������ڹ���ʵ��
	//��sendmessage��ʽ���͹�����Ϣ
	int SendLinkageMsg(UINT nMsg,CLinkageParam* pLinkageParam);
	//��postmessage��ʽ���͹�����Ϣ
	int PostLinkageMsg(UINT nMsg,CLinkageParam* pLinkageParam);
	//ע����Ҫ��Ӧ��linkage msessage
	BOOL RegisterLinkageMsg(UINT nMsg);
	//ȡ��ע����Ҫ��Ӧ��linkage msessage
	BOOL UnregisterLinkageMsg(UINT nMsg);
	//ע���ȼ�
	BOOL RegisterHotKey(UINT id,UINT fsModifiers,UINT vk);
	//ȡ��ע���ȼ�
	BOOL UnregisterHotKey(UINT id,UINT fsModifiers,UINT vk);
	//��������Ƿ���ʾ
	BOOL IsShow();
	//��������������
	BOOL ActiveSelf();
	//�򿪼��̾���
	void OpenKeyBoard(int nCode,HSShowKeyboardInfo* pKeyInfo=NULL);
	void OpenKeyBoard(CString strCode,HSShowKeyboardInfo* pKeyInfo=NULL);
protected:
	virtual void PostNcDestroy();
	virtual void OnOK();
	virtual void OnCancel();
	
   //�������֪ͨ�ص�
	//��ʼ��: ���ڴ�����ɺ����
	virtual BOOL OnInit();
	//������ʾ/����(����tabҳ�л�)ʱ���� 
	virtual void OnShow(BOOL bShow);
	//�ȼ���Ӧ
	virtual void OnHotKey(UINT nId);
	//���㴰��
	virtual void OnWindowFocused();
	//��Ӧ�����¼�: �յ�������Ϣʱ����
	virtual void OnLinkageMsg(CLinkageParam* pParam);	
	//����������ڲ���״̬�ı�
	virtual void OnLayoutStateChange();
	virtual void OnNoOperatTimeout();
	virtual void OnParamChanged(IHSParam* pParam);
	//���̾������֪ͨ
	virtual BOOL OnOpenKeyboard(int nCode);
	virtual void OnKeyBoardCodeNotify(StockUserInfo* pStkInfo);
	virtual BOOL OnKeyBoardNotify(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
	afx_msg LRESULT OnBizWindowNotify(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnLinkageNotify(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnKeyboardNotify(WPARAM wParam, LPARAM lParam);

protected:
	BOOL             m_bFreeSelf; //�Ƿ����ͷ�
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