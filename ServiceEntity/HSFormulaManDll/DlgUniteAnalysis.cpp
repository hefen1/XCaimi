///////////////////////////////////////////////////////////////////////////////////////////
// 文件名:			DlgUniteAnalysis.cpp
// 创建者：				
// 创建时间：		2002-3-6
// 内容描述：		"联动分析"功能主模块
///////////////////////////////////////////////////////////////////////////////
// DlgUniteAnalysis.cpp : implementation file
//

#include "stdafx.h"
#include "DlgUniteAnalysis.h"
#include	"winbase.h"
#include "Express.h"
#include "GeneralHelper.h"
#include "hsothercoldefine.h"
extern CMapStringToPtr* g_pTableHeadData;
extern HWND			 g_hParentWnd;
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
//给List控件用来sort的加调FUN
int CALLBACK UniteAnalyseCompareFunc(LPARAM lParam1, LPARAM lParam2,LPARAM lParamSort)
{
		ListSortParam	*pParam;	
		DisplayLineData *pLine1, *pLine2;
		CString			strTmp;
		int			iSort;
		int			nReturn = 0;

		pParam = (ListSortParam *)lParamSort;

		iSort = pParam->nCurSel;

		pLine1 = pParam->pListData[lParam1];
		pLine2 = pParam->pListData[lParam2];
		strTmp = pLine1->m_ayStrData.GetAt(iSort);
		if( pParam->bSortFlag == FALSE )
		{
			nReturn = 0 - strTmp.Compare( pLine2->m_ayStrData.GetAt(iSort) );
		}
		else
		{
			nReturn = strTmp.Compare( pLine2->m_ayStrData.GetAt(iSort) );
		}

		if( nReturn == TRUE )
		{
			iSort = pLine1->m_nCurIndex;
			pLine1->m_nCurIndex = pLine2->m_nCurIndex;
			pLine2->m_nCurIndex = iSort;
		}
		return nReturn;
} 

CUniteLineData::CUniteLineData(StockUserInfo* StockA/* = NULL*/, 
		StockUserInfo* StockB/* = NULL*/, 
		StockUserInfo* StockH/* = NULL*/
		)
{
	m_pStockA = StockA;
	m_pStockB = StockB;
	m_pStockH = StockH;
}

CUniteLineData::~CUniteLineData()
{
	Free();
}

void CUniteLineData::Free()
{
	if(m_pStockA != NULL)
		m_pStockA = NULL;
	if(m_pStockB != NULL)
		m_pStockB = NULL;
	if(m_pStockH != NULL)
		m_pStockH = NULL;
}
void CUniteLineData::Save(CFile *pFile, int nKeyIndex)
{
	CString strTemp;
	strTemp.Format("[%d]\nA-Type=%c\nA-Code=%s\nB-Type=%c\nB-Code=%s\nH-Type=%c\nH-Code=%s\n", 
		nKeyIndex, 
		m_pStockA->m_ciStockCode.m_cCodeType,
		m_pStockA->m_ciStockCode.m_cCode,
		m_pStockB->m_ciStockCode.m_cCodeType,
		m_pStockB->m_ciStockCode.m_cCode,
		m_pStockH->m_ciStockCode.m_cCodeType,
		m_pStockH->m_ciStockCode.m_cCode
		);
	pFile->Write(strTemp, strTemp.GetLength());
}

