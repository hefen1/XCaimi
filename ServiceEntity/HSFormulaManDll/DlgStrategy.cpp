// DlgStrategy.cpp : 实现文件
//

#include "stdafx.h"
#include "DlgStrategy.h"
#include "DlgFormulaMan.h"
#include "DlgFormulaFind.h"
#include "DlgFormulaEdit.h"
#include "DlgNotesAndEidolon.h"
#include "CompileEditView.h"
#include "dlgCondition.h"
#include ".\dlgstrategy.h"
#include "..\..\BizEntity\Quote\QuoteComm\pageid.h"
#include "PubStruct.h"
#include "VarValue.h"
#include "GeneralHelper.h"
// CDlgStrategy 对话框
extern HWND g_hParentWnd;
IMPLEMENT_DYNAMIC(CDlgStrategy, CDialog)
CDlgStrategy::CDlgStrategy(CWnd* pParent /*=NULL*/,HSOpenExpressUseData* pData /*= NULL*/)
	: CDialog(CDlgStrategy::IDD, pParent)
{
	m_pData = pData;
	m_hParent = g_hParentWnd;
	m_pTrig = NULL;
}

CDlgStrategy::~CDlgStrategy()
{
}

void CDlgStrategy::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_DATETIMEPICKERStart, m_wndEffectStart);
	DDX_Control(pDX, IDC_DATETIMEPICKEREnd, m_wndEffectEnd);
	DDX_Control(pDX, IDC_COMBOCYCLE, m_wndPeriodType);
	DDX_Control(pDX, IDC_EDITDAY, m_wndPeriodNum);
	DDX_Control(pDX, IDC_CHECKFUQ, m_wndFuQ);
	DDX_Control(pDX, IDC_EDITBUY, m_wndMaxBuy);
	DDX_Control(pDX, IDC_EDITSELL, m_wndMaxSell);
	DDX_Control(pDX, IDC_EDITGAP, m_wndGap);
	DDX_Control(pDX, IDC_TABSTRATEGY, m_wndTab);
	
	DDX_Control(pDX, IDD_CONNOTES, m_wndConNotes);
	DDX_Control(pDX, IDD_CON12, m_wndCon);
	DDX_Control(pDX, IDC_STATIC_CON12, m_wndStaticCon);

	DDX_Control(pDX, IDD_COMPARE12, m_wndCompare);
	DDX_Control(pDX, IDD_DATA112, m_wndData2);
	DDX_Control(pDX, IDD_AND_OR1, m_wndAndOr);
	DDX_Control(pDX, IDD_DATA212, m_wndData1);
	DDX_Control(pDX, IDC_MIDDLE2, m_wndMiddle);
}


BEGIN_MESSAGE_MAP(CDlgStrategy, CDialog)
	ON_MESSAGE(HX_USER_LOADCUR, LoadCurExp)
	ON_CBN_SELCHANGE(IDC_COMBOCYCLE, OnCbnSelchangeCombocycle)
END_MESSAGE_MAP()


