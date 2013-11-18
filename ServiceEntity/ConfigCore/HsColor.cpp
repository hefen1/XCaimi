
#include "stdafx.h"
#include "HsColor.h"
#include "HsFileManager.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CHsFileManager gFileManager; 

CHsColor::CHsColor()
{
	m_pConfigBase = NULL;
	m_bInited = FALSE;
}
CHsColor::~CHsColor()
{
	Release(TRUE);
}

void CHsColor::Init()
{
	m_pConfigBase = gFileManager.GetColorCfg();

	if (m_pConfigBase && !::IsBadReadPtr(m_pConfigBase, 1))
	{
		//设置各个文件对象的根节点
		m_pConfigBase->SetRootNode("ClassicalColor", NULL, NULL, ResourceDoc);
		m_pConfigBase->SetRootNode("ClassicalColor", NULL, NULL, SystemDoc);
		m_pConfigBase->SetRootNode("ClassicalColor", NULL, NULL, UserDoc);
		m_bInited = TRUE;
	}
}

BOOL CHsColor::Release(BOOL bSaveToFile)
{
	if (m_pConfigBase)
	{
		m_pConfigBase->Release(bSaveToFile);
	}
		
	POSITION posColor = m_CurrColorMap.GetStartPosition();
	while(posColor)
	{
		WORD nColorId;
		ColorProperty *pColor = NULL;
		m_CurrColorMap.GetNextAssoc(posColor,nColorId,(void*&)pColor);
		if (pColor)
		{
			delete pColor;
			pColor = NULL;
		}
	}
	m_CurrColorMap.RemoveAll();

	return TRUE;
}

TiXmlElement* CHsColor::GetGroupElementFromId( WORD nColorId ) 
{
	CString strPath = "Group";

	TiXmlNode* pNode = NULL;
	if (nColorId > COLOR_REPORT_BEGIN && nColorId < COLOR_REPORT_END)
	{
		pNode = m_pConfigBase->GetNode(strPath, "name", "ReportColor");	
	}
	else if (nColorId > COLOR_TREND_BEGIN && nColorId < COLOR_TREND_END)
	{
		pNode = m_pConfigBase->GetNode(strPath, "name", "TrendColor");
	}
	else if (nColorId > COLOR_KLINE_BEGIN && nColorId < COLOR_KLINE_END)
	{
		pNode = m_pConfigBase->GetNode(strPath, "name", "KLineColor");
	}
	else if (nColorId > COLOR_RIGHTVIEW_BEGIN && nColorId < COLOR_RIGHTVIEW_END)
	{
		pNode = m_pConfigBase->GetNode(strPath, "name", "RightViewColor");
	}
	else if (nColorId > COLOR_INFORMATION_BEGIN && nColorId < COLOR_INFORMATION_END)
	{
		pNode = m_pConfigBase->GetNode(strPath, "name", "Information");
	}
	else if (nColorId > COLOR_MISCELLANY_BEGIN && nColorId < COLOR_MISCELLANY_END)
	{
		pNode = m_pConfigBase->GetNode(strPath, "name", "Miscellany");
	}
	else if (nColorId > COLOR_DEFAULTINDEX_BEGIN && nColorId < COLOR_DEFAULTINDEX_END)
	{
		pNode = m_pConfigBase->GetNode(strPath, "name", "DefaultIndexColor");
	}

	if (pNode)
	{
		return pNode->ToElement();
	}
	else
	{
		return NULL;
	}
}
BOOL CHsColor::SaveToFile()
{
	if (m_pConfigBase)
	{
		m_pConfigBase->SaveToFile();
	}
	return TRUE;
}
COLORREF CHsColor::StringToColor(const char *pColor)
{
	ASSERT(pColor);
	CString strColor(pColor);
	int nLeftParen = strColor.Find("(");
	int nRightParen = strColor.Find(")");
	ASSERT(nLeftParen>=3);
	ASSERT(nRightParen>=5);
	CString strData = strColor.Mid(nLeftParen + 1,nRightParen - nLeftParen -1);
	CStringArray strColorAry;
	CGeneralHelper::AnalyseStr(strData,",",strColorAry);
	ASSERT(strColorAry.GetCount()==3);
	CString strR = strColorAry.GetAt(0);
	strR.Trim();
	long nR = CGeneralHelper::StringToLongSimple(strR);
	CString strG = strColorAry.GetAt(1);
	long nG = CGeneralHelper::StringToLongSimple(strG);
	CString strB = strColorAry.GetAt(2);
	long nB = CGeneralHelper::StringToLongSimple(strB);

	COLORREF cl = RGB(nR,nG,nB);
	return cl;
}
CString CHsColor::ColorToString(COLORREF lColor)
{
	ASSERT(lColor >0);
	BYTE R = GetRValue(lColor);
	BYTE G = GetGValue(lColor);
	BYTE B = GetBValue(lColor);
	CString strHead("RGB(");
	char chR[5],chG[5],chB[5];
	memset(chR,0,5); memset(chG,0,5); memset(chB,0,5);
	itoa(R,chR,10); itoa(G,chG,10);itoa(B,chB,10);
	CString strR(chR);
	strR +=",";
	CString strG(chG);
	strG += ",";
	CString strB(chB);
	strB +=")";
	CString strRGB = strHead + strR + strG + strB;
	return strRGB;
}

