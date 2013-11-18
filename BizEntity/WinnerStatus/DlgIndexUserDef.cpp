// DlgIndexUserDef.cpp : 实现文件
//

#include "stdafx.h"
#include "WinnerStatus.h"
#include "DlgIndexUserDef.h"
#include "DlgSelStock.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
// CDlgIndexUserDef 对话框

IMPLEMENT_DYNAMIC(CDlgIndexUserDef, CDialog)

CDlgIndexUserDef::CDlgIndexUserDef(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgIndexUserDef::IDD, pParent)
	, m_threeShow(false)
	, m_fourShow(false)
	, m_scrollSpeed(0)
{
	m_oneList.RemoveAll();
	m_twoList.RemoveAll();
	m_threeList.RemoveAll();
	m_fourList.RemoveAll();

// 	m_pOneList = NULL;
// 	m_pTwoList = NULL;
// 	m_pThreeList = NULL;
// 	m_pFourList = NULL;
	m_pDataSource = NULL;
	m_pBlockManager = NULL;

	m_nSelectIndex = 1;
}

CDlgIndexUserDef::~CDlgIndexUserDef()
{
}

void CDlgIndexUserDef::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_myListBox1);
	DDX_Control(pDX, IDC_LIST2, m_myListBox2);
	DDX_Control(pDX, IDC_LIST3, m_myListBox3);
	DDX_Control(pDX, IDC_LIST4, m_myListBox4);
	DDX_Check(pDX,IDC_THREECHECK, m_threeShow);
	DDX_Check(pDX,IDC_FOURCHECK, m_fourShow);
	DDX_Text(pDX,IDC_EDIT1,m_scrollSpeed);
	DDV_MinMaxInt(pDX,IDC_EDIT1,0,INT_MAX);
}


BEGIN_MESSAGE_MAP(CDlgIndexUserDef, CDialog)
	ON_LBN_SETFOCUS(IDC_LIST1, &CDlgIndexUserDef::OnLbnSetfocusList1)
	ON_LBN_SETFOCUS(IDC_LIST2, &CDlgIndexUserDef::OnLbnSetfocusList2)
	ON_LBN_SETFOCUS(IDC_LIST3, &CDlgIndexUserDef::OnLbnSetfocusList3)
	ON_LBN_SETFOCUS(IDC_LIST4, &CDlgIndexUserDef::OnLbnSetfocusList4)
	ON_BN_CLICKED(IDC_BUTTON2, &CDlgIndexUserDef::OnBnClickedButton2)
	ON_LBN_SELCHANGE(IDC_LIST1, &CDlgIndexUserDef::OnLbnSelchangeList1)
	ON_LBN_SELCHANGE(IDC_LIST2, &CDlgIndexUserDef::OnLbnSelchangeList2)
	ON_LBN_SELCHANGE(IDC_LIST3, &CDlgIndexUserDef::OnLbnSelchangeList3)
	ON_LBN_SELCHANGE(IDC_LIST4, &CDlgIndexUserDef::OnLbnSelchangeList4)
	ON_BN_CLICKED(IDC_BUTTON1, &CDlgIndexUserDef::OnBnClickedButton1)
	ON_WM_RBUTTONDOWN()
END_MESSAGE_MAP()


// CDlgIndexUserDef 消息处理程序

