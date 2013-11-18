/*******************************************************************************
* Copyright (c)2010, 杭州恒生电子股份有限公司
* All rights reserved.
*
* 文件名称：ConfigBase.h
* 摘    要：文件操作实现类
*
* 当前版本：
* 作    者：王超
* 完成日期：2010-12-10
* 备	 注：
*******************************************************************************/
#pragma once
#include "MyXmlDocument.h"

enum DocType{Default, ResourceDoc, SystemDoc, UserDoc};		// 指定数据源类型


#define IntDefault     INT_MAX
#define DoubleDefault  -2222.222

class CConfigBase
{
public:
	CConfigBase();
	~CConfigBase();

private:
	/********************************************************************************
	* 函数功能 : 初始化资源中的配置文件
	* 函数参数 : UINT nRcID : 指定待加载的资源文件ID
	*			 
	*  返回值  : BOOL : 是否加载成功
	* 调用依赖 : 
	* 备    注 :  
	* 作    者 : 王超
	* 完成日期 : 2010-12-14
	* 修改记录 : 
	*******************************************************************************/
	BOOL InitResourceDoc(UINT nRcID);
	/********************************************************************************
	* 函数功能 : 初始化本地系统配置文件
	* 函数参数 : CString strDocFilePath : 指定待加载的文件路径
	*			 
	*  返回值  : BOOL : 是否加载成功
	* 调用依赖 : 
	* 备    注 :  
	* 作    者 : 王超
	* 完成日期 : 2010-12-14
	* 修改记录 : 
	*******************************************************************************/
	BOOL InitSysFileDoc(const CString& strDocFilePath);
	/********************************************************************************
	* 函数功能 : 初始化本地自定义配置文件
	* 函数参数 : CString strDocFilePath : 指定待加载的文件路径
	*			 
	*  返回值  : BOOL : 是否加载成功
	* 调用依赖 : 
	* 备    注 :  
	* 作    者 : 王超
	* 完成日期 : 2010-12-14
	* 修改记录 : 
	*******************************************************************************/
	BOOL InitUserFileDoc(const CString& strDocFilePath);

	/********************************************************************************
	* 函数功能 : 在用户文档将给出的节点存放指定的路径
	* 函数参数 : TiXmlNode* : 需要插入的节点
	*            CString    : 路径
	*            TiXmlNode* : 系统文件或者资源中对应的节点
	*            CString    : 路径扩展属性名
	*            CString    : 路径扩展属性值 
	*  返回值  : BOOL       : 返回操作是否成功
	* 调用依赖 : 
	* 备    注 : 
	* 作    者 : 王超
	* 完成日期 : 2010-12-15
	* 修改记录 : 
	*******************************************************************************/
	BOOL InsertNodes(TiXmlNode* pNewNode, CString strFindPath, TiXmlNode* pOtherNode, const CString strAttrName = "", const CString strAttrValue = "");

	/********************************************************************************
	* 函数功能 : 将源节点的所有属性以及文本内容 复制到目标节点
	* 函数参数 : TiXmlNode* : 源节点
	*  返回值  : TiXmlNode* : 目标节点
	* 调用依赖 : 
	* 备    注 : 
	* 作    者 : 王超
	* 完成日期 : 2010-12-16
	* 修改记录 : 
	*******************************************************************************/
	TiXmlNode* CopyNodeFromAnther(TiXmlNode* pSourceNode);

public:
	// 配置初始化
	/********************************************************************************
	* 函数功能 : 初始化配置
	* 函数参数 : UINT nRcID : 指定待加载的资源文件ID
	*			 CString strSysDocFilePath : 指定待加载的系统默认文件路径
	*			 CString strUserDocFilePath : 指定待加载的用户自定义文件路径
	*			 
	*  返回值  : BOOL : 是否加载成功
	* 调用依赖 : 
	* 备    注 :  
	* 作    者 : 王超
	* 完成日期 : 2010-12-14
	* 修改记录 : 
	*******************************************************************************/
	virtual BOOL Init(UINT nRcID, const CString &strSysDocFilePath, const  CString &strUserDocFilePath);

	/********************************************************************************
	* 函数功能 : 判断是否已初始化过
	* 函数参数 : 
	*  返回值  : BOOL : 已初始化过为TRUE,未初始化为FALSE
	* 调用依赖 : 
	* 作    者 : lxqi
	* 完成日期 : 2010-11-12
	* 修改记录 : 
	*******************************************************************************/
	virtual BOOL IsInit();

