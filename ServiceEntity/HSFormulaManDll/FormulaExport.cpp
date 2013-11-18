// CompExport.cpp: implementation of the CCompExport class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "FormulaExport.h"
#include "RegMsg.h"
#include "resource.h"
#include "CompileEditView.h"
#include "HSDataSource.h"
#include "DlgFormulaMan.h"
#include "DlgFormulaEdit.h"
#include "DlgCondition.h"
#include "DlgFormulaFind.h"  
#include "PubStruct.h"
#include "WinnerApplication.h"
#include "HSDownloadData.h"
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

UINT g_WindowDataExchangeMessage = RegisterWindowMessage(HS_WINDOW_DATA_EXCHANGE);
BOOL g_bShowCmd;
extern HWND g_hParentWnd;
CDlgFormulaMan* g_pFormulaMan = NULL;
extern BOOL g_bIsExeFile;
extern CDlgFormulaFind *g_pDlgFormulaFind;
extern CHSDataSource* g_pHxDataSource;
CFormulaExport* g_HSCompExport;


CWnd* GetMainWnd()
{
	return g_pFormulaMan;
}

CFormulaExport::CFormulaExport(long lExe /*= 0*/)
{
	g_bIsExeFile = lExe;
	m_AyData.RemoveAll();
	m_pHSColor = (IHsColor*)CWinnerApplication::GetObject(OBJ_COLORCFG);
}

CFormulaExport::~CFormulaExport()
{
	if( g_pFormulaMan != NULL)
	{
		g_pFormulaMan->DestroyWindow();
		delete g_pFormulaMan;
		g_pFormulaMan = NULL;
	}
}
void CFormulaExport::initExpress(HWND hwndNotify,long** pThis)
{
	if( g_pFormulaMan != NULL )
	{
		g_pFormulaMan->ShowWindow(SW_SHOWNORMAL);//
		return ;
	}
 	AfxInitRichEdit();
 	AfxEnableControlContainer();

	g_pFormulaMan = new CDlgFormulaMan(NULL,(long*)&g_pFormulaMan);
	CWnd* pWnd = CWnd::FromHandle(g_pFormulaMan->m_pMainWnd->GetMainWndHandle());
	g_pFormulaMan->Create(CDlgFormulaMan::IDD,pWnd);

	SetWindowLong(g_pFormulaMan->m_hWnd,GWL_STYLE,
		WS_CAPTION		  | \
		WS_SYSMENU        | \
		WS_THICKFRAME     | \
		WS_MINIMIZEBOX    | \
		WS_MAXIMIZEBOX    | \
		WS_POPUP);

	if ( g_pFormulaMan )
		g_pFormulaMan->ShowWindow(SW_HIDE);	
}
void CFormulaExport::CloseCompile(HWND hwndNotify)
{
	if(g_pFormulaMan != NULL)
	{
		CWinApp* pApp = AfxGetApp();
		if(pApp != NULL)
		{
			pApp->m_pMainWnd = NULL;
		}
		g_pFormulaMan->Save();
		g_pFormulaMan->DestroyWindow();
	}
}