// CDlgStrategy 消息处理程序
BOOL CDlgStrategy::OnInitDialog()
{
	CDialog::OnInitDialog();
	
	LPSYSTEMTIME time=new SYSTEMTIME;
	ZeroMemory(time,sizeof(SYSTEMTIME));
	time->wYear = 1990;
	time->wMonth = 1;
	time->wDay = 1;
	time->wHour = 9;
	time->wMinute = 30;
	m_wndEffectStart.SetTime(time);
	time->wHour = 15;
	time->wMinute = 0;
	m_wndEffectEnd.SetTime(time);

	m_wndPeriodType.SetCurSel(0);
	m_wndPeriodNum.SetWindowText("");

	m_EditEidolon.CreateEx(WS_EX_CLIENTEDGE,NULL,NULL,WS_CHILD|WS_VISIBLE|WS_TABSTOP|WS_BORDER,
		CRect(0,0,0,0),this,1001);
	m_EditEidolon.SetFont( GetFont() );

	CRect rect;
	CWnd* pWnd = GetDlgItem(IDC_PARAM);
	pWnd->GetWindowRect(&rect);
	ScreenToClient(&rect);
	m_EditEidolon.MoveWindow(rect);

	CTreeCtrlFormula* pCurTree = m_wndTab.GetTree();
	if( pCurTree )
	{
		CDlgFormulaMan* pDlg = (CDlgFormulaMan*)AfxGetMainWnd();

		pCurTree->ModifyStyleEx(0,WS_EX_CLIENTEDGE);
		pCurTree->SetImageList(&CDlgFormulaMan::m_expressImages, TVSIL_NORMAL);
		pCurTree->m_pFormulaMan = pDlg;

		if( m_pData != NULL )
		{
			WORD nType = ((m_pData->m_wType & 0xFF) & Strategy);
			int nCopy = CTreeCtrlFormula::CopyNone;
			CTreeCtrlFormula* pTree;
			for( int i = 0; i < 5; i++ ) //20090302 YJT 修改
			{
				switch(i)
				{
				case 0:
					nType = ((m_pData->m_wType & 0xF) & Tech);
					break;
				case 1:
					nType = ((m_pData->m_wType & 0xF) & Condition);
					break;
				case 2:
					nType = ((m_pData->m_wType & 0xF) & Exchange);
					break;
				case 3:
					nType = ((m_pData->m_wType & 0xF) & MoreKLine);
					break;
				case 4:
					nType = ((m_pData->m_wType & 0xFF) & Strategy);
					break;
				}

				pTree = pDlg->GetCurTree( nType );
				pCurTree->CopyTree(pTree,nCopy,m_pData->m_dStyle);
			}
			SetParam();
		}

	}
	return TRUE;
}

