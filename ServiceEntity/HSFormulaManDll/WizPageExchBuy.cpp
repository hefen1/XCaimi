// WizPageExchBuy.cpp : implementation file
//

#include "stdafx.h"

#include "WizPageExchBuy.h"
#include "SheetExchMain.h"
#include "WizPageExchSell.h"
// CWizPageExchBuy dialog


CWizPageExchBuy::CWizPageExchBuy(CWnd* pParent /*=NULL*/)
	: CWizPageExchBase(CWizPageExchBuy::IDD, pParent)
{
	m_pData = &m_TExchangeBuy;
}

CWizPageExchBuy::~CWizPageExchBuy()
{
}

void CWizPageExchBuy::DoDataExchange(CDataExchange* pDX)
{
	CWizPageExchBase::DoDataExchange(pDX);
	DDX_Control(pDX, IDT_TIEMSTART, m_wndTimeBegin);
	DDX_Control(pDX, IDT_TIMEEND, m_wndTimeEnd);
	DDX_Control(pDX, IDC_COUNTIN, m_wndButtonDecount);
	DDX_Control(pDX, IDC_COMCOND, m_wndConditionA);
	DDX_Control(pDX, IDC_COMCON1, m_wndConditionR);
	DDX_Control(pDX, IDC_COMCON2, m_wndConditionB1);
	DDX_Control(pDX, IDC_COMCON3, m_wndConditionB2);
	DDX_Control(pDX, IDC_STATIC10, m_wndStaticRelation);
	DDX_Control(pDX, IDC_COMUSE, m_wndBuyMethod);
	DDX_Control(pDX, IDC_STAITC6, m_wndStaticBuyMethod);
	DDX_Control(pDX, IDC_COMFOLLOW, m_wndAddMethod);
	DDX_Control(pDX, IDC_STATICDE, m_wndStaticAddMethod);
	DDX_Control(pDX, IDC_INPUTCOUNT, m_wndEditBuyMount);
	DDX_Control(pDX, IDC_DECOUNT, m_wndEditAddMount);
	DDX_Control(pDX, IDC_SKIP, m_wndCheckSkip);
	DDX_Control(pDX, IDC_SKIPCOUNT, m_wndEditSkipPeriod);
	DDX_Control(pDX, IDC_STATIC4, m_wndStaticMiddle);
}


BEGIN_MESSAGE_MAP(CWizPageExchBuy, CWizPageExchBase)
	ON_CBN_SELCHANGE(IDC_COMCON1, OnCbnSelchangeComcon1)
	ON_CBN_SELCHANGE(IDC_COMUSE, OnCbnSelchangeComuse)
	ON_CBN_SELCHANGE(IDC_COMFOLLOW, OnCbnSelchangeComfollow)
	ON_CBN_SELCHANGE(IDC_COMCOND, OnCbnSelchangeComcond)
	ON_CBN_SELCHANGE(IDC_COMCON2, OnCbnSelchangeComcon2)
	ON_CBN_KILLFOCUS(IDC_COMCON2, OnCbnKillfocusComcon2)
END_MESSAGE_MAP()


// CWizPageExchBuy message handlers

void CWizPageExchBuy::OnCbnSelchangeComcon1()
{
	// TODO: Add your control notification handler code here
	int nIndex = m_wndConditionR.GetCurSel();
	if( nIndex == -1 ) 
		return;

	ShowBuyWin(nIndex);
	return;
}