BOOL CUniteLineData::Read(CString strFileName, CString strKeyIndex)
{
	CString strCode;
	CString strType;
	CodeInfo stock;
	BOOL bFlag = FALSE;

	if( _access(strFileName, 0) < 0 )
	{
		AfxMessageBox(_T("文件") + strFileName + _T("不存在!"));
		//::MessageBox(AfxGetMainWnd()->m_hWnd, (_T("文件") + strFileName + _T("不存在!")), _T("提示..."),MB_OK);
		return FALSE;
	}
	char test[100];
	memset(test, 0, 100);
	::GetPrivateProfileString(strKeyIndex, "A-Type", "", test, 99, strFileName);
	strType.Format("%s", test);
	memset(test, 0, 100);
	long l = ::GetPrivateProfileString(strKeyIndex, "A-Code", "", test, 99, strFileName);
	strCode.Format("%s", test);

	if(strType.IsEmpty() || strCode.IsEmpty())
	{
		if(m_pStockA != NULL)
			m_pStockA = NULL;
	}
	else
	{
		strCode.TrimLeft();
		strType.TrimLeft();
		strncpy( stock.m_cCode, (LPCSTR)strCode, sizeof(stock.m_cCode) );
		stock.m_cCodeType = strType.GetAt(0);
		m_pStockA = CodeInfoToStockUserInfo(&stock);
		strCode.Empty();
		strType.Empty();
		bFlag = TRUE;
	}

	memset(test, 0, 100);
	l = ::GetPrivateProfileString(strKeyIndex, "B-Type", "", test, 99, strFileName);
	strType.Format("%s", test);
	memset(test, 0, 100);
	::GetPrivateProfileString(strKeyIndex, "B-Code", "", test, 99, strFileName);
	strCode.Format("%s", test);
	if(strType.IsEmpty() || strCode.IsEmpty())
	{
		if(m_pStockB != NULL)
			m_pStockB = NULL;
	}
	else
	{
		strCode.TrimLeft();
		strType.TrimLeft();
		strncpy( stock.m_cCode, (LPCSTR)strCode, sizeof(stock.m_cCode) );
		stock.m_cCodeType = strType.GetAt(0);
		m_pStockB = CodeInfoToStockUserInfo(&stock);
		strCode.Empty();
		strType.Empty();
		bFlag = TRUE;
	}

	memset(test, 0, 100);
	::GetPrivateProfileString(strKeyIndex, "H-Type", "", test, 100, strFileName);
	strType.Format("%s", test);
	memset(test, 0, 100);
	::GetPrivateProfileString(strKeyIndex, "H-Code", "", test, 100, strFileName);
	strCode.Format("%s", test);
	if(strType.IsEmpty() || strCode.IsEmpty())
	{
		if(m_pStockH != NULL)
			m_pStockH = NULL;
	}
	else
	{
		strCode.TrimLeft();
		strType.TrimLeft();
		strncpy( stock.m_cCode, (LPCSTR)strCode, sizeof(stock.m_cCode) );
		stock.m_cCodeType = strType.GetAt(0);
		m_pStockH = CodeInfoToStockUserInfo(&stock);
		strCode.Empty();
		strType.Empty();
		bFlag = TRUE;
	}
	return bFlag;
}

StockUserInfo* CUniteLineData::CodeInfoToStockUserInfo(CodeInfo * pCodeInfo)
{
	return (StockUserInfo *)::SendMessage(g_hParentWnd, HX_USER_COMPILEDATA, 19, (LPARAM)pCodeInfo);
}


//CDlgUniteAnalysis dialog
IMPLEMENT_DYNAMIC(CDlgUniteAnalysis, CDialog)
CDlgUniteAnalysis::CDlgUniteAnalysis(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgUniteAnalysis::IDD, pParent)
{
	m_nCurCol = 0;
	m_bCurSortNum = -1;
	m_bCurSortFlag = FALSE;
	m_strStockIniFileName =  CGeneralHelper::GetSpecifyPath(CGeneralHelper::SETTING_PATH)  + _T("UniteAnalyseStock.ini");
	m_strColIniFileName	=  CGeneralHelper::GetSpecifyPath(CGeneralHelper::SETTING_PATH)  + _T("UniteAnalyseColName.def");
	Create(IDD, pParent);
}

CDlgUniteAnalysis::~CDlgUniteAnalysis()
{
}

void CDlgUniteAnalysis::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST, m_ctrlList);
}


BEGIN_MESSAGE_MAP(CDlgUniteAnalysis, CDialog)
	ON_COMMAND_RANGE(COLUMN_LINKAGE_BEGIN, COLUMN_LINKAGE_END, OnMenuMsg)
	ON_REGISTERED_MESSAGE(HX_USER_COMPILEDATA, OnMsgFun)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST, OnLvnColumnclickList)
	ON_WM_SIZE()
//	ON_NOTIFY(HDN_ITEMDBLCLICK, 0邐乏, OnHdnItemdblclickList)
ON_NOTIFY(NM_DBLCLK, IDC_LIST, OnNMDblclkList)
END_MESSAGE_MAP()



