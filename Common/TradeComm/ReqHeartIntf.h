/*******************************************************
  Դ��������:ReqHeartIntf.h
  �������Ȩ:�������ӹɷ����޹�˾
  ϵͳ����:  Ͷ��Ӯ�ҽ�������ն�V1.0
  ģ������:  ��������
  ����˵��:  �����ӿڶ���
  ��    ��:  shenglq
  ��������:  20100720
*********************************************************/
#pragma once
#include "TradingIntf.h"

struct IReqHeart : virtual public ITrading
{
	virtual CString GetVersion() = 0;
	virtual int GetClientAdd() = 0;
	virtual CString GetJrName() = 0;
	virtual CString GetUpdateCrc() = 0;
	virtual int GetOnlineNums() = 0;
	virtual int GetMaxNums() = 0;
};