BOOL CWizPageExchBuy::OnInitDialog()
{
	CWizPageExchBase::OnInitDialog();

	const CTime tMin(1978,1,1,0,0,0);
	const CTime tMax(2038,1,1,0,0,0);

	m_wndTimeBegin.SetFormat(_T("yyyy��MM��dd�� dddd"));
	m_wndTimeBegin.SetRange(&tMin, &tMax);
	m_wndTimeEnd.SetFormat(_T("yyyy��MM��dd�� dddd"));
	m_wndTimeEnd.SetRange(&tMin, &tMax);

	// �Ƿ��б��������
	if( m_TExchangeBuy.m_lDateCount == 0 )
	{
		struct tm *stm;
		time_t	lTime;
		time(&lTime);
		stm = localtime(&lTime);

		CTime	sTiem(stm->tm_year+1900-1, stm->tm_mon+1, stm->tm_mday, 0, 0, 0);
		m_wndTimeBegin.SetTime(&sTiem);
	}
	else
	{
		CTime	sTiem(m_TExchangeBuy.m_lBeginDate/10000, 
			m_TExchangeBuy.m_lBeginDate/100%100, 
			m_TExchangeBuy.m_lBeginDate%100, 
			0, 0, 0);
		m_wndTimeBegin.SetTime(&sTiem);

		CTime	sTemp(m_TExchangeBuy.m_lEndDate/10000, 
			m_TExchangeBuy.m_lEndDate/100%100, 
			m_TExchangeBuy.m_lEndDate%100, 
			0, 0, 0);
		m_wndTimeEnd.SetTime(&sTemp);
	}

	InitCondition();
	
	return TRUE; 
}

// ��似��ָ��
void CWizPageExchBuy::FullTech()
{
	m_wndConditionA.ResetContent();

	CSheetExchMain* pWnd = (CSheetExchMain*)GetParent();
	if( pWnd == NULL )
		return;
	
	CExpression* pExpression = pWnd->m_pMainTech.GetExp();
	CValue* valoare = NULL;
	if( pExpression == NULL )
		return;

	if( CExpression::m_pExternExpression->Lookup(pExpression->GetName(),pExpression->GetExpressType(),valoare) ) // over
	{
		CExpValue* pExpValue = (CExpValue*)valoare;

		BOOL bFree = FALSE;
		CExpression* pCurExpression = pExpValue->GetExp();
		if( pCurExpression && pCurExpression == pExpression )
		{
			if( !pCurExpression->IsCompile() )
			{
				if( !pCurExpression->Compile() )
				{
					bFree = TRUE;
				}
			}
			CArray<CFormulaDrawParam*,CFormulaDrawParam*> ayDraw;
			CString strParam;
			int nCount = pCurExpression->GetDraw(strParam,ayDraw);
			CFormulaDrawParam* pParam;
			if( nCount > 0 )
			{
				for(int i = 0; i < nCount; i++)
				{
					pParam = ayDraw.GetAt(i);
					if( nCount == 1 && CString(pParam->m_strName).IsEmpty() )
					{
						m_wndConditionA.AddString(CTreeCtrlFormula::g_strFirstTechLine);
					}
					else
					{
						//yulx modify ȥ������λ��0
						CString name = pParam->m_strName;
						name = name.Left(name.GetLength() - 3);
						m_wndConditionA.AddString(name/*pParam->m_strName*/);
					}
					delete pParam;
				}
			}
			else
			{
				m_wndConditionA.AddString(CTreeCtrlFormula::g_strFirstTechLine);
			}
		}
		if( bFree )
		{
			pExpValue->Free();
		}
	}				
}

void CWizPageExchBuy::SetSelectData(CString& str, CComboBox& wndCom)
{
	long lCount = wndCom.GetCount();
	CString strTemp;

	if( str.IsEmpty() && wndCom.GetDlgCtrlID() == IDC_COMCOND )
	{
		if( wndCom.GetCount() > 0 )
			wndCom.SetCurSel(0);
		return;
	}

	for( int i=0; i<lCount; i++ )
	{
		wndCom.GetLBText(i, strTemp);
		if( strTemp.Compare(str) == 0 )
		{
			wndCom.SetCurSel(i);
			return;
		}
	}
	
	if( str.IsEmpty() || wndCom.GetDlgCtrlID() == IDC_COMCOND )
	{
		if( wndCom.GetCount() > 0 )
			wndCom.SetCurSel(0);
	}
	else
	{
		wndCom.SetWindowText(str);
	}
}

