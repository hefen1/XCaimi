#include "stdafx.h"
#include "GraphicsData.h"
#include "..\..\..\Common\WinnerApplication.h"
#include "..\..\..\Common\QuoteLangMngInterFace.h"
#include "..\..\..\Resource\QuoteLang\Resource.h"
#include "..\..\..\Common\FormulaManPublic.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
//////////////////////////////////////////////////////////////////////////
int CImageListEx::Draw(CDC* pDC, int nImage, POINT pt, UINT nStyle,CRect rcFocus /*= CRect(0,0,0,0)*/)
{
	int nRet = 0;
	if( nStyle & ILD_FOCUS )
	{
		nStyle &= ~ILD_FOCUS;

		if( rcFocus.IsRectEmpty() )
		{
			rcFocus.left   = pt.x - 1;
			rcFocus.top    = pt.y;
			rcFocus.right  = rcFocus.left + 11;
			rcFocus.bottom = rcFocus.top  + 13;
		}

		CPen hPen;
		hPen.CreatePen(PS_DOT, 1, g_hsGraphicsData.clPageBK.GetInvertColor());
		CPen* hOldPen = pDC->SelectObject(&hPen);

		pDC->MoveTo(rcFocus.left, rcFocus.top);
		pDC->LineTo(rcFocus.left, rcFocus.bottom);
		pDC->LineTo(rcFocus.right, rcFocus.bottom);
		pDC->LineTo(rcFocus.right, rcFocus.top);
		pDC->LineTo(rcFocus.left, rcFocus.top);

		nRet = rcFocus.right + 6;
		pDC->SelectObject(hOldPen);	
		hPen.DeleteObject();
	}
	else
		nRet = pt.x + 16;
	CImageList::Draw(pDC, nImage, pt, nStyle);
	return nRet;
}

BOOL CImageListEx::DrawEx(CDC* pDC, int nImage, POINT pt, SIZE sz, 
						  COLORREF clrBk, COLORREF clrFg, UINT nStyle)
{
	return CImageList::DrawEx(pDC, nImage, pt, sz, clrBk, clrFg, nStyle);
}
//////////////////////////////////////////////////////////////////////////
//
CGraphicsData::CGraphicsData()
{
	m_expressImages = NULL;
	Init();
}

CGraphicsData::~CGraphicsData()
{
	if(m_expressImages != NULL)
	{
		delete m_expressImages;
		m_expressImages = NULL;
	}
	while (m_ayDefTechColor.GetSize() > 0)
	{
		ColorProperty* pItem = m_ayDefTechColor.GetAt(0);
		m_ayDefTechColor.RemoveAt(0);
		if (pItem != NULL)
		{
			delete pItem;
			pItem = NULL;
		}
	}
}

