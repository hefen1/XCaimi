/*******************************************************
  Դ��������:TradeDialogBase.h
  �������Ȩ:�������ӹɷ����޹�˾
  ϵͳ����:  Ͷ��Ӯ�ҽ�������ն�V1.0
  ģ������:  ����
  ����˵��:  ���״��ڻ���
  ��    ��:  shenglq
  ��������:  20100903
*********************************************************/
#pragma once
#include "HSBizDialog.h"
#include "WinnerApplication.h"
#include "TradeCoreIntf.h"
#include "BusinesslogicIntf.h"
#include "DataSourceDefineEx.h"
#include "TradeLangMngIntf.h"
#include "TradeLangDefined.h"
#include "TradeDataCoreIntf.h"
#include "tinyxml.h"


#define  FIRST_COL_H		13
#define  FIRST_COL_X		10
#define  FIRST_COL_Y		11
#define  FIRST_COL_Z		10
#define  SECOND_COL_H		21
#define  SECOND_COL_X		63
#define  SECOND_COL_Y		7
#define  SECOND_COL_Z		2
#define	 STOCKCODE			"stock_code"
#define  ENTRUSTPRICE		"entrust_price"
#define	 FUNDCODE			"fund_code"

//��������            �ϵ���  ����  �µ���  �ҵ���
enum EAdjustDirection {adTToB, adLToR, adBToT, adRToL};
//��������              
enum EAdjustDirectionEx {adxRightTop, adxRightBottom, adxBottomLeft, adxBottomRight, adxTop, adxCenter, adxBottom};

class CTradeDialogBase : public CHSBizDialog, public ITradingAsyncCallBack, public ITradeDataCallBack, public IDataSourceSink
{
	DECLARE_DYNAMIC(CTradeDialogBase)
public:
	CTradeDialogBase(UINT nID, CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CTradeDialogBase();
	void NotifyQuoteChangeCode(CString strStkCode);
	virtual void TradingAsyncCallBack(ITrading* pTrading, int iAction);
	virtual BOOL HSDataSourceSink_OnCommNotify(void* pData);
	virtual BOOL HSDataSourceSink_OnRecvData(void* pData, int nLen);
	virtual void SetAction(const int iAction);
	virtual DWORD GetDataType();
	virtual void OnReflash(IDataList * pDataList, DWORD dDataType); 
	virtual void OnChange(IDataItem * pDataItem, EDataChangeType dtChangeType, DWORD dDataType); 
protected:
	IAccount* m_pAccount;
	ITradeConfigObj* m_pTradeConfigObj;
	ITradeCoreObj* m_pTradeCoreObj;
	int m_iAction;
	IBusinessLogic* m_pBusinessLogic;
	IDataSourceEx* m_pDataSourceEx;
	ITradeLang* m_pTradeLang;
	long m_DataSourceHandle;
	CPtrArray m_paCWnd;
	TiXmlDocument* m_pXMLDataDoc;
	ICCommParamList* m_pAccountParamList;

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInit();
	virtual void PostRefreshMsg();
	virtual void LoadWnd();
	void AdjustLayout(int nHeight, int x = 0, int y = 0, int z = 0,EAdjustDirection adValue = adTToB);
	virtual void AdjustLayoutEx(CWnd* pMain, CWnd* pDetail, int x = 0, int y = 0, EAdjustDirectionEx adxValue = adxRightTop);
	virtual void AdjustLayoutEx(CWnd* pWnd, int x, EAdjustDirectionEx adxValue = adxCenter);

	void SetComoboxHeight(CComboBox* pComoboBox, int nHeight = 100);


	DECLARE_MESSAGE_MAP()	
};
