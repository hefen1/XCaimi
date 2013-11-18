#include "StdAfx.h"
#include "LayoutContext.h"
#include "WinnerView.h"
#include ".\MessagePromptDialog.h"

#include "..\HelperLib\GeneralHelper.h"

#include <stack>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace std;

CLayoutNode::CLayoutNode()
{
	m_cLocation = 0;
	m_nSize = 0;
	m_bSpliter = TRUE;
	m_left = NULL;
	m_right = NULL;
	m_pView = NULL;
	m_pXmlNode = NULL;
}

CLayoutNode::~CLayoutNode()
{
	for(int i=0; i < m_bizWndArray.GetCount(); i++)
	{
		CNodeWndDescription* pbd = m_bizWndArray[i];
		if(pbd)
			delete pbd;
	}
	m_bizWndArray.RemoveAll();
}

BOOL CLayoutNode::IsLeafNode()
{
	return m_left == NULL && m_right == NULL;
}

UINT CLayoutNode::GetSplitType()
{
	if ( m_left == NULL || m_right == NULL)
		return 0;

	CString str1(_T("TtBb"));
	if(str1.Find(m_left->GetLocation()) >= 0)
		return 1;

	CString str2(_T("RrLl"));
	if(str2.Find(m_left->GetLocation()) >= 0 )
		return 2;

	return 0;
}

CNodeWndDescription* CLayoutNode::GetBizWndInfo(CString strCaption)
{
	for(int i = 0 ; i < m_bizWndArray.GetCount() -1; i++)
	{
		CNodeWndDescription* pInfo = m_bizWndArray[i];
		if(pInfo->m_strCaption == strCaption )
			return pInfo;
	}
	return NULL;
}

CLayoutContext::CLayoutContext(CString strLayoutName)
{
	m_strLayoutName = strLayoutName;
	m_pWinnerView = NULL;
	m_pFocusBizWnd = NULL;
}

CLayoutContext::~CLayoutContext(void)
{
	ClearLayoutTree();
}

BOOL CLayoutContext::LoadLayoutInfo(CString strLayoutName)
{
	if(m_strLayoutName.IsEmpty())
		return FALSE;

	CString strAppPath = CGeneralHelper::GetAppPath();
	CString strLayoutFile;
	BOOL bFind = FALSE;
	//1�������Զ���
	strLayoutFile.Format("%s%s%s.xml",strAppPath, _T("layout\\self\\"),strLayoutName); 
	if( PathFileExists(strLayoutFile) ) 
	{
		bFind = TRUE;
	}
	//2�����ҽ��ײ���	
	if( FALSE == bFind) 
	{
		strLayoutFile.Format("%s%s%s.xml",strAppPath, _T("layout\\trade\\"),strLayoutName); 
		if(PathFileExists(strLayoutFile) )
			bFind = TRUE;
	}
	//3���������鲼��
	if( FALSE == bFind) 
	{
		strLayoutFile.Format("%s%s%s.xml",strAppPath, _T("layout\\quote\\"),strLayoutName); 
		if(PathFileExists(strLayoutFile) )
			bFind = TRUE;
	}
	
	TiXmlDocument xmldoc;
	TiXmlElement* pRoot = NULL;
	if( bFind )
	{
		if( !xmldoc.LoadFile(strLayoutFile) )
		{
			CString strError;
			strError.Format(_T("���������ļ�(%s)ʧ��.\r\n��:%d ��:%d\r\n��ʶԭ��:%s"),
				strLayoutFile,xmldoc.ErrorRow(),xmldoc.ErrorCol(),xmldoc.ErrorDesc());
			CMessagePromptDialog dlg;
			dlg.ShowMsg(CMessagePromptDialog::mtError,strError);
			return FALSE;
		}
		pRoot = xmldoc.FirstChildElement(_T("Layout"));
	}
	else //ϵͳ���岼��
	{
		strLayoutFile = CGeneralHelper::GetAppPath() + _T("layout\\sys_layout.xml");
		if( !xmldoc.LoadFile(strLayoutFile) )
		{
			CString strError;
			strError.Format(_T("����ϵͳ�����ļ�ʧ��.\r\n��:%d ��:%d\r\n��ʶԭ��:%s"),
				xmldoc.ErrorRow(),xmldoc.ErrorCol(),xmldoc.ErrorDesc());
			CMessagePromptDialog dlg;
			dlg.ShowMsg(CMessagePromptDialog::mtError,strError);
			return FALSE;
		}
		pRoot = xmldoc.FirstChildElement(_T("SysLayout"));
		pRoot = pRoot->FirstChildElement(strLayoutName);
	}
	
	ASSERT(pRoot != NULL);
	m_strFocusWnd = pRoot->ToElement()->Attribute(_T("focus_window")); //��ǰ���ֵ�Ĭ�Ͻ��㴰��
//	CString strSpliter = pRoot->ToElement()->Attribute(_T("spliter")); //����ָ������
// 	if(strSpliter == _T("no"))
// 		m_rootNode.EableSpliter(FALSE);

	TiXmlElement* pFistNode = pRoot->FirstChildElement("Node");
	CString strTemp = pFistNode->Attribute("location");
	if(strTemp.IsEmpty())
	{
		SetNodeInfo(&m_rootNode,pFistNode);
		return TRUE;
	}
	else
		return BuildLayoutTree(&m_rootNode,pFistNode);
}

