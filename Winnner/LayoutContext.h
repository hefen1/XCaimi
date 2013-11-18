#pragma once

#include "tinyxml.h"
#include "LayoutView.h"
#include "HSSplitterWnd.h"
#include "WinnerApplication.h"
#include "..\Common\HSWinner.h"

class CWinnerView;
class CLayoutView;

class CNodeWndDescription:public CObject
{
public:
	IBizWindow*     m_pBizWindow;              //业务窗口句柄(在插件中创建)
	ILayoutWindow*  m_pChildLayoutWindow;      //子布局接口指针
	CString m_strCaption; //标题(显示在tab页中)
	CString m_strLocation;//业务窗口位置(插件名.窗口逻辑名)
	CString m_strSpliter;
	CHSParam m_param;

	CNodeWndDescription()
	{
		m_pBizWindow = NULL;
		m_pChildLayoutWindow = NULL;
		m_strCaption.Empty();
		m_strLocation.Empty();
		m_strSpliter.Empty();
	}

	~CNodeWndDescription()
	{
	}
};

typedef CArray<CNodeWndDescription*,CNodeWndDescription*>  CBizWndInfoArray;

//布局分割节点描述类
class CLayoutNode: public CObject
{
private:
	TCHAR             m_cLocation; //位置T-上;B-下;L-左;R-右
	int               m_nSize;     //大小
	BOOL              m_bSpliter;
	CLayoutNode*      m_left;      //左(上)子结点
	CLayoutNode*      m_right;     //右(下)子结点
	CLayoutView*      m_pView;     //结点上关联的视图
	TiXmlNode*        m_pXmlNode;

public:
	CBizWndInfoArray  m_bizWndArray; //业务窗口描述信息

	CLayoutNode();
	~CLayoutNode();

	//判断是否为叶子节点
	BOOL IsLeafNode();
	//返回分割类型:1-上下分割 2-左右分割
	UINT GetSplitType();

	TCHAR GetLocation() { return m_cLocation;}
	void  SetLocation(TCHAR clocation) { m_cLocation = clocation;}

	int  GetSize() { return m_nSize;} 
	void SetSize(int nSize) { m_nSize = nSize;}

	CLayoutNode* GetLeftNode() { return m_left;}
	void         SetLeftNode(CLayoutNode* pNode) { m_left = pNode; }

	CLayoutNode* GetRightNode() { return m_right;}
	void         SetRightNode(CLayoutNode* pNode) { m_right = pNode; }

	CLayoutView* GetView() { return m_pView;}
	void         SetView(CLayoutView* pView) { m_pView = pView;}

	TiXmlNode*   GetXmlNode() { return m_pXmlNode;}
	void         SetXmlNode(TiXmlNode* pNode) { m_pXmlNode = pNode;}

	BOOL         HasSpliter() { return m_bSpliter;}
	void         EableSpliter(BOOL bEnable) { m_bSpliter = bEnable; }

	CNodeWndDescription* GetBizWndInfo(CString strCaption);
};

class CLayoutContext: public CObject
{
private:
	CString          m_strLayoutName; 
	CString          m_strFocusWnd;
	IBizWindow*      m_pFocusBizWnd;
	CLayoutNode      m_rootNode; //布局信息根节点
	CWinnerView*     m_pWinnerView;

	BOOL BuildLayoutTree(CLayoutNode* parent,TiXmlElement* pNode);
	void SetNodeInfo(CLayoutNode* pNode,TiXmlElement* pXmlElem);
	void ClearLayoutTree();
	void RemoveChildrenNode(CLayoutNode* pNode);
public:
	CLayoutContext(CString strLayoutName);
	~CLayoutContext(void);

	//加载布局信息
	BOOL LoadLayoutInfo(CString strLayoutName);
	//保存布局信息
	BOOL SaveLayoutInfo(CString strLayoutFile);
	//根据布局描述信息分割视图:pView-要分割的视图
	BOOL SplitWindow(CLayoutView *pRootView,CRect rect);
	//
	void NotifyFocusWindow(BOOL bPost = FALSE);
	void NotifyToBizWindows(UINT msg,WPARAM wp, LPARAM lp,BOOL bPost = FALSE);
	//取视图相关布局分割节点描述
	CLayoutNode* GetLayoutNode(CLayoutView *pView);
	//设置根节点对应的视图
	void SetRootView(CLayoutView* pView);
	void SetWinnerView(CWinnerView* pWinnerView);

	//获取布局窗口接口指针
	ILayoutWindow* GetLayoutWindow(CString strLayoutName = _T(""));
};
