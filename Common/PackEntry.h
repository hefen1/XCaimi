/*******************************************************
  Դ��������:PackEntry.h 
  �������Ȩ:�������ӹɷ����޹�˾
  ϵͳ����:  Ͷ��Ӯ�ҽ�������ն�V1.0
  ģ������:  ��������
  ����˵��:  ��������
  ��    ��:  shenglq
  ��������:  20100720
*********************************************************/
#pragma once
#include "DLLEntryBase.h"
/*
#include "as_packet.h"
#include "as_ResultSet.h"
#include "hs_interface.h"
*/

#define S_BIN_DATA			"hsreq" // ������������
#define S_BIN_DATA_ANS	    "hsans" // ������������
#define S_BIN_REQ			"jyreq"  // ������������
#define S_BIN_ANS			"jyans"  // ������������
#define S_NV_ReqSerialNo	"reqserialno" //������ˮ��

enum EPacketType {ptNone, ptNV, ptCompose, ptBin};
enum EWholePacketHeade {phAction, phCellIndex, phOnlineID, phOnlineToken, phReturnCode, phClientAddr, phSenderID};

typedef int	PACKET_HANDLE;

class CPackEntry : public CDLLEntryBase
{
public:
	CPackEntry();
	~CPackEntry();
	PACKET_HANDLE CreatePM();
	void DestroyPM(const PACKET_HANDLE hPM);
	// ����ͷ��ȡ������
	BOOL GetHeader(const PACKET_HANDLE hPM, const EWholePacketHeade iIndex, unsigned int& iValue);
	BOOL SetHeader(const PACKET_HANDLE hPM, const EWholePacketHeade iIndex, unsigned int iValue);
	// ��ȡ������Ϣ
	CString GetLastError();
	// ��������
	BOOL ParseData(const PACKET_HANDLE hPM, char * lpPackData, const int iDataLen);
	// �������ݶ�ȡ������
	char* GetData(const PACKET_HANDLE hPM, const CString sName, int& iDataLen);
	BOOL SetData(const PACKET_HANDLE hPM, const EPacketType eType, const CString sName, char* lpData, int iDataLen);
	BOOL DataExist(const PACKET_HANDLE hPM, const CString sName);
	//��ȡ����
    char* GetPackData(const PACKET_HANDLE hPM, int& iDataLen);
	int GetNVCount(const PACKET_HANDLE hPM);
	char* GetNVNameByIndex(const PACKET_HANDLE hPM, const int iIndex, int& iDataLen, int& iType);
protected:
	CString GetDLLFileName();
private:
	void Error();
};
/*

class CAr2PackEntry : public CDLLEntryBase
{
public:
	CAr2PackEntry();
	~CAr2PackEntry();
	IPackService * GetPackServiceInstance();
protected:
	CString GetDLLFileName();
private:
};*/



class CPacker
{
public:
	CPacker();
	~CPacker();
	unsigned int GetHeader(const EWholePacketHeade iIndex);
	BOOL SetHeader(const EWholePacketHeade iIndex, unsigned int iValue);
	CString GetLastError();
	BOOL ParseData(char* lpPackData, int iDataLen);
	char* GetData(CString sName, int& iDataLen);
	BOOL SetData(const EPacketType eType, const CString sName, char* lpData, int iDataLen);
	BOOL DataExist(const CString sName);
	char* GetPackData(int& iDataLen);
	int GetNVCount();
	CString GetNVNameByIndex(const int iIndex, int& iType);
	CString GetStr(CString sName);
protected:
private:
	PACKET_HANDLE m_hPM;
	CPackEntry* m_lpPackEntry;	
};