// ��ʾ����
void CWizPageExchBuy::PlayData()
{
	m_wndButtonDecount.SetCheck(m_TExchangeBuy.m_bNoCountIn);
	SetSelectData(m_TExchangeBuy.m_strTechLeft, m_wndConditionA);
	SetSelectData(m_TExchangeBuy.m_SubData.m_dData1, m_wndConditionB1);
	SetSelectData(m_TExchangeBuy.m_SubData.m_dData2, m_wndConditionB2);
	m_wndConditionR.SetCurSel((int)m_TExchangeBuy.m_SubData.m_cType);
	m_wndBuyMethod.SetCurSel((int)m_TExchangeBuy.m_nBuyMethod);
	CString str;
	str.Format("%.2f", m_TExchangeBuy.m_fOneBuyCount);
	m_wndEditBuyMount.SetWindowText(str);
	m_wndAddMethod.SetCurSel((int)m_TExchangeBuy.m_nFollowBuyMethod);
	str.Format("%.2f", m_TExchangeBuy.m_fAddOrSubCount);
	m_wndEditAddMount.SetWindowText(str);
	m_wndCheckSkip.SetCheck(m_TExchangeBuy.m_bSkip);
	str.Format("%d", m_TExchangeBuy.m_nInPeriod);
	m_wndEditSkipPeriod.SetWindowText(str);
}

void CWizPageExchBuy::InitCondition()
{
	// ��似��ָ��
	FullTech();
	SetSelectData(m_TExchangeBuy.m_strTechLeft, m_wndConditionA);

	// �����������Ƚ�
	ShowBuyWin((int)m_TExchangeBuy.m_SubData.m_cType);

	// �������뷽��
	ShowBuyMedthWin(m_TExchangeBuy.m_nBuyMethod);

	// ��ʾ����
	PlayData();
}

BOOL CWizPageExchBuy::OnKillActive(int nPos /*= -1*/)
{
	if(nPos < 0) return TRUE;

	UpdateData();
	BOOL breturn;
	breturn =  SetData();
	// SaveOption();

	return breturn;
}

CExpression* CWizPageExchBuy::GetExp()
{
	CSheetExchMain* pWnd = (CSheetExchMain*)this->GetParent();
	if( pWnd == NULL )
	{
		return NULL;
	}
	return pWnd->m_pMainTech.GetExp();
}

void CWizPageExchBuy::OnSetActive()
{
	CSheetExchMain* pParent = (CSheetExchMain*)GetParent();
	if( pParent != NULL )
	{
		CString str(_T("ϵͳ����-"));
		CExchCalcMain* pData = pParent->GetData();
		
		if( pData != NULL )
		{
			CExpression *pExp = pData->GetExp();
			if( pExp != NULL )
			{
				str += pExp->GetName();
				str += "-";
			}
		}
		str += _T("��������");
		pParent->SetWindowText(str);
	}
	InitCondition();
}

void CWizPageExchBuy::OnCbnSelchangeComuse()
{
	int nIndex = m_wndBuyMethod.GetCurSel();
	if( nIndex == -1 ) 
		return;

	ShowBuyMedthWin(nIndex);

	return;
}

void CWizPageExchBuy::OnCbnSelchangeComfollow()
{
	int nIndex = m_wndAddMethod.GetCurSel();
	if( nIndex == -1 ) 
		return;

	ShowLSWin(nIndex);

	return;
}

