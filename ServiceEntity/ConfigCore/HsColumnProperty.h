/*******************************************************************************
* Copyright (c)2010, 杭州恒生电子股份有限公司
* All rights reserved.
*
* 文件名称：HsColumnProperty.h
* 摘    要：行情报价列属性实现类
*
* 当前版本：
* 作    者：王超
* 完成日期：2010-12-22
* 备	 注：
*******************************************************************************/

#pragma once
#include "ConfigBase.h"
#include "ConfigStruct.h"
#include "ConfigInterface.h"

class CHsColumnProperty : public IHsColumnProperty
{
public:
	CHsColumnProperty();
	~CHsColumnProperty();

	void Init();
	BOOL IsInit() {return m_bInited;}

	BOOL Release(BOOL bSaveToFile = TRUE);
	BOOL SaveToFile();

private:

	void InitAySys(); // 初始化 系统列Tag名称 、系统列name属性名

	// 根据strPath父节点找name属性为strName的孩子节点
	TiXmlNode* GetNodeByName(CConfigBase* pConfig, CString strPath, CString strName);
	//在DispColumn文件中 通过name属性值 查找节点 如:<stock>、<Fund>
	TiXmlNode* GetNodeByNameFromDisp(CString strName);
	//在ReportColumn文件中 通过name属性值 查找节点 如:<StockQuote>、<StockQuoteEx>
	TiXmlNode* GetNodeByNameFromReport(CString strName);
	
	// 通过节点的Tag值获取节点的Name属性值 找不到则返回strName
	CString GetGroupNameByTagName(CString strName);
	// 通过节点的Name属性值获取节点的Tag值 找不到则返回strName
	CString GetTagNameByGroupName(CString strName);
	// 根据strPath父节点求所有子节点name属性值 
	long    GetGroupName(CConfigBase* pConfigBase, CString strPath, CStringArray& ayGroup);

	TableColProperty*	GetColumn(TiXmlNode* pNode);  	// 获取列属性
//接口实现
public:
	virtual TableColProperty*	GetColumn(int nID);  	// 获取列属性
	virtual CString		GetDefDispColGroupName();		    // 获取默认显示列组名称
	virtual CString		GetDefaultColGroupName();		    // 获取默认列组名称 可删除先保留
	virtual long		GetAllColumnGroupName(CStringArray& ayGroup);		// 获取所有列组名称
	virtual CColPropArray*   GetColumnGroupProp(CString strGroupName);		// 获取指定列组所有列属性 
	virtual BOOL             GetColumnGroupPropEx(CColPropIDIndex &mapProp, CString strGroupName);    // 功能同上 返回值不同
	virtual CString		GetFinanceGroupName();  			// 获取财务数据列组名称
	
	virtual long		GetAllDispColTemplName(CStringArray& ayGroup);	    // 获取所有显示列模板名称
	virtual RefreshDispTextProp* GetQuoteRefreshProp();   // 获取刷新属性结构

	virtual CString		GetColumnName(int nID);       // 获取列名称
	virtual long		GetColumnMask(int nID);       // 获取列掩码
	virtual long		GetColumnWidth(int nID);      // 获取列宽度

	//获取显示列中的各个列的属性 ayGroup中奇数项存放ColumnID，偶数项存放ColumnWidth
	virtual long        GetDispColGroup(CString strGroup, CUIntArray*& pAyGroup);  
	virtual void		SetDispColGroup(CString strGroup, CUIntArray* pAyGroup);	// 更改显示列组内容
	virtual BOOL		AddDispColGroup(CString strGroup, CUIntArray* pAyGroup);	// 添加显示列组
	virtual BOOL		DelDispColGroup(CString strGroup);		// 删除显示列组
private:
	CConfigBase*  m_pReportColumnCfg;
	CConfigBase*  m_pDisplayColumnCfg;
	BOOL m_bInited;

	//以下同一个下标的两个值 属于同一节点
	CStringArray    m_aySysTagName;             // 系统列Tag名称
	CStringArray    m_aySysAttrName;            // 系统列name属性名

	CMapWordToPtr   m_mapColIDIndex;			// 按ID索引的所有列 (TableColProperty*)
	CMapStringToPtr m_mapColPropGroup;          // 所有列(分组存放) (CColPropArray*)
	CMapStringToPtr m_mapShowColGroup;          // 显示列组的属性   (CUIntArray*)
	RefreshDispTextProp m_sRefreshDispTextProp; // 行情报价刷新样式结构
};