long CFormulaExport::OpenExpress( WPARAM wParam,LPARAM lParam )
{
	if(lParam == NULL)
	{
		return 0;
	} 
    g_bShowCmd = HIWORD(wParam);
    wParam = LOWORD(wParam);

	if(g_pFormulaMan == NULL)
	{
		long* pThis = new long;
		*pThis = 0;
		OpenCompile(NULL,&pThis);
		delete pThis;
	}
	if(g_pFormulaMan != NULL)
	{
		CDlgFormulaMan* pMain = (CDlgFormulaMan*)g_pFormulaMan;

		if( pMain != NULL )
		{
			if (wParam == 1)
			{
				StrategyInfo *pData = (StrategyInfo*)lParam;
				return pMain->Open(pData);
			}
			else if (wParam == 2)
			{
				StrategyHis *pData = (StrategyHis*)lParam;
				return pMain->Open(pData);
			}
			else if (wParam == 3)
			{
				return pMain->StrategyManage();
			}
		
			HSOpenExpressUseData* pData = (HSOpenExpressUseData*)lParam;
			if( (pData->m_wType & 0xF00) == hxOpenExpress_Edit )			// 修改公式
			{			
				pMain->SendMessage(HX_USER_LOADCUR,CCompileEditView::UpdateExpress,(LPARAM)pData->m_pData);
			}
			else if( (pData->m_wType & 0xF00) == hxOpenExpress_Notes )			// 注释
			{
				return pMain->OnNotes(wParam,lParam);
			}
			else if( (pData->m_wType & 0xF00) == hxOpenExpress_Eidolon )			// 参数
			{
				return pMain->OnParam(wParam,lParam);
			}
			else if( (pData->m_wType & 0xF00) == hxShowAlignWindow )			// 
			{
				return (long)pMain->GetIndividualInfo(pData);//CreateIndividualInfo();
			}
			else if( (pData->m_wType & 0xF00) == hsOpenExpress_Create )  // 快速新建指述指标公式
			{
				return (long)pMain->NewExpress(Tech,CDlgFormulaEdit::NewExp | CDlgFormulaEdit::SpeedNew );//SendMessage(WM_COMMAND,IDD_NEW,0);//SpeedCreateTech();
			}
			else if(pData->m_wType == hxSaveLoadTree)
			{
				pMain->Save();
			}
			else if( pData->m_wType == hxSettingExpressParent )// 设置公式dll父窗口
			{
			}
			else
			{
				return pMain->Open(pData);
			}
		}
	}

	return 0;
}

long CFormulaExport::OpenCompile(HWND hwndNotify,long** pThis)
{ 
	if( g_pFormulaMan != NULL )
	{
		g_pFormulaMan->ShowWindow(SW_SHOWNORMAL);
		return (long)g_pFormulaMan->m_hWnd;
	}

 	AfxInitRichEdit();
 	AfxEnableControlContainer();

	g_pFormulaMan = new CDlgFormulaMan(NULL,(long*)&g_pFormulaMan);
	CWnd* pWnd = CWnd::FromHandle(g_pFormulaMan->m_pMainWnd->GetMainWndHandle());
	g_pFormulaMan->Create(CDlgFormulaMan::IDD,pWnd);

	SetWindowLong(g_pFormulaMan->m_hWnd,GWL_STYLE,
		WS_CAPTION		  | \
		WS_SYSMENU        | \
		WS_THICKFRAME     | \
		WS_MINIMIZEBOX    | \
		WS_MAXIMIZEBOX    | \
		WS_POPUP);
	if ( g_pFormulaMan )
		g_pFormulaMan->ShowWindow(SW_SHOWNORMAL);
	return (long)g_pFormulaMan->m_hWnd;
}