BOOL CHsColor::SetColorNode(short nID, ColorProperty* pColor)
{
	if (nID <= 0 || pColor == NULL || IsBadReadPtr(pColor, 1))
	{
		return FALSE;
	}

	CString strPath; //路径
	strPath.Format("//ClassicalColor/Group/ID%d", nID);

	BOOL bRet = FALSE;
	CString str = pColor->m_szName;       //名称
	bRet = m_pConfigBase->SetNodeAttrString(strPath, "name", str);

	str.Format("%d", pColor->m_nPenStyle); //线类型
	bRet = m_pConfigBase->SetNodeAttrString(strPath, "PenStyle", str);

	str.Format("%d", pColor->m_nWidth);    //线宽度
	bRet = m_pConfigBase->SetNodeAttrString(strPath, "PenWidth", str);

	str = ColorToString(pColor->m_lColor); //RGB颜色
	bRet = m_pConfigBase->SetNodeAttrString(strPath, "ColorValue", str);

	return bRet;
	
}

TiXmlNode* CHsColor::GetColorNode(short nID, DocType nType/* = Default */)
{
	CString strPath;
	strPath.AppendFormat("//Group/ID%d", nID);

	return m_pConfigBase->GetNode(strPath, nType);
}

ColorProperty* CHsColor::GetColorProperty(short nID)
{
	ColorProperty* pColorProperty = NULL;
	//首先从缓冲区里面寻找
	if (m_CurrColorMap.Lookup(nID, (void*&)pColorProperty))
	{
		return pColorProperty;
	}

	//从文件中寻找
	TiXmlNode* pNode = NULL;
	pNode = GetColorNode(nID);
	if ( pNode != NULL && !::IsBadReadPtr(pNode, 1))
	{
		TiXmlElement* pElement = pNode->ToElement();
		int nPenStyle = -123,nPenWidth;
		
		CString strPenStyle = pElement->Attribute("PenStyle",&nPenStyle);
		if (strPenStyle.Find("PS") != -1)
		{
			if (strPenStyle.CompareNoCase("PS_SOLID") == 0)
				nPenStyle = 0;
			if (strPenStyle.CompareNoCase("PS_DASH") == 0)
				nPenStyle = 1;
			if (strPenStyle.CompareNoCase("PS_DOT") == 0)
				nPenStyle = 2;
		}
		pElement->Attribute("PenWidth",&nPenWidth);
		CString strName(pElement->Attribute("name"));
		CString strColor(pElement->Attribute("ColorValue"));
		COLORREF color = StringToColor(strColor);
		ColorProperty* pColor = new ColorProperty;
		pColor->Set(nID, color, strName, NULL, nPenStyle , nPenWidth);
		ColorProperty *pColorExist = NULL;
		if(m_CurrColorMap.Lookup(nID,(void*&)pColorExist))
		{
			if(pColorExist)
				delete pColorExist;
		}
		m_CurrColorMap.SetAt(nID,(void*)pColor);
		return pColor;
	}

	return NULL;
}

const COLORREF CHsColor::GetColumnColor(UINT nColorId)
{
	ColorProperty* pColorProperty = GetColorProperty(nColorId);
	if (pColorProperty)
		return pColorProperty->GetColor();
	else
		return RGB(255,255,255);
}
const COLORREF CHsColor::GetBlackgroundColor()
{
	ColorProperty* pColorProperty = GetColorProperty(COLOR_Background);
	if (pColorProperty)
		return pColorProperty->GetColor();
	else
	{
		COLORREF cl = RGB(0, 0,0);
		return cl;
	}

}
const COLORREF  CHsColor::GetFixedCodeColor()
{
	ColorProperty* pColorProperty = GetColorProperty(COLOR_ReportCode);
	if (pColorProperty)
		return pColorProperty->GetColor();
	else
	{
		COLORREF cl = RGB(68, 226, 220);
		return cl;
	}

}

