/*******************************************************
  Դ��������:TradeConfigObjIntf.h
  �������Ȩ:�������ӹɷ����޹�˾
  ϵͳ����:  Ͷ��Ӯ�ҽ�������ն�V1.0
  ģ������:  ���׽�������
  ����˵��:  
  ��    ��:  shenglq
  ��������:  20100720
*********************************************************/
#pragma once
#include "TradeTablesIntf.h"
#include "TradeSysConfigIntf.h"

#define OBJ_TRADECONFIG			_T("ConfigCore.TradeConfigObj")

struct ITradeConfigObj
{
	//�ͷ�
	virtual void Release() = 0;
	//��ȡӪҵ���б�
	virtual ICellList* GetCellList() = 0;
	//virtual TiXmlNode* GetMemTablesRoot() = 0; 
	//���ݹ��ܺŻ�ȡ��ͷ��Ϣ
	virtual ITradeTable* GetTradeTableData(const int iAction) = 0;
	//��ȡ�����ֵ�
	virtual ITradeDict* GetTradeDict(CString sName) = 0;
	////���ݽڵ����ƻ�ȡNV�б�
	//virtual INodeTreeData* GetNodeTreeData(CString sName, CString sCellID = "", CString sAccount = "") = 0;
	////��ȡ�Ϻ��м�ί������
	//virtual INodeTreeData* GetSHSJEntrustTypeList(CString sCellID = "", CString sAccount = "") = 0;
	////��ȡ�����м�ί������
	//virtual INodeTreeData* GetSZSJEntrustTypeList(CString sCellID = "", CString sAccount = "") = 0;
	////��ȡ�г����
	//virtual ITradeMarketList* GetTradeMarketList(CString sCellID = "", CString sAccount = "") = 0;
	////��ȡ�˺����
	//virtual ITradeAccountTypeList* GetTradeAccountTypeList(CString sCellID = "", CString sAccount = "") = 0;
	////��ȡ�޶����
	//virtual INodeTreeData* GetExceedFlagList(CString sCellID = "", CString sAccount = "") = 0;
	////��ȡ�շѷ�ʽ
	//virtual INodeTreeData* GetChargeTypeList(CString sCellID = "", CString sAccount = "") = 0;
	////��ȡ�ֺ췽ʽ
	//virtual INodeTreeData* GetDividendMethodList(CString sCellID = "", CString sAccount = "") = 0;
	////��ȡ��������
	//virtual INodeTreeData* GetMoneyTypeList(CString sCellID = "", CString sAccount = "") = 0;
	////��ȡ�޸���������
	//virtual IModifyPassowrdTypeList* GetModifyPassowrdTypeList(CString sCellID = "", CString sAccount = "") = 0;
	////��ȡ��Ʊ�������
	//virtual INodeTreeData* GetStockEntrustType(CString sCellID = "", CString sAccount = "") = 0;
	////��ȡ֤������
	//virtual INodeTreeData* GetIDKindList(CString sCellID = "", CString sAccount = "") = 0;
	////��ȡί��״̬
	//virtual INodeTreeData* GetEntrustStatusList(CString sCellID = "", CString sAccount = "") = 0;
	////��ȡĬ�������б�
	//virtual INodeTreeData* GetDefaultReqDataList(CString sCellID = "", CString sAccount = "") = 0;
	//���ݹ��ܺŻ�ȡ�������
	virtual INodeTreeData* GetReqParamList(const int iAction) = 0;
	//��ȡ�������ڵ�
	virtual TiXmlElement* GetFunctionTree(CString sCellID, CString sAccount = "") = 0;
	//��ȡ�ʺŲ����б�
	virtual ICCommParamList* GetAccountParamList(CString sCellID, CString sAccount) = 0;
};