BOOL CDlgStrategy::GetTrigData(StrategyTrig* pTrig, BOOL IsByDay/* = FALSE*/)
{
	if( m_pData == NULL )
		goto _Exit;
	CDlgFormulaMan* pParentDlg  = (CDlgFormulaMan*)AfxGetMainWnd();
	if( pParentDlg == NULL )
		goto _Exit;

	CTreeCtrlFormula* pCurTree = m_wndTab.GetTree();
	if( !pCurTree )
		goto _Exit;
	HTREEITEM hItem = pCurTree->GetSelectedItem();
	if( hItem == NULL )
		goto _Exit;

	CTreeGroup* pTreeGroup = (CTreeGroup*)pCurTree->GetItemData(hItem);
	if( (pTreeGroup == NULL)			|| 
		(pTreeGroup->GetData() == NULL) ||
		!pTreeGroup->IsStatus(hx_GroupItem) )
		goto _Exit;

	if (pTrig)
	{
		CTime time;
		m_wndEffectStart.GetTime(time);
		pTrig->m_lStartTime = time.GetHour()*100 + time.GetMinute();
		m_wndEffectEnd.GetTime(time);
		pTrig->m_lEndTime = time.GetHour() * 100 + time.GetMinute();
		CString strTemp;
		m_wndPeriodNum.GetWindowText(strTemp);
		if (m_wndPeriodType.GetCurSel() == 8 && (strTemp.GetLength() <= 0 || strTemp == "0"))
		{
			MessageBox("数据周期错误，请重新填写多日周期！",_T("提示"),MB_ICONINFORMATION);
			m_wndPeriodType.SetFocus();
			return FALSE;
		}
		else
		{
			pTrig->m_nPeriodType = m_wndPeriodType.GetCurSel() + 1;
			pTrig->m_nPeriodNumber = atoi(strTemp);
		}
		if (IsByDay)
		{
			if (pTrig->m_nPeriodType >= AnalisysForDay)
			{
				MessageBox("每日分析周期必须是日线以内！","策略分析");
				m_wndPeriodType.SetFocus();
				return FALSE;
			}
		}
		m_wndMaxBuy.GetWindowText(strTemp);
		if (strTemp.GetLength() <= 0 /*|| atoi(strTemp) < -1*/)
		{
			MessageBox("买入委托次数限制错误，请重新填写买入委托次数限制！",_T("提示"),MB_ICONINFORMATION);
			m_wndMaxBuy.SetFocus();
			return FALSE;
		}
		else
		{
			pTrig->m_nMaxBuyTimes = atoi(strTemp);
		}

		m_wndMaxSell.GetWindowText(strTemp);
		if (strTemp.GetLength() <= 0 /*|| atoi(strTemp) < -1*/)
		{
			MessageBox("卖出委托次数限制错误，请重新填写卖出委托次数限制！",_T("提示"),MB_ICONINFORMATION);
			m_wndMaxSell.SetFocus();
			m_wndMaxSell.SetSel(strTemp.GetLength());
			return FALSE;
		}
		else
		{
			pTrig->m_nMaxSellTimes = atoi(strTemp);
		}

		m_wndGap.GetWindowText(strTemp);
		if (strTemp.GetLength() <= 0 /*|| atoi(strTemp) < -1*/)
		{
			MessageBox("连续委托间隔错误，请重新填写连续委托间隔！",_T("提示"),MB_ICONINFORMATION);
			m_wndGap.SetFocus();
			m_wndGap.SetSel(strTemp.GetLength());
			return FALSE;
		} 
		else
		{
			pTrig->m_nGap = atoi(strTemp);
		}

		if (m_wndFuQ.GetCheck() == TRUE)
		{
			pTrig->m_bFuQuan = 0;
		} 
		else
		{
			pTrig->m_bFuQuan = 1;
		}

		//公式
		if (!pTrig->m_pExpValue)
		{
			pTrig->m_pExpValue = new CExpValue;
		}
		//只保存公式名和变量
		CExpression *pNew;
		if (!pTrig->m_pExpValue->GetExp())
		{
			pNew = new CExpression;
		}
		else
			pNew = pTrig->m_pExpValue->GetExp();
		CExpression *pTreeExp = pTreeGroup->GetData();
		if (pTreeExp)//复制参数值
		{
			CMapVariabile *pMv = pTreeExp->GetParamVar();
			//----------------------------------------------
			if (pMv)
			{
				CMapVariabile *pMap = new CMapVariabile;
				CString key;
				CValue *cvalue;
				POSITION pos;
				pos = pMv->GetStartPosition();
				while(pos)
				{
					pMv->GetNextAssoc(pos,key,cvalue);
					CNumericValue *pnum = new CNumericValue;
					double db = cvalue->GetValue();
					pnum->SetValue(db);
					pnum->SetStyle(cvalue->GetStyle());
					pnum->SetMaxValue(((CNumericValue*)cvalue)->GetMaxValue());
					pnum->SetMinValue(((CNumericValue*)cvalue)->GetMinValue());
					pnum->SetTestStepValue(((CNumericValue*)cvalue)->GetTestStepValue());
					pnum->SetDescribe(((CNumericValue*)cvalue)->GetDescribe());
					pMap->SetAt(key,pnum);
				}
				//===============================================================
				pNew->SetParamVar(pMap);//存储变量值
			}
			pNew->SetName(pTreeExp->GetName());//存储公式名
		}
		pTrig->m_pExpValue->SetValue(pNew);
		return TRUE;
	}
_Exit:
	::MessageBox( m_hWnd,_T(" 请选择一个公式! "),_T(" 提示 "), MB_OK);
	return FALSE;
}
void CDlgStrategy::SetTrigData(StrategyTrig* pTrig)
{
	if (!pTrig)
	{
		return;
	}
	if (!m_pTrig)
	{
		m_pTrig = new StrategyTrig;
	}
	m_pTrig->m_bFuQuan = pTrig->m_bFuQuan;
	strncpy(m_pTrig->m_cName,pTrig->m_cName,32);
	//strncpy(m_pTrig->m_cExplain,pTrig->m_cExplain,64);
	m_pTrig->m_lEndTime = pTrig->m_lEndTime;
	m_pTrig->m_lStartTime = pTrig->m_lStartTime;
	m_pTrig->m_nGap = pTrig->m_nGap;
	m_pTrig->m_nMaxBuyTimes = pTrig->m_nMaxBuyTimes;
	m_pTrig->m_nMaxSellTimes = pTrig->m_nMaxSellTimes;
	m_pTrig->m_nPeriodNumber = pTrig->m_nPeriodNumber;
	m_pTrig->m_nPeriodType = pTrig->m_nPeriodType;
	m_pTrig->m_nReserve = pTrig->m_nReserve;
	m_pTrig->m_pExpValue = pTrig->m_pExpValue;
	SetParam();
}
long CDlgStrategy::LoadCurExp(WPARAM wParam /*= 0*/, LPARAM lParam /*= 0*/)
{	
	CDlgFormulaMan* pParentDlg  = (CDlgFormulaMan*)AfxGetMainWnd();
	if( pParentDlg == NULL )
		return 0;

	EnableItem(FALSE);

	if( wParam == CCompileEditView::UpdateExpress )
	{
		CDlgFormulaEdit* pDlg = pParentDlg->GetExpDlg();
		if( pDlg != NULL )
		{
			if(pDlg->SendMessage(HX_USER_LOADCUR,wParam,lParam))
			{
				pParentDlg->Enable(FALSE);
				pDlg->ShowWindow(SW_SHOWNORMAL);
			}
		}
	}
	else if( wParam == HX_EXPRESSSENTENCE )
	{
		CExpression* pExpression = (CExpression*)lParam;
		if( pExpression == NULL )
			return 0;
		CString csName = pExpression->GetName();
		if (m_pTrig && m_pTrig->m_pExpValue)
		{
			CString cExpName = m_pTrig->m_pExpValue->GetExp()->GetName();
			if (csName == cExpName)
			{
				CExpression *pex = m_pTrig->m_pExpValue->GetExp();
				if (pex)//设置参数
				{
					CMapVariabile *pMap = pExpression->GetParamVar();
					CMapVariabile *pMv = pex->GetParamVar();
					//--------------------------------------------------
					if (pMv && pMap)
					{
						CString key;
						CValue *cvalue;
						CValue *pval;
						POSITION pos;
						pos = pMv->GetStartPosition();
						while(pos)
						{
							pMv->GetNextAssoc(pos,key,cvalue);
							if (pMap->Lookup(key,pval))
							{
								((CNumericValue*)pval)->SetValueOnly(cvalue->GetValue());
							}
						}
						//=====================================================================
					}
				}
			}
		}
		CString cs = pExpression->GetName();
		m_EditEidolon.ShowWindow(SW_SHOW);

		CDlgNotesAndEidolon* pDlgNotes = pParentDlg->GetDlgNotes();
		CDlgNotesAndEidolon* pDlgParam = pParentDlg->GetDlgParam();
		if( pDlgNotes != NULL && 
			pDlgNotes->IsWindowVisible() )
		{
			pDlgNotes->SetNotes(pExpression->GetNotes(), pExpression->GetName());
		}

		if( m_EditEidolon.m_hWnd != NULL )
		{		
			CString strText;
			strText.Format("%s",pExpression->GetEidolon());
			if( !strText.IsEmpty() )
			{
				m_EditEidolon.SetText(strText,pExpression->GetParamVar(),FALSE);
			}
			else
			{
				CStringArray* pArray = pExpression->GetDefEidolon();
				if(pArray->GetSize() <= 1)
				{
					pArray = NULL;
				}
				m_EditEidolon.SetText(pArray,pExpression->GetParamVar(),FALSE);
			}
			m_EditEidolon.Invalidate();
		}
	}

	return 0;
}

