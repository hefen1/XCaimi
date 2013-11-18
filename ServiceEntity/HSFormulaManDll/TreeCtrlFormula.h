/*************************************************************
*	Copyright (c)2010, 恒生电子股份有限公司
*	All rights reserved.
*
*	文件名称：	TreeCtrlFormula.h
*	文件标识：	公式树控件类及其它相关结构定义
*	摘	  要：	
*
*	当前版本：	1.0
*	作	  者：	
*	更新日期：
*
*	修改说明：	
*
***************************************************************/
#if !defined(AFX_HXTREECTRL_H__D334AF88_CFC7_11D4_903D_5254AB285D5A__INCLUDED_)
#define AFX_HXTREECTRL_H__D334AF88_CFC7_11D4_903D_5254AB285D5A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TreeCtrlFormula.h : header file
//
#include "TreeDropTarget.h"
#include "Express.h"
#include "ExpPublic.h"
#include "ExpValue.h"
/////////////////////////////////////////////////////////////////////////////
// CTreeCtrlFormula window

/*************************************************************
*	类 名 称：	FunctionNameTable
*	类 描 述：	函数分类名
***************************************************************/
struct FunctionNameTable
{
	CString		 m_strName; // 分类名
	FunctionNameTable(CString strName)
	{
		m_strName.Format("%s",strName);
	}
};


#ifdef Support_HWJBKCJ
#define Express_Header "!@#$%^&*()QWAZXSDDCFVGH-1970"
#else
#define Express_Header "!@#$%^&*()QWAZXSDDCFVGH-1971"
#endif

struct SaveTransZipData
{
	char			m_szHeader[256];
	unsigned short	m_nType;		// 请求类型,恒为RT_ZIPDATA

	short 			m_nAlignment;	// 为了4字节对齐而添加的字段

	long			m_lZipLen;		// 压缩后的长度
	long			m_lOrigLen;		// 压缩前的长度
	char*		    m_cData;		// 压缩后的数据

	SaveTransZipData()
	{
		//memset(this,0,sizeof(SaveTransZipData));
		strncpy(m_szHeader,Express_Header,sizeof(m_szHeader));
	}

	void Free()
	{
		if( m_cData )
		{
			delete[] m_cData;
			m_cData = NULL;
		}
	}
};

extern int   IsZipExpressData(const char* szData);
extern char* unzipData(SaveTransZipData* pZipData);
extern SaveTransZipData* zipData(void *lpszBuffer, long cbBuffer);


// CTreeGroup::
#define hx_RootGroup 0x01	// 根组结点
#define hx_AddGroup  0x02	// 结结点(根结点下面的组结点)
#define hx_GroupItem 0x04	// 叶子项目
#define hx_RecycleBinGroup 0x08 // 加收站 
#define hx_OtherData	   0x10 // 其它,这时CTreeGroup::m_pData无效

struct CTreeGroup
{	
	char		m_nStatus;	// 状态
	HTREEITEM   m_hTree;    // 树项目相关信息
	union
	{		
		void*		m_pData;  // 指向CExpValue对象(!(m_nStatus&hx_OtherData))
		long		m_dwID;	  // 财务数据ID
	}; // ID或是数据指针(根据m_nStatus判断)

	CTreeGroup()
	{
		Empty();
	}
	CTreeGroup(char nStatus,long dwID)
	{
		m_nStatus = nStatus;
		m_dwID    = dwID;
		m_hTree   = NULL;
	}

	CTreeGroup(void* pData,char nStatus,HTREEITEM hTree)
	{
		m_pData   = pData;
		m_nStatus = nStatus;
		m_hTree	  = hTree;
	}

	// 状态操作
	BOOL IsStatus(char nStatus) { return (m_nStatus == nStatus); }
	void SetStatus(char nStatus) { m_nStatus = nStatus; }
	// 清空
	void Empty()
	{
		memset(this,0,sizeof(CTreeGroup));
	}
	
	// 得到包含的公式
	CExpression* GetData()
	{
		if( !(m_nStatus & hx_OtherData) && 
			(m_pData != NULL) )
		{
			return ((CExpValue*)m_pData)->GetExp();
		}
		return NULL;
	}

	// 是否指向公式(CExpValue*)pValue
	BOOL IsExpValue(long pValue) 
	{
		return ( !(m_nStatus & hx_OtherData) && ((long)m_pData == pValue) );
	}

	// 比较两个对象是不是相同
	BOOL Compare(CTreeGroup* pTreeGroup)
	{
		if( GetData() != NULL && 
			pTreeGroup != NULL && pTreeGroup->GetData() != NULL )
		{
			return (GetData() == pTreeGroup->GetData());
		}
		return FALSE;
	}
	// 得到公式名
	CString GetName()
	{
		CExpression* pExp = GetData();
		if( pExp == NULL )
			return "";
		
		CString strName;
		strName.Format("%s",pExp->GetName());
		return strName;
	}
};

class CDlgFormulaMan;

/*************************************************************
*	类 名 称：	CTreeCtrlFormula
*	类 描 述：	公式树控件
***************************************************************/
class CTreeCtrlFormula : public CTreeCtrl
{
// Construction
public:
	// strName公式树名称,一般是公式分类名
	CTreeCtrlFormula(CString strName = ""); 
	virtual ~CTreeCtrlFormula();

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTreeCtrlFormula)
	//}}AFX_VIRTUAL

	// Generated message map functions
