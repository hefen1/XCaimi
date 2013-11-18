// DlgFormulaFind.cpp : implementation file-- added by HS
//

#include "stdafx.h"
#include "resource.h"
//#include "HSCompMan.h"
#include "DlgFormulaFind.h"
#include "DlgFormulaMan.h"
#include ".\dlgformulafind.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern HWND g_hParentWnd;
extern CDlgFormulaFind *g_pDlgFormulaFind;
extern CDlgFormulaMan* g_pFormulaMan;
BOOL CDlgFormulaFind::bEscMsgOfDlgFind = FALSE;

IMPLEMENT_DYNAMIC(CDlgFormulaFind, CHSBaseDialog)
CDlgFormulaFind::CDlgFormulaFind(DWORD dwMask/*=0xFFFF*/,CWnd* pParent /*=NULL*/)
	: CHSBaseDialog(CDlgFormulaFind::IDD, pParent)
	, m_bCompAll(false)
	, m_bCompDiscribe(FALSE)
	, m_bCompEdolon(FALSE)
	, m_bCompName(FALSE)
	, m_bCompSource(FALSE)
	, m_bCompNotes(FALSE)
{
	this->m_dwMask = dwMask;
	this->m_pParentWnd = pParent;
	Create(CDlgFormulaFind::IDD,pParent);
	this->ShowWindow(SW_SHOW);
}

CDlgFormulaFind::~CDlgFormulaFind()
{
}

void CDlgFormulaFind::DoDataExchange(CDataExchange* pDX)
{
	CHSBaseDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_FINDANIMATE, m_wndAnimate);
	DDX_Control(pDX, IDC_KEYCOMBO, m_wndKeyCombo);
//	DDX_Control(pDX, IDC_RANGECOMBO, m_wndRangeCombo);
	DDX_Control(pDX, IDC_RESULTLIST, m_wndResultList);
	DDX_Check(pDX, IDC_COMPALL     , m_bCompAll);
	DDX_Check(pDX, IDC_COMPDISCRIBE, m_bCompDiscribe);
	DDX_Check(pDX, IDC_COMPEDOLON  , m_bCompEdolon);
	DDX_Check(pDX, IDC_COMPNAME    , m_bCompName);
	DDX_Check(pDX, IDC_COMPSOURCE  , m_bCompSource);
	DDX_Check(pDX, IDC_COMPNOTES   , m_bCompNotes);
}


BEGIN_MESSAGE_MAP(CDlgFormulaFind, CHSBaseDialog)
	ON_CBN_SELCHANGE(IDC_KEYCOMBO, OnCbnSelchangeKeycombo)
	ON_BN_CLICKED(IDC_STARTBUTTON, OnBnClickedStartbutton)
	ON_BN_CLICKED(IDC_STOPBUT, OnBnClickedStopbut)
    ON_NOTIFY(LVN_ITEMACTIVATE, IDC_RESULTLIST, OnLvnItemActivateResultlist)
	ON_WM_CHAR()
	ON_BN_CLICKED(IDC_COMPALL, OnBnClickedCompall)
	ON_BN_CLICKED(IDC_COMPNAME, OnBnClickedCompname)
	ON_BN_CLICKED(IDC_COMPDISCRIBE, OnBnClickedCompdiscribe)
	ON_BN_CLICKED(IDC_COMPNOTES, OnBnClickedCompnotes)
	ON_BN_CLICKED(IDC_COMPEDOLON, OnBnClickedCompedolon)
	ON_BN_CLICKED(IDC_COMPSOURCE, OnBnClickedCompsource)
END_MESSAGE_MAP()

