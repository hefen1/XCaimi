#include "StdAfx.h"
#include ".\generalhelper.h"
#include <math.h>
#include <io.h>
#include <sys/stat.h>

CGeneralHelper::CGeneralHelper(void)
{
}

CGeneralHelper::~CGeneralHelper(void)
{
}

CString CGeneralHelper::GetAppPath()
{
	CString strAppPath;
	CString strPath;
	DWORD dwSize=MAX_PATH;
	::GetModuleFileName(NULL,strPath.GetBuffer(MAX_PATH),dwSize); //AfxGetResourceHandle()
	strPath.ReleaseBuffer(dwSize);
	strAppPath=strPath.Left(strPath.ReverseFind('\\')+1);
	return strAppPath;
}

CString CGeneralHelper::GetSpecifyPath(PathFlag flag)
{
	CString strAppPath = CGeneralHelper::GetAppPath();
	switch (flag)
	{
	case SYSCONFIG_PATH:
		return strAppPath + _T("config\\");
	case FACE_PATH:
		return strAppPath + _T("face\\");
	case SETTING_PATH:
		return strAppPath + _T("Setting\\");
	case BLOCK_PATH:
		return strAppPath + _T("Block\\");
	case DATA_PATH:
		return strAppPath + _T("Data\\");
	case ZIXUN_PATH:
		return strAppPath + _T("InfoData\\");
	}
	return _T("");
}

void CGeneralHelper::GetEnvironmentString(CStringArray &envArr)
{
	PTSTR pEnvBlock = ::GetEnvironmentStrings();
	// Parse the block with the following format:
	// =::=::\
	// =...
	// var=value\0
	// ...
	// var=value\0\0
	// Note that some other strings might begin with '='.
	// Here is an example when the application is started from a network share.
	// [0] =::=::\
	// [1] =C:=C:\Windows\System32
	//
	PTSTR pszCurrent = pEnvBlock;
	while (pszCurrent != NULL) 
	{
		CString strOneEnvItem(pszCurrent,_tcslen(pszCurrent));
		envArr.Add(strOneEnvItem);
		while (*pszCurrent != TEXT('\0'))
		{
			pszCurrent++;
		}
		pszCurrent++;
		// Check if it was not the last string.
		if(*pszCurrent == TEXT('\0'))
			break;
	};
	FreeEnvironmentStrings(pEnvBlock);
}

CString CGeneralHelper::GetEnvironmentString()
{
	CStringArray strArr;
	CString strEnvs;
	GetEnvironmentString(strArr);
	for(int i=0; i < strArr.GetCount();i++)
	{
		strEnvs = strEnvs + strArr[i] + _T('\n');
	}	
	return strEnvs;
}

//////////////////////////////////////////////////////////////////////////
long long CGeneralHelper::GetDivision(long long x, long y, int nRoundType /* = 0 */)
{
	long long round;
	long double v = (long double)x / (double)y;

	if(nRoundType > 0)			// 小数进位
		round = (long long)(ceil(v) + .5);
	else if(nRoundType == 0)		// 四舍五入
		round = (long long)(v + .5);
	else				// 小数舍去
		round = (long long)(floor(v) + .5);

	return round;
}


void CGeneralHelper::LongToString(long lData, long lClose, long lUnit, short nDecimal, char* strText,short lMask /* = 0 */)
{
	if( lUnit == 0 ) 
	{
		lUnit = 1;
	}
	if (nDecimal > 10)
	{
		nDecimal = 2;
	}

	if( lData==0 || lUnit<0 )		// 数据为 0
	{
		if( lMask & LTOA_MASK_NOTHING_IFZERO )		// 显示为空
		{
			strcpy(strText,  _T(" "));
		}
		else if (lMask & LTOA_MASK_FIXEDLEN_IFZERO )	// 显示为一串0
		{
			strText[0] = ' ';
			char strFor[16]; 
			sprintf(strFor,  "%%.%dlf", nDecimal);
			sprintf(&strText[1], strFor,  0.000001);
			if (lMask & LTOA_MASK_SHOW_PERSENT)
			{
				strcat(strText,"%");
			}
		}
		else		// 默认显示为 0
		{
			strcpy(strText, _T(" -"));
		}
	}
	else			// 数据不为 0
	{		
		if(lData > lClose)		// 涨
		{
			if( lMask & LTOA_MASK_SHOW_PLUS ) // 显示 +
			{
				strText[0] = '+';
			}
			else
			{
				strText[0] = ' ';
			}
		}
		else if(lData == lClose)	// 平
		{		
			strText[0] = ' ';
		}
		else					// 跌
		{
			if( lMask & LTOA_MASK_SHOW_SUB ) // 显示 -
			{
				strText[0] = '-';
			}
			else
			{
				strText[0] = ' ';
			}
		}
 
		char strFormat[16];
		if(nDecimal < 0)				//长度限定
		{
			sprintf(strFormat,  "%%0%dld", -nDecimal);
			sprintf(&strText[1], strFormat,  (long)(lData/lUnit));
		}
		else if(nDecimal == 0)
		{
			if( (float)lData/lUnit > 0.01 )
			{
				sprintf(&strText[1],  "%.lf",  ((float)lData/lUnit));
			}
			else
			{
				if( lMask & LTOA_MASK_NOTHING_IFZERO )		// 显示为空
				{
					strcpy(strText,  _T(" "));
				}
				else		// 默认显示为 -
				{
					strcpy(strText,  _T(" -"));
				}
			}
		}                           
		else
		{
			sprintf(strFormat,  "%%.%dlf", nDecimal);
			sprintf(&strText[1], strFormat,  (double)lData/(double)lUnit);
		}
		if( lMask & LTOA_MASK_SHOW_PERSENT )
		{
			strcat(strText,  "%");
		}
	}

}

