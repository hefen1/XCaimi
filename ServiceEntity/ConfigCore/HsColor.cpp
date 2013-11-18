
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
		//���ø����ļ�����ĸ��ڵ�
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

	CString strPath; //·��
	strPath.Format("//ClassicalColor/Group/ID%d", nID);

	BOOL bRet = FALSE;
	CString str = pColor->m_szName;       //����
	bRet = m_pConfigBase->SetNodeAttrString(strPath, "name", str);

	str.Format("%d", pColor->m_nPenStyle); //������
	bRet = m_pConfigBase->SetNodeAttrString(strPath, "PenStyle", str);

	str.Format("%d", pColor->m_nWidth);    //�߿��
	bRet = m_pConfigBase->SetNodeAttrString(strPath, "PenWidth", str);

	str = ColorToString(pColor->m_lColor); //RGB��ɫ
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
	//���ȴӻ���������Ѱ��
	if (m_CurrColorMap.Lookup(nID, (void*&)pColorProperty))
	{
		return pColorProperty;
	}

	//���ļ���Ѱ��
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
COLORREF CHsColor::GetTotalColor()              // ��ȡ������ɫ
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
COLORREF CHsColor::GetNowColor()                // ��ȡ������ɫ
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
COLORREF CHsColor::GetDoneColor()               // ��ȡ�ɽ���ɫ
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
COLORREF CHsColor::GetChangeColor()             // ��ȡ������ɫ
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
COLORREF CHsColor::GetQuantitativeRateColor()   // ��ȡ������ɫ 
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

COLORREF CHsColor::GetOrderBuyVolColor()        // ��ȡί������ɫ
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

COLORREF CHsColor::GetOrderSellVolColor()       // ��ȡί������ɫ
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

const COLORREF CHsColor::GetTipTitleBk()		//tip��ʾ���ⱳ��
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
const COLORREF CHsColor::GetTipTitleFg()		//tip��ʾ����ǰ��
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
const COLORREF CHsColor::GetTipTextFg()		     //tip��ʾ�ı�����
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
const COLORREF CHsColor::GetTipTextBk() 		//tip��ʾ�ı�ǰ��
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

COLORREF CHsColor::GetQuoteFixedRowColBKColor()					// ��ȡ���鱨�۹̶��й̶��б���ɫ
{
	ColorProperty* pColorProperty = GetColorProperty(COLOR_FixedRowColBK);
	if (pColorProperty)
		return pColorProperty->GetColor();
	else
		return RGB(55, 50, 45);
}
COLORREF CHsColor::GetQuoteFixedRowBKColor()					// ��ȡ���鱨�۹̶��б���ɫ
{
	ColorProperty* pColorProperty = GetColorProperty(COLOR_FixedRowBK);
	if (pColorProperty)
		return pColorProperty->GetColor();
	else
		return RGB(100, 90, 80);
}
COLORREF CHsColor::GetQuoteFixedColBKColor()					// ��ȡ���鱨�۹̶��б���ɫ
{
	ColorProperty* pColorProperty = GetColorProperty(COLOR_FixedColBK);
	if (pColorProperty)
		return pColorProperty->GetColor();
	else
		return RGB(30, 30, 50);
}

COLORREF CHsColor::GetQuoteHighLightFgColor()					// ��ȡ���鱨�۸���ǰ��ɫ
{
	ColorProperty* pColorProperty = GetColorProperty(COLOR_HightForegroud);
	if (pColorProperty)
		return pColorProperty->GetColor();
	else
		return RGB(255, 255, 18);
}
COLORREF CHsColor::GetQuoteHighLightBKColor()					// ��ȡ���鱨�۸�������ɫ
{
	ColorProperty* pColorProperty = GetColorProperty(COLOR_HightBackground);
	if (pColorProperty)
		return pColorProperty->GetColor();
	else
		return RGB(0, 0, 128);
}

COLORREF CHsColor::GetQuotePriceChangeBKColor()				// ��ȡ���鱨�۸���.�����ı�����
{
	ColorProperty* pColorProperty = GetColorProperty(COLOR_PriceChangeBk);
	if (pColorProperty)
		return pColorProperty->GetColor();
	else
		return RGB(0, 0, 81);
}


