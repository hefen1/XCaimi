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

	m_QuoteDefaultFont.CreatePointFont(100, _T("宋体"));
	m_TradeDefaultFont.CreatePointFont(100, _T("宋体"));
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
		//设置各个文件对象的根节点
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

	CString strPath; //路径
	strPath.Format("//ClassicalFont/Group/ID%d", nID);

	BOOL bRet = FALSE;
	CString str = pFont->m_strName;      //名称
	bRet = m_pConfigBase->SetNodeAttrString(strPath, "name", str);

	LOGFONT lFont;
	pFont->m_pFont->GetLogFont(&lFont);

	str = lFont.lfFaceName;              //字体名称
	bRet = m_pConfigBase->SetNodeAttrString(strPath, "FontName", str);

	str.Format("%d", -lFont.lfHeight);   //字体高度
	bRet = m_pConfigBase->SetNodeAttrString(strPath, "FontHeight", str);

	if (lFont.lfWeight == FW_BOLD)       //字体重量
	{
		str = "FONT_BOLD";
	}
	else
	{
		str = "FONT_NORMAL";
	}
	bRet = m_pConfigBase->SetNodeAttrString(strPath, "FontWeight", str);

                                         //是否斜体
	bRet = m_pConfigBase->SetNodeAttrInt(strPath, "FontItalic", (int)lFont.lfItalic);
                                         //输出精度
	bRet = m_pConfigBase->SetNodeAttrInt(strPath, "OutPre", (int)lFont.lfOutPrecision);
	                                     //指定字体的字符间距和族
	bRet = m_pConfigBase->SetNodeAttrInt(strPath, "Pitch", (int)lFont.lfPitchAndFamily);
                                         //指定字符集
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
	//首先从缓冲区里面寻找
	if (m_CurrFontMap.Lookup(nID, (void*&)pFontProperty))
	{
		return pFontProperty;
	}

	//从文件中寻找
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

 CFont* CHsFont::GetTipTitleFont()     //获取TIP提示标题字体
{
	FontProperty* pFontProp = GetFontProperty(FONT_TipTitle);
	return pFontProp ? pFontProp->m_pFont : NULL;
}
 CFont* CHsFont::GetTipTxtFont()       //获取TIP提示文本字体
{
	FontProperty* pFontProp = GetFontProperty(FONT_TipTxt);
	return pFontProp ? pFontProp->m_pFont : NULL;
}

 CFont* CHsFont::GetQuoteTabFont()		//获取行情报价标签页字体
{
	FontProperty* pFontProp = GetFontProperty(FONT_TabTxt);
	return pFontProp ? pFontProp->m_pFont : NULL;
}
 CFont* CHsFont::GetQuoteGuideTabFont()		//获取行情报价向导标签页字体
{
	FontProperty* pFontProp = GetFontProperty(FONT_TabTxt);
	return pFontProp ? pFontProp->m_pFont : NULL;
}

 CFont* CHsFont::GetStatusFontProt()		//获取状态栏指数字体
{
	FontProperty* pFontProp = GetFontProperty(FONT_StatusTxt);
	return pFontProp ? pFontProp->m_pFont : NULL;
}

 CFont* CHsFont::GetScrollFontProt()        //获取跑马灯数据字体
{
	FontProperty* pFontProp = GetFontProperty(FONT_ScrollTxt);
	return pFontProp ? pFontProp->m_pFont : NULL;
}

 CFont* CHsFont::GetQuoteRefreshFont()     
{
	FontProperty* pFontProp = GetFontProperty(FONT_ReportRefreshTxt);
	return pFontProp ? pFontProp->m_pFont : NULL;
}

 CFont* CHsFont::GetQuoteColHeadFont()		// 获取行情报价表头字体
{
	FontProperty* pFontProp = GetFontProperty(FONT_ReportColumnName);
	return pFontProp ? pFontProp->m_pFont : NULL;
}

 CFont* CHsFont::GetQuoteStringFont()		// 获取行情报价文字字体(中文字)
{
	FontProperty* pFontProp = GetFontProperty(FONT_ReportCodeName);
	return pFontProp ? pFontProp->m_pFont : NULL;
}

 CFont* CHsFont::GetQuoteCodeFont()		// 获取行情报价股票代码字体
{
	FontProperty* pFontProp = GetFontProperty(FONT_ReportCode);
	return pFontProp ? pFontProp->m_pFont : NULL;
}

 CFont* CHsFont::GetQuoteDynamicFont()		// 获取行情报价动态文本字体
{
	FontProperty* pFontProp = GetFontProperty(FONT_ReportDynamicTxt);
	return pFontProp ? pFontProp->m_pFont : NULL;
}

 CFont* CHsFont::GetQuoteFixedFont()		// 获取行情报价固定文本字体（序）
{
	FontProperty* pFontProp = GetFontProperty(FONT_ReportfixedTxt);
	return pFontProp ? pFontProp->m_pFont : NULL;
}

