/*******************************************************
  源程序名称:TradeDataBaseIntf.h
  软件著作权:恒生电子股份有限公司
  系统名称:  投资赢家金融理财终端V1.0
  模块名称:  交易中心
  功能说明:  交易数据基础接口
  作    者:  shenglq
  开发日期:  20100720
*********************************************************/
#pragma once
#include "TradeConst.h"

//               无，  统计， 求和，根据币种求和，根据市场求和
enum ESumType {stNone, stCount, stSum, stMoneySum,stMarketSum};
//                   升序，降序
enum ESortDirection {sdASC, sdDESC};
//                            买      卖
enum EBSDirection {bsNone, bsBuy, bsSell};


//struct IColumn : IXMLPropAccess
//struct IColumn
//{
//	virtual ~IColumn() = 0;
//	//获取列名
//	virtual CString GetColumnName() = 0;
//	//获取显示名称
//	virtual CString GetDispName() = 0; 
//	//获取数据字典表名
//	virtual CString GetDataDict() = 0;
//	//显示格式
//	virtual CString GetDispFormat() = 0;
//	//获取合计类型
//	virtual ESumType GetSumType() = 0;  
//	//是否计算字段
//	virtual BOOL GetIsCalc() = 0; 
//	//对齐
//	virtual EAlign GetAlign() = 0;
//};
//
////需要显示的表字段信息
////struct IColumns : INodeData
//struct IColumns
//{
//	virtual ~IColumns() = 0;
//	//列数
//	virtual int GetCount() = 0;
//	//根据索引获取列
//	virtual IColumn * GetColumns(const int i) = 0;	
//	//根据名称获取列
//	virtual IColumn * GetColumnByName(const CString sName) = 0;
//	//获取关联表名
//	virtual CString GetLinkTable() = 0;
//	//获取默认排序列索引
//	virtual int GetDefaultSortIndex() = 0;
//	//获取默认排序列方向
//	virtual ESortDirection GetDefaultSortDirction() = 0;
//};



/*
//NV类
struct INVData
{
	virtual ~INVData() = 0;
	virtual CString GetName() = 0;
	virtual void SetName(const CString Value) = 0;
	virtual CString GetValue() = 0;
	virtual void SetValue(const CString Value) = 0;
};*/


// 每条数据访问接口
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
	//获取错误号
	virtual int GetErrorNo() = 0;
	//获取错误信息
	virtual CString GetErrorInfo() = 0; 
	//获取委托号
	virtual CString GetEntrustNo() = 0;

	virtual void SetData(const CString sName, const CString sValue) = 0;
	//获取委托流水号
	virtual CString GetSerialNo() = 0;
	virtual void AddData(const CString sName, const CString sValue) = 0;
	virtual int GetCount() = 0;
	virtual void GetFieldAndValue(const int i, CString &sField, CString &sValue) = 0;

};
//数据集
struct IDataList
{
	//释放
	virtual void Release() = 0;
	virtual int GetCount() = 0;
	////从配置文件加载表头配置
	//virtual void LoadColumns() = 0;
	//// 获取列信息
	//virtual IColumn * GetColumns(const int i) = 0;
	////获取列数
	//virtual int GetColumnCount() = 0;
	//根据索引获取一条记录   
	virtual IDataItem * GetItems(const int i) = 0;
	//增加一条记录
	virtual void AddItem(IDataItem * pItem) = 0;
	//插入一条记录
	virtual void InsertItem(const int iIndex, IDataItem * pItem) = 0;
	//删除一条记录
	virtual void DeleteItem(const int iIndex) = 0;
	//增加一条记录
	virtual IDataItem * NewItem() = 0;
	virtual void Lock() = 0;
	virtual void UnLock() = 0;
	virtual IDataList* Clone() = 0;
};
