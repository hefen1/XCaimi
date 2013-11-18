// DlgWatcherParams.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DiagramCombAnalysis.h"
#include "DlgWatcherParams.h"
#include "GeneralHelper.h"


// CDlgWatcherParams �Ի���

IMPLEMENT_DYNAMIC(CDlgWatcherParams, CDialog)

CDlgWatcherParams::CDlgWatcherParams(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgWatcherParams::IDD, pParent)
{
	m_pWatcherInfo = NULL;
}

CDlgWatcherParams::~CDlgWatcherParams()
{
}

void CDlgWatcherParams::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_WATCHER_NAME, m_edtWatcherName);
	DDX_Control(pDX, IDC_RICHEDIT_FORMULA, m_rcedtFormula);
	DDX_Control(pDX, IDC_STATIC_PROPERTY_POS, m_stPropertyPos);
	DDX_Control(pDX, IDC_BTN_INDICATOR, m_btnMenu);
}


BEGIN_MESSAGE_MAP(CDlgWatcherParams, CDialog)
	ON_WM_SIZE()
	ON_WM_CREATE()
	ON_BN_CLICKED(IDC_BTN_INDICATOR, &CDlgWatcherParams::OnBnClickedBtnIndicator)
	ON_BN_CLICKED(IDC_BTN_OK, &CDlgWatcherParams::OnBnClickedBtnOk)
	ON_BN_CLICKED(IDC_BTN_CANCLE, &CDlgWatcherParams::OnBnClickedBtnCancle)
	ON_REGISTERED_MESSAGE(AFX_WM_PROPERTY_CHANGED,&CDlgWatcherParams::OnPropertyChanged)
END_MESSAGE_MAP()


// CDlgWatcherParams ��Ϣ�������

BOOL CDlgWatcherParams::OnInitDialog()
{
	CDialog::OnInitDialog();
	if(m_pWatcherInfo && m_pWatcherInfo->GetName().IsEmpty() == FALSE)
	{
		m_edtWatcherName.SetWindowText(m_pWatcherInfo->GetName());
		m_edtWatcherName.SetReadOnly(TRUE);
		m_edtWatcherName.EnableWindow(FALSE);
	}
	AdjustLayout();
	InitPropStyle();
	InitInidicatorBtnMenu();
	FillParamsProperty();
	return FALSE; 
}

void CDlgWatcherParams::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);
}

void CDlgWatcherParams::AdjustLayout()
{
	if (GetSafeHwnd() == NULL || m_stPropertyPos.GetSafeHwnd() == NULL )
	{
		return;
	}
	CRect rectPos;
	m_stPropertyPos.GetWindowRect(&rectPos);
	ScreenToClient(&rectPos);
	m_wndPropList.SetWindowPos(NULL,rectPos.left,rectPos.top,rectPos.Width(),rectPos.Height(), SWP_NOACTIVATE );
}

void CDlgWatcherParams::InitPropStyle()
{
	m_wndPropList.EnableHeaderCtrl(FALSE);
	m_wndPropList.EnableDescriptionArea();
	m_wndPropList.SetVSDotNetLook();
	m_wndPropList.MarkModifiedProperties();
}

void CDlgWatcherParams::InitInidicatorBtnMenu()
{
	m_indicatorMenu.CreatePopupMenu();
	m_indicatorMenu.AppendMenu(MF_STRING | MF_BYPOSITION,1,_T("���в���"));
	for( int i = 0; i < m_pAnalysisTable->GetColumnCount(); i ++ )
	{
		IAnalysisColumn* pCol =	 m_pAnalysisTable->GetColumn(i);
		m_indicatorMenu.AppendMenu(MF_STRING | MF_BYPOSITION,i+2,pCol->GetDispName());
	}
	m_btnMenu.m_hMenu = m_indicatorMenu.GetSafeHmenu();
	m_btnMenu.SizeToContent();
	m_btnMenu.m_bOSMenu = FALSE;
}

