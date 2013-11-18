// DlgIntelligentSelStock.cpp : implementation file
//

#include "stdafx.h"
//#include "HSCompMan.h"
#include "DlgIntelligentSelStock.h"

#include "DlgFormulaMan.h"
#include "GeneralHelper.h"
#include "PubStruct.h"

#include <stdio.h>
#include <io.h>
#include <sys/stat.h>

extern HWND g_hParentWnd;
// CDlgIntelligentSelStock dialog

#define INI_FILE_GROUP  "IntelligentGroup.ini"
#define INI_FILE_SELECT "IntelligentSelect.ini"


IMPLEMENT_DYNAMIC(CDlgIntelligentSelStock, CDialog)
CDlgIntelligentSelStock::CDlgIntelligentSelStock(CWnd* pParent /*=NULL*/,
								   HSOpenExpressUseData* pData/* = NULL*/,
								   long* pThis /*= NULL*/)
	: CDialog(CDlgIntelligentSelStock::IDD, pParent)
{
	/*SetThisParam(pThis);*/

	m_pData = pData;
	if( pThis )
	{
		CDialog::Create(CDlgIntelligentSelStock::IDD,pParent);
		CenterWindow(CWnd::FromHandle(g_hParentWnd));
	}
}

CDlgIntelligentSelStock::~CDlgIntelligentSelStock()
{
}

void CDlgIntelligentSelStock::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB, m_wndTab);
	DDX_Control(pDX, IDC_COMPLIST, m_wndCompList);
	DDX_Control(pDX, IDD_SELECT, m_wndSelectText);
	DDX_Control(pDX, IDC_NOTESEDIT, m_wndNotes);
	DDX_Control(pDX, IDC_RECTGRID, m_wndRectGrid);
}


BEGIN_MESSAGE_MAP(CDlgIntelligentSelStock, CDialog)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB, OnTabSelChange)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_COMPLIST, OnListItemChanged)
	ON_BN_CLICKED(IDC_SAVEASBLOCK, OnSaveAsBock)

	ON_REGISTERED_MESSAGE(HX_USER_COMPILEDATA, OnExchangeData)

	ON_BN_CLICKED(IDC_USERNOTES, OnBnClickedUsernotes)
END_MESSAGE_MAP()


// CDlgIntelligentSelStock message handlers
BOOL CDlgIntelligentSelStock::OnInitDialog() 
{
	CDialog::OnInitDialog();
	this->ShowWindow(SW_HIDE);

	if( m_imgList.m_hImageList == NULL )
	{
		m_imgList.Create(IDD_ALLIMAGE, 16, 0, RGB (0, 128, 128));
		m_wndCompList.SetImageList(&m_imgList,LVSIL_SMALL);
	}
	DWORD dwStyle = m_wndCompList.GetExtendedStyle();
	dwStyle &= ~LVS_EX_ONECLICKACTIVATE;
	dwStyle |= LVS_EX_FULLROWSELECT|LVS_EX_TWOCLICKACTIVATE|LVS_EX_UNDERLINEHOT;
	m_wndCompList.SetExtendedStyle(dwStyle);

	CString str;
	str = _T("名称");
	m_wndCompList.InsertColumn(0,(char*)LPCTSTR(str),LVCFMT_CENTER,128);


	this->GetAllSection(m_ayGroupList);
	for( int i = 0;i < m_ayGroupList.GetSize();i++)
	{
		m_wndTab.InsertItem(i,m_ayGroupList.GetAt(i));
	}

/*	CRect rect;
	GetClientRect(&rect);
	m_pHSGridSelStockResult->Create( rect, this, 1001);

	CWnd* pWnd = GetDlgItem(IDC_RECTGRID);
	pWnd->GetWindowRect(&rect);
	ScreenToClient(&rect);
	m_pHSGridSelStockResult->MoveWindow(rect);
	m_pHSGridSelStockResult->ShowWindow(SW_SHOW);

	InitTableData inittabledata;
	m_pHSGridSelStockResult->AddColumn(&inittabledata);
	*/
	CRect rect;
	CWnd* pWnd = GetDlgItem(IDC_RECTGRID);
	pWnd->GetWindowRect(&rect);
	ScreenToClient(&rect);
	m_wndNotes.MoveWindow(rect);
	this->m_wndNotes.ShowWindow(SW_HIDE);


	m_wndTab.SetCurSel(0);
//	LoadUnion();
//	this->ShowWindow(SW_HIDE);
	

	pWnd = this->GetDlgItem(IDOK);
	if(pWnd)
	{
		pWnd->SetFocus();
	}
	
	// by HS add 2002.06.16 向父窗口发送，创建选股结果表格
	::SendMessage(g_hParentWnd,HX_USER_COMPILEDATA,37,(LPARAM)&m_wndRectGrid);
		// 
	SendMessage(HX_USER_COMPILEDATA,8,0);
	// end
	return false;
}

