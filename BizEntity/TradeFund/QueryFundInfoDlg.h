/*******************************************************
  Դ��������:QueryFundInfoDlg.h
  �������Ȩ:�������ӹɷ����޹�˾
  ϵͳ����:  Ͷ��Ӯ�ҽ�������ն�V1.0
  ģ������:  TradeFund
  ����˵��:  ��ѯ������Ϣ
  ��    ��:  shenglq
  ��������:  20100910
*********************************************************/
#pragma once
#include "TradeQueryBaseDlg.h"

class CQueryFundInfoDlg : public CTradeQueryBaseDlg
{
	DECLARE_DYNAMIC(CQueryFundInfoDlg)
public:
	CQueryFundInfoDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CQueryFundInfoDlg();
protected:
	virtual BOOL OnInit();
	virtual void OnShow(BOOL bShow);
	virtual void OnLinkageMsg(CLinkageParam* pParam);
private:
	IFundInfoList* m_pFundInfoList;
};