void CGeneralHelper::LongToString(double long lData, long lClose, long lUnit, short nDecimal, char* strText,short lMask /* = 0 */)
{
	if( lUnit == 0 ) 
	{
		lUnit = 1;
	}

	if(abs(lData) < 0.000001 || (lUnit < 0) )
	{
		if( lMask & LTOA_MASK_NOTHING_IFZERO )		// 显示为空
		{
			strcpy(strText,  _T(" "));
		}
		else if (lMask & LTOA_MASK_FIXEDLEN_IFZERO )	// 显示为一串0
		{
			strText[0] = ' ';
			char strFor[16];
			sprintf(strFor,  "%%.%dlf", nDecimal);
			sprintf(&strText[1], strFor,  0.000001);
			if (lMask & LTOA_MASK_SHOW_PERSENT)
			{
				strcat(strText,"%");
			}
		}
		else		// 默认显示为 -
		{
			strcpy(strText,  _T(" -"));
		}
	}
	else
	{		
		if(lData > lClose)
		{
			if( lMask & LTOA_MASK_SHOW_PLUS ) // 显示 +
			{
				strText[0] = '+';
			}
			else
			{
				strText[0] = ' ';
			}
		}
		else if(lData == lClose)
		{		
			strText[0] = ' ';
		}
		else
		{
			if( lMask & LTOA_MASK_SHOW_SUB ) // 显示 -
			{
				strText[0] = '-';
			}
			else
			{
				strText[0] = ' ';
			}
		}
		char strFormat[16];
		if(nDecimal < 0)
		{
			sprintf(strFormat,  "%%0%dld", -nDecimal);
			sprintf(&strText[1], strFormat,  (long)(lData/lUnit));
		}
		else if(nDecimal == 0)
		{
			strcpy(strFormat, "%ld");
			if(lMask & LTOA_MASK_SHOWEX_UNITAUTO) //自动选择单位
			{
				lData = lData / lUnit;
				if(lData >= OneHundredMillion)
				{
					lData = lData/OneHundredMillion;
					sprintf(&strText[1], strFormat,  (long)lData);
					strcat(strText, "亿");
				}
				else if(lData >= TenThousand)
				{
					lData = lData / TenThousand;
					sprintf(&strText[1], strFormat, (long)lData);
					strcat(strText, "万");
				}
				else
				{
					sprintf(&strText[1], "%d", (long)lData);
				}
			}
			else if(lMask & LTOA_MASK_SHOWEX_UNITYI) //固定显示"亿"
			{
				lData = lData / lUnit;
				if(lData >= OneHundredMillion)
				{
					lData = lData / OneHundredMillion;
					sprintf(&strText[1], strFormat, (long)lData);
					strcat(strText, "亿");
				}
				else
					sprintf(&strText[1], strFormat, (long)lData);
			}
			else if(lMask & LTOA_MASK_SHOWEX_UNITWAN) //固定显示"万"
			{
				lData = lData / lUnit;
				if(lData >= TenThousand)
				{
					lData = lData / TenThousand;
					sprintf(&strText[1], strFormat, (long)lData);
					strcat(strText, "万");
				}
				else
					sprintf(&strText[1], strFormat, (long)lData);
			}
			else
			{
				if( lData/lUnit )
				{
					long v;
					if(!(lMask & LTOA_MASK_DIVISION_NOROUND))
						v = (long)GetDivision((long long)lData, lUnit, lMask & 0x80);
					else
						v = (long)(lData/lUnit);

					sprintf(&strText[1],  "%ld",  v);
				}
				else
				{
					if( lMask & LTOA_MASK_NOTHING_IFZERO )		// 显示为空
					{
						strcpy(strText,  _T(" "));
					}
					else	// 默认显示为 “-”
					{
						strcpy(strText,  _T(" -"));
					}
				}
			}
		}
		else
		{
			sprintf(strFormat,  "%%.%dlf", nDecimal);
			if(lMask & LTOA_MASK_SHOWEX_UNITAUTO) //自动选择单位
			{
				lData = lData / lUnit;
				if(lData >= OneHundredMillion)
				{
					sprintf(&strText[1], strFormat,  lData/OneHundredMillion + 0.00001);
					strcat(strText, "亿");
				}
				else if(lData >= TenThousand)
				{
					sprintf(&strText[1], strFormat,  lData/TenThousand + 0.00001);
					strcat(strText, "万");
				}
				else
				{
					if (lData > 0)
					{// 需要截断小数部分
						sprintf(&strText[1], "%d", (long)lData);
					}
					else
					{
						sprintf(&strText[1], strFormat,  lData + 0.00001);
					}
					
				}
			}
			else if(lMask & LTOA_MASK_SHOWEX_UNITYI) //固定显示"亿"
			{
				lData = lData / lUnit;
				if(lData >= OneHundredMillion)
				{
					sprintf(&strText[1], strFormat,  lData/OneHundredMillion + 0.00001);
					strcat(strText, "亿");
				}
				else
					sprintf(&strText[1], strFormat,  lData + 0.00001);
			}
			else if(lMask & LTOA_MASK_SHOWEX_UNITWAN) //固定显示"万"
			{
				lData = lData / lUnit;
				if(lData >= TenThousand)
				{
					sprintf(&strText[1], strFormat,  lData/TenThousand + 0.00001);
					strcat(strText, "万");
				}
				else
					sprintf(&strText[1], strFormat, lData + 0.00001);
			}
			else
				sprintf(&strText[1], strFormat,  lData/lUnit + 0.00001);
		}
		if( lMask & LTOA_MASK_SHOW_PERSENT )
		{
			strcat(strText,  "%");
		}
	}
}