const COLORREF  CHsColor::GetFixedNameColor()
{
	ColorProperty* pColorProperty = GetColorProperty(COLOR_RepottCodeName);
	if (pColorProperty)
		return pColorProperty->GetColor();
	else
	{
		COLORREF cl = RGB(249, 219, 9);
		return cl;
	}
}

const COLORREF CHsColor::GetFixedNameSelfColor()
{
	ColorProperty* pColorProperty = GetColorProperty(COLOR_SelfSelectCode);
	if (pColorProperty)
		return pColorProperty->GetColor();
	else
	{
		COLORREF cl = RGB(41, 222, 214);
		return cl;
	}
}

const COLORREF  CHsColor::GetUpColor()
{
	ColorProperty* pColorProperty = GetColorProperty(COLOR_PriceUp);
	if (pColorProperty)
		return pColorProperty->GetColor();
	else
	{
		COLORREF cl = RGB(229, 13, 34);
		return cl;
	}
}

const COLORREF  CHsColor::GetDownColor()
{	
	ColorProperty* pColorProperty = GetColorProperty(COLOR_PriceDown);
	if (pColorProperty)
		return pColorProperty->GetColor();
	else
	{
		COLORREF cl = RGB(77, 220, 35);
		return cl;
	}
}

const COLORREF  CHsColor::GetEqualColor()
{
	ColorProperty* pColorProperty = GetColorProperty(COLOR_PriceEqual);
	if (pColorProperty)
		return pColorProperty->GetColor();
	else
	{
		COLORREF cl = RGB(178, 178, 178);
		return cl;
	}
}
COLORREF CHsColor::GetTotalColor()              // 获取总手颜色
{
	ColorProperty* pColorProperty = GetColorProperty(COLOR_TotalHand);
	if (pColorProperty)
		return pColorProperty->GetColor();
	else
	{
		COLORREF cl = RGB(239, 213, 27);
		return cl;
	}
}
COLORREF CHsColor::GetNowColor()                // 获取现手颜色
{
	ColorProperty* pColorProperty = GetColorProperty(COLOR_CurrentHand);
	if (pColorProperty)
		return pColorProperty->GetColor();
	else
	{
		COLORREF cl = RGB(239, 213, 27);
		return cl;
	}
}
COLORREF CHsColor::GetDoneColor()               // 获取成交颜色
{
	ColorProperty* pColorProperty = GetColorProperty(COLOR_TradeMoney);
	if (pColorProperty)
		return pColorProperty->GetColor();
	else
	{
		COLORREF cl = RGB(239, 213, 27);
		return cl;
	}
}
COLORREF CHsColor::GetChangeColor()             // 获取换手颜色
{
	ColorProperty* pColorProperty = GetColorProperty(COLOR_ExchangeRatio);
	if (pColorProperty)
		return pColorProperty->GetColor();
	else
	{
		COLORREF cl = RGB(188, 188, 188);
		return cl;
	}
}
COLORREF CHsColor::GetQuantitativeRateColor()   // 获取量比颜色 
{
	ColorProperty* pColorProperty = GetColorProperty(COLOR_LiangBi);
	if (pColorProperty)
		return pColorProperty->GetColor();
	else
	{
		COLORREF cl = RGB(188, 188, 188);
		return cl;
	}
}

COLORREF CHsColor::GetOrderBuyVolColor()        // 获取委买量颜色
{
	ColorProperty* pColorProperty = GetColorProperty(COLOR_OrderBuyVolume);
	if (pColorProperty)
		return pColorProperty->GetColor();
	else
	{
		COLORREF cl = RGB(68, 226,220);
		return cl;
	}
} 

COLORREF CHsColor::GetOrderSellVolColor()       // 获取委卖量颜色
{
	ColorProperty* pColorProperty = GetColorProperty(COLOR_OrderSellVolume);
	if (pColorProperty)
		return pColorProperty->GetColor();
	else
	{
		COLORREF cl = RGB(68, 226,220);
		return cl;
	}
}