// CDlgUniteAnalysis message handlers

BOOL CDlgUniteAnalysis::OnInitDialog()
{
	CDialog::OnInitDialog();

	DWORD	dStyle;
	dStyle = m_ctrlList.GetExtendedStyle();
	dStyle |=  LVS_EX_FULLROWSELECT;
	dStyle |=  LVS_EX_FLATSB;
	m_ctrlList.SetExtendedStyle(dStyle);

	m_ctrlList.AddHeader(1);
	ReadStockIni();
	ReadColIni();
	ReDrawHandCtrl();
	ComputeAll();
	ReDraw();

	return TRUE;  // return TRUE unless you set the focus to a control
}

void CDlgUniteAnalysis::SaveStockIni()		//存取股票配置
{
	CUniteLineData*	pLine;
	CFile			file;

	file.Open(m_strStockIniFileName, CFile::modeCreate|CFile::modeWrite);
	file.SetLength(0);
	for(int i = 0; i < m_ayTabData.GetSize(); i++ )
	{
		pLine = m_ayTabData.GetAt(i);
		pLine->Save(&file, i);
	}
}

void CDlgUniteAnalysis::ReadStockIni()
{
	CUniteLineData*	pLine;
	CString			strTemp;
	BOOL			bReturn = TRUE;
	int				i = 0;

	for( int j = 0; j < m_ayTabData.GetSize(); j++ )
	{
		delete m_ayTabData.GetAt(j);
	}
	m_ayTabData.RemoveAll();

	while( bReturn == TRUE )
	{
		pLine = new CUniteLineData;
		strTemp.Format("%d", i++);
		bReturn = pLine->Read(m_strStockIniFileName, strTemp);
		if( bReturn == FALSE )
		{
			delete pLine;
		}
		else
		{
			m_ayTabData.Add(pLine);
		}
	}
}

void CDlgUniteAnalysis::SaveColIni()		//存取列设置
{
	CFile file;
	file.Open(m_strColIniFileName, CFile::modeWrite|CFile::modeCreate);
	CArchive ar(&file, CArchive::store);
	UniteColData* pCol;
	ar<<m_ayColData.GetSize();

	for(int i = 0; i < m_ayColData.GetSize(); i++ )
	{
		pCol = m_ayColData.GetAt(i);
		pCol->Serialize(ar);
	}
	ar.Close();
	file.Close();
}

void CDlgUniteAnalysis::ReadColIni()
{
	CFile file;
	file.Open(m_strColIniFileName, CFile::modeRead);
	if( file.GetLength() <= 0 )
	{
		file.Close();
		return;
	}
	CArchive ar(&file, CArchive::load);

	UniteColData* pCol;
	int	nEnd;

	ar>>nEnd;
	int i;
	for(  i = 0; i < m_ayColData.GetSize(); i++ )
	{
		delete m_ayColData.GetAt(i);
	}
	m_ayColData.RemoveAll();
	
	for(i = 0; i < nEnd; i++ )
	{
		pCol = new UniteColData;
		m_ayColData.Add(pCol);
		pCol->Serialize(ar);
	}
	ar.Close();
	file.Close();
	for( i = 0; i < m_ayDisplayData.GetSize(); i++ )
	{
		delete m_ayDisplayData.GetAt(i);
	}
	m_ayDisplayData.RemoveAll();

	DisplayLineData* pPlay;
	for(i = 0; i < m_ayTabData.GetSize(); i++ )
	{
		pPlay = new DisplayLineData;
		pPlay->m_pUniteLineData = m_ayTabData.GetAt(i);
		pPlay->m_ayStrData.SetSize(nEnd);
		pPlay->m_nCurIndex = i;
		m_ayDisplayData.Add(pPlay);
	}
}