COLORREF CHsColor::GetQuoteTabBKColor()				//��ȡ���鱨�۱�ǩҳ������ɫ
{
	ColorProperty* pColorProperty = GetColorProperty(COLOR_TabBK);
	if (pColorProperty)
		return pColorProperty->GetColor();
	else
		return RGB(28, 28, 28);
}

COLORREF CHsColor::GetQuoteTabTextColor()			//��ȡ���鱨�۱�ǩҳ������ɫ
{
	ColorProperty* pColorProperty = GetColorProperty(COLOR_TabText);
	if (pColorProperty)
		return pColorProperty->GetColor();
	else
		return RGB(178, 178, 178);
}

COLORREF CHsColor::GetQuoteGuideTabBKColor()			//��ȡ���鱨���򵼱�ǩҳ������ɫ
{
	ColorProperty* pColorProperty = GetColorProperty(COLOR_GuideTab_BK);
	if (pColorProperty)
		return pColorProperty->GetColor();
	else
		return RGB(0, 0,0);
}

COLORREF CHsColor::GetQuoteGuideTabTextColor()		//��ȡ���鱨���򵼱�ǩҳ������ɫ
{
	ColorProperty* pColorProperty = GetColorProperty(COLOR_FixTabText);
	if (pColorProperty)
		return pColorProperty->GetColor();
	else
		return RGB(200, 80, 200);
}

COLORREF CHsColor::GetQuoteActiveTabBKColor()		// ��ȡ���鱨�ۼ����ǩҳ������ɫ
{
	ColorProperty* pColorProperty = GetColorProperty(COLOR_ActiveTabBK);
	if (pColorProperty)
		return pColorProperty->GetColor();
	else
		return RGB(0, 0,0);
}

COLORREF CHsColor::GetQuoteActiveTabTextColor()		// ��ȡ���鱨�ۼ����ǩҳ������ɫ
{
	ColorProperty* pColorProperty = GetColorProperty(COLOR_ActiveTabText);
	if (pColorProperty)
		return pColorProperty->GetColor();
	else
		return RGB(80, 200, 200);
}

const ColorProperty* CHsColor::GetQuoteTabFrameColorProp()	//��ȡ���鱨�۱�ǩҳ�߿���������ɫ
{
	return (const ColorProperty*)GetColorProperty(COLOR_TabGrid);
}

const ColorProperty* CHsColor::GetQuoteGuideTabFrameColorProp()	//��ȡ���鱨���򵼱�ǩҳ�߿���������ɫ
{
	return (const ColorProperty*)GetColorProperty(COLOR_GuideTab_Frame);
}

COLORREF CHsColor::GetTechLineColor(int nID)         //��ȡ��������ɫ
{
	ColorProperty* pColorProperty = GetColorProperty(COLOR_DEFAULTINDEX_BEGIN+nID);
	if (pColorProperty)
		return pColorProperty->GetColor();
	else
		return RGB(255, 255,255);
}
COLORREF CHsColor::GetQuoteTabButtonBKColor()						// ��ȡ���鱨�۱�ǩҳ��ť������ɫ
{
	ColorProperty* pColorProperty = GetColorProperty(COLOR_TabButtonBK);
	if (pColorProperty)
		return pColorProperty->GetColor();
	else
		return RGB(28, 28, 28);
}