const COLORREF CHsColor::GetStatusTextColor()
{
	ColorProperty* pColorProperty = GetColorProperty(COLOR_MISCELLANY_StatusTxt);
	if (pColorProperty)
		return pColorProperty->GetColor();
	else
	{
		COLORREF cl = RGB(239,213,27);
		return cl;
	}
}

const COLORREF CHsColor::GetScrollTestColor()
{
	ColorProperty* pColorProperty = GetColorProperty(COLOR_MISCELLANY_ScrollTxt);
	if (pColorProperty)
		return pColorProperty->GetColor();
	else
	{
		COLORREF cl = RGB(0, 0, 0);
		return cl;
	}
}

const COLORREF CHsColor::GetScrollBKColor()
{
	ColorProperty* pColorProperty = GetColorProperty(COLOR_MISCELLANY_ScrollBK);
	if (pColorProperty)
		return pColorProperty->GetColor();
	else
	{
		COLORREF cl = RGB(85, 85, 85);
		return cl;
	}
}
void  CHsColor::SetScrollTextColor(const COLORREF clrText) 
{
	ASSERT(clrText>0);
	
	ColorProperty* pColorProperty = GetColorProperty(COLOR_MISCELLANY_ScrollTxt);
	if(pColorProperty)
	{
		pColorProperty->m_lColor = clrText;
		SetColorNode(COLOR_MISCELLANY_ScrollTxt, pColorProperty);
	}
	
}
void  CHsColor::SetScrollBKColor(const COLORREF clrBk)   
{
	ASSERT(clrBk>0);
	ColorProperty* pColorProperty = GetColorProperty(COLOR_MISCELLANY_ScrollBK);
	if(pColorProperty)
	{
		pColorProperty->m_lColor = clrBk;
		SetColorNode(COLOR_MISCELLANY_ScrollBK, pColorProperty);
	}
}

const COLORREF CHsColor::GetTipTitleBk()		//tip提示标题背景
{
	ColorProperty* pColorProperty = GetColorProperty(COLOR_KLINE_TipTitleBk);
	if (pColorProperty)
		return pColorProperty->GetColor();
	else
	{
		COLORREF cl = RGB(50,51,58);
		return cl;
	}
}
const COLORREF CHsColor::GetTipTitleFg()		//tip提示标题前景
{
	ColorProperty* pColorProperty = GetColorProperty(COLOR_KLINE_TipTitleFg);
	if (pColorProperty)
		return pColorProperty->GetColor();
	else
	{
		COLORREF cl = RGB(255,255,255);
		return cl;
	}
}
const COLORREF CHsColor::GetTipTextFg()		     //tip提示文本背景
{
	ColorProperty* pColorProperty = GetColorProperty(COLOR_KLINE_TipTextFg);
	if (pColorProperty)
		return pColorProperty->GetColor();
	else
	{
		COLORREF cl = RGB(98,98,98);
		return cl;
	}
}
const COLORREF CHsColor::GetTipTextBk() 		//tip提示文本前景
{
	ColorProperty* pColorProperty = GetColorProperty(COLOR_KLINE_TipTextBK);
	if (pColorProperty)
		return pColorProperty->GetColor();
	else
	{
		COLORREF cl = RGB(255,255,255);
		return cl;
	}
}

COLORREF CHsColor::GetQuoteFixedRowColBKColor()					// 获取行情报价固定行固定列背景色
{
	ColorProperty* pColorProperty = GetColorProperty(COLOR_FixedRowColBK);
	if (pColorProperty)
		return pColorProperty->GetColor();
	else
		return RGB(55, 50, 45);
}
COLORREF CHsColor::GetQuoteFixedRowBKColor()					// 获取行情报价固定行背景色
{
	ColorProperty* pColorProperty = GetColorProperty(COLOR_FixedRowBK);
	if (pColorProperty)
		return pColorProperty->GetColor();
	else
		return RGB(100, 90, 80);
}
COLORREF CHsColor::GetQuoteFixedColBKColor()					// 获取行情报价固定列背景色
{
	ColorProperty* pColorProperty = GetColorProperty(COLOR_FixedColBK);
	if (pColorProperty)
		return pColorProperty->GetColor();
	else
		return RGB(30, 30, 50);
}