BOOL CLayoutContext::SaveLayoutInfo(CString strLayoutFile)
{
	const TCHAR* str =	"<?xml version=\"1.0\" encoding=\"GB2312\"?>\t<Layout>\n</Layout>";
 	TiXmlDocument    xmldoc; 
 	xmldoc.SetTabSize( 2 );
	xmldoc.Parse( str );

	TiXmlHandle docHandle( &xmldoc );
	TiXmlHandle roomHandle = docHandle.FirstChildElement( "Layout" );
	TiXmlNode* pXmlNode = roomHandle.Node();
	assert( pXmlNode );

	CLayoutNode* node = &m_rootNode;
	node->SetXmlNode(pXmlNode);
	stack<CLayoutNode*> s;

	//���ڵ㲻Ϊ�ջ��߶�ջ��Ϊ�ս���ѭ����
	while ((NULL != node) || !s.empty())
	{
		if (NULL != node)
		{
			pXmlNode = node->GetXmlNode();

			if(!node->IsLeafNode())
			{
				CString strLocation,strSize;
				CLayoutNode* pLeft = node->GetLeftNode();
				TiXmlElement leftItem( _T("Node") );	
				strLocation.Format("%c", pLeft->GetLocation());
				leftItem.SetAttribute(_T("location"),strLocation);
				strSize.Format("%d",pLeft->GetSize());
				leftItem.SetAttribute(_T("size"),strSize);

				CLayoutNode* pRight = node->GetRightNode();
				TiXmlElement rightItem( _T("Node") );	
				strLocation.Format("%c", pRight->GetLocation());
				rightItem.SetAttribute(_T("location"),strLocation);
				strSize.Format("%d",pRight->GetSize());
				rightItem.SetAttribute(_T("size"),strSize);


				TiXmlNode* pL = pXmlNode->InsertEndChild(leftItem);
				TiXmlNode* pR = pXmlNode->InsertEndChild(rightItem);

				pLeft->SetXmlNode(pL);
				pRight->SetXmlNode(pR);
			}
			else
			{
				if(node == &m_rootNode)
				{
					CString strLocation,strSize;
					TiXmlElement rootItem( _T("Node") );	
					strLocation.Format("%c", node->GetLocation());
					rootItem.SetAttribute(_T("location"),strLocation);
					strSize.Format("%d",node->GetSize());
					rootItem.SetAttribute(_T("size"),strSize);
					TiXmlNode* pRoot = pXmlNode->InsertEndChild(rootItem);
					node->SetXmlNode(pRoot);
				}
				for(int i=0;i< node->m_bizWndArray.GetSize(); i++)
				{
					CNodeWndDescription *pInfo = node->m_bizWndArray[i];
					TiXmlElement item( _T("Window") );
					item.SetAttribute(_T(_T("caption")),pInfo->m_strCaption);
					item.SetAttribute(_T("location"),pInfo->m_strLocation);
					node->GetXmlNode()->InsertEndChild(item);
				}
			}
		
			//�ڵ����ջ
			s.push(node);
			node = node->GetLeftNode();		
		}
		else 
		{
			//���nodeΪ�գ�ָ���ջ�Ķ��ˣ��������ýڵ�
			node = s.top();
			s.pop();
			node = node->GetRightNode();		
		}
	}

	xmldoc.SaveFile(strLayoutFile);
	return TRUE;
}

