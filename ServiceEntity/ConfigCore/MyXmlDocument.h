#pragma once

#include "QuoteLangMngInterFace.h"
#include "TradeComm\TradeLangMngIntf.h"
#include "tinyxml.h"
#include "WinnerApplication.h"



#define Root_Text _T("root")

class CMyXmlDocument : public TiXmlDocument
{
public:
	CMyXmlDocument();
	~CMyXmlDocument();
protected:
	/********************************************************************************
	* 函数功能 : 从指定资源获取文件数据
	* 函数参数 : UINT nFileID : 资源ID
	*  返回值  : BOOL : 成功为TRUE,失败为FALSE
	* 调用依赖 : 
	* 作    者 : lxqi
	* 完成日期 : 2010-11-18
	* 修改记录 : 
	*******************************************************************************/
	char * GetDataFromResource(UINT nFileID);
public:
	/********************************************************************************
	* 函数功能 : 从资源里加载xml文件
	* 函数参数 : UINT nFileID : 资源ID
	*			 
	*  返回值  : BOOL : 加载成功返回TRUE,失败返回FALSE
	* 调用依赖 : 
	* 备    注 : 分别从行情资源库(QuoteLang)和交易资源库(TradeLang)查找对应资源，若ID重名
	*			 则优先从行情库加载
	* 作    者 : lxqi
	* 完成日期 : 2010-11-18
	* 修改记录 : 
	*******************************************************************************/
	virtual BOOL	LoadFromResource(UINT nFileID);

	/********************************************************************************
	* 函数功能 : 加载指定XML文件
	* 函数参数 : const char *pFilePath : 文件路径
	*			 
	*  返回值  : BOOL : 加载成功返回TRUE,失败返回FALSE
	* 调用依赖 : 
	* 备    注 : 需要绝对路径
	* 作    者 : lxqi
	* 完成日期 : 2010-11-18
	* 修改记录 : 
	*******************************************************************************/
	virtual BOOL	LoadFromFile(const char* pFilePath);

	/********************************************************************************
	* 函数功能 : 释放本文档所有资源
	* 函数参数 : 
	*  返回值  : 
	* 调用依赖 : 
	* 备    注 : 
	* 作    者 : lxqi
	* 完成日期 : 2010-11-18
	* 修改记录 : 
	*******************************************************************************/
	virtual void	Release();

	/********************************************************************************
	* 函数功能 : 保存当前文档内容
	* 函数参数 : 
	*  返回值  : 
	* 调用依赖 : 
	* 备    注 : 
	* 作    者 : 王超
	* 完成日期 : 2010-12-23
	* 修改记录 : 
	*******************************************************************************/
	virtual BOOL	SaveFile(const CString& sFileName);

	/********************************************************************************
	* 函数功能 : 获取当前文档根节点 
	* 函数参数 : 
	*  返回值  : TiXmlNode* : 根节点指针 如果设置逻辑根返回逻辑根节点，没有返回文档根节点
	* 调用依赖 : 
	* 备    注 : 
	* 作    者 : 王超
	* 完成日期 : 2010-12-14
	* 修改记录 : 
	*******************************************************************************/
	virtual TiXmlNode* GetRootNode();

	/********************************************************************************
	* 函数功能 : 获取子节点个数
	* 函数参数 : CString strFindPath : 统计节点路径(若为空，则默认为根节点)
	*			 TiXmlNode* pParentNode : 统计节点指针(若为空，则默认为根节点)
	*			 CString strChildElement : 子节点标签名(若为空则不考虑子标签名因素，只返回下一级所有子节点个数)
	*  返回值  : int : 下一层子节点个数，为找到统计节点返回-1
	* 调用依赖 : 
	* 备    注 : 
	* 作    者 : lxqi
	* 完成日期 : 2010-11-18
	* 修改记录 : 
	*******************************************************************************/
	virtual int GetChildNodeCount(const CString& strFindPath,
		const CString& strAttrName = "", 
		const CString& strAttrValue = "",
		const CString& strChildElement = "");
	virtual int GetChildNodeCount(TiXmlNode* pParentNode = NULL, const CString& strChildElement = "");

	/********************************************************************************
	* 函数功能 : 根据指定节点路径查找节点
	* 函数参数 : CString sFileName pFindPath : 路径信息，用‘/’分隔
	*  返回值  : TiXmlNode* : 找到的节点指针，未找的返回NULL
	* 调用依赖 : 
	* 备    注 : 传入的节点路径里，各节点之间需由“/”分隔开；如System/HsQuote
	*			 查找路径均为相对于GetRootNode()所得的根节点
	* 作    者 : lxqi
	* 完成日期 : 2010-11-18
	* 修改记录 : 
	*******************************************************************************/
	virtual TiXmlNode* GetNode(const CString& strFindPath); 