BOOL CDlgFormulaFind::OnInitDialog() 
{
	CHSBaseDialog::OnInitDialog();

    CenterWindow( this->GetParent() );

/*	this->m_wndRangeCombo.SetItemData(0,DWORD(FindInName    ));
	this->m_wndRangeCombo.SetItemData(0,DWORD(FindInDescribe));
	this->m_wndRangeCombo.SetItemData(0,DWORD(FindInNotes   ));
	this->m_wndRangeCombo.SetItemData(0,DWORD(FindInEidolon ));
	this->m_wndRangeCombo.SetItemData(0,DWORD(FindInSource  ));
	this->m_wndRangeCombo.SetItemData(0,DWORD(FindInAll     ));
*/

	m_wndAnimate.Open(IDR_AVI1);
	m_wndAnimate.Stop();

	m_wndKeyCombo.SetEditSel(-1,0);

	m_wndResultList.ModifyStyle(0,LVS_REPORT|LVS_SINGLESEL);
	DWORD dwStyle = m_wndResultList.GetExtendedStyle();
	dwStyle &= ~LVS_EX_ONECLICKACTIVATE;
	dwStyle |= LVS_EX_FULLROWSELECT/*|LVS_EX_GRIDLINES|LVS_EX_TWOCLICKACTIVATE*//*|LVS_EX_UNDERLINEHOT*/;
	m_wndResultList.SetExtendedStyle(dwStyle);

	LV_COLUMN lvcolumn;
	lvcolumn.mask = LVCF_FMT|LVCF_WIDTH|LVCF_TEXT|LVCF_SUBITEM;
	lvcolumn.fmt  = LVCFMT_CENTER;
	lvcolumn.cx   = 25;
	lvcolumn.iSubItem = 0;
	CString str;
	str = _T("��");
	lvcolumn.pszText = (char*)LPCTSTR(str);
	m_wndResultList.InsertColumn(0,&lvcolumn);
	//lvcolumn.cx   = 60;
	lvcolumn.iSubItem = 1;
	str = _T("����");
	lvcolumn.pszText = (char*)LPCTSTR(str);
	lvcolumn.cx   = 65;
	m_wndResultList.InsertColumn(1,&lvcolumn);
	lvcolumn.iSubItem = 2;
	str = _T("����");
	lvcolumn.pszText = (char*)LPCTSTR(str);
	lvcolumn.cx   = 130;
	m_wndResultList.InsertColumn(2,&lvcolumn);
	lvcolumn.iSubItem = 3;
	str = _T("λ��");
	lvcolumn.pszText = (char*)LPCTSTR(str);
	lvcolumn.cx   = 60;
	m_wndResultList.InsertColumn(3,&lvcolumn);

	CWnd* pWnd = this->GetDlgItem(IDC_RESULTSTATIC);
	if( pWnd != NULL)
	{
		pWnd->SetWindowText(_T("��������"));
	}

	pWnd = GetDlgItem(IDC_STARTBUTTON);
	if( pWnd != NULL)
	{
		pWnd->EnableWindow(true);
	}
	pWnd = GetDlgItem(IDC_STOPBUT);
	if( pWnd != NULL)
	{
		pWnd->EnableWindow(false);
	}

	m_bCompAll = true;
	UpdateData(false);

	this->m_wndKeyCombo.SetFocus();

	return FALSE;
}

// CDlgFormulaFind message handlers

void CDlgFormulaFind::OnCbnSelchangeKeycombo()
{
	// TODO: Add your control notification handler code here
}

void CDlgFormulaFind::OnBnClickedStartbutton()
{
	// TODO: Add your control notification handler code here

	m_bThreadRun = true;
	CString strComboText;
	this->m_wndKeyCombo.GetWindowText(strComboText);

	if(strComboText == "")
	{
		MessageBox(_T("������Ҫ���ҵĹؼ��֣�лл��"),_T("��ʾ"),MB_ICONINFORMATION);
		return;
	}
	if(this->GetFindMask() == 0)
	{
		MessageBox(_T("��ѡ��������Χ��лл��"),_T("��ʾ"),MB_ICONINFORMATION);
		return;
	}

	if(m_wndKeyCombo.FindString(-1,strComboText) == CB_ERR )
	{
		this->m_wndKeyCombo.InsertString(0,strComboText);
	}
	DWORD dwThreadID;
	HANDLE hThread = ::CreateThread(NULL, 0, FindComp,(LPVOID)this, 0, &dwThreadID);
	if (hThread)
	{
		CloseHandle(hThread);
		hThread = NULL;
		/*	while( (pParam->m_dStyle & DownloadParam_Data_StartThread) )
		{
		Sleep(10);
		}*/
	}
}