void CGeneralHelper::LongToString(unsigned long lData, long lClose, long lUnit, short nDecimal, char* strText,short lMask /* = 0 */)
{
	CGeneralHelper::LongToString((double long)lData, lClose, lUnit, nDecimal, strText, lMask);
}

void CGeneralHelper::LongToString(double lData, long lClose, long lUnit, short nDecimal, char* strText,short lMask /* = 0 */)
{
	CGeneralHelper::LongToString((double long)lData, lClose, lUnit, nDecimal, strText, lMask);
}

CString CGeneralHelper::DoubleToString( double lData, int nDecimal /* = 0 */ )
{
	int  dec, sign;
	return fcvt( lData, nDecimal, &dec, &sign );
}

long CGeneralHelper::StringToLong( char* strText, long lClose, long lUnit,short nDecimal )
{
	int   dec, sign;
	char* pBuffer = fcvt( atof(strText), nDecimal, &dec, &sign );

	long lData = atol(pBuffer);
	if(nDecimal > dec)
	{
		nDecimal = (nDecimal - dec);
		lData = (long)(lData * pow((float)10,(int)nDecimal));
	}
	if(lData < lClose)
	{
		lData *= -1;
	}
	return lData;
}
long CGeneralHelper::StringToLongSimple(const char *pBuffer)
{
	ASSERT(pBuffer);
	return atol(pBuffer);
}
COLORREF CGeneralHelper::DelutionColor(COLORREF colSrc, COLORREF colDes, int nPersent)
{
	int nRed = GetRValue(colSrc);
	int nGreen = GetGValue(colSrc);
	int nBlue = GetBValue(colSrc);
	if (nRed < GetRValue(colDes))
	{
		nRed += (GetRValue(colDes)-nRed) * nPersent / 100;
		nRed = nRed>GetRValue(colDes) ? GetRValue(colDes) : nRed;
	} 
	else
	{
		nRed -= (nRed-GetRValue(colDes)) * nPersent / 100;
		nRed = nRed<GetRValue(colDes) ? GetRValue(colDes) : nRed;
	}

	if (nGreen < GetGValue(colDes))
	{
		nGreen += (GetGValue(colDes)-nGreen) * nPersent / 100;
		nGreen = nGreen>GetGValue(colDes) ? GetGValue(colDes) : nGreen;
	} 
	else
	{
		nGreen -= (nGreen-GetGValue(colDes)) * nPersent / 100;
		nGreen = nGreen<GetGValue(colDes) ? GetGValue(colDes) : nGreen;
	}

	if (nBlue < GetBValue(colDes))
	{
		nBlue += (GetBValue(colDes)-nBlue) * nPersent / 100;
		nBlue = nBlue>GetBValue(colDes) ? GetRValue(colDes) : nBlue;
	}
	else
	{
		nBlue -= (nBlue-GetBValue(colDes)) * nPersent / 100;
		nBlue = nBlue<GetBValue(colDes) ? GetBValue(colDes) : nBlue;
	}
	return RGB(nRed,nGreen,nBlue);

}
int  CGeneralHelper::AnalyseStr(CString strResource,const char* strDelimit,CStringArray& ayRet)
{
	ayRet.RemoveAll();

	CString strCur;
	strCur.Format("%s",strResource);
	CString tmp;

	char * next_token = NULL;
	char * token = strtok_s( (char*)LPCSTR(strCur), strDelimit,&next_token);
	while(token != NULL)
	{
		tmp.Format("%s",token);
		ayRet.Add(tmp);
		token = strtok_s(NULL, strDelimit,&next_token);
	}
	return ayRet.GetSize();
}