// gbq note 20060329, 函数功能于注释不符 ?
long CFormulaExport::OnMessage(UINT message ,WPARAM wParam,LPARAM lParam)
{
	if(g_pFormulaMan != NULL/* && lParam*/)
	{
		if( message == HX_USER_DRAGDATA )	  // 设定公式源
		{
			if( lParam == NULL || wParam == NULL )
				return 0;

			// gbq add 20060329, 注意这里设置当前参数, 非好的修改方法,因为上面设定公式源违反了消息定义的原意,导致有些地方调用错误
			if( wParam == HSCurKeyInfo::UpdateDefExpress )
			{
				g_pFormulaMan->SendMessage(message,wParam,lParam);
				return 0;
			}
			// gbq end

			CString* pStr = (CString*)wParam;
			CExpression* pCExpression = (CExpression* )lParam;

			CValue* pValoare = NULL;
			CString strName = pCExpression->GetName();
			if( !CDlgFormulaMan::m_pExternExpression->Lookup(strName,pCExpression->GetExpressType(),pValoare) )
				return 0;

			CString strValue;
			strValue.Format("%s",*pStr);

			CExpression* pExpresie = new CExpression(CDlgFormulaMan::m_pExternExpression,
				CDlgFormulaMan::m_pExternVariabile,CDlgFormulaMan::m_pExternFunction);

			int nErrorCount = pExpresie->ChangeExpression(strValue);
			if( nErrorCount != 0 )
				return 0;

			CExpValue* pValue = NULL;
			if(pValoare != NULL)
			{
				pValue = (CExpValue*)pValoare;
				CExpression* pOldExpress = pValue->GetExp();
				if(pOldExpress != NULL)
				{
					pExpresie->SetExpressType(pCExpression->GetExpressType());
				}
				pValue->Clean();					
			}

			//
			pValue->SetValue(pExpresie);
			pExpresie->SetName(strName);
			pExpresie->AddStyle( HS_EXP_SUBCHART );

			return 1;

			//return pCExpression->ChangeExpression(str);
		}
		else if( message == g_WindowDataExchangeMessage )
		{
			CWinApp* pApp = AfxGetApp();
			if(pApp != NULL)
			{
				MSG* pMsg = (MSG*)lParam;
				if(pMsg != NULL)
				{
					if( g_pFormulaMan->IsWindowVisible() )
						return FALSE;
					HWND hWnd = ::GetFocus();
					if(hWnd == NULL || !::IsWindow(hWnd) )
						return FALSE;
					if( !IsChild(g_pFormulaMan->m_hWnd,hWnd) ) 
					{
						::SetFocus(g_pFormulaMan->m_hWnd);
					}
					pMsg->hwnd = hWnd;//g_pFormulaMan->m_hWnd;
					//return g_pFormulaMan->PreTranslateMessage(pMsg);
					::SendMessage(pMsg->hwnd,pMsg->message,pMsg->wParam,pMsg->lParam);
					return TRUE;
					//return pApp->PreTranslateMessage(pMsg);
				}
			}
		}
		else
		{
			g_pFormulaMan->SendMessage(message,wParam,lParam);
		}

		//RECT* rect = (RECT*)lParam;
		//ScreenToClient(g_hParentWnd,(POINT*)rect);
		//g_pFormulaMan->MoveWindow(rect->left,rect->top,rect->right,rect->bottom);
	}
	return 0;
}

BOOL CFormulaExport::CalcValue( CString strName, DWORD style ,CArray<CLineProperty*,CLineProperty*>* pAyData
			  /* HSCurDataInfoStatus* pCurDataStatus,*/
			  /* CTValues<void>* pRefCalcValues*/)
{	
	if ( pAyData == NULL )
		return FALSE;
	GetLineProperty(strName,style,pAyData);
	strName.MakeUpper();
	CValue* valoare = NULL;
	CMapVariabile* pVar = NULL;
	DWORD nMask = style & 0x00FF;
	if ( CDlgFormulaMan::m_pExternExpression != NULL && 
		CDlgFormulaMan::m_pExternExpression->Lookup(strName, nMask, valoare) )
	{
		CExpression* pExp = ((CExpValue*)valoare)->GetExpByCompile();
		if(pExp != NULL)
		{			
			pVar = pExp->GetExpDraw();
		}
	}
	if( pVar != NULL &&
		pVar->GetObjArray() != NULL )
	{
		short nDrawCount;
		pVar->GetDrawCount(nDrawCount);
		if( nDrawCount == 0 )
		{
			return FALSE;
		}

		CExpValue* pCurVal;
		CLineProperty* pRefData=NULL;
		int nObj;
		bool bEnd = false;
		int aySize = pAyData->GetSize();
		for(nObj = 0; nObj < pVar->GetObjArray()->GetSize(); nObj++)
		{
			pCurVal = (CExpValue*)pVar->GetObjArray()->GetAt(nObj);
			
			if (nObj >= aySize)
				break;
			pRefData = pAyData->GetAt(nObj);
			if( pCurVal != NULL && pRefData != NULL)
			{
				pRefData->init();	
				int nEnd = pRefData->m_nSize;
				if (nObj ==  pVar->GetObjArray()->GetSize()-1)
					bEnd = true;
				pCurVal->m_CurExecInWhile = false;
				pCurVal->CalcValue(pRefData,0,nEnd,NULL,bEnd,pAyData);				
			}
		}			
	}	
	return FALSE;
}