protected:
	//{{AFX_MSG(CTreeCtrlFormula)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	// 单击项目事件处理( 不做处理 ) 
	afx_msg void OnClick(NMHDR* pNMHDR, LRESULT* pResult);
	// 双击项目事件处理 调用AddFunToEdit()
	afx_msg void OnDblclk(NMHDR* pNMHDR, LRESULT* pResult);
	// 项目按键消息处理( 不做处理 ) 
	afx_msg void OnKeydown(NMHDR* pNMHDR, LRESULT* pResult);
	// 选择改变处理
	afx_msg void OnSelchanged(NMHDR* pNMHDR, LRESULT* pResult);
	// 右键按下处理
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	// 左键按下处理
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	// 右键单击项目事件处理
	afx_msg void OnRclick(NMHDR* pNMHDR, LRESULT* pResult);
	// 左键双击处理
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	// 修改项目名字"开始处理"
	afx_msg void OnBeginlabeledit(NMHDR* pNMHDR, LRESULT* pResult);
	// 修改项目名字"结束处理"
	afx_msg void OnEndlabeledit(NMHDR* pNMHDR, LRESULT* pResult);
	// 字符输入消息处理
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	// 键盘消息处理
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnSysKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) ;
	//}}AFX_MSG
	// 浮动树时的拖放处理
	afx_msg void OnBegindrag(NMHDR* pNMHDR, LRESULT* pResult);

	afx_msg void OnNewGroup();
	afx_msg void OnExpandGroup();
	afx_msg void OnReduceGroup();

	afx_msg void OnNewExpress();	
	afx_msg void OnEditExpress();

	afx_msg void OnDelExpress();
	afx_msg void OnDelGroup();

	afx_msg void OnDefaultPress();

	afx_msg void OnDelUnion();

	afx_msg void OnKillFocus( CWnd* pNewWnd );

	afx_msg LRESULT OnUserDragData(WPARAM wParam, LPARAM lParam);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	DECLARE_MESSAGE_MAP()

protected:
	// 公式编辑控件指针
	static class CCompileEditView*  m_pCompileEditView;
	// 公式帮助近件指针
	static class CCompileEditView*  m_pHelpView;
	

	/******************************************************************
	*	函数功能：	插入类型为nType的所有函树到节点名为g_strFunName[nType].m_strName的节点下
	*	函数参数：	short nType : [in] 函数类型(也为索引号)
	*	返 回 值：	
	*	调用依赖：	
	*	作    者：	
	*	完成日期：	2003年5月7日
	*	修改说明：	
	*****************************************************************/
	void		InsertFunByType(short nType);

	/******************************************************************
	*	函数功能：	插入类型为nType的所有公式到节点名为strName的节点下
	*	函数参数：	CString strName : [in] 要插入的根节点名
	*				short nType		: [in] 插入的公式类型
	*				short nImage	: [in] 插入的根结点的图标索引
	*	返 回 值：	HTREEITEM  : 返回的根结点句柄
	*	调用依赖：	
	*	作    者：	
	*	完成日期：	2003年5月7日
	*	修改说明：	
	*****************************************************************/
	HTREEITEM   InsertExpressByType(CString strName,short nType,short nImage);

	/******************************************************************
	*	函数功能：	设置注释信息到帮助的窗口里面
	*	函数参数：	HTREEITEM hItem : [in] 要设置帮助的项目句柄
	*	返 回 值：	
	*	调用依赖：	
	*	作    者：	
	*	完成日期：	2003年5月7日
	*	修改说明：	
	*****************************************************************/
	void		SetFunNotes(HTREEITEM hItem);

	/******************************************************************
	*	函数功能：	根据自身的类型做不同的处理
						Export : 向导入导出窗口发送公式数据信息
					EditFunRef : 向公式编辑器窗口发送函数名字串
					EditExpRef : 向公式编辑器窗口发送公式数据(CValue*)
					  Function : 向公式编辑控件发送函数信息数据HSAddStrCompileEdit*
					   Express : 打开公式编辑器窗口
	*	函数参数：	（无）
	*	返 回 值：	
	*	调用依赖：	
	*	作    者：	
	*	完成日期：	2003年5月7日
	*	修改说明：	
	*****************************************************************/
	void		AddFunToEdit();

public:
	CMapVariabile*  m_pInExpression;     // 外部表达式表

public:
	// 设置公式编辑控件及帮助控件
	void  Atatch(class CCompileEditView* pCompileEditView,class CCompileEditView* pHelpView);

public:

	/******************************************************************
	*	函数功能：	将所有类型的公式分类填充到控件中
	*	函数参数：	CMapVariabile* pExternFunction : [in] 无用
	*	返 回 值：	void 
	*	调用依赖：	InsertFunByType()
	*	作    者：	
	*	完成日期：	2003年5月7日
	*	修改说明：	
	*****************************************************************/
	void FillFunction(CMapVariabile* pExternFunction);

	/******************************************************************
	*	函数功能：	将所有公式按分类填充到控件中
	*	函数参数：	CMapVariabile* pExternExpression : [in] 无用
	*	返 回 值：	void 
	*	调用依赖：	InsertExpressByType()
	*	作    者：	
	*	完成日期：	2003年5月7日
	*	修改说明：	
	*****************************************************************/
	void FillExpress(CMapVariabile* pExternExpression);

	/******************************************************************
	*	函数功能：	将公式分类名为strName的所有公式填充到结点名为strName的根结点下
	*	函数参数：	CMapVariabile* pExternExpression : [in] 无用
	*				CString strName : [in] 分类名
	*				char cType		: [in] 公式类型
	*				short nImage	: [in] 列表的图标索引
	*	返 回 值：	void 
	*	调用依赖：	InsertExpressByType()
	*	作    者：	
	*	完成日期：	2003年5月7日
	*	修改说明：	
	*****************************************************************/
	void FillExpress(CMapVariabile* pExternExpression,CString strName,char cType,short nImage);

