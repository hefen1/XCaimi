/*******************************************************
  Դ��������:QuerySBQuoteDlg.h
  �������Ȩ:�������ӹɷ����޹�˾
  ϵͳ����:  Ͷ��Ӯ�ҽ�������ն�V1.0
  ģ������:  TradeStock
  ����˵��:  ��ѯ����Э������
  ��    ��:  shenglq
  ��������:  20110323
*********************************************************/
#pragma once
#include "TradeQueryBaseDlg.h"

class CQuerySBQuoteDlg : public CTradeQueryBaseDlg
{
	DECLARE_DYNAMIC(CQuerySBQuoteDlg)
public:
	CQuerySBQuoteDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CQuerySBQuoteDlg();
	//virtual DWORD GetDataType();
protected:
	//virtual void DoGridDBClick();
	virtual void DoDataCore(IDataList * pDataList);
private:
};