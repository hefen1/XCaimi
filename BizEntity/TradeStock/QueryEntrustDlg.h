/*******************************************************
  Դ��������:QueryEntrustDlg.h
  �������Ȩ:�������ӹɷ����޹�˾
  ϵͳ����:  Ͷ��Ӯ�ҽ�������ն�V1.0
  ģ������:  TradeStock
  ����˵��:  ��ѯ����ί��
  ��    ��:  shenglq
  ��������:  20100910
*********************************************************/
#pragma once
#include "TradeQueryBaseDlg.h"

class CQueryEntrustDlg : public CTradeQueryBaseDlg
{
	DECLARE_DYNAMIC(CQueryEntrustDlg)
public:
	CQueryEntrustDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CQueryEntrustDlg();
	virtual DWORD GetDataType();
protected:
	void DoEntrust();
	virtual void DoDataCore(IDataList * pDataList);
	virtual void DoPopupMenuEntrust();
private:
};