CFont* CGeneralHelper::CreateFont(char * cFontName,int nFontSize, int nFontStyle)
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
CFont* CGeneralHelper::CreateFont(char * cFontName, int nFontHeight,  int nFontWeight,int nFontItalic)
{
	CFont *pNewFont = new CFont;
	if (pNewFont == NULL)
	{
		return NULL;
	}
	if((cFontName == NULL) || cFontName[0] == '\0')
	{
		cFontName = "宋体";
		nFontHeight = 16;
		nFontWeight = 0;
	}
	LOGFONT font;
	memset(&font, 0, sizeof(font));
	font.lfHeight  = -nFontHeight;
	font.lfWeight  = FW_NORMAL;
	font.lfCharSet = GB2312_CHARSET;
	strcpy(font.lfFaceName, cFontName);
	if (nFontWeight == FONT_BOLD)
	{
		font.lfWeight = FW_BOLD;
	}
	if (nFontItalic)
	{
		font.lfItalic = TRUE;
	}

	if(!pNewFont->CreateFontIndirect(&font)) // CreatePointFontIndirect
	{
		delete pNewFont;
		pNewFont = NULL;
	}

	return pNewFont;
}
CFont* CGeneralHelper::CreateFontEx(char * cFontName, int nFontHeight, int nFontWeight,int nFontItalic, 
									BYTE nOutPre, BYTE nPitch,BYTE nCharset)
{

	CFont *pNewFont = new CFont;
	if (pNewFont == NULL)
	{
		return NULL;
	}
	if((cFontName == NULL) || cFontName[0] == '\0')
	{
		//pNewFont->CreateStockObject(DEVICE_DEFAULT_FONT);
		//return pNewFont;
		cFontName = "宋体";
		nFontHeight = 16;
		nFontWeight = 0;
	}
	LOGFONT font;
	memset(&font, 0, sizeof(font));
	font.lfHeight  = -nFontHeight;
	font.lfWeight  = FW_NORMAL;
	font.lfCharSet = nCharset;
	font.lfOutPrecision = nOutPre;
	font.lfPitchAndFamily = nPitch;
	strcpy(font.lfFaceName, cFontName);
	if (nFontWeight == FONT_BOLD)
	{
		font.lfWeight = FW_BOLD;
	}
	if (nFontItalic)
	{
		font.lfItalic = TRUE;
	}
	if(!pNewFont->CreateFontIndirect(&font)) // CreatePointFontIndirect
	{
		delete pNewFont;
		pNewFont = NULL;
	}
	return pNewFont;
}

int CGeneralHelper::DrawTriangle( HDC hDC, CPoint ptOrg, int iRadius, BOOL bUp )
{//画三角型

	CPoint ptThree[3];
	ptThree[0].x = ptOrg.x;                      
	ptThree[1].x = ptOrg.x + (int)(iRadius*0.866);
	ptThree[2].x = ptOrg.x - (int)(iRadius*0.866);

	if(bUp)
	{
		ptThree[0].y = ptOrg.y - iRadius;
		ptThree[1].y = ptOrg.y + (iRadius+1)/2;
		ptThree[2].y = ptOrg.y + (iRadius+1)/2;
	}	
	else
	{
		ptThree[0].y = ptOrg.y + iRadius;
		ptThree[1].y = ptOrg.y - (iRadius+1)/2;
		ptThree[2].y = ptOrg.y - (iRadius+1)/2;
	}

	return Polygon(hDC,  ptThree, 3);
}