BOOL CWizPageExchBuy::SetData()
{
	m_TExchangeBuy.m_bNoCountIn =  m_wndButtonDecount.GetCheck();
	m_TExchangeBuy.m_bSkip      =  m_wndCheckSkip.GetCheck();

	CString str;
	CTime timeDest;
	m_wndTimeEnd.GetTime(timeDest);
	str.Format("%04d%02d%02d",timeDest.GetYear(),timeDest.GetMonth(),timeDest.GetDay());
	m_TExchangeBuy.m_lEndDate = atol(str);

	CTimeSpan endSpan(timeDest.GetTime());

	m_wndTimeBegin.GetTime( timeDest );
	str.Format("%04d%02d%02d",timeDest.GetYear(),timeDest.GetMonth(),timeDest.GetDay());
	m_TExchangeBuy.m_lBeginDate = atol(str);

	CTimeSpan beginSpan(timeDest.GetTime());
	
	
	CTimeSpan countSpan = endSpan - beginSpan;
	m_TExchangeBuy.m_lDateCount = countSpan.GetDays();
	if( m_TExchangeBuy.m_lDateCount <= 0 )
	{
		MessageBox(_T("����ʱ�䳤�Ȳ���Ϊ0!"), FORMULAMAN_EXCH_PROMPT, MB_OK);
		return FALSE;
	}

	m_wndEditSkipPeriod.GetWindowText(str);
	int	tem = atoi(str);

	if( m_TExchangeBuy.m_bSkip )
	{
		if(str.IsEmpty())
		{
			MessageBox(_T("����д��������"), FORMULAMAN_EXCH_PROMPT, MB_OK);
			return FALSE;
		}
		if(tem < 0)
		{
			MessageBox(_T("�������ڱ������0"), FORMULAMAN_EXCH_PROMPT, MB_OK);
			return FALSE;
		}
		m_TExchangeBuy.m_nInPeriod = tem;
	}
	
	m_wndEditAddMount.GetWindowText(str);
	float ftem = atof(str);
	m_TExchangeBuy.m_fAddOrSubCount = ftem;

	m_wndEditBuyMount.GetWindowText(str);
	ftem = atof(str);
	m_TExchangeBuy.m_fOneBuyCount = ftem;

	m_TExchangeBuy.m_nBuyMethod = m_wndBuyMethod.GetCurSel();

	m_TExchangeBuy.m_nFollowBuyMethod = m_wndAddMethod.GetCurSel();

	CExpression* pExp = GetExp();
	if( pExp == NULL )
	{
		MessageBox(_T("��ʽ���ݲ���ȷ!"), FORMULAMAN_EXCH_PROMPT, MB_OK);
		return FALSE;
	}
	char chExpType = pExp->GetExpressType();
		//|| chExpType == CExpression::MoreKLine
		//|| chExpType == CExpression::Condition )
	if( chExpType == Exchange )
	{
		m_TExchangeBuy.m_SubData.m_cType = FormulaCompareSubData::Exchange;
		m_TExchangeBuy.m_SubData.m_dData1 = "EXITLONG";
		m_TExchangeBuy.m_SubData.m_dData2 = "";
		m_TExchangeBuy.m_strTechLeft = "ENTERLONG";
	}
	if( chExpType == Strategy ) //20090302 YJT ���� Strategy
	{
		m_TExchangeBuy.m_SubData.m_cType = FormulaCompareSubData::Strategy;
		m_TExchangeBuy.m_SubData.m_dData1 = "SELL";
		m_TExchangeBuy.m_SubData.m_dData2 = "";
		m_TExchangeBuy.m_strTechLeft = "BUY";
	}
	else if( chExpType == MoreKLine )
	{
		m_TExchangeBuy.m_SubData.m_cType = FormulaCompareSubData::MoreKLine;
		m_wndConditionB1.GetWindowText(str);
		m_TExchangeBuy.m_SubData.m_dData1 = str;
		m_wndConditionB2.GetWindowText(str);
		m_TExchangeBuy.m_SubData.m_dData2 = str;
		m_wndConditionA.GetWindowText(m_TExchangeBuy.m_strTechLeft);
	}
	else if( chExpType == Condition )
	{
		m_TExchangeBuy.m_SubData.m_cType = FormulaCompareSubData::Condition;
		m_wndConditionB1.GetWindowText(str);
		m_TExchangeBuy.m_SubData.m_dData1 = str;
		m_wndConditionB2.GetWindowText(str);
		m_TExchangeBuy.m_SubData.m_dData2 = str;
		m_wndConditionA.GetWindowText(m_TExchangeBuy.m_strTechLeft);
	}
	else
	{
		m_TExchangeBuy.m_SubData.m_cType = m_wndConditionR.GetCurSel();
		m_wndConditionB1.GetWindowText(str);
		m_TExchangeBuy.m_SubData.m_dData1 = str;
		m_wndConditionB2.GetWindowText(str);
		m_TExchangeBuy.m_SubData.m_dData2 = str;
		m_wndConditionA.GetWindowText(m_TExchangeBuy.m_strTechLeft);
	}
	
	return SaveOption();
}