BOOL CDlgIndexUserDef::OnInitDialog()
{
	CDialog::OnInitDialog();

	for(int i=0; i<m_oneList.GetCount(); i++)
		m_myListBox1.AddString(m_oneList.GetAt(i).myStockName);
	for(int i=0; i<m_twoList.GetCount(); i++)
	    m_myListBox2.AddString(m_twoList.GetAt(i).myStockName);
	for(int i=0; i<m_threeList.GetCount(); i++)
		m_myListBox3.AddString(m_threeList.GetAt(i).myStockName);
	for(int i=0; i<m_fourList.GetCount(); i++)
		m_myListBox4.AddString(m_fourList.GetAt(i).myStockName);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CDlgIndexUserDef::OnLbnSetfocusList1()
{
	m_myListBox2.SetCurSel(-1);
	m_myListBox3.SetCurSel(-1);
	m_myListBox4.SetCurSel(-1);
	m_nSelectIndex = 1;
}

void CDlgIndexUserDef::OnLbnSetfocusList2()
{
	m_myListBox1.SetCurSel(-1);
	m_myListBox3.SetCurSel(-1);
	m_myListBox4.SetCurSel(-1);
	m_nSelectIndex = 2;
}

void CDlgIndexUserDef::OnLbnSetfocusList3()
{
	m_myListBox1.SetCurSel(-1);
	m_myListBox2.SetCurSel(-1);
	m_myListBox4.SetCurSel(-1);
	m_nSelectIndex = 3;
}

void CDlgIndexUserDef::OnLbnSetfocusList4()
{
	m_myListBox1.SetCurSel(-1);
	m_myListBox2.SetCurSel(-1);
	m_myListBox3.SetCurSel(-1);
	m_nSelectIndex = 4;
}

//添加按钮
void CDlgIndexUserDef::OnBnClickedButton1()
{
	CDlgSelStock dlg;

	if( IDOK == dlg.DoModal())
	{
		switch(m_nSelectIndex)
		{
		case 1:
			InsertList(m_myListBox1,m_oneList,dlg.m_stockInfoSelected);
			break;
		case 2:
			InsertList(m_myListBox2,m_twoList,dlg.m_stockInfoSelected);
			break;
		case 3:
			InsertList(m_myListBox3,m_threeList,dlg.m_stockInfoSelected);
			break;
		case 4:
			InsertList(m_myListBox4,m_fourList,dlg.m_stockInfoSelected);
			break;
		default:
			break;
		}
	}
}

//删除按钮
void CDlgIndexUserDef::OnBnClickedButton2()
{
	int index;
	if( (index=m_myListBox1.GetCurSel()) != -1)
	{
		m_myListBox1.DeleteString(index);
		m_oneList.RemoveAt(index);
	}
	else if( (index=m_myListBox2.GetCurSel()) != -1)
	{
		m_myListBox2.DeleteString(index);
		m_twoList.RemoveAt(index);
	}
	else if ( (index=m_myListBox3.GetCurSel()) != -1)
	{
		m_myListBox3.DeleteString(index);
		m_threeList.RemoveAt(index);
	}
	else if ( (index=m_myListBox4.GetCurSel()) != -1)
	{
		m_myListBox4.DeleteString(index);
		m_fourList.RemoveAt(index);
	}

}


void CDlgIndexUserDef::OnLbnSelchangeList1()
{
	//将i移动到j
	int i = m_myListBox1.m_Oldposition;
	int j = m_myListBox1.GetCurSel();

	if(i!=j)
	{
		showList temp = m_oneList.GetAt(i);
		m_oneList.RemoveAt(i);
		m_oneList.InsertAt(j,temp);
	}
	
}

void CDlgIndexUserDef::OnLbnSelchangeList2()
{
	//将i移动到j
	int i = m_myListBox2.m_Oldposition;
	int j = m_myListBox2.GetCurSel();

	if(i!=j)
	{
		showList temp = m_twoList.GetAt(i);
		m_twoList.RemoveAt(i);
		m_twoList.InsertAt(j,temp);
	}

}

void CDlgIndexUserDef::OnLbnSelchangeList3()
{
	//将i移动到j
	int i = m_myListBox3.m_Oldposition;
	int j = m_myListBox3.GetCurSel();

	if(i!=j)
	{
		showList temp = m_threeList.GetAt(i);
		m_threeList.RemoveAt(i);
		m_threeList.InsertAt(j,temp);
	}
	
}

void CDlgIndexUserDef::OnLbnSelchangeList4()
{
	//将i移动到j
	int i = m_myListBox4.m_Oldposition;
	int j = m_myListBox4.GetCurSel();

	if(i!=j)
	{
		showList temp = m_fourList.GetAt(i);
		m_fourList.RemoveAt(i);
		m_fourList.InsertAt(j,temp);
	}
}

void CDlgIndexUserDef::InsertList(CMyListBox& myListBox, CArray<showList>& myShowList ,StockUserInfo aStock)
{
	if(aStock.m_cStockName[0] == '\0')
		return;

	for(int i=0; i<m_oneList.GetCount(); i++)
	{
		if(m_oneList.GetAt(i).myCodeInfo.CompareCode(&aStock.m_ciStockCode)) //有重复
			return ;
	}
	for(int i=0; i<m_twoList.GetCount(); i++)
	{
		if(m_twoList.GetAt(i).myCodeInfo.CompareCode(&aStock.m_ciStockCode)) //有重复
			return ;
	}
	for(int i=0; i<m_threeList.GetCount(); i++)
	{
		if(m_threeList.GetAt(i).myCodeInfo.CompareCode(&aStock.m_ciStockCode)) //有重复
			return ;
	}
	for(int i=0; i<m_fourList.GetCount(); i++)
	{
		if(m_fourList.GetAt(i).myCodeInfo.CompareCode(&aStock.m_ciStockCode)) //有重复
			return ;
	}


	if(myShowList.GetCount() == LIST_MAX_COUNT) 
	{//如果达到最大值 则把最后一个替换掉

		myShowList.GetAt(LIST_MAX_COUNT-1).myCodeInfo = aStock.m_ciStockCode;
		strncpy(myShowList.GetAt(LIST_MAX_COUNT-1).myStockName, aStock.m_cStockName, strStructLenth);
	
		myListBox.DeleteString(LIST_MAX_COUNT-1);
		myListBox.AddString(aStock.m_cStockName);
		return;
	}

	showList one;
	one.myCodeInfo = aStock.m_ciStockCode;
	strncpy(one.myStockName, aStock.m_cStockName, strStructLenth);
	myShowList.Add(one);
	myListBox.AddString(aStock.m_cStockName);
}