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
	IBizWindow*     m_pBizWindow;              //ҵ�񴰿ھ��(�ڲ���д���)
	ILayoutWindow*  m_pChildLayoutWindow;      //�Ӳ��ֽӿ�ָ��
	CString m_strCaption; //����(��ʾ��tabҳ��)
	CString m_strLocation;//ҵ�񴰿�λ��(�����.�����߼���)
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

//���ַָ�ڵ�������
class CLayoutNode: public CObject
{
private:
	TCHAR             m_cLocation; //λ��T-��;B-��;L-��;R-��
	int               m_nSize;     //��С
	BOOL              m_bSpliter;
	CLayoutNode*      m_left;      //��(��)�ӽ��
	CLayoutNode*      m_right;     //��(��)�ӽ��
	CLayoutView*      m_pView;     //����Ϲ�������ͼ
	TiXmlNode*        m_pXmlNode;

public:
	CBizWndInfoArray  m_bizWndArray; //ҵ�񴰿�������Ϣ

	CLayoutNode();
	~CLayoutNode();

	//�ж��Ƿ�ΪҶ�ӽڵ�
	BOOL IsLeafNode();
	//���طָ�����:1-���·ָ� 2-���ҷָ�
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
	CLayoutNode      m_rootNode; //������Ϣ���ڵ�
	CWinnerView*     m_pWinnerView;

	BOOL BuildLayoutTree(CLayoutNode* parent,TiXmlElement* pNode);
	void SetNodeInfo(CLayoutNode* pNode,TiXmlElement* pXmlElem);
	void ClearLayoutTree();
	void RemoveChildrenNode(CLayoutNode* pNode);
public:
	CLayoutContext(CString strLayoutName);
	~CLayoutContext(void);

	//���ز�����Ϣ
	BOOL LoadLayoutInfo(CString strLayoutName);
	//���沼����Ϣ
	BOOL SaveLayoutInfo(CString strLayoutFile);
	//���ݲ���������Ϣ�ָ���ͼ:pView-Ҫ�ָ����ͼ
	BOOL SplitWindow(CLayoutView *pRootView,CRect rect);
	//
	void NotifyFocusWindow(BOOL bPost = FALSE);
	void NotifyToBizWindows(UINT msg,WPARAM wp, LPARAM lp,BOOL bPost = FALSE);
	//ȡ��ͼ��ز��ַָ�ڵ�����
	CLayoutNode* GetLayoutNode(CLayoutView *pView);
	//���ø��ڵ��Ӧ����ͼ
	void SetRootView(CLayoutView* pView);
	void SetWinnerView(CWinnerView* pWinnerView);

	//��ȡ���ִ��ڽӿ�ָ��
	ILayoutWindow* GetLayoutWindow(CString strLayoutName = _T(""));
};
