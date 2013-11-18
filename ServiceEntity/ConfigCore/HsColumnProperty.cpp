#include "StdAfx.h"
#include "HsColumnProperty.h"
#include "HsFileManager.h"
#include "Bridge.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CHsFileManager gFileManager; 

CHsColumnProperty::CHsColumnProperty(void)
{
	m_pReportColumnCfg = NULL;
	m_pDisplayColumnCfg = NULL;
	m_bInited = FALSE;

	m_mapColIDIndex.RemoveAll();
}

CHsColumnProperty::~CHsColumnProperty(void)
{
	Release(TRUE);
}

void CHsColumnProperty::Init()
{
	m_pReportColumnCfg  = gFileManager.GetReportColumnCfg();
	m_pDisplayColumnCfg = gFileManager.GetDispColumnCfg();
	if ( m_pReportColumnCfg && !::IsBadReadPtr(m_pReportColumnCfg, 1) 
		 && m_pDisplayColumnCfg && !::IsBadReadPtr(m_pDisplayColumnCfg, 1) 
	   )
	{
		//设置各个文件的根节点
		m_pReportColumnCfg->SetRootNode("ColumnProperty", "" , "", ResourceDoc);
		m_pReportColumnCfg->SetRootNode("ColumnProperty", "" , "", SystemDoc);
		m_pReportColumnCfg->SetRootNode("ColumnProperty", "" , "", UserDoc);
		
		m_pDisplayColumnCfg->SetRootNode("DispColumn", "", "", ResourceDoc);
		m_pDisplayColumnCfg->SetRootNode("DispColumn", "", "", SystemDoc);
		m_pDisplayColumnCfg->SetRootNode("DispColumn", "", "", UserDoc);
		
		InitAySys();
		m_bInited = TRUE;
	}
}

void CHsColumnProperty::InitAySys() // 初始化 系统列Tag名称 、系统列name属性名
{//DispColumn.xml

	CString strPath = "//DispColumn";
	TiXmlNode* pNode = m_pDisplayColumnCfg->GetNode(strPath, "", "", SystemDoc);
	if (pNode == NULL)
		pNode = m_pDisplayColumnCfg->GetNode(strPath, "", "", UserDoc);

	if (pNode && !::IsBadReadPtr(pNode, 1))
	{
		CString str;
		TiXmlNode* pChildNode = pNode->FirstChild();
		while(pChildNode)
		{
			//Tag名称
			str = pChildNode->Value();
			m_aySysTagName.Add(str);

			//Attr名称
			str = pChildNode->ToElement()->Attribute("name");
			m_aySysAttrName.Add(str);
			
			pChildNode = pChildNode->NextSibling();
		}
	}

	//////////////////////////////////////////////////////////////////////////
	// 将ReportColunm.xml里面的数据都加载进来
	strPath = "//ColumnProperty";
	pNode = m_pReportColumnCfg->GetNode(strPath, "", "", SystemDoc); 
	if (pNode == NULL)
		pNode = m_pReportColumnCfg->GetNode(strPath, "", "", ResourceDoc);
	if (pNode && !::IsBadReadPtr(pNode, 1))
	{
		TiXmlNode* pChildNode = pNode->FirstChild();
		while (pChildNode)
		{
			TiXmlNode* pItemNode = pChildNode->FirstChild();
			while (pItemNode)
			{
				GetColumn(pItemNode);	
				pItemNode = pItemNode->NextSibling();
			}
			pChildNode = pChildNode->NextSibling();
		}
	}
	//////////////////////////////////////////////////////////////////////////
}