void CGeneralHelper::DrawArrow( CDC * pDC,CRect rect,COLORREF lColor,BOOL bUp )
{

	int iHeight = 10;
	
	CRgn  rgn1;

	short bDown = LOWORD(bUp);
	short bType = HIWORD(bUp);

	CPen hPen;
	hPen.CreatePen(PS_SOLID,  1,  lColor);
	CPen* hOldPen = pDC->SelectObject(&hPen);

	POINT ptArrow[7];
	if( !bDown )
	{
		if( bType == 0 )//倒三角
		{
			ptArrow[0].x = rect.left;		ptArrow[0].y = rect.top + 2;
			ptArrow[1].x = rect.left + 8;	ptArrow[1].y = rect.top + 2;
			ptArrow[2].x = rect.left + 4;	ptArrow[2].y = rect.top + 10;

			pDC->Polyline(ptArrow, 3);

			rgn1.CreatePolygonRgn(ptArrow,  3,  ALTERNATE);
			pDC->FillRgn(&rgn1, &CBrush(lColor));
			rgn1.DeleteObject();
		}
		else if( bType == 1 )
		{
			ptArrow[0].x = 8;	ptArrow[0].y = 0;
			ptArrow[1].x = 8;	ptArrow[1].y = 4;
			ptArrow[2].x = 11;	ptArrow[2].y = 4;
			ptArrow[3].x = 6;	ptArrow[3].y = 9;
			ptArrow[4].x = 1;	ptArrow[4].y = 4;
			ptArrow[5].x = 4;	ptArrow[5].y = 4;
			ptArrow[6].x = 4;	ptArrow[6].y = 0;

			rgn1.CreatePolygonRgn(ptArrow,  7,  ALTERNATE);
			rgn1.OffsetRgn(rect.TopLeft());
			pDC->FillRgn(&rgn1, &CBrush(lColor));
			rgn1.DeleteObject();
		}
		else if( bType == 2 )
		{
			ptArrow[0].x = rect.left;		ptArrow[0].y = rect.top;
			ptArrow[1].x = rect.left + 4;	ptArrow[1].y = rect.top + 4;
			ptArrow[2].x = rect.left;	    ptArrow[2].y = rect.top + 8;

			pDC->Polyline(ptArrow, 3);

			rgn1.CreatePolygonRgn(ptArrow,  3,  ALTERNATE);
			pDC->FillRgn(&rgn1, &CBrush(lColor));
			rgn1.DeleteObject();
		}
		else if( bType == 3 ) // 排名
		{
			//ptArrow[0].x = rect.left;		ptArrow[0].y = rect.top + 10;
			//ptArrow[1].x = rect.left + 3;	ptArrow[1].y = rect.top + 13;
			//ptArrow[2].x = rect.left + 6;	ptArrow[2].y = rect.top + 10;
			//ptArrow[3].x = rect.left + 3;	ptArrow[3].y = rect.top;

			//pDC->MoveTo(ptArrow[0]);
			//pDC->LineTo(ptArrow[1]);

			//pDC->MoveTo(ptArrow[1]);
			//pDC->LineTo(ptArrow[2]);

			//pDC->MoveTo(ptArrow[1]);
			//pDC->LineTo(ptArrow[3]);

			ptArrow[0].x = rect.left; ptArrow[0].y = rect.top+6; 
			ptArrow[1].x = rect.left + 6;	ptArrow[1].y = rect.top + 12;
			ptArrow[2].x = rect.left + 12;	ptArrow[2].y = rect.top + 6;
			ptArrow[3].x = rect.left + 9;	ptArrow[3].y = rect.top + 6;
			ptArrow[4].x = rect.left + 9;	ptArrow[4].y = rect.top;
			ptArrow[5].x = rect.left + 3;	ptArrow[5].y = rect.top;
			ptArrow[6].x = rect.left + 3;	ptArrow[6].y = rect.top + 6;
			rgn1.CreatePolygonRgn(ptArrow,  7,  ALTERNATE);
			pDC->FillRgn(&rgn1, &CBrush(lColor));
			rgn1.DeleteObject();
		}		
	}
	else
	{
		if( bType == 0 )//正三角
		{   			
			ptArrow[0].x = rect.left ;		ptArrow[0].y = rect.top + 8;
			ptArrow[1].x = rect.left + 8;	ptArrow[1].y = rect.top + 8 ;
			ptArrow[2].x = rect.left + 4;	ptArrow[2].y = rect.top ;

			pDC->Polyline(ptArrow, 3);

			rgn1.CreatePolygonRgn(ptArrow,  3,  ALTERNATE);
			pDC->FillRgn(&rgn1, &CBrush(lColor));
			rgn1.DeleteObject();
		}
		else if( bType == 1 )
		{
			ptArrow[0].x = 4;	ptArrow[0].y = iHeight;
			ptArrow[1].x = 4;	ptArrow[1].y = iHeight-4;
			ptArrow[2].x = 0;	ptArrow[2].y = iHeight-4;
			ptArrow[3].x = 6;	ptArrow[3].y = iHeight-10;
			ptArrow[4].x = 12;	ptArrow[4].y = iHeight-4;
			ptArrow[5].x = 8;	ptArrow[5].y = iHeight-4;
			ptArrow[6].x = 8;	ptArrow[6].y = iHeight;

			rgn1.CreatePolygonRgn(ptArrow,  7,  ALTERNATE);
			rgn1.OffsetRgn(rect.TopLeft());		
			pDC->FillRgn(&rgn1, &CBrush(lColor));		
			rgn1.DeleteObject();
		}
		else if( bType == 2 )
		{
			ptArrow[0].x = rect.left;		ptArrow[0].y = rect.top;
			ptArrow[1].x = rect.left + 4;	ptArrow[1].y = rect.top + 4;
			ptArrow[2].x = rect.left;	    ptArrow[2].y = rect.top + 8;

			pDC->Polyline(ptArrow, 3);

			rgn1.CreatePolygonRgn(ptArrow,  3,  ALTERNATE);
			pDC->FillRgn(&rgn1, &CBrush(lColor));
			rgn1.DeleteObject();
		}
		else if( bType == 3 ) // 排名
		{
			//ptArrow[0].x = rect.left;		ptArrow[0].y = rect.top + 3;
			//ptArrow[1].x = rect.left + 3;	ptArrow[1].y = rect.top;
			//ptArrow[2].x = rect.left + 6;	ptArrow[2].y = rect.top + 3;
			//ptArrow[3].x = rect.left + 3;	ptArrow[3].y = rect.top + 13;

			//pDC->MoveTo(ptArrow[0]);
			//pDC->LineTo(ptArrow[1]);

			//pDC->MoveTo(ptArrow[1]);
			//pDC->LineTo(ptArrow[2]);

			//pDC->MoveTo(ptArrow[1]);
			//pDC->LineTo(ptArrow[3]);
			ptArrow[0].x = rect.left; ptArrow[0].y = rect.top+6; 
			ptArrow[1].x = rect.left + 3;	ptArrow[1].y = rect.top + 6;
			ptArrow[2].x = rect.left + 3;	ptArrow[2].y = rect.top + 12;
			ptArrow[3].x = rect.left + 9;	ptArrow[3].y = rect.top + 12;
			ptArrow[4].x = rect.left + 9;	ptArrow[4].y = rect.top + 6;
			ptArrow[5].x = rect.left + 12;	ptArrow[5].y = rect.top+6;
			ptArrow[6].x = rect.left + 6;	ptArrow[6].y = rect.top;
			rgn1.CreatePolygonRgn(ptArrow,  7,  ALTERNATE);
			pDC->FillRgn(&rgn1, &CBrush(lColor));
			rgn1.DeleteObject();
		}
		else if(bType == 4)//实心方块
		{  
			//20030920 modify by fangly:不划实心方块 以下被注释
			/*
 			ptArrow[0].x = rect.left + 2;		ptArrow[0].y = rect.top + 2;
			ptArrow[1].x = rect.left + 2;		ptArrow[1].y = rect.top + 8;
			ptArrow[2].x = rect.left + 8;	ptArrow[2].y = rect.top + 8;
			ptArrow[3].x = rect.left + 8;	ptArrow[3].y = rect.top + 1;
 
			pDC->Polyline(ptArrow, 4);

			rgn1.CreatePolygonRgn(ptArrow,  4,  ALTERNATE);
			pDC->FillRgn(&rgn1, &CBrush(lColor));
			rgn1.DeleteObject();
			*/
		}
	}

	pDC->SelectObject(hOldPen);
}

