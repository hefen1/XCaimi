#pragma once
#define  HS_DLL_EXPORT __declspec( dllexport )

//OnFrameNotify通知类型定义
#define SN_BEFORE_FRAME_CLOSE   1

//IMainWnd::ShowLayout Flag参数
#define SP_SHOW       1
#define SP_MAX        2
#define SP_HIDE       3
#define SP_RESTORE    4

#define PUBLIC_OWNER           _T("public_owner")
//命名面版名称
#define PN_STATUSBAR           _T("status_bar")
#define PN_QUOTEREPORT         _T("quote_report")
#define PN_TRADE               _T("trade")
#define PN_QUOTEDIAGRAM        _T("quote_diagram")
#define PN_BLOCKADSCRIPTION    _T("Block_Adscription")   // 所属板块
#define PN_QUICKTRADE_BUY      _T("lightning_entrust_b") // 闪电下单买
#define PN_QUICKTRADE_SELL     _T("lightning_entrust_s") // 闪电下单卖
//命名布局定义
#define LN_TRADINGPANEL        _T("TradingPanel")   //交易面版布局
#define LN_TRADEWORKSPACE      _T("TradeWorkspace") //交易面版工作区
#define LN_TRADELOCKER         _T("TradeLocker")    //交易销屏

//热键
#define HK_NOOPERATNOTIFY      0

//业务窗口参数接口
struct IHSParam
{
	virtual CString WINAPI GetStringParam(CString strParamName,CString strDefault = _T("")) = 0;
	virtual int WINAPI GetIntegerParam(CString strParamName,int nDefault = 0  ) = 0;
	virtual double WINAPI GetDoubleParam(CString strParamName,double nDefault = 0 ) = 0;
	virtual CString GetXMLData() = 0;
};

//主框架接口
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
// |_ILayoutWindow(布) == > CWinnerView      
//   |_ILayoutPanel == > CLayoutView
//     |_ILayoutPanel
//     |_ILayoutWindow
//     |_ILayoutPanel
//       |_IBizWindow

struct IBizWindow;
struct IHSParam;
//布局接口
#define CURRENT_LAYOUT_TARGET     _T(".")
struct ILayoutWindow
{
	/********************************************************************************
	* 函数功能 :在ILayoutWindow中打开指定布局
	* 函数参数 :CString strTarget: 布局所在的区域名称(区域是LayoutWindow整合区域的传".")
	*           CString strLayoutName: 要打开的布局名称
	*  返回值  :成功返回TRUE，反之为FALSE	
	*******************************************************************************/
	virtual BOOL WINAPI OpenLayout(CString strTarget,CString strLayoutName,CString strOwner,IHSParam *pParam = NULL) = 0;

	/********************************************************************************
	* 函数功能 :更改布局窗口大小
	* 函数参数 :int cx: 布局宽度
	*           int cy: 布局高度
	*  返回值  :成功返回TRUE，反之为FALSE	
	*******************************************************************************/
	virtual BOOL WINAPI ChangeSize(int cx,int cy) = 0;

	/********************************************************************************
	* 函数功能 :注册接收消息
	* 函数参数 :UINT nMsg: 消息号
	*           IBizWindow *pBizWnd: 业务窗口接口指针
	*  返回值  :成功返回TRUE，反之为FALSE	
	*******************************************************************************/
	virtual BOOL WINAPI RegisterLinkageMsg(UINT nMsg,IBizWindow *pBizWnd) = 0;

	/********************************************************************************
	* 函数功能 :取消注册接收消息
	* 函数参数 :UINT nMsg: 消息号
	*           IBizWindow *pBizWnd: 业务窗口接口指针
	*  返回值  :成功返回TRUE，反之为FALSE	
	*******************************************************************************/
	virtual BOOL WINAPI UnregisterLinkageMsg(UINT nMsg,IBizWindow *pBizWnd) = 0;


	/********************************************************************************
	* 函数功能 :发送关联消息
	* 函数参数 :UINT nMsg: 消息号
	*           WPARAM wParam
	*           LPARAM lParam
	*           BOOL bPost:是否以PossMessage方式发送
	*  返回值  :成功返回TRUE，反之为FALSE	
	*******************************************************************************/
	virtual int WINAPI DispatchLinkageMsg(UINT nMsg,WPARAM wParam,LPARAM lParam,BOOL bPost) = 0;

	/********************************************************************************
	* 函数功能 :取消注册接收消息
	* 函数参数 :UINT id: 热键号,保证同一IBizWindow内唯一
	*           UINT fsModifiers:可以是以下值的组合: MOD_ALT,MOD_CONTROL,MOD_SHIFT,MOD_WIN
	*           UINT vk: Specifies the virtual-key code of the hot key
	*           IBizWindow *pBizWnd: 业务窗口接口指针
	*  返回值  :成功返回TRUE，反之为FALSE	
	*******************************************************************************/
	virtual BOOL WINAPI RegisterHotKey(UINT id,UINT fsModifiers,UINT vk,IBizWindow *pBizWnd) = 0;

	/********************************************************************************
	* 函数功能 :取消注册接收消息
	* 函数参数 :UINT fsModifiers:可以是以下值的组合: MOD_ALT,MOD_CONTROL,MOD_SHIFT,MOD_WIN
	*           UINT vk: Specifies the virtual-key code of the hot key
	*           IBizWindow *pBizWnd: 业务窗口接口指针
	*  返回值  :成功返回TRUE，反之为FALSE	
	*******************************************************************************/
	virtual BOOL WINAPI UnregisterHotKey(UINT fsModifiers,UINT vk,IBizWindow *pBizWnd) = 0;
};


//布局区域接口
struct ILayoutPanel
{
	/********************************************************************************
	* 函数功能 :获取ILayoutPanel所在的ILayoutWindow
	*  返回值  :成功返回ILayoutWindow指针，反之为NULL	
	*******************************************************************************/
	virtual ILayoutWindow*  WINAPI GetLayoutWindow() = 0;

	/********************************************************************************
	* 函数功能 :激活同一ILayoutPanel中的IBizWindow
	* 函数参数 :IBizWindow *pBizWnd:要激活的IBizWindow指针
	*  返回值  :成功返回TRUE，反之为FALSE	
	*******************************************************************************/
	virtual BOOL WINAPI ActiveBizWindow(IBizWindow *pBizWnd) = 0;

	/********************************************************************************
	* 函数功能 :显示或隐藏ILayoutPanel
	* 函数参数 :BOOL bShow:显示--TRUE 隐藏--FALSE
	*  返回值  :
	*******************************************************************************/
	virtual void WINAPI ShowLayout(BOOL bShow) = 0;
};

//业务窗口接口
struct IBizWindow
{
	/********************************************************************************
	* 函数功能 :获取业务窗口的窗口句柄
	* 函数参数 :
	*  返回值  :业务窗口的窗口句柄
	*******************************************************************************/
	virtual HWND WINAPI GetHandle() = 0;

	/********************************************************************************
	* 函数功能 :初始化业务窗口
	* 函数参数 :ILayoutPanel* pLayoutPanel:业务窗口所在的ILayoutPanel
	*  返回值  :成功返回TRUE,反之为FALSE	
	*******************************************************************************/
	virtual BOOL WINAPI Init(IMainWnd* pMain,ILayoutPanel* pLayoutPanel,IHSParam* pParam) = 0;

	/********************************************************************************
	* 函数功能 :获取业务窗口所在的ILayoutPanel
	* 函数参数 :
	*  返回值  :业务窗口所在的ILayoutPanel
	*******************************************************************************/
	virtual ILayoutPanel* WINAPI GetLayoutPanel() = 0;
};