#pragma once
#define  HS_DLL_EXPORT __declspec( dllexport )

//OnFrameNotify֪ͨ���Ͷ���
#define SN_BEFORE_FRAME_CLOSE   1

//IMainWnd::ShowLayout Flag����
#define SP_SHOW       1
#define SP_MAX        2
#define SP_HIDE       3
#define SP_RESTORE    4

#define PUBLIC_OWNER           _T("public_owner")
//�����������
#define PN_STATUSBAR           _T("status_bar")
#define PN_QUOTEREPORT         _T("quote_report")
#define PN_TRADE               _T("trade")
#define PN_QUOTEDIAGRAM        _T("quote_diagram")
#define PN_BLOCKADSCRIPTION    _T("Block_Adscription")   // �������
#define PN_QUICKTRADE_BUY      _T("lightning_entrust_b") // �����µ���
#define PN_QUICKTRADE_SELL     _T("lightning_entrust_s") // �����µ���
//�������ֶ���
#define LN_TRADINGPANEL        _T("TradingPanel")   //������沼��
#define LN_TRADEWORKSPACE      _T("TradeWorkspace") //������湤����
#define LN_TRADELOCKER         _T("TradeLocker")    //��������

//�ȼ�
#define HK_NOOPERATNOTIFY      0

//ҵ�񴰿ڲ����ӿ�
struct IHSParam
{
	virtual CString WINAPI GetStringParam(CString strParamName,CString strDefault = _T("")) = 0;
	virtual int WINAPI GetIntegerParam(CString strParamName,int nDefault = 0  ) = 0;
	virtual double WINAPI GetDoubleParam(CString strParamName,double nDefault = 0 ) = 0;
	virtual CString GetXMLData() = 0;
};

//����ܽӿ�
#define OBJ_MAINFRAME  _T("IMainWnd")
struct IMainWnd
{
	virtual BOOL WINAPI ShowPanel(CString strPanelName,UINT uFlag,CString strOwner = PUBLIC_OWNER) = 0;
	virtual BOOL WINAPI ClosePanel(CString strPanelName,CString strOwner = PUBLIC_OWNER) = 0;
	virtual BOOL WINAPI IsPanelZoomed(CString strPanelName,CString strOwner = PUBLIC_OWNER) = 0;
	virtual int WINAPI  DispatchLinkageMsg(UINT nMsg,WPARAM wParam,LPARAM lParam,BOOL bPost) = 0;
	virtual HWND GetMainWndHandle() = 0;
};


// IMainWnd  == > CMainFrame
// |_ILayoutWindow(��) == > CWinnerView      
//   |_ILayoutPanel == > CLayoutView
//     |_ILayoutPanel
//     |_ILayoutWindow
//     |_ILayoutPanel
//       |_IBizWindow

struct IBizWindow;
struct IHSParam;
//���ֽӿ�
#define CURRENT_LAYOUT_TARGET     _T(".")
struct ILayoutWindow
{
	/********************************************************************************
	* �������� :��ILayoutWindow�д�ָ������
	* �������� :CString strTarget: �������ڵ���������(������LayoutWindow��������Ĵ�".")
	*           CString strLayoutName: Ҫ�򿪵Ĳ�������
	*  ����ֵ  :�ɹ�����TRUE����֮ΪFALSE	
	*******************************************************************************/
	virtual BOOL WINAPI OpenLayout(CString strTarget,CString strLayoutName,CString strOwner,IHSParam *pParam = NULL) = 0;

	/********************************************************************************
	* �������� :���Ĳ��ִ��ڴ�С
	* �������� :int cx: ���ֿ��
	*           int cy: ���ָ߶�
	*  ����ֵ  :�ɹ�����TRUE����֮ΪFALSE	
	*******************************************************************************/
	virtual BOOL WINAPI ChangeSize(int cx,int cy) = 0;

	/********************************************************************************
	* �������� :ע�������Ϣ
	* �������� :UINT nMsg: ��Ϣ��
	*           IBizWindow *pBizWnd: ҵ�񴰿ڽӿ�ָ��
	*  ����ֵ  :�ɹ�����TRUE����֮ΪFALSE	
	*******************************************************************************/
	virtual BOOL WINAPI RegisterLinkageMsg(UINT nMsg,IBizWindow *pBizWnd) = 0;

