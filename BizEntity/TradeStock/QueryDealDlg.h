/*******************************************************
  Դ��������:QueryDealDlg.h
  �������Ȩ:�������ӹɷ����޹�˾
  ϵͳ����:  Ͷ��Ӯ�ҽ�������ն�V1.0
  ģ������:  TradeStock
  ����˵��:  ��ѯ���ճɽ�
  ��    ��:  shenglq
  ��������:  20100910
*********************************************************/
#pragma once
#include "TradeQueryBaseDlg.h"

class CQueryDealDlg : public CTradeQueryBaseDlg
{
	DECLARE_DYNAMIC(CQueryDealDlg)
public:
	CQueryDealDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CQueryDealDlg();
	virtual DWORD GetDataType();
protected:
	virtual void DoDataCore(IDataList * pDataList);
private:
};