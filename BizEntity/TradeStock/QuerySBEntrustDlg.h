/*******************************************************
  Դ��������:QuerySBEntrustDlg.h
  �������Ȩ:�������ӹɷ����޹�˾
  ϵͳ����:  Ͷ��Ӯ�ҽ�������ն�V1.0
  ģ������:  TradeStock
  ����˵��:  ��ѯ����ί��
  ��    ��:  shenglq
  ��������:  20110323
*********************************************************/
#pragma once
#include "TradeQueryBaseDlg.h"

class CQuerySBEntrustDlg : public CTradeQueryBaseDlg
{
	DECLARE_DYNAMIC(CQuerySBEntrustDlg)
public:
	CQuerySBEntrustDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CQuerySBEntrustDlg();
	//virtual DWORD GetDataType();
protected:
	virtual void DoDataCore(IDataList * pDataList);
	virtual void DoPopupMenuEntrust();
private:
};