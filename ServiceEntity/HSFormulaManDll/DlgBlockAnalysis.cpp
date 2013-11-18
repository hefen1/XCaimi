///////////////////////////////////////////////////////////////////////////////////////////
// 文件名:			DlgBlockAnalysis.cpp
// 创建者：				
// 创建时间：		2002-3-6
// 内容描述：		"板块分析"功能主模块
///////////////////////////////////////////////////////////////////////////////
// DlgBlockAnalysis.cpp : implementation file
//

#include "stdafx.h"
#include "DlgAddProjectToBlock.h"
#include "DlgBlockAnalysis.h"
#include "DlgBlockPicAnalysis.h"
#include "GeneralHelper.h"
#include "ExpPublic.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgBlockAnalysis dialog

#define		COL_0NAME	_T("板块名")
#define		COL_1NAME	_T("股票数")
#define		ALL_BLOCK	_T("全部板块")

extern HWND			 g_hParentWnd;
HSCurKeyInfo m_strData;

//BYTE CDlgBlockAnalysis::m_bRunFlag = 0;

int CALLBACK CompareFunc(LPARAM lParam1, LPARAM lParam2,LPARAM lParamSort)
{
		PARAMVAL	*pp;	
		int			iFlag;
		int			iSort;

		pp = (PARAMVAL *)lParamSort;

		iFlag = pp->iDownOrUp;
		iSort = pp->iCurCol;

		if(iSort == 0)
			return (iFlag * (pp->pBlock[lParam1]->strLineName.Compare(pp->pBlock[lParam2]->strLineName)));
		else
		{
			if(((double)pp->pBlock[lParam1]->ayLine.GetAt(iSort - 1)) > ((double)pp->pBlock[lParam2]->ayLine.GetAt(iSort - 1)))
				return iFlag;
			else if(((double)pp->pBlock[lParam1]->ayLine.GetAt(iSort - 1)) == ((double)pp->pBlock[lParam2]->ayLine.GetAt(iSort -1)))
				return 0;
			else return -1 * iFlag;
		}
		return 0;
} 



CDlgBlockAnalysis::CDlgBlockAnalysis(CWnd* pParent /*=NULL*/, HSOpenExpressUseData* pData)
	: CDialog(CDlgBlockAnalysis::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgBlockAnalysis)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pData = pData;
	LISTCOLHANDNAME		listColName;
	m_Form = pParent;
	m_AryListColHandName.SetSize(2);
	listColName.iCx = 70;
	listColName.strColName = COL_0NAME;
	m_AryListColHandName.SetAt(0, listColName);
	listColName.iCx = 70;
	listColName.strColName = COL_1NAME;
	m_AryListColHandName.SetAt(1, listColName);		//选初始设置CListCtrl中的前两列: 板块名 股票数

	m_strColHandFileName =  CGeneralHelper::GetSpecifyPath(CGeneralHelper::SETTING_PATH)  + "blockanalyse.def";

	m_HandColName = new CStringArray;
	
	//m_bRunFlag = 1;

	Create( IDD, pParent );
}


void CDlgBlockAnalysis::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgBlockAnalysis)
	DDX_Control(pDX, IDC_BEGIN_ANALYSE, m_begin);
	DDX_Control(pDX, IDC_ADD_PROJECT, m_add);
	DDX_Control(pDX, IDC_TAB, m_Tab_Ctrl);
	DDX_Control(pDX, IDC_LIST, m_List_Ctrl);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgBlockAnalysis, CDialog)

	//{{AFX_MSG_MAP(CDlgBlockAnalysis)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB, OnSelchangeTab)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST, OnColumnclickList)
	ON_BN_CLICKED(IDC_ADD_PROJECT, OnAddProject)
	ON_BN_CLICKED(IDC_BEGIN_ANALYSE, OnBeginAnalyse)
	ON_COMMAND(ID_PROJECTADD, OnProjectadd)
	ON_COMMAND(ID_PROJECTDEL, OnProjectdel)
	ON_COMMAND(ID_PROJECTCHAGE, OnProjectchage)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST, OnDblclkList)
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_PAINT()
	//}}AFX_MSG_MAP

	ON_REGISTERED_MESSAGE(HX_USER_COMPILEDATA, GetBlockStockEventPress)
//	ON_REGISTERED_MESSAGE(CExpression::HS_HANDLER_COL, OnHandCol)

ON_BN_CLICKED(IDC_SAVEAS, OnBnClickedSaveas)
ON_BN_CLICKED(IDC_SAVEAS2, OnBnClickedSaveas2)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgBlockAnalysis message handlers

