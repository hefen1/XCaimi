#include "StdAfx.h"
#include "HsFont.h"
#include "..\..\HelperLib\GeneralHelper.h"
#include "configDefined.h"
#include "HsFileManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CHsFileManager gFileManager; 

CHsFont::CHsFont()
{
	m_pConfigBase = NULL;
	m_bInited = FALSE;

	m_QuoteDefaultFont.CreatePointFont(100, _T("����"));
	m_TradeDefaultFont.CreatePointFont(100, _T("����"));
}

CHsFont::~CHsFont()
{
	Release(TRUE);
}

void CHsFont::Init()
{
	m_pConfigBase = gFileManager.GetFontCfg();

	if (m_pConfigBase && !::IsBadReadPtr(m_pConfigBase, 1))
	{
		//���ø����ļ�����ĸ��ڵ�
		m_pConfigBase->SetRootNode("ClassicalFont", NULL, NULL, ResourceDoc);
		m_pConfigBase->SetRootNode("ClassicalFont", NULL, NULL, SystemDoc);
		m_pConfigBase->SetRootNode("ClassicalFont", NULL, NULL, UserDoc);
		m_bInited = TRUE;
	}
}


BOOL CHsFont::Release(BOOL bSaveToFile)
{
	if (m_pConfigBase)
	{
		m_pConfigBase->Release(bSaveToFile);
	}

	POSITION posColor = m_CurrFontMap.GetStartPosition();
	while(posColor)
	{
		WORD nColorId;
		FontProperty *pFont = NULL;
		m_CurrFontMap.GetNextAssoc(posColor,nColorId,(void*&)pFont);
		if (pFont)
		{
			delete pFont;
			pFont = NULL;
		}
	}
	m_CurrFontMap.RemoveAll();

	m_QuoteDefaultFont.DeleteObject();
	m_TradeDefaultFont.DeleteObject();
	return TRUE;
}
BOOL CHsFont::SaveToFile()
{
	if (m_pConfigBase)
	{
		m_pConfigBase->SaveToFile();
	}
	return TRUE;
}

BOOL CHsFont::SetFontNode(short nID, FontProperty* pFont)
{
	if (nID <= 0 || pFont == NULL || ::IsBadReadPtr(pFont, 1) || pFont->m_pFont == NULL || pFont->m_pFont->m_hObject == 0)
	{
		return FALSE;
	}

	CString strPath; //·��
	strPath.Format("//ClassicalFont/Group/ID%d", nID);

	BOOL bRet = FALSE;
	CString str = pFont->m_strName;      //����
	bRet = m_pConfigBase->SetNodeAttrString(strPath, "name", str);

	LOGFONT lFont;
	pFont->m_pFont->GetLogFont(&lFont);

	str = lFont.lfFaceName;              //��������
	bRet = m_pConfigBase->SetNodeAttrString(strPath, "FontName", str);

	str.Format("%d", -lFont.lfHeight);   //����߶�
	bRet = m_pConfigBase->SetNodeAttrString(strPath, "FontHeight", str);

	if (lFont.lfWeight == FW_BOLD)       //��������
	{
		str = "FONT_BOLD";
	}
	else
	{
		str = "FONT_NORMAL";
	}
	bRet = m_pConfigBase->SetNodeAttrString(strPath, "FontWeight", str);

                                         //�Ƿ�б��
	bRet = m_pConfigBase->SetNodeAttrInt(strPath, "FontItalic", (int)lFont.lfItalic);
                                         //�������
	bRet = m_pConfigBase->SetNodeAttrInt(strPath, "OutPre", (int)lFont.lfOutPrecision);
	                                     //ָ��������ַ�������
	bRet = m_pConfigBase->SetNodeAttrInt(strPath, "Pitch", (int)lFont.lfPitchAndFamily);
                                         //ָ���ַ���
	bRet = m_pConfigBase->SetNodeAttrInt(strPath, "ChartSet", (int)lFont.lfCharSet);

	return bRet;
}

TiXmlNode* CHsFont::GetFontNode( short nID, DocType nType /*= Default*/ )
{
	CString strPath;
	strPath.AppendFormat("//Group/ID%d", nID);

	return m_pConfigBase->GetNode(strPath, nType);
}