CFont* CHsFont::GetRTableFixedFont()       // 获取右小图字段名称
{
	FontProperty* pFontProp = GetFontProperty(FONT_RightViewFixed);
	return pFontProp ? pFontProp->m_pFont : NULL;
}

CFont* CHsFont::GetRTableDynamicalFont()     // 获取右小图动态字段字体
{
	FontProperty* pFontProp = GetFontProperty(FONT_RightViewDynamic);
	return pFontProp ? pFontProp->m_pFont : NULL;
}

CFont* CHsFont::GetRTableTitleCodeFont()     // 获取右小图标题股票代码字体
{
	FontProperty* pFontProp = GetFontProperty(FONT_RightViewCode);
	return pFontProp ? pFontProp->m_pFont : NULL;
}

CFont* CHsFont::GetRTableTitleNameFont()     // 获取右小图标题股票名称字体
{
	FontProperty* pFontProp = GetFontProperty(FONT_RightViewCodeName);
	return pFontProp ? pFontProp->m_pFont : NULL;
}
CFont* CHsFont::GetRTableFinanceFont()       // 获取右小图配财值页文本
{
	FontProperty* pFontProp = GetFontProperty(FONT_RightViewFinace);
	return pFontProp ? pFontProp->m_pFont : NULL;
}

CFont* CHsFont::GetRTableTimeFont()        // 获取右小图时间列字体
{
	FontProperty* pFontProp = GetFontProperty(FONT_DetailTradeTime);
	return pFontProp ? pFontProp->m_pFont : NULL;
}

//获取图层标签页字体
CFont* CHsFont::GetDiagramLayoutTabFont()   
{
	FontProperty* pFontProp = GetFontProperty(FONT_DiagramLayoutTab);
	return pFontProp ? pFontProp->m_pFont : NULL;
}

CFont* CHsFont::GetDiagramTableTitleFont()  // 获取表格标题图元文本字体
{
	FontProperty* pFontProp = GetFontProperty(FONT_DiagramTableTitle);
	return pFontProp ? pFontProp->m_pFont : NULL;
}

CFont* CHsFont::GetZiXunFont()              // 获取资讯字体
{ 
	FontProperty* pFontProp = GetFontProperty(FONT_F10Info);
	return pFontProp ? pFontProp->m_pFont : NULL;
}

CFont* CHsFont::GetDiagramTradeFont()      // 获取交易图元字体
{
	FontProperty* pFontProp = GetFontProperty(FONT_DiagramTrade);
	return pFontProp ? pFontProp->m_pFont : NULL;
}

BOOL CHsFont::SetScrollFontProt(CFont* pFontScroll)  // 设置跑马灯数据字体
{
	if (pFontScroll == NULL || ::IsBadReadPtr(pFontScroll, 1) || !pFontScroll->m_hObject)
	{
		return FALSE;
	}

	FontProperty* pFontProp = GetFontProperty(FONT_ScrollTxt);
	if(pFontProp)
	{
		//直接取出来保存即可 因为是将CFont指针给出去 
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

