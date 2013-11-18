#include "StdAfx.h"
#include "HsScrollNews.h"
#include "HsFileManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CHsFileManager gFileManager; 

CHsScrollNews::CHsScrollNews(void)
{
	m_pConfigBase = NULL;    // �ļ�������ָ��
	m_bInited = FALSE;       // �Ƿ��ʼ��
	m_strRootPath = "//ScrollNews"; // �߼����ڵ�
}

CHsScrollNews::~CHsScrollNews(void)
{
	Release(TRUE);
}

void CHsScrollNews::Init()
{
	m_pConfigBase = gFileManager.GetSystemCfg();

	if (m_pConfigBase && !::IsBadReadPtr(m_pConfigBase, 1))
	{
		m_bInited = TRUE;
	}
}

BOOL CHsScrollNews::Release(BOOL bSaveToFile /*= FALSE */)
{
	if (m_pConfigBase)
	{
		m_pConfigBase->Release(bSaveToFile);
	}
	return TRUE;
}

BOOL CHsScrollNews::SaveToFile()
{
	if (m_pConfigBase)
	{
		m_pConfigBase->SaveToFile("");
	}
	
	return TRUE;
}

int	CHsScrollNews::IsDispThirdIndex()		    //�������Ƿ���ʾ
{
	CString strPath  = m_strRootPath + "/Status/ShowThirdIndex";
	return m_pConfigBase->GetNodeAttrInt(strPath, "value"); 
}

int CHsScrollNews::IsDispFourthIndex()         //�������Ƿ���ʾ
{
	CString strPath = m_strRootPath + "/Status/ShowFourthIndex";
	return m_pConfigBase->GetNodeAttrInt(strPath, "value");
}

int CHsScrollNews::IsScrollFixed()             //������Ƿ�̶���״̬����
{
	CString strPath = m_strRootPath + "/ScrollNewsBar/ScrollFixed";
	return m_pConfigBase->GetNodeAttrInt(strPath, "value");
}

int CHsScrollNews::IsScrollMultiLine()         //������Ƿ������ʾ 
{
	CString strPath = m_strRootPath + "/ScrollNewsBar/ScrollMultiLine";
	return m_pConfigBase->GetNodeAttrInt(strPath, "value");
}

int CHsScrollNews::GetShowScrollAlone()        //�������ʾ��ʽ
{
	CString strPath = m_strRootPath + "/ScrollNewsBar/ShowScrollAlone";
	return m_pConfigBase->GetNodeAttrInt(strPath, "value");
}

CString CHsScrollNews::GetStatusName()         //��ȡ����Ʊ�������
{
	CString strPath = m_strRootPath + "/Status/StatusName";
	return m_pConfigBase->GetNodeAttrString(strPath, "value");
}

int CHsScrollNews::GetStatusScrollSpeed()      //������� ��Ϊ��λ
{
	CString strPath = m_strRootPath + "/Status/scrollSpeed";
	return m_pConfigBase->GetNodeAttrInt(strPath, "value");
}

int CHsScrollNews::GetStockNewName( CStringArray &ayStockList ) //��ȡ��Ʊ������
{
	CString strPath = m_strRootPath + "/Status/StockName";
	TiXmlNode* pNode = m_pConfigBase->GetNode(strPath, "", "");

	if (pNode)
	{
		TiXmlNode* pChild = pNode->FirstChild();
		CString strCode = "";
		CString strNewName = "";
		while(pChild)
		{
			strCode = pChild->ToElement()->Attribute("code");
			ayStockList.Add(strCode);

			strNewName = pChild->ToElement()->Attribute("newName");
			ayStockList.Add(strNewName);

			pChild = pChild->NextSibling();
		}

		return ayStockList.GetCount();
	}

	return -1;

}

int CHsScrollNews::GetUpDownRate()    //�Ƿ���ʾ�ǵ���
{
	CString strPath = m_strRootPath + "/Status/UpDownRate";
	return m_pConfigBase->GetNodeAttrInt(strPath, "value");
}

int CHsScrollNews::GetTotal()         //�Ƿ���ʾ�ܶ�
{
	CString strPath = m_strRootPath + "/Status/Total";
	return m_pConfigBase->GetNodeAttrInt(strPath, "value");
}

int CHsScrollNews::GetShowNewPirce()  //�Ƿ���ʾ���³ɽ���
{
	CString strPath = m_strRootPath + "/Status/ShowNewPirce";
	return m_pConfigBase->GetNodeAttrInt(strPath, "value");
}

int CHsScrollNews::GetUpDown()        //�Ƿ���ʾ�ǵ�
{
	CString strPath = m_strRootPath + "/Status/UpDown";
	return m_pConfigBase->GetNodeAttrInt(strPath, "value");
}

