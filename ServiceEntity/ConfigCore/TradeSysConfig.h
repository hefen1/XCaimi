/*******************************************************
  Դ��������:TradeSysConfig.h
  �������Ȩ:�������ӹɷ����޹�˾
  ϵͳ����:  Ͷ��Ӯ�ҽ�������ն�V1.0
  ģ������:  ��������
  ����˵��:  ����ϵͳ����
  ��    ��:  shenglq
  ��������:  20101012
*********************************************************/
#pragma once
#include "XMLDataBase.h"
#include "TradeSysConfigIntf.h"
#pragma warning (disable:4250)

//////////////////////////////////////////////////////////////////////////
class CTradeMarketItem : public CNodeData, public ITradeMarketItem
{
public:
	CTradeMarketItem();
	virtual ~CTradeMarketItem();

	CString GetMarketName();
	CString GetDispName();
	CString GetAccountType();
	EMoneyType GetMoneyType();
};
//////////////////////////////////////////////////////////////////////////
class CTradeMarketList : public CNodeTreeData, public ITradeMarketList
{
public:
	CTradeMarketList();
	virtual ~CTradeMarketList();
	ITradeMarketItem* GetItems(const int iIndex);
	ITradeMarketItem* GetItemsByName(const CString sName);
protected:
	virtual ITradeMarketItem* GetItemsFull(CXML xPath, int iIndex = -1);
};
///////////////////////////////////////////////////////////////////////////
//�˺����������
class CTradeAccountTypeItem : public CNodeData, public ITradeAccountTypeItem
{
public:
	CTradeAccountTypeItem();
	virtual ~CTradeAccountTypeItem();
	CString GetAccountTypeName();
	CString GetAccountType();
	CString GetBackType();
	CString GetShortName();
	CString GetMarketType();
	CString GetVisibleName();
};

class CTradeAccountTypeList : public CNodeTreeData, public ITradeAccountTypeList
{
public:
	CTradeAccountTypeList();
	virtual ~CTradeAccountTypeList();
	ITradeAccountTypeItem* GetItems(const int iIndex);
	ITradeAccountTypeItem* GetItemsByName(const CString sName);
	CString GetBackTypeByAccountType(const CString sName);
	CString GetMarketTypeByAccountType(const CString sName);
protected:
	virtual ITradeAccountTypeItem* GetItemsFull(CXML xPath, int iIndex = -1);
};

///////////////////////////////////////////////////////////////////////////////////
class CModifyPassowrdTypeItem : public CNodeData, public IModifyPassowrdTypeItem
{
public:
	CModifyPassowrdTypeItem();
	virtual ~CModifyPassowrdTypeItem();
	CString GetOldCharType();
	int GetOldMaxLen();
	int GetOldMinLen();
	CString GetNewCharType();
	int GetNewMaxLen();
	int GetNewMinLen();
};
//////////////////////////////////////////////////////////////////////////
class CModifyPassowrdTypeList : public CNodeTreeData, public IModifyPassowrdTypeList
{
public:
	CModifyPassowrdTypeList();
	virtual ~CModifyPassowrdTypeList();
	IModifyPassowrdTypeItem* GetItems(const int iIndex);
	IModifyPassowrdTypeItem* GetItemsByName(const CString sName);
protected:
	virtual IModifyPassowrdTypeItem* GetItemsFull(CXML xPath, int iIndex = -1);
};