public:
	/******************************************************************
	*	函数功能：	是否可拖放
	*	函数参数：	HTREEITEM hSelectedItem : [in] 源项目句柄
	*				HTREEITEM hTDropItem    : [in] 目的项目句柄
	*	返 回 值：	BOOL : 可拖放(TRUE), 不可拖放(FALSE)
	*	调用依赖：	
	*	作    者：	
	*	完成日期：	2003年5月7日
	*	修改说明：	
	*****************************************************************/
	BOOL IsDrag(HTREEITEM hSelectedItem,HTREEITEM hTDropItem);

	// 树类型种类
	enum TreeType{ None/*正常*/,Function/*函数*/,Express/*公式*/,AlignWin/*浮动*/,EditFunRef/*函数(插入时用)*/,EditExpRef/*公式引用(插入时用)*/,Export/*导入导出*/,HSBrowser/*浏览*/ };
	enum CopyType { CopyNone     = 0x00010000, // 正常拷贝
					CopyDelete   = 0x00020000, // 先删除再拷贝
					CopyLoad     = 0x00040000, // 拷贝并且
					CopyNotGroup = 0x00080000, // 不要组
					CopyNoRoot	 = 0x00100000, // 不要根节点
					CopyMustHasChildren = 0x00200000, // 必须要有孩子节点的组
					CopyMask     = 0xFFFF0000  // 拷贝分类MASK
	              };

	// 树类型
	TreeType   m_treeType;
	// 树类型操作
	BOOL	   IsType(TreeType nType)  { return m_treeType == nType;}
	void	   SetType(TreeType nType) { m_treeType = nType; }


	/******************************************************************
	*	函数功能：	展开或收缩指定组下的所有组
	*	函数参数：	HTREEITEM hCurItem : [in] 指定的组,为NULL时取ROOT
	*				UINT nCode : [in] 操作方式, 有如下几种:
										 TVE_COLLAPSE  -- Collapses the list.
									TVE_COLLAPSERESET  -- Collapses the list and removes the child items.
										   TVE_EXPAND  -- Expands the list.
										   TVE_TOGGLE  -- Collapses the list if it is currently expanded or expands it if it is currently collapsed. 
	*	返 回 值：	void	  
	*	调用依赖：	Expand()
	*	作    者：	
	*	完成日期：	2003年5月7日
	*	修改说明：	
	*****************************************************************/
	void	   ExpandGroup(HTREEITEM hCurItem,UINT nCode);

	/******************************************************************
	*	函数功能：	查找组名strName是否已经存在
	*	函数参数：	CString strName		: [in] 要查找的组名
	*				BOOL bPrompt = TRUE : [in] 如果查找的组名存在,是否提示信息
	*	返 回 值：	HTREEITEM  : 找不到(NULL), 否则为找到项目的句柄
	*	调用依赖：	
	*	作    者：	
	*	完成日期：	2003年5月7日
	*	修改说明：	
	*****************************************************************/
	HTREEITEM  FindGroupName(CString strName,BOOL bPrompt = TRUE,HTREEITEM hRootItem = NULL);

	/******************************************************************
	*	函数功能：	查找组名strName在hParentItem子女中(不包含孙子 ??? )是否已经存在
					如果hParentItem为叶子结点则在其兄弟中寻找
	*	函数参数：	CString strName			: [in] 要查找的组名
	*				HTREEITEM hParentItem	: [in] 要查找的组句柄
	*	返 回 值：	HTREEITEM : 找不到(NULL), 否则为找到项目的句柄
	*	调用依赖：	
	*	作    者：	
	*	完成日期：	2003年5月7日
	*	修改说明：	
	*****************************************************************/
	HTREEITEM  FindNameInGroup(CString strName,HTREEITEM hParentItem);

	/******************************************************************
	*	函数功能：	删除项目hItem及所下属的所有子项目
	*	函数参数：	HTREEITEM hItem  : [in] 要删除的项目
	*				BOOL bRecycleBin : [in] 是否放入回收站(不真正删除)
	*	返 回 值：	void	  
	*	调用依赖：	
	*	作    者：	
	*	完成日期：	2003年5月7日
	*	修改说明：	
	*****************************************************************/
	void	   DelAll(HTREEITEM hItem,BOOL bRecycleBin);
	/* 删除所有没有孩子的树枝 */
	void	   DelAllNoChildren(HTREEITEM hItem = NULL);

	/******************************************************************
	*	函数功能：	删除项目数据数组里的公式信息(在使用也就是在IsUsed()就不能删除,我觉得这里有点问题 ??? )
	*	函数参数：	HTREEITEM hItem			: [in] 要删除的项目句柄
	*				BOOL bDelSource = TRUE  : [in] 是否删除公式体(全局)
	*	返 回 值：	void	  
	*	调用依赖：	
	*	作    者：	
	*	完成日期：	2003年5月7日
	*	修改说明：	
	*****************************************************************/
	void	   DelFromArray(HTREEITEM hItem,BOOL bDelSource = TRUE);

	/******************************************************************
	*	函数功能：	插入一个项目
	*	函数参数：	HTREEITEM hParent  : [in] 插入项目的父项目
	*				CString csItemName : [in] 项目名
	*				HTREEITEM hInsAfter : [in] 在hInsAfter后插入项目
	*				int iSelImage      : [in] 选中时显示的图标
	*				int iNonSelImage   : [in] 没选中时显示的图标
	*				long lParam	: [in] 关联的数据
	*	返 回 值：	HTREEITEM : 加入的项目句柄, 否则为NULL
	*	调用依赖：	
	*	作    者：	
	*	完成日期：	2003年5月7日
	*	修改说明：	
	*****************************************************************/
	HTREEITEM  AddItem( HTREEITEM hParent, CString csItemName, HTREEITEM hInsAfter, int iSelImage , int iNonSelImage, long lParam);

	/******************************************************************
	*	函数功能：	插入一个项目
	*	函数参数：	CString strName : [in] 项目名(同时也是公式表pExternExpression中的一个公式名)
	*				CMapVariabile* pExternExpression : [in] 外部公式表
	*	返 回 值：	BOOL : 成功(TRUE), 失败(FALSE)
	*	调用依赖：	
	*	作    者：	
	*	完成日期：	2003年5月7日
	*	修改说明：	
	*****************************************************************/
	BOOL	   AddItem(CString strName,CMapVariabile* pExternExpression);

	/******************************************************************
	*	函数功能：	插入一个项目
	*	函数参数：	CString strName : [in] 项目名
	*				CExpValue* pValue : [in] 公式数据
	*				CMapVariabile* pExternExpression : [in] 无用
	*	返 回 值：	BOOL : 成功(TRUE), 失败(FALSE)  
	*	调用依赖：	
	*	作    者：	
	*	完成日期：	2003年5月7日
	*	修改说明：	
	*****************************************************************/
	BOOL	   AddItem(CString strName,CExpValue* pValue,CMapVariabile* pExternExpression);

	/******************************************************************
	*	函数功能：	在hItem(有叶子)或hItem(无叶子)的父节点下插入一个项目,同时将公式类型设置为cType
	*	函数参数：	CString strName : [in] 项目名(同时也是公式表pExternExpression中的一个公式名)
	*				char cType		: [in] 新设置的公式类型 
	*				HTREEITEM hItem : [in] 要插入节点的父节点或是兄弟结点
	*				CMapVariabile* pExternExpression : [in] 外部公式表
	*	返 回 值：	HTREEITEM  : 加入的项目句柄, 否则为NULL
	*	调用依赖：	
	*	作    者：	
	*	完成日期：	2003年5月7日
	*	修改说明：	
	*****************************************************************/
	HTREEITEM  AddItem(CString strName,char cType,HTREEITEM hItem,CMapVariabile* pExternExpression);

	/******************************************************************
	*	函数功能：	将源项目hSrcTItem及其子孙项目(除了hSrcDestTItem外)拷贝到hDestTItem下
	*	函数参数：	HTREEITEM hSrcTItem	    : [in] 源项目
	*				HTREEITEM hDestTItem    : [in] 目标父项目
	*				HTREEITEM hSrcDestTItem : [in] 不拷贝的项目 
	*	返 回 值：	void 
	*	调用依赖：	
	*	作    者：	
	*	完成日期：	2003年5月7日
	*	修改说明：	
	*****************************************************************/
	void AddItem(HTREEITEM hSrcTItem,HTREEITEM hDestTItem,HTREEITEM hSrcDestTItem);

	/******************************************************************
	*	函数功能：	一般是由于公式改名,这里在hItem组或是hItem的兄弟中进行更新相应项的项目名更改
	*	函数参数：	CString strName : [in] 改名后的公式名
	*				HTREEITEM hItem : [in] 要改名的项目所在的组或是其兄弟
	*				CMapVariabile* pExternExpression : [in] 外部公式表
	*	返 回 值：	BOOL : 成功(TRUE), 失败(FALSE)  
	*	调用依赖：	
	*	作    者：	
	*	完成日期：	2003年5月7日
	*	修改说明：	
	*****************************************************************/
	BOOL	   UpDateItem(CString strName,HTREEITEM hItem,CMapVariabile* pExternExpression);

	/******************************************************************
	*	函数功能：	将pNewVal的公式类型设置为pOldVal的公式类型,并且,更新m_ayTreeGroup中,原来指向pOldVal的现在指向pNewVal
	*	函数参数：	CValue* pOldVal : [in] 旧公式
	*				CValue* pNewVal : [in] 新公式
	*	返 回 值：	BOOL : 成功(TRUE), 失败(FALSE)     
	*	调用依赖：	
	*	作    者：	
	*	完成日期：	2003年5月7日
	*	修改说明：	
	*****************************************************************/
	BOOL	   UpdateTreeItemData(CValue* pOldVal,CValue* pNewVal);

	/******************************************************************
	*	函数功能：	处到项目的图标索引
	*	函数参数：	HTREEITEM hSrcTItem : [in] 要操作的项目句柄
	*				int &nSelItemImage  : [in] 选中时的图标索引
	*				int &nNonSelItemImage : [in] 没选中时的图标索引
	*	返 回 值：	void 
	*	调用依赖：	
	*	作    者：	
	*	完成日期：	2003年5月7日
	*	修改说明：	
	*****************************************************************/
	void GetItemImages(HTREEITEM hSrcTItem, int &nSelItemImage, int &nNonSelItemImage);

	// 得到项目数据(当前控件所有)
	CArray<CTreeGroup*,CTreeGroup*>* GetTreeGroup() { return &m_ayTreeGroup; }