void CDlgUniteAnalysis::DrawCurCol(int nCol)
{
	LV_ITEM				lvitem;
	char				chTemp[256];
	DisplayLineData*	pLine;

	for(int i = 0; i < m_ayDisplayData.GetSize(); i++ )
	{
		pLine = m_ayDisplayData.GetAt(i);
		if( nCol == 0)
		{
			lvitem.mask = LVIF_TEXT|LVIF_IMAGE|LVIF_PARAM;
		}
		else 
		{
			lvitem.mask = LVIF_TEXT;
		}
		lvitem.iItem = i;
		lvitem.lParam = i;
		lvitem.iSubItem = nCol;
		strcpy( chTemp, pLine->m_ayStrData.GetAt(nCol) );
		lvitem.pszText = chTemp;
		m_ctrlList.SetItem(&lvitem);
	}
}
void CDlgUniteAnalysis::ReDraw()			//重新画数据
{
	LV_ITEM				lvitem;
	char				chTemp[256];
	DisplayLineData*	pLine;
	int					iPos;

	m_ctrlList.DeleteAllItems();

	for(int i = 0; i < m_ayDisplayData.GetSize(); i++ )
	{
		pLine = m_ayDisplayData.GetAt(i);
		lvitem.mask = LVIF_TEXT|LVIF_IMAGE|LVIF_PARAM;
		lvitem.iItem = i;
		lvitem.lParam = i;
		lvitem.iSubItem = 0;
		strcpy( chTemp, pLine->m_ayStrData.GetAt(0) );
		lvitem.pszText = chTemp;
		iPos = m_ctrlList.InsertItem(&lvitem);//返回表项插入后的索引号

		lvitem.mask = LVIF_TEXT;
		for( int j = 1; j < pLine->m_ayStrData.GetSize(); j++ )
		{
			lvitem.iSubItem = j;
			strcpy( chTemp, pLine->m_ayStrData.GetAt(j) );
			lvitem.pszText = chTemp;
			iPos = m_ctrlList.SetItem(&lvitem);
		}
	}
}