CGraphicsData g_hsGraphicsData;
//////////////////////////////////////////////////////////////////////////
//
BOOL CGraphicsData::Init()
{
// ��ɫ����
	short nID = 1;
	clPageBK.Set(nID++,  RGB(0, 0, 0), _T("����"), &m_ayColorProperty);
	clMouseMoveMarkBK.Set(nID++,  RGB(10, 36, 106), _T("�ƶ���Ǳ���"), NULL);
	clUp   .Set(nID++,  RGB(229, 13, 34), _T("����"), &m_ayColorProperty);
	clDown .Set(nID++,  RGB(77, 220, 35), _T("�µ�"), &m_ayColorProperty);
	clEqual .Set(nID++,  RGB(178, 178, 178), _T("ƽ��"), &m_ayColorProperty);  
	clPreClose .Set(nID++,  RGB(188, 188, 188), _T("����"), &m_ayColorProperty); //8

	clHorLine .Set(nID++,  RGB(155, 0, 0), _T("��ͼ����"), &m_ayColorProperty);  //����
	clVerLine .Set(nID++,  RGB(155, 0, 0), _T("��ͼ����"), &m_ayColorProperty);   //����

	clJsAvgLine.Set(nID++, RGB(237, 217, 39),_T( "��ʱͼ.����"), &m_ayColorProperty);   //����
	clJsPriceLine.Set(nID++, RGB(230, 230, 230), _T("��ʱͼ.�۸���"), &m_ayColorProperty);
	clJsDownVolLine.Set(nID++, RGB(80, 228, 221), _T("��ʱͼ.�ɽ�������"), &m_ayColorProperty);	// �ɽ�����
	clJsUpVolLine.Set(nID++, RGB(229, 13, 34), _T("��ʱͼ.�ɽ�������"), &m_ayColorProperty);	// �ɽ�����
	clVerAmountTXT.Set(nID++, RGB(237, 217, 39), _T("�ɽ��������ı�"), &m_ayColorProperty);	// �ɽ����������ı�
	clDotHorLine .Set(nID++,  RGB(155, 0, 0), _T("��ʱͼ.�������"), &m_ayColorProperty,  PS_SOLID,  1);
	clDotVerLine .Set(nID++,  RGB(128, 0, 0), _T("��ʱͼ.��������"),NULL );
	clJsPriceMiddleLine.Set(nID++, RGB(155, 0, 0), _T("��ʱͼ.�м����"), &m_ayColorProperty,  PS_SOLID,  2);
	clJsPriceMiddleVerLine.Set(nID++, RGB(155, 0, 0), _T("��ʱͼ.�м�����"), &m_ayColorProperty, PS_SOLID, 2);
	clJsPriceHourLine.Set(nID++, RGB(155, 0, 0), _T("��ʱͼ.Сʱ��"), &m_ayColorProperty, PS_SOLID, 1);
	clJsPriceHalfHourLine.Set(nID++, RGB(155, 0, 0), _T("��ʱͼ.��Сʱ��"), &m_ayColorProperty, PS_DOT, 1);
	clGridLine.Set(nID++,  RGB(155,  0,  0), _T("��ʱ,����ͼ������"), &m_ayColorProperty,  PS_SOLID,  1);  //ͼ�α߿���
	clChartTick.Set(nID++,  RGB(155,  0,  0), _T("��ʱ,��������̶���"), &m_ayColorProperty,  PS_SOLID,  1);   //ͼ������̶���

	clChartTitle.Set(nID++,  RGB(0,  192,  192), _T("ͼ�α���"), NULL);	
	clChartBK.Set(nID++,  RGB(0,  192,  0), _T("ͼ�α���"), NULL); 

	// ����
	clHorTXT.Set(nID++,  RGB(192,  0,  0), _T("���ߺ������ı�"), &m_ayColorProperty);
	clChartTickTXT.Set(nID++,  RGB(192,  0,  0), _T("�����������ı�"), &m_ayColorProperty);
	clDayGridVerLine.Set(nID++, RGB(168, 0, 0), _T("���߶�������")/*, &m_ayColorProperty*/);	 // �������񶥲�����
	clDayUpLine.Set(nID++, RGB(255, 84, 84), _T("����ͼ.������(����)"), &m_ayColorProperty);		// ������(����)
	clDayDownLine.Set(nID++, RGB(84, 255, 255), _T("����ͼ.�µ���(����)"), &m_ayColorProperty);		// �µ���(����)
	clDayUpFillLine.Set(nID++, RGB(0, 0, 0), _T("����ͼ.�������ɫ"), &m_ayColorProperty);  //�������(����)
	clDayDownFillLine.Set(nID++, RGB(84, 255, 255),_T( "����ͼ.�������ɫ"), &m_ayColorProperty);  //�µ����(����)
	clMaxMinValueBk.Set(nID++,  RGB(0, 0, 245), _T("�����Сֵ��������"), &m_ayColorProperty);
	clMaxMinValueFK.Set(nID++,  RGB(128, 0, 0), _T("�����Сֵ����ǰ��(δʹ��)"), NULL);
	clDayHorLine.Set(nID++,  RGB(78, 0, 0), _T("����ͼ.�������"), &m_ayColorProperty, PS_DOT,  PS_SOLID);
	clDayVerLine.Set(nID++,  RGB(155, 0, 0), _T("����ͼ.��������"), &m_ayColorProperty, PS_DOT,  PS_SOLID);
	clDayTopLine.Set(nID++,  RGB(155, 0, 0), _T("����ͼ.��������"), &m_ayColorProperty ,PS_DOT,  1);

	clInChartTitleTXT.Set(nID++,  RGB(255, 255, 0), _T("ͼԪ�ڶ��������ı�"), NULL);	
	clVerDayTXT.Set(nID++, RGB(204, 204, 0), _T("δʹ��"), NULL);		
	clDayAmericaLine.Set(nID++, RGB(0, 255, 255), _T("δʹ��"), NULL);	
	clDayTrendLine.Set(nID++, RGB(0, 255, 255), _T("δʹ��"), NULL);		
	clDayZeroHorLine.Set(nID++, RGB(0, 255, 255), _T("δʹ��"), NULL);
	clMoveLine.Set(nID++, RGB(98, 98, 98),_T( "�ƶ������"), &m_ayColorProperty,  PS_SOLID,  1);

	clVerLiangBiTXT.Set(nID++, RGB(176, 0, 0), _T("����ָ�������ı�"), &m_ayColorProperty);	// �����������ı�	
	clVerADLTXT.Set(nID++, RGB(176, 0, 0), _T("ADLָ�������ı�"), &m_ayColorProperty);		//���ģ��������ı�
	clJsLiangBiLine.Set(nID++, RGB(230, 230, 230), _T("����ָ����"), &m_ayColorProperty);    // ����ָ��
	clJsADLLine.Set(nID++, RGB(230, 230, 230),_T( "ADLָ����"), &m_ayColorProperty,  PS_SOLID);
	clVerDouKongTXT.Set(nID++, RGB(176, 0, 0), _T("���ָ�������ı�"), &m_ayColorProperty);	// ����������ı�
	clVerBuySellTXT.Set(nID++, RGB(176, 0, 0), _T("�������������ı�"), &m_ayColorProperty);		// ���������������ı�
	clJsDouKongLine.Set(nID++, RGB(230,230,230), _T("���ָ����"), &m_ayColorProperty,  PS_SOLID);
	crDiffValueColor.Set(nID++, RGB(255, 62, 255), _T("������ֵ"), &m_ayColorProperty);  
	clJsBuyLine.Set(nID++, RGB(255, 255, 84), _T("��������֮������"), &m_ayColorProperty);  //��������֮����
	clJsSellLine.Set(nID++, RGB(230, 230, 230), _T("��������֮������"), &m_ayColorProperty);  //��������֮����
	clJsIOPVLine.Set(nID++, RGB(15, 255, 15),_T( "IOPV��"), &m_ayColorProperty);
	crHightBkColor.Set(nID++,  RGB(49, 49, 49), _T("���ۼ�������ɫ"), &m_ayColorProperty, PS_DOT);

	clFixedCodeTXT .Set(nID++,  RGB(68, 226, 220), _T("����֤ȯ����"), &m_ayColorProperty);  
	clFixedNameTXT.Set(nID++,  RGB(249, 219, 9), _T("����֤ȯ����"), &m_ayColorProperty);  
	clBoxLineWidth.Set(nID++,  RGB(178, 178, 178), _T("������߿�(������)"), &m_ayColorProperty ,PS_SOLID,  1);//
	clDrawBoxValue.Set(nID++,  RGB(178, 178, 178), _T("�������ֶ��ı�"), &m_ayColorProperty);
	clBoxLine.Set(nID++,  RGB(255, 255, 255), _T("������߿�ɫ"),NULL);

	clBottomBK.Set(nID++,  RGB(0, 0, 128), _T("�ײ�ʱ����ʾ����"), &m_ayColorProperty ,PS_SOLID,  1);//
	clBottomChoose.Set(nID++,  RGB(255, 255, 0), _T("�ײ��л��ı���ɫ"), &m_ayColorProperty ,PS_SOLID,  1);//
	clInfoLine.Set(nID++,  RGB(231, 198, 16), _T("��Ϣ���ױ߿���ɫ"), &m_ayColorProperty ,PS_SOLID,  1);//
	clInfoBk.Set(nID++,  RGB(16, 16, 66), _T("��Ϣ���ױ���"), &m_ayColorProperty ,PS_SOLID,  1);
	clInfoHilight.Set(nID++,  RGB(0, 130, 170), _T("��Ϣ���׸���"), &m_ayColorProperty ,PS_SOLID,  1);
	clSplitLine.Set(nID++,  RGB(255, 255, 255), _T("�ָ���"), &m_ayColorProperty ,PS_SOLID,  1);
	clInChartText.Set(nID++, RGB(230, 230, 230), _T("ͼԪ���ı�"), &m_ayColorProperty); 

	// ȱʡָ����ɫ
	ColorProperty* pColor = new ColorProperty;
	pColor->Set(nID++, RGB(230,  230,  230), _T("ȱʡָ����1"), &m_ayColorProperty,  PS_SOLID ,  1); // Brown
	m_ayDefTechColor.Add(pColor);
	pColor = new ColorProperty;
	pColor->Set(nID++, RGB(234,  234,  0), _T("ȱʡָ����2"), &m_ayColorProperty,  PS_SOLID,  1); // _T("Dark Olive Green")  }, 
	m_ayDefTechColor.Add(pColor);
	pColor = new ColorProperty;
	pColor->Set(nID++, RGB(213,  0,  186), _T("ȱʡָ����3"), &m_ayColorProperty,  PS_SOLID,  1); //     _T("Dark Green")        }, 
	m_ayDefTechColor.Add(pColor);
	pColor = new ColorProperty;
	pColor->Set(nID++, RGB(68,  226,  220), _T("ȱʡָ����4"), &m_ayColorProperty,  PS_SOLID,  1); //         _T("Dark Teal")         }, 
	m_ayDefTechColor.Add(pColor);
	pColor = new ColorProperty;
	pColor->Set(nID++, RGB(0,  0,  255), _T("ȱʡָ����5"), &m_ayColorProperty,  PS_SOLID,  1); //    _T("Dark blue")         }, 
	m_ayDefTechColor.Add(pColor);
	pColor = new ColorProperty;
	m_ayDefTechColor.Add(pColor);
	pColor->Set(nID++, RGB(0,  255,  0), _T("ȱʡָ����6"), &m_ayColorProperty,  PS_SOLID,  1); //    _T("Indigo")            }, 
	pColor = new ColorProperty;
	m_ayDefTechColor.Add(pColor);
	pColor->Set(nID++, RGB(128,  128,  128), _T("ȱʡָ����7"), &m_ayColorProperty,  PS_SOLID,  1); //    _T("Dark grey")         }, 
	pColor = new ColorProperty;
	m_ayDefTechColor.Add(pColor);
	pColor->Set(nID++, RGB(128,  128,  0), _T("ȱʡָ����8"), &m_ayColorProperty,  PS_SOLID,  1); //    _T("Dark red")          }, 
	pColor = new ColorProperty;
	m_ayDefTechColor.Add(pColor);
	pColor->Set(nID++, RGB(128,  0,  0), _T("ȱʡָ����9"), &m_ayColorProperty,  PS_SOLID,  1); //    _T("Orange")            }, 
	pColor = new ColorProperty;
	m_ayDefTechColor.Add(pColor);
	pColor->Set(nID++, RGB(0,  128,  128), _T("ȱʡָ����10"), &m_ayColorProperty,  PS_SOLID,  1); //    _T("Dark yellow")       }, 
	pColor = new ColorProperty;
	m_ayDefTechColor.Add(pColor);
	pColor->Set(nID++, RGB(0,  0,  128), _T("ȱʡָ����11"), &m_ayColorProperty,  PS_SOLID,  1); //    _T("Green")             }, 
	pColor = new ColorProperty;
	m_ayDefTechColor.Add(pColor);
	pColor->Set(nID++, RGB(0,  128,  0), _T("ȱʡָ����12"), &m_ayColorProperty,  PS_SOLID,  1); //    _T("Teal")              }, 
	pColor = new ColorProperty;
	m_ayDefTechColor.Add(pColor);
	pColor->Set(nID++, RGB(0,  0,  255), _T("ȱʡָ����13"), &m_ayColorProperty,  PS_SOLID,  1); //    _T("Blue")              }, 
	pColor = new ColorProperty;
	m_ayDefTechColor.Add(pColor);
	pColor->Set(nID++, RGB(123,  123,  192), _T("ȱʡָ����14"), &m_ayColorProperty,  PS_SOLID,  1); //    _T("Blue-grey")         }, 
	pColor = new ColorProperty;
	m_ayDefTechColor.Add(pColor);
	pColor->Set(nID++, RGB(102,  102,  102), _T("ȱʡָ����15"), &m_ayColorProperty,  PS_SOLID,  1); //    _T("Grey - 40")         }, 
	pColor = new ColorProperty;
	m_ayDefTechColor.Add(pColor);
	pColor->Set(nID++, RGB(0xFF,  0x00,  0x00), _T("ȱʡָ����16"), &m_ayColorProperty,  PS_SOLID,  1); //    _T("Red")               }, 
	pColor = new ColorProperty;
	m_ayDefTechColor.Add(pColor);
	pColor->Set(nID++, RGB(255,  173,  91), _T("ȱʡָ����17"), &m_ayColorProperty,  PS_SOLID,  1); //    _T("Light orange")      }, 
	pColor = new ColorProperty;
	m_ayDefTechColor.Add(pColor);
	pColor->Set(nID++, RGB(50,  205,  50), _T("ȱʡָ����18"), &m_ayColorProperty,  PS_SOLID,  1); //    _T("Lime")              },  
	pColor = new ColorProperty;
	m_ayDefTechColor.Add(pColor);
	pColor->Set(nID++, RGB(60,  179,  113), _T("ȱʡָ����19"), &m_ayColorProperty,  PS_SOLID,  1); //    _T("Sea green")         }, 
	pColor = new ColorProperty;
	m_ayDefTechColor.Add(pColor);
	pColor->Set(nID++, RGB(255,  255,  0), _T("ȱʡָ����20"), &m_ayColorProperty,  PS_SOLID,  1); //    _T("Yellow")            },     
	pColor = new ColorProperty;
	m_ayDefTechColor.Add(pColor);
	pColor->Set(nID++, RGB(0,  255,  0), _T("ȱʡָ����21"), &m_ayColorProperty,  PS_SOLID,  1); //    _T("Bright green")      }, 
	pColor = new ColorProperty;
	m_ayDefTechColor.Add(pColor);
	pColor->Set(nID++, RGB(104,  131,  139), _T("ȱʡָ����22"), &m_ayColorProperty,  PS_SOLID,  1); //    _T("Pale blue")         }, 
	pColor = new ColorProperty;
	m_ayDefTechColor.Add(pColor);
	pColor->Set(nID++, RGB(255,  255,  255), _T("ȱʡָ����23"), &m_ayColorProperty,  PS_SOLID,  1); //    _T("White")             }

//����
	ftXAxis.Set( nID++,  CreateFont("System",  12,  FONT_BOLD),  _T("������"),  &m_ayFontProperty);			// ��������
	ftYAxis.Set( nID++,  CreateFont("System",  12,  FONT_BOLD), _T("������"),  &m_ayFontProperty);			// ��������
	ftMulti.Set( nID++,  CreateFont("Arial",  12,  FW_NORMAL), _T("�Ŵ���"),  NULL);	
	ftDrawVarTitle.Set( nID++,  CreateFont("����", 12,  FONT_NORMAL), _T("ͼԪ��������"),  &m_ayFontProperty);	// ����˵��
	ftDrawInText.Set( nID++,  CreateFont("Arial",  12,  FW_NORMAL), _T("ͼԪ���ı�"),  &m_ayFontProperty);		// ͼ���ڱ����ı�
	ftPopupText.Set( nID++,  CreateFont("Arial",  14,  FW_NORMAL), _T("����С�����ı�"),  NULL);
	ftPeriod.Set(nID++,  CreateFont("����", 12,  FONT_BOLD),  _T("��������"),  &m_ayFontProperty);	// K��������������
	ftTab.Set( nID++,  CreateFont("����",  12,  FONT_NORMAL),  _T("Tab��ǩ"),  &m_ayFontProperty);	// ��ǩ�ı�

	m_sStyle.SetStyle(GL_FenShiHorAxes | GL_FenShiVerAxes | GL_RIGHTSTARTZOOM | GL_BOTTOMSTARTZOOM | GL_STOCK_INFO_SHOW |	GL_PERIOD_TYPE_DAY_CHART_DRAG | 
		GL_PERIOD_TYPE_DAY_TIP_DATA | GL_PERIOD_TYPE_DAY_CHUQUAN_TEXT |	GL_PERIOD_TYPE_DAY_AUTO_CHUQUAN | GL_TITLE_ADVISE_ICON_SHOW |  
		GL_PERIOD_TYPE_DAY_HUOYANSHAN_COST | GL_PERIOD_TYPE_DAY_SHOWHIDE_COST |	GL_KLine_TopLine | GL_STOCK_INFO_SHOW_EX);	

	// ����ͼƬ��Դ
	IQuoteLang* pQuoteLang = (IQuoteLang*)CWinnerApplication::GetObject(OBJ_QUOTELANG);
	HANDLE handle = pQuoteLang->LoadHImage(IDB_TECH_ICON, IMAGE_BITMAP, 0, 0, LR_DEFAULTCOLOR);
	if (handle)
	{
		CBitmap bit;
		bit.Attach((HBITMAP)handle);
		m_expressImages = new CImageListEx;
		if(!m_expressImages->Create(16, 16, ILC_COLOR24 | ILC_MASK, 1, 1))
		{
			delete m_expressImages;
			m_expressImages = NULL;
		}
		else
			m_expressImages->Add(&bit, RGB(255,0,255));
		bit.DeleteObject();
	}

	return TRUE;
}