BOOL CDlgBlockAnalysis::OnInitDialog() 
{
	CDialog::OnInitDialog();

	try
	{
		SetSize();
		//::SendMessage(g_hParentWnd, HX_USER_COMPILEDATA, 22, (LPARAM)this);

		SendGetBlockStockEvent();			//发送得到板块信息的信号, 得到所有板块的信息,也就是得到了m_TypeAndBlock[0]的值

		ReadListColHandName();				//读入表头

		DWORD	dStyle;
		dStyle = m_List_Ctrl.GetExtendedStyle();
		dStyle |=  LVS_EX_FULLROWSELECT;
		dStyle |=  LVS_EX_FLATSB;
		m_List_Ctrl.SetExtendedStyle(dStyle);

		ReDrowTab();
		ReDrowListColHandName();
		ReDrowListData(0);
		m_List_Ctrl.AddHeader(0);
		SetHSBlockInfoColName();			//设置计算时用的共享公式列表

	}
	catch(...)
	{
		//AfxMessageBox("Error!");
		MessageBox(_T("Error!"), _T("提示..."),MB_ICONINFORMATION);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

int	CDlgBlockAnalysis::SaveListColHandName()
{
	CFile	fp;
	if(fp.Open(m_strColHandFileName, CFile::modeWrite|CFile::modeCreate, NULL))
	{
		fp.SetLength(0);

		CString	strWrite;
		for(int i = 2; i < m_AryListColHandName.GetSize(); i ++)
		{
			CString strTemp;
			strTemp.Format("[%s]\n;%s\n%d-%d\n", m_AryListColHandName[i].strColName, m_AryListColHandName[i].strDesc, m_AryListColHandName[i].iFormat, m_AryListColHandName[i].iType);
			strWrite += strTemp;
		}
		fp.Write(strWrite, strWrite.GetLength());
	}
	fp.Close();
	return 0;
}

int	CDlgBlockAnalysis::ReadListColHandName()
{
	CFile fp;
	LISTCOLHANDNAME	listTemp;
	int				iCurListNum = 0;
	CString strTemp;

	if(fp.Open(m_strColHandFileName, CFile::modeRead, NULL))
	{
		char	*chTemp;

		chTemp = new char[(int)fp.GetLength() + 1];
		fp.Read(chTemp, (UINT)fp.GetLength());
		chTemp[fp.GetLength()] = 0;
		fp.Close();

		char	*pNext;
		char	chKey[256];

		pNext = chTemp;

		while(pNext != NULL && pNext[1] != 0)
		{

			sscanf(pNext, "%s\n", chKey);
			strTemp.Format("%s", chKey);
			strTemp.TrimLeft();
			if(strTemp.GetAt(0) == '[' && strTemp.GetAt(strTemp.GetLength() - 1) == ']')
			{
				strTemp.Delete(0);
				strTemp.Delete(strTemp.GetLength() - 1);
				listTemp.strColName = strTemp;
				iCurListNum = m_AryListColHandName.Add(listTemp);
			}
			else if(strTemp.GetAt(0) >= '0' && strTemp.GetAt(0) <= '9')
			{
				int	iFormat, iType;

				sscanf(chKey, "%d-%d", &iFormat, &iType);
				((LISTCOLHANDNAME *)m_AryListColHandName.GetData())[iCurListNum].iFormat = iFormat;
				((LISTCOLHANDNAME *)m_AryListColHandName.GetData())[iCurListNum].iType = iType;
				((LISTCOLHANDNAME *)m_AryListColHandName.GetData())[iCurListNum].iCx = 70;
			}
			else if(strTemp.GetAt(0) == ';')
			{
				strTemp.Delete(0);
				((LISTCOLHANDNAME *)m_AryListColHandName.GetData())[iCurListNum].strDesc = strTemp;
			}
			pNext = strstr(pNext, "\n");
			while(pNext != NULL && (*pNext == '\n' || *pNext == '\r')) 
				pNext ++;
		}
		delete[] chTemp;
		return 0;
	}
	else
	{
		//AfxMessageBox("打开文件" + fp.GetFilePath() + "错误!");
		return -1;
	}
}


int	CDlgBlockAnalysis::AddTab(CString strTabName)				
{
	m_Tab_Ctrl.InsertItem(m_Tab_Ctrl.GetItemCount(), strTabName);
	return 0;
}


int	CDlgBlockAnalysis::RemoveListColHand(int iIndex)		
{
	if( iIndex >= m_AryListColHandName.GetSize() )
		return -1;

	m_AryListColHandName.RemoveAt(iIndex);
	ReDrowListColHandName();
	ListDrowData	**pList = m_ayDrowData.GetData();
	for(int i = 0; i < m_ayDrowData.GetSize(); i++)
	{
		pList[i]->iDrowNum = 2;
	}
	ReDrowListData(m_Tab_Ctrl.GetCurSel());
	SetHSBlockInfoColName();
	return 0;
}


int	CDlgBlockAnalysis::AddListColHand(CString strExpressName, int iType, int iFormat, CString strDesc)	
{
	
	LISTCOLHANDNAME listTemp;

	listTemp.strColName = strExpressName;
	listTemp.iType = iType;
	listTemp.iFormat = iFormat;
	listTemp.strDesc = strDesc;
	m_AryListColHandName.Add(listTemp);
	ReDrowListColHandName();
	ListDrowData	**pList = m_ayDrowData.GetData();
	for(int i = 0; i < m_ayDrowData.GetSize(); i++)
	{
		pList[i]->iDrowNum = 2;
	}
	ReDrowListData(m_Tab_Ctrl.GetCurSel());	
	SetHSBlockInfoColName();
	return 0;
}


int	CDlgBlockAnalysis::SetColIniFileName(CString strFileName)
{
	m_strColHandFileName = strFileName;
	return 0;
}

int	CDlgBlockAnalysis::ReDrowListColHandName()				
{
	LV_COLUMN	lvcol;
	int			i;
	char		chTemp[256];
	LISTCOLHANDNAME  *pListName = (LISTCOLHANDNAME *)m_AryListColHandName.GetData();

	lvcol.mask=LVCF_FMT|LVCF_SUBITEM|LVCF_TEXT|LVCF_WIDTH;
	lvcol.fmt=LVCFMT_CENTER;//居中

	m_List_Ctrl.DeleteAllItems();
	while(m_List_Ctrl.DeleteColumn(0) != 0);

	for(i = 0; i < m_AryListColHandName.GetSize(); i ++)
	{
		memset(chTemp, 0, sizeof(char) * 256);
		strcpy(chTemp, pListName[i].strColName);
		lvcol.pszText = chTemp;
		lvcol.iSubItem = i;
		lvcol.cx = pListName[i].iCx;
		m_List_Ctrl.InsertColumn(i,&lvcol);
		lvcol.fmt=LVCFMT_RIGHT;
	}
	return 0;
}


int	CDlgBlockAnalysis::ReDrowTab()							
{
	if( m_TypeAndBlock == NULL )
		return -1;
	TYPEANDBLOCK	**pType = (TYPEANDBLOCK **)m_TypeAndBlock->m_ayBlockCalc.GetData();

	for(int i = 0; i < m_TypeAndBlock->m_ayBlockCalc.GetSize(); i++)
	{
		AddTab(pType[i]->strTypeName);
	}
	m_Tab_Ctrl.SetCurFocus(0);
	m_Tab_Ctrl.SetCurSel(0);
	ReDrowListData(0);	
	return 0;
}


int	CDlgBlockAnalysis::ReDrowListData(int iCurTabSel)						
{
	ListDrowData		**pListData = (ListDrowData **)m_ayDrowData.GetData();
	LineDrowData		**pLine = (LineDrowData **)pListData[iCurTabSel]->ayList.GetData();
	LISTCOLHANDNAME		*pListColName = (LISTCOLHANDNAME *)m_AryListColHandName.GetData();

	int				iEnd = pListData[iCurTabSel]->ayList.GetSize();
	int				iDrowCol = pListData[iCurTabSel]->iDrowNum;
	int				iPos;
	LV_ITEM			lvitem;
	char			chTemp[256];
	int				i,j;

	m_List_Ctrl.DeleteAllItems();

	for(i = 0; i < iEnd; i++)
	{
		lvitem.mask = LVIF_TEXT|LVIF_IMAGE|LVIF_PARAM;
		lvitem.iItem = i;
		lvitem.iSubItem = 0;
		strcpy(chTemp, pLine[i]->strLineName);
		lvitem.pszText = chTemp;
		lvitem.lParam = i;
		iPos = m_List_Ctrl.InsertItem(&lvitem);//返回表项插入后的索引号

		lvitem.mask = LVIF_TEXT;
		lvitem.iItem = iPos;
		lvitem.iSubItem = 1;
		j = (int)pLine[i]->ayLine.GetAt(0);
		if(j == 0)
			strcpy(chTemp, "0");
		else
			sprintf(chTemp, "%d", j);
		lvitem.pszText = chTemp;
		iPos = m_List_Ctrl.SetItem(&lvitem);

		for(j = 2; j < iDrowCol; j ++)
		{
			lvitem.iSubItem = j;
			memset(chTemp, 0, sizeof(char) * 256);
			switch(pListColName[j].iFormat)
			{
			case 0:
				if((double)pLine[i]->ayLine.GetAt(j - 1) == 0)
					strcpy(chTemp, "0");
				else
					sprintf(chTemp, "%d", (int)pLine[i]->ayLine.GetAt(j - 1));
				break;
			case 1:
				if((double)pLine[i]->ayLine.GetAt(j - 1) == 0)
					strcpy(chTemp, "0.00");
				else
					sprintf(chTemp, "%.2f", (double)pLine[i]->ayLine.GetAt(j - 1));
				break;	
			case 2:
				if((double)pLine[i]->ayLine.GetAt(j - 1) == 0)
					strcpy(chTemp, "0.000");
				else
					sprintf(chTemp, "%.3f", (double)pLine[i]->ayLine.GetAt(j - 1));
				break;	
			case 3:
				if((double)pLine[i]->ayLine.GetAt(j - 1) == 0)
					strcpy(chTemp, "0");
				else
					sprintf(chTemp, "%e", (double)pLine[i]->ayLine.GetAt(j - 1));
				break;	
			case 4:
				if((double)pLine[i]->ayLine.GetAt(j - 1) == 0)
					strcpy(chTemp, "0");
				else
					sprintf(chTemp, "%.3f", (double)pLine[i]->ayLine.GetAt(j - 1));
				break;		
			default:								
				if((double)pLine[i]->ayLine.GetAt(j - 1) == 0)
					strcpy(chTemp, "0");
				else
					sprintf(chTemp, "%.2f", (double)pLine[i]->ayLine.GetAt(j - 1));
				break;
			}
			lvitem.pszText = chTemp;
			iPos = m_List_Ctrl.SetItem(&lvitem);
		}
	}

  return 0;
}


int	CDlgBlockAnalysis::ComputeListData(int iCurTab)					
{
	
	TYPEANDBLOCK	*pType = m_TypeAndBlock->m_ayBlockCalc.GetAt(iCurTab);
	CArray<BlockInfo*, BlockInfo*>	*pBlock = &pType->ayBlock;
	BlockInfo	*pCurBlock;

	ListDrowData		*pListData = m_ayDrowData.GetAt(iCurTab);
	CArray<LineDrowData*, LineDrowData*>	*pLine = &pListData->ayList;
	LineDrowData	*pCurLine;

	CArray<double, double>	ayDouble;

	CArray<CodeInfo*, CodeInfo*>	*payStock;
	CArray<StockUserInfo*, StockUserInfo*>  *payCalStock;
	CodeInfo							*pStock;
	CString			strStockCode;

	int				iIend = pType->ayBlock.GetSize();		//相当在LIST表明表中有几行
	int				iJend = m_AryListColHandName.GetSize();				//相当于在LIST中有几列
	int				iZend;												//当前行的股票数
	int				f, iFend = pType->ayCalcStock.GetSize();	//当前TAB的所有股票数
	double			iCheckNum = 0;										//流通盘总数
	int				j;

	pListData->iDrowNum = iJend;
	ayDouble.SetSize(iJend - 1);

	for(int i = 0; i < iIend; i++)			//列表中的行数，也就是当前类的板块数
	{
		pCurLine = pLine->GetAt(i);
		pCurLine->ayLine.SetSize(iJend - 1);

		for(j = 0; j< iJend - 1; j++)
		{
			ayDouble.SetAt(j, 0);
			pCurLine->ayLine.SetAt(j, 0);
		}

		pCurBlock = pBlock->GetAt(i);
		iZend = pCurBlock->ayStockList.GetSize();
		pCurLine->ayLine.SetAt(0, iZend);				//股票数

		payStock = &(pCurBlock->ayStockList);
		payCalStock = &(pType->ayCalcStock);

		iCheckNum = 0;
		for(int z = 0; z < iZend; z ++)		//每一行中，股票的数量
		{
			pStock = payStock->GetAt(z);
			strStockCode.Format("%s", pStock->m_cCode);

			//for(f = 0; f < iFend; f ++)		//得到当前股票在当前TAB中的索引f
			//{
			//	pCalStock = payCalStock->GetAt(f);
			//	if(strStockCode.Compare(pCalStock->m_ciStockCode.GetCode()) == 0)
			//		break;
			//}
			TabStockSortData* pIndex = m_ayStockIndex.GetAt(iCurTab);
			f = GetStockIndex(pIndex, strStockCode);

			if(f == -1)					//如果没有
				continue;

			BlockCalcRet *pCalRet = pType->ayCalcRet.GetAt(f);
			BlockOtherData *pOtherRet = pType->ayOtherData.GetAt(f);

			if( pCalRet->ayValues.GetSize() <= 0 )
				continue;
			for(j = 2; j < iJend; j++)	//对每支股票进行list的列数次计算
			{
				LISTCOLHANDNAME pHandCol = m_AryListColHandName.GetAt(j);

				if(pHandCol.iType == 1)	//如果的加权平均	
					ayDouble[j - 1] += (double)(pCalRet->ayValues.GetAt(j -2)) 
											  * (double)(pOtherRet->m_dCapital);
				else																	//否则全部累加
					ayDouble[j - 1] += (double)(pCalRet->ayValues.GetAt(j - 2));
			}
			iCheckNum += pOtherRet->m_dCapital;
		}
		if(iZend == 0)
			continue;
		for(j = 2; j < iJend; j++)
		{
			LISTCOLHANDNAME pHandCol = m_AryListColHandName.GetAt(j);

			if(pHandCol.iType == 0)
				ayDouble[j - 1] /= iZend;
			else if(pHandCol.iType == 1)
				ayDouble[j - 1] /= iCheckNum;
			pCurLine->ayLine.SetAt(j - 1, ayDouble[j - 1]);
		}
		int iTransmited = 100 + i * 100 / iIend;
		::SendMessage(g_hParentWnd, HX_USER_COMPILEDATA, 13, (LPARAM)iTransmited);
	}
	::SendMessage(g_hParentWnd, HX_USER_COMPILEDATA, 14, 0);
	return 0;
}


int	CDlgBlockAnalysis::SetHSBlockInfoColName()
{
	LISTCOLHANDNAME *pList = m_AryListColHandName.GetData();

	m_HandColName->RemoveAll();
	for(int i = 2; i < m_AryListColHandName.GetSize(); i++)
	{
		m_HandColName->Add(pList[i].strDesc);
	}

	BlockTypeInfo* pHSBlockCalcInfo;
	for(int j = 0; j < m_TypeAndBlock->m_ayBlockCalc.GetSize(); j++)
	{
		pHSBlockCalcInfo = m_TypeAndBlock->m_ayBlockCalc.GetAt(j);
		pHSBlockCalcInfo->m_pColName = (void *)m_HandColName;
	}
	return 0;
}


//发送得到板块股票的信息

int	CDlgBlockAnalysis::SendGetBlockStockEvent()
{
	//need to modify
	::SendMessage(g_hParentWnd, HX_USER_COMPILEDATA, 10, (LPARAM)0);
	return 0;
}


//信号处理;

long CDlgBlockAnalysis::GetBlockStockEventPress(WPARAM wp,LPARAM lp)
{
	if(wp == 0)		//得到初始页--也就是第0页的所有股票
	{
		m_TypeAndBlock = (BlockTabInitData *)lp;

		CArray<TYPEANDBLOCK*, TYPEANDBLOCK*>	*payType = &m_TypeAndBlock->m_ayBlockCalc;
		TYPEANDBLOCK	*pType;
		CArray<BlockInfo*, BlockInfo*>	*payBlock;
		BlockInfo	*pBlock;
		ListDrowData	*pList;
		LineDrowData	*pLine;
		int				iEnd = payType->GetSize();
		int				jEnd;
		int				iColLen = m_AryListColHandName.GetSize();

		pList = new ListDrowData;
		pList->iDrowNum = 2;
		pType = payType->GetAt(0);

		jEnd = pType->ayBlock.GetSize();
		payBlock = &pType->ayBlock;

		if( m_bClickFlag.GetSize() > 0 )
			m_bClickFlag.RemoveAll();
		TabStockSortData* pIndex;
		for(int i = 0; i < iEnd; i++)
		{
			m_bClickFlag.Add(FALSE);
			pIndex = new TabStockSortData;
			m_ayStockIndex.Add(pIndex);
		}
		for(int j = 0; j < jEnd; j++)
		{
			pBlock = payBlock->GetAt(j);
			pLine = new LineDrowData;
			pLine->strLineName = pBlock->strBlockName;
			pLine->ayLine.SetSize(iColLen - 1);
			pLine->ayLine.SetAt(0, pBlock->ayStockList.GetSize());
			pList->ayList.Add(pLine);
		}
		m_ayDrowData.Add(pList);
		for( int i = 1; i < iEnd; i++ )
		{
			pList = new ListDrowData;
			m_ayDrowData.Add(pList);
		}
		EnableCtrl(TRUE);
		return 0;
	}
	else if( wp == 1 )		//本地统计开始
	{
		int	iCurTab = (int)lp;
		ComputeListData(iCurTab);
		ReDrowListData(iCurTab);
		EnableCtrl(TRUE);
	}
	else if( wp == 2 )		//返回当前Tab的所有板块的股票
	{
		int nCurTab = (int)lp;

		CArray<TYPEANDBLOCK*, TYPEANDBLOCK*>	*payType = &m_TypeAndBlock->m_ayBlockCalc;
		TYPEANDBLOCK	*pType;
		CArray<BlockInfo*, BlockInfo*>	*payBlock;
		BlockInfo	*pBlock;
		ListDrowData	*pList;
		LineDrowData	*pLine;
		int				jEnd;
		int				iColLen = m_AryListColHandName.GetSize();

		pList = m_ayDrowData.GetAt(nCurTab);
		pList->iDrowNum = 2;
		pType = payType->GetAt(nCurTab);

		jEnd = pType->ayBlock.GetSize();
		payBlock = &pType->ayBlock;

		for(int j = 0; j < jEnd; j++)
		{
			pBlock = payBlock->GetAt(j);
			pLine = new LineDrowData;
			pLine->strLineName = pBlock->strBlockName;
			pLine->ayLine.SetSize(iColLen - 1);
			pLine->ayLine.SetAt(0, pBlock->ayStockList.GetSize());
			pList->ayList.Add(pLine);
		}
		m_ayDrowData.SetAt(nCurTab, pList);
		//EnableCtrl(TRUE);
		return 0;
	}
	else if( wp == 3 )		//中止运行时,使窗体及按钮有效
	{
		if( !m_begin.IsWindowEnabled() )
			EnableCtrl(TRUE);
	}
	else if( wp == 10 )			//从handcol得到当前列
	{
		m_iCurHandlerNum = (int)lp;
	}
	return 0;
}

void CDlgBlockAnalysis::OnSelchangeTab(NMHDR* pNMHDR, LRESULT* pResult) 
{
	int		iCurSel;
	iCurSel = m_Tab_Ctrl.GetCurSel();
	if( m_bClickFlag[iCurSel] == FALSE && iCurSel != 0)
	{
		m_bClickFlag[iCurSel] = TRUE;
		::SendMessage(g_hParentWnd, HX_USER_COMPILEDATA, 10, (LPARAM)iCurSel);
	}
	ReDrowListData(iCurSel);
	//if( !m_begin.IsWindowEnabled() )
	//{
	//	EnableCtrl(TRUE);
	//}
	*pResult = 0;
}

void CDlgBlockAnalysis::OnColumnclickList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	int iSorted=-1;//排列序号
	
	if (pNMListView->iSubItem==iSorted) return;

	iSorted=pNMListView->iSubItem;
	static PARAMVAL	pp;
	pp.iCurCol = iSorted;

	if(iSorted == 0)
		pp.iDownOrUp = 1;
	else
		pp.iDownOrUp = -1;

	ListDrowData **ppd = (ListDrowData **)m_ayDrowData.GetData();
	pp.pBlock = (LineDrowData **)ppd[m_Tab_Ctrl.GetCurSel()]->ayList.GetData();
	if(ppd[m_Tab_Ctrl.GetCurSel()]->iDrowNum <= iSorted)
	{
		return;
	}

	m_List_Ctrl.SortItems((PFNLVCOMPARE)CompareFunc, (LPARAM)&pp);
	*pResult = 0;
}

void CDlgBlockAnalysis::OnAddProject() 
{
	
	CDlgAddProjectToBlock dlg(this/*m_Form*/);

	this->EnableWindow(FALSE);
	if(dlg.DoModal() == IDOK)
	{
		LISTCOLHANDNAME		listTemp;

		listTemp.iCx = 70;
		listTemp.iFormat = dlg.m_iCurFormat;
		listTemp.iType = dlg.m_iCurType;
		listTemp.strColName = dlg.m_strCurName;
		listTemp.strDesc = dlg.m_strCurDesc;

		m_AryListColHandName.Add(listTemp);
		ReDrowListColHandName();
		ListDrowData *pList;
		LineDrowData *pLine;

		ListDrowData **pType = (ListDrowData **)m_ayDrowData.GetData();

		for(int i = 0; i < m_ayDrowData.GetSize(); i++)
		{
			pList = m_ayDrowData.GetAt(i);
			pList->iDrowNum = 2;

			for(int j = 0; j < pList->ayList.GetSize(); j ++)
			{
				pLine = pList->ayList.GetAt(j);
				pLine->ayLine.SetSize(m_AryListColHandName.GetSize());
			}
		}
		ReDrowListData(m_Tab_Ctrl.GetCurSel());
		SetHSBlockInfoColName();
	}	
	this->EnableWindow();
}


void CDlgBlockAnalysis::OnBeginAnalyse() 
{
	int			iCurTab = m_Tab_Ctrl.GetCurSel();
	TYPEANDBLOCK *pBlock = m_TypeAndBlock->m_ayBlockCalc.GetAt(iCurTab);

//	this->EnableWindow(FALSE);
	if( m_AryListColHandName.GetSize() <= 2 )
	{
		//AfxMessageBox(_T("请增加项目(列)再开始计算!"));
		MessageBox(_T("请增加项目(列)再开始计算!"), _T("提示..."),MB_ICONINFORMATION);
		return;
	}
	if( pBlock->ayBlock.GetSize() <= 0 )
	{
		return;
	}
	EnableCtrl(FALSE);
	::SendMessage(g_hParentWnd, HX_USER_COMPILEDATA, 11, (LPARAM)iCurTab);
	CArray<StockUserInfo*,StockUserInfo*>*	payStock = &pBlock->ayCalcStock;
	TabStockSortData*						pIndex = m_ayStockIndex.GetAt(iCurTab);
	BuildStockIndex(payStock, pIndex);
	//EnableCtrl(TRUE);
}


void CDlgBlockAnalysis::OnProjectadd() 
{
	OnAddProject();
}

void CDlgBlockAnalysis::OnProjectdel() 
{
	CString strTemp;
	
	strTemp.Format(_T("确实要删除第%d列:\"%s\"吗？"), m_iCurHandlerNum, m_AryListColHandName[m_iCurHandlerNum].strColName);
	if( MessageBox(strTemp, _T("提示..."), MB_YESNO|MB_ICONINFORMATION) == IDYES)
		RemoveListColHand(m_iCurHandlerNum);
}


void CDlgBlockAnalysis::OnProjectchage() 
{
	if( m_iCurHandlerNum >= m_AryListColHandName.GetSize() )
		return;
	CDlgAddProjectToBlock dlg(this/*m_Form*/, m_AryListColHandName[m_iCurHandlerNum].iFormat, m_AryListColHandName[m_iCurHandlerNum].iType, m_AryListColHandName[m_iCurHandlerNum].strColName, m_AryListColHandName[m_iCurHandlerNum].strDesc, _T("修改") + m_AryListColHandName[m_iCurHandlerNum].strColName + _T("项目"));
	if(dlg.DoModal() == IDOK)
	{
		m_AryListColHandName[m_iCurHandlerNum].iFormat = dlg.m_iCurFormat;
		m_AryListColHandName[m_iCurHandlerNum].iType = dlg.m_iCurType;
		m_AryListColHandName[m_iCurHandlerNum].strColName = dlg.m_strCurName;
		m_AryListColHandName[m_iCurHandlerNum].strDesc = dlg.m_strCurDesc;
		ReDrowListColHandName();

		ListDrowData **pType = (ListDrowData **)m_ayDrowData.GetData();

		for(int i = 0; i < m_ayDrowData.GetSize(); i++)
		{
			pType[i]->iDrowNum = 2;
			for(int j = 0; j < pType[i]->ayList.GetSize(); j ++)
			{
				(pType[i]->ayList.GetData())[j]->ayLine.SetSize(m_AryListColHandName.GetSize());
			}
		}
		ReDrowListData(m_Tab_Ctrl.GetCurSel());
		SetHSBlockInfoColName();
	}
}


void CDlgBlockAnalysis::OnDblclkList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pListView = (NM_LISTVIEW*)pNMHDR;
	int	iItem = pListView->iItem;
	int	iSubItem = pListView->iSubItem;
	int	iTab = m_Tab_Ctrl.GetCurSel();
	
	ListDrowData* pList = m_ayDrowData.GetAt(m_Tab_Ctrl.GetCurSel());
	if(iSubItem >= pList->iDrowNum)
	{
		return;
	}
	if(iSubItem < 2)
	{
		CString strBlock = m_List_Ctrl.GetItemText(iItem, 0);
		if( strBlock.IsEmpty() )
		{
			return;
		}

		m_strData.Empty();
		m_strData.Copy(strBlock);

		::SendMessage(g_hParentWnd, HX_USER_DRAGDATA, 
			HX_USER_DRAGDATA - 102,(LPARAM)&m_strData);
	}
	else
	{
		TYPEANDBLOCK*	pList = m_TypeAndBlock->m_ayBlockCalc.GetAt(iTab);
		CString strTemp;
		strTemp.Format("%s", m_List_Ctrl.GetItemText(iItem, 0));
		BlockInfo *pBlock;
		iItem = pList->ayBlock.GetSize();
		int i;
		for( i = 0; i < iItem; i ++)
		{
			pBlock = pList->ayBlock.GetAt(i);
			if(strTemp.Compare(pBlock->strBlockName) == 0)
				break;
		}
		if(i >= iItem)
		{
			return;
		}
		iItem = i;
		CDlgBlockPicAnalysis	*pdlgPicAnaly = new CDlgBlockPicAnalysis(this, pList, m_ayStockIndex.GetAt(iTab), iItem, iSubItem);
		pdlgPicAnaly->CenterWindow(this);
		pdlgPicAnaly->ShowWindow(SW_SHOW);
	}
	*pResult = 0;
}