	/********************************************************************************
	* 函数功能 : 释放内存
	* 函数参数 : UINT nFileID : 资源ID
	*  返回值  : BOOL : 成功为TRUE,失败为FALSE
	* 调用依赖 : 
	* 作    者 : lxqi
	* 完成日期 : 2010-11-12
	* 修改记录 : 
	*******************************************************************************/
	virtual BOOL Release(BOOL bSaveToFile = FALSE);

	/********************************************************************************
	* 函数功能 : 将修改内容保存到对应文件
	* 函数参数 : CString strFilePath : 保存文件路径
	*  返回值  : 
	* 调用依赖 : 
	* 备    注 : 若传入路径为空，则保存文件为UserDoc初始化时的文件名，
	*			 若无此文件，则默认为SystemDoc文件名后加"User"
	* 作    者 : lxqi
	* 完成日期 : 2010-11-12
	* 修改记录 : 
	*******************************************************************************/
	virtual void SaveToFile(const CString &strFileName = "");

	/********************************************************************************
	* 函数功能 : 根据指定路径查找指定属性名和属性值的节点
	* 函数参数 : CString strFindPath : 路径信息，用‘/’分隔（不包含根节点）
	*			 DocType nType : 指定查找文件，默认为自动三级查找（User->System->Resource）
	*  返回值  : TiXmlNode* : 找到的节点指针，未找的返回NULL
	*			 TiXmlElement* : 找到的节点指针，未找的返回NULL
	* 调用依赖 : 
	* 备    注 : 传入的节点路径里，各节点之间需由“/”分隔开；如System/HsQuote（不包含根节点）
	*			 每次成功搜索到节点后，自动查找该节点是否有id和name属性，若有，则加入相应索引
	*			 若需要添加其他索引，需要重载此函数
	* 作    者 : lxqi
	* 完成日期 : 2010-11-12
	* 修改记录 : 
	*******************************************************************************/
	virtual TiXmlNode* GetNode(const CString& strFindPath, DocType nType = Default);

	/********************************************************************************
	* 函数功能 : 根据指定路径查找指定属性名和属性值的节点
	* 函数参数 : CString strFindPath : 路径信息，用‘/’分隔（不包含根节点）
	*			 CString strAttrName : 属性名称
	*			 CString strAttrValue : 属性值
	*			 DocType nType : 指定查找文件，默认为自动三级查找（User->System->Resource）
	*  返回值  : TiXmlNode* : 找到的节点指针，未找的返回NULL
	*			 TiXmlElement* : 找到的节点指针，未找的返回NULL
	* 调用依赖 : 
	* 备    注 : 传入的节点路径里，各节点之间需由“/”分隔开；如System/HsQuote（不包含根节点）
	*			 每次成功搜索到节点后，自动查找该节点是否有id和name属性，若有，则加入相应索引
	*			 若需要添加其他索引，需要重载此函数
	* 作    者 : lxqi
	* 完成日期 : 2010-11-12
	* 修改记录 : 
	*******************************************************************************/
	virtual TiXmlNode* GetNode(const CString& strFindPath, const CString& strAttrName, const CString& strAttrValue, DocType nType = Default  );
	virtual TiXmlNode* GetNode(const CString& strFindPath, const CString& strAttrName, int nNameValue, DocType nType = Default  );
	virtual TiXmlNode* GetNode(const CString& strFindPath, const CString& strAttrName,double dNameValue, DocType nType = Default  );
	/********************************************************************************
	* 函数功能 : 在用户文件中新建一个节点(该节点必须是系统文件或者资源文件中存在的)
	* 函数参数 : CString strPath : 节点路径
	*  返回值  : TiXmlNode* : 新建的节点 失败则为空
	* 调用依赖 : 
	* 备    注 : 
	* 作    者 : 王超
	* 完成日期 : 2010-12-23
	* 修改记录 : 
	*******************************************************************************/
	virtual TiXmlNode* SetNode(const CString& strPath, const CString& strAttrName = "", const CString& strAttrValue = "");