FontProperty* CHsFont::GetFontProperty(short nID)
{
	FontProperty* pFontProperty = NULL;
	//���ȴӻ���������Ѱ��
	if (m_CurrFontMap.Lookup(nID, (void*&)pFontProperty))
	{
		return pFontProperty;
	}

	//���ļ���Ѱ��
	TiXmlNode* pNode = NULL;
	pNode = GetFontNode(nID);
	if ( pNode != NULL && !::IsBadReadPtr(pNode, 1))
	{
		TiXmlElement* pElement = pNode->ToElement();

		int nId,nFontHeight,nFontItalic,nOutPre,nFontWeight=0;
		CString strName(pElement->Attribute("name"));
		CString strItem(pElement->Value());
		strItem = strItem.Right(strItem.GetLength() -2);
		
		nId =  CGeneralHelper::StringToLongSimple(strItem);		
		pElement->Attribute("FontHeight",&nFontHeight);
		pElement->Attribute("FontItalic",&nFontItalic);
		CString strFontName(pElement->Attribute("FontName"));
		CString strFontWeight(pElement->Attribute("FontWeight"));
		char *pFontName = strFontName.GetBuffer();

		if (strFontWeight.CompareNoCase("FONT_BOLD") == 0)
			nFontWeight = FONT_BOLD;
		else if (strFontWeight.CompareNoCase("FONT_NORMAL") == 0)
			nFontWeight = FONT_NORMAL;

		const char *pOutPre = pElement->Attribute("OutPre",&nOutPre);
		FontProperty *pFont = new FontProperty;
		if (!pOutPre)
			pFont->Set(nId,  CGeneralHelper::CreateFont(pFontName,nFontHeight,nFontWeight,nFontItalic), strName);
		else
		{
			int nPitch,nChartSet;
			pElement->Attribute("Pitch",&nPitch);
			pElement->Attribute("ChartSet",&nChartSet);
			pFont->Set(nId,  CGeneralHelper::CreateFontEx(pFontName,nFontHeight,nFontWeight,nFontItalic,nOutPre,nPitch,nChartSet), strName);
		}
		FontProperty *pFontExist = NULL;
		if(m_CurrFontMap.Lookup(nId,(void*&)pFontExist))
		{
			CWinnerApplication::FormatLog(ERROR_LOG_LEVEL,_T("winner"),_T("font item repetition. id=%d"),nId);
			if(pFontExist)
			{
				pFontExist->Free();
				delete pFontExist;
			}
		}
		m_CurrFontMap.SetAt(nId,(void*)pFont);
		strFontName.ReleaseBuffer();

		pFontProperty = pFont;
	}

	return pFontProperty;
}

 CFont* CHsFont::GetTipTitleFont()     //��ȡTIP��ʾ��������
{
	FontProperty* pFontProp = GetFontProperty(FONT_TipTitle);
	return pFontProp ? pFontProp->m_pFont : NULL;
}
 CFont* CHsFont::GetTipTxtFont()       //��ȡTIP��ʾ�ı�����
{
	FontProperty* pFontProp = GetFontProperty(FONT_TipTxt);
	return pFontProp ? pFontProp->m_pFont : NULL;
}

 CFont* CHsFont::GetQuoteTabFont()		//��ȡ���鱨�۱�ǩҳ����
{
	FontProperty* pFontProp = GetFontProperty(FONT_TabTxt);
	return pFontProp ? pFontProp->m_pFont : NULL;
}
 CFont* CHsFont::GetQuoteGuideTabFont()		//��ȡ���鱨���򵼱�ǩҳ����
{
	FontProperty* pFontProp = GetFontProperty(FONT_TabTxt);
	return pFontProp ? pFontProp->m_pFont : NULL;
}

 CFont* CHsFont::GetStatusFontProt()		//��ȡ״̬��ָ������
{
	FontProperty* pFontProp = GetFontProperty(FONT_StatusTxt);
	return pFontProp ? pFontProp->m_pFont : NULL;
}

 CFont* CHsFont::GetScrollFontProt()        //��ȡ�������������
{
	FontProperty* pFontProp = GetFontProperty(FONT_ScrollTxt);
	return pFontProp ? pFontProp->m_pFont : NULL;
}

 CFont* CHsFont::GetQuoteRefreshFont()     
{
	FontProperty* pFontProp = GetFontProperty(FONT_ReportRefreshTxt);
	return pFontProp ? pFontProp->m_pFont : NULL;
}

 CFont* CHsFont::GetQuoteColHeadFont()		// ��ȡ���鱨�۱�ͷ����
{
	FontProperty* pFontProp = GetFontProperty(FONT_ReportColumnName);
	return pFontProp ? pFontProp->m_pFont : NULL;
}

 CFont* CHsFont::GetQuoteStringFont()		// ��ȡ���鱨����������(������)
{
	FontProperty* pFontProp = GetFontProperty(FONT_ReportCodeName);
	return pFontProp ? pFontProp->m_pFont : NULL;
}

 CFont* CHsFont::GetQuoteCodeFont()		// ��ȡ���鱨�۹�Ʊ��������
{
	FontProperty* pFontProp = GetFontProperty(FONT_ReportCode);
	return pFontProp ? pFontProp->m_pFont : NULL;
}

 CFont* CHsFont::GetQuoteDynamicFont()		// ��ȡ���鱨�۶�̬�ı�����
{
	FontProperty* pFontProp = GetFontProperty(FONT_ReportDynamicTxt);
	return pFontProp ? pFontProp->m_pFont : NULL;
}

 CFont* CHsFont::GetQuoteFixedFont()		// ��ȡ���鱨�۹̶��ı����壨��
{
	FontProperty* pFontProp = GetFontProperty(FONT_ReportfixedTxt);
	return pFontProp ? pFontProp->m_pFont : NULL;
}

