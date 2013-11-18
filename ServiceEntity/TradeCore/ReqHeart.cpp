/*******************************************************
  Դ��������:ReqHeart.cpp
  �������Ȩ:�������ӹɷ����޹�˾
  ϵͳ����:  Ͷ��Ӯ�ҽ�������ն�V1.0
  ģ������:  ��������
  ����˵��:  �����ӿ�ʵ��
  ��    ��:  shenglq
  ��������:  20100720
*********************************************************/
#include "stdafx.h"
#include "ReqHeart.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CString CReqHeart::GetVersion()
{
	int iDataLen;
	char* pTmpBuf = CTrading::GetBinData("version", iDataLen);
	char* pName = new char[iDataLen + 1];
	memcpy(pName, pTmpBuf, iDataLen);
	memcpy(pName + iDataLen, "\0", 1);
	CString sName = pName;
	delete[] pName;
	pName = NULL;
	delete[] pTmpBuf;
	pTmpBuf = NULL;
	return sName;
}

int CReqHeart::GetClientAdd()
{
	return CTrading::GetHeader(phClientAddr);
}

CString CReqHeart::GetJrName()
{
	return CTrading::GetBinStr("jrid");
}

CString CReqHeart::GetUpdateCrc()
{
	return CTrading::GetBinStr("updatestr_crc");
}

int CReqHeart::GetOnlineNums()
{
	return _ttoi(CTrading::GetBinStr("onlineNums"));
}

int CReqHeart::GetMaxNums()
{
	return _ttoi(CTrading::GetBinStr("maxnums"));
}