void CWizPageExchBuy::ShowLastLsWin(int nCmd)
{
	if( nCmd != SW_SHOW )
	{
		CString str;
	}
	m_wndStaticAddMethod.ShowWindow(nCmd);
	m_wndEditAddMount.ShowWindow(nCmd);
}

void CWizPageExchBuy::ShowLSWin(int nIdx)
{
	switch( nIdx )
	{
	case FollowBuyMethod_NO:// ��������
	case FollowBuyMethod_DL:// ��������
		ShowLastLsWin(SW_HIDE);
		break;
	case FollowBuyMethod_DJ:// �ݼ�����
		ShowLastLsWin(SW_SHOW);
		m_wndStaticAddMethod.SetWindowText(_T("���ϴεݼ��ٷ�֮"));
		break;
	case FollowBuyMethod_DZ:// ��������
		ShowLastLsWin(SW_SHOW);
		m_wndStaticAddMethod.SetWindowText(_T("���ϴε����ٷ�֮"));
		break;
	}
	EnableSkipWin(nIdx);
}

void CWizPageExchBuy::ShowLastBuyWin(int nCmd)
{
	m_wndConditionB2.ShowWindow(nCmd);
	m_wndStaticMiddle.ShowWindow(SW_HIDE);
	m_wndStaticRelation.ShowWindow(nCmd);
}

void CWizPageExchBuy::ShowBuyWin(int nIdx)
{
	CExpression* pExp = GetExp();
	if( pExp != NULL )
	{
		if( pExp->GetExpressType() != Tech )
		{
			CArray<CFormulaDrawParam*,CFormulaDrawParam*> ayDraw;
			CString strParam;
			int nCount = pExp->GetDraw(strParam,ayDraw);
			CFormulaDrawParam* pParam;
			if( nCount > 1 )
			{
				pParam = ayDraw.GetAt(0);
				m_TExchangeBuy.m_strTechLeft = pParam->m_strName;
				pParam = ayDraw.GetAt(1);
				m_TExchangeBuy.m_SubData.m_dData1 = pParam->m_strName;
			}
			else
			{
				m_TExchangeBuy.m_strTechLeft = "ENTERLONG";
				m_TExchangeBuy.m_SubData.m_dData1 = "EXITLONG";
			}
			m_wndConditionA.ShowWindow(SW_HIDE);
			m_wndConditionR.ShowWindow(SW_HIDE);
			m_wndConditionB1.ShowWindow(SW_HIDE);
			ShowLastBuyWin(SW_HIDE);
			return;
		}
		else
		{
			m_wndConditionA.ShowWindow(SW_SHOW);
			m_wndConditionR.ShowWindow(SW_SHOW);
			m_wndConditionB1.ShowWindow(SW_SHOW);
		}
	}
	else
	{
		m_wndConditionA.ShowWindow(SW_SHOW);
		m_wndConditionR.ShowWindow(SW_SHOW);
		m_wndConditionB1.ShowWindow(SW_SHOW);
	}
	
	// ���뷽��(����С�ڵ���)
	m_wndConditionR.SetCurSel(nIdx);

	switch( nIdx )
	{
	case FormulaCompareSubData::ShangChuan:	// �ϴ�
	case FormulaCompareSubData::XiaoPo:		// ����
	case FormulaCompareSubData::DaYu: // >
	case FormulaCompareSubData::XiaoYu: // <		
	case FormulaCompareSubData::DengYu: // =
		{
			ShowLastBuyWin(SW_HIDE);
			FullConOne(1);
		}
		break;
	case FormulaCompareSubData::Condition:
	case FormulaCompareSubData::Exchange:
	case FormulaCompareSubData::MoreKLine:
		{

		}
		break;
	case FormulaCompareSubData::BiaoChi: // ����
		{
			ShowLastBuyWin(SW_SHOW);
			FullConTwo();
			FullConOne(-1);
			m_wndStaticRelation.SetWindowText(_T("���ں�����"));
		}
		break;
	case FormulaCompareSubData::WeiChi:// ά��
		{
			ShowLastBuyWin(SW_SHOW);
			FullConTwo();
			FullConOne(-1);
			m_wndStaticRelation.SetWindowText(_T("���ں��ϴ�"));
		}
		break;
	case FormulaCompareSubData::JieYu:// ����
		{
			ShowLastBuyWin(SW_SHOW);
			m_wndStaticRelation.SetWindowText(_T("��"));
			FullConOne(1);
			FullConTwo(FALSE);
			m_wndStaticMiddle.ShowWindow(SW_SHOW);
		}
	}
}