BOOL CHsColumnProperty::Release( BOOL bSaveToFile /*= FALSE*/ )
{
	if (m_pReportColumnCfg)
	{
		m_pReportColumnCfg->Release(bSaveToFile);
	}

	if (m_pDisplayColumnCfg)
	{
		m_pDisplayColumnCfg->Release(bSaveToFile);
	}

	//map 释放的顺序不能改变 
	POSITION pos = m_mapColIDIndex.GetStartPosition();
	while(pos)
	{
		WORD nColorId;
		TableColProperty *pCol = NULL;
		m_mapColIDIndex.GetNextAssoc(pos,nColorId,(void*&)pCol);
		if (pCol)
		{
			delete pCol;
			pCol = NULL;
		}
	}
	m_mapColIDIndex.RemoveAll();


	pos = m_mapColPropGroup.GetStartPosition();
	while(pos)
	{
		CString strName;
		CColPropArray *pColAy = NULL;
		m_mapColPropGroup.GetNextAssoc(pos,strName,(void*&)pColAy);
		if (pColAy && !::IsBadReadPtr(pColAy, 1))
		{
			pColAy->RemoveAll();
			delete pColAy;
		}
	}
	m_mapColPropGroup.RemoveAll();

	pos = m_mapShowColGroup.GetStartPosition();
	while(pos)
	{
		CString strName;
		CUIntArray* pAy = NULL;
		m_mapShowColGroup.GetNextAssoc(pos, strName, (void*&)pAy);
		if (pAy && !::IsBadReadPtr(pAy, 1))
		{
			pAy->RemoveAll();
			delete pAy;
		}
	}
	m_mapShowColGroup.RemoveAll();

	return TRUE;
}

BOOL CHsColumnProperty::SaveToFile()
{
	if (m_pReportColumnCfg)
	{
		m_pReportColumnCfg->SaveToFile("");
	}
	if (m_pDisplayColumnCfg)
	{
		m_pDisplayColumnCfg->SaveToFile("");
	}
	return TRUE;
}

TiXmlNode* CHsColumnProperty::GetNodeByName(CConfigBase* pConfig, CString strPath, CString strName)
{

	if (pConfig == NULL || strPath.IsEmpty() || strName.IsEmpty())
	{
		return NULL;
	}

	TiXmlNode* pNode = pConfig->GetNode(strPath, "", "", UserDoc);
	if (pNode && !::IsBadReadPtr(pNode, 1))
	{
		TiXmlNode* pChildNode = pNode->FirstChild();
		pNode = NULL;
		CString strNodeAttr;
		CString strGroupName = GetGroupNameByTagName(strName);
		while (pChildNode)
		{
			strNodeAttr = pChildNode->ToElement()->Attribute("name");
			if (strNodeAttr.Compare(strGroupName) == 0)
			{
				pNode = pChildNode;
				break;
			}
			pChildNode = pChildNode->NextSibling();
		}
	}

	if (pNode == NULL)
	{
		pNode = pConfig->GetNode(strPath, "", "", SystemDoc);
		if (pNode == NULL)
		{
			pNode = pConfig->GetNode(strPath, "", "", ResourceDoc);
		}
		if (pNode && !::IsBadReadPtr(pNode, 1))
		{
			TiXmlNode* pChildNode = pNode->FirstChild();
			pNode = NULL;
			CString strNodeAttr;
			CString strGroupName = GetGroupNameByTagName(strName);
			while (pChildNode)
			{
				strNodeAttr = pChildNode->ToElement()->Attribute("name");
				if (strNodeAttr.Compare(strGroupName) == 0)
				{
					pNode = pChildNode;
					break;
				}
				pChildNode = pChildNode->NextSibling();
			}
		}
	}
	return pNode;
}

TiXmlNode* CHsColumnProperty::GetNodeByNameFromDisp(CString strName)
{
	CString strPath = "//DispColumn";
	return GetNodeByName(m_pDisplayColumnCfg, strPath, strName);
}

TiXmlNode* CHsColumnProperty::GetNodeByNameFromReport(CString strName)
{
	CString strPath = "//ColumnProperty";
	return GetNodeByName(m_pReportColumnCfg, strPath, strName);
}

CString CHsColumnProperty::GetGroupNameByTagName(CString strName)
{
	if (m_aySysTagName.GetCount() <= 0 || m_aySysAttrName.GetCount() <= 0)
	{
		return strName;
	}

	for (int i = 0; i < m_aySysTagName.GetCount(); i++)
	{
		if (m_aySysTagName.GetAt(i).Compare(strName) == 0)
		{
			return m_aySysAttrName.GetAt(i);
		}
	}
	return strName;
}

CString CHsColumnProperty::GetTagNameByGroupName(CString strName)
{
	if (m_aySysTagName.GetCount() <= 0 || m_aySysAttrName.GetCount() <= 0)
	{
		return strName;
	}

	for (int i = 0; i < m_aySysAttrName.GetCount(); i++)
	{
		if (m_aySysAttrName.GetAt(i).Compare(strName) == 0)
		{
			return m_aySysTagName.GetAt(i);
		}
	}
	return strName;
}

