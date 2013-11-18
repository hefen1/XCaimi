/*******************************************************
  源程序名称:FundRiskTestDlg.cpp
  软件著作权:恒生电子股份有限公司
  系统名称:  投资赢家金融理财终端V1.0
  模块名称:  交易
  功能说明:  基金风险测试
  作    者:  shenglq
  开发日期:  20110425
*********************************************************/
#include "stdafx.h"
#include "FundRiskTestDlg.h"
#include "TradeConst.h"
#include "TradeFunc.h"
#include "TradeAction.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define  IDC_GROUP_BEGIN		1000
#define  IDC_GROUP_END			1099
#define  IDC_RADIO_BEGIN		1100
#define  IDC_RADIO_END			1999
#define  TESTRESULT "尊敬的客户：\r\n根据您所提供的回答，您的总分是（%d）分。\r\n您的投资风险承受能力评估结果为（%s）。\r\n确定要提交吗？"

CMyGroupBox::CMyGroupBox()
{
	m_nRadioHeight = 20;
}

CMyGroupBox::~CMyGroupBox()
{
	for(int i = 0; i < m_RadioList.GetCount(); i ++)
	{
		CButton* pBtn = m_RadioList.GetAt(i);
		delete pBtn;
	}
	m_RadioList.RemoveAll();
	for(int i = 0; i < m_QuestionItemList.GetCount(); i ++)
	{
		IFundRiskQuestionItem* pItem = m_QuestionItemList.GetAt(i);
		pItem->Release();
	}
	m_QuestionItemList.RemoveAll();
}

void CMyGroupBox::OnSize()
{

}
void CMyGroupBox::NewRadio(CString sCaption, UINT nID)
{
	CFont* font = GetFont();
	CButton* pBtn = new CButton();
	int nCount = GetRadioCount();
	int t = 20 + m_nRadioHeight * nCount;
	pBtn->Create(sCaption, WS_CHILD|WS_VISIBLE|BS_AUTORADIOBUTTON, CRect(10, t, 400, t + m_nRadioHeight), this, nID);
	if (nCount == 0)
	{
		pBtn->SetCheck(1);
	}
	pBtn->SetFont(font);
	m_RadioList.Add(pBtn);
}

void CMyGroupBox::AdjustRadio()
{
	CRect rcTmp;
	GetClientRect(rcTmp);
	int nWith = rcTmp.Width(); 
	for (int i = 0; i < GetRadioCount(); i++)
	{
		CButton* pBtn = m_RadioList.GetAt(i);
		pBtn->MoveWindow(10, 20 + i * m_nRadioHeight, nWith - 10 * 2, m_nRadioHeight);
	}
}

void CMyGroupBox::AdjustGroupWidth(int nWidth)
{
	this->MoveWindow(10, 10, nWidth - 10 * 2, 20*2 + GetRadioCount() * m_nRadioHeight);
	AdjustRadio();
}

int CMyGroupBox::GetScore()
{
	int nScore = 0;
	for (int i = 0; i < m_RadioList.GetCount(); i++)
	{
		CButton* pBtn = m_RadioList.GetAt(i);
		if (pBtn->GetCheck() == 1)
		{
		 	IFundRiskQuestionItem* pItem = m_QuestionItemList.GetAt(i);
			nScore = pItem->GetChoiceScore();
			break;
		}
	}
	return nScore;
}

IMPLEMENT_DYNAMIC(CFundRiskTestDlg, CTradeDialogBase)

CFundRiskTestDlg::CFundRiskTestDlg(CWnd* pParent /* = NULL */)
: CTradeDialogBase(CFundRiskTestDlg::IDD, pParent)
{
	m_pQuestionList = NULL;
	m_pFundAccRiskList = NULL;
	m_nCurrIndex= -1;
}

CFundRiskTestDlg::~CFundRiskTestDlg()
{
	if (m_pQuestionList)
	{
		m_pQuestionList->Release();
	}
	if (m_pFundAccRiskList)
	{
		m_pFundAccRiskList->Release();
	}

	for (int i = 0; i < m_MyGroupBoxList.GetCount(); i++)
	{
		CMyGroupBox* pGroupBox = m_MyGroupBoxList.GetAt(i);
		delete pGroupBox;
	}
	m_MyGroupBoxList.RemoveAll();
}

void CFundRiskTestDlg::DoDataExchange(CDataExchange* pDX)
{
	__super::DoDataExchange(pDX);	
	DDX_Control(pDX, IDC_BT_NEXT, m_btNext);
	DDX_Control(pDX, IDC_BT_OK, m_btOK);
	DDX_Control(pDX, IDC_BT_BACK, m_btBack);
}

BEGIN_MESSAGE_MAP(CFundRiskTestDlg, CTradeDialogBase)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BT_NEXT, &CFundRiskTestDlg::OnBnClickedBtNext)
	ON_BN_CLICKED(IDC_BT_OK, &CFundRiskTestDlg::OnBnClickedBtOk)
	ON_BN_CLICKED(IDC_BT_BACK, &CFundRiskTestDlg::OnBnClickedBtBack)
END_MESSAGE_MAP()
void CFundRiskTestDlg::OnBnClickedBtNext()
{
	// TODO: Add your control notification handler code here
  CMyGroupBox* pMyGroupbox = m_MyGroupBoxList.GetAt(m_nCurrIndex);
  pMyGroupbox->ShowWindow(SW_HIDE);
  m_nCurrIndex++;
  pMyGroupbox = m_MyGroupBoxList.GetAt(m_nCurrIndex);
  pMyGroupbox->ShowWindow(SW_SHOW);
  if ((m_MyGroupBoxList.GetCount() - 1) == m_nCurrIndex)
  {
	  m_btNext.EnableWindow(FALSE);
	  m_btOK.EnableWindow();
  }   
  else
	  m_btBack.EnableWindow();
  AdjustButton();
}

