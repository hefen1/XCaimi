/*******************************************************
  Դ��������:Query7431Dlg.h
  �������Ȩ:�������ӹɷ����޹�˾
  ϵͳ����:  Ͷ��Ӯ�ҽ�������ն�V1.0
  ģ������:  TradeStock
  ����˵��:  ��ѯ����Ͷί��
  ��    ��:  shenglq
  ��������:  20110323
*********************************************************/
#pragma once
#include "TradeQueryBaseDlg.h"

class CQuery7431Dlg : public CTradeQueryBaseDlg
{
	DECLARE_DYNAMIC(CQuery7431Dlg)
public:
	CQuery7431Dlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CQuery7431Dlg();
	//virtual DWORD GetDataType();
protected:
	virtual void DoDataCore(IDataList * pDataList);
	virtual void DoPopupMenuEntrust();
private:
};