BOOL CFormulaExport::SetDayData(StockDay* pDay,long lCurSize,
				   int nVolumeUnit,
				   int nMoneyUnit,short nMoneyDecimal,
				   int nPriceUnit,short nPriceDecimal,CodeInfo* pCodeinfo,
				   WORD nKey )
{
	//多代码处理
	//一个代码一个数据源，存放在m_mapDataSource中
	if ( g_pHxDataSource == NULL )
	{
		g_pHxDataSource = new  CHSDataSource;
	}
	return g_pHxDataSource->SetDay(pDay,lCurSize,nVolumeUnit,nMoneyUnit,nMoneyDecimal,nPriceUnit,
		nPriceDecimal,nKey,NULL);
}

void CFormulaExport::AddStyle(long dwStyle)
{
	if ( g_pHxDataSource )
		g_pHxDataSource->AddStyle(dwStyle);
}

int CFormulaExport::GetLineNum(CString strName,DWORD style	) 
{
	CValue* valoare = NULL;
	CMapVariabile* pVar = NULL;
	strName.MakeUpper();
	DWORD nMask = style & 0x00FF;
	if ( CDlgFormulaMan::m_pExternExpression != NULL && 
		CDlgFormulaMan::m_pExternExpression->Lookup(strName,nMask, valoare) )
	{
		CExpression* pExp = ((CExpValue*)valoare)->GetExpByCompile();
		if(pExp != NULL)
		{			
			pVar = pExp->GetExpDraw();
		}
	}
	short nDrawCount = 0;
	short nSize = 0;
	if( pVar != NULL &&
		pVar->GetObjArray() != NULL )
	{			
		nDrawCount = pVar->GetSize();
	/*	pVar->GetDrawCount(nDrawCount);*/
		if (nDrawCount == 0)
			return 0;
	}
	return nDrawCount;
}

