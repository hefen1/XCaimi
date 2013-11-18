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
// 颜色部分
	short nID = 1;
	clPageBK.Set(nID++,  RGB(0, 0, 0), _T("背景"), &m_ayColorProperty);
	clMouseMoveMarkBK.Set(nID++,  RGB(10, 36, 106), _T("移动标记背景"), NULL);
	clUp   .Set(nID++,  RGB(229, 13, 34), _T("上涨"), &m_ayColorProperty);
	clDown .Set(nID++,  RGB(77, 220, 35), _T("下跌"), &m_ayColorProperty);
	clEqual .Set(nID++,  RGB(178, 178, 178), _T("平盘"), &m_ayColorProperty);  
	clPreClose .Set(nID++,  RGB(188, 188, 188), _T("昨收"), &m_ayColorProperty); //8

	clHorLine .Set(nID++,  RGB(155, 0, 0), _T("右图横线"), &m_ayColorProperty);  //横线
	clVerLine .Set(nID++,  RGB(155, 0, 0), _T("右图竖线"), &m_ayColorProperty);   //竖线

	clJsAvgLine.Set(nID++, RGB(237, 217, 39),_T( "分时图.均线"), &m_ayColorProperty);   //均线
	clJsPriceLine.Set(nID++, RGB(230, 230, 230), _T("分时图.价格线"), &m_ayColorProperty);
	clJsDownVolLine.Set(nID++, RGB(80, 228, 221), _T("分时图.成交量跌线"), &m_ayColorProperty);	// 成交量跌
	clJsUpVolLine.Set(nID++, RGB(229, 13, 34), _T("分时图.成交量涨线"), &m_ayColorProperty);	// 成交量涨
	clVerAmountTXT.Set(nID++, RGB(237, 217, 39), _T("成交量坐标文本"), &m_ayColorProperty);	// 成交量纵坐标文本
	clDotHorLine .Set(nID++,  RGB(155, 0, 0), _T("分时图.网格横线"), &m_ayColorProperty,  PS_SOLID,  1);
	clDotVerLine .Set(nID++,  RGB(128, 0, 0), _T("分时图.网格竖线"),NULL );
	clJsPriceMiddleLine.Set(nID++, RGB(155, 0, 0), _T("分时图.中间横线"), &m_ayColorProperty,  PS_SOLID,  2);
	clJsPriceMiddleVerLine.Set(nID++, RGB(155, 0, 0), _T("分时图.中间竖线"), &m_ayColorProperty, PS_SOLID, 2);
	clJsPriceHourLine.Set(nID++, RGB(155, 0, 0), _T("分时图.小时线"), &m_ayColorProperty, PS_SOLID, 1);
	clJsPriceHalfHourLine.Set(nID++, RGB(155, 0, 0), _T("分时图.半小时线"), &m_ayColorProperty, PS_DOT, 1);
	clGridLine.Set(nID++,  RGB(155,  0,  0), _T("分时,日线图坐标轴"), &m_ayColorProperty,  PS_SOLID,  1);  //图形边框线
	clChartTick.Set(nID++,  RGB(155,  0,  0), _T("分时,日线坐标刻度线"), &m_ayColorProperty,  PS_SOLID,  1);   //图形坐标刻度线

	clChartTitle.Set(nID++,  RGB(0,  192,  192), _T("图形标题"), NULL);	
	clChartBK.Set(nID++,  RGB(0,  192,  0), _T("图形背景"), NULL); 

	// 日线
	clHorTXT.Set(nID++,  RGB(192,  0,  0), _T("日线横坐标文本"), &m_ayColorProperty);
	clChartTickTXT.Set(nID++,  RGB(192,  0,  0), _T("日线纵坐标文本"), &m_ayColorProperty);
	clDayGridVerLine.Set(nID++, RGB(168, 0, 0), _T("日线顶部横线")/*, &m_ayColorProperty*/);	 // 日线网格顶部横线
	clDayUpLine.Set(nID++, RGB(255, 84, 84), _T("日线图.上涨线(阳线)"), &m_ayColorProperty);		// 上涨线(阳线)
	clDayDownLine.Set(nID++, RGB(84, 255, 255), _T("日线图.下跌线(阴线)"), &m_ayColorProperty);		// 下跌线(阴线)
	clDayUpFillLine.Set(nID++, RGB(0, 0, 0), _T("日线图.阳线填充色"), &m_ayColorProperty);  //上涨填充(阳线)
	clDayDownFillLine.Set(nID++, RGB(84, 255, 255),_T( "日线图.阴线填充色"), &m_ayColorProperty);  //下跌填充(阴线)
	clMaxMinValueBk.Set(nID++,  RGB(0, 0, 245), _T("最大最小值加亮背景"), &m_ayColorProperty);
	clMaxMinValueFK.Set(nID++,  RGB(128, 0, 0), _T("最大最小值加亮前景(未使用)"), NULL);
	clDayHorLine.Set(nID++,  RGB(78, 0, 0), _T("日线图.网格横线"), &m_ayColorProperty, PS_DOT,  PS_SOLID);
	clDayVerLine.Set(nID++,  RGB(155, 0, 0), _T("日线图.网格竖线"), &m_ayColorProperty, PS_DOT,  PS_SOLID);
	clDayTopLine.Set(nID++,  RGB(155, 0, 0), _T("日线图.顶部横线"), &m_ayColorProperty ,PS_DOT,  1);

	clInChartTitleTXT.Set(nID++,  RGB(255, 255, 0), _T("图元内顶部标题文本"), NULL);	
	clVerDayTXT.Set(nID++, RGB(204, 204, 0), _T("未使用"), NULL);		
	clDayAmericaLine.Set(nID++, RGB(0, 255, 255), _T("未使用"), NULL);	
	clDayTrendLine.Set(nID++, RGB(0, 255, 255), _T("未使用"), NULL);		
	clDayZeroHorLine.Set(nID++, RGB(0, 255, 255), _T("未使用"), NULL);
	clMoveLine.Set(nID++, RGB(98, 98, 98),_T( "移动光标线"), &m_ayColorProperty,  PS_SOLID,  1);

	clVerLiangBiTXT.Set(nID++, RGB(176, 0, 0), _T("量比指标坐标文本"), &m_ayColorProperty);	// 量比纵坐标文本	
	clVerADLTXT.Set(nID++, RGB(176, 0, 0), _T("ADL指标坐标文本"), &m_ayColorProperty);		//ＡＤＬ纵坐标文本
	clJsLiangBiLine.Set(nID++, RGB(230, 230, 230), _T("量比指标线"), &m_ayColorProperty);    // 量比指标
	clJsADLLine.Set(nID++, RGB(230, 230, 230),_T( "ADL指标线"), &m_ayColorProperty,  PS_SOLID);
	clVerDouKongTXT.Set(nID++, RGB(176, 0, 0), _T("多空指标坐标文本"), &m_ayColorProperty);	// 多空纵坐标文本
	clVerBuySellTXT.Set(nID++, RGB(176, 0, 0), _T("买卖力道坐标文本"), &m_ayColorProperty);		// 买卖力道纵坐标文本
	clJsDouKongLine.Set(nID++, RGB(230,230,230), _T("多空指标线"), &m_ayColorProperty,  PS_SOLID);
	crDiffValueColor.Set(nID++, RGB(255, 62, 255), _T("买卖差值"), &m_ayColorProperty);  
	clJsBuyLine.Set(nID++, RGB(255, 255, 84), _T("买卖力道之买量线"), &m_ayColorProperty);  //买卖力道之买量
	clJsSellLine.Set(nID++, RGB(230, 230, 230), _T("买卖力道之卖量线"), &m_ayColorProperty);  //买卖力道之卖量
	clJsIOPVLine.Set(nID++, RGB(15, 255, 15),_T( "IOPV线"), &m_ayColorProperty);
	crHightBkColor.Set(nID++,  RGB(49, 49, 49), _T("报价加亮背景色"), &m_ayColorProperty, PS_DOT);

	clFixedCodeTXT .Set(nID++,  RGB(68, 226, 220), _T("报价证券代码"), &m_ayColorProperty);  
	clFixedNameTXT.Set(nID++,  RGB(249, 219, 9), _T("报价证券名称"), &m_ayColorProperty);  
	clBoxLineWidth.Set(nID++,  RGB(178, 178, 178), _T("浮动框边框(须重启)"), &m_ayColorProperty ,PS_SOLID,  1);//
	clDrawBoxValue.Set(nID++,  RGB(178, 178, 178), _T("浮动框字段文本"), &m_ayColorProperty);
	clBoxLine.Set(nID++,  RGB(255, 255, 255), _T("浮动框边框色"),NULL);

	clBottomBK.Set(nID++,  RGB(0, 0, 128), _T("底部时间显示背景"), &m_ayColorProperty ,PS_SOLID,  1);//
	clBottomChoose.Set(nID++,  RGB(255, 255, 0), _T("底部切换文本颜色"), &m_ayColorProperty ,PS_SOLID,  1);//
	clInfoLine.Set(nID++,  RGB(231, 198, 16), _T("信息地雷边框颜色"), &m_ayColorProperty ,PS_SOLID,  1);//
	clInfoBk.Set(nID++,  RGB(16, 16, 66), _T("信息地雷背景"), &m_ayColorProperty ,PS_SOLID,  1);
	clInfoHilight.Set(nID++,  RGB(0, 130, 170), _T("信息地雷高亮"), &m_ayColorProperty ,PS_SOLID,  1);
	clSplitLine.Set(nID++,  RGB(255, 255, 255), _T("分割线"), &m_ayColorProperty ,PS_SOLID,  1);
	clInChartText.Set(nID++, RGB(230, 230, 230), _T("图元内文本"), &m_ayColorProperty); 

	// 缺省指标颜色
	ColorProperty* pColor = new ColorProperty;
	pColor->Set(nID++, RGB(230,  230,  230), _T("缺省指标线1"), &m_ayColorProperty,  PS_SOLID ,  1); // Brown
	m_ayDefTechColor.Add(pColor);
	pColor = new ColorProperty;
	pColor->Set(nID++, RGB(234,  234,  0), _T("缺省指标线2"), &m_ayColorProperty,  PS_SOLID,  1); // _T("Dark Olive Green")  }, 
	m_ayDefTechColor.Add(pColor);
	pColor = new ColorProperty;
	pColor->Set(nID++, RGB(213,  0,  186), _T("缺省指标线3"), &m_ayColorProperty,  PS_SOLID,  1); //     _T("Dark Green")        }, 
	m_ayDefTechColor.Add(pColor);
	pColor = new ColorProperty;
	pColor->Set(nID++, RGB(68,  226,  220), _T("缺省指标线4"), &m_ayColorProperty,  PS_SOLID,  1); //         _T("Dark Teal")         }, 
	m_ayDefTechColor.Add(pColor);
	pColor = new ColorProperty;
	pColor->Set(nID++, RGB(0,  0,  255), _T("缺省指标线5"), &m_ayColorProperty,  PS_SOLID,  1); //    _T("Dark blue")         }, 
	m_ayDefTechColor.Add(pColor);
	pColor = new ColorProperty;
	m_ayDefTechColor.Add(pColor);
	pColor->Set(nID++, RGB(0,  255,  0), _T("缺省指标线6"), &m_ayColorProperty,  PS_SOLID,  1); //    _T("Indigo")            }, 
	pColor = new ColorProperty;
	m_ayDefTechColor.Add(pColor);
	pColor->Set(nID++, RGB(128,  128,  128), _T("缺省指标线7"), &m_ayColorProperty,  PS_SOLID,  1); //    _T("Dark grey")         }, 
	pColor = new ColorProperty;
	m_ayDefTechColor.Add(pColor);
	pColor->Set(nID++, RGB(128,  128,  0), _T("缺省指标线8"), &m_ayColorProperty,  PS_SOLID,  1); //    _T("Dark red")          }, 
	pColor = new ColorProperty;
	m_ayDefTechColor.Add(pColor);
	pColor->Set(nID++, RGB(128,  0,  0), _T("缺省指标线9"), &m_ayColorProperty,  PS_SOLID,  1); //    _T("Orange")            }, 
	pColor = new ColorProperty;
	m_ayDefTechColor.Add(pColor);
	pColor->Set(nID++, RGB(0,  128,  128), _T("缺省指标线10"), &m_ayColorProperty,  PS_SOLID,  1); //    _T("Dark yellow")       }, 
	pColor = new ColorProperty;
	m_ayDefTechColor.Add(pColor);
	pColor->Set(nID++, RGB(0,  0,  128), _T("缺省指标线11"), &m_ayColorProperty,  PS_SOLID,  1); //    _T("Green")             }, 
	pColor = new ColorProperty;
	m_ayDefTechColor.Add(pColor);
	pColor->Set(nID++, RGB(0,  128,  0), _T("缺省指标线12"), &m_ayColorProperty,  PS_SOLID,  1); //    _T("Teal")              }, 
	pColor = new ColorProperty;
	m_ayDefTechColor.Add(pColor);
	pColor->Set(nID++, RGB(0,  0,  255), _T("缺省指标线13"), &m_ayColorProperty,  PS_SOLID,  1); //    _T("Blue")              }, 
	pColor = new ColorProperty;
	m_ayDefTechColor.Add(pColor);
	pColor->Set(nID++, RGB(123,  123,  192), _T("缺省指标线14"), &m_ayColorProperty,  PS_SOLID,  1); //    _T("Blue-grey")         }, 
	pColor = new ColorProperty;
	m_ayDefTechColor.Add(pColor);
	pColor->Set(nID++, RGB(102,  102,  102), _T("缺省指标线15"), &m_ayColorProperty,  PS_SOLID,  1); //    _T("Grey - 40")         }, 
	pColor = new ColorProperty;
	m_ayDefTechColor.Add(pColor);
	pColor->Set(nID++, RGB(0xFF,  0x00,  0x00), _T("缺省指标线16"), &m_ayColorProperty,  PS_SOLID,  1); //    _T("Red")               }, 
	pColor = new ColorProperty;
	m_ayDefTechColor.Add(pColor);
	pColor->Set(nID++, RGB(255,  173,  91), _T("缺省指标线17"), &m_ayColorProperty,  PS_SOLID,  1); //    _T("Light orange")      }, 
	pColor = new ColorProperty;
	m_ayDefTechColor.Add(pColor);
	pColor->Set(nID++, RGB(50,  205,  50), _T("缺省指标线18"), &m_ayColorProperty,  PS_SOLID,  1); //    _T("Lime")              },  
	pColor = new ColorProperty;
	m_ayDefTechColor.Add(pColor);
	pColor->Set(nID++, RGB(60,  179,  113), _T("缺省指标线19"), &m_ayColorProperty,  PS_SOLID,  1); //    _T("Sea green")         }, 
	pColor = new ColorProperty;
	m_ayDefTechColor.Add(pColor);
	pColor->Set(nID++, RGB(255,  255,  0), _T("缺省指标线20"), &m_ayColorProperty,  PS_SOLID,  1); //    _T("Yellow")            },     
	pColor = new ColorProperty;
	m_ayDefTechColor.Add(pColor);
	pColor->Set(nID++, RGB(0,  255,  0), _T("缺省指标线21"), &m_ayColorProperty,  PS_SOLID,  1); //    _T("Bright green")      }, 
	pColor = new ColorProperty;
	m_ayDefTechColor.Add(pColor);
	pColor->Set(nID++, RGB(104,  131,  139), _T("缺省指标线22"), &m_ayColorProperty,  PS_SOLID,  1); //    _T("Pale blue")         }, 
	pColor = new ColorProperty;
	m_ayDefTechColor.Add(pColor);
	pColor->Set(nID++, RGB(255,  255,  255), _T("缺省指标线23"), &m_ayColorProperty,  PS_SOLID,  1); //    _T("White")             }