void CDlgStrategy::EnableItem(BOOL bEnable)
{
	m_wndConNotes.ShowWindow(bEnable);
	m_wndCon.ShowWindow(bEnable);
	m_wndStaticCon.ShowWindow(bEnable);

	m_wndCompare.ShowWindow(bEnable);
	m_wndData1.ShowWindow(bEnable);
	m_wndAndOr.ShowWindow(bEnable);
	m_wndData2.ShowWindow(bEnable);
	m_wndMiddle.ShowWindow(bEnable);

	m_EditEidolon.ShowWindow(bEnable);
}

void CDlgStrategy::SetParam()
{
	CTreeCtrlFormula* pCurTree = m_wndTab.GetTree();
	if (!m_pTrig)
	{
		pCurTree->Show(NULL,0);
		return;
	}
	CString cs;
	//--------------------------------------------
	switch(m_pTrig->m_nPeriodType)
	{
	case AnalisysFor1:
		cs = "1分钟";
		break;
	case AnalisysFor5:
		cs = "5分钟";
		break;
	case AnalisysFor15:
		cs = "15分钟";
		break;
	case AnalisysFor30:
		cs = "30分钟";
		break;
	case AnalisysFor60:
		cs = "60分钟";
		break;
	case AnalisysForDay:
		cs = "日线";
		break;
	case AnalisysForWeek:
		cs = "周线";
		break;
	case AnalisysForMonth:
		cs = "月线";
		break;
	case AnalisysForMoreDay:
		cs = "多日线";
		break;
	default:
		cs = "日线";
		break;
	}
	if (cs == "多日线")
	{
		m_wndPeriodNum.EnableWindow(TRUE);
		CString str;
		str.Format("%d",m_pTrig->m_nPeriodNumber);
		m_wndPeriodNum.SetWindowText(str);
	}
	else
	{
		m_wndPeriodNum.EnableWindow(FALSE);
		m_wndPeriodNum.SetWindowText(NULL);
	}
	m_wndPeriodType.SelectString(-1,cs);
	//--------------------------------------------------
	cs.Format("%d",m_pTrig->m_nMaxBuyTimes);
	m_wndMaxBuy.SetWindowText(cs);
	cs.Format("%d",m_pTrig->m_nMaxSellTimes);
	m_wndMaxSell.SetWindowText(cs);
	cs.Format("%d",m_pTrig->m_nGap);
	m_wndGap.SetWindowText(cs);
	LPSYSTEMTIME time=new SYSTEMTIME;
	ZeroMemory(time,sizeof(SYSTEMTIME));
	time->wYear = 1990;
	time->wMonth = 1;
	time->wDay = 1;
	time->wHour = m_pTrig->m_lStartTime / 100;
	time->wMinute = m_pTrig->m_lStartTime % 100;
	m_wndEffectStart.SetTime(time);
	time->wHour = m_pTrig->m_lEndTime / 100;
	time->wMinute = m_pTrig->m_lEndTime % 100;
	m_wndEffectEnd.SetTime(time);
	delete time;
	if (m_pTrig->m_bFuQuan == 0)
	{
		m_wndFuQ.SetCheck(TRUE);
	}
	else
		m_wndFuQ.SetCheck(FALSE);
	CArray<CTreeGroup*,CTreeGroup*>* pTreeGp = pCurTree->GetTreeGroup();
	if(m_pTrig->m_pExpValue && m_pTrig->m_pExpValue->GetExp())
	{
		CString cExpName = m_pTrig->m_pExpValue->GetExp()->GetName();
		int sel = 0;
		for (; sel<pTreeGp->GetCount(); sel++)
		{
			CTreeGroup *pGp = pTreeGp->GetAt(sel);
			CString csName = pGp->GetName();
			if (pGp && csName == cExpName)
			{
				break;
			}
		}
		if (sel < pTreeGp->GetCount())
		{
			CTreeGroup *ptg = pTreeGp->GetAt(sel);					
			pCurTree->Show(ptg->GetData(),0);
			SendMessage(HX_USER_LOADCUR,HX_EXPRESSSENTENCE,(LPARAM)ptg->GetData());
		}
		else
		{
			pCurTree->Show(NULL,0);
		}
	}
	else
	{
		pCurTree->Show(NULL,0);
	}
}