//�����̴߳�����
DWORD WINAPI CDlgFormulaFind::FindComp(LPVOID pParameter)
{
	CDlgFormulaFind* pCompFind = (CDlgFormulaFind*)(pParameter);
	if( pCompFind == NULL)
		return 0;

	CWnd* pWnd = pCompFind->GetDlgItem(IDC_STARTBUTTON);
	if(pWnd != NULL)
	{
		pWnd->EnableWindow(false);
	}
	pWnd = pCompFind->GetDlgItem(IDC_STOPBUT);
	if(pWnd != NULL)
	{
		pWnd->EnableWindow(true);
	}

	pCompFind->m_wndAnimate.Play(0,-1,-1);
	CString strKey ;
	pCompFind->m_wndKeyCombo.GetWindowText(strKey);
	strKey.MakeUpper();
//	DWORD dwFindMask = pCompFind->GetFindMask();
	DWORD dwFindMask = pCompFind->m_nFindMask;
	if( dwFindMask == 0 ) 
		return 0;
	
	CListCtrl* pListCtrl = &(pCompFind->m_wndResultList);
	if( pListCtrl == NULL)
		return 0;

	pListCtrl->DeleteAllItems();
	CMapVariabile* pVarsMap = CDlgFormulaMan::m_pExternExpression;
	if(pVarsMap != NULL)
	{
		POSITION pos = pVarsMap->GetStartPosition();
		pWnd = pCompFind->GetDlgItem(IDC_RESULTSTATIC);

		CString sName;
		CValue *pVal;
		while (pos != NULL && pCompFind->m_bThreadRun)
		{			
			pVarsMap->GetNextAssoc (pos, sName, pVal);

			if(pVal != NULL)
			{
				if( pVal->IsStyle(HX_EXPRESSSENTENCE) )
				{
					CString strFindText = _T("���ڲ��� --");
					strFindText +=  ((CExpValue*)pVal)->GetWholeName();
					pWnd->SetWindowText( strFindText);
					CString strRet;
					if( ((CExpValue*)pVal)->Find(strKey,strRet,dwFindMask) )
					{

						int nCount = pListCtrl->GetItemCount();
						CString strText;
						strText.Format(TEXT("%d"), nCount+1);

						// Insert the item, select every other item.
						CExpression* pExpression = ((CExpValue*)pVal)->GetExp();
						if(pExpression )
						{
							DWORD dwMask = (DWORD)pExpression->GetExpressType();
							dwMask &= 0xf;
							if ((pCompFind->m_dwMask & dwMask) == 0)
							{
								continue;
							}

							pListCtrl->InsertItem(
								LVIF_TEXT|LVIF_STATE, nCount, strText, 
								(nCount%2)==0 ? LVIS_SELECTED : 0, LVIS_SELECTED,
								0, 0);
							pListCtrl->SetItemData(nCount,(DWORD)pExpression);

							int nNameIndex = strRet.Find(_T("��ʽȫ��"));
							int nDescrible = strRet.Find(_T("��ʽ����"));
							if( nDescrible - nNameIndex != 8)
							{
								CString strDescrible = strRet.Mid(
									nNameIndex+8,nDescrible-nNameIndex-8);
								pListCtrl->SetItemText(nCount, 2,  strDescrible);
							}

							CString strWholeName = strRet.Left(nNameIndex);
							pListCtrl->SetItemText(nCount, 1, strWholeName.Mid(1,nNameIndex));

							CString strGroup;
							switch( dwMask )
							{
							case Tech :
								{
									strGroup = CTreeCtrlFormula::g_strTech;
									break;
								}
							case Exchange :
								{

									strGroup = CTreeCtrlFormula::g_strExchange;
									break;
								}
							case Condition :
								{
									strGroup = CTreeCtrlFormula::g_strCondition;
									break;
								}
							case MoreKLine :
								{
									strGroup = CTreeCtrlFormula::g_strColorKLine;
									break;
								}
							case Strategy : //20090302 YJT ���� Strategy
								{
									strGroup = CTreeCtrlFormula::g_strStrategy;
									break;
								}
							default :
								{
									return 0;
								}
							}

							pListCtrl->SetItemText(nCount, 3, strGroup);
						}
					}
				}		
			}
		}

		CString strFindText ;
		strFindText.Format(_T("�ҵ� %d ������"),pListCtrl->GetItemCount());
		pWnd->SetWindowText( strFindText);
		pWnd = pCompFind->GetDlgItem(IDC_STARTBUTTON);

		if(pWnd != NULL)
		{
			pWnd->EnableWindow(true);
		}
		pWnd = pCompFind->GetDlgItem(IDC_STOPBUT);
		if(pWnd != NULL)
		{
			pWnd->EnableWindow(false);
		}
		pCompFind->m_wndAnimate.Stop();

	}
	return 0;
}