void CGeneralHelper::GetListImage(CImageList& ImageList, CBitmap& bitmap, int nImageIndex)
{
	CClientDC dcClient(NULL);

	IMAGEINFO ImageInfo;
	ImageList.GetImageInfo(nImageIndex, &ImageInfo);

	int nWidth  = abs(ImageInfo.rcImage.right - ImageInfo.rcImage.left);
	int nHeight = abs(ImageInfo.rcImage.bottom - ImageInfo.rcImage.top);

	bitmap.CreateCompatibleBitmap(&dcClient, nWidth, nHeight);

	CDC dcMemory;
	dcMemory.CreateCompatibleDC((CDC*)NULL) ;

	CBitmap* pBmpOld = dcMemory.SelectObject(&bitmap);
	dcMemory.FillSolidRect(0, 0, nWidth, nHeight, RGB(255,255,255));

	ImageList.Draw(&dcMemory, nImageIndex, CPoint(0,0), ILD_NORMAL);
	dcMemory.SelectObject(pBmpOld);
	dcMemory.DeleteDC();
	::DeleteObject( ImageInfo.hbmImage );
	::DeleteObject( ImageInfo.hbmMask );
}

void CGeneralHelper::ScaleBitmap(CBitmap *pBitmap,CBitmap &BitmapNew, int nWidth,int nHeight)
{
	CDC  dcSrc,dcDst ;
	int      nWidthOld, nHeightOld ;
	int      nWidthNew, nHeightNew ;

	BITMAP    pBitMap ;
	pBitmap->GetBitmap(&pBitMap) ;

	nWidthOld       = pBitMap.bmWidth ;
	nHeightOld   = pBitMap.bmHeight ;
	nWidthNew  = nWidth ;
	nHeightNew = nHeight ;

	// Create DC
	dcSrc.CreateCompatibleDC((CDC*)NULL);
	dcDst.CreateCompatibleDC((CDC*)NULL);

	// Source Bitmap
	dcSrc.SelectObject(pBitmap);

	// New Bitmap
	BitmapNew.CreateCompatibleBitmap(&dcSrc,nWidthNew,nHeightNew);

	// Scale Bitmap
	dcDst.SelectObject(&BitmapNew);
	//Maps pixels from the source rectangle into blocks of pixels 
	//in the destination rectangle. The average color over 
	//the destination block of pixels approximates the color of the source pixels.
	dcDst.SetStretchBltMode(HALFTONE) ;
	dcDst.StretchBlt(0, 0, nWidthNew, nHeightNew, &dcSrc, 0, 0, nWidthOld, nHeightOld, SRCCOPY);

	// Free Resource
	dcSrc.DeleteDC() ;
	dcDst.DeleteDC() ;
}