protected:
	// 拖放数据  -- 没用使用，优化后否删除  ???
	CTreeDropTarget m_CTreeDropTarget;

protected:
	// 项目数据 
	CArray<CTreeGroup*,CTreeGroup*> m_ayTreeGroup;

public:
	// 清空项目数据
	void Delete();
	// 清空项目数据同时清空显示的项目
	void DeleteTree();

public:
	// 公式保存结构
	struct LoadItemData
	{
		CExpValue* pExpValue; // 公式 

		CString	   strName;   // 公式名
		char	   nStatus;   // 结点状态与CTreeGroup::m_nStatus
		int		   nSelItemImage; // 选中时的图标索引
		int		   nNonSelItemImage; // 没选中时的图标索引

		char       nExpressType; // 公式类型

		UINT stateMask,state; // tree state

		LoadItemData()
		{
			pExpValue = NULL;
			stateMask = TVIS_BOLD;
			state = 0;
		}

		// 串行化
		void Serialize(CArchive& ar)
		{
			char cStatus = 0;
			if( ar.IsStoring() )
			{
				if( pExpValue != NULL )
				{
					// cStatus = 1;
					// ar << cStatus;
					pExpValue->Serialize(ar);
				}
				else
				{
					ar << cStatus;
				}
				ar << nExpressType;
				ar << strName;
				ar << nStatus;
				ar << nSelItemImage;
				ar << nNonSelItemImage;
			}
			else
			{
				ar >> cStatus;
				if(cStatus)
				{
					if( pExpValue != NULL )
					{
						delete pExpValue;
					}
					if( cStatus == 1 )
					{
						pExpValue = new CExpValue;
					}
					else if( cStatus == 2 )
					{
						pExpValue = new CExpValue;
						pExpValue->SetStyle(HX_UNION_EXPRESS);
					}
					pExpValue->Serialize(ar);
				}
				ar >> nExpressType;
				ar >> strName;
				ar >> nStatus;
				ar >> nSelItemImage;
				ar >> nNonSelItemImage;
			}
		}
	};