void CDlgBlockAnalysis::OnDestroy() 
{
	SaveListColHandName();

	m_AryListColHandName.RemoveAll();
	int i;
	for( i = 0; i < m_ayDrowData.GetSize(); i++)
	{
		delete m_ayDrowData.GetAt(i);
	}
	m_ayDrowData.RemoveAll();
	for(i = 0; i < m_ayStockIndex.GetSize(); i ++ )
	{
		delete m_ayStockIndex.GetAt(i);
	}
	m_ayStockIndex.RemoveAll();
	
	::SendMessage(g_hParentWnd, HX_USER_COMPILEDATA, 12, 0);
	// 表示本窗口停止
	//m_bRunFlag = 0;
	
	CDialog::OnDestroy();
}

void CDlgBlockAnalysis::SetSize()
{
	CRect rcClient, rcTab, rcList, rcCancel;
	CRect rcButton;

	GetClientRect(&rcClient);

	CWnd *pWnd = GetDlgItem(IDC_SAVEAS2);
	if( pWnd != NULL )
	{
		pWnd->GetWindowRect(&rcButton);
	}
	else
	{
		return;
	}

	rcClient.left += HS_TAB_X;
	rcClient.right -= HS_TAB_X;

	rcTab = rcClient;
	rcTab.top += HS_TAB_Y;
	rcTab.bottom = rcTab.top + HS_LIST_Y;
	m_Tab_Ctrl.MoveWindow(rcTab);

	rcList = rcClient;
	rcList.top = rcTab.bottom + HS_BORDER_H;
	rcList.bottom -= (HS_BORDER_H * 2 + rcButton.Height());
	m_List_Ctrl.MoveWindow(&rcList);

	rcClient.top = rcList.bottom + HS_BORDER_H;
	rcClient.bottom = rcClient.top + rcButton.Height();

	rcCancel = rcClient;
	rcCancel.left = rcCancel.right - rcButton.Width();
	pWnd->MoveWindow(&rcCancel);

	rcClient.right = rcCancel.left - 2*HS_BORDER_W;
	rcClient.left = rcClient.right - rcButton.Width();
	pWnd = GetDlgItem(IDC_SAVEAS);
	pWnd->MoveWindow(&rcClient);

	rcCancel.right = rcClient.left - HS_BORDER_W;
	rcCancel.left = rcCancel.right - rcButton.Width();
	m_add.MoveWindow(&rcCancel);

	rcClient.right = rcCancel.left - HS_BORDER_W;
	rcClient.left = rcClient.right - rcButton.Width();
	pWnd = GetDlgItem(IDC_SAVEAS);
	m_begin.MoveWindow(&rcClient);
}