	/********************************************************************************
	* 函数功能 : 根据指定路径查找指定属性名和属性值的节点
	* 函数参数 : CString strFindPath : 路径信息，用‘/’分隔
	*			 CString strName : 属性名称
	*			 CString strNameValue : 属性值
	*			 int nNameValue : 属性值
	*			 double dNameValue : 属性值
	*  返回值  : TiXmlNode* : 找到的节点指针，未找的返回NULL
	* 调用依赖 : 
	* 备    注 : 传入的节点路径里，各节点之间需由“/”分隔开；如System/HsQuote
	*			 查找路径均为相对于GetRootNode()所得的根节点
	* 作    者 : lxqi
	* 完成日期 : 2010-11-18
	* 修改记录 : 
	*******************************************************************************/
	virtual TiXmlNode* GetNode(const CString& strFindPath, const CString& strName, const CString& strNameValue);
	virtual TiXmlNode* GetNode(const CString& strFindPath, const CString& strName, int nNameValue);
	virtual TiXmlNode* GetNode(const CString& strFindPath, const CString& strName, double dNameValue);

	/********************************************************************************
	* 函数功能 : 设置根节点
	* 函数参数 : CString strFindPath : 路径信息，用‘/’分隔（不包含根节点）
	*			 CString strAttrName : 属性名称
	*			 CString strAttrValue : 属性值
	*			 TiXmlNode* pRootNode : 指定根节点指针
	*  返回值  : TiXmlNode* : 当前根节点
	* 调用依赖 : 
	* 备    注 : 若子类根节点不同需要重载此函数
	* 作    者 : 王超
	* 完成日期 : 2010-12-1
	* 修改记录 : 
	*******************************************************************************/
	virtual BOOL SetRootNode(const CString& strFindPath,
		const CString& strAttrName = "", 
		const CString& strAttrValue = "");
	virtual BOOL SetRootNode(TiXmlNode* pRootNode);

	//////////////////////////////////////////////////////////////////////////以下各查找函数支持xPath标准语句
	/********************************************************************************
	* 函数功能 : 获取或设定指定节点的
	* 函数参数 : CString strFindPath : 路径信息，用‘/’分隔（不包含根节点）
	*			 CString strNewValue : 新设定值
	*			 CString strAttrName : 属性名称
	*			 CString strAttrValue : 属性值
	*  返回值  : CString : 节点包含的文本内容
	*			 BOOL : 设定成功否
	* 调用依赖 : 
	* 备    注 : 
	* 作    者 : lxqi
	* 完成日期 : 2010-11-16
	* 修改记录 : 
	*******************************************************************************/
	virtual CString GetNodeData(TiXmlNode* pNode);
	virtual CString	GetNodeData(const CString& strFindPath,
		const CString& strAttrName = "", 
		const CString& strAttrValue = "");
	virtual BOOL SetNodeData(TiXmlNode* pNode, const CString& strNewValue);
	virtual BOOL SetNodeData(const CString& strFindPath,
		const CString& strNewValue,
		const CString& strAttrName = NULL, 
		const CString& strAttrValue = NULL);


	/********************************************************************************
	* 函数功能 : 获取或设定指定节点的整型属性值
	* 函数参数 : CString strpFindPath : 路径信息，用‘/’分隔（不包含根节点）
	*			 TiXmlNode* pNode : 节点指针
	*			 CString strAttrName : 需要设定值的属性名
	*			 int nNewAttrValue : 新设定值
	*			 int nDefault : 读取不成功时的默认值
	*			 CString strExtAttrName :  用于查找的属性名称
	*			 CString strExtAttrValue : 用于查找的属性值
	*  返回值  : int : 读取的值，失败时返回默认值
	*			 BOOL : 设定成功否，成功返回TRUE,失败返回FALSE
	* 调用依赖 : 
	* 备    注 : 
	* 作    者 : lxqi
	* 完成日期 : 2010-11-16
	* 修改记录 : 
	*******************************************************************************/
	virtual int GetNodeAttrInt(TiXmlNode* pNode,const CString& strAttrName,int nDefault = 0);
	virtual int	GetNodeAttrInt(const CString& strFindPath,
		const CString& strAttrName, 
		int nDefault = 0,
		const CString& strExtAttrName = "", 
		const CString& strExtAttrValue = "");
	virtual BOOL SetNodeAttrInt(TiXmlNode* pNode, const CString& strNewAttrName, int nNewAttrValue);
	virtual BOOL SetNodeAttrInt(const CString& strFindPath, 
		const CString& strNewAttrName, 
		int nNewAttrValue, 
		const CString& strAttrName = "", 
		const CString& strAttrValue = "");