protected:
	/******************************************************************
	*	函数功能：	保存树hRootItem下的所有结点数据
	*	函数参数：	CArchive& ar		: [in] 串行化文件
	*				HTREEITEM hRootItem : [in] 要保存的树的树根
	*				LoadItemData* status: [in] 保存时使用的中间变量结构 
	*				int& nCountRef : [in/out] 保存的结点数
	*	返 回 值：	void	  
	*	调用依赖：	
	*	作    者：	
	*	完成日期：	2003年5月8日
	*	修改说明：	
	*****************************************************************/
	void	     Save(CArchive& ar,HTREEITEM hRootItem,LoadItemData* status,int& nCountRef);

	/******************************************************************
	*	函数功能：	加入m_ayTreeGroup并插入树hParentItem中 ( ??? 指针使用前没有判断 )
					根据status.m_strName从外部公式表中得到数据,如果找不到则当公式组处理
	*	函数参数：	HTREEITEM hParentItem : [in] 父结点
	*				LoadItemData& status  : [in] 读入的数据
	*				int nCopy = CopyNone  : [in] 公式类型
	*	返 回 值：	HTREEITEM : 插入树中的项目句柄,否则为NULL
	*	调用依赖：	
	*	作    者：	
	*	完成日期：	2003年5月8日
	*	修改说明：	
	*****************************************************************/
	HTREEITEM	 Load(HTREEITEM hParentItem,LoadItemData& status,int nCopy = CopyNone,DWORD dwStyle = 0);

	/******************************************************************
	*	函数功能：	将status加入数组m_ayTreeGroup并插入结点hParentItem下
					根据status.m_pData得到公式数据,替换外部公式表或是导入导出公式表中的公式数据)
	*	函数参数：	HTREEITEM hParentItem : [in] 父结点
	*				LoadItemData& status  : [in] 读入的数据
	*				int nCopy = CopyNone  : [in] 无用
	*	返 回 值：	HTREEITEM : 插入树中的项目句柄,否则为NULL
	*	调用依赖：	
	*	作    者：	
	*	完成日期：	2003年5月8日
	*	修改说明：	
	*****************************************************************/
	HTREEITEM	 Read(HTREEITEM hParentItem,LoadItemData& status,int nCopy = CopyNone,DWORD dwStyle = 0);

	/******************************************************************
	*	函数功能：	将树控件pTree下的树hRootItem的子孙拷贝当前控件下的hDesRootItem下面
	*	函数参数：	CTreeCtrlFormula* pTree	: [in] 源树控件
	*				HTREEITEM hRootItem : [in] 源树根结点
	*				LoadItemData* status: [in] 树结点数据中间变量 
	*				HTREEITEM hDesRootItem : [in] 目标树根结点
	*				int nCopy : [in] 拷贝方式 enum CopyType
	*	返 回 值：	void		 
	*	调用依赖：	
	*	作    者：	
	*	完成日期：	2003年5月8日
	*	修改说明：	
	*****************************************************************/
	void		 Copy(CTreeCtrlFormula* pTree,HTREEITEM hRootItem,LoadItemData* status,HTREEITEM hDesRootItem,int nCopy,DWORD dwStyle = 0);

	/******************************************************************
	*	函数功能：	串行化,这里只实现的保存(在导出时使用)
	*	函数参数：	CArchive& ar   : [in] 串行化对象
	*				int& nCountRef : [in] 保存的结点数据个数
	*	返 回 值：	void 
	*	调用依赖：	
	*	作    者：	
	*	完成日期：	2003年5月8日
	*	修改说明：	
	*****************************************************************/
	virtual void Serialize(CArchive& ar,int& nCountRef);

	// 保存文件名(除去后缀及路径)--一般是分类名
	CString		 m_strFileName; 

public:
	/******************************************************************
	*	函数功能：	保存或读入公式数据
	*	函数参数：	UINT nMode : [in] 保存(CArchive::store)还是读入(CArchive::load)
	*				CString strFileName = "" : [in] 文件名,为空时,是指 CGeneralHelper::GetSpecifyPath(CGeneralHelper::SETTING_PATH) +m_strFileName+".exp";
	*				CDlgFormulaMan* pFormulaMan = NULL : [in] 公式管理窗口
	*				DWORD dStyle = 0 : [in] 读入类型, HS_EXP_TEMPIN临时引入
	*	返 回 值：	BOOL : 成功(TRUE), 失败(FALSE)
	*	调用依赖：	
	*	作    者：	
	*	完成日期：	2003年5月13日
	*	修改说明：	
	*****************************************************************/
	BOOL SaveLoad(UINT nMode,CString strFileName = "",CDlgFormulaMan* pFormulaMan = NULL,DWORD dStyle = 0);	

public:
	/******************************************************************
	*	函数功能：	pCurGroup数据是否被使用
					当hRootItem为叶子时,在其兄弟中寻找,节点数据中是否包含pCurGroup,如果包含就返回TRUE,否则FALSE
					当hRootItem有子孙结点,在其子孙中寻找,节点数据中是否包含pCurGroup,如果包含就返回TRUE,否则FALSE
	*	函数参数：	HTREEITEM hRootItem  : [in] 目标树根结点
	*				CTreeGroup* pCurGroup: [in] 目标数据
	*	返 回 值：	BOOL : 找到也就是在使用(TRUE), 否则(FALSE)
	*	调用依赖：	
	*	作    者：	
	*	完成日期：	2003年5月8日
	*	修改说明：	
	*****************************************************************/
	BOOL IsUsed(HTREEITEM hRootItem,CTreeGroup* pCurGroup);
	// pCurGroup是否被在除了"回收站"外的树中使用
	BOOL IsUsed(CTreeGroup* pCurGroup);

	// 根据strName(分类名)得到公式类型
	char GetType(CString strName);
	// 得到hCurItem节点的公式类型如果节点的公式类型不存在则从其父节点查找,直找到最顶层结点或是找到类型为止
	char GetType(HTREEITEM& hCurItem);
	// 根据m_strName(分类名)得到公式类型
	char GetType();