int CHsScrollNews::GetScrollSpeed()   //��������ٶ� ����Ϊ��λ
{
	CString strPath = m_strRootPath + "/ScrollNewsBar/ScrollSpeed";
	return m_pConfigBase->GetNodeAttrInt(strPath, "value");
}

int CHsScrollNews::GetHorizonScrollWay() //����ˮƽ�������� 3: ����; 4������
{
	CString strPath = m_strRootPath + "/ScrollNewsBar/HorizonScrollWay";
	return m_pConfigBase->GetNodeAttrInt(strPath, "value");
}

int CHsScrollNews::GetVerticalScrollWay() //���鴹ֱ�������� 1: ����; 2������
{
	CString strPath = m_strRootPath + "/ScrollNewsBar/VerticalScrollWay";
	return m_pConfigBase->GetNodeAttrInt(strPath, "value");
}

int CHsScrollNews::GetClickWay()          //��ʾ���鷽ʽ 0������; 1��˫��
{
	CString strPath = m_strRootPath + "/ScrollNewsBar/clickWay";
	return m_pConfigBase->GetNodeAttrInt(strPath, "value");
}

void CHsScrollNews::GetScrollText( CStringArray &ayScrollText ) //��ȡ����ƹ�����ѯ
{
	CString strScrollTextFileName = CGeneralHelper::GetAppPath() + _T("Setting\\dynnews.ini");

	CStringArray aySectionEntry;
	if (CGeneralHelper::ReadConfigSectionEntry(NULL,aySectionEntry,strScrollTextFileName))
	{
		for(int i=0; i<aySectionEntry.GetCount(); i++)
		{
			CString strOneSection = aySectionEntry.GetAt(i);
			CStringArray ayContentSection;
			if(CGeneralHelper::ReadConfigSectionEntry(strOneSection,ayContentSection,strScrollTextFileName))
			{
				for(int i=0; i<ayContentSection.GetCount(); i++)
				{
					CString strOneContent = ayContentSection.GetAt(i);
					CString strItem;
					if(CGeneralHelper::ReadConfigString(strOneSection,strOneContent,strItem,strScrollTextFileName))
					{
						ayScrollText.Add(strItem);
					}
					else
						ayScrollText.Add("");
				}
			}

		}
	}
}

int CHsScrollNews::GetStockInfo( TiXmlNode *pNode ,CStringArray &ayStockList )
{
	if (pNode == NULL)
	{
		return -1;
	}

	TiXmlNode* pChild = pNode->FirstChild();
	CString strCode = "";
	CString strCodeType = "";
	while(pChild)
	{
		strCode = pChild->ToElement()->Attribute("code");
		ayStockList.Add(strCode);

		strCodeType = pChild->ToElement()->Attribute("codeType");
		ayStockList.Add(strCodeType);

		pChild = pChild->NextSibling();
	}

	return ayStockList.GetCount();
}

int CHsScrollNews::GetFirstListStockInfo( CStringArray &ayStockList )
{
	CString strPath = m_strRootPath + "/Status/stockUserInfo/FirstList";
	TiXmlNode* pNode = m_pConfigBase->GetNode(strPath, "", "");

	if (pNode)
	{
		return GetStockInfo(pNode, ayStockList);
	}

	return -1;
}

int CHsScrollNews::GetSecondListStockInfo( CStringArray &ayStockList )
{
	CString strPath = m_strRootPath + "/Status/stockUserInfo/SecondList";
	TiXmlNode* pNode = m_pConfigBase->GetNode(strPath, "", "");

	if (pNode)
	{
		return GetStockInfo(pNode, ayStockList);
	}

	return -1;
}

int CHsScrollNews::GetThirdListStockInfo( CStringArray &ayStockList )
{
	CString strPath = m_strRootPath + "/Status/stockUserInfo/ThirdList";
	TiXmlNode* pNode = m_pConfigBase->GetNode(strPath, "", "");

	if (pNode)
	{
		return GetStockInfo(pNode, ayStockList);
	}

	return -1;
}

int CHsScrollNews::GetFourthListStockInfo( CStringArray &ayStockList )
{
	CString strPath = m_strRootPath + "/Status/stockUserInfo/FourthList";
	TiXmlNode* pNode = m_pConfigBase->GetNode(strPath, "", "");

	if (pNode)
	{
		return GetStockInfo(pNode, ayStockList);
	}

	return -1;
}

BOOL CHsScrollNews::SetDispThirdIndex( int nValue )
{
	CString strPath = m_strRootPath + "/Status/ShowThirdIndex";
	return m_pConfigBase->SetNodeAttrInt(strPath, "value", nValue);
}