void CDlgBlockAnalysis::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);

	if(m_Tab_Ctrl.GetSafeHwnd() == NULL)
		return;

	SetSize();
}


void CDlgBlockAnalysis::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
}

void CDlgBlockAnalysis::BuildStockIndex(CArray<StockUserInfo*,StockUserInfo*> *payStock, TabStockSortData* pIndex)
{
	int nIend = payStock->GetSize();
	StockSortData* pSort;
	StockUserInfo* pStock;

	if( pIndex->m_ayTabSortData.GetSize() > 0 )
	{
		for(int i = 0; i < pIndex->m_ayTabSortData.GetSize(); i++ )
		{
			delete pIndex->m_ayTabSortData.GetAt(i);
		}
		pIndex->m_ayTabSortData.RemoveAll();
	}

	for(int i = 0; i < nIend; i++ )
	{
		pStock = payStock->GetAt(i);
		pSort = new StockSortData;
		pSort->m_Stock = atol( (LPCSTR)pStock->m_ciStockCode.GetCode() );
		pSort->nIndex = i;

		StockSortData *pSortTemp;
		int nJend = pIndex->m_ayTabSortData.GetSize();

		if( nJend <= 0 )
		{
			pIndex->m_ayTabSortData.Add(pSort);
		}
		else
		{
			pSortTemp = pIndex->m_ayTabSortData.GetAt(0);
			int j = 0;
			if( pSort->m_Stock < pSortTemp->m_Stock )
			{
				pIndex->m_ayTabSortData.InsertAt(j, pSort);
			}
			else
			{
				for(j = 0; j < nJend; j++)
				{
					pSortTemp = pIndex->m_ayTabSortData.GetAt(j);
					if( pSort->m_Stock > pSortTemp->m_Stock )
					{
						if( j == (nJend - 1) )
							break;
						pSortTemp = pIndex->m_ayTabSortData.GetAt(j + 1);
						if( pSort->m_Stock < pSortTemp->m_Stock )
							break;
					}
				}
				if( j >= (nJend - 1) )
				{
					pIndex->m_ayTabSortData.Add(pSort);
				}
				else
				{
					pIndex->m_ayTabSortData.InsertAt(j + 1, pSort);
				}
			}
			
		}
		
	}
}