BOOL CGeneralHelper::ReadConfigSectionEntry(const char* strSection, CStringArray& strArray,const char* strFileName /* = NULL */)
{
	strArray.RemoveAll();

	if(strFileName == NULL)
		return NULL;
	if(_access(strFileName, 0) != 0)
	{
		return FALSE;
	}
	struct _stat buf;
	memset(&buf, 0, sizeof(buf));
	_stat(strFileName, &buf);
	int nSize = (int)buf.st_size;
	if(nSize <= 0)
	{
		return FALSE;
	}
	char* strBuf = new char[nSize+1];
	memset(strBuf,0,nSize+1);

	::GetPrivateProfileString(strSection,NULL,"",strBuf,nSize,strFileName);
	char* p = strBuf;

	int nLen;
	int nTotal = 0;

	while(*p != '\0')
	{
		strArray.Add(p);
		nLen = strlen(p) + 1;

		nTotal += nLen;
		if( nTotal >= nSize )
			break;

		p += nLen;
	}  
	delete[] strBuf;

	return TRUE;

}

CString GetProfileText(LPCTSTR lpAppName,        // section name
						  LPCTSTR lpKeyName,        // key name
						  LPCTSTR lpDefault,        // default string
						  LPCTSTR lpFileName)       // initialization file name
{
	CString str;
	int nLen = 128;
	int nRes;
	do
	{
		nLen *= 2;
		str.GetBufferSetLength(nLen);
		::GetPrivateProfileString(lpAppName,lpKeyName,lpDefault,str.GetBuffer(),nLen,lpFileName);
		nRes = lstrlen(str);
	} while (nRes == nLen-1);
	str.ReleaseBuffer();

	return str;
}

BOOL CGeneralHelper::ReadConfigString(const char* strSection,const char* strEntry,
												CString& strItem,const char* strFileName)
{
	if(strFileName == NULL)
		return NULL;;
	if(_access(strFileName, 0) != 0)
	{
		return FALSE;
	}

// 	char strBuf[128];   
// 	::GetPrivateProfileString(strSection,strEntry,"",strBuf,127,strFileName);
// 	if(strBuf[0] == '\0')
// 	   return FALSE;
// 	strItem = strBuf;

	strItem = GetProfileText(strSection,strEntry,"",strFileName);
	if( strItem.IsEmpty() )
		return FALSE;

	return TRUE;
}

BOOL CGeneralHelper::ReadConfigInt(const char* strSection,const char* strEntry,
											 int& nValue,const char* strFileName)
{
	if(strFileName == NULL)
		return NULL;;
	if(_access(strFileName, 0) != 0)
	{
		return FALSE;
	}

	nValue = ::GetPrivateProfileInt(strSection,strEntry, INT_MAX, strFileName);
	if( nValue == INT_MAX )
	{
		nValue = 0;
		return FALSE;
	}

	return TRUE;
}