//重新计算nCol列
void CDlgUniteAnalysis::ReCompute(int nCol)	
{
	CUniteLineData*		pLineStock;
	UniteColData*		pCol = m_ayColData.GetAt(nCol);
	int					nFlag = pCol->nID;
	CString				strStockCode;
	double				dReturn;
	DisplayLineData*	pPlayLine;

	//对股分类,A = 0, B = 1, H = 2, Other = 3
	nFlag = IdToType(nFlag);

	//如果为A\B\H股代码， 则....
	if( pCol->nID == AU_CODE_ID || 
		pCol->nID == BU_CODE_ID || 
		pCol->nID == HU_CODE_ID 
		)
	{
		for( int i = 0; i < m_ayDisplayData.GetSize(); i++ )
		{
			strStockCode.Empty();
			pLineStock = m_ayTabData.GetAt(i);
			switch( nFlag )
			{
			case 0:
				if( pLineStock->m_pStockA != NULL )
					strStockCode.Format("%s", pLineStock->m_pStockA->m_ciStockCode.GetCode());
				break;
			case 1:
				if( pLineStock->m_pStockB != NULL )
					strStockCode.Format("%s", pLineStock->m_pStockB->m_ciStockCode.GetCode());
				break;
			case 2:
				if( pLineStock->m_pStockH != NULL )
					strStockCode.Format("%s", pLineStock->m_pStockH->m_ciStockCode.GetCode());
				break;
			default:
				break;
			}
			pPlayLine = m_ayDisplayData.GetAt(i);
			pPlayLine->m_ayStrData.SetAt(nCol, strStockCode);
		}
	}
	//如果为A\B\H股名称， 则....
	else if ( pCol->nID == AU_NAME_ID || 
			  pCol->nID == BU_NAME_ID ||
			  pCol->nID == HU_NAME_ID
			 )
	{
		for( int i = 0; i < m_ayDisplayData.GetSize(); i++ )
		{
			strStockCode.Empty();
			pLineStock = m_ayTabData.GetAt(i);
			switch( nFlag )
			{
			case 0:
				if( pLineStock->m_pStockA != NULL )
					strStockCode.Format("%s", CString(pLineStock->m_pStockA->m_cStockName,sizeof(pLineStock->m_pStockA->m_cStockName)));
				break;
			case 1:
				if( pLineStock->m_pStockB != NULL )
					strStockCode.Format("%s", CString(pLineStock->m_pStockB->m_cStockName, sizeof(pLineStock->m_pStockB->m_cStockName)));
				break;
			case 2:
				if( pLineStock->m_pStockH != NULL )
					strStockCode.Format("%s", CString(pLineStock->m_pStockH->m_cStockName, sizeof(pLineStock->m_pStockH->m_cStockName)));
				break;
			default:
				break;
			}
			pPlayLine = m_ayDisplayData.GetAt(i);
			pPlayLine->m_ayStrData.SetAt(nCol, strStockCode);
		}
	}
	else if( pCol->nID == ABU_PRICE_ID )
	{
		for(int i = 0; i < m_ayDisplayData.GetSize(); i++ )
		{
			pLineStock = m_ayTabData.GetAt(i);
			if( pLineStock->m_pStockA != NULL )
				strStockCode.Format("%s", pLineStock->m_pStockA->m_ciStockCode.GetCode());
			pPlayLine = m_ayDisplayData.GetAt(i);
			if( strStockCode.IsEmpty() )
			{
				pPlayLine->m_ayStrData.SetAt(nCol, strStockCode);
			}
			else
			{
				dReturn = Compute(strStockCode, AU_LATEST_ID);
				if( dReturn * dReturn < 0.00001 )
					strStockCode = "0.00";
				else
				{
					if( pLineStock->m_pStockB == NULL )
					{
						strStockCode.Empty();
					}
					else
					{
						strStockCode.Format("%s", pLineStock->m_pStockB->m_ciStockCode.GetCode());
						double dTemp = Compute(strStockCode, BU_LATEST_ID);
						if( dTemp * dTemp < 0.00001 )
						{
							strStockCode.Empty();
						}
						else
						{
							strStockCode.Format("%.2f", dReturn / dTemp);
						}
					}
				}
				pPlayLine->m_ayStrData.SetAt(nCol, strStockCode);
			}
		}
	}
	else if( pCol->nID == AHU_PRICE_ID )
	{
		for(int i = 0; i < m_ayDisplayData.GetSize(); i++ )
		{
			pLineStock = m_ayTabData.GetAt(i);
			if( pLineStock->m_pStockA != NULL )
				strStockCode.Format("%s", pLineStock->m_pStockA->m_ciStockCode.GetCode());
			pPlayLine = m_ayDisplayData.GetAt(i);
			if( strStockCode.IsEmpty() )
			{
				pPlayLine->m_ayStrData.SetAt(nCol, strStockCode);
			}
			else
			{
				dReturn = Compute(strStockCode, AU_LATEST_ID);
				if( dReturn * dReturn < 0.00001 )
					strStockCode = "0.00";
				else
				{
					if( pLineStock->m_pStockH == NULL )
					{
						strStockCode.Empty();
					}
					else
					{
						strStockCode.Format("%s", pLineStock->m_pStockH->m_ciStockCode.GetCode());
						double dTemp = Compute(strStockCode, HU_LATEST_ID);
						if( dTemp * dTemp < 0.00001 )
						{
							strStockCode.Empty();
						}
						else
						{
							strStockCode.Format("%.2f", dReturn / dTemp);
						}
					}
				}
				pPlayLine->m_ayStrData.SetAt(nCol, strStockCode);
			}
		}
	}
	else if( pCol->nID > AHU_PRICE_ID && pCol->nID < DELETE_ID )
	{
		for(int i = 0; i < m_ayDisplayData.GetSize(); i++ )
		{
			pLineStock = m_ayTabData.GetAt(i);
			if( pLineStock->m_pStockA != NULL )
				strStockCode.Format("%s", pLineStock->m_pStockA->m_ciStockCode.GetCode());
			pPlayLine = m_ayDisplayData.GetAt(i);
			if( strStockCode.IsEmpty() )
			{
				pPlayLine->m_ayStrData.SetAt(nCol, strStockCode);
			}
			else
			{
				dReturn = Compute(strStockCode, pCol->nID);
				if( dReturn * dReturn < 0.00001 )
					strStockCode = "0.00";
				else
				{
					strStockCode.Format("%.2f", dReturn);
				}
				pPlayLine->m_ayStrData.SetAt(nCol, strStockCode);
			}
		}

	}
	//如果为其它的ID,则....
	else
	{
		for(int i = 0; i < m_ayDisplayData.GetSize(); i++ )
		{
			pLineStock = m_ayTabData.GetAt(i);
			switch( nFlag )
			{
			case  0:
				{
				if( pLineStock->m_pStockA != NULL )
					strStockCode.Format("%s", pLineStock->m_pStockA->m_ciStockCode.GetCode());
				}
				break;
			case  1:
				{
				if( pLineStock->m_pStockB != NULL )
					strStockCode.Format("%s", pLineStock->m_pStockB->m_ciStockCode.GetCode());
				}
				break;
			case  2:
				{
				if( pLineStock->m_pStockH != NULL )
					strStockCode.Format("%s", pLineStock->m_pStockH->m_ciStockCode.GetCode());
				}
				break;
			default:
				{
				if( pLineStock->m_pStockA != NULL )
					strStockCode.Format("%s", pLineStock->m_pStockA->m_ciStockCode.GetCode());
				}
				break;
			}
			pPlayLine = m_ayDisplayData.GetAt(i);
			if( strStockCode.IsEmpty() )
			{
				pPlayLine->m_ayStrData.SetAt(nCol, strStockCode);
			}
			else
			{
				dReturn = Compute(strStockCode, pCol->nID);
				if( dReturn * dReturn < 0.00001 )
					strStockCode = "0.00";
				else
					strStockCode.Format("%2.f", dReturn);
				pPlayLine->m_ayStrData.SetAt(nCol, strStockCode);
			}
		}
	}
}