long CHsColumnProperty::GetGroupName(CConfigBase* pConfigBase, CString strPath, CStringArray& ayGroup)
{

	if (pConfigBase == NULL || strPath.IsEmpty())
	{
		return -1;
	}

	CMapStringToPtr mapTemp;

	// 先从用户文件读取显示列名称
	TiXmlNode* pNode = pConfigBase->GetNode(strPath, "", "", UserDoc);
	if (pNode && !::IsBadReadPtr(pNode, 1))
	{
		CString strName = "";
		TiXmlNode* pChildNode = pNode->FirstChild();
		while(pChildNode)
		{
			strName = pChildNode->ToElement()->Attribute("name");
			mapTemp.SetAt(strName, NULL);
			pChildNode = pChildNode->NextSibling();
		}
	}

	// 再从系统文件读取显示列名称 去除重复的
	pNode = pConfigBase->GetNode(strPath, "", "", SystemDoc);
	if (pNode == NULL)
	{	 // 系统文件读取不到 从资源文件读取
		pNode = pConfigBase->GetNode(strPath, "", "", ResourceDoc);
	}
	if (pNode && !::IsBadReadPtr(pNode, 1))
	{
		CString strName = "";
		TiXmlNode* pChildNode = pNode->FirstChild();
		while(pChildNode)
		{
			strName = pChildNode->ToElement()->Attribute("name");
			mapTemp.SetAt(strName, NULL);
			pChildNode = pChildNode->NextSibling();
		}
	}

	CString str = "";
	void* pNull;
	POSITION pos = mapTemp.GetStartPosition();
	while(pos)
	{
		mapTemp.GetNextAssoc(pos, str, pNull);
		ayGroup.Add(str);
	}

	return ayGroup.GetCount();
}

TableColProperty* CHsColumnProperty::GetColumn(TiXmlNode* pNode)
{
	if (pNode == NULL || IsBadReadPtr(pNode, 1))
	{
		return NULL;
	}
	TableColProperty* pCol = NULL;

	// 列ID
	int	nColumnID(0);
	pNode->ToElement()->Attribute("id", &nColumnID);   
	if (m_mapColIDIndex.Lookup(nColumnID, (void*&)pCol))
	{
		return pCol;
	}
	// 名字
	CString strName = pNode->ToElement()->Attribute("name");
	// 列状态 例如高亮
	CString strMask = pNode->ToElement()->Attribute("CellMask");	
	int	nCellMask = strtol(strMask, NULL, 16); //转化成16进制
	// 默认列宽度
	int	nWidth(0);
	pNode->ToElement()->Attribute("width", &nWidth);

	pCol = new TableColProperty(strName.GetString(), nCellMask, nColumnID, nWidth);
	m_mapColIDIndex.SetAt(nColumnID, (void*)pCol);

	return pCol;
}

TableColProperty* CHsColumnProperty::GetColumn( int nID)
{//ReportColumn.xml

	TableColProperty* pCol = NULL;
	if (m_mapColIDIndex.Lookup(nID, (void*&)pCol))
	{
		return pCol;
	}

	CString strPath = "//*/Item";
	TiXmlNode* pNode = m_pReportColumnCfg->GetNode(strPath, "id", nID);

	return GetColumn(pNode);
	
}

CString CHsColumnProperty::GetDefDispColGroupName()
{//DispColumn.xml

	CString strPath = "//DefaultDispColGroup";
	return m_pDisplayColumnCfg->GetNodeAttrString(strPath, "name");
}

CString CHsColumnProperty::GetDefaultColGroupName()
{//ReportColumn.xml

	CString strPath = "//ColumnProperty";
	return m_pReportColumnCfg->GetNodeAttrString(strPath, "DefaultGroup");
}

long CHsColumnProperty::GetAllColumnGroupName( CStringArray& ayGroup )
{//ReportColumn.xml

	CString strPath = "//ColumnProperty";
	return GetGroupName(m_pReportColumnCfg, strPath, ayGroup);
}

