/*******************************************************
  源程序名称:TradeDataBase.h
  软件著作权:恒生电子股份有限公司
  系统名称:  投资赢家金融理财终端V1.0
  模块名称:  交易中心
  功能说明:  交易数据基础类定义
  作    者:  shenglq
  开发日期:  20100720
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
//	//获取列名
//	CString GetColumnName();
//	void SetColumnName(const CString Value);
//	//获取显示名称
//	CString GetDispName(); 
//	void SetDispName(const CString Value);
//	//获取数据字典表名
//	CString GetDataDict();
//	void SetDataDict(const CString Value);
//	//显示格式
//	CString GetDispFormat();
//	void SetDispFormat(const CString Value);
//	//获取合计类型
//	ESumType GetSumType(); 
//	void SetSumType(const ESumType Value);
//	//是否计算字段
//	BOOL GetIsCalc();
//	void SetIsCalc(const BOOL Value);
//	//对齐
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
////需要显示的表字段信息
//class CColumns : public IColumns
//{
//public:
//	CColumns();
//	virtual~CColumns();
//	void LoadColumns(const int iAction);
//	//列数
//	int GetCount();
//	//根据索引获取列
//	IColumn * GetColumns(const int i);	
//	//根据名称获取列
//	IColumn * GetColumnByName(const CString sName);
//	//获取关联表名
//	CString GetLinkTable();
//	//获取默认排序列索引
//	int GetDefaultSortIndex();
//	//获取默认排序列方向
//	ESortDirection GetDefaultSortDirction();
//protected:
//
//private:
//	CColumnList m_ColumnList;
//	CString m_sLinkTable;
//	int m_iDefaultSortIndex;
//	ESortDirection m_eDefaultSortDirction;
//};


//NV类
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
// 每条数据访问接口
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
	//获取错误号
	virtual int GetErrorNo();
	//获取错误信息
	virtual CString GetErrorInfo(); 
	//获取委托号
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
//数据集
class CDataList : virtual public IDataList
{
public:
	CDataList();
	virtual ~CDataList();
	//释放
	void Release();
	int GetCount();
	////从配置文件加载表头配置
	//void LoadColumns();
	//// 获取列信息
	//virtual IColumn * GetColumns(const int i);
	////获取列数
	//virtual int GetColumnCount();
	//根据索引获取一条记录   
	virtual IDataItem * GetItems(const int i);
	//增加一条记录
	virtual void AddItem(IDataItem * pItem);
	//插入一条记录
	virtual void InsertItem(const int iIndex, IDataItem * pItem);
	//删除一条记录
	virtual void DeleteItem(const int iIndex);
	virtual IDataItem * NewItem();
	virtual void Lock();
	virtual void UnLock();
	virtual IDataList* Clone();
protected:
	virtual IDataList* NewDataList();//需要克隆必须重载    
private:	
	//CColumns* m_pColumns;
	CDataItemList m_DataItemList;
	CRITICAL_SECTION m_cs;
};
