/*******************************************************
  Դ��������:TradeDataBase.h
  �������Ȩ:�������ӹɷ����޹�˾
  ϵͳ����:  Ͷ��Ӯ�ҽ�������ն�V1.0
  ģ������:  ��������
  ����˵��:  �������ݻ����ඨ��
  ��    ��:  shenglq
  ��������:  20100720
*********************************************************/
#pragma once
#include <afxtempl.h>
#include <afxcoll.h>
#include "TradeDataBaseIntf.h"


//class CColumn : public IColumn
//{
//public:
//	CColumn();
//	virtual ~CColumn();
//	//��ȡ����
//	CString GetColumnName();
//	void SetColumnName(const CString Value);
//	//��ȡ��ʾ����
//	CString GetDispName(); 
//	void SetDispName(const CString Value);
//	//��ȡ�����ֵ����
//	CString GetDataDict();
//	void SetDataDict(const CString Value);
//	//��ʾ��ʽ
//	CString GetDispFormat();
//	void SetDispFormat(const CString Value);
//	//��ȡ�ϼ�����
//	ESumType GetSumType(); 
//	void SetSumType(const ESumType Value);
//	//�Ƿ�����ֶ�
//	BOOL GetIsCalc();
//	void SetIsCalc(const BOOL Value);
//	//����
//	EAlign GetAlign();
//	void SetAlign(const EAlign Value);
//protected:
//
//private:
//	CString m_sColumnName;
//	CString m_sDispName;
//	CString m_sDataDict;
//	CString m_sDispFormat;
//	ESumType m_eSumType;
//	BOOL m_bIsCalc;
//	EAlign m_eAlign;
//};

//typedef CTypedPtrArray<CPtrArray, CColumn*> CColumnList;
//
////��Ҫ��ʾ�ı��ֶ���Ϣ
//class CColumns : public IColumns
//{
//public:
//	CColumns();
//	virtual~CColumns();
//	void LoadColumns(const int iAction);
//	//����
//	int GetCount();
//	//����������ȡ��
//	IColumn * GetColumns(const int i);	
//	//�������ƻ�ȡ��
//	IColumn * GetColumnByName(const CString sName);
//	//��ȡ��������
//	CString GetLinkTable();
//	//��ȡĬ������������
//	int GetDefaultSortIndex();
//	//��ȡĬ�������з���
//	ESortDirection GetDefaultSortDirction();
//protected:
//
//private:
//	CColumnList m_ColumnList;
//	CString m_sLinkTable;
//	int m_iDefaultSortIndex;
//	ESortDirection m_eDefaultSortDirction;
//};


//NV��
//class CNVData : public CObject, public INVData
class CNVData
{
public:	
	CNVData();
	virtual ~CNVData();
	CString GetName();
	void SetName(const CString Value);
	CString GetValue();
	void SetValue(const CString Value);
	double GetDouble(const double dDef);
	int GetInt(const int iDef);
	long GetLong(const int iDef);
protected:

private:
	CString m_sName;
	CString m_sValue;
};

typedef CTypedPtrArray<CPtrArray, CNVData*> CNVDataList;
// ÿ�����ݷ��ʽӿ�
class CDataItem : virtual public IDataItem
{
public:	
	CDataItem();
	virtual ~CDataItem();
	//virtual INVData * AddField();
	virtual void DeleteField(const int i);
	virtual void DeleteFieldByName(const CString sName); 
	virtual CString GetStr(const int i); 
	virtual char GetChar(const int i); 
	virtual int GetInt(const int i, const int iDef = 0);
	virtual long GetLong(const int i, const int iDef = 0);
	virtual double GetDouble(const int i, const double dDef = 0);
	//COleCurrency GetCurrency(const int i, COleCurrency iDef);
	virtual CString GetStrByName(const CString sName); 
	virtual char GetCharByName(const CString sName); 
	virtual int GetIntByName(const CString sName, const int iDef = 0);
	virtual long GetLongByName(const CString sName, const int iDef = 0);
	virtual double GetDoubleByName(const CString sName, const double dDef = 0);
	//COleCurrency GetCurrencyByName(const CString sName, COleCurrency iDef);
	//��ȡ�����
	virtual int GetErrorNo();
	//��ȡ������Ϣ
	virtual CString GetErrorInfo(); 
	//��ȡί�к�
	virtual CString GetEntrustNo();

	virtual void SetData(const CString sName, const CString sValue);

	virtual CString GetSerialNo();
	void AddData(const CString sName, const CString sValue);
	virtual int GetCount();
	virtual void GetFieldAndValue(const int i, CString &sField, CString &sValue);
protected:
private:
	int GetIndexByName(const CString sName);
	CNVDataList m_NVDataList;
};

typedef CTypedPtrArray<CPtrArray, IDataItem*> CDataItemList;
//���ݼ�
class CDataList : virtual public IDataList
{
public:
	CDataList();
	virtual ~CDataList();
	//�ͷ�
	void Release();
	int GetCount();
	////�������ļ����ر�ͷ����
	//void LoadColumns();
	//// ��ȡ����Ϣ
	//virtual IColumn * GetColumns(const int i);
	////��ȡ����
	//virtual int GetColumnCount();
	//����������ȡһ����¼   
	virtual IDataItem * GetItems(const int i);
	//����һ����¼
	virtual void AddItem(IDataItem * pItem);
	//����һ����¼
	virtual void InsertItem(const int iIndex, IDataItem * pItem);
	//ɾ��һ����¼
	virtual void DeleteItem(const int iIndex);
	virtual IDataItem * NewItem();
	virtual void Lock();
	virtual void UnLock();
	virtual IDataList* Clone();
protected:
	virtual IDataList* NewDataList();//��Ҫ��¡��������    
private:	
	//CColumns* m_pColumns;
	CDataItemList m_DataItemList;
	CRITICAL_SECTION m_cs;
};