void CDlgIntelligentSelStock::LoadUnion()
{
	          // 板块名
	
//	m_curSelStockCondition.m_sSingleSel = &m_sSingleSel;		// 单个选股条件
	
	UnionSelConditionSub* pUnionGroup = new UnionSelConditionSub;
	pUnionGroup->m_strName = /*m_strCond[1]*/_T("批量选股");
	pUnionGroup->m_cAndOr  = 0;
	m_UnionSelCondition.m_aySub->Add(pUnionGroup);

	CString strSection;
	for(int i = 0 ;i < m_ayGroupList.GetSize() ;i ++)
	{
		strSection = m_ayGroupList.GetAt(i);
		if(strSection != "")
		{
			CStringArray ayList;
			this->SaveLoad(INI_FILE_GROUP,strSection,ayList,false);
			//此处加入
			CString strKey;
			for(int j = 0; j < ayList.GetSize(); j++)
			{
				strKey = ayList.GetAt(j);
				strKey.TrimLeft();
				strKey.TrimRight();
				CString strExp;
				if(strKey != "")
				{
					//CString strFile = this->GetTempletFileName(INI_FILE_GROUP);
					//::GetPrivateProfileString(strSection,strKey,"",strExp.GetBuffer(),10,strFile);
					//strExp.ReleaseBuffer();
					//此处加入批量条件到m_UnionSelCondition中
					SingleSelCondition* pSingleSel = new SingleSelCondition;
					if(pUnionGroup->m_aySub)
					{
						pSingleSel->m_nType = Condition; 
						strExp.TrimLeft();
						strExp.TrimRight();
						pSingleSel->m_strExp = strKey;
						pSingleSel->m_SubData.m_cType = FormulaCompareSubData::Condition;
						//pSingleSel->m_strCondition = strSection;
						pUnionGroup->m_aySub->Add(pSingleSel);
					}
				}
			}
		}
	}

	m_curSelStockCondition.m_dStyle = SelectStock_SelStockAllCalc;
	//	m_curSelStockCondition.m_ayCheck	= &m_ayCheck;			// 股票范围
//	m_curSelStockCondition.m_strBlock	= &m_strBlock;
	m_curSelStockCondition.m_pHSUnion  = &m_UnionSelCondition;
	
	
	CString str;
	CTime timeDest = CTime::GetCurrentTime();
	str.Format("%04d%02d%02d",timeDest.GetYear(),timeDest.GetMonth(),timeDest.GetDay());
	m_curSelStockCondition.m_lEndDate = atol(str);   

	m_curSelStockCondition.m_lDateCount = 0;

	m_curSelStockCondition.m_pData = NULL;
	
	//long lRet = ::SendMessage(g_hParentWnd,HX_USER_COMPILEDATA,	3,(LPARAM)&m_curSelStockCondition);
	long lRet = ::SendMessage(g_hParentWnd,HX_USER_COMPILEDATA,	48,(LPARAM)&m_curSelStockCondition);
	//	this->m_wndCompList.SetItem(
	//	this->OnListItemChanged(

}

//void CDlgIntelligentSelStock::OnCancel() 
//{
//
//	Delete();
//
//	if( ::IsWindow(m_hWnd) )
//	{
//		DestroyWindow();
//	}
//}

void CDlgIntelligentSelStock::OnOK() 
{
	// by HS replace 2002.06.16
	Delete();
	CDialog::OnOK();

	/*
	Delete();
	if( ::IsWindow(m_hWnd) )
	{
		DestroyWindow();
	}
	*/
	// end
}

void CDlgIntelligentSelStock::MyNcDestroy()
{
	::SendMessage(g_hParentWnd,HX_USER_COMPILEDATA,20,0);
}

void CDlgIntelligentSelStock::Delete()
{
	CString* pStr;
	for(int i = 0;i <  m_wndCompList.GetItemCount();i++)
	{
		pStr = (CString*)(m_wndCompList.GetItemData(i));
		if(pStr != NULL)
		{
			delete pStr;
		}
	}
/*	if( m_pHSGridSelStockResult != NULL )
	{
		delete m_pHSGridSelStockResult;
		m_pHSGridSelStockResult = NULL;
	}
	return;
*/
}