BOOL CFormulaExport::GetLineProperty(CString strName,DWORD style, CArray<CLineProperty*,CLineProperty*>*& pAyData)
{
	if ( pAyData == NULL )
		return FALSE;
	CValue* valoare = NULL;
	strName.MakeUpper();
	CMapVariabile* pVar = NULL;
	DWORD nMask = style & 0x00FF;
	if ( CDlgFormulaMan::m_pExternExpression != NULL && 
		CDlgFormulaMan::m_pExternExpression->Lookup(strName, nMask, valoare) )
	{
		CExpression* pExp = ((CExpValue*)valoare)->GetExpByCompile();
		if(pExp != NULL)
		{			
			pVar = pExp->GetExpDraw();
		}
	}
	if( pVar != NULL &&
		pVar->GetObjArray() != NULL )
	{
		CExpValue* pCurVal;      // 当前绘图表达式
		CExpValue* pCurParamVal; // 绘图参数
		int		   nPar;
		HSDouble   lValue;
		short	   nParamSize = 0;
		CLineProperty* pData;		

		short nColorPos = 0;
		CString strLineName = "";
		for(int nObj = 0; nObj < pVar->GetObjArray()->GetSize(); nObj++)
		{
			pCurVal = (CExpValue*)pVar->GetObjArray()->GetAt(nObj);
			if( pCurVal != NULL )
			{
				if ( nObj < pAyData->GetSize())
					pData = pAyData->GetAt(nObj);
				if( pData != NULL )
				{
					pData->FreeStr();

					if( pCurVal->IsStyle(HX_EXPRESSDRAW) )
					{
						pData->SetStyle(HX_EXPRESSDRAW);

						// 参数名称
 						if( pCurVal->IsStyle(HX_EXPRESSDRAW_NONAME) )
 						{
 							/*pData->m_strName = "";*/
							memset(pData->m_strName, 0, sizeof(pData->m_strName));
 						}
 						else
						{
							CString str = pVar->GetObjectName(pCurVal).Left( pVar->GetObjectName(pCurVal).GetLength() - 3);// add 去掉最后的000
							memcpy(pData->m_strName, str, min(sizeof(pData->m_strName)-1,str.GetLength()));
						}

						// 参数数值
						CMapVariabile* pParam = ((CExpValue*)pCurVal)->GetParam();
						if( pParam != NULL )
						{
							// 参数：绘图类型;绘图颜色;线风格;线宽度;...PS_SOLID
							nParamSize = pParam->GetObjArray()->GetSize();

							// 是自定义坐标
							CString strName;
							strName.Format("%s", pData->m_strName);
							if( !strName.CompareNoCase(EXP_REFLINE) )
							{
								pData->RemoveStyle(HX_EXPRESSDRAW);								
							}
							else
							{
								// 表达式后的4个参数：绘图类型,绘图颜色,线风格,线宽度
								for( nPar = 0; nPar < nParamSize; nPar++ )
								{
									HSCurDataInfoStatus pCurDataStatus;

									pCurParamVal = (CExpValue*)pParam->GetObjArray()->GetAt(nPar);
									lValue		 = pCurParamVal->GetValue(NULL);
								
								    if( nPar == 3 )	     // 线宽度
									{
										if( (long)lValue.m_dData != -1 )
										{
											pData->m_lColor.m_nWidth = lValue;
											if( pData->m_lColor.m_nWidth > 10 )
												pData->m_lColor.m_nWidth = 0;
										}
										else
											pData->m_lColor.m_nWidth = 1;
									}
									else if( nPar == 2 ) // 线风格
									{									
										if( (long)lValue.m_dData != -1 )
										{
											pData->m_lColor.m_nPenStyle = lValue;
											if( pData->m_lColor.m_nPenStyle > PS_NULL )
												pData->m_lColor.m_nPenStyle = PS_SOLID;
										}
									}
									else if( nPar == 1 ) // 绘图颜色
									{
										if( (long)lValue.m_dData != -1 )
										{
											pData->m_lColor.m_lColor = (long)lValue;
										}
										else
										{				 
 											if( m_pHSColor)
 											{
 												pData->m_lColor.m_lColor =  m_pHSColor->GetTechLineColor(nColorPos+1);
 											}
 											else
 											{
												(pData->m_lColor).m_nPenStyle = 0;
												pData->m_lColor.m_lColor = RGB((BYTE)rand(),(BYTE)rand(),(BYTE)rand()); 												
 											}
											nColorPos ++;
										}
									}
									else if( nPar == 0 ) // 绘图类型
									{
										if( (long)lValue.m_dData != -1 )
										{
											long lStyle = lValue;
											if( lStyle & HX_NOTEXPRESSDRAW )
											{
												lStyle &= ~HX_EXPRESSDRAW;
												pData->RemoveStyle(HX_EXPRESSDRAW);
											}
											pData->AddStyle( lValue );
										}
									}									
									else            // 多余的参数无效
									{
										break;
									}
								}
								if( nParamSize == 1 )  // 缺省颜色
								{
									if( m_pHSColor)
									{
										pData->m_lColor.m_lColor =  m_pHSColor->GetTechLineColor(nColorPos+1);
										nColorPos ++;
									}
									else
									{
										(pData->m_lColor).m_nPenStyle = 0;
										pData->m_lColor.m_lColor = RGB((BYTE)rand(),(BYTE)rand(),(BYTE)rand()); 												
									}
								}
							}
						}
						else  // 缺省颜色
						{
							if( m_pHSColor)
							{
								pData->m_lColor.m_lColor =  m_pHSColor->GetTechLineColor(nColorPos+1);
							}
							else
							{
								(pData->m_lColor).m_nPenStyle = 0;
								pData->m_lColor.m_lColor = RGB((BYTE)rand(),(BYTE)rand(),(BYTE)rand()); 												
							}
							nColorPos ++;
						}
						
					}
					else
					{
						pData->RemoveStyle(HX_EXPRESSDRAW);
					}
				}
			}
		}
	}
	return TRUE;
}

long CFormulaExport::ShowExpression(WPARAM wParam)
{
	CString* strKey = (CString*)wParam;
	return (long)g_pFormulaMan->ShowExpression( *strKey );
}

long CFormulaExport::FindExpression(WPARAM wParam)
{
	g_pFormulaMan->OnButfind();
	return 1;
	CString* strKey = (CString*)wParam;
	return (long)g_pFormulaMan->FindExpression( *strKey );
}


