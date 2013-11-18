/*******************************************************
  Դ��������:TradeTablesIntf.h
  �������Ȩ:�������ӹɷ����޹�˾
  ϵͳ����:  Ͷ��Ӯ�ҽ�������ն�V1.0
  ģ������:  ��������
  ����˵��:  ���ײ�ѯ�������ͷ�ӿڶ���
  ��    ��:  shenglq
  ��������:  20100926
*********************************************************/
#pragma once
#include "XMLDataBaseIntf.h"
#include "TradeConst.h"

struct ITradeDict: virtual public INodeTreeData
{
	virtual CString GetValueByName(CString sName) = 0;
};
//////////////////////////////////////////////////////////////////////////

struct ITradeColumn
{
	virtual void Release() = 0;
	virtual CString GetValue(CString sPropName) = 0;
	//��ȡ����
	virtual CString GetColumnName() = 0;
	//��ȡ��ʾ����
	virtual CString GetDispName() = 0;
	//����
	virtual EAlign GetAlign() = 0;
	//��ȡ�����ֵ�
	virtual ITradeDict* GetTradeDict() = 0;
	virtual CString GetDictValue(CString sName) = 0;

};
//////////////////////////////////////////////////////////////////////////

struct ITradeTable: virtual public INodeData
{
	virtual int GetCount() = 0;
	virtual ITradeColumn* GetItems(const int iIndex) = 0;
	virtual ITradeColumn* GetItemsByName(const CString sName) = 0;
};
//////////////////////////////////////////////////////////////////////////
struct ITradeTables : virtual public INodeDataList
{
	virtual ITradeTable* GetItems(const int iIndex) = 0;
	virtual ITradeTable* GetItemsByName(const CString sName) = 0;
	//�������ƻ�ȡ�����ֵ�
	virtual ITradeDict* GetTradeDictByName(CString sName) = 0;
	//���ݹ��ܺŻ�ȡ�������
	virtual INodeTreeData* GetReqParamByAction(const int nAction) = 0;
};