/*******************************************************
  Դ��������:TradeDataBaseIntf.h
  �������Ȩ:�������ӹɷ����޹�˾
  ϵͳ����:  Ͷ��Ӯ�ҽ�������ն�V1.0
  ģ������:  ��������
  ����˵��:  �������ݻ����ӿ�
  ��    ��:  shenglq
  ��������:  20100720
*********************************************************/
#pragma once
#include "TradeConst.h"

//               �ޣ�  ͳ�ƣ� ��ͣ����ݱ�����ͣ������г����
enum ESumType {stNone, stCount, stSum, stMoneySum,stMarketSum};
//                   ���򣬽���
enum ESortDirection {sdASC, sdDESC};
//                            ��      ��
enum EBSDirection {bsNone, bsBuy, bsSell};


//struct IColumn : IXMLPropAccess
//struct IColumn
//{
//	virtual ~IColumn() = 0;
//	//��ȡ����
//	virtual CString GetColumnName() = 0;
//	//��ȡ��ʾ����
//	virtual CString GetDispName() = 0; 
//	//��ȡ�����ֵ����
//	virtual CString GetDataDict() = 0;
//	//��ʾ��ʽ
//	virtual CString GetDispFormat() = 0;
//	//��ȡ�ϼ�����
//	virtual ESumType GetSumType() = 0;  
//	//�Ƿ�����ֶ�
//	virtual BOOL GetIsCalc() = 0; 
//	//����
//	virtual EAlign GetAlign() = 0;
//};
//
////��Ҫ��ʾ�ı��ֶ���Ϣ
////struct IColumns : INodeData
//struct IColumns
//{
//	virtual ~IColumns() = 0;
//	//����
//	virtual int GetCount() = 0;
//	//����������ȡ��
//	virtual IColumn * GetColumns(const int i) = 0;	
//	//�������ƻ�ȡ��
//	virtual IColumn * GetColumnByName(const CString sName) = 0;
//	//��ȡ��������
//	virtual CString GetLinkTable() = 0;
//	//��ȡĬ������������
//	virtual int GetDefaultSortIndex() = 0;
//	//��ȡĬ�������з���
//	virtual ESortDirection GetDefaultSortDirction() = 0;
//};



/*
//NV��
struct INVData
{
	virtual ~INVData() = 0;
	virtual CString GetName() = 0;
	virtual void SetName(const CString Value) = 0;
	virtual CString GetValue() = 0;
	virtual void SetValue(const CString Value) = 0;
};*/


// ÿ�����ݷ��ʽӿ�
struct IDataItem
{
	virtual ~IDataItem() = 0;
	//virtual INVData * AddField() = 0;
	virtual void DeleteField(const int i) = 0;
	virtual void DeleteFieldByName(const CString sName) = 0; 
	virtual CString GetStr(const int i) = 0; 
	virtual char GetChar(const int i) = 0; 
	virtual int GetInt(const int i, const int iDef) = 0;
	virtual long GetLong(const int i, const int iDef) = 0;
	virtual double GetDouble(const int i, const double dDef) = 0;
	//virtual COleCurrency GetCurrency(const int i, COleCurrency iDef) = 0;
	virtual CString GetStrByName(const CString sName) = 0; 
	virtual char GetCharByName(const CString sName) = 0; 
	virtual int GetIntByName(const CString sName, const int iDef) = 0;
	virtual long GetLongByName(const CString sName, const int iDef) = 0;
	virtual double GetDoubleByName(const CString sName, const double dDef) = 0;
	//virtual COleCurrency GetCurrencyByName(const CString sName, COleCurrency iDef) = 0;
	//��ȡ�����
	virtual int GetErrorNo() = 0;
	//��ȡ������Ϣ
	virtual CString GetErrorInfo() = 0; 
	//��ȡί�к�
	virtual CString GetEntrustNo() = 0;

	virtual void SetData(const CString sName, const CString sValue) = 0;
	//��ȡί����ˮ��
	virtual CString GetSerialNo() = 0;
	virtual void AddData(const CString sName, const CString sValue) = 0;
	virtual int GetCount() = 0;
	virtual void GetFieldAndValue(const int i, CString &sField, CString &sValue) = 0;

};
//���ݼ�
struct IDataList
{
	//�ͷ�
	virtual void Release() = 0;
	virtual int GetCount() = 0;
	////�������ļ����ر�ͷ����
	//virtual void LoadColumns() = 0;
	//// ��ȡ����Ϣ
	//virtual IColumn * GetColumns(const int i) = 0;
	////��ȡ����
	//virtual int GetColumnCount() = 0;
	//����������ȡһ����¼   
	virtual IDataItem * GetItems(const int i) = 0;
	//����һ����¼
	virtual void AddItem(IDataItem * pItem) = 0;
	//����һ����¼
	virtual void InsertItem(const int iIndex, IDataItem * pItem) = 0;
	//ɾ��һ����¼
	virtual void DeleteItem(const int iIndex) = 0;
	//����һ����¼
	virtual IDataItem * NewItem() = 0;
	virtual void Lock() = 0;
	virtual void UnLock() = 0;
	virtual IDataList* Clone() = 0;
};