// bool CDlgIntelligentSelStock::AddToIni(CNI_ReceiveData* pInfo,CString strTeam)
// {
// 	//SelStockResult* pStock = (SelStockResult*)pInfo->m_pszData;
// /*	StockUserInfo* pStock = (StockUserInfo*)pInfo->m_pszData;
// 	if(pStock != NULL )
// 	{
// 		CodeInfo codeInfo  = pStock->m_ciStockCode;
// 		CString strKeyName = codeInfo.GetKey();
// 
// 		CStringArray strNull;
// 		SaveLoad(INI_FILE_SELECT,strTeam,strKeyName,strNull,true);
// //		SaveLoad(INI_FILE_SELECT,strTeam,strKeyName,NULL,true);
// 		return true;
// 	}
// */	return false;
// }

bool CDlgIntelligentSelStock::ReadFromIni(CArray<SelStockResult*,
			SelStockResult*>& pSelectArray,CString strTeam)
{
	//SelStockResult* pStock = (SelStockResult*)pInfo->m_pszData;
	for( int i = 0;i < pSelectArray.GetSize();i++)
	{
		if(pSelectArray.GetAt(i) != NULL)
		{
			delete pSelectArray.GetAt(i);
		}
	}
	pSelectArray.RemoveAll();

	CStringArray ayRet;
	
	if(SaveLoad(INI_FILE_SELECT,strTeam,ayRet,false) == false)
	{
		return false;
	}

	CodeInfo codeInfo;
	memset(&codeInfo,0,sizeof(codeInfo));
	StockUserInfo* pStockUserInfo;
	CString strCur;
	int i;
	for( i = 0 ; i < ayRet.GetSize() ; i++)
	{
		strCur = ayRet.GetAt(i);
		if( strCur != "")
		{
			codeInfo.m_cCodeType = strCur.GetAt(0);
			strcpy(codeInfo.m_cCode,strCur.Right(6));
			pStockUserInfo = 
				CodeInfoToStockUserInfo(&codeInfo);
			if( pStockUserInfo != NULL )
			{
				pSelectArray.Add((SelStockResult*)pStockUserInfo);
			}
		}
	}
	return true;
}

bool CDlgIntelligentSelStock::SaveLoad(LPCTSTR szFileName,CString strSection,
		CStringArray& ayKeyName, bool bSave/*=true*/)
{
	CString strFile = GetTempletFileName(szFileName);
	
	if( bSave )
	{
		CString strKeyName;
		for(int i = 0;i < ayKeyName.GetSize();i++)
		{
			strKeyName = ayKeyName.GetAt(i);
			if(strKeyName != "")
			{
				::WritePrivateProfileString(strSection,
				strKeyName,"",strFile);
			}
		}
		::WritePrivateProfileString(strSection,
				strKeyName,"",strFile);
		
		return true;
	}
	else
	{
		struct _stat buf;
		memset(&buf, 0, sizeof(buf));
		_stat(strFile, &buf);
		int nSize = (int)buf.st_size;
		if(nSize <= 0)
		{
			return 0;
		}
		char* strBuf = new char[nSize+1];

		::GetPrivateProfileString(strSection,NULL,"",strBuf,nSize,strFile);
		char* p = strBuf;

		int nLen;
		char strItem[1024];
	//	CStringArray* pArray;
	//	CString strValue;
		while(*p != '\0')
		{
			::GetPrivateProfileString(strSection,p,"",strItem,1023,strFile);		
			ayKeyName.Add(p);
			nLen = strlen(p) + 1;
			p += nLen;
		}  
		delete[] strBuf;
		return true;
	}
	return false;
}

CString CDlgIntelligentSelStock::GetTempletFileName(LPCTSTR pFileName)
{
	CString strFile =  CGeneralHelper::GetSpecifyPath(CGeneralHelper::SETTING_PATH) ;
	strFile += pFileName;
	if( _access(strFile, 0) != 0 )
	{
		FILE* fp = fopen(strFile, "w");
		fclose(fp);
	}
	return strFile;
}

int CDlgIntelligentSelStock::GetAllSection(CStringArray& aySectionList)
{
	CString strFile = GetTempletFileName(INI_FILE_GROUP);
	
	struct _stat buf;
	memset(&buf, 0, sizeof(buf));
	_stat(strFile, &buf);
	int nSize = (int)buf.st_size;
	if(nSize <= 0)
	{
		return 0;
	}
	char* strBuf = new char[nSize+1];
	
	::GetPrivateProfileString(NULL,NULL,"",strBuf,nSize,strFile);
	char* p = strBuf;

	int nLen;
	while(*p != '\0')
	{
		aySectionList.Add(p);
		nLen = strlen(p) + 1;
		p += nLen;
	}  
	delete[] strBuf;
	
	return aySectionList.GetSize();
}