void CDlgStrategy::SetRDOnly(BOOL ro/*=TRUE*/)
{
	m_wndEffectEnd.EnableWindow(!ro);
	m_wndEffectStart.EnableWindow(!ro);
	m_wndFuQ.EnableWindow(!ro);
	m_wndGap.SetReadOnly(ro);
	m_wndMaxBuy.SetReadOnly(ro);
	m_wndMaxSell.SetReadOnly(ro);
	m_wndPeriodNum.SetReadOnly(ro);
	m_wndPeriodType.EnableWindow(!ro);
	//m_wndTab.EnableWindow(!ro);
}
void CDlgStrategy::OnCbnSelchangeCombocycle()
{
	CString cs;
	m_wndPeriodType.GetWindowText(cs);
	if (cs == "多日线" || cs.IsEmpty())
	{
		m_wndPeriodNum.SetWindowText(NULL);
		m_wndPeriodNum.EnableWindow(TRUE);
	}
	else
	{
		m_wndPeriodNum.SetWindowText(NULL);
		m_wndPeriodNum.EnableWindow(FALSE);
	}
}
void CDlgStrategy::Serialize(CArchive & ar,CExpValue *& exp)
{
	CExpression *pex = exp->GetExp();
	if (!pex)
	{
		pex = new CExpression;
	}
	if (ar.IsStoring())
	{
		CString sname = pex->GetName();
		ar<<sname;
		if(pex->GetParamVar() != NULL)
		{
			CArray<class CValue*,class CValue*>* pArray = pex->GetParamVar()->GetObjArray();
			if( pArray != NULL )
			{
				ar << pArray->GetSize();

				CValue* pCurVal;	
				CString strName;
				for(int nObj = 0; nObj < pArray->GetSize(); nObj++)
				{
					pCurVal = pArray->GetAt(nObj);
					strName = pex->GetParamVar()->GetObjectName(pCurVal);
					ar << strName;
					pCurVal->Serialize(ar);
				}
			}
			else
			{
				ar << 0;
			}
		}
		else
		{
			ar << 0;
		}

	}
	else
	{
		CString sname;
		int nCountVar;
		ar >> sname;
		pex->SetName(sname);
		ar >> nCountVar;
		for(short i = 0; i < nCountVar;i++)
		{
			CString sName;
			CNumericValue *pVal = new CNumericValue();
			ar >> sName;
			pVal->Serialize(ar);
			pex->AddParamer(sName,pVal);
		}
		exp->SetValue(pex);
	}
}
BOOL CDlgStrategy::SaveAllTrig(CMapWordToPtr *m_map)
{
	CFile file;
	CString path;
	path = CGeneralHelper::GetAppPath() + "StrategyTrade\\";
	if (_access(path, 0) != 0)
	{
		_mkdir(path);
	}
	path += "StrategyTrig.dat";
	if (!file.Open(path,CFile::modeCreate|CFile::modeWrite))
	{
		return FALSE;
	}
	CArchive ar(&file,CArchive::store);
	WORD key;
	void *val;
	StrategyTrig *Trig;
	POSITION pos = m_map->GetStartPosition();
	ar<<m_map->GetCount();
	while (pos)
	{
		m_map->GetNextAssoc(pos,key,val);
		Trig = (StrategyTrig*)val;
	
		char name[21]={0}, explain[65]={0};
		CString sname;
		strncpy(name,Trig->m_cName,20);
		sname = name;
		//strncpy(explain,Trig->m_cExplain,64);
		ar<<sname;
		//ar<<explain;
		ar<<Trig->m_bFuQuan;
		ar<<Trig->m_lStartTime;
		ar<<Trig->m_lEndTime;
		ar<<Trig->m_nGap;
		ar<<Trig->m_nMaxBuyTimes;
		ar<<Trig->m_nMaxSellTimes;
		ar<<Trig->m_nPeriodType;
		ar<<Trig->m_nPeriodNumber;
		ar<<Trig->m_nReserve;
		Serialize(ar,Trig->m_pExpValue);
	}
	return TRUE;
}
BOOL CDlgStrategy::IsEqual(StrategyTrig *trig1,StrategyTrig *trig2)
{
	if (//(strncmp(trig1->m_cExplain, trig2->m_cExplain,64) == 0)&& 
		 trig1->m_lStartTime == trig2->m_lStartTime
		&& trig1->m_lEndTime == trig2->m_lEndTime
		&& trig1->m_nPeriodType == trig2->m_nPeriodType
		&& trig1->m_nPeriodNumber == trig2->m_nPeriodNumber
		&& trig1->m_nMaxBuyTimes == trig2->m_nMaxBuyTimes
		&& trig1->m_nMaxSellTimes == trig2->m_nMaxSellTimes
		&& trig1->m_nGap == trig2->m_nGap
		&& trig1->m_bFuQuan == trig2->m_bFuQuan
		)
	{
		if (trig1->m_pExpValue && trig2->m_pExpValue)
		{
			if(trig1->m_pExpValue->GetExp() && trig2->m_pExpValue->GetExp())
			{
				if (trig1->m_pExpValue->GetExp()->GetName()==trig2->m_pExpValue->GetExp()->GetName())//同一个公式
				{
					CMapVariabile *pmap = trig1->m_pExpValue->GetExp()->GetParamVar();
					CMapVariabile *ptmap = trig2->m_pExpValue->GetExp()->GetParamVar();
					if (!pmap && !ptmap)//两个都没参数
					{
						return TRUE;
					}
					else if (!pmap || !ptmap)//有一个有参数
					{
						return FALSE;
					}
					CString key;
					CValue *cvalue, *ctval;
					POSITION pos;
					pos = pmap->GetStartPosition();
					while(pos)
					{
						pmap->GetNextAssoc(pos,key,cvalue);
						if (ptmap->Lookup(key,ctval) 
							&& (cvalue->GetValue() == ctval->GetValue()))//参数相等
						{
							;
						}
						else
						{
							return FALSE;
						}
					}
					return TRUE;
				}
			}
		}
	}
	return FALSE;
}

