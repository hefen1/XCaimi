/*******************************************************
  Դ��������:ReqHeart.h
  �������Ȩ:�������ӹɷ����޹�˾
  ϵͳ����:  Ͷ��Ӯ�ҽ�������ն�V1.0
  ģ������:  ��������
  ����˵��:  �����ඨ��
  ��    ��:  shenglq
  ��������:  20100720
*********************************************************/
#pragma once
#include "Trading.h"
#include "..\..\Common\TradeComm\ReqHeartIntf.h"
#pragma warning (disable:4250)

class CReqHeart : public CTrading, public IReqHeart
{
	CString GetVersion();
	int GetClientAdd();
	CString GetJrName();
	CString GetUpdateCrc();
	int GetOnlineNums();
	int GetMaxNums();
};