BOOL CDlgFormulaFind::ShowWindow( int nCmdShow )
{

	if( !::IsWindow(this->m_hWnd) )
		return 0;

	if( nCmdShow == SW_HIDE || nCmdShow == SW_MINIMIZE )
		return CHSBaseDialog::ShowWindow(nCmdShow);


	if( IsIconic() )
	{
		return CHSBaseDialog::ShowWindow(SW_RESTORE);
	}

	if(  IsZoomed() ) // max
		return CHSBaseDialog::ShowWindow(nCmdShow);

	if( ::AnimateWindow(m_hWnd,100,AW_CENTER | AW_ACTIVATE) )
	{
		this->Invalidate();
	}
	else
	{
		return CHSBaseDialog::ShowWindow(nCmdShow);
	}

	return 1;
}
void CDlgFormulaFind::OnBnClickedStopbut()
{
	// TODO: Add your control notification handler code here
	m_bThreadRun = false;
}

void CDlgFormulaFind::OnOK()
{
	//WRITE_LOG ::OnOK();
}

void CDlgFormulaFind::OnCancel()
{

	//ShowWindowEx(WM_QUIT);

	//WRITE_LOG ::OnCancel();
	//delete this;

	this->DestroyWindow();

}

void CDlgFormulaFind::PostNcDestroy()
{
	delete this;
	if (g_pDlgFormulaFind!=NULL)
	{
		g_pDlgFormulaFind = NULL;
	}
}

//�õ����ҷ�Χ
short CDlgFormulaFind::GetFindMask(void)
{
	UpdateData();

	short nFindMask = 0;
	if(m_bCompAll     ) nFindMask |= FindInAll;
	if(m_bCompDiscribe) nFindMask |= FindInDescribe;
	if(m_bCompEdolon  ) nFindMask |= FindInEidolon;
	if(m_bCompName    ) nFindMask |= FindInName;
	if(m_bCompSource  ) nFindMask |= FindInSource;
	if(m_bCompNotes   ) nFindMask |= FindInNotes;

	this->m_nFindMask = nFindMask;

	return nFindMask;

}

void CDlgFormulaFind::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	OnBnClickedStartbutton();	
}

