/*******************************************************
  Դ��������:QueryHoldDlg.h
  �������Ȩ:�������ӹɷ����޹�˾
  ϵͳ����:  Ͷ��Ӯ�ҽ�������ն�V1.0
  ģ������:  TradeFund
  ����˵��:  ��ѯ�ֲ�
  ��    ��:  shenglq
  ��������:  20100910
*********************************************************/
#pragma once
#include "TradeQueryBaseDlg.h"

class CQueryHoldDlg : public CTradeQueryBaseDlg
{
	DECLARE_DYNAMIC(CQueryHoldDlg)
public:
	CQueryHoldDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CQueryHoldDlg();
	virtual DWORD GetDataType();
protected:
	virtual void DoDataCore(IDataList * pDataList);
private:
};