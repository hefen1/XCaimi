/*******************************************************
  Դ��������:QueryStockFundEntrustDlg.h
  �������Ȩ:�������ӹɷ����޹�˾
  ϵͳ����:  Ͷ��Ӯ�ҽ�������ն�V1.0
  ģ������:  TradeStock
  ����˵��:  ��ѯ�����̺�ҵ��ί��
  ��    ��:  shenglq
  ��������:  20110323
*********************************************************/
#pragma once
#include "TradeQueryBaseDlg.h"

class CQueryStockFundEntrustDlg : public CTradeQueryBaseDlg
{
	DECLARE_DYNAMIC(CQueryStockFundEntrustDlg)
public:
	CQueryStockFundEntrustDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CQueryStockFundEntrustDlg();
	//virtual DWORD GetDataType();
protected:
	virtual void DoDataCore(IDataList * pDataList);
	virtual void DoPopupMenuEntrust();
private:
};