void CDlgFormulaFind::OnLvnItemActivateResultlist(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMIA = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	DWORD dwData = m_wndResultList.GetItemData(pNMIA->iItem);
	//��˫���ڣ��ߴ���ʾ��Ϣ
	//˫��ָ�� ��ָ�귢�ͻ��ⲿ
	if(m_pParentWnd != NULL)
	{
		m_pParentWnd->SendMessage(HX_USER_DRAGDATA,
			HSCurKeyInfo::UpdateExpressData/*HX_USER_DRAGDATA-1*/, (LPARAM)dwData);

		if( !m_pParentWnd->IsZoomed() && !m_pParentWnd->IsIconic() )
		{
			CRect rect,rcDes,rcThis;
			CWnd* pDesWnd = GetDesktopWindow();
			if( pDesWnd )
			{
				pDesWnd->GetWindowRect(rcDes);
			}
			m_pParentWnd->GetWindowRect(rect);
			this->GetWindowRect(rcThis);
			if( rect.right + rcThis.Width() > rcDes.right )
				rect.right = rcDes.right - rcThis.Width();
			this->SetWindowPos(NULL,rect.right,rcThis.top,0,0,SWP_NOSIZE);
		}
		//m_pParentWnd->SetFocus();
	}
	else
	{
		CExpression* pExpression = (CExpression*)dwData;
		ExpPropery NameProp;
		NameProp.m_dExpType = pExpression->GetExpressType();
		NameProp.m_strName.Format("%s", pExpression->GetName());
		NameProp.m_strDescribe.Format("%s",  pExpression->GetDescribe());
		NameProp.m_bStyle = pExpression->GetStyle();		
		if ( g_pFormulaMan )
			g_pFormulaMan->SendLinkMsg(DiagramMsg_FormulaParamChanged, (LPARAM)&NameProp);
	}
	*pResult = 0;
}

BOOL CDlgFormulaFind::PreTranslateMessage(MSG* pMsg)
{
	
	if( pMsg->message == WM_KEYDOWN )
	{
		CWnd* pWndFocus = this->GetFocus();
		if( ::IsWindow(m_wndKeyCombo.m_hWnd) &&
			(m_wndKeyCombo.IsChild(pWndFocus) ||
			pWndFocus == &m_wndKeyCombo) &&
			pMsg->wParam == VK_RETURN )
		{
			OnBnClickedStartbutton();
			return 1;
		}
		if (pMsg->wParam == VK_ESCAPE)
		{
			CDlgFormulaFind::bEscMsgOfDlgFind = TRUE;
		}
	}

	return CHSBaseDialog::PreTranslateMessage(pMsg);
}

//yangdl 2008.05.19 ȫѡ�ͷ�ѡ����
void CDlgFormulaFind::OnBnClickedCompall()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
    m_bCompAll = (!m_bCompAll);
	if(m_bCompAll )
	{
		m_bCompDiscribe = false;
		m_bCompEdolon = false;
		m_bCompName = false;
		m_bCompSource = false;
		m_bCompNotes = false;
	}
	UpdateData(false);
}

void CDlgFormulaFind::OnBnClickedCompname()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
    m_bCompName = (!m_bCompName);
	if(m_bCompName)
		m_bCompAll = FALSE;
	UpdateData(false);
}

void CDlgFormulaFind::OnBnClickedCompdiscribe()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_bCompDiscribe = (!m_bCompDiscribe);
	if(m_bCompDiscribe)
		m_bCompAll = FALSE;
	UpdateData(false);
}

void CDlgFormulaFind::OnBnClickedCompnotes()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_bCompNotes = (!m_bCompNotes);
	if(m_bCompNotes)
		m_bCompAll = FALSE;
	UpdateData(false);
}

void CDlgFormulaFind::OnBnClickedCompedolon()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_bCompEdolon = (!m_bCompEdolon);
	if(m_bCompEdolon)
		m_bCompAll = FALSE;
	UpdateData(false);
}

void CDlgFormulaFind::OnBnClickedCompsource()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
    m_bCompSource = (!m_bCompSource);
	if(m_bCompSource)
		m_bCompAll = FALSE;
	UpdateData(false);
}