CString CGeneralHelper::ConvertChinaOfWeek(int lDate)
{
	CString strRet;
	switch(lDate)
	{
	case 1:
		strRet.Format(_T("日"));
		break;
	case 2:
		strRet.Format(_T("一"));
		break;
	case 3:
		strRet.Format(_T("二"));
		break;
	case 4:
		strRet.Format(_T("三"));
		break;
	case 5:
		strRet.Format(_T("四"));
		break;
	case 6:
		strRet.Format(_T("五"));
		break;
	case 7:
		strRet.Format(_T("六"));
		break;
	}
	return strRet;
}

CTime CGeneralHelper::CompToAbs(long lDisc, long lRefDate)
{
	CTime curTime;

	if( lRefDate == 0 )
	{
		curTime = CTime::GetCurrentTime();
	}
	else
	{
		curTime = curTime.CTime::CTime(lRefDate/10000, lRefDate%10000/100, lRefDate%100, 0, 0, 0);
	}

	if( lDisc < 0 ) lDisc = 0 - lDisc;
	CTimeSpan delOneDay(1, 0, 0, 0);
	int nWeek = curTime.GetDayOfWeek();

	if( lDisc == 0 )
	{
		while( nWeek <= 1 || nWeek >= 7 )
		{
			curTime -= delOneDay;
			nWeek = curTime.GetDayOfWeek();
		}
	}
	else
	{
		while( lDisc > 0 )
		{
			curTime -= delOneDay;
			nWeek = curTime.GetDayOfWeek();
			if( nWeek > 1 && nWeek < 7 )
				lDisc--;
			if (nWeek <= 0)
			{
				break;
			}
		}
	}

	return curTime;
	//lDisc = curTime.GetYear() * 10000 + curTime.GetMonth() * 100 + curTime.GetDay();
	//return lDisc;
}

CTime CGeneralHelper::LongToDate(long lDate, long lRefDate)
{
	try
	{
		lDate %= 100000000;
		lRefDate %= 100000000;
		// 2003-2-21 llx
		if( lDate <= 0 )
		{
			lDate = 0 - lDate;
			return CGeneralHelper::CompToAbs(lDate, lRefDate);
		}


		int nYear = int(lDate/10000);
		int nMonth, nDay, nHour, nMinute;
		if(nYear < 1970)
		{
			CTime curTime = CTime::GetCurrentTime();

			nMonth  = nYear/100;
			nDay    = nYear%100;
			nHour   = (int)((lDate%10000)/100);
			nMinute = (int)(lDate%100);
			nYear = curTime.GetYear();
		}
		else if(nYear < 2038)
		{
			nMonth  = (int)((lDate%10000)/100);
			nDay    = (int)(lDate%100);
			nHour   = 0;
			nMinute = 0;
			//if(nYear > 2038)  // ??
			//	nYear = 2038;
		}
		else if(nYear >= 2038)
		{
			nYear = 2038 - 1;
			nMonth  = nYear/100;
			nDay    = nYear%100;
			nHour   = (int)((lDate%10000)/100);
			nMinute = (int)(lDate%100);
		}

		if(nDay < 1 || nDay > 31)
			nDay = 1;
		if(nMonth < 1 || nMonth > 12)
			nMonth = 1;
		if(nYear == 1970 && nMonth == 1 && nDay < 8 && nHour == 0)//这个时间CTime会报错
			nHour = 8;
		return CTime(nYear,nMonth,nDay,nHour,nMinute,0);
	}
	catch(...){}
	return 0;
}

CString CGeneralHelper::GetWeekDayString(unsigned long lDate)  //2010.11.09 litao增加判断是周几的函数
{
	CString strWeek="";
	int nWeek;
	if (lDate < 100000000)  //为日期
	{
		CTime ctDate = CGeneralHelper::LongToDate(lDate);
		nWeek = ctDate.GetDayOfWeek();
	}
	else //为分钟
	{
		long ltmpDate = 0;
		ltmpDate = lDate / 10000;
		long lYear = ltmpDate / 10000 - 10 + 2000 ;
		ltmpDate = lYear * 10000 + ltmpDate % 10000;
		CTime ctDate = CGeneralHelper::LongToDate(ltmpDate);
		nWeek = ctDate.GetDayOfWeek();
	}

	switch (nWeek)
	{
	case 1:
		strWeek = "日";
		break;
	case 2:
		strWeek = "一";
		break;
	case 3:
		strWeek = "二";
		break;
	case 4:
		strWeek = "三";
		break;
	case 5:
		strWeek = "四";
		break;
	case 6:
		strWeek = "五";
		break;
	case 7:
		strWeek = "六";
		break;
	}
	return strWeek;
}
