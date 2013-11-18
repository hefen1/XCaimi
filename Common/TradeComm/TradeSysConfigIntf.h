/*******************************************************
  Դ��������:TradeSysConfigIntf.h
  �������Ȩ:�������ӹɷ����޹�˾
  ϵͳ����:  Ͷ��Ӯ�ҽ�������ն�V1.0
  ģ������:  ��������
  ����˵��:  ����ϵͳ����
  ��    ��:  shenglq
  ��������:  20101012
*********************************************************/
#pragma once
#include "TradeConst.h"
#include "XMLDataBaseIntf.h"
#include "tinyxml.h"

//�г�����
struct ITradeMarketItem : virtual public INodeData
{
	//��ȡ��ʾ����
	virtual CString GetMarketName() = 0;
	virtual CString GetDispName() = 0;
	virtual CString GetAccountType() = 0;
	virtual EMoneyType GetMoneyType() = 0;
};

struct ITradeMarketList : virtual public INodeTreeData
{
	virtual ITradeMarketItem* GetItems(const int iIndex) = 0;
	virtual ITradeMarketItem* GetItemsByName(const CString sName) = 0;
};
//////////////////////////////////////////////////////////////////////////
//�˺�����
struct ITradeAccountTypeItem : virtual public INodeData
{
	virtual CString GetAccountTypeName() = 0;
	virtual CString GetAccountType() = 0;
	virtual CString GetBackType() = 0;
	virtual CString GetShortName() = 0;
	virtual CString GetMarketType() = 0;
	virtual CString GetVisibleName() = 0;
};

struct ITradeAccountTypeList : virtual public INodeTreeData
{
	virtual ITradeAccountTypeItem* GetItems(const int iIndex) = 0;
	virtual ITradeAccountTypeItem* GetItemsByName(const CString sName) = 0;

	virtual CString GetBackTypeByAccountType(const CString sName) = 0;
	virtual CString GetMarketTypeByAccountType(const CString sName) = 0;
};

//////////////////////////////////////////////////////////////////////////
//���޸���������
struct IModifyPassowrdTypeItem : virtual public INodeData
{
	//��ȡ��ʾ����
	virtual CString GetOldCharType() = 0;
	virtual int GetOldMaxLen() = 0;
	virtual int GetOldMinLen() = 0;
	virtual CString GetNewCharType() = 0;
	virtual int GetNewMaxLen() = 0;
	virtual int GetNewMinLen() = 0;
};

struct IModifyPassowrdTypeList : virtual public INodeTreeData
{
	virtual IModifyPassowrdTypeItem* GetItems(const int iIndex) = 0;
	virtual IModifyPassowrdTypeItem* GetItemsByName(const CString sName) = 0;
};

//////////////////////////////////////////////////////////////////////////
//��������ʾ�

struct IFundRiskQuestionItem : virtual public INodeData
{
	virtual CString GetCode() = 0;
	virtual CString GetTitle() = 0;
	virtual CString GetChoice() = 0;
	virtual CString GetChoiceText() = 0;
	virtual int GetChoiceScore() = 0;
	virtual CString GetRemark() = 0;
};
struct IFundRiskQuestionList : virtual public INodeTreeData
{
	virtual IFundRiskQuestionItem* GetItems(const int iIndex) = 0;
	virtual IFundRiskQuestionItem* GetItemsByName(const CString sName) = 0;
};


//��������
struct ICCommParamList: virtual public INodeTreeData
{
	virtual void SetNextParamList(ICCommParamList* pParamlist) = 0;
	virtual void SetCellAndAccount(const CString sCellID, const CString sAccount) = 0;
	//��ȡĬ�������б�
	virtual INodeTreeData* GetDefaultReqDataList(CString sID = "") = 0;
	//��ȡ�˺����
	virtual ITradeAccountTypeList* GetAccountTypeList(const CString sID = "") = 0;
	//���ݽڵ����ƻ�ȡNV�б�
	virtual INodeTreeData* GetNVListByName(const CString sNodeName, const CString sID = "") = 0;
	//��ȡ�޸���������
	virtual IModifyPassowrdTypeList* GetModifyPassowrdTypeList() = 0;
	//��ȡ�г����
	virtual ITradeMarketList* GetMarketList() = 0;	
	//��ȡ�Ϻ��м�ί������
	virtual INodeTreeData* GetSHSJEntrustTypeList() = 0;
	//��ȡ�����м�ί������
	virtual INodeTreeData* GetSZSJEntrustTypeList() = 0;
	//��ȡ�޶����
	virtual INodeTreeData* GetExceedFlagList() = 0;
	//��ȡ�շѷ�ʽ
	virtual INodeTreeData* GetChargeTypeList() = 0;
	//��ȡ�ֺ췽ʽ
	virtual INodeTreeData* GetDividendMethodList() = 0;
	//��ȡ��������
	virtual INodeTreeData* GetMoneyTypeList() = 0;
	//��ȡ��Ʊ�������
	virtual INodeTreeData* GetStockEntrustType() = 0;
	//��ȡ֤������
	virtual INodeTreeData* GetIDKindList() = 0;
	//��ȡί��״̬
	virtual INodeTreeData* GetEntrustStatusList() = 0;
	//��ȡ������ղ����ʾ�
	virtual IFundRiskQuestionList* GetFundRiskQuestionList() = 0;
	//��ȡ�û����յȼ�
	virtual INodeTreeData* GetFundAccRiskTypes() = 0;

};

//ȯ�̲�������
struct ICellItem : virtual public INodeData
{
	virtual CString GetCellID() = 0;
	virtual CString GetCaption() = 0;
	virtual CString GetType() = 0;
	virtual CString GetTypeName() = 0;
	virtual int GetServerID() = 0;
	virtual	CString GetBranchNo() = 0;
	virtual TiXmlElement* GetFunctionTree() = 0;
};

struct ICellList : virtual public ICCommParamList
{
	virtual ICellItem* GetItems(const int iIndex) = 0;
	virtual ICellItem* GetItemsByName(const CString sName) = 0;
};

//////////////////////////////////////////////////////////////////////////
//�ʺŲ�������
struct IAccountConfigItem : virtual public INodeData
{
	virtual CString GetID() = 0;
};

struct IAccountConfigList : virtual public ICCommParamList
{
	virtual IAccountConfigItem* GetItems(const int iIndex) = 0;
	virtual IAccountConfigItem* GetItemsByName(const CString sName) = 0;
};

//////////////////////////////////////////////////////////////////////////