public:
	/******************************************************************
	*	函数功能：	要所有根结点中查找是否存在结点名为strName的结点
	*	函数参数：	CString strName : [in] 要查找的结点名,为空时,找m_strName
	*	返 回 值：	HTREEITEM  : 找不到则为NULL, 否则为找到的根结点句柄
	*	调用依赖：	
	*	作    者：	
	*	完成日期：	2003年5月8日
	*	修改说明：	
	*****************************************************************/
	HTREEITEM   GetRootTree(CString strName);

	// 此函数不在使用,功能不是很明白
	HTREEITEM	GetCurSavePos(HTREEITEM hCurItem);

	/******************************************************************
	*	函数功能：	将公式树控件pTree中所有结点拷贝到当前树中
	*	函数参数：	CTreeCtrlFormula* pTree  : [in] 源树控件
	*				int nCopy = CopyNone : [in] 拷贝方式
	*	返 回 值：	
	*	调用依赖：	
	*	作    者：	
	*	完成日期：	2003年5月8日
	*	修改说明：	
	*****************************************************************/
	void		CopyTree(CTreeCtrlFormula* pTree,int nCopy = CopyNone,DWORD dwStyle = 0);

	/******************************************************************
	*	函数功能：	项目hCurItem是否在项目hDesItem下
	*	函数参数：	HTREEITEM hCurItem : [in] 要找的项目
	*				HTREEITEM hDesItem : [in] 在此项目下寻找 
	*	返 回 值：	BOOL : 项目hCurItem在项目hDesItem下返回TRUE, 否则返回FALSE
	*	调用依赖：	
	*	作    者：	
	*	完成日期：	2003年5月8日
	*	修改说明：	
	*****************************************************************/
	BOOL		OneItemInOtherItem(HTREEITEM hCurItem,HTREEITEM hDesItem);
	
	/******************************************************************
	*	函数功能：	将当前控件中,所有前两级结点都加入菜单中
	*	函数参数：	UINT nID	: [in] 菜单ID起始
	*				CMenu& popup: [in] 目标菜单
	*	返 回 值：	void	  
	*	调用依赖：	
	*	作    者：	
	*	完成日期：	2003年5月8日
	*	修改说明：	
	*****************************************************************/
	void	  AddTreeGroupToMenu(UINT nID,CMenu& popup);

	/******************************************************************
	*	函数功能：	得到树结点pRootTree下的子节点是否包含结点名为strRefName的结点
	*	函数参数：	CString strRefName  : [in] 要查找的结点名
	*				HTREEITEM pRootTree : [in] 根结点, 为NULL时找所有树
	*	返 回 值：	HTREEITEM  : NULL找不到,否则为找到的子结点句柄
	*	调用依赖：	
	*	作    者：	
	*	完成日期：	2003年5月8日
	*	修改说明：	
	*****************************************************************/
	HTREEITEM GetTreeItem(CString strRefName,HTREEITEM pRootTree);

	// 得到hItem项目的TVITEM::lParam数据
	DWORD	  GetItemData(HTREEITEM hItem);

	static CString g_strTech;			// 技术指标
	static CString g_strCondition;		// 条件选股
	static CString g_strExchange;		// 交易系统
	static CString g_strColorKLine;		// 五彩K线
	static CString g_strStrategy;		// 策略交易 20090302 YJT 增加
	static CString g_strRecycleBin;		// 回收站

	static CString g_strUnionCondition;	// 组合公式 
	static CString g_strJiben;			// 基本面

	static CString g_strFirstTechLine;  // 第1条指标线
	
	static CString g_strDefault;		// 默认指标 == "其它"

	static FunctionNameTable g_strFunName[]; // 函数分类名

	static CString m_strExpGroup;	// "分组";
	static CString m_strExpAll  ;	// "所有";
	static CString m_strExpComm ;	// "常用";
	static CString m_strExpUser ;	// "自编";

public:
	class CDlgFormulaMan* m_pFormulaMan;	// 公式管理器窗口

public:		
	/******************************************************************
	*	函数功能：	将根结点hRootItem下的所有子孙结点的公式类型全改为nExpressType
	*	函数参数：	HTREEITEM hRootItem : [in] 要操作的树
	*				int nExpressType    : [in] 公式类型
	*	返 回 值：	void 
	*	调用依赖：	
	*	作    者：	
	*	完成日期：	2003年5月8日
	*	修改说明：	
	*****************************************************************/
	void Save(HTREEITEM hRootItem,int nExpressType);

	/******************************************************************
	*	函数功能：	保存所有公式
	*	函数参数：	int nExpressType : [in] 无意义
	*				void* pData      : [in] 无意义
	*	返 回 值：	void 
	*	调用依赖：	SaveLoad()
	*	作    者：	
	*	完成日期：	2003年5月8日
	*	修改说明：	
	*****************************************************************/
	void Save(int nExpressType,void* pData);

protected:
	// 树名称
	CString m_strName; 
public:
	CString GetTreeName() { return m_strName; }

	// 当前树名是否为strName
	BOOL	  IsTree(CString strName);

	/******************************************************************
	*	函数功能：	插入一个项目并在项目数组m_ayTreeGroup中加入
	*	函数参数：	CString strRootName : [in] 根结点名称
	*				int nType			: [in] 公式类型	
	*				CString strSub		: [in] 子组结点名称
	*				CExpValue* pValue	: [in] 插入的公式
	*				HTREEITEM hItem = NULL : [in] 根结点句柄
	*				DWORD dStyle = 0 : [in] 是否具有TVIS_BOLD(加粗)风格
	*	返 回 值：	HTREEITEM : 插入后的结点句柄
	*	调用依赖：	
	*	作    者：	
	*	完成日期：	2003年5月8日
	*	修改说明：	
	*****************************************************************/
	HTREEITEM InputItem(CString strRootName,int nType,CString strSub,CExpValue* pValue,HTREEITEM hItem = NULL,DWORD dStyle = 0);

	/******************************************************************
	*	函数功能：	在根节点hItem(名为strRootName)下的strSub(可为空)组节点下插入一个函数节点
	*	函数参数：	CString strRootName : [in] 根节名
	*				int nImg			: [in] 无用
	*				CString strSub		: [in] 子组节点名, 为空则没组节点
	*				CFunExpresion* pValue : [in] 
	*				HTREEITEM hItem = NULL: [in] 根节点句柄
	*	返 回 值：	HTREEITEM : 插入的节点句柄, 不成功则为NULL
	*	调用依赖：	
	*	作    者：	
	*	完成日期：	2003年5月8日
	*	修改说明：	
	*****************************************************************/
	HTREEITEM InputFunItem(CString strRootName,int nImg,CString strSub,CFunExpresion* pValue,HTREEITEM hItem = NULL);

public:
	/******************************************************************
	*	函数功能：	读入公式数据
	*	函数参数：	CString strName : [in] 公式文件名
	*	返 回 值：	void 
	*	调用依赖：	SaveLoad();
	*	作    者：	
	*	完成日期：	2003年5月8日
	*	修改说明：	
	*****************************************************************/
	void LoadFromFile(CString strName);

	// 插入操作,全是调用基类,在此不做说明