BOOL CHsScrollNews::SetDispFourthIndex( int nValue )
{
	CString strPath = m_strRootPath + "/Status/ShowFourthIndex";
	return m_pConfigBase->SetNodeAttrInt(strPath, "value", nValue);
}

BOOL CHsScrollNews::SetScrollFixed( int nValue )
{
	CString strPath = m_strRootPath + "/ScrollNewsBar/ScrollFixed";
	return m_pConfigBase->SetNodeAttrInt(strPath, "value", nValue);
}

BOOL CHsScrollNews::SetScrollMultiLine( int nValue )
{
	CString strPath = m_strRootPath + "/ScrollNewsBar/ScrollMultiLine";
	return m_pConfigBase->SetNodeAttrInt(strPath, "value", nValue);
}

BOOL CHsScrollNews::SetStatusScrollSpeed( int nValue )
{
	CString strPath = m_strRootPath + "/Status/scrollSpeed";
	return m_pConfigBase->SetNodeAttrInt(strPath, "value", nValue);	
}

BOOL CHsScrollNews::SetScrollSpeed( int nValue )
{
	CString strPath = m_strRootPath + "/ScrollNewsBar/ScrollSpeed";
	return m_pConfigBase->SetNodeAttrInt(strPath, "value", nValue);	
}

BOOL CHsScrollNews::SetHorizonScrollWay( int nValue )
{
	CString strPath = m_strRootPath + "/ScrollNewsBar/HorizonScrollWay";
	return m_pConfigBase->SetNodeAttrInt(strPath, "value", nValue);	
}

BOOL CHsScrollNews::SetVerticalScrollWay( int nValue )
{
	CString strPath = m_strRootPath + "/ScrollNewsBar/VerticalScrollWay";
	return m_pConfigBase->SetNodeAttrInt(strPath, "value", nValue);	
}

BOOL CHsScrollNews::SetClickWay( int nValue )
{
	CString strPath = m_strRootPath + "/ScrollNewsBar/clickWay";
	return m_pConfigBase->SetNodeAttrInt(strPath, "value", nValue);	
}

BOOL CHsScrollNews::SetStockList( const CString &strPath, const CString &strParentPath, const CStringArray &ayStockList )
{
	if (strPath.IsEmpty())
	{
		return FALSE;
	}

	//ȷ�����·��--strParentPath�µĽڵ�һ������
	m_pConfigBase->SetNodeAttrString(strPath, "code", "delete"); 

	TiXmlNode* pNode = m_pConfigBase->GetNode(strParentPath, "", "", UserDoc);
	if (pNode == NULL)
	{
		return FALSE;
	}

	//ȥ�����к���
	TiXmlNode* pChildNode = pNode->FirstChild();
	while (pChildNode)
	{
		pNode->RemoveChild(pChildNode);
		pChildNode = pNode->FirstChild();
	}

	//�����µĺ���
	CString str = "";
	for (int i = 0; i < ayStockList.GetCount(); i++)
	{
		TiXmlElement* pElement = new TiXmlElement("stock");
		
		str = ayStockList.GetAt(i++);
		pElement->SetAttribute("code", str);

		str = ayStockList.GetAt(i);
		pElement->SetAttribute("codeType", str);

		pNode->LinkEndChild(pElement);
	}

	return TRUE;

}
BOOL CHsScrollNews::SetFirstListStockInfo( const CStringArray &ayStockList )
{
	CString strPath = m_strRootPath + "/Status/stockUserInfo/FirstList/stock";
	CString strParentPath = m_strRootPath + "/Status/stockUserInfo/FirstList";
	return SetStockList(strPath, strParentPath, ayStockList);
}

BOOL CHsScrollNews::SetSecondListStockInfo( const CStringArray &ayStockList )
{
	CString strPath = m_strRootPath + "/Status/stockUserInfo/SecondList/stock";
	CString strParentPath = m_strRootPath + "/Status/stockUserInfo/SecondList";
	return SetStockList(strPath, strParentPath, ayStockList);
}

BOOL CHsScrollNews::SetThirdListStockInfo( const CStringArray &ayStockList )
{
	CString strPath = m_strRootPath + "/Status/stockUserInfo/ThirdList/stock";
	CString strParentPath = m_strRootPath + "/Status/stockUserInfo/ThirdList";
	return SetStockList(strPath, strParentPath, ayStockList);
}

BOOL CHsScrollNews::SetFourthListStockInfo( const CStringArray &ayStockList )
{
	CString strPath = m_strRootPath + "/Status/stockUserInfo/FourthList/stock";
	CString strParentPath = m_strRootPath + "/Status/stockUserInfo/FourthList";
	return SetStockList(strPath, strParentPath, ayStockList);
}