void CWizPageExchBuy::EnableLSWin(int nIdx)
{
	switch( nIdx )
	{
	default:
	case BuyMethod_All_Buy:// ȫ������
		m_wndAddMethod.EnableWindow(FALSE);
		m_wndEditAddMount.EnableWindow(FALSE);
		m_wndCheckSkip.EnableWindow(FALSE);
		m_wndEditSkipPeriod.EnableWindow(FALSE);
		break;
	case BuyMethod_Portion_Buy:// ��������
	case BuyMethod_Fixation_Money:// �̶��ʽ�����
	case BuyMethod_Fixation_NUM:// �̶���Ʊ������
		m_wndAddMethod.EnableWindow(TRUE);
		m_wndEditAddMount.EnableWindow(TRUE);
		EnableSkipWin(m_wndAddMethod.GetCurSel());
		ShowLSWin(m_wndAddMethod.GetCurSel());
		break;
	}
}

void CWizPageExchBuy::EnableSkipWin(int nIdx)
{
	switch( nIdx )
	{
	case FollowBuyMethod_NO:// ��������	
		m_wndCheckSkip.EnableWindow(FALSE);
		m_wndEditSkipPeriod.EnableWindow(FALSE);
		break;
	case FollowBuyMethod_DJ:// �ݼ�����
	case FollowBuyMethod_DL:// ��������
	case FollowBuyMethod_DZ:// ��������
		m_wndCheckSkip.EnableWindow(TRUE);
		m_wndEditSkipPeriod.EnableWindow(TRUE);
		break;
	}
}

// ����һ�������б�
void CWizPageExchBuy::FullConOne(int nIdx)
{
	m_wndConditionB1.ResetContent();

	if( nIdx == 1 )
	{
		int lCursel = m_wndConditionA.GetCurSel();
		CString strWin;
		
		long lCount = m_wndConditionA.GetCount();
		for( int i = 0; i < lCount; i++ )
		{
			if( i == lCursel )
				continue;
			m_wndConditionA.GetLBText(i, strWin);
			m_wndConditionB1.AddString(strWin);
		}
	}
	m_wndConditionB1.SetWindowText("5");
}

// ���ڶ��������б�
void CWizPageExchBuy::FullConTwo(BOOL bIsTech)
{
	m_wndConditionB2.ResetContent();

	CComboBox *pBox = NULL;
	if( bIsTech )
	{
		pBox = &m_wndConditionA;
	}
	else
	{
		pBox = &m_wndConditionB1;
	}

	CString strTemp;
	pBox->GetWindowText(strTemp);
	long lCursel = pBox->GetCurSel();
	if( pBox->FindString(0, strTemp) < 0 )
	{
		lCursel = -1;
	}

	CString strWin;

	long lCount = pBox->GetCount();
	for( int i = 0; i < lCount; i++ )
	{
		if( lCursel == i )
			continue;
		pBox->GetLBText(i, strWin);
		m_wndConditionB2.AddString(strWin);
	}
	m_wndConditionB2.SetWindowText("5");
}

void CWizPageExchBuy::ShowMedthWin(int nCmd)
{
	CWnd* pWnd = this->GetDlgItem(IDC_STATICINPUT);
	pWnd->ShowWindow(nCmd);
	pWnd = this->GetDlgItem(IDC_STATIC6);
	pWnd->ShowWindow(nCmd);
	m_wndEditBuyMount.ShowWindow(nCmd);
}