void CDlgUniteAnalysis::ComputeAll()
{
	for(int i = 0; i < m_ayColData.GetSize(); i++ )
	{
		ReCompute(i);
	}
}

double	CDlgUniteAnalysis::Compute(LPCSTR strCode, DWORD dwID)
{
	return 1.0 * dwID;
}


void CDlgUniteAnalysis::ReDrawHandCtrl()
{
	LV_COLUMN	lvcol;
	UniteColData* pCol;
	char		chTemp[256];

	lvcol.mask=LVCF_FMT|LVCF_SUBITEM|LVCF_TEXT|LVCF_WIDTH;
	lvcol.fmt=LVCFMT_CENTER;//居中
	m_ctrlList.DeleteAllItems();
	while(m_ctrlList.DeleteColumn(0) != 0);

	for(int i = 0; i < m_ayColData.GetSize(); i ++)
	{
		pCol = m_ayColData.GetAt(i);
		memset(chTemp, 0, sizeof(char) * 256);
		strcpy(chTemp, pCol->strColName);
		lvcol.pszText = chTemp;
		lvcol.iSubItem = i;
		lvcol.cx = 70;
		m_ctrlList.InsertColumn(i,&lvcol);
	}
}

void CDlgUniteAnalysis::OnCancel()
{
	SaveColIni();
	//SaveStockIni();
	int i, j;
	for( i = 0; i < m_ayDisplayData.GetSize(); i++ )
	{
		delete m_ayDisplayData.GetAt(i);
	}
	m_ayDisplayData.RemoveAll();

	for(  j = 0; j < m_ayTabData.GetSize(); j++ )
	{
		delete m_ayTabData.GetAt(j);
	}
	m_ayTabData.RemoveAll();

	for( i = 0; i < m_ayColData.GetSize(); i++ )
	{
		delete m_ayColData.GetAt(i);
	}
	m_ayColData.RemoveAll();
	if( ::IsWindow(m_hWnd) )
	{
		DestroyWindow();
	}
}


void CDlgUniteAnalysis::OnMenuMsg(UINT nID)
{
	if( nID == DELETE_ID )
	{
		if( m_nCurCol < m_ayColData.GetSize() )
			DelColAt(m_nCurCol);
		return;
	}
	UniteColData *pCol;
	pCol = new UniteColData;
	pCol->nID = nID;
	pCol->strColName.Format("%s", GetNameByKey(nID));

	if( m_nCurCol < m_ayColData.GetSize() )
	{
		ChageCol(pCol);
	}
	else
	{
		AppendCol(pCol);
	}
}