	/********************************************************************************
	* �������� :ȡ��ע�������Ϣ
	* �������� :UINT nMsg: ��Ϣ��
	*           IBizWindow *pBizWnd: ҵ�񴰿ڽӿ�ָ��
	*  ����ֵ  :�ɹ�����TRUE����֮ΪFALSE	
	*******************************************************************************/
	virtual BOOL WINAPI UnregisterLinkageMsg(UINT nMsg,IBizWindow *pBizWnd) = 0;


	/********************************************************************************
	* �������� :���͹�����Ϣ
	* �������� :UINT nMsg: ��Ϣ��
	*           WPARAM wParam
	*           LPARAM lParam
	*           BOOL bPost:�Ƿ���PossMessage��ʽ����
	*  ����ֵ  :�ɹ�����TRUE����֮ΪFALSE	
	*******************************************************************************/
	virtual int WINAPI DispatchLinkageMsg(UINT nMsg,WPARAM wParam,LPARAM lParam,BOOL bPost) = 0;

	/********************************************************************************
	* �������� :ȡ��ע�������Ϣ
	* �������� :UINT id: �ȼ���,��֤ͬһIBizWindow��Ψһ
	*           UINT fsModifiers:����������ֵ�����: MOD_ALT,MOD_CONTROL,MOD_SHIFT,MOD_WIN
	*           UINT vk: Specifies the virtual-key code of the hot key
	*           IBizWindow *pBizWnd: ҵ�񴰿ڽӿ�ָ��
	*  ����ֵ  :�ɹ�����TRUE����֮ΪFALSE	
	*******************************************************************************/
	virtual BOOL WINAPI RegisterHotKey(UINT id,UINT fsModifiers,UINT vk,IBizWindow *pBizWnd) = 0;

	/********************************************************************************
	* �������� :ȡ��ע�������Ϣ
	* �������� :UINT fsModifiers:����������ֵ�����: MOD_ALT,MOD_CONTROL,MOD_SHIFT,MOD_WIN
	*           UINT vk: Specifies the virtual-key code of the hot key
	*           IBizWindow *pBizWnd: ҵ�񴰿ڽӿ�ָ��
	*  ����ֵ  :�ɹ�����TRUE����֮ΪFALSE	
	*******************************************************************************/
	virtual BOOL WINAPI UnregisterHotKey(UINT fsModifiers,UINT vk,IBizWindow *pBizWnd) = 0;
};


//��������ӿ�
struct ILayoutPanel
{
	/********************************************************************************
	* �������� :��ȡILayoutPanel���ڵ�ILayoutWindow
	*  ����ֵ  :�ɹ�����ILayoutWindowָ�룬��֮ΪNULL	
	*******************************************************************************/
	virtual ILayoutWindow*  WINAPI GetLayoutWindow() = 0;

	/********************************************************************************
	* �������� :����ͬһILayoutPanel�е�IBizWindow
	* �������� :IBizWindow *pBizWnd:Ҫ�����IBizWindowָ��
	*  ����ֵ  :�ɹ�����TRUE����֮ΪFALSE	
	*******************************************************************************/
	virtual BOOL WINAPI ActiveBizWindow(IBizWindow *pBizWnd) = 0;

	/********************************************************************************
	* �������� :��ʾ������ILayoutPanel
	* �������� :BOOL bShow:��ʾ--TRUE ����--FALSE
	*  ����ֵ  :
	*******************************************************************************/
	virtual void WINAPI ShowLayout(BOOL bShow) = 0;
};

//ҵ�񴰿ڽӿ�
struct IBizWindow
{
	/********************************************************************************
	* �������� :��ȡҵ�񴰿ڵĴ��ھ��
	* �������� :
	*  ����ֵ  :ҵ�񴰿ڵĴ��ھ��
	*******************************************************************************/
	virtual HWND WINAPI GetHandle() = 0;

	/********************************************************************************
	* �������� :��ʼ��ҵ�񴰿�
	* �������� :ILayoutPanel* pLayoutPanel:ҵ�񴰿����ڵ�ILayoutPanel
	*  ����ֵ  :�ɹ�����TRUE,��֮ΪFALSE	
	*******************************************************************************/
	virtual BOOL WINAPI Init(IMainWnd* pMain,ILayoutPanel* pLayoutPanel,IHSParam* pParam) = 0;

	/********************************************************************************
	* �������� :��ȡҵ�񴰿����ڵ�ILayoutPanel
	* �������� :
	*  ����ֵ  :ҵ�񴰿����ڵ�ILayoutPanel
	*******************************************************************************/
	virtual ILayoutPanel* WINAPI GetLayoutPanel() = 0;
};