CFont* CGraphicsData::CreateFont(char * cFontName,int nFontSize, int nFontStyle)
{    
	CFont *pNewFont = new CFont;
	if (pNewFont == NULL)
	{
		return NULL;
	}
	if((cFontName == NULL) || cFontName[0] == '\0')
	{
		cFontName = "����";
		nFontSize = 16;
		nFontStyle = 0;
	}
	LOGFONT font;
	memset(&font, 0, sizeof(font));
	font.lfHeight  = -nFontSize;
	font.lfWeight  = FW_NORMAL;
	font.lfCharSet = DEFAULT_CHARSET;//GB2312_CHARSET;
	strcpy(font.lfFaceName, cFontName);
	if (nFontStyle & FONT_BOLD)
	{
		font.lfWeight = FW_BOLD;
	}
	if (nFontStyle & FONT_ITALIC)
	{
		font.lfItalic = TRUE;
	}
	if (nFontStyle & FONT_UNDERLINE)
	{
		font.lfUnderline = TRUE;
	}
	if(!pNewFont->CreateFontIndirect(&font)) // CreatePointFontIndirect
	{
		delete pNewFont;
		pNewFont = NULL;
	}

	return pNewFont;
}

BOOL CGraphicsData::IsValue(double* lValue,...)
{
	if( (*lValue == (double)hx_DefMinValues) || 
		(*lValue == (double)hx_DefMaxValues) )
		return FALSE;

	return TRUE;
}

long CGraphicsData::HsGetYearMothDay(long lDate)
{
	lDate /= 10000;
	if( (lDate / 100000) > 0 )
	{
		lDate -= 100000;
		lDate += 20000000;
	}
	else
	{
		lDate += 19000000;
	}
	return lDate;
}

ColorProperty*	CGraphicsData::GetDefTechColor(short nIndex)
{
	if( nIndex < 0 || nIndex >= m_ayDefTechColor.GetSize() )
		return NULL;
	return m_ayDefTechColor.GetAt(nIndex);

}