long CFormulaExport::PreTranslateMsgDlg( MSG* pMsg)
{
	if (g_pDlgFormulaFind != NULL)
	{
		return g_pDlgFormulaFind->PreTranslateMessage(pMsg);
	}
	return 0;
	
}
void CFormulaExport::GetExpProp(CArray<ExpPropery*,ExpPropery*>& ayNameProp)
{
	CMapVariabile* pExpressMap = CDlgFormulaMan::m_pExternExpression;
	if(pExpressMap != NULL)
	{
		CExpression* pExpress;
		POSITION pos = pExpressMap->GetStartPosition();
		CString sName;
		CValue *pCurVal;
		CString strText;
		
		while ( pos != NULL )
		{	
			pExpressMap->GetNextAssoc (pos, sName, pCurVal);
			if( pCurVal != NULL )
			{
				pExpress = pCurVal->GetExp();
				if( pExpress != NULL	&&
					!pExpress->IsStyle(HS_EXP_PROTECTED) && // 公式是否为保护的
					( pExpress->GetExpressType() == Tech	     || 
					  pExpress->GetExpressType()  == Exchange  ||
					  pExpress->GetExpressType()  == Strategy  || 
					  pExpress->GetExpressType()  == MoreKLine
					)
					)
				{
					sName = pExpress->GetName();
					sName.MakeUpper();					
					ExpPropery* pNameProp = new ExpPropery;
					pNameProp->m_dExpType = pExpress->GetExpressType();
					pNameProp->m_strName.Format("%s", sName);
					pNameProp->m_strDescribe.Format("%s",  pExpress->GetDescribe());
					pNameProp->m_bStyle = pExpress->GetStyle();
					ayNameProp.Add(pNameProp);					
				}
			}
		}
	}
}
//
void CFormulaExport::GetNowData(CodeInfo* pCode, CArray<int> ayID, CArray<DOUBLE>& ayData)
{
}

CString CFormulaExport::GetExpressVar( CString strName,DWORD style )
{
	int allocSize = 50;
	CValue* valoare = NULL;
	strName.MakeUpper();
	CString strValue = strName;
	CMapVariabile* pVar = NULL;
	strValue += "(";
	DOUBLE dValue;
	DWORD nMask = style & 0x00FF;
	if ( CDlgFormulaMan::m_pExternExpression != NULL && 
		CDlgFormulaMan::m_pExternExpression->Lookup(strName, nMask, valoare) )
	{
		CExpression* pExp = ((CExpValue*)valoare)->GetExpByCompile();
		if ( pExp )
		{
			pVar = pExp->GetParamVar();
			if ( pVar && pVar->GetObjArray())
			{
				for(int nObj = 0; nObj < pVar->GetObjArray()->GetSize(); nObj++)
				{
					CNumericValue* pValue = (CNumericValue*)pVar->GetAt(nObj);
					if (pValue && !IsBadReadPtr(pValue,1) )
					{
						CString tempValue;
						dValue = pValue->GetValue();
						tempValue.Format("%f",dValue);
						tempValue = hxSubZero(tempValue);			
						if (nObj != pVar->GetObjArray()->GetSize()-1)
							tempValue += ",";
						strValue += tempValue;
					}
				}
			}
		
		}
		
	}
	strValue += ")";
	return strValue;		
}

void CFormulaExport::OpenExpWindow( CString strName, DWORD dwStyle, DWORD dwType )
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	if (g_HSCompExport == NULL)
	{
		g_HSCompExport = new CFormulaExport;
		g_HSCompExport->initExpress(NULL, 0);
	}

	CValue* valoare = NULL;
	strName.MakeUpper();
	CMapVariabile* pVar = NULL;
	DWORD nMask = dwStyle & 0x00FF;
	CExpression* pExp = NULL;
	if ( CDlgFormulaMan::m_pExternExpression != NULL && 
		CDlgFormulaMan::m_pExternExpression->Lookup(strName, nMask, valoare) )
	{
		pExp = ((CExpValue*)valoare)->GetExpByCompile();
	}
	if ( g_HSCompExport )
	{
		HSOpenExpressUseData data(dwType);
		if ( (dwType & 0xF00) <= 0x0300 )
		{	
			if (  pExp)
			{
				data.m_pData = pExp;
				g_HSCompExport->OpenExpress(0,(LPARAM)&data);
			}			
		}
		else
			g_HSCompExport->OpenExpress(0,(LPARAM)&data);		
	}
}