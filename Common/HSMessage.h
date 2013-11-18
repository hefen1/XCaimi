#pragma once

//**框架消息定义
#define WM_BIZWINDOW_NOTIFY       WM_USER + 1000
	#define BWN_WINDOW_SHOW             1 //通知：业务窗口显示
	#define BWN_ACTIVATE_CHANGE         2 //通知：同一tab页内的业务窗口活动状态被切换
	#define BWN_HOT_KEY                 3 //通知：热键响应
    #define BWN_FOCUS_WND               4 //通知：焦点激活
    #define BWN_LAYOUT_STATE_CHANGE     5 //通知：layout状态(最大化，还原)改变
    #define BWN_ON_OPERAT_TIMEOUT       6 //通知：无操作通知
    #define BWN_PARAM_CHANGED           7 //通知：布局参数变更
#define WM_BIZWINDOW_LINKAGE      WM_USER + 1001
#define WM_KEYBOARD_NOTIFY        WM_USER + 1002 //键盘精灵通知 
  
//**关联消息定义
//公共类消息定义
//新增消息号在PUBLIC_MESSAGE_BASE上累加
//示例: PUBLIC_MSG_DEMO           PUBLIC_MESSAGE_BASE + 1 
#define PUBLIC_MESSAGE_BASE       1000 
#define PUBLIC_MESSAGE_QUICKTRADE PUBLIC_MESSAGE_BASE + 1  // 闪电下单消息
   // 闪电下单消息参数
   #define PARAM_QUICKTRADE_PRICE _T("price")   // 价格 - double 
   #define PARAM_QUICKTRADE_CODE  _T("code")    // 代码 - CString
   // 参数定义结束
#define PUBLIC_MESSAGE_TRADE      PUBLIC_MESSAGE_BASE + 2  // 普通买卖消息
   // 普通买卖消息参数
   #define PARAM_TRADE_PRICE _T("price")  // 价格 - double
   #define PARAM_TRADE_CODE _T("code")    // 代码 - CString
   // 参数定义结束


//行情类消息定义
//新增消息号在QUOTE_MESSAGE_BASE上累加
//示例: QUOTE_MSG_DEMO           QUOTE_MESSAGE_BASE + 1 
//      PARAM_1                  参数名称定义宏1
//      PARAM_2                  参数名称定义宏2
//      PARAM_n                  参数名称定义宏n
#define QUOTE_MESSAGE_BASE       10000  
//代码变更 
#define QUOTE_MESSAGE_CODE_CHANGE        QUOTE_MESSAGE_BASE + 1
    #define PARAM_CODE_CHANGE_MARK_TYPE     _T("mark_type")    // 代码类型 - CString type
    #define PARAM_CODE_CHANGE_STOCK_CODE    _T("stock_code")   // 代码 - CString code
    #define PARAM_CODE_CHANGE_MSG_FROM      _T("message_from")     // 消息来源 1:来自交易
       #define  PARAM_CODE_CHANGE_MSG_FROM_TRADE   _T("1")
//所属板块
#define QUOTE_MESSAGE_BLOCK_ADSCRIPTION  QUOTE_MESSAGE_BASE + 2
    #define PARAM_BLOCK_ADCRIPTION_CODETYPE _T("mark_type")    // 代码类型 - CString type
    #define PARAM_BLOCK_ADCRIPTION_CODE     _T("stock_code")   // 代码 - CString code
//通知图层
#define QUOTE_MESSAGE_NOTIFY_DIAGRAM_LAYOUT  QUOTE_MESSAGE_BASE + 3
    #define PARAM_DIAGRAM_LAYOUT_NAME       _T("diagram_layout_name") //图元消息id 参见QuoteDefine.h定义
    #define PARAM_DIAGRAMMSGID              _T("diagram_msg_id")      //图元消息id 参见QuoteDefine.h定义
//代码上下翻动（变更） 
#define QUOTE_MESSAGE_CODE_PAGE_UP_DOWN      QUOTE_MESSAGE_BASE + 4
	#define PARAM_CODE_PAGE_UP_DOWN_DIRECTION       _T("direction")//翻动方向
		#define  PARAM_CODE_PAGE_UP_DOWN_DIRECTION_UP     _T("UP")     //direction:UP
        #define  PARAM_CODE_PAGE_UP_DOWN_DIRECTION_DOWN   _T("DOWN")   //direction:DOWN
//交易消息定义
//新增消息号在TRADE_MESSAGE_BASE上累加
//示例: TRADE_MSG_DEMO           TRADE_MESSAGE_BASE + 1 
#define TRADE_MESSAGE_BASE			20000       
#define TRADE_MESSAGE_REFRESH		TRADE_MESSAGE_BASE + 1	
#define TRADE_MESSAGE_ACTIONNAME	TRADE_MESSAGE_BASE + 2	
	#define PARAM_ACTIONNAME			_T("action_name")
#define TRADE_MESSAGE_OPENLAYOUT	TRADE_MESSAGE_BASE + 3
#define TRADE_MESSAGE_QUERYCOUNT	TRADE_MESSAGE_BASE + 4
	#define TRADE_PARAM_COUNT			_T("count")
#define TRADE_MESSAGE_WITHDRAW		TRADE_MESSAGE_BASE + 5
#define TRADE_MESSAGE_STOCKCODE		TRADE_MESSAGE_BASE + 6
	#define TRADE_PARAM_STOCKCODE			_T("stock_code")
#define TRADE_MESSAGE_FUNDCODE		TRADE_MESSAGE_BASE + 7
#define TRADE_PARAM_FUNDCODE			_T("fund_code")

//**菜单消息:menu action定义，方便配置文件维护
//规范:#define MA_XXX  模块名.自定义名称
//Winner.exe
#define MA_NEWPAGE                  _T("Winner.NewPage")
#define MA_EXITAPP                  _T("Winner.ExitApp")
//FormulaMan.dll
#define MA_FORMULAMANANAGER             _T("FormulaMan.FormulaManager")  //公式管理
#define MA_FORMULAFORMULAFINDER       _T("FormulaMan.FormulaFinder")   //公式查找
#define MA_SELECTSTOCK			                  _T("FormulaMan.SelectStock")     //选股
#define MA_TRADETEST			                      _T("FormulaMan.TradeTest")     //选股
//DiagramHold.dll
#define MA_OPENQUOTEREPORT      _T("DiagramHolder.QuoteReport")  //行情报价
#define MA_OPENCOMBANALYSIS     _T("DiagramHolder.CombAnalysis") //组合分析

struct ILinkageParam
{
	virtual ILinkageParam* Clone() = 0;
	virtual LONG GetFrom() = 0;
	virtual BOOL IsSendToSameLayout() = 0;
	virtual CString GetStringField(CString strFieldName) = 0;
	virtual int GetIntegerField(CString strFieldName) = 0;
	virtual double GetDoubleField(CString strFieldName) = 0;
	virtual POSITION GetStartPosition() = 0;
	virtual void GetNextAssoc(POSITION& rNextPosition,CString& strParamName,CString& strValue) = 0;
	virtual void Release() = 0;
};