	/********************************************************************************
	* 函数功能 : 获取或设定指定节点的浮点属性值
	* 函数参数 : CString strpFindPath : 路径信息，用‘/’分隔（不包含根节点）
	*			 TiXmlNode* pNode : 节点指针
	*			 CString strAttrName : 需要设定值的属性名
	*			 int nNewAttrValue : 新设定值
	*			 int nDefault : 读取不成功时的默认值
	*			 CString strExtAttrName :  用于查找的属性名称
	*			 CString strExtAttrValue : 用于查找的属性值
	*  返回值  : double : 读取的值，失败时返回默认值
	*			 BOOL : 设定成功否，成功返回TRUE,失败返回FALSE
	* 调用依赖 : 
	* 备    注 : 
	* 作    者 : lxqi
	* 完成日期 : 2010-11-16
	* 修改记录 : 
	*******************************************************************************/
	virtual double GetNodeAttrDouble(TiXmlNode* pNode,const CString& strAttrName,double dDefault = 0.0);
	virtual double GetNodeAttrDouble(const CString& strFindPath,
		const CString& strAttrName, 
		double dDefault = 0.0,
		const CString& strExtAttrName = "", 
		const CString& strExtAttrValue = "");
	virtual BOOL SetNodeAttrDouble(TiXmlNode* pNode, const CString& strNewAttrName, double dNewAttrValue);
	virtual BOOL SetNodeAttrDouble(const CString& strFindPath, 
		const CString& strNewAttrName, 
		double dNewAttrValue, 
		const CString& strAttrName = "", 
		const CString& strAttrValue = "");

	/********************************************************************************
	* 函数功能 : 获取或设定指定节点的字符串属性值
	* 函数参数 : CString strpFindPath : 路径信息，用‘/’分隔（不包含根节点）
	*			 TiXmlNode* pNode : 节点指针
	*			 CString strAttrName : 需要设定值的属性名
	*			 int nNewAttrValue : 新设定值
	*			 int nDefault : 读取不成功时的默认值
	*			 CString strExtAttrName :  用于查找的属性名称
	*			 CString strExtAttrValue : 用于查找的属性值
	*  返回值  : double : 读取的值，失败时返回默认值
	*			 BOOL : 设定成功否，成功返回TRUE,失败返回FALSE
	* 调用依赖 : 
	* 备    注 : 
	* 作    者 : lxqi
	* 完成日期 : 2010-11-16
	* 修改记录 : 
	*******************************************************************************/
	virtual CString GetNodeAttrString(TiXmlNode* pNode,const CString& strAttrName,const CString& strDefault=_T(""));
	virtual CString GetNodeAttrString(const CString& strFindPath,
		const CString& strAttrName, 
		const CString& strDefault = "",
		const CString& strExtAttrName = "", 
		const CString& strExtAttrValue = "");
	virtual BOOL SetNodeAttrString(TiXmlNode* pNode, const CString& strNewAttrName, const CString& strNewAttrValue);
	virtual BOOL SetNodeAttrString(const CString& strFindPath, 
		const CString& strNewAttrName, 
		const CString& strNewAttrValue, 
		const CString& strAttrName = "", 
		const CString& strAttrValue = "");


	/********************************************************************************
	* 函数功能 : 获取/设置修改标识
	* 函数参数 : BOOL bModified : 是否已修改
	*  返回值  : BOOL : 是否已修改
	* 调用依赖 : 
	* 备    注 : 
	* 作    者 : lxqi
	* 完成日期 : 2010-11-18
	* 修改记录 : 
	*******************************************************************************/
	BOOL GetModified() {return m_bModified;}
	void SetModified(BOOL bModified){ m_bModified = bModified; }

	/********************************************************************************
	* 函数功能 : 获取/设置加载标识
	* 函数参数 : BOOL bLoaded : 是否已加载
	*  返回值  : BOOL : 是否已加载
	* 调用依赖 : 
	* 备    注 : 
	* 作    者 : 王超
	* 完成日期 : 2010-12-14
	* 修改记录 : 
	*******************************************************************************/
	BOOL GetLoaded() {return m_bLoaded;}
	void SetLoaded(BOOL bLoaded){ m_bLoaded = bLoaded; }

	/********************************************************************************
	* 函数功能 : 判断节点是否属于本文件
	* 函数参数 : TiXmlNode* : 节点指针
	*  返回值  : BOOL : 是否属于
	* 调用依赖 : 
	* 备    注 : 
	* 作    者 : 王超
	* 完成日期 : 2010-12-15
	* 修改记录 : 
	*******************************************************************************/
	BOOL IsBelongToMy(TiXmlNode* pNode);

private:
	BOOL m_bModified;  //是否修改
	BOOL m_bLoaded;    //是否加载
	TiXmlNode* m_pRootNode;
//	CMapStringToPtr m_CurNodeMap;   //存放读取过的节点 索引:传入的路径
public:
	static IQuoteLang* m_iQuoteLang;
	static ITradeLang* m_iTradeLang;
};