int	CDlgBlockAnalysis::GetStockIndex(TabStockSortData* pIndex, CString StockCode)
{
	long lTemp = atol( (LPCSTR)StockCode );
	int	nCount = pIndex->m_ayTabSortData.GetSize();

	if( lTemp == ( (StockSortData*)(pIndex->m_ayTabSortData.GetAt(0)) )->m_Stock )
	{
		return ( (StockSortData*)(pIndex->m_ayTabSortData.GetAt(0)) )->nIndex;
	}
	else if( lTemp == ( (StockSortData*)(pIndex->m_ayTabSortData.GetAt(nCount - 1)) )->m_Stock )
	{
		return  ((StockSortData*)(pIndex->m_ayTabSortData.GetAt(nCount - 1)) )->nIndex;
	}
	else
	{
		int nBegin = 0, nEnd = nCount - 1;
		int	nCur;
		while( nBegin < nEnd )
		{
			nCur = (nBegin + nEnd) / 2;
			if( nCur == nBegin || nCur == nEnd)
			{
				return -1;
			}
			if( lTemp == ( (StockSortData*)(pIndex->m_ayTabSortData.GetAt(nCur)) )->m_Stock )
			{
				break;
			}
			else if( lTemp > ( (StockSortData*)(pIndex->m_ayTabSortData.GetAt(nCur)) )->m_Stock )
			{
				nBegin = nCur;
			}
			else
			{
				nEnd = nCur;
			}
		}
		return ( (StockSortData*)(pIndex->m_ayTabSortData.GetAt(nCur)) )->nIndex;
	}
	return -1;
}

