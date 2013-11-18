#pragma once

//**�����Ϣ����
#define WM_BIZWINDOW_NOTIFY       WM_USER + 1000
	#define BWN_WINDOW_SHOW             1 //֪ͨ��ҵ�񴰿���ʾ
	#define BWN_ACTIVATE_CHANGE         2 //֪ͨ��ͬһtabҳ�ڵ�ҵ�񴰿ڻ״̬���л�
	#define BWN_HOT_KEY                 3 //֪ͨ���ȼ���Ӧ
    #define BWN_FOCUS_WND               4 //֪ͨ�����㼤��
    #define BWN_LAYOUT_STATE_CHANGE     5 //֪ͨ��layout״̬(��󻯣���ԭ)�ı�
    #define BWN_ON_OPERAT_TIMEOUT       6 //֪ͨ���޲���֪ͨ
    #define BWN_PARAM_CHANGED           7 //֪ͨ�����ֲ������
#define WM_BIZWINDOW_LINKAGE      WM_USER + 1001
#define WM_KEYBOARD_NOTIFY        WM_USER + 1002 //���̾���֪ͨ 
  
//**������Ϣ����
//��������Ϣ����
//������Ϣ����PUBLIC_MESSAGE_BASE���ۼ�
//ʾ��: PUBLIC_MSG_DEMO           PUBLIC_MESSAGE_BASE + 1 
#define PUBLIC_MESSAGE_BASE       1000 
#define PUBLIC_MESSAGE_QUICKTRADE PUBLIC_MESSAGE_BASE + 1  // �����µ���Ϣ
   // �����µ���Ϣ����
   #define PARAM_QUICKTRADE_PRICE _T("price")   // �۸� - double 
   #define PARAM_QUICKTRADE_CODE  _T("code")    // ���� - CString
   // �����������
#define PUBLIC_MESSAGE_TRADE      PUBLIC_MESSAGE_BASE + 2  // ��ͨ������Ϣ
   // ��ͨ������Ϣ����
   #define PARAM_TRADE_PRICE _T("price")  // �۸� - double
   #define PARAM_TRADE_CODE _T("code")    // ���� - CString
   // �����������


//��������Ϣ����
//������Ϣ����QUOTE_MESSAGE_BASE���ۼ�
//ʾ��: QUOTE_MSG_DEMO           QUOTE_MESSAGE_BASE + 1 
//      PARAM_1                  �������ƶ����1
//      PARAM_2                  �������ƶ����2
//      PARAM_n                  �������ƶ����n
#define QUOTE_MESSAGE_BASE       10000  
//������ 
#define QUOTE_MESSAGE_CODE_CHANGE        QUOTE_MESSAGE_BASE + 1
    #define PARAM_CODE_CHANGE_MARK_TYPE     _T("mark_type")    // �������� - CString type
    #define PARAM_CODE_CHANGE_STOCK_CODE    _T("stock_code")   // ���� - CString code
    #define PARAM_CODE_CHANGE_MSG_FROM      _T("message_from")     // ��Ϣ��Դ 1:���Խ���
       #define  PARAM_CODE_CHANGE_MSG_FROM_TRADE   _T("1")
//�������
#define QUOTE_MESSAGE_BLOCK_ADSCRIPTION  QUOTE_MESSAGE_BASE + 2
    #define PARAM_BLOCK_ADCRIPTION_CODETYPE _T("mark_type")    // �������� - CString type
    #define PARAM_BLOCK_ADCRIPTION_CODE     _T("stock_code")   // ���� - CString code
//֪ͨͼ��
#define QUOTE_MESSAGE_NOTIFY_DIAGRAM_LAYOUT  QUOTE_MESSAGE_BASE + 3
    #define PARAM_DIAGRAM_LAYOUT_NAME       _T("diagram_layout_name") //ͼԪ��Ϣid �μ�QuoteDefine.h����
    #define PARAM_DIAGRAMMSGID              _T("diagram_msg_id")      //ͼԪ��Ϣid �μ�QuoteDefine.h����
//�������·���������� 
#define QUOTE_MESSAGE_CODE_PAGE_UP_DOWN      QUOTE_MESSAGE_BASE + 4
	#define PARAM_CODE_PAGE_UP_DOWN_DIRECTION       _T("direction")//��������
		#define  PARAM_CODE_PAGE_UP_DOWN_DIRECTION_UP     _T("UP")     //direction:UP
        #define  PARAM_CODE_PAGE_UP_DOWN_DIRECTION_DOWN   _T("DOWN")   //direction:DOWN
//������Ϣ����
//������Ϣ����TRADE_MESSAGE_BASE���ۼ�
//ʾ��: TRADE_MSG_DEMO           TRADE_MESSAGE_BASE + 1 
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

//**�˵���Ϣ:menu action���壬���������ļ�ά��
//�淶:#define MA_XXX  ģ����.�Զ�������
//Winner.exe
#define MA_NEWPAGE                  _T("Winner.NewPage")
#define MA_EXITAPP                  _T("Winner.ExitApp")
//FormulaMan.dll
#define MA_FORMULAMANANAGER             _T("FormulaMan.FormulaManager")  //��ʽ����
#define MA_FORMULAFORMULAFINDER       _T("FormulaMan.FormulaFinder")   //��ʽ����
#define MA_SELECTSTOCK			                  _T("FormulaMan.SelectStock")     //ѡ��
#define MA_TRADETEST			                      _T("FormulaMan.TradeTest")     //ѡ��
//DiagramHold.dll
#define MA_OPENQUOTEREPORT      _T("DiagramHolder.QuoteReport")  //���鱨��
#define MA_OPENCOMBANALYSIS     _T("DiagramHolder.CombAnalysis") //��Ϸ���

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