CFont* CHsFont::GetRTableFixedFont()       // ��ȡ��Сͼ�ֶ�����
{
	FontProperty* pFontProp = GetFontProperty(FONT_RightViewFixed);
	return pFontProp ? pFontProp->m_pFont : NULL;
}

CFont* CHsFont::GetRTableDynamicalFont()     // ��ȡ��Сͼ��̬�ֶ�����
{
	FontProperty* pFontProp = GetFontProperty(FONT_RightViewDynamic);
	return pFontProp ? pFontProp->m_pFont : NULL;
}

CFont* CHsFont::GetRTableTitleCodeFont()     // ��ȡ��Сͼ�����Ʊ��������
{
	FontProperty* pFontProp = GetFontProperty(FONT_RightViewCode);
	return pFontProp ? pFontProp->m_pFont : NULL;
}

CFont* CHsFont::GetRTableTitleNameFont()     // ��ȡ��Сͼ�����Ʊ��������
{
	FontProperty* pFontProp = GetFontProperty(FONT_RightViewCodeName);
	return pFontProp ? pFontProp->m_pFont : NULL;
}
CFont* CHsFont::GetRTableFinanceFont()       // ��ȡ��Сͼ���ֵҳ�ı�
{
	FontProperty* pFontProp = GetFontProperty(FONT_RightViewFinace);
	return pFontProp ? pFontProp->m_pFont : NULL;
}

CFont* CHsFont::GetRTableTimeFont()        // ��ȡ��Сͼʱ��������
{
	FontProperty* pFontProp = GetFontProperty(FONT_DetailTradeTime);
	return pFontProp ? pFontProp->m_pFont : NULL;
}

//��ȡͼ���ǩҳ����
CFont* CHsFont::GetDiagramLayoutTabFont()   
{
	FontProperty* pFontProp = GetFontProperty(FONT_DiagramLayoutTab);
	return pFontProp ? pFontProp->m_pFont : NULL;
}

CFont* CHsFont::GetDiagramTableTitleFont()  // ��ȡ������ͼԪ�ı�����
{
	FontProperty* pFontProp = GetFontProperty(FONT_DiagramTableTitle);
	return pFontProp ? pFontProp->m_pFont : NULL;
}

CFont* CHsFont::GetZiXunFont()              // ��ȡ��Ѷ����
{ 
	FontProperty* pFontProp = GetFontProperty(FONT_F10Info);
	return pFontProp ? pFontProp->m_pFont : NULL;
}

CFont* CHsFont::GetDiagramTradeFont()      // ��ȡ����ͼԪ����
{
	FontProperty* pFontProp = GetFontProperty(FONT_DiagramTrade);
	return pFontProp ? pFontProp->m_pFont : NULL;
}

BOOL CHsFont::SetScrollFontProt(CFont* pFontScroll)  // �����������������
{
	if (pFontScroll == NULL || ::IsBadReadPtr(pFontScroll, 1) || !pFontScroll->m_hObject)
	{
		return FALSE;
	}

	FontProperty* pFontProp = GetFontProperty(FONT_ScrollTxt);
	if(pFontProp)
	{
		//ֱ��ȡ�������漴�� ��Ϊ�ǽ�CFontָ�����ȥ 
		LOGFONT lf;
		pFontScroll->GetLogFont(&lf);
		
		CFont* pFontOld = pFontProp->GetFont();
		if (pFontOld->m_hObject)
		{
			pFontOld->DeleteObject();
		}
		pFontOld->CreateFontIndirect(&lf);
		
		SetFontNode(FONT_ScrollTxt, pFontProp);
	}
	return TRUE;
}