//////////////////////////////////////////////////////////////////////////
//��������ʾ�
class CFundRiskQuestionItem : public CNodeData, public IFundRiskQuestionItem
{
public:
	CFundRiskQuestionItem();
	~CFundRiskQuestionItem();
	CString GetCode();
	CString GetTitle();
	CString GetChoice();
	CString GetChoiceText();
	int GetChoiceScore();
	CString GetRemark();
protected:
	virtual CString GetIDPropName();
};
class CFundRiskQuestionList : public CNodeTreeData, public IFundRiskQuestionList
{
public:
	CFundRiskQuestionList();
	~CFundRiskQuestionList();
	IFundRiskQuestionItem* GetItems(const int iIndex);
	IFundRiskQuestionItem* GetItemsByName(const CString sName);
protected:
	virtual IFundRiskQuestionItem* GetItemsFull(CXML xPath, int iIndex = -1);
};
//////////////////////////////////////////////////////////////////////////
//����������
class CCommParamList : public CNodeTreeData, virtual public ICCommParamList
{
public:	
	CCommParamList();
	virtual ~CCommParamList();
	virtual CXML GetListPath();
	void SetNextParamList(ICCommParamList* pParamlist);
	void SetCellAndAccount(const CString sCellID, const CString sAccount);
	//��ȡĬ�������б�
	INodeTreeData* GetDefaultReqDataList(CString sID = "");
	//��ȡ�˺����
	ITradeAccountTypeList* GetAccountTypeList(const CString sID = "");
	//���ݽڵ����ƻ�ȡNV�б�
	INodeTreeData* GetNVListByName(const CString sNodeName, const CString sID = "");
	//��ȡ�޸���������
	IModifyPassowrdTypeList* GetModifyPassowrdTypeList();
	//��ȡ�г����
	ITradeMarketList* GetMarketList();	
	//��ȡ�Ϻ��м�ί������
	INodeTreeData* GetSHSJEntrustTypeList();
	//��ȡ�����м�ί������
	INodeTreeData* GetSZSJEntrustTypeList();
	//��ȡ�޶����
	INodeTreeData* GetExceedFlagList();
	//��ȡ�շѷ�ʽ
	INodeTreeData* GetChargeTypeList();
	//��ȡ�ֺ췽ʽ
	INodeTreeData* GetDividendMethodList();
	//��ȡ��������
	INodeTreeData* GetMoneyTypeList();
	//��ȡ��Ʊ�������
	INodeTreeData* GetStockEntrustType();
	//��ȡ֤������
	INodeTreeData* GetIDKindList();
	//��ȡί��״̬
	INodeTreeData* GetEntrustStatusList();
	//��ȡ������ղ����ʾ�
	IFundRiskQuestionList* GetFundRiskQuestionList();
	//��ȡ�û����յȼ�
	INodeTreeData* GetFundAccRiskTypes();
	
protected:
	CString m_sCellID;
	CString m_sAccount;
	ICCommParamList* m_pNextParamList;
	CString m_sID;
private:
	TiXmlNode* GetChildNodeByIDName(const CString sID, const CString sName);
};

//////////////////////////////////////////////////////////////////////////
class CCellItem : public CNodeData, public ICellItem
{
public:
	CCellItem();
	virtual ~CCellItem();
	CString GetCellID();
	CString GetCaption();
	CString GetType();
	CString GetTypeName();
	int GetServerID();
	CString GetBranchNo();
	TiXmlElement* GetFunctionTree();
protected:
	virtual CString GetIDPropName();
};
//ȯ�̲�����
class CCellList : public CCommParamList, public ICellList
{
public:
	CCellList();
	virtual ~CCellList();
	virtual CXML GetListPath();
	ICellItem* GetItems(const int iIndex);
	ICellItem* GetItemsByName(const CString sName);
protected:
	virtual CXML GetChildNodeName();
	virtual CString GetIDPropName();
	virtual ICellItem* GetItemsFull(CXML xPath, int iIndex = -1);
};

//////////////////////////////////////////////////////////////////////////

class CAccountConfigItem : public CNodeData, public IAccountConfigItem
{
public:
	CAccountConfigItem();
	virtual ~CAccountConfigItem();
	CString GetID();
protected:
	virtual CString GetIDPropName();
};
//�ʺŲ�����
class CAccountConfigList : public CCommParamList, public IAccountConfigList
{
public:
	CAccountConfigList();
	virtual ~CAccountConfigList();
	virtual CXML GetListPath();
	IAccountConfigItem* GetItems(const int iIndex);
	IAccountConfigItem* GetItemsByName(const CString sName);
protected:
	virtual CXML GetChildNodeName();
	virtual CString GetIDPropName();
	virtual IAccountConfigItem* GetItemsFull(CXML xPath, int iIndex = -1);
};

class CTradeConfigDef : public CNodeDataList
{
public:
	CTradeConfigDef();
	virtual~CTradeConfigDef();
	ICCommParamList* GetDefaultConfigList();
protected:
	virtual UINT GetIDResource();
};

class CTradeConfigCell : public CNodeDataList
{
public:
	CTradeConfigCell();
	virtual~CTradeConfigCell();
	ICellList* GetCellList();
protected:
	virtual CString GetFileName();
};

class CTradeConfigAccount : public CNodeDataList
{
public:
	CTradeConfigAccount();
	virtual~CTradeConfigAccount();
	IAccountConfigList* GetAccountConfigList();
protected:
	virtual CString GetFileName();
};