CColPropArray* CHsColumnProperty::GetColumnGroupProp( CString strGroupName )
{//ReportColumn.xml

	CColPropArray* pColAy = NULL;
	if (m_mapColPropGroup.Lookup(strGroupName, (void*&)pColAy))
	{
		return pColAy;
	}

	TiXmlNode* pNode = GetNodeByNameFromReport(strGroupName);
	if (pNode && !::IsBadReadPtr(pNode, 1))
	{
		pColAy = new CColPropArray;
		TiXmlNode* pChildNode = pNode->FirstChild();
		TableColProperty* pCol = NULL;
		while(pChildNode)
		{
			pCol = GetColumn(pChildNode);
			if (pCol)
			{
				pColAy->Add(pCol);
			}

			pChildNode = pChildNode->NextSibling();
		}
		m_mapColPropGroup.SetAt(strGroupName, (void*)pColAy);
	}

	return pColAy;
}

BOOL CHsColumnProperty::GetColumnGroupPropEx(CColPropIDIndex &mapProp, CString strGroupName)
{
	CColPropArray* pColAy = NULL;
	pColAy = GetColumnGroupProp(strGroupName);

	if (pColAy)
	{
		for (int i = 0; i < pColAy->GetCount(); i++)
		{
			TableColProperty* pProp = pColAy->GetAt(i);
			if (pProp)
			{
				mapProp.SetAt(pProp->m_ColumnID, pProp);
			}
		}
		return TRUE;
	}
	else
		return FALSE;
}

CString CHsColumnProperty::GetFinanceGroupName()
{//DispColumn.xml

	CString strPath = "//Finance";
	return m_pDisplayColumnCfg->GetNodeAttrString(strPath, "name", "");

}

long CHsColumnProperty::GetAllDispColTemplName( CStringArray& ayGroup )
{//DispColunm.xml

	CString strPath = "//DispColumn";
	return GetGroupName(m_pDisplayColumnCfg, strPath, ayGroup);
}

RefreshDispTextProp* CHsColumnProperty::GetQuoteRefreshProp()
{
	if (m_sRefreshDispTextProp.m_dwBkColor == 0)
	{
		m_sRefreshDispTextProp.m_dwBkColor = CBridge::GetHsColor()->GetQuotePriceChangeBKColor()/*RGB(140,90,120)*/;
	}
	if (m_sRefreshDispTextProp.m_dwFgColor == 0)
	{
		m_sRefreshDispTextProp.m_dwFgColor = CLR_DEFAULT;
	}
	if (m_sRefreshDispTextProp.m_pFont == NULL)
	{
		m_sRefreshDispTextProp.m_pFont = CBridge::GetHsFont()->GetQuoteRefreshFont();
	}
	return &m_sRefreshDispTextProp;
}


CString CHsColumnProperty::GetColumnName( int nID )
{//DispColunm.xml
	const TableColProperty* pColumnProperty = GetColumn(nID);
	if (pColumnProperty)
	{
		 return pColumnProperty->strName;
	}
	return "";
}

long CHsColumnProperty::GetColumnMask( int nID )
{//DispColunm.xml
	const TableColProperty* pColumnProperty = GetColumn(nID);
	if (pColumnProperty)
	{
		return pColumnProperty->m_nCellMask;
	}
	return -1;
}

long CHsColumnProperty::GetColumnWidth( int nID )
{//DispColunm.xml
	const TableColProperty* pColumnProperty = GetColumn(nID);
	if (pColumnProperty)
	{
		return pColumnProperty->m_nWidth;
	}
	return -1;
}

long CHsColumnProperty::GetDispColGroup( CString strGroup, CUIntArray*& pAyGroup )
{//DispColunm.xml

	if (m_mapShowColGroup.Lookup(strGroup, (void*&)pAyGroup))
	{
		return pAyGroup->GetCount(); 
	}

	TiXmlNode* pNode = GetNodeByNameFromDisp(strGroup); //先通过属性名 找到节点
	if (pNode && !::IsBadReadPtr(pNode, 1))
	{
		CUIntArray* pAy = new CUIntArray;
		TiXmlNode* pChildNode = pNode->FirstChild();
		int nValue(0);
		while(pChildNode)
		{
			pChildNode->ToElement()->Attribute("ColumnID", &nValue);
			pAy->Add(nValue);

			pChildNode->ToElement()->Attribute("ColumnWidth", &nValue);
			pAy->Add(nValue);

			pChildNode = pChildNode->NextSibling();
		}
		pAyGroup = pAy;
		m_mapShowColGroup.SetAt(strGroup, (void*)pAy);
		return pAy->GetCount();
	}
	return -1;
}