COLORREF CHsColor::GetRTitleCode()          // ��Сͼ�����Ʊ������ɫ
{
	ColorProperty* pColorProperty = GetColorProperty(COLOR_RIGHTVIEW_Code);
	if (pColorProperty)
		return pColorProperty->GetColor();
	else
		return RGB(249, 219, 9);
}
COLORREF CHsColor::GetRTitleCodeName()      // ��Сͼ�����Ʊ������ɫ
{
	ColorProperty* pColorProperty = GetColorProperty(COLOR_RIGHTVIEW_CodeName);
	if (pColorProperty)
		return pColorProperty->GetColor();
	else
		return RGB(68, 226, 220);
}
COLORREF CHsColor::GetRTableFixedName()     // ��Сͼ�̶��ı���ɫ
{
	ColorProperty* pColorProperty = GetColorProperty(COLOR_RIGHTVIEW_Head);
	if (pColorProperty)
		return pColorProperty->GetColor();
	else
		return RGB(178, 178, 178);
}
COLORREF CHsColor::GetRTableTime()          // ��Сͼʱ����ɫ
{
	ColorProperty* pColorProperty = GetColorProperty(COLOR_RIGHTVIEW_Time);
	if (pColorProperty)
		return pColorProperty->GetColor();
	else
		return RGB(178,178,178);
	
}
COLORREF CHsColor::GetRTableVer()           // ��Сͼ������ɫ
{
	ColorProperty* pColorProperty = GetColorProperty(COLOR_RIGHTVIEW_VerLine);
	if (pColorProperty)
		return pColorProperty->GetColor();
	else
		return RGB(155, 0, 0);
}
COLORREF CHsColor::GetRTableHor()           // ��Сͼ������ɫ
{
	ColorProperty* pColorProperty = GetColorProperty(COLOR_RIGHTVIEW_HorLine);
	if (pColorProperty)
		return pColorProperty->GetColor();
	else
		return RGB(155, 0, 0);
}

COLORREF CHsColor::GetTableTitle()          // ������ͼԪ������ɫ
{
	ColorProperty* pColorProperty = GetColorProperty(COLOR_RIGHTVIEW_TableTitle);
	if (pColorProperty)
		return pColorProperty->GetColor();
	else
		return RGB(192, 192, 192);
}
COLORREF CHsColor::GetTableTitleBK()        // ������ͼԪ������ɫ
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

COLORREF CHsColor::GetZiXunBK()             // ��Ѷ����ɫ
{
	ColorProperty* pColorProperty = GetColorProperty(COLOR_INFORMATION_Bk);
	if (pColorProperty)
		return pColorProperty->GetColor();
	else
		return RGB(0, 0, 0);
}

COLORREF CHsColor::GetZiXunFG()             // ��Ѷ�ı�ǰ��ɫ
{
	ColorProperty* pColorProperty = GetColorProperty(COLOR_INFORMATION_Fg);
	if (pColorProperty)
		return pColorProperty->GetColor();
	else
		return RGB(248, 218, 96);
}

COLORREF CHsColor::GetDiagramLayoutBKColor()              // ͼ���ǩҳ����
{
	ColorProperty* pColorProperty = GetColorProperty(COLOR_MISCELLANY_DiagramLayoutBKColor);
	if (pColorProperty)
		return pColorProperty->GetColor();
	else
		return RGB(0,0,0);
}

COLORREF CHsColor::GetDiagramLayoutActiveFillColor()       // ͼ���ǩҳ����
{
	ColorProperty* pColorProperty = GetColorProperty(COLOR_MISCELLANY_DiagramLayoutActiveFill);
	if (pColorProperty)
		return pColorProperty->GetColor();
	else
		return RGB(48,48,48);
}

COLORREF CHsColor::GetDiagramLayoutFillColor()            // ͼ���ǩҳ����
{
	ColorProperty* pColorProperty = GetColorProperty(COLOR_MISCELLANY_DiagramLayoutFill);
	if (pColorProperty)
		return pColorProperty->GetColor();
	else
		return RGB(0,0,0);
}

COLORREF CHsColor::GetDiagramLayoutForeColor()            // ͼ���ǩҳǰ��
{
	ColorProperty* pColorProperty = GetColorProperty(COLOR_MISCELLANY_DiagramLayoutFore);
	if (pColorProperty)
		return pColorProperty->GetColor();
	else
		return RGB(192,192,192);
}

COLORREF CHsColor::GetDiagramLayoutBorderColor()           // ͼ���ǩҳ�߿�
{
	ColorProperty* pColorProperty = GetColorProperty(COLOR_MISCELLANY_DiagramLayoutBorder);
	if (pColorProperty)
		return pColorProperty->GetColor();
	else
		return RGB(192,0,0);
}