WORD CDlgStrategy::GetStrategyTrigKey(CMapWordToPtr *&m_map,char* TrigName)
{
	POSITION pos = m_map->GetStartPosition();
	WORD nTemp = 0;
	void* pTemp = NULL;
	while(pos)
	{
		m_map->GetNextAssoc(pos,nTemp,pTemp);
		if (!strncmp(((StrategyTrig*)pTemp)->m_cName,TrigName,32))
		{
			return nTemp;
		}
	}
	return 0;
}
WORD CDlgStrategy::AddStrategyTrig(CMapWordToPtr *&m_map,void* pTrig)
{
	POSITION pos = m_map->GetStartPosition();
	WORD nTemp = 1;
	void* pTemp = NULL;
	while(m_map->Lookup(nTemp,pTemp))
	{
		nTemp++;
	}
	m_map->SetAt(nTemp,pTrig);
	return nTemp;
}
BOOL CDlgStrategy::DelStrategyTrig(CMapWordToPtr *&m_map,WORD TrigID)
{
	void* pTemp = NULL;
	m_map->Lookup(TrigID,pTemp);
	if (pTemp == NULL)
	{
		return FALSE;
	}
	else
	{
		delete (StrategyTrig*)pTemp;
		m_map->RemoveKey(TrigID);
		return TRUE;
	}
}

void* CDlgStrategy::GetStrategyTrig(CMapWordToPtr *&m_map,char* TrigName)
{
	POSITION pos = m_map->GetStartPosition();
	WORD nTemp = 0;
	void* pTemp = NULL;
	while(pos)
	{
		m_map->GetNextAssoc(pos,nTemp,pTemp);
		if (!strncmp(((StrategyTrig*)pTemp)->m_cName,TrigName,32))
		{
			return pTemp;
		}
	}
	return NULL;
}

BOOL CDlgStrategy::PreTranslateMessage(MSG* pMsg) 
{ 
	if(pMsg->message == WM_KEYDOWN) 
	{ 
		if(pMsg->wParam == VK_RETURN ) 
		{ 
			UINT nID = GetFocus()->GetDlgCtrlID(); 
			switch( nID) 
			{ 
		/*	default: */
				NextDlgCtrl(); 
			} 
			return TRUE;
		}
		else if (pMsg->wParam == VK_TAB)
		{
			NextDlgCtrl();
			return TRUE;
		}	
	} 
	return CDialog::PreTranslateMessage(pMsg);
} 