void CLayoutContext::SetNodeInfo(CLayoutNode* pNode,TiXmlElement* pXmlElem)
{
	if(pNode == NULL || pXmlElem == NULL)
		return;
	
	const char* pLocation = pXmlElem->Attribute("location");
	if(pLocation)
		pNode->SetLocation(pLocation[0]);

	const char* pSize = pXmlElem->Attribute("size");
	if(pSize)
		pNode->SetSize(atoi(pSize));

	CString strSpliter = pXmlElem->Attribute(_T("spliter"));
	if(strSpliter == _T("no"))
		pNode->EableSpliter(FALSE);
	else
		pNode->EableSpliter(TRUE);

	TiXmlNode* pWndXmlNode = pXmlElem->FirstChild(_T("Window"));
	while(pWndXmlNode)
	{
		CNodeWndDescription  *pbd= new CNodeWndDescription();
		TiXmlElement* pWndXmlElem = pWndXmlNode->ToElement();
		pbd->m_strCaption = pWndXmlElem->Attribute(_T("caption"));
		pbd->m_strLocation = pWndXmlElem->Attribute(_T("location"));

		//�����Զ����ʽ��xml��������ڲ����н���--���ڸ��Ӳ�����ʽ��
		CString strText = pWndXmlElem->GetText();
		pbd->m_param.SetXMLData(strText);
		//���ò���
		TiXmlElement* pParamElem = pWndXmlElem->FirstChildElement(_T("Param"));
		while(pParamElem != NULL)
		{
			CString strParamName = pParamElem->Attribute(_T("name"));
			CString strParamValue = pParamElem->Attribute(_T("value"));
			pbd->m_param.AddParam(strParamName,strParamValue);

			pParamElem = pParamElem->NextSiblingElement(_T("Param"));
		}
		pNode->m_bizWndArray.Add(pbd);
		pWndXmlNode = pWndXmlNode->NextSibling(_T("Window"));
	}
}

BOOL CLayoutContext::BuildLayoutTree(CLayoutNode* parent,TiXmlElement* pNode)
{
	TiXmlElement* pElem = pNode;
	while( pElem != NULL)
	{
		CLayoutNode* pNode = new CLayoutNode();
		SetNodeInfo(pNode,pElem);

		TCHAR cLoc = pNode->GetLocation();
		CString strFilter1(_T("RrLlTtBb"));
		ASSERT( strFilter1.Find(cLoc) >= 0);

		CString strFilter2(_T("LlTt"));
		if (strFilter2.Find(cLoc) >= 0)
			parent->SetLeftNode(pNode);
		else 
			parent->SetRightNode(pNode);

		if(!pElem->NoChildren() && pElem->FirstChild("Node") != NULL)
		{
			TiXmlElement* pChildElem = pElem->FirstChild("Node")->ToElement();
			BuildLayoutTree(pNode,pChildElem);
		}
		pElem=pElem->NextSiblingElement();
	}
	return TRUE;
}

void CLayoutContext::ClearLayoutTree()
{
	CLayoutContext::RemoveChildrenNode(&m_rootNode);
}

void CLayoutContext::RemoveChildrenNode(CLayoutNode* pNode)
{
	if(!pNode->IsLeafNode())
	{
		CLayoutNode *pLeftNode = pNode->GetLeftNode();
		RemoveChildrenNode(pLeftNode);

		CLayoutNode *pRightNode = pNode->GetRightNode();
		RemoveChildrenNode(pRightNode);

		pNode->SetLeftNode(NULL);
		pNode->SetRightNode(NULL);
	}

	if(pNode != &m_rootNode)
		delete pNode;
}

BOOL CLayoutContext::SplitWindow(CLayoutView *pRootView,CRect rect)
{
	if(m_rootNode.GetView() == NULL)
		SetRootView(pRootView);
	CLayoutNode* node = &m_rootNode;
	stack<CLayoutNode*> s;

	//���ڵ㲻Ϊ�ջ��߶�ջ��Ϊ�ս���ѭ����
	while ((NULL != node) || !s.empty())
	{
		if (NULL != node)
		{
			if(!node->IsLeafNode())
			{
				UINT type = node->GetSplitType();
				ASSERT(type == 1 || type == 2);

				CLayoutView *pLayoutView = node->GetView();
				CHSSplitterWnd *pSplitter = pLayoutView->GetSplitterWnd();
				CRuntimeClass *pClass = RUNTIME_CLASS(CLayoutView);

				CLayoutView* pLeftView = NULL;
				CLayoutView* pRightView = NULL;
				pSplitter->EnableSpliter(node->HasSpliter());
				if(1 == type)
				{
					pSplitter->SplitWindow(CHSSplitterWnd::XSplit,pLayoutView,pClass,pClass);
					pLeftView = (CLayoutView*)pSplitter->GetPane(0,0);
					pRightView = (CLayoutView*)pSplitter->GetPane(1,0);	
				}
				else
				{
					pSplitter->SplitWindow(CHSSplitterWnd::YSplit,pLayoutView,pClass,pClass);
					pLeftView = (CLayoutView*)pSplitter->GetPane(0,0);
					pRightView = (CLayoutView*)pSplitter->GetPane(0,1);
				}
				pLeftView->SetLayoutContext(this);
				pRightView->SetLayoutContext(this);
				node->GetLeftNode()->SetView(pLeftView);
				node->GetRightNode()->SetView(pRightView);
			}
			else
			{
			}
			//�ڵ����ջ
			s.push(node);
			node->GetView()->AttachPluginWnd();

			//Ѱ��Ĭ�Ͻ��㴰��
			if(m_pFocusBizWnd == NULL)
			{
				for(int i = 0 ; i < node->m_bizWndArray.GetCount(); i++)
				{
					CNodeWndDescription *pNodeWndDsp = node->m_bizWndArray.GetAt(i);
					if (m_strFocusWnd.IsEmpty() || pNodeWndDsp->m_strCaption == m_strFocusWnd)
					{
						m_pFocusBizWnd = pNodeWndDsp->m_pBizWindow;
						break;
					}
				}
			}		
		    node = node->GetLeftNode();
		}
		else 
		{
			//���nodeΪ�գ�ָ���ջ�Ķ��ˣ��������ýڵ�
			node = s.top();
			s.pop();
			node = node->GetRightNode();
		}
	}
	return TRUE;
}