//字体
	ftXAxis.Set( nID++,  CreateFont("System",  12,  FONT_BOLD),  _T("横坐标"),  &m_ayFontProperty);			// 横向坐标
	ftYAxis.Set( nID++,  CreateFont("System",  12,  FONT_BOLD), _T("纵坐标"),  &m_ayFontProperty);			// 纵向坐标
	ftMulti.Set( nID++,  CreateFont("Arial",  12,  FW_NORMAL), _T("放大倍数"),  NULL);	
	ftDrawVarTitle.Set( nID++,  CreateFont("宋体", 12,  FONT_NORMAL), _T("图元顶部标题"),  &m_ayFontProperty);	// 顶部说明
	ftDrawInText.Set( nID++,  CreateFont("Arial",  12,  FW_NORMAL), _T("图元内文本"),  &m_ayFontProperty);		// 图形内标题文本
	ftPopupText.Set( nID++,  CreateFont("Arial",  14,  FW_NORMAL), _T("浮动小窗口文本"),  NULL);
	ftPeriod.Set(nID++,  CreateFont("宋体", 12,  FONT_BOLD),  _T("日线周期"),  &m_ayFontProperty);	// K线周期字体设置
	ftTab.Set( nID++,  CreateFont("宋体",  12,  FONT_NORMAL),  _T("Tab标签"),  &m_ayFontProperty);	// 标签文本

	m_sStyle.SetStyle(GL_FenShiHorAxes | GL_FenShiVerAxes | GL_RIGHTSTARTZOOM | GL_BOTTOMSTARTZOOM | GL_STOCK_INFO_SHOW |	GL_PERIOD_TYPE_DAY_CHART_DRAG | 
		GL_PERIOD_TYPE_DAY_TIP_DATA | GL_PERIOD_TYPE_DAY_CHUQUAN_TEXT |	GL_PERIOD_TYPE_DAY_AUTO_CHUQUAN | GL_TITLE_ADVISE_ICON_SHOW |  
		GL_PERIOD_TYPE_DAY_HUOYANSHAN_COST | GL_PERIOD_TYPE_DAY_SHOWHIDE_COST |	GL_KLine_TopLine | GL_STOCK_INFO_SHOW_EX);	

	// 加载图片资源
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
		cFontName = "宋体";
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