/*******************************************************
  Դ��������:Ar1Packet.h
  �������Ȩ:�������ӹɷ����޹�˾
  ϵͳ����:  Ͷ��Ӯ�ҽ�������ն�V1.0
  ģ������:  ��������
  ����˵��:  Ar1��������
  ��    ��:  shenglq
  ��������:  20100830
*********************************************************/
#pragma once

#include <afxtempl.h>
#include <afxcoll.h>


#define	SOH char(1)

#pragma pack(push) //�������״̬
#pragma pack(1)//�趨Ϊ1�ֽڶ���
#define PacketAR1Size sizeof(AR1PacketHeader)

struct AR1PacketHeader 
{
	char HeaderVersion;		// ��ͷ�汾�ţ�Ŀǰ����Ϊ1
	char PacketType;		// 0-����1-Ӧ��
	short	Branch;			// ��������ţ�0-9999
	WORD	FunctionNo;		// ���ܺţ�0-9999
	int	SenderID;		// ���ͷ���ʶ��ԭ������
	char AddressLevel;		// ��ַ������Ŀǰ�������Ϊ32
	int	DataLength;		// ����������(������ַ��)
	int	ReturnCode;		// �����루ֻ��Ӧ����Ч����0��ʾ�ɹ�
};
#pragma pack(pop)//�ָ�����״̬


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