void CWizPageExchBuy::ShowBuyMedthWin(int nIdx)
{
	switch( nIdx )
	{
	default:
	case BuyMethod_All_Buy:// ȫ������
		{
			ShowMedthWin(SW_HIDE);
		}
		break;
	case BuyMethod_Portion_Buy:// ��������
		{
			ShowMedthWin(SW_SHOW);
			CWnd* pWnd = this->GetDlgItem(IDC_STATICINPUT);
			if( pWnd )
				pWnd->SetWindowText(_T("ÿ��Ͷ��"));
			pWnd = this->GetDlgItem(IDC_STATIC6);
			if( pWnd )
				pWnd->SetWindowText(_T("%�Ŀ����ʽ�"));
			m_wndEditBuyMount.SetWindowText("30");
		}
		break;
	case BuyMethod_Fixation_Money:// �̶��ʽ�����
		{
			ShowMedthWin(SW_SHOW);
			CWnd* pWnd = this->GetDlgItem(IDC_STATICINPUT);
			if( pWnd )
				pWnd->SetWindowText(_T("ÿ��Ͷ��"));
			pWnd = this->GetDlgItem(IDC_STATIC6);
			if( pWnd )
				pWnd->SetWindowText(_T("Ԫ�ʽ�"));
			m_wndEditBuyMount.SetWindowText("30");
		}
		break;
	case BuyMethod_Fixation_NUM:// �̶���Ʊ������
		{
			ShowMedthWin(SW_SHOW);
			CWnd* pWnd = this->GetDlgItem(IDC_STATICINPUT);
			if( pWnd )
				pWnd->SetWindowText(_T("ÿ������"));
			pWnd = this->GetDlgItem(IDC_STATIC6);
			if( pWnd )
				pWnd->ShowWindow(TRUE);
			pWnd->SetWindowText(_T("��"));
			m_wndEditBuyMount.SetWindowText("30");
		}
		break;
	}
	// ������������
	EnableLSWin(nIdx);
}
void CWizPageExchBuy::OnCbnSelchangeComcond()
{
	int nIdx = m_wndConditionR.GetCurSel();

	switch( nIdx )
	{
	case FormulaCompareSubData::ShangChuan:	// �ϴ�
	case FormulaCompareSubData::XiaoPo:		// ����
	case FormulaCompareSubData::DaYu:			// >
	case FormulaCompareSubData::XiaoYu:		// <		
	case FormulaCompareSubData::DengYu:		// =
		{
			FullConOne(1);
			m_wndConditionB1.SetWindowText("5");
		}
		break;
	case FormulaCompareSubData::Condition:
	case FormulaCompareSubData::Exchange:
	case FormulaCompareSubData::MoreKLine:
		{

		}
		break;
	case FormulaCompareSubData::BiaoChi: // ����
	case FormulaCompareSubData::WeiChi:// ά��
		{
			FullConOne(-1);
			FullConTwo();
		}
		break;
	case FormulaCompareSubData::JieYu:// ����
		{
			FullConOne(1);
			FullConTwo(FALSE);
		}
	}
}

void CWizPageExchBuy::OnCbnSelchangeComcon2()
{
	FullConTwo(FALSE);
}

void CWizPageExchBuy::OnCbnKillfocusComcon2()
{
	OnCbnSelchangeComcon2();
}

/******************************************************************
*	�������ܣ�	��ʼ����������
*	����������	TExchHead* pData : [in] ��ֵ����
*	�� �� ֵ��	BOOL : �ɹ���TRUE����ʧ�ܣ�FALSE��
*	����������	���ظ��ຯ��
*	��    �ߣ�
*	������ڣ�
*	�޸�˵����	
*****************************************************************/
BOOL CWizPageExchBuy::InitialData(TExchHead* pData)
{
	m_TExchangeBuy = *((TExchBuy*)pData);

	return TRUE;
}