public:
	HTREEITEM InsertItem(LPTVINSERTSTRUCT lpInsertStruct);
	HTREEITEM InsertItem(UINT nMask, LPCTSTR lpszItem, int nImage,
						int nSelectedImage, UINT nState, UINT nStateMask, LPARAM lParam,
						HTREEITEM hParent, HTREEITEM hInsertAfter);
	HTREEITEM InsertItem(LPCTSTR lpszItem, HTREEITEM hParent = TVI_ROOT,
						 HTREEITEM hInsertAfter = TVI_SORT);
	HTREEITEM InsertItem(LPCTSTR lpszItem, int nImage, int nSelectedImage,
						 HTREEITEM hParent = TVI_ROOT, HTREEITEM hInsertAfter = TVI_SORT);
	

	// check
public:
	/******************************************************************
	*	函数功能：	将hCurItem及其子孙所有的CHECK状态改为nCode
	*	函数参数：	HTREEITEM hCurItem : [in] 要操作的树节点
	*				UINT nCode         : [in] CHECK状态
	*	返 回 值：	void 
	*	调用依赖：	
	*	作    者：	
	*	完成日期：	2003年5月8日
	*	修改说明：	
	*****************************************************************/
	void Check(HTREEITEM hCurItem, UINT nCode);

	/******************************************************************
	*	函数功能：	将hTSelItem的所有子孙改成与hTSelItem一样的状态的同时,改变其父节点状态 ??? 
	*	函数参数：	HTREEITEM hTSelItem : [in] 要操作的树节点
	*	返 回 值：	void 
	*	调用依赖：	
	*	作    者：	
	*	完成日期：	2003年5月8日
	*	修改说明：	
	*****************************************************************/
	void Check(HTREEITEM hTSelItem);
	

	/******************************************************************
	*	函数功能：	hCurItem及其子节点中是否有CHECKED的节点存在
	*	函数参数：	HTREEITEM hCurItem : [in] 要操作的树节点
	*	返 回 值：	BOOL : 本身或其子节点中存在CHECKED节点,则返回TRUE, 否则返回FALSE
	*	调用依赖：	
	*	作    者：	
	*	完成日期：	2003年5月8日
	*	修改说明：	
	*****************************************************************/
	BOOL IsSubCheck( HTREEITEM hCurItem );

	/******************************************************************
	*	函数功能：	子节点改变状态后,同时改变其父节点状态
	*	函数参数：	HTREEITEM hCurItem : [in] 子节点
	*	返 回 值：	void 
	*	调用依赖：	
	*	作    者：	
	*	完成日期：	2003年5月8日
	*	修改说明：	
	*****************************************************************/
	void CheckParent( HTREEITEM hCurItem );

	/******************************************************************
	*	函数功能：	查看其兄弟节点的CHECK状态
	*	函数参数：	HTREEITEM hCurItem : [in] 要查看的节点
	*				int& nCount		   : [in] 总节点数
	*				int& nCheckCount   : [in] CHECKED的节点数
	*				int& nWaitCheck	   : [in] 部分CHECKED的节点数
	*	返 回 值：	BOOL : (int)后是节点的总数
	*	调用依赖：	
	*	作    者：	
	*	完成日期：	2003年5月8日
	*	修改说明：	
	*****************************************************************/
	BOOL IsSiblingItemCheck(HTREEITEM hCurItem,int& nCount,int& nCheckCount,int& nWaitCheck);


	/******************************************************************
	*	函数功能：	设置hItem的Check状态
	*	函数参数：	HTREEITEM hItem : [in] 要操作的句柄
	*				BOOL fCheck     : [in] 要设置的状态
	*	返 回 值：	BOOL : 成功(TRUE), 失败(FALSE)
	*	调用依赖：	
	*	作    者：	
	*	完成日期：	2003年5月8日
	*	修改说明：	
	*****************************************************************/
	BOOL TreeViewSetCheckState(HTREEITEM hItem, BOOL fCheck);
	/******************************************************************
	*	函数功能：	得到hItem的Check状态
	*	函数参数：	HTREEITEM hItem : [in] 要操作的句柄
	*	返 回 值：	BOOL : checked( TRUE ), 否则(FALSE)
	*	调用依赖：	
	*	作    者：	
	*	完成日期：	2003年5月8日
	*	修改说明：	
	*****************************************************************/
	BOOL TreeViewGetCheckState( HTREEITEM hItem);

// export / import
public:
	/******************************************************************
	*	函数功能：	将当前公式树控件里的所有结点所对应的公式加入全局公式表中
	*	函数参数：	CDlgFormulaMan* pFormulaMan : [in] 公式管理器窗口
	*				WORD wStyle				 : [in] 公式引入方式,EXTERN_EXPRESS_OVR则覆盖已有公式
	*	返 回 值：	void 
	*	调用依赖：	
	*	作    者：	
	*	完成日期：	2003年5月8日
	*	修改说明：	
	*****************************************************************/
	void LoadTree(CDlgFormulaMan* pFormulaMan,WORD wStyle);

	/******************************************************************
	*	函数功能：	将节点hRootItem下所有节点所对应的公式加入到全局公式表中(注意,节点本身不做处理)
	*	函数参数：	CDlgFormulaMan* pFormulaMan : [in] 公式管理器窗口
	*				WORD wStyle				 : [in] 公式引入方式,EXTERN_EXPRESS_OVR则覆盖已有公式
	*				HTREEITEM hRootItem		 : [in] 要处理的根结点
	*				LoadItemData* status	 : [in] 中间变量,起做用的是公式类型数据
	*	返 回 值：	void 
	*	调用依赖：	
	*	作    者：	
	*	完成日期：	2003年5月8日
	*	修改说明：	
	*****************************************************************/
	void Load(CDlgFormulaMan* pFormulaMan,WORD wStyle,HTREEITEM hRootItem,LoadItemData* status);