void CLayoutContext::NotifyFocusWindow(BOOL bPost /*= FALSE*/)
{
	if(m_pFocusBizWnd != NULL)
	{
		if(bPost)
			::PostMessage(m_pFocusBizWnd->GetHandle(),WM_BIZWINDOW_NOTIFY,BWN_FOCUS_WND,0);
		else
			::SendMessage(m_pFocusBizWnd->GetHandle(),WM_BIZWINDOW_NOTIFY,BWN_FOCUS_WND,0);
	}
}

void CLayoutContext::NotifyToBizWindows(UINT msg,WPARAM wp, LPARAM lp,BOOL bPost /*= FALSE*/)
{
	CLayoutNode* node = &m_rootNode;
	stack<CLayoutNode*> s;

	//���ڵ㲻Ϊ�ջ��߶�ջ��Ϊ�ս���ѭ����
	while ((NULL != node) || !s.empty())
	{
		if (NULL != node)
		{
			if(node->IsLeafNode())
			{
				for(int i=0; i < node->m_bizWndArray.GetCount(); i++)
				{
					CNodeWndDescription* pWndDescrip = node->m_bizWndArray.GetAt(i);
					if(pWndDescrip->m_pBizWindow)
					{
						if(wp == BWN_WINDOW_SHOW)
						{
							if(bPost)
								::PostMessage(pWndDescrip->m_pBizWindow->GetHandle(),msg,wp, i == 0);
							else
								::SendMessage(pWndDescrip->m_pBizWindow->GetHandle(),msg,wp, i == 0);
						}
						else
						{
							if(bPost)
								::PostMessage(pWndDescrip->m_pBizWindow->GetHandle(),msg,wp,lp);
							else
								::SendMessage(pWndDescrip->m_pBizWindow->GetHandle(),msg,wp,lp);
						}
					}
				}
			}
			//�ڵ����ջ
			s.push(node);		
			node = node->GetLeftNode();
		}
		else 
		{
			//���nodeΪ�գ�ָ���ջ�Ķ��ˣ��������ýڵ�
			node = s.top();
			s.pop();
			node = node->GetRightNode();
		}
	}
}

CLayoutNode* CLayoutContext::GetLayoutNode(CLayoutView *pView)
{
	stack<CLayoutNode*> s;
	CLayoutNode* node = &m_rootNode;
	while ((NULL != node) || !s.empty())
	{
		if (NULL != node)
		{
			if(node->GetView() == pView)
				return node;
			s.push(node);
			node = node->GetLeftNode();
		}
		else 
		{
			node = s.top();
			s.pop();
			node = node->GetRightNode();
		}
	}
	return NULL;
}

void CLayoutContext::SetRootView(CLayoutView* pView)
{
	m_rootNode.SetView(pView);
}

void CLayoutContext::SetWinnerView(CWinnerView* pWinnerView)
{
	m_pWinnerView = pWinnerView;
}

ILayoutWindow* CLayoutContext::GetLayoutWindow(CString strLayoutName/* = _T("")*/)
{
   if(strLayoutName.IsEmpty())
   {
	   return (ILayoutWindow*)m_pWinnerView;
   }
   else
   {
	   stack<CLayoutNode*> s;
	   CLayoutNode* node = &m_rootNode;
	   while ((NULL != node) || !s.empty())
	   {
		   if (NULL != node)
		   {
			   for(int i = 0; i < node->m_bizWndArray.GetCount(); i++)
			   {
			      CNodeWndDescription *pWndDescription = node->m_bizWndArray[i];
				  if( pWndDescription->m_strCaption == strLayoutName )
				  {
					  return node->m_bizWndArray[i]->m_pChildLayoutWindow;
				  }
			   }
			   s.push(node);
			   node = node->GetLeftNode();
		   }
		   else 
		   {
			   node = s.top();
			   s.pop();
			   node = node->GetRightNode();
		   }
	   }
	   return NULL;
   }
}