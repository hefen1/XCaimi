#pragma once
#include "NetEngineJYAPI.h"

enum EWholePacketHeader {phAction, phCellIndex, phOnlineID, phOnlineToken, phReturnCode, phClientAddr, phSenderID};
class CTradePacker
{
public:
	CTradePacker();
	virtual ~CTradePacker();
	int ParseData(const char *pData,UINT len);
	int GetHeader(EWholePacketHeader index, UINT &ivalue);
	int SetHeader(EWholePacketHeader index, UINT ivalue);
	char* GetPackData(UINT &len);
	char* GetData(CString sName, UINT& iDataLen);

private:
	HJC_PACKET_MANAGER m_hPM;
};