COLORREF CHsColor::GetQuoteHighLightFgColor()					// 获取行情报价高亮前景色
{
	ColorProperty* pColorProperty = GetColorProperty(COLOR_HightForegroud);
	if (pColorProperty)
		return pColorProperty->GetColor();
	else
		return RGB(255, 255, 18);
}
COLORREF CHsColor::GetQuoteHighLightBKColor()					// 获取行情报价高亮背景色
{
	ColorProperty* pColorProperty = GetColorProperty(COLOR_HightBackground);
	if (pColorProperty)
		return pColorProperty->GetColor();
	else
		return RGB(0, 0, 128);
}

COLORREF CHsColor::GetQuotePriceChangeBKColor()				// 获取行情报价更新.高亮文本背景
{
	ColorProperty* pColorProperty = GetColorProperty(COLOR_PriceChangeBk);
	if (pColorProperty)
		return pColorProperty->GetColor();
	else
		return RGB(0, 0, 81);
}


COLORREF CHsColor::GetQuoteTabBKColor()				//获取行情报价标签页背景颜色
{
	ColorProperty* pColorProperty = GetColorProperty(COLOR_TabBK);
	if (pColorProperty)
		return pColorProperty->GetColor();
	else
		return RGB(28, 28, 28);
}

COLORREF CHsColor::GetQuoteTabTextColor()			//获取行情报价标签页字体颜色
{
	ColorProperty* pColorProperty = GetColorProperty(COLOR_TabText);
	if (pColorProperty)
		return pColorProperty->GetColor();
	else
		return RGB(178, 178, 178);
}

COLORREF CHsColor::GetQuoteGuideTabBKColor()			//获取行情报价向导标签页背景颜色
{
	ColorProperty* pColorProperty = GetColorProperty(COLOR_GuideTab_BK);
	if (pColorProperty)
		return pColorProperty->GetColor();
	else
		return RGB(0, 0,0);
}

COLORREF CHsColor::GetQuoteGuideTabTextColor()		//获取行情报价向导标签页字体颜色
{
	ColorProperty* pColorProperty = GetColorProperty(COLOR_FixTabText);
	if (pColorProperty)
		return pColorProperty->GetColor();
	else
		return RGB(200, 80, 200);
}

COLORREF CHsColor::GetQuoteActiveTabBKColor()		// 获取行情报价激活标签页背景颜色
{
	ColorProperty* pColorProperty = GetColorProperty(COLOR_ActiveTabBK);
	if (pColorProperty)
		return pColorProperty->GetColor();
	else
		return RGB(0, 0,0);
}

COLORREF CHsColor::GetQuoteActiveTabTextColor()		// 获取行情报价激活标签页字体颜色
{
	ColorProperty* pColorProperty = GetColorProperty(COLOR_ActiveTabText);
	if (pColorProperty)
		return pColorProperty->GetColor();
	else
		return RGB(80, 200, 200);
}

const ColorProperty* CHsColor::GetQuoteTabFrameColorProp()	//获取行情报价标签页边框线形与颜色
{
	return (const ColorProperty*)GetColorProperty(COLOR_TabGrid);
}

const ColorProperty* CHsColor::GetQuoteGuideTabFrameColorProp()	//获取行情报价向导标签页边框线形与颜色
{
	return (const ColorProperty*)GetColorProperty(COLOR_GuideTab_Frame);
}

COLORREF CHsColor::GetTechLineColor(int nID)         //获取技术线颜色
{
	ColorProperty* pColorProperty = GetColorProperty(COLOR_DEFAULTINDEX_BEGIN+nID);
	if (pColorProperty)
		return pColorProperty->GetColor();
	else
		return RGB(255, 255,255);
}
COLORREF CHsColor::GetQuoteTabButtonBKColor()						// 获取行情报价标签页按钮背景颜色
{
	ColorProperty* pColorProperty = GetColorProperty(COLOR_TabButtonBK);
	if (pColorProperty)
		return pColorProperty->GetColor();
	else
		return RGB(28, 28, 28);
}