void CDlgBlockAnalysis::OnBnClickedSaveas()
{
	CFileDialog dlg(FALSE, "Bak", 
		"BlockAnalyseResult", 
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, 
		_T("分析结果输出文件(*.Bak)|*.Bak|所有文件|*.*||"), this);

	if( dlg.DoModal() == IDOK )
	{
		CString StrFilePath;
		StrFilePath = dlg.GetPathName();
		CFile file;
		file.Open(StrFilePath, CFile::modeCreate | CFile::modeWrite);
		ListDrowData* pList = m_ayDrowData.GetAt(m_Tab_Ctrl.GetCurSel());
		int nEnd = pList->iDrowNum;
		int i;
		for( i = 0; i < m_AryListColHandName.GetSize(); i++ )
		{
			LISTCOLHANDNAME List;
			List = m_AryListColHandName.GetAt(i);
			file.Write( (LPCSTR)List.strColName, List.strColName.GetLength() );
			file.Write("\t\t", sizeof(char) * 2);
		}
		file.Write("\r\n", sizeof(char) * 2);

		CString StrTemp;
		for(i = 0; i < pList->ayList.GetSize(); i++ )
		{
			LineDrowData *pLine = pList->ayList.GetAt(i);
			if(pLine->strLineName.GetLength() < 8 )
			{
				StrTemp.Format("%s\t\t%.0f\t\t", pLine->strLineName, pLine->ayLine.GetAt(0));
			}
			else
			{
				StrTemp.Format("%s\t%.0f\t\t", pLine->strLineName, pLine->ayLine.GetAt(0));
			}
			file.Write( (LPCSTR)StrTemp, StrTemp.GetLength() );
			for(int j = 1; j < nEnd - 1; j++ )
			{
				StrTemp.Format("%.2f\t\t", pLine->ayLine.GetAt(j));
				file.Write( (LPCSTR)StrTemp, StrTemp.GetLength() );
			}
			file.Write("\r\n", sizeof(char) * 2);
		}
		file.Close();
	}
}

void CDlgBlockAnalysis::PostNcDestroy()
{
	::SendMessage(g_hParentWnd, HX_USER_COMPILEDATA, 12, 0);

	CDialog::PostNcDestroy();
}

void CDlgBlockAnalysis::OnCancel()
{
	if( ::IsWindow(m_hWnd) )
	{
		DestroyWindow();
	}
}

void CDlgBlockAnalysis::EnableCtrl(BOOL bFlag)
{
	m_begin.EnableWindow(bFlag);
	m_add.EnableWindow(bFlag);

	CWnd *pWnd = GetDlgItem(IDC_SAVEAS);
	pWnd->EnableWindow(bFlag);
	m_Tab_Ctrl.EnableWindow(bFlag);
}

void CDlgBlockAnalysis::OnBnClickedSaveas2()
{
	OnCancel();
}
