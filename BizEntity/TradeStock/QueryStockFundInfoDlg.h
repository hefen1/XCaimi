/*******************************************************
  Դ��������:QueryStockFundInfoDlg.h
  �������Ȩ:�������ӹɷ����޹�˾
  ϵͳ����:  Ͷ��Ӯ�ҽ�������ն�V1.0
  ģ������:  TradeFund
  ����˵��:  ��ѯ�����̺�ҵ�������Ϣ
  ��    ��:  shenglq
  ��������:  20100910
*********************************************************/
#pragma once
#include "TradeQueryBaseDlg.h"

class CQueryStockFundInfoDlg : public CTradeQueryBaseDlg
{
	DECLARE_DYNAMIC(CQueryStockFundInfoDlg)
public:
	CQueryStockFundInfoDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CQueryStockFundInfoDlg();
protected:
	virtual BOOL OnInit();
	virtual void OnShow(BOOL bShow);
	virtual void OnLinkageMsg(CLinkageParam* pParam);
	virtual void DoGridDBClick();
private:
};