BOOL CDlgIntelligentSelStock::ShowWindow( int nCmdShow )
{

	if( !::IsWindow(this->m_hWnd) )
		return 0;

	if( nCmdShow == SW_HIDE || nCmdShow == SW_MINIMIZE )
		return CDialog::ShowWindow(nCmdShow);


	if( IsIconic() )
	{
		return CDialog::ShowWindow(SW_RESTORE);
	}

	if(  IsZoomed() ) // max
		return CDialog::ShowWindow(nCmdShow);

	if( ::AnimateWindow(m_hWnd,100,AW_CENTER | AW_ACTIVATE) )
	{
		this->Invalidate();
	}
	else
	{
		return CDialog::ShowWindow(nCmdShow);
	}

	return 1;
}
/*BOOL CDlgIntelligentSelStock::GetListFromSection(CString strGroup,CStringArray& ayList)
{
	CString strFile = GetTempletFileName(INI_FILE_GROUP);
	struct _stat buf;
		memset(&buf, 0, sizeof(buf));
		_stat(strFile, &buf);
		int nSize = (int)buf.st_size;
		if(nSize <= 0)
		{
			return 0;
		}
		char* strBuf = new char[nSize+1];

		::GetPrivateProfileString(strGroup,NULL,"",strBuf,nSize,strFile);
		char* p = strBuf;

		int nLen;
		char strItem[1024];
	//	CStringArray* pArray;
	//	CString strValue;
		CHSAccel curHSAccel;
		while(*p != '\0')
		{
			::GetPrivateProfileString(strSection,p,"",strItem,1023,strFile);		
			ayKeyName->Add(p);
			nLen = strlen(p) + 1;
			p += nLen;
		}  
		delete[] strBuf;
		return true;
	}

}
*/
StockUserInfo* CDlgIntelligentSelStock::CodeInfoToStockUserInfo(CodeInfo * pCodeInfo)
{
	return (StockUserInfo *)::SendMessage(g_hParentWnd, HX_USER_COMPILEDATA, 19, (LPARAM)pCodeInfo);
}


void CDlgIntelligentSelStock::OnTabSelChange(NMHDR *pNMHDR, LRESULT *pResult)
{
	int nIndex       = m_wndTab.GetCurSel( );
	if (nIndex == -1)
	{
		return ;
	}
	CString strGroup =	this->m_ayGroupList.GetAt(nIndex);
	
	CStringArray ayList;
	SaveLoad(INI_FILE_GROUP,strGroup,ayList,false);

	this->m_wndCompList.DeleteAllItems();
	CString strList;
	for(int i = 0 ;i < ayList.GetSize();i ++)
	{
	//	strList = strGroup;
		strList = ayList.GetAt(i);
		int nIndex ;
	    CString* pStr;
		if( strList != "")
		{
			nIndex = m_wndCompList.InsertItem(LVIF_TEXT|LVIF_PARAM|LVIF_IMAGE,i,
			strList,0,0,0,LVIF_PARAM);
			/*strList = strGroup;
			strList += "+";*/
			strList = ayList.GetAt(i);
			pStr = new CString;
			*pStr = strList;
			m_wndCompList.SetItemData( nIndex,(DWORD)pStr );
		}
	}
	ListView_SetItemState(m_wndCompList.m_hWnd,0,
		LVIS_SELECTED|LVIS_FOCUSED,LVIS_SELECTED|LVIS_FOCUSED);

	if(pResult)
	{
		*pResult = 0;
	}
}

void CDlgIntelligentSelStock::OnListItemChanged(NMHDR *pNMHDR, LRESULT *pResult)
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	if(pNMListView != NULL && (*pNMListView).uNewState == (LVIS_SELECTED | LVIS_FOCUSED) )
	{
		if(pNMListView->iItem == -1)
		{
			return ;
		}
		CString * pStr;
		pStr = (CString*)(m_wndCompList.GetItemData(
			pNMListView->iItem));

		CValue *pVal;
		if(CDlgFormulaMan::m_pExternExpression->Lookup(*pStr,Condition,pVal) )
		{
			if(pVal)
			{
				CExpression* pExpression = ((CExpValue*)pVal)->GetExp();
				if(pExpression)
				{
					m_wndNotes.SetWindowText(pExpression->GetNotes());
				}
			}
		}
		
		CArray<SelStockResult*,SelStockResult*> aySelect;
		ReadFromIni(aySelect, *pStr);

		::SendMessage(g_hParentWnd, HX_USER_COMPILEDATA, 21, (LPARAM)(&aySelect));

		CString strText;
		strText.Format(_T("共选中%d支股票！"),
			aySelect.GetSize());

		this->m_wndSelectText.SetWindowText(strText);
	}
	*pResult = 0;
}