void CDlgUniteAnalysis::DelColAt(int nColNum)
{
	DisplayLineData *pPlay;

	delete m_ayColData.GetAt(nColNum);
	m_ayColData.RemoveAt(nColNum);

	for( int i = 0; i < m_ayDisplayData.GetSize(); i++ )
	{
		pPlay = m_ayDisplayData.GetAt(i);
		pPlay->m_ayStrData.RemoveAt(nColNum);
	}
	ReDrawHandCtrl();
	ReDraw();
}

void CDlgUniteAnalysis::AppendCol(UniteColData * pUniteCOl)
{
	DisplayLineData *pPlay;
	m_ayColData.Add(pUniteCOl);
	for( int i = 0; i < m_ayDisplayData.GetSize(); i++ )
	{
		pPlay = m_ayDisplayData.GetAt(i);
		pPlay->m_ayStrData.Add("0");
	}
	ReCompute(m_ayColData.GetSize() - 1);
	ReDrawHandCtrl();
	ReDraw();
}

void CDlgUniteAnalysis::ChageCol(UniteColData * pUniteCOl, int nCol)
{
	if( nCol == -1 ) 
		nCol = m_nCurCol;
	delete m_ayColData.GetAt(nCol);
	m_ayColData.RemoveAt(nCol);
	m_ayColData.InsertAt(nCol, pUniteCOl);
	ReCompute(nCol);
	ReDrawHandCtrl();
	ReDraw();
}


CString CDlgUniteAnalysis::GetNameByKey(UINT nKey) 
{
	CString strFindKeyName;
	strFindKeyName.Format("%d", nKey);

	CString strRet;
	if( g_pTableHeadData )
	{
		void* pValue;
		CMapWordToPtr* pMap;

		CString strName = LOCK_NAME;
		if( g_pTableHeadData->Lookup(strName,pValue) )
		{
			pMap = (CMapWordToPtr*)pValue;
			if( !pMap->IsEmpty() )
			{
				if( pMap->Lookup(atol(strFindKeyName),pValue))
				{
					strRet.Format("%s",((TableColProperty*)pValue)->strName );
				}
			}
		}
	}
	return strRet;
}

long CDlgUniteAnalysis::OnMsgFun(WPARAM wp,LPARAM lp)
{
	if( wp == 10 )
	{
		m_nCurCol = (int)lp;
	}
	return 0;
}

void CDlgUniteAnalysis::OnLvnColumnclickList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	
	static ListSortParam strSortData;
	strSortData.nCurSel = pNMLV->iSubItem;
	if( m_bCurSortNum == pNMLV->iSubItem )
	{
		m_bCurSortFlag = (m_bCurSortFlag == FALSE) ? TRUE : FALSE;
	}
	else
	{
		m_bCurSortNum = pNMLV->iSubItem;
		m_bCurSortFlag = FALSE;
	}
	strSortData.bSortFlag = m_bCurSortFlag;
	strSortData.pListData = m_ayDisplayData.GetData();
	m_ctrlList.SortItems((PFNLVCOMPARE)UniteAnalyseCompareFunc, (LPARAM)&strSortData);
	//DisplayLineData*	pPlay;
	//int					nCurIndex;
	//for( int i = 0; i < m_ayDisplayData.GetSize(); i++ )
	//{
	//	pPlay = m_ayDisplayData.GetAt(i);
	//	if( pPlay->m_nCurIndex == i )
	//		continue;
	//	nCurIndex = pPlay->m_nCurIndex;
	//	pPlay->CaseData( (DisplayLineData*)m_ayDisplayData.GetAt(nCurIndex) );
	//}
	*pResult = 0;
}

void CDlgUniteAnalysis::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	if( !m_ctrlList.GetSafeHwnd() )
		return;
	CRect rect;
	this->GetClientRect(&rect);
	m_ctrlList.SetWindowPos(0, 0, 0, rect.Width(), rect.Height(), SWP_NOZORDER);
}

void CDlgUniteAnalysis::PostNcDestroy()
{
	delete this;
	//WRITE_LOG ::PostNcDestroy();
}

