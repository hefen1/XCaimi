/*******************************************************
  源程序名称:PackEntry.h 
  软件著作权:恒生电子股份有限公司
  系统名称:  投资赢家金融理财终端V1.0
  模块名称:  交易中心
  功能说明:  打包解包器
  作    者:  shenglq
  开发日期:  20100720
*********************************************************/
#pragma once
#include "DLLEntryBase.h"
/*
#include "as_packet.h"
#include "as_ResultSet.h"
#include "hs_interface.h"
*/

#define S_BIN_DATA			"hsreq" // 二进制数据名
#define S_BIN_DATA_ANS	    "hsans" // 二进制数据名
#define S_BIN_REQ			"jyreq"  // 二进制数据名
#define S_BIN_ANS			"jyans"  // 二进制数据名
#define S_NV_ReqSerialNo	"reqserialno" //请求流水号

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
	// 报文头读取、设置
	BOOL GetHeader(const PACKET_HANDLE hPM, const EWholePacketHeade iIndex, unsigned int& iValue);
	BOOL SetHeader(const PACKET_HANDLE hPM, const EWholePacketHeade iIndex, unsigned int iValue);
	// 获取出错信息
	CString GetLastError();
	// 解析报文
	BOOL ParseData(const PACKET_HANDLE hPM, char * lpPackData, const int iDataLen);
	// 报文内容读取、设置
	char* GetData(const PACKET_HANDLE hPM, const CString sName, int& iDataLen);
	BOOL SetData(const PACKET_HANDLE hPM, const EPacketType eType, const CString sName, char* lpData, int iDataLen);
	BOOL DataExist(const PACKET_HANDLE hPM, const CString sName);
	//获取报文
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