void CDlgIntelligentSelStock::OnSaveAsBock()
{
	// TODO: Add your control notification handler code here
	::SendMessage(g_hParentWnd,HX_USER_COMPILEDATA,8,(LPARAM)this);
}

LRESULT CDlgIntelligentSelStock::OnExchangeData(WPARAM wp,LPARAM lp)
{
	if( wp == 4 ) // 选股结束
	{
		this->m_wndTab.SetCurSel(0);
		this->OnTabSelChange(NULL,NULL);
/*		if(m_wndCompList.GetItemCount() > 0)
		{
			this->m_wndCompList.SetSelectionMark(0);
		}
		this->OnListItemChanged(NULL,NULL);
*/
		this->ShowWindow(SW_SHOW);
		//		EnableBut(TRUE);
	}
	else if( wp == 5 ) // 
	{
	}
	else if (wp == 8) // 
	{
		this->ShowWindow(SW_HIDE);
		this->LoadUnion();
		
	}
	/*else if (wp == 2) // 板块名称
	{
		CWnd* pWnd = this->GetDlgItem(IDC_RECTGRID);
		CRect rect;
		pWnd->GetClientRect(rect);
		return (LRESULT)&rect;
	}
	if( wp == 1 ) // 股票列表
	{		
	}
	else if (wp == 2) // 板块名称
	{
		m_strBlock.Format("%s",*(CString*)lp);
		Select();
	}
	else if (wp == 3) // 返回当前选中股票
	{
		return (long)&m_ayCheck;
	}
	else if( wp == 4 ) // 选股结束
	{
		CWnd* pOKBut = this->GetDlgItem(IDOK);
		if( pOKBut != NULL )
		{
			pOKBut->SetWindowText("执行");
		}
		CString strSelectStockNum;
		m_wndSelectStock.GetWindowText(strSelectStockNum);
		if( strSelectStockNum == "0")
		{
			MessageBox("很遗憾，没找到您想要的股票！","提示");
		}
		//		EnableBut(TRUE);
	}

	else if( wp == 5 ) // 返回当前选中股票数
	{
		CNI_ReceiveData* pInfo2 = (CNI_ReceiveData*)lp;

		CString strText;
		strText.Format("%d",pInfo2->m_nStatus);
		m_wndSelectStock.SetWindowText(strText);

		m_wndTotalStock.GetWindowText(strText);
		if( strText.IsEmpty() )
		{
			strText.Format("%d",pInfo2->m_lDataTotal);
			m_wndTotalStock.SetWindowText(strText);
		}
	}
	else if( wp == 6 ) // 返回所有被查找的股票数
	{
		CString strText;
		strText.Format("%d",lp);
		this->m_wndTotalStock.SetWindowText(strText);

	}
*/
	return 0;
}

CString CDlgIntelligentSelStock::NameToExp(CString strName)
{
	CString strExp;

	CString strSection;
	for(int i = 0 ;i < m_ayGroupList.GetSize() ;i ++)
	{
		strSection = m_ayGroupList.GetAt(i);
		if(strSection != "")
		{
			CStringArray ayList;
			this->SaveLoad(INI_FILE_GROUP,strSection,ayList,false);
			//此处加入
			CString strKey;
			for(int j = 0; j < ayList.GetSize(); j++)
			{
				strKey = ayList.GetAt(j);
				CString strExp;
				if(strKey == strName)
				{
					CString strFile = this->GetTempletFileName(INI_FILE_GROUP);
					::GetPrivateProfileString(strSection,strKey,"",strExp.GetBuffer(10),10,strFile);
					strKey.ReleaseBuffer();
					return strExp;
				}
			}
		}
	}

	return "";

}

void CDlgIntelligentSelStock::OnBnClickedUsernotes()
{
	// TODO: Add your control notification handler code here
	CWnd* pWnd = this->GetDlgItem(IDC_USERNOTES);
	CString strBut;
	pWnd->GetWindowText(strBut);
	strBut.TrimLeft();
	strBut.TrimRight();
	if(strBut == _T("注释..."))
	{
		this->m_wndNotes.ShowWindow(SW_SHOW);
		m_wndRectGrid.ShowWindow(SW_HIDE);
		pWnd->SetWindowText(_T("选出股票..."));
	}
	else if(strBut == _T("选出股票..."))
	{
		m_wndRectGrid.ShowWindow(SW_SHOW);
		this->m_wndNotes.ShowWindow(SW_HIDE);
		pWnd->SetWindowText(_T("注释..."));
	}


}