void CDlgWatcherParams::FillParamsProperty(CString strExpName /*= _T("")*/)
{
	if(m_pAnalysisTable == NULL)
		return;

	m_wndPropList.RemoveAll();

	CMapStringToString mapAllParams,mapParams;
	CString strGroupName = _T("��ʽ����");
	CMFCPropertyGridProperty* pGroup1 = new CMFCPropertyGridProperty(strGroupName);
	for( int i = 0; i < m_pAnalysisTable->GetColumnCount(); i ++ )
	{
	    IAnalysisColumn* pCol =	 m_pAnalysisTable->GetColumn(i);
		IExpressionInfo* pExpInfo = pCol->GetExpInfo();
		if(pExpInfo == NULL)
			continue;
		if( (!strExpName.IsEmpty()) && (pExpInfo->GetName() != strExpName)) //�����Ʋ�Ϊ��ʱ���������ƹ���
			continue;
		
		for(int i = 0; i <pExpInfo->GetParamCount(); i++ )
		{
			IExpParamInfo* pExpParam = pExpInfo->GetParamInfo(i);
			CString strParam = pExpParam->GetName();
			CString strNote = pExpParam->GetNote();
			IExpParamInfo* pTempExpParam = NULL;

			//��¼�����Ĳ�����Ϣ
			if( !m_watherParams.Lookup(strParam,(void*&)pTempExpParam))
			{
				m_watherParams.SetAt(strParam,(void*)pExpParam);
			}

			//��¼�����Ĳ�����Ϣ
			if( !mapAllParams.Lookup(strParam,strNote) )
			{
				mapAllParams.SetAt(strParam,strNote);
				CString strParamValue;
				IParamInfo* p = m_pWatcherInfo->GetParamInfo(strParam);
				if(p)
					strParamValue = p->GetParamValue();
				CMFCPropertyGridProperty* pProp = new CMFCPropertyGridProperty(strParam,strParamValue ,strNote);
				pGroup1->AddSubItem(pProp);	
			}
		}	
	}
	m_wndPropList.AddProperty(pGroup1);
}

int CDlgWatcherParams::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rectDummy;
	rectDummy.SetRectEmpty();
	if (!m_wndPropList.Create(WS_VISIBLE | WS_CHILD, rectDummy, this, 2))
	{
		TRACE0("δ�ܴ�����������\n");
		return -1;      // δ�ܴ���
	}
	return 0;
}

void CDlgWatcherParams::OnBnClickedBtnIndicator()
{
	UpdateWatchInfo();

	CString  strMenuString;
	m_indicatorMenu.GetMenuString(m_btnMenu.m_nMenuResult,strMenuString,MF_BYCOMMAND); 
	m_btnMenu.SetWindowText(strMenuString);
	for(int i = 0 ; i < m_pAnalysisTable->GetColumnCount();i++)
	{
		IAnalysisColumn* pCol  = m_pAnalysisTable->GetColumn(i);
		CString strDispName = pCol->GetDispName();
		if( strDispName.Compare(strMenuString) == 0 )
		{
			//������
			FillParamsProperty(pCol->GetName());
			//��乫ʽ����:��ʽ��������Щ���У��ո������¹���
			IExpressionInfo* pExpInfo = pCol->GetExpInfo();
			if(pExpInfo)
			{
				CString strExp = pExpInfo->GetExpression();
				strExp.Trim();
				CStringArray ayRet;
				CGeneralHelper::AnalyseStr(strExp,_T("\n\r"),ayRet);
				CString strDisp;
				for(int j = 0; j < ayRet.GetCount(); j++)
				{
					CString strTemp = ayRet.GetAt(j);
					strTemp.Trim();
					if( !strTemp.IsEmpty() )
					{
						strDisp += strTemp;
						strDisp += _T("\r");
					}
				}
				m_rcedtFormula.SetWindowText(strDisp);
			}	
			return;
		}
	}

	//ѡ����ȫ��
	FillParamsProperty();
	m_rcedtFormula.SetWindowText(_T(""));
}


LRESULT CDlgWatcherParams::OnPropertyChanged(WPARAM,LPARAM lParam)
{
	CMFCPropertyGridProperty* pProp = (CMFCPropertyGridProperty*)lParam;
	CString strName = pProp->GetName();
	CString strValue( pProp->GetValue().bstrVal);
	OutputDebugString(strValue);
	return 0;
}

void CDlgWatcherParams::OnBnClickedBtnOk()
{
	CString strWatchName;
	m_edtWatcherName.GetWindowText(strWatchName);
	if(strWatchName.IsEmpty())
	{
		MessageBox(_T("������Ʋ���Ϊ��"),_T("����"),MB_OK);
		return;
	}

	if( UpdateWatchInfo() )
		CDialog::OnOK();
}

void CDlgWatcherParams::OnBnClickedBtnCancle()
{
	CDialog::OnCancel();
}

BOOL CDlgWatcherParams::UpdateWatchInfo()
{
	UpdateData(TRUE);

	CString strWatchName;
	m_edtWatcherName.GetWindowText(strWatchName);
	m_pWatcherInfo->SetName(strWatchName);

	for( int i = 0; i < m_wndPropList.GetPropertyCount(); i++)
	{
		CMFCPropertyGridProperty* pProp = m_wndPropList.GetProperty(i);
		for( int j =0; j < pProp->GetSubItemsCount(); j++)
		{
			CMFCPropertyGridProperty* pSub = pProp->GetSubItem(j);

			CString strParamName = pSub->GetName(); 
			IExpParamInfo* pTempExpParam = NULL;
			if( m_watherParams.Lookup(strParamName,(void*&)pTempExpParam))
			{
				CString strTemp ( pSub->GetValue().bstrVal );
				m_pWatcherInfo->UpdateParam(strParamName,pTempExpParam->GetType(),strTemp);
			}
		}
	}
	return TRUE;
}
