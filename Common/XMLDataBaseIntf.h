/*******************************************************
  源程序名称:XMLDataBaseIntf.h
  软件著作权:恒生电子股份有限公司
  系统名称:  投资赢家金融理财终端V1.0
  模块名称:  配置中心
  功能说明:  XML配置文件基础数据接口定义
  作    者:  shenglq
  开发日期:  20100926
*********************************************************/
#pragma once
typedef CString CXML;

struct INodeData 
{
	//释放
	virtual void Release() = 0;
	//根据属性名返回值
	virtual CString GetValue(const CString sPropName) = 0;
	virtual CString Name() = 0;
	virtual CString Value() = 0;
	virtual CString ValueEx() = 0;
};
//////////////////////////////////////////////////////////////////////////
struct INodeTreeData : virtual public INodeData
{
	virtual int GetCount() = 0;
	virtual INodeData* GetItems(const int iIndex) = 0;
	virtual INodeData* GetItemsByName(const CString sName) = 0;
};

//////////////////////////////////////////////////////////////////////////

struct INodeDataList
{
	//释放
	virtual void Release() = 0;
	//统计接点个数
	virtual int GetCount() = 0;
	virtual INodeData* GetItems(const int iIndex) = 0;
	virtual INodeData* GetItemsByName(const CString sName) = 0;
	virtual INodeTreeData* GetNodeTreeData(const CString sName) = 0;
};