void CDlgUniteAnalysis::OnNMDblclkList(NMHDR *pNMHDR, LRESULT *pResult)
{
	NM_LISTVIEW* pListView = (NM_LISTVIEW*)pNMHDR;
	*pResult = 0;

	pListView->iItem;
	pListView->iSubItem;
	CString		strCode;
	/*int			j;*/
	/*CUniteLineData* pLine;*/
	DisplayLineData* pPlay;
	int i;
	for(  i = 0; i < m_ayDisplayData.GetSize(); i++ )
	{
		pPlay = m_ayDisplayData.GetAt(i);
		if( pPlay->m_nCurIndex == pListView->iItem )
			break;
	}
	
	if( i >= m_ayDisplayData.GetSize() )
		return;

	UniteColData* pCol = m_ayColData.GetAt(pListView->iSubItem);

	//nFlag = IdToType(pCol->nID);   //得到类别

	//StockUserInfo* pStock;
	//switch( nFlag )
	//{
	//case 0:
	//	pStock = pPlay->m_pUniteLineData->m_pStockA;
	//	break;
	//case 1:
	//	pStock = pPlay->m_pUniteLineData->m_pStockB;
	//	break;
	//case 2:
	//	pStock = pPlay->m_pUniteLineData->m_pStockH;
	//	break;
	//default:
	//	pStock = pPlay->m_pUniteLineData->m_pStockA;
	//	break;
	//}
	//if( pStock == NULL )
	//	return;

	//HSCurKeyInfo keyInfo;
	//keyInfo.Copy(&pStock->m_ciStockCode);
	//::SendMessage(g_hParentWnd,HX_USER_DRAGDATA,
	//						HX_USER_DRAGDATA,(LPARAM)&keyInfo);

	HSCurKeyInfo keyInfo;

	if( pPlay->m_pUniteLineData->m_pStockA != NULL )
	{
		keyInfo.Copy(&pPlay->m_pUniteLineData->m_pStockA->m_ciStockCode);
		::SendMessage(g_hParentWnd,HX_USER_DRAGDATA,
			HX_USER_DRAGDATA,(LPARAM)&keyInfo);
		if( pPlay->m_pUniteLineData->m_pStockH != NULL )
		{
			::SendMessage(g_hParentWnd, HX_USER_COMPILEDATA, 20, (LPARAM)pPlay->m_pUniteLineData->m_pStockH);
		}
		if( pPlay->m_pUniteLineData->m_pStockB != NULL )
		{
			::SendMessage(g_hParentWnd, HX_USER_COMPILEDATA, 20, (LPARAM)pPlay->m_pUniteLineData->m_pStockB);
		}
	
	}
	else if( pPlay->m_pUniteLineData->m_pStockB != NULL )
	{
		keyInfo.Copy(&pPlay->m_pUniteLineData->m_pStockB->m_ciStockCode);
		::SendMessage(g_hParentWnd,HX_USER_DRAGDATA,
			HX_USER_DRAGDATA,(LPARAM)&keyInfo);
		if( pPlay->m_pUniteLineData->m_pStockH != NULL )
		{
			::SendMessage(g_hParentWnd, HX_USER_COMPILEDATA, 20, (LPARAM)pPlay->m_pUniteLineData->m_pStockH);
		}

	}
	else if( pPlay->m_pUniteLineData->m_pStockH != NULL )
	{
		keyInfo.Copy(&pPlay->m_pUniteLineData->m_pStockH->m_ciStockCode);
		::SendMessage(g_hParentWnd,HX_USER_DRAGDATA,
			HX_USER_DRAGDATA,(LPARAM)&keyInfo);
	}
}

//对股分类,A = 0, B = 1, H = 2, Other = 3
int CDlgUniteAnalysis::IdToType(int nID)
{
	int nFlag = nID;

	if( nFlag >= AU_CODE_ID && nFlag < BU_CODE_ID )
	{
		nFlag = 0;
	}
	else if( nFlag >= BU_CODE_ID && nFlag < HU_CODE_ID )
	{
		nFlag = 1;
	}
	else if( nFlag >= HU_CODE_ID && nFlag < ABU_PRICE_ID )
	{
		nFlag = 2;
	}
	else
	{
		nFlag = 3;
	}
	return nFlag;
}
