/*******************************************************
  源程序名称:Ar1Packet.h
  软件著作权:恒生电子股份有限公司
  系统名称:  投资赢家金融理财终端V1.0
  模块名称:  交易中心
  功能说明:  Ar1打包解包器
  作    者:  shenglq
  开发日期:  20100830
*********************************************************/
#pragma once

#include <afxtempl.h>
#include <afxcoll.h>


#define	SOH char(1)

#pragma pack(push) //保存对齐状态
#pragma pack(1)//设定为1字节对齐
#define PacketAR1Size sizeof(AR1PacketHeader)

struct AR1PacketHeader 
{
	char HeaderVersion;		// 包头版本号，目前必须为1
	char PacketType;		// 0-请求，1-应答
	short	Branch;			// 开户网点号，0-9999
	WORD	FunctionNo;		// 功能号，0-9999
	int	SenderID;		// 发送方标识，原样返回
	char AddressLevel;		// 地址层数，目前限制最大为32
	int	DataLength;		// 数据区长度(不含地址表)
	int	ReturnCode;		// 返回码（只对应答有效），0表示成功
};
#pragma pack(pop)//恢复对齐状态


class CAr1Packer
{
public:
	CAr1Packer();
	~CAr1Packer();
	AR1PacketHeader* GetHeader();
	void AddData(const CString sField, const CString sValue);
	char* GetPackBuf(int& iLen);
private:
	CMapStringToString m_Ar1DataList;
	AR1PacketHeader* m_Header;
};

typedef CTypedPtrArray<CPtrArray, CMapStringToString*> CAr1DataList;
class CAr1UnPacker
{
public:
	CAr1UnPacker();
	~CAr1UnPacker();
	BOOL open(char* lpBuf, int iLen);
    AR1PacketHeader* GetHeader();
	BOOL isEof();
	void Next();
	CMapStringToString* GetItem();
	int GetRowCount();
	int GetColCount();
protected:
private:
	void Clear();
	CAr1DataList m_Ar1DataList;
	AR1PacketHeader* m_Header;
	int m_CurrRow;
	int m_nRowCount;
	int m_nColCount;
};

class CAr1Packet
{
public:
	CAr1Packet();
	~CAr1Packet();
	CAr1Packer* GetAr1Packer();
	CAr1UnPacker* GetAr1UnPacker(char* lpBuf, int iLen);
	void FreePacker();
	void FreeUnPacker();
private:
	CAr1Packer* m_Ar1Packer;
	CAr1UnPacker* m_Ar1UnPacker;
};