COLORREF CHsColor::GetRTitleCode()          // 右小图标题股票代码颜色
{
	ColorProperty* pColorProperty = GetColorProperty(COLOR_RIGHTVIEW_Code);
	if (pColorProperty)
		return pColorProperty->GetColor();
	else
		return RGB(249, 219, 9);
}
COLORREF CHsColor::GetRTitleCodeName()      // 右小图标题股票名称颜色
{
	ColorProperty* pColorProperty = GetColorProperty(COLOR_RIGHTVIEW_CodeName);
	if (pColorProperty)
		return pColorProperty->GetColor();
	else
		return RGB(68, 226, 220);
}
COLORREF CHsColor::GetRTableFixedName()     // 右小图固定文本颜色
{
	ColorProperty* pColorProperty = GetColorProperty(COLOR_RIGHTVIEW_Head);
	if (pColorProperty)
		return pColorProperty->GetColor();
	else
		return RGB(178, 178, 178);
}
COLORREF CHsColor::GetRTableTime()          // 右小图时间颜色
{
	ColorProperty* pColorProperty = GetColorProperty(COLOR_RIGHTVIEW_Time);
	if (pColorProperty)
		return pColorProperty->GetColor();
	else
		return RGB(178,178,178);
	
}
COLORREF CHsColor::GetRTableVer()           // 右小图竖线颜色
{
	ColorProperty* pColorProperty = GetColorProperty(COLOR_RIGHTVIEW_VerLine);
	if (pColorProperty)
		return pColorProperty->GetColor();
	else
		return RGB(155, 0, 0);
}
COLORREF CHsColor::GetRTableHor()           // 右小图横线颜色
{
	ColorProperty* pColorProperty = GetColorProperty(COLOR_RIGHTVIEW_HorLine);
	if (pColorProperty)
		return pColorProperty->GetColor();
	else
		return RGB(155, 0, 0);
}

COLORREF CHsColor::GetTableTitle()          // 表格标题图元字体颜色
{
	ColorProperty* pColorProperty = GetColorProperty(COLOR_RIGHTVIEW_TableTitle);
	if (pColorProperty)
		return pColorProperty->GetColor();
	else
		return RGB(192, 192, 192);
}
COLORREF CHsColor::GetTableTitleBK()        // 表格标题图元背景颜色
{
	ColorProperty* pColorProperty = GetColorProperty(COLOR_RIGHTVIEW_TableTitleBK);
	if (pColorProperty)
		return pColorProperty->GetColor();
	else
		return RGB(48, 48, 48);
}

COLORREF CHsColor::GetTradeTableFixedName()
{
	ColorProperty* pColorProperty = GetColorProperty(COLOR_RIGHTVIEW_TRADEFIXEDNAME);
	if (pColorProperty)
		return pColorProperty->GetColor();
	else
		return RGB(0, 0, 0);
}

COLORREF CHsColor::GetZiXunBK()             // 资讯背景色
{
	ColorProperty* pColorProperty = GetColorProperty(COLOR_INFORMATION_Bk);
	if (pColorProperty)
		return pColorProperty->GetColor();
	else
		return RGB(0, 0, 0);
}

COLORREF CHsColor::GetZiXunFG()             // 资讯文本前景色
{
	ColorProperty* pColorProperty = GetColorProperty(COLOR_INFORMATION_Fg);
	if (pColorProperty)
		return pColorProperty->GetColor();
	else
		return RGB(248, 218, 96);
}

COLORREF CHsColor::GetDiagramLayoutBKColor()              // 图层标签页背景
{
	ColorProperty* pColorProperty = GetColorProperty(COLOR_MISCELLANY_DiagramLayoutBKColor);
	if (pColorProperty)
		return pColorProperty->GetColor();
	else
		return RGB(0,0,0);
}

COLORREF CHsColor::GetDiagramLayoutActiveFillColor()       // 图层标签页激活
{
	ColorProperty* pColorProperty = GetColorProperty(COLOR_MISCELLANY_DiagramLayoutActiveFill);
	if (pColorProperty)
		return pColorProperty->GetColor();
	else
		return RGB(48,48,48);
}

COLORREF CHsColor::GetDiagramLayoutFillColor()            // 图层标签页常规
{
	ColorProperty* pColorProperty = GetColorProperty(COLOR_MISCELLANY_DiagramLayoutFill);
	if (pColorProperty)
		return pColorProperty->GetColor();
	else
		return RGB(0,0,0);
}

COLORREF CHsColor::GetDiagramLayoutForeColor()            // 图层标签页前景
{
	ColorProperty* pColorProperty = GetColorProperty(COLOR_MISCELLANY_DiagramLayoutFore);
	if (pColorProperty)
		return pColorProperty->GetColor();
	else
		return RGB(192,192,192);
}

COLORREF CHsColor::GetDiagramLayoutBorderColor()           // 图层标签页边框
{
	ColorProperty* pColorProperty = GetColorProperty(COLOR_MISCELLANY_DiagramLayoutBorder);
	if (pColorProperty)
		return pColorProperty->GetColor();
	else
		return RGB(192,0,0);
}