void CFundRiskTestDlg::OnBnClickedBtOk()
{
	// TODO: Add your control notification handler code here
	int nScore = 0;
	CString sFundAccRisk = "0";
	CString sFundAccRiskName = "";
	for (int i = 0; i < m_MyGroupBoxList.GetCount(); i++)
	{
		CMyGroupBox* pGroupBox = m_MyGroupBoxList.GetAt(i);
		nScore += pGroupBox->GetScore();
	}
	for (int i = 0; i < m_pFundAccRiskList->GetCount(); i++)
	{
		INodeData* pItem = m_pFundAccRiskList->GetItems(i);
		CString sScore = pItem->ValueEx();
		sFundAccRisk = pItem->Value();
		sFundAccRiskName = pItem->Name();
		int nTmp = atoi(sScore);
		if (nScore <= nTmp)
		{
			break;
		}
		pItem->Release();
	}
	CString sMsg = "";	
	sMsg.Format(TESTRESULT, nScore, sFundAccRiskName);
	if(MessageBox(sMsg,"提示",MB_ICONQUESTION|MB_OKCANCEL) == IDOK)
	{
		m_pBusinessLogic->FundSetRiskLevel(sFundAccRisk, sMsg);
		MessageBox(sMsg, "提示", MB_ICONINFORMATION);
	}
	
}

BOOL CFundRiskTestDlg::OnInit()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	if (__super::OnInit())
	{
		m_pFundAccRiskList = m_pAccountParamList->GetFundAccRiskTypes();
		m_pQuestionList = m_pAccountParamList->GetFundRiskQuestionList();
		CString sCode = "";
		CMyGroupBox* pMyGroupbox = NULL;
		int j = 0;
		CFont* font = this->GetFont();
		for (int i = 0; i < m_pQuestionList->GetCount(); i++)
		{
			IFundRiskQuestionItem* pItem = m_pQuestionList->GetItems(i);
			CString sCaption = pItem->GetChoice() + "、" + pItem->GetChoiceText();
			CString sCode1 = pItem->GetCode();			
			if (sCode1 != sCode)
			{
				pMyGroupbox = new CMyGroupBox();
				m_MyGroupBoxList.Add(pMyGroupbox);
				sCode = sCode1;
				pMyGroupbox->SetCode(sCode);
				//pMyGroupbox->SetRadioHeight(20);
				CString sTitle = pItem->GetTitle();
				pMyGroupbox-> Create(sTitle, WS_CHILD|BS_GROUPBOX, CRect(10, 10, 500, 200),this,IDC_GROUP_BEGIN + j); 
				pMyGroupbox->SetFont(font);
				j++;
			}
			pMyGroupbox->NewRadio(sCaption, IDC_RADIO_BEGIN + i);
			pMyGroupbox->AddItem(pItem);
		}
		if (m_MyGroupBoxList.GetCount() > 0)
		{
			pMyGroupbox = m_MyGroupBoxList.GetAt(0);
			pMyGroupbox->ShowWindow(SW_SHOW);
			m_nCurrIndex = 0;
		}
		m_btBack.EnableWindow(FALSE);
		if (m_MyGroupBoxList.GetCount() == 1)
		{
			m_btOK.EnableWindow(TRUE);
			m_btNext.EnableWindow(FALSE);
		}
		else
		{
			m_btOK.EnableWindow(FALSE);
			m_btNext.EnableWindow(TRUE);
		}
		AdjustButton();
		return TRUE;
	}
	else
		return FALSE;
}

void CFundRiskTestDlg::OnSize(UINT nType, int cx, int cy)
{
	CRect rcTmp;
	GetClientRect(rcTmp);
	int nWith = rcTmp.Width(); 
	for (int i = 0 ; i < m_MyGroupBoxList.GetCount(); i++ )
	{
		CMyGroupBox* pMyGroupbox = m_MyGroupBoxList.GetAt(i);
		pMyGroupbox->AdjustGroupWidth(nWith);
		AdjustLayoutEx(&m_btBack, pMyGroupbox, 0, 10, adxBottomLeft);
	}	
}
void CFundRiskTestDlg::OnBnClickedBtBack()
{
	// TODO: Add your control notification handler code here
	CMyGroupBox* pMyGroupbox = m_MyGroupBoxList.GetAt(m_nCurrIndex);
	pMyGroupbox->ShowWindow(SW_HIDE);
	m_nCurrIndex--;
	pMyGroupbox = m_MyGroupBoxList.GetAt(m_nCurrIndex);
	pMyGroupbox->ShowWindow(SW_SHOW);
	if (0 == m_nCurrIndex)
	{
		m_btBack.EnableWindow(FALSE);
	}  
	else
		m_btNext.EnableWindow();
	m_btOK.EnableWindow(FALSE);
	AdjustButton();
}

void CFundRiskTestDlg::AdjustButton()
{
	//CMyGroupBox* pMyGroupbox = m_MyGroupBoxList.GetAt(m_nCurrIndex);
	//CRect rcTmp;
	//m_btBack.GetClientRect(rcTmp);
	//int nWith = rcTmp.Width() + 30;
	//int x = 100; 
	//AdjustLayoutEx(pMyGroupbox, &m_btBack, x, 10, adxBottomLeft);
	//x += nWith;
	//AdjustLayoutEx(pMyGroupbox, &m_btNext, x, 10, adxBottomLeft);
	//x += nWith;
	//AdjustLayoutEx(pMyGroupbox, &m_btOK, x, 10, adxBottomLeft);
}