public:
	/******************************************************************
	*	函数功能：	展开并选中在节点hChildItem下包含的公式存在于CExpression::m_pExternExpression表中的项目
	*	函数参数：	DWORD dOperator	: [in] 必须为EXTERN_EXPRESS_SAME_NAME,否则不操作
	*				HTREEITEM hChildItem = NULL : [in] 要处理的根结点, 为NULL时指最根节点
	*	返 回 值：	long : 成功则返回EXTERN_EXPRESS_SAME_NAME, 否则返回其它
	*	调用依赖：	
	*	作    者：	
	*	完成日期：	2003年5月8日
	*	修改说明：	
	*****************************************************************/
	long OperatorTree(DWORD dOperator,HTREEITEM hChildItem = NULL);

	/******************************************************************
	*	函数功能：	将hitemDrag及其子孙项目移动到hitemDrop下
					注意, 移动后,源节点必须在接下去动行的语句中删除
					如:
						TransferItem(hItem, hItemTO);
						DeleteItem(hItem);
					才能算是一个完整的移动过程
	*	函数参数：	HTREEITEM hitemDrag : [in] 源根节点
	*				HTREEITEM hitemDrop : [in] 目标根节点
	*	返 回 值：	BOOL : 移动成功TRUE, 否则为FALSE 
	*	调用依赖：	
	*	作    者：	
	*	完成日期：	2003年5月8日
	*	修改说明：	
	*****************************************************************/
	BOOL		 TransferItem(HTREEITEM hitemDrag, HTREEITEM hitemDrop);

	/******************************************************************
	*	函数功能：	将叶子节点hitemDrag移动到
					组节点hitemDrop(为空时,表示移动到节点名为strInsertName的组节点下)下
	*	函数参数：	HTREEITEM hitemDrag : [in] 要移动的叶子
	*				HTREEITEM hitemDrop : [in] 目标组节点
	*				CString strInsertName = "" : [in] 目标组节点名
	*				DWORD dwMask = TVE_EXPAND  : [in] 如果为TVE_EXPAND则移动完成后展开移动后的节点
	*	返 回 值：	BOOL : 移动成功TRUE,否则为FALSE
	*	调用依赖：	
	*	作    者：	
	*	完成日期：	2003年5月8日
	*	修改说明：	
	*****************************************************************/
	BOOL	     ChangItemToGroup(HTREEITEM hitemDrag, HTREEITEM hitemDrop,
								  CString strInsertName = "",
								  DWORD dwMask = TVE_EXPAND);
// drag/drop
protected:

	/******************************************************************
	*	函数功能：	鼠标左键按下后处理,主要是拖放处理
	*	函数参数：	UINT nFlags	 : [in] 按下标志
	*				CPoint point : [in] 按下事件发生时光标指针所在位置
	*	返 回 值：	BOOL : 处理失败(FALSE), 成功(TRUE)		 
	*	调用依赖：	
	*	作    者：	
	*	完成日期：	2003年5月8日
	*	修改说明：	
	*****************************************************************/
	BOOL		 MouseDown(UINT nFlags, CPoint point);

	/******************************************************************
	*	函数功能：	hitemSuspectedParent是否为hitemChild的祖宗
	*	函数参数：	HTREEITEM hitemChild	: [in] 子节点
	*				HTREEITEM hitemSuspectedParent : [in] 祖节点
	*	返 回 值：	BOOL : hitemSuspectedParent是hitemChild的祖宗则返回TRUE, 否则FALSE
	*	调用依赖：	
	*	作    者：	
	*	完成日期：	2003年5月8日
	*	修改说明：	
	*****************************************************************/
	BOOL		 IsChildNodeOf(HTREEITEM hitemChild, HTREEITEM hitemSuspectedParent);

// find
public:

	/******************************************************************
	*	函数功能：	选中并且展开公式pExpression所在的组节点
	*	函数参数：	CExpression* pExpression : [in] 要展开的公式
	*				DWORD dMask : [in] TVE_EXPAND则展开,否则不展开
	*	返 回 值：	
	*	调用依赖：	
	*	作    者：	
	*	完成日期：	2003年5月8日
	*	修改说明：	
	*****************************************************************/
	void		Show(CExpression* pExpression,DWORD dMask); // show express

	/******************************************************************
	*	函数功能：	选中并且展开公式pExpression所在的组节点, 并返回组数据
	*	函数参数：	CExpression* pExpression : [in] 要展开的公式
	*				DWORD dMask = 0 : [in] TVE_EXPAND则展开, 否则不展开
	*	返 回 值：	CTreeGroup* : 项目数据(包含公式及一些结点句柄相关数据)
	*	调用依赖：	
	*	作    者：	
	*	完成日期：	2003年5月8日
	*	修改说明：	
	*****************************************************************/
	CTreeGroup*	Find(CExpression* pExpression,DWORD dMask = 0); // find express

	/******************************************************************
	*	函数功能：	得到组节点名为strGroupName的节点句柄,如果该节点不存在,则加入
	*	函数参数：	CString strGroupName : [in] 组节点名
	*				bool bPrompt = TRUE	 : [in] 无用
	*	返 回 值：	HTREEITEM : NULL得加入失败, 否则为加入或找到的节点句柄
	*	调用依赖：	
	*	作    者：	
	*	完成日期：	2003年5月8日
	*	修改说明：	
	*****************************************************************/
	HTREEITEM   NewGroup(CString strGroupName, bool bPrompt = TRUE);

public:
	// 得到当前公式树所包含的公式类型(底16位),及是否有基本面公式(高16位)
//	long GetExpressType();
	long GetHasExpressType(long* pJiben = NULL,long* pUnion = NULL);

	/******************************************************************
	*	函数功能：	初始化财务公式(从表格头定义中得到财务公式名及ID)
	*	函数参数：	CString strName : [in] 财务公式名
	*	返 回 值：	void 
	*	调用依赖：	
	*	作    者：	
	*	完成日期：	2003年5月8日
	*	修改说明：	
	*****************************************************************/
	void InitCWData(CString strName);

	BOOL IsEditExpress(HTREEITEM hHitItem);

	// gbq add, 检查公式是否需要密码
	BOOL ExPassword(HTREEITEM hItem);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HXTREECTRL_H__D334AF88_CFC7_11D4_903D_5254AB285D5A__INCLUDED_)