void CHsColumnProperty::SetDispColGroup( CString strGroup, CUIntArray* pAyGroup )
{//DispColunm.xml

	CUIntArray* pAyOldGroup = NULL;
	GetDispColGroup(strGroup, pAyOldGroup);
	
	if (pAyOldGroup == NULL)
	{
		return;
	}
	
	if (pAyGroup->GetCount() == pAyOldGroup->GetCount())
	{
		int i(0);
		for (i = 0; i < pAyOldGroup->GetCount(); i++)
		{
			if (pAyGroup->GetAt(i) != pAyOldGroup->GetAt(i))
			{
				break;
			}
		}

		if (i == pAyOldGroup->GetCount())
		{
			return;
		}
	}
	
	TiXmlNode* pNode = GetNodeByNameFromDisp(strGroup);
	if (pNode && !::IsBadReadPtr(pNode, 1))
	{
		// 确保在DispColunm.xml文件中 一定存在所需存放节点的所有祖先节点
 		CString strPath;
		strPath.Format("//DispColumn/%s", pNode->Value());
		m_pDisplayColumnCfg->SetNodeAttrString(strPath, "name", strGroup);
		pNode = m_pDisplayColumnCfg->GetNode(strPath, "name", strGroup, UserDoc);
		if (pNode && !::IsBadReadPtr(pNode, 1))
		{	
			//清除所有孩子
			pNode->Clear();
		
			//开始添加新的内容
			for (int i = 0; i < pAyGroup->GetCount(); i++)
			{
				TiXmlElement* pElement = new TiXmlElement("Item");
				pElement->SetAttribute("ColumnID", pAyGroup->GetAt(i++));  // ColumnID

				pElement->SetAttribute("ColumnWidth", pAyGroup->GetAt(i)); // ColumnWidth
				pNode->LinkEndChild(pElement);
			}
		}
	}

	CUIntArray* pAy = NULL;
	if (m_mapShowColGroup.Lookup(strGroup, (void*&)pAy))
	{
		pAy->RemoveAll();
		delete pAy;
	}
	m_mapShowColGroup.RemoveKey(strGroup);
}

BOOL CHsColumnProperty::AddDispColGroup( CString strGroup, CUIntArray* pAyGroup )
{//DispColunm.xml
	
	// 如果已经存在 则添加失败
	TiXmlNode* pNode = GetNodeByNameFromDisp(strGroup);
	if (pNode)
	{
		return FALSE;
	}

	CString strPath = "//DispColumn";
	pNode = m_pDisplayColumnCfg->SetNode(strPath);
	if (pNode && !::IsBadReadPtr(pNode, 1))
	{
		//pNode = m_pDisplayColumnCfg->GetNode(strPath, "", "", UserDoc);
		//开始添加新的内容
		for (int i = 0; i < pAyGroup->GetCount(); i++)
		{
			TiXmlElement* pElement = new TiXmlElement("Item");
			pElement->SetAttribute("ColumnID", pAyGroup->GetAt(i++));  // ColumnID

			pElement->SetAttribute("ColumnWidth", pAyGroup->GetAt(i)); // ColumnWidth

			pNode->LinkEndChild(pElement);
		}

		return TRUE;
	}
	
	return FALSE;
}

BOOL CHsColumnProperty::DelDispColGroup( CString strGroup)
{//DispColunm.xml

	for (int i = 0; i < m_aySysAttrName.GetCount(); i++)
	{
		if (m_aySysAttrName.GetAt(i).Compare(strGroup) == 0)
		{// 系统默认列无法删除
			return FALSE;
		}
	}

	TiXmlNode* pNode = GetNodeByNameFromDisp(strGroup);
	if (m_pDisplayColumnCfg->RemoveNode(pNode))
	{
		CUIntArray* pAy = NULL;
		if (m_mapShowColGroup.Lookup(strGroup, (void*&)pAy))
		{
			delete pAy;
		}

		m_pDisplayColumnCfg->GetXmlDocument(UserDoc)->SetModified(TRUE);
		return TRUE;
	}
	else
		return FALSE;
}