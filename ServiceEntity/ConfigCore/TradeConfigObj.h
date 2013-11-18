/*******************************************************
  Դ��������:TradeConfigObj.h
  �������Ȩ:�������ӹɷ����޹�˾
  ϵͳ����:  Ͷ��Ӯ�ҽ�������ն�V1.0
  ģ������:  ���׽�������
  ����˵��:  
  ��    ��:  shenglq
  ��������:  20100720
*********************************************************/
#pragma once
#include "TradeConfigObjIntf.h"
#include "TradeTables.h"
#include "TradeSysConfig.h"
class CTradeConfigObj : public ITradeConfigObj
{
public:
	CTradeConfigObj();
	~CTradeConfigObj();
	void Release();
	//��ȡӪҵ���б�
	ICellList* GetCellList();
	//���ݹ��ܺŻ�ȡ��ͷ��Ϣ
	ITradeTable* GetTradeTableData(const int iAction);
	//��ȡ�����ֵ�
	ITradeDict* GetTradeDict(CString sName);
	//���ݹ��ܺŻ�ȡ�������
	INodeTreeData* GetReqParamList(const int iAction);
	//��ȡ�������ڵ�
	TiXmlElement* GetFunctionTree(CString sCellID, CString sAccount = "");
	//��ȡ�ʺŲ����б�
	ICCommParamList* GetAccountParamList(CString sCellID, CString sAccount);
	//TiXmlNode* GetMemTablesRoot();
private:
	CTradeTables* m_pTradeTables;
	CTradeConfigDef* m_pConfigDef;
	CTradeConfigCell* m_pConfigCell;
	CTradeConfigAccount* m_pConfigAccount;
	BOOL LoadTradeSysConfigs();
	BOOL LoadTradeTables();
};