	/********************************************************************************
	* 函数功能 : 在用户文件中删除一个节点
	* 函数参数 : CString strPath : 节点路径
	*  返回值  : BOOL : 操作是否成功
	* 调用依赖 : 
	* 备    注 : 
	* 作    者 : 王超
	* 完成日期 : 2010-12-23
	* 修改记录 : 
	*******************************************************************************/
	virtual BOOL RemoveNode(TiXmlNode* pNode);
	virtual BOOL RemoveNode(const CString& strPath, const CString& strAttrName = "", const CString& strAttrValue = "");
	/********************************************************************************
	* 函数功能 : 获取当前根节点
	* 函数参数 : DocType nType : 指定文档类型，默认为用户文件
	*  返回值  : TiXmlNode* : 当前根节点
	* 调用依赖 : 
	* 备    注 : 若子类根节点不同需要重载此函数
	* 作    者 : lxqi
	* 完成日期 : 2010-11-12
	* 修改记录 : 
	*******************************************************************************/
	virtual TiXmlNode*	GetRootNode(DocType nType = Default  );
	/********************************************************************************
	* 函数功能 : 设置根节点
	* 函数参数 : CString strFindPath : 路径信息，用‘/’分隔（不包含根节点）
	*			 CString strAttrName : 属性名称
	*			 CString strAttrValue : 属性值
	*			 TiXmlNode* pRootNode : 指定根节点指针
	*			 DocType nType : 指定查找文件，默认为用户文件
	*  返回值  : TiXmlNode* : 当前根节点
	* 调用依赖 : 
	* 备    注 : 若子类根节点不同需要重载此函数
	* 作    者 : lxqi
	* 完成日期 : 2010-11-12
	* 修改记录 : 
	*******************************************************************************/
	virtual BOOL SetRootNode(const CString& strFindPath,
		const CString& strAttrName = "", 
		const CString& strAttrValuev = "", 
		DocType nType = Default  );
	virtual BOOL SetRootNode(TiXmlNode* pRootNode,DocType nType = Default  );
	//////////////////////////////////////////////////////////////////////////以下各查找函数支持xPath标准语句
	/********************************************************************************
	* 函数功能 : 获取或设定指定节点的
	* 函数参数 : CString strFindPath : 路径信息，用‘/’分隔（不包含根节点）
	*			 CString strNewValue : 新设定值
	*			 int nIndex : 起始索引号（0..n）
	*			 CString strAttrName : 属性名称
	*			 CString strAttrValue : 属性值
	*			 DocType nType : 指定查找文件，默认为自动三级查找（User->System->Resource）
	*  返回值  : CString : 节点包含的文本内容
	*			 BOOL : 设定成功否
	* 调用依赖 : 
	* 备    注 : 
	* 作    者 : lxqi 
	* 完成日期 : 2010-12-1
	* 修改记录 : 
	*******************************************************************************/
//	virtual CString GetNodeData(TiXmlNode* pNode, DocType nType = Default  );
	virtual CString	GetNodeData(const CString& strFindPath,
		const CString& strAttrName = "", 
		const CString& strAttrValue = "", 
		DocType nType = Default  );
	virtual BOOL SetNodeData(TiXmlNode* pNode, const CString& strNewValue);
	virtual BOOL SetNodeData(const CString& strFindPath,
		const CString& strNewValue,
		const CString& strAttrName = "", 
		const CString& strAttrValue = "");

	/********************************************************************************
	* 函数功能 : 获取或设定指定节点的整型属性值
	* 函数参数 : CString strpFindPath : 路径信息，用‘/’分隔（不包含根节点）
	*			 CString strNewAttrName : 需要设定值的属性名
	*			 int nNewAttrValue : 新设定值
	*			 int nDefault : 读取不成功时的默认值
	*			 CString strExtAttrName :  用于查找的属性名称
	*			 CString strExtAttrValue : 用于查找的属性值
	*			 DocType nType : 指定查找文件，默认为自动三级查找（User->System->Resource）
	*  返回值  : int : 读取的值，失败时返回默认值
	*			 BOOL : 设定成功否，成功返回TRUE,失败返回FALSE
	* 调用依赖 : 
	* 备    注 : 
	* 作    者 : lxqi 
	* 完成日期 : 2010-11-16
	* 修改记录 : 2010-12-1 实现by 王超
	*******************************************************************************/
	//virtual int GetNodeAttrInt(TiXmlNode* pNode, CString strAttrName, int nDefault = IntDefault, DocType nType = Default);
	virtual int	GetNodeAttrInt(const CString& strFindPath,
		const CString& strAttrName, 
		int nDefault = IntDefault,
		const CString& strExtAttrName = "", 
		const CString& strExtAttrValue = "", 
		DocType nType = Default  );
	virtual BOOL SetNodeAttrInt(TiXmlNode* pNode, const CString& strNewAttrName, int nNewAttrValue);
	virtual BOOL SetNodeAttrInt(const CString& strFindPath, 
		const CString& strNewAttrName, 
		int nNewAttrValue, 
		const CString& strAttrName = "", 
		const CString& strAttrValue = "");

	/********************************************************************************
	* 函数功能 : 获取或设定指定节点的浮点属性值
	* 函数参数 : CString strpFindPath : 路径信息，用‘/’分隔（不包含根节点）
	*			 CString strNewAttrName : 新设定属性名
	*			 double dNewAttrValue : 新设定值
	*			 double dNewAttrValue : 读取不成功时的默认值
	*			 int nIndex : 起始索引号（0..n）
	*			 CString strAttrName : 属性名称
	*			 CString strAttrValue : 属性值
	*			 DocType nType : 指定查找文件，默认为自动三级查找（User->System->Resource）
	*  返回值  : double : 读取的值，失败时返回默认值
	*			 BOOL : 设定成功否，成功返回TRUE,失败返回FALSE
	* 调用依赖 : 
	* 备    注 : Set函数不需要文档类型 只有用户文件可以更新
	* 作    者 : lxqi
	* 完成日期 : 2010-11-16
	* 修改记录 : 2010-12-1 实现by 王超
	*******************************************************************************/
	//virtual double GetNodeAttrDouble(TiXmlNode* pNode,CString strAttrName, double dDefault = DoubleDefault, DocType nType = Default);
	virtual double GetNodeAttrDouble(const CString& strFindPath,
		const CString& strAttrName = "", 
		double  dDefault = DoubleDefault,
		const CString& strExtAttrName = "", 
		const CString& strExtAttrValue = "", 
		DocType nType = Default);
	virtual BOOL SetNodeAttrDouble(TiXmlNode* pNode, const CString& strNewAttrName, double dNewAttrValue);
	virtual BOOL SetNodeAttrDouble(const CString& strFindPath, 
		const CString& strNewAttrName, 
		double dNewAttrValue, 
		const CString& strAttrName = "", 
		const CString& strAttrValue = "");

	/********************************************************************************
	* 函数功能 : 获取或设定指定节点的字符串属性值
	* 函数参数 : CString strpFindPath : 路径信息，用‘/’分隔（不包含根节点）
	*			 CString strNewAttrName : 新设定属性名
	*			 double dNewAttrValue : 新设定值
	*			 double dNewAttrValue : 读取不成功时的默认值
	*			 int nIndex : 起始索引号（0..n）
	*			 CString strAttrName : 属性名称
	*			 CString strAttrValue : 属性值
	*			 DocType nType : 指定查找文件，默认为自动三级查找（User->System->Resource）
	*  返回值  : double : 读取的值，失败时返回默认值
	*			 BOOL : 设定成功否，成功返回TRUE,失败返回FALSE
	* 调用依赖 : 
	* 备    注 : Set函数不需要文档类型 只有用户文件可以更新
	* 作    者 : lxqi
	* 完成日期 : 2010-11-16
	* 修改记录 : 2010-12-1 实现by 王超
	*******************************************************************************/
	//virtual CString GetNodeAttrString(TiXmlNode* pNode,CString strAttrName, CString strDefault = _T(""), DocType nType = Default);
	virtual CString GetNodeAttrString(const CString& strFindPath,
		const CString& strAttrName, 
		const CString& strDefault = "",
		const CString& strExtAttrName = "", 
		const CString& strExtAttrValue = "", 
		DocType nType = Default);
	virtual BOOL SetNodeAttrString(TiXmlNode* pNode, const CString& strNewAttrName, const CString& strNewAttrValue);
	virtual BOOL SetNodeAttrString(const CString& strFindPath, 
		const CString& strNewAttrName, 
		const CString& strNewAttrValue, 
		const CString& strAttrName = "", 
		const CString& strAttrValue = "");

	/********************************************************************************
	* 函数功能 : 获取文件对象指针
	* 函数参数 : DocType nType : 指定查找文件，默认为自动三级查找（User->System->Resource）	 
	*  返回值  : CMyXmlDocument* : 文件对象指针
	* 调用依赖 : 
	* 备    注 : 
	* 作    者 : 王超
	* 完成日期 : 2010-12-15
	* 修改记录 : 
	*******************************************************************************/
	CMyXmlDocument* GetXmlDocument(DocType nType = Default);


protected:
	CMyXmlDocument	m_ResourceDoc;
	CMyXmlDocument	m_SystemDoc;
	CMyXmlDocument	m_UserDoc;

	BOOL			m_bInited;				// 是否已初始化过

public:
	static IQuoteLang*		m_iQuoteLang;			// 行情资源库
	static ITradeLang*		m_iTradeLang;			// 交易资源库

};
