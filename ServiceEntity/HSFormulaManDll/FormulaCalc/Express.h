/*************************************************************
*	Copyright (c)2010, �������ӹɷ����޹�˾
*	All rights reserved.
*
*	�ļ����ƣ�	Express.h
*	�ļ���ʶ��	
*	ժ	  Ҫ��	��ʽ��Ķ���
*
*	��ǰ�汾��	1.0
*	��	  �ߣ�	
*	�������ڣ�	
*
***************************************************************/

#if !defined(_EXPRESSION_H_)
#define _EXPRESSION_H_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#ifndef __AFXWIN_H__
#error include 'stdafx.h' before including this file for PCH
#endif

/*#include "..\comm\HSexportbase.h"*/

#include "value.h"
#include "FxjFormulaStruct.h"

/////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////
class CFunExpresion;
class CSystemFun;
class CExpValue;

// WIN message ID
extern UINT HX_USER_COMPILEDATA;			   // ����
extern UINT HX_USER_UPDATEDATA;				   // ����
extern UINT HX_USER_DRAGDATA;				   // �Ϸ�����
extern UINT EXPRESS_HS_FIND_DATA_MSG;         // ��������
extern UINT EXPRESS_ZiXun_Data_Request;        // ��Ѷ������������
extern UINT m_HS_DOWNLOADDATA_MSG;			   // http ����
extern UINT m_HS_DOWNLOADDATA_Other_MSG;      // http ������Ѷmsg
extern UINT HS_USER_ZOOM;
extern UINT HS_USER_STRATEGY;
extern UINT EXPRESS_HS_FIND_DATA_MSG;

// ������ͼ����
#define HS_MAIN_PKLINE		    "KLINE"
#define HS_MAIN_FENSHI		    "CLOSE"
#define HS_MAIN_AMERICA	    "BAR"
#define HS_MAIN_XOLINE		    "XO"
#define HS_MAIN_NEWPRICELINE "NEWPRICE"
#define HS_MAIN_TWR		      "TWR"
#define HS_MAIN_CNDEQV		    "CNDEQV"
#define HS_MAIN_TREND        "TREND"
#define HS_TREND_VOL         "TVOL"
#define HS_TREND_LB          "LB"
#define HS_TREND_MMLD        "MMLD"
#define HS_TREND_TOTALHOLD   "TTH"
#define HS_TREND_ETFBALANCE  "ETFBP"
#define HS_MAJOR_INDEXTREND  "MAJORTREND"

// 
#define HS_MAIN_VOL		  "VOL" // �ɽ���
//

// ��ʽ��������
#define G_STRTECH			 _T("����ָ��")
#define G_STRCONDITION	     _T("����ѡ��")
#define G_STREXCHANGE	     _T("����ϵͳ")
#define G_STRCOLORKLINE	     _T("��ʣ���")
#define G_STRRECYCLEBIN	     _T("����վ")
#define G_STRSTRATEGY		 _T("���Խ���") //20090302 YJT ����

#define G_STRUNIONCONDITION  _T("�������")
#define G_STRJIBEN           _T("��ǰ<������/����>ѡ��")
//

// ��ʽ�ڲ��ַ��ؼ���
#define EXP_REFLINE			 "REFLINE"    // �Զ�������,���磺REFLINE:L1,L2,��LN;	
										  //                  ��ʾ��L1��L2����LN������ˮƽ�����ߡ�

#define EXP_ENTERLONG		 "ENTERLONG"  // ������������,�ڽ���ϵͳ��ʹ��
#define EXP_EXITLONG		 "EXITLONG"   // ������������,�ڽ���ϵͳ��ʹ��

/*************************************************************
*	�� �� �ƣ�	CExpression
*	�� �� ����	��ʽ�ඨ��
***************************************************************/
class  CExpression: public CExpressionBase
{
public:
	/*************************************************************
	*	�� �� �ƣ�	HSFormulaFileHead
	*	�� �� ����	��ʽ�����ļ����ļ�ͷ�ṹ
	***************************************************************/
	class BCGCONTROLBARDLLEXPORT HSFormulaFileHead
	{
	public:
		// ��ʽ����
		long     m_lCount;
		// ��ǰ�汾100001001
		long     m_lVersion;
		// ͷ˵��
		CString  m_strHead;

		// ����,��ʼ������
		HSFormulaFileHead();
		// ���л�
		void  Serialize(CArchive& ar);
		// �汾���Ƿ���ȷ
		BOOL  IsValid();
	};

	/*************************************************************
	*	�� �� �ƣ�	InExternal
	*	�� �� ����	���ô�����м�����ṹ
	***************************************************************/
	class BCGCONTROLBARDLLEXPORT InExternal
	{
	public:
		// ������
		CString m_strName;
		// ���õķ���".", "#", "$", ��-1
		char	m_cKey;
		InExternal()
		{
			m_cKey = 0;
		}
		InExternal(CString strName,char	cKey)
		{
			m_strName = strName;
			m_cKey	  = cKey;
		}
		// ����m_strName
		CString GetName();
		// �ж�m_cKeyֵ
		BOOL    IsKey(char cKey) { return m_cKey == cKey; }
	};

public:
	DECLARE_DYNCREATE(CExpression)

	CExpression();
	CExpression( CExpression& expresie );
	CExpression( CMapVariabile* pExternExpression,
		CMapVariabile* pExternVars,		         
		CMapVariabile* pExternFunVars);

	// ��ֵ����
	CExpression &operator= (CExpression& expr);

	/******************************************************************
	*	�������ܣ�	�����ⲿ��
	*	����������	CArray<CMapVariabile*
	*				CMapVariabile*>& ayExternVar : ayExternVar[0] Ϊ������ʽ��, ayExternVar[1] Ϊ�ⲿ������, ayExternVar[2] Ϊ�ⲿ������
	*	�� �� ֵ��	
	*	����������	
	*	��    �ߣ�
	*	������ڣ�
	*	�޸�˵����	
	*****************************************************************/
	void		AtachVariables(CArray<CMapVariabile*,CMapVariabile*>& ayExternVar);

	// ɾ������ʽռ�õ�˽�пռ�
	virtual ~CExpression();		

	// ɾ������ʽ��ʽ���ļ��ڲ���ͼ��Ŀռ�
	void	 Free();

	/******************************************************************
	*	�������ܣ�	������ǰ��ʽ��������
	*	����������	void
	*	�� �� ֵ��	LPNODELEMENT : ��ǰ��ʽ������copy�������Ŀռ�ָ��	
	*	����������	
	*	��    �ߣ�
	*	������ڣ�
	*	�޸�˵����	
	*****************************************************************/
	LPNODELEMENT  CloneTree(void);						// 

	/******************************************************************
	*	�������ܣ�	��������tree
	*	����������	void
	*	�� �� ֵ��	int	: ��ʽ����Ϊ���򷵻�0, ���򷵻�-1	  
	*	����������	
	*	��    �ߣ�
	*	������ڣ�
	*	�޸�˵����	
	*****************************************************************/
	int			  UpdateArbore(void);					// 

	/******************************************************************
	*	�������ܣ�	���ر���ʽ����
	*	����������	���ޣ�
	*	�� �� ֵ��	LPNODELEMENT  : ���ص���
	*	����������	
	*	��    �ߣ�
	*	������ڣ�
	*	�޸�˵����	
	*****************************************************************/
	LPNODELEMENT  GetArbore();

	/******************************************************************
	*	�������ܣ�	���ù�ʽ����ʽ��
	*	����������	CExpression* pThisExp	: [in] Ŀ�깫ʽ����
	*				LPNODELEMENT Arbore		: [in] Ŀ�깫ʽ��
	*	�� �� ֵ��	void		  
	*	����������	
	*	��    �ߣ�
	*	������ڣ�
	*	�޸�˵����	
	*****************************************************************/
	void		  SetArbore(CExpression* pThisExp,LPNODELEMENT Arbore);

	/******************************************************************
	*	�������ܣ�	�ڲ���ʽ����ƽӿ�
	*	����������	double& valoare		: [out] ������
	*				HSCurDataInfoStatus* pCurDataStatus : [in/out] ����������������
	*	�� �� ֵ��	int	: 0�ɹ�, -1���ɹ�	  
	*	����������	vexp()
	*	��    �ߣ�
	*	������ڣ�
	*	�޸�˵����	
	*****************************************************************/
	int			  Value(double& valoare,HSCurDataInfoStatus* pCurDataStatus);				// gets the value of the expression

	/******************************************************************
	*	�������ܣ�	�ı乫ʽ����
	*	����������	CString& expresie : ��Ĺ�ʽ������
	*	�� �� ֵ��	int	: ����Ĵ�����		  
	*	����������	
	*	��    �ߣ�
	*	������ڣ�
	*	�޸�˵����	
	*****************************************************************/
	int			  ChangeExpression(CString& expresie);	// Change expression


	// ��ʽ�ļ��Ĵ�ȡ
	virtual void  Serialize(CArchive& ar);				// The serialization function
	virtual void  Serialize(FxjExpress*  pFxjExpress);  // ���������
	virtual void  Serialize(CExpression* pExpress);     // copy 

	/******************************************************************
	*	�������ܣ�	�½���ʽ
	*	����������	CExpression* pExpress : �������ʽ
	*				ExpressParam* pParam  : ���ù�ʽ����,����pParam->pValueΪCExpression����
	*	�� �� ֵ��	void  
	*	����������	
	*	��    �ߣ�
	*	������ڣ�
	*	�޸�˵����	
	*****************************************************************/
	virtual void  NewExpress(CExpression* pExpress,ExpressParam* pParam);

	enum  ExpValuesType // ��ֵ����
	{	
		ExpNone,	     // ��

		ExpExternExp,	 // �ⲿ���ʽ
		ExpExternVar,	 // �ⲿ�������������ʽ����Ĳ�������		
		ExpExternFun,	 // �ⲿ����������ϵͳ�Ѷ���ĺ������Զ��庯��		

		ExpDrawObj,		 // ��ͼ���ʽ���ڴ����ϻ�ͼ�ı��ʽ
		ExpInsideVar,    // �ڲ�������һ��ָ�ڱ��ʽ�ڲ�����ı���
		ExpParamVar,	 // �ڲ�������
		ExpDllParam		 // �ⲿ��̬�����
	};


protected:
	// �õ���������(��ϸ)
	void   GetDetailVariable( LPNODELEMENT pTree,FormulaGeneralParam* ayVars );
	// �õ�һ�ж��Ų���
	void   GetVariable( CExpValue* pValoare,LPNODELEMENT pTree,HSCurDataInfoStatus* pCurDataStatus);

	LPNODELEMENT GetVarFromTree(LPNODELEMENT pTree,CString strName);

public:

	/******************************************************************
	*	�������ܣ�	ִ�к���
	*	����������	 LPNODELEMENT pTree						: [in] ����������,pTree->pData�����˺���ָ�뼰�����б�ָ��
	*				HSCurDataInfoStatus* pCurDataStatus : [in/out] ������
	*	�� �� ֵ��	double : �����Ƿ�ɹ��������
	*	����������	
	*	��    �ߣ�
	*	������ڣ�
	*	�޸�˵����	
	*****************************************************************/
	double ExeFunction( LPNODELEMENT pTree,HSCurDataInfoStatus* pCurDataStatus );  // 

	/******************************************************************
	*	�������ܣ�	ִ�����ù�ʽ
	*	����������	 LPNODELEMENT pTree						: [in] ��ʽ��
	*				HSCurDataInfoStatus* pCurDataStatus	: [in/out] ������
	*	�� �� ֵ��	double : �ɹ�0, ʧ��hx_DefMinValues
	*	����������	
	*	��    �ߣ�	
	*	������ڣ�	
	*	�޸�˵����	
	*****************************************************************/
	double ExeExpress( LPNODELEMENT pTree ,HSCurDataInfoStatus* pCurDataStatus );  

	/******************************************************************
	*	�������ܣ�	ִ����������
	*	����������	 LPNODELEMENT pTree : [in] ��ʽ��
	*				HSCurDataInfoStatus* pCurDataStatus : [in/out] ������
	*	�� �� ֵ��	double  : ʧ��hx_DefMinValues, ���ط������õ�����
	*	����������	
	*	��    �ߣ�	
	*	������ڣ�	
	*	�޸�˵����	
	*****************************************************************/
	double ExeInport( LPNODELEMENT pTree ,HSCurDataInfoStatus* pCurDataStatus );   // 

	/******************************************************************
	*	�������ܣ�	ִ�и������
	*	����������	 LPNODELEMENT a : [in] ��ʽ��
	*				HSCurDataInfoStatus* pCurDataStatus : [in/out] ������
	*	�� �� ֵ��	double  : ���ؾ�Ϊhx_DefMinValues,������
	*	����������	
	*	��    �ߣ�	
	*	������ڣ�	
	*	�޸�˵����	
	*****************************************************************/
	double ExeEvaluate( LPNODELEMENT a,HSCurDataInfoStatus* pCurDataStatus );

	/******************************************************************
	*	�������ܣ�	ִ�ж��Ų���
	*	����������	LPNODELEMENT a : [in] ��ʽ��
	*				HSCurDataInfoStatus* pCurDataStatus : [in/out] �ܵļ�����
	*				double& v1  : [out] ��ʽ����ߵļ����Ƿ�ɹ��������
	*				double& v2	: [out] ��ʽ���ұߵļ����Ƿ�ɹ��������
	*				HSCurDataInfoStatus& calc1 : [in/out] ��ʽ����ߵļ�����
	*				HSCurDataInfoStatus& calc2 : [in/out] ��ʽ���ұߵļ�����
	*	�� �� ֵ��	double : ʧ��hx_DefMinValues, �ɹ�hx_ReturnPointValues,�����Ǽ�����
	*	����������	
	*	��    �ߣ�
	*	������ڣ�
	*	�޸�˵����	
	*****************************************************************/
	double ExeKomma(LPNODELEMENT a,HSCurDataInfoStatus* pCurDataStatus,
		double& v1,double& v2,
		HSCurDataInfoStatus& calc1,
		HSCurDataInfoStatus& calc2);

	/******************************************************************
	*	�������ܣ�	ָ�зֺŲ���,��Ҫ�����ڲ����������һ����һ�й�ʽ�����������Ĺ�ʽ
	*	����������	LPNODELEMENT a	: [in] һ���Էֺ�Ϊ������������
	*				HSCurDataInfoStatus* pCurDataStatus : [in] ����m_dwStatus���ж��Ƿ񽫴���������
	*	�� �� ֵ��	double : ��Ϊhx_DefMinValues,������
	*	����������	
	*	��    �ߣ�
	*	������ڣ�
	*	�޸�˵����	
	*****************************************************************/
	double ExeColon(LPNODELEMENT a,HSCurDataInfoStatus* pCurDataStatus);

	/******************************************************************
	*	�������ܣ�	ִ�к��ϲ���,һ���Ǵ��ּ�������е�ȡ����(���ͼ�������ڲ�����)
	*	����������	LPNODELEMENT a	: [in] ��ʽ��
	*				HSCurDataInfoStatus* pCurDataStatus : [out] ������
	*	�� �� ֵ��	double : hx_ReturnPointValues����ɹ�,�����pCurDataStatus��,hx_DefMinValues����ʧ��, ������ֱָ�ӷ��ص�����
	*	����������	
	*	��    �ߣ�
	*	������ڣ�
	*	�޸�˵����	
	*****************************************************************/
	double ExeDefault(LPNODELEMENT a,HSCurDataInfoStatus* pCurDataStatus);

	double ExeWhile(LPNODELEMENT a,HSCurDataInfoStatus* pCurDataStatus);

protected:
	BOOL   IsOperInTree(LPNODELEMENT pTree,CArray<unsigned char,unsigned char>& ayOperatie,int nSize);

protected:	
	// ;  ���б���
	LPNODELEMENT	LineOp();			  
	// := ��ֵ����,�ڱ��ʽ�ﶨ�����
	LPNODELEMENT	EvaluateOp();		  
	// "," ����	
	LPNODELEMENT	KommaOp();		     
	// || or
	BOOL			IsOrlogic();
	LPNODELEMENT	OrlogicalOp();        
	// && and
	BOOL			IsAndlogic();
	LPNODELEMENT	AndlogicalOp();       
	// "< > = == >= <= <> !=" ����
	LPNODELEMENT	logicalOp();         
	// !		
	LPNODELEMENT	sgOp();               
	// �����ո�
	void			SkipSpaces();		
	// ����ע��
	BOOL			SkipNote();   
	// �õ���ǰ֮ǰ��ID��
	int				GetDefinedVarPos();	  // 
	// �Ƿ�Ϊ����
	int				IsNegativeSign();
	// �Ƿ�Ϊ��Ч�Ĳ���(��Ҫ�ǲ��������Ƿ����)
	BOOL IsValidateParam(CFunExpresion* pFun,LPNODELEMENT pTree,FormulaGeneralParam* ayVars); // �����Ƿ���Ч
	BOOL IsValidateParam(CExpression* pExpression,LPNODELEMENT pTree,FormulaGeneralParam* ayVars); // ���ò����Ƿ���Ч
public:

	/******************************************************************
	*	�������ܣ�	����a���н�������(�ڲ�������Ҫ��,���ײ�)
	*	����������	 LPNODELEMENT a							: [in] ��ʽ��
	*				HSCurDataInfoStatus* pCurDataStatus : [in/out] �������������������������� 
	*	�� �� ֵ��	double : hx_ReturnPointValues����ɹ�,�����pCurDataStatus��,hx_DefMinValues����ʧ��, ������ֱָ�ӷ��ص����� 
	*	����������	
	*	��    �ߣ�
	*	������ڣ�
	*	�޸�˵����	
	*****************************************************************/
	virtual double vexp ( LPNODELEMENT a,HSCurDataInfoStatus* pCurDataStatus);       // 

	// �������
protected:
	// - () | | 
	LPNODELEMENT factor (void);		
	// ��ֵ������������
	LPNODELEMENT identificator(void);  
	// ^
	LPNODELEMENT putere(void);            
	// * /
	LPNODELEMENT termen(void);            
	// + -
	LPNODELEMENT expresie(void);          
	// ɾ�� m_Arbore
	void   elibmem ( LPNODELEMENT a);     
	// ����
	LPNODELEMENT clone(LPNODELEMENT arb); 

	//******************************������litao����*****************************************
	//while�������� �� ѭ�����ڸ�ֵ��䣬����while������ʽ�����ڵ�
	LPNODELEMENT whiledo(CValue* valoare,int nEnd);
	//ѭ������
	HSDouble WHILE (CExpression* pCompile, FormulaGeneralParam* ayVars,
		HSCurDataInfoStatus* pCurDataStatus, int& nError);

	//����WHILEѭ��֮������
	CString GetWhileDefinitie(CString strSource);
	//��WHILE֮ǰ��WHILE��ֵ
	CString AddWHILE(CString strSource);
	//******************************������litao����*****************************************

	// �Ƿ���뵽�ı������
	BOOL IsEnd();

	// ����Ϊ�����ж�
protected:	
	int IsAddSign(BYTE cChar,const CString& strSource,int& nPos);		// +
	int IsSubSign(BYTE cChar,const CString& strSource,int& nPos);		// -
	int IsRideSign(BYTE cChar,const CString& strSource,int& nPos);		// *
	int IsDivideSign(BYTE cChar,const CString& strSource,int& nPos);	// /

	int IsNot(BYTE cChar,const CString& strSource,int& nPos);			// !
	int IsPower(BYTE cChar,const CString& strSource,int& nPos);			// ^
	int IsAnd(BYTE cChar,const CString& strSource,int& nPos);			// &
	int IsOr(BYTE cChar,const CString& strSource,int& nPos);			// |
	int IsBracketLeft(BYTE cChar,const CString& strSource,int& nPos);   // (
	int IsBracketRight(BYTE cChar,const CString& strSource,int& nPos);  // )		

	//******************************������litao����*****************************************
	int IsBigBracketLeft(BYTE cChar,const CString& strSource,int& nPos);   // {
	int IsBigBracketRight(BYTE cChar,const CString& strSource,int& nPos);  // }	
	//******************************������litao����*****************************************

	int IsSemicolon(BYTE cChar,const CString& strSource,int& nPos);		// ;
	int IsKomma(BYTE cChar,const CString& strSource,int& nPos);			// ,
	int IsColon(BYTE cChar,const CString& strSource,int& nPos);			// :

	int IsGreatLess(BYTE cChar,const CString& strSource,int& nPos);		// >
	int IsLessThan(BYTE cChar,const CString& strSource,int& nPos);		// <
	int IsEqual(BYTE cChar,const CString& strSource,int& nPos);			// =

	int IsDot(BYTE cChar,const CString& strSource,int& nPos);			// .
	int Is$(BYTE cChar,const CString& strSource,int& nPos);				// $
	int IsWell(BYTE cChar,const CString& strSource,int& nPos);			// #
	int IsChineseSpace(BYTE cChar,const CString& strSource,int& nPos);	// ȫ�ǿո�
	int IsInvertedComma(BYTE cChar,const CString& strSource,int& nPos);	// " ����
	int IsText(BYTE cChar,const CString& strSource,int& nPos);			// ' �ַ���

protected:
	// �Ƿ�Ϊ��ѧ����,������򷵻ذ���������ĺ�����
	LPNODELEMENT IsNumFun(CString id);

public:

	/******************************************************************
	*	�������ܣ�	��һ���ڲ����������
	*	����������	CString strName	: [in] ������
	*				CValue* pExpress: [in] ��������
	*	�� �� ֵ��	void 
	*	����������	
	*	��    �ߣ�
	*	������ڣ�	2003��4��16��
	*	�޸�˵����	
	*****************************************************************/
	void AddParamer(CString strName,CValue* pExpress);

	/******************************************************************
	*	�������ܣ�	��һ����ʽ���ڲ���ͼ����(����һ��";",һ���Ҫ��һ��)
	*	����������	LPNODELEMENT pTree : �����ɵĹ�ʽ�ɴ�������
	*	�� �� ֵ��	void 
	*	����������	AddExternDraw(CString strName,CValue* pExpress);
	*	��    �ߣ�
	*	������ڣ�	2003��4��16��
	*	�޸�˵����	
	*****************************************************************/
	void AddExternDraw(LPNODELEMENT pTree);

	/******************************************************************
	*	�������ܣ�	��һ����ʽ���ڲ���ͼ����(����һ��";",һ���Ҫ��һ��)
	*	����������	CString strName : [in] ��ʽ��
	*				CValue* pExpress: [in] ��ʽ��
	*	�� �� ֵ��	void 
	*	����������	
	*	��    �ߣ�
	*	������ڣ�	2003��4��16��
	*	�޸�˵����	
	*****************************************************************/
	void AddExternDraw(CString strName,CValue* pExpress);

	// ��ǰ��ʽ�Ƿ��Ѿ�����
	BOOL IsCompile();
	// ɾ����һ�δ���,Ȼ����й�ʽ�����±���
	int  Compile();

public:	
	// �Ӳ����б��еõ���������,����param#1 .. param#9 ... param#��������
	CStringArray* GetDefEidolon();

	// �õ���ʽ����
	char    GetExpressType()    { return m_nExpressType; }
	// ���ʽ����
	CString GetName()		    { return m_strName;}	 
	// ȫ�ֵı��ʽ����(��������������ǰ�����һ�����ͺ�)
	CString GetWholeName();								
	// ���ʽ����
	CString GetDescribe()		{ return m_strDescribe; }
	// ���ʽע��
	CString GetNotes()			{ return m_strNotes; }   
	// ��������
	CString GetEidolon()		{ return m_strEidolon; } 
	// �õ�����
	CString GetPWD()			{ return m_strPWD; }
	// Ҫ������ַ���
	CString	GetSource()	 		{ return m_definitie; }
	int		SetSource(CString definitie,void* pData = NULL);
	// �Զ������꣬�ַ��������÷ֺ�(;)�ָ�,Ŀǰ������
	CString GetDefAxis() 		{ return m_strDefAxis; }	 

	// ���ù�ʽ����
	void SetExpressType(char nExpressType){ m_nExpressType = nExpressType; }
	// ��ʽ�����ж�
	BOOL IsExpressType(char nExpressType) { return (m_nExpressType == nExpressType); }

	// ���ñ��ʽ����
	void SetName(CString strName)		  { m_strName.Format("%s",strName);}		
	// ���ñ��ʽ����
	void SetDescribe(CString strDescribe) { m_strDescribe.Format("%s",strDescribe);}
	// ���ñ��ʽע��
	void SetNotes(CString strNotes)       { m_strNotes.Format("%s",strNotes);}      
	// ���ò�������
	void SetEidolon(CString strEidolon)   { m_strEidolon.Format("%s",strEidolon);}  
	// ���ÿ����ִ�
	void SetPWD(CString strPWD)			  { m_strPWD.Format("%s",strPWD);}

	// �õ��ڲ�������
	CMapVariabile* GetParamVar();			
	// �õ��ڲ�������Ĳ�������
	int			   GetParamVarCount();						 

	// �ڲ���ͼ������
	CMapVariabile* GetExpDraw()    { return m_pExpDraw;   }	 
	void	SetExpDraw(CMapVariabile *pMap){ m_pExpDraw = pMap;}
	// �����Ƶõ��ڲ���ͼ����
	CValue*		   GetDrawExpress(CString strName);		
	// ��λ�õõ��ڲ���ͼ����
	CValue*		   GetDrawExpress(int nPos);                 

	// ����У��
	BOOL IsValidPWD(CString strPwd);

	// �õ���������,Ŀǰ���ڲ��ô˹���
	short* GetEstopPeriod()   { return &m_wEstopPeriod;   }   
	// �õ�ȱʡ����
	short* GetDefaultPeriod() { return &m_wDefaultPeriod; }   
	// �õ���ʽpExpress�Ĳ�����ǰֵ����ִ�,��"1,10,20"
	CString GetParamString(CExpression* pExpress);
	// �õ��ڲ���ͼ��ĵ�һ����ͼ�����Ĳ�����ǰֵ����ִ�
	CString GetParamString();
	// 

	/******************************************************************
	*	�������ܣ�	�õ���ǰ�ڲ���ͼ�����ĵ�һ������Ĳ�������ִ������л�ͼ����Ļ������Ƽ���صĻ�������:����ɫ,�߷��,�߿�ȵ�
	*	����������	CString& strParam		   : [out] �ڲ���ͼ�����ĵ�һ������Ĳ�������ִ�
	*				CArray<CFormulaDrawParam*  
	*				CFormulaDrawParam*>& ayDraw : [out] ���л�ͼ����Ļ������Ƽ���صĻ�����������
	*	�� �� ֵ��	int : �����ͼ�����ĸ���
	*	����������	
	*	��    �ߣ�
	*	������ڣ�	2003��4��16��
	*	�޸�˵����	
	*****************************************************************/
	int		GetDraw(CString& strParam,CArray<CFormulaDrawParam*,CFormulaDrawParam*>& ayDraw);

public:
	/******************************************************************
	*	�������ܣ�	��������,�õ������ⲿ���ʽ��,�ⲿ������,�ⲿ������,�ڲ���ͼ��,�ڲ��������Լ����������ʽ���ֵ
	*	����������	CString strName	: [in] ��ֵ
	*				short& nRet		: [out] 1:�ⲿ������
	*										2:�ⲿ������
	*										3:�ⲿ��ͼ��
	*										4:�ڲ�������
	*										5:�ڲ�������
	*				int nGetType = -1: [in] -1: �����ⲿ���ʽ���в���, �������ⲿ���ʽ���в���
	*	�� �� ֵ��	CValue* : �ҵ��Ķ���ָ��,�Ҳ�����ΪNULL
	*	����������	
	*	��    �ߣ�
	*	������ڣ�	2003��4��16��
	*	�޸�˵����	
	*****************************************************************/
	CValue* GetVariable(CString& strName,short& nRet,int nGetType = -1,int nHaveID = 0);

	//******************************������litao����*****************************************
	//ִ��ʱ�õ�ǰһ��ʶ�Ĺ�ʽ
	CValue* GetVariableExec(CString strName,short& nRet,int nGetType = -1);

	CValue* GetVariable2(CString& strName,short& nRet,int nGetType = -1);
	//******************************������litao����*****************************************

	// �����ж�
	BOOL	IsPeriod(CString strPeriod);

	// �������������(+-*/,��ѧ����,�߼�,ϵͳ��������ʽ-�������ò���
public:
	// ���������������,������ȫ��ͬ,������"�Ӳ���"Ҳ����,�ڴ�ֻ��ϸд��"�Ӳ���"�Ĳ�������
	// ����Ĳ���������Բ�������
	/******************************************************************
	*	�������ܣ�	�Ӳ���
	*	����������	LPNODELEMENT a : [in] �ӷ������Ĺ�ʽ��
	*				HSCurDataInfoStatus* pCurDataStatus : [out] ������, ����ΪNULL,���������
	*				CExpression* pExpression : [in] ��ʽָ��,��Ҫ��Ϊ�˵�������ļ��㺯��, ����ΪNULL,���������
	*				double& v1 : [out] ��������߼�����
										hx_ReturnPointValues: ��������߼���ɹ�, �����calc1��,
										hx_DefMinValues: ����ʧ��
										����ֵ: ���������� 
	*				double& v2 : [out] �������ұ߼�����
										hx_ReturnPointValues: �������ұ߼���ɹ�, �����calc2��,
										hx_DefMinValues: ����ʧ��
										����ֵ: ���������� 
	*				HSCurDataInfoStatus& calc1 : [out] ��������߼�����
	*				HSCurDataInfoStatus& calc2 : [out] �������ұ߼�����
	*	�� �� ֵ��	double : �Ӳ���������
								hx_ReturnPointValues: ����ɹ�, �����pCurDataStatus��,
								hx_DefMinValues: ����ʧ��
								����ֵ: ���������� 
	*	����������	
	*	��    �ߣ�
	*	������ڣ�	2003��4��17��
	*	�޸�˵����	
	*****************************************************************/
	// +����
	static double Add(  LPNODELEMENT a,
		HSCurDataInfoStatus* pCurDataStatus,
		CExpression* pExpression,
		double& v1,double& v2,
		HSCurDataInfoStatus& calc1,
		HSCurDataInfoStatus& calc2);
	// -����
	static double Sub(  LPNODELEMENT a,
		HSCurDataInfoStatus* pCurDataStatus,
		CExpression* pExpression,
		double& v1,double& v2,
		HSCurDataInfoStatus& calc1,
		HSCurDataInfoStatus& calc2);
	// *����
	static double Ride(  LPNODELEMENT a,
		HSCurDataInfoStatus* pCurDataStatus,
		CExpression* pExpression,
		double& v1,double& v2,
		HSCurDataInfoStatus& calc1,
		HSCurDataInfoStatus& calc2);
	// /����
	static double Divide(  LPNODELEMENT a,
		HSCurDataInfoStatus* pCurDataStatus,
		CExpression* pExpression,
		double& v1,double& v2,
		HSCurDataInfoStatus& calc1,
		HSCurDataInfoStatus& calc2);
	// |����
	static double Or(  LPNODELEMENT a,
		HSCurDataInfoStatus* pCurDataStatus,
		CExpression* pExpression,
		double& v1,double& v2,
		HSCurDataInfoStatus& calc1,
		HSCurDataInfoStatus& calc2);
	// ^����(���η�����)
	static double Pow(  LPNODELEMENT a,
		HSCurDataInfoStatus* pCurDataStatus,
		CExpression* pExpression,
		double& v1,double& v2,
		HSCurDataInfoStatus& calc1,
		HSCurDataInfoStatus& calc2);
	// <����
	static double LessThan(  LPNODELEMENT a,
		HSCurDataInfoStatus* pCurDataStatus,
		CExpression* pExpression,
		double& v1,double& v2,
		HSCurDataInfoStatus& calc1,
		HSCurDataInfoStatus& calc2);
	// >����
	static double GreatLess(  LPNODELEMENT a,
		HSCurDataInfoStatus* pCurDataStatus,
		CExpression* pExpression,
		double& v1,double& v2,
		HSCurDataInfoStatus& calc1,
		HSCurDataInfoStatus& calc2);

	// =����
	static double Equal(  LPNODELEMENT a,
		HSCurDataInfoStatus* pCurDataStatus,
		CExpression* pExpression,
		double& v1,double& v2,
		HSCurDataInfoStatus& calc1,
		HSCurDataInfoStatus& calc2);
	// !����
	static double Not(  LPNODELEMENT a,
		HSCurDataInfoStatus* pCurDataStatus,
		CExpression* pExpression,
		double& v1,double& v2,
		HSCurDataInfoStatus& calc1,
		HSCurDataInfoStatus& calc2);
	// sin ����(����IDΪOperator_SIN)
	static double Sin(  LPNODELEMENT a,
		HSCurDataInfoStatus* pCurDataStatus,
		CExpression* pExpression,
		double& v1,double& v2,
		HSCurDataInfoStatus& calc1,
		HSCurDataInfoStatus& calc2);
	// Cos ����(����IDΪOperator_COS
	static double Cos(  LPNODELEMENT a,
		HSCurDataInfoStatus* pCurDataStatus,
		CExpression* pExpression,
		double& v1,double& v2,
		HSCurDataInfoStatus& calc1,
		HSCurDataInfoStatus& calc2);
	// exp ����(����IDΪOperator_EXP
	static double Exp(  LPNODELEMENT a,
		HSCurDataInfoStatus* pCurDataStatus,
		CExpression* pExpression,
		double& v1,double& v2,
		HSCurDataInfoStatus& calc1,
		HSCurDataInfoStatus& calc2);
	// SQRT ����(����IDΪOperator_SQRT
	static double Sqrt(  LPNODELEMENT a,
		HSCurDataInfoStatus* pCurDataStatus,
		CExpression* pExpression,
		double& v1,double& v2,
		HSCurDataInfoStatus& calc1,
		HSCurDataInfoStatus& calc2);
	// log ����(����IDΪOperator_LOG
	static double Log(  LPNODELEMENT a,
		HSCurDataInfoStatus* pCurDataStatus,
		CExpression* pExpression,
		double& v1,double& v2,
		HSCurDataInfoStatus& calc1,
		HSCurDataInfoStatus& calc2);
	// tg ����(����IDΪOperator_TG
	static double Tg(  LPNODELEMENT a,
		HSCurDataInfoStatus* pCurDataStatus,
		CExpression* pExpression,
		double& v1,double& v2,
		HSCurDataInfoStatus& calc1,
		HSCurDataInfoStatus& calc2);
	// ctg ����(����IDΪOperator_CTG
	static double Ctg(  LPNODELEMENT a,
		HSCurDataInfoStatus* pCurDataStatus,
		CExpression* pExpression,
		double& v1,double& v2,
		HSCurDataInfoStatus& calc1,
		HSCurDataInfoStatus& calc2);
	//ASIN ����(����IDΪOperator_ASIN
	static double Asin(  LPNODELEMENT a,
		HSCurDataInfoStatus* pCurDataStatus,
		CExpression* pExpression,
		double& v1,double& v2,
		HSCurDataInfoStatus& calc1,
		HSCurDataInfoStatus& calc2);
	// ACOS ����(����IDΪOperator_ACOS
	static double Acos(  LPNODELEMENT a,
		HSCurDataInfoStatus* pCurDataStatus,
		CExpression* pExpression,
		double& v1,double& v2,
		HSCurDataInfoStatus& calc1,
		HSCurDataInfoStatus& calc2);
	// ATG ����(����IDΪOperator_ATG
	static double ATG(  LPNODELEMENT a,
		HSCurDataInfoStatus* pCurDataStatus,
		CExpression* pExpression,
		double& v1,double& v2,
		HSCurDataInfoStatus& calc1,
		HSCurDataInfoStatus& calc2);
	// LargerEqual ����(����IDΪOperator_LargerEqual
	static double LargerEqual(  LPNODELEMENT a,
		HSCurDataInfoStatus* pCurDataStatus,
		CExpression* pExpression,
		double& v1,double& v2,
		HSCurDataInfoStatus& calc1,
		HSCurDataInfoStatus& calc2);
	// SmallEqual ����(����IDΪOperator_SmallEqual
	static double SmallEqual(  LPNODELEMENT a,
		HSCurDataInfoStatus* pCurDataStatus,
		CExpression* pExpression,
		double& v1,double& v2,
		HSCurDataInfoStatus& calc1,
		HSCurDataInfoStatus& calc2);
	// NotEqual ����(����IDΪOperator_NotEqual
	static double NotEqual(  LPNODELEMENT a,
		HSCurDataInfoStatus* pCurDataStatus,
		CExpression* pExpression,
		double& v1,double& v2,
		HSCurDataInfoStatus& calc1,
		HSCurDataInfoStatus& calc2);
	// || ����(����IDΪOperator_OR
	static double DoubleOR(  LPNODELEMENT a,
		HSCurDataInfoStatus* pCurDataStatus,
		CExpression* pExpression,
		double& v1,double& v2,
		HSCurDataInfoStatus& calc1,
		HSCurDataInfoStatus& calc2);
	// AND && ����(����IDΪOperator_AND
	static double AND(  LPNODELEMENT a,
		HSCurDataInfoStatus* pCurDataStatus,
		CExpression* pExpression,
		double& v1,double& v2,
		HSCurDataInfoStatus& calc1,
		HSCurDataInfoStatus& calc2);
	// Semicolon ; ����(����IDΪOperator_Semicolon
	static double Semicolon(  LPNODELEMENT a,
		HSCurDataInfoStatus* pCurDataStatus,
		CExpression* pExpression,
		double& v1,double& v2,
		HSCurDataInfoStatus& calc1,
		HSCurDataInfoStatus& calc2);
	// Error ����(����IDΪOperator_Error
	static double Error(  LPNODELEMENT a,
		HSCurDataInfoStatus* pCurDataStatus,
		CExpression* pExpression,
		double& v1,double& v2,
		HSCurDataInfoStatus& calc1,
		HSCurDataInfoStatus& calc2);
	// Komma , ����(����IDΪOperator_Komma
	static double Komma(  LPNODELEMENT a,
		HSCurDataInfoStatus* pCurDataStatus,
		CExpression* pExpression,
		double& v1,double& v2,
		HSCurDataInfoStatus& calc1,
		HSCurDataInfoStatus& calc2);
	// Colon : ����(����IDΪOperator_Colon
	static double Colon(  LPNODELEMENT a,
		HSCurDataInfoStatus* pCurDataStatus,
		CExpression* pExpression,
		double& v1,double& v2,
		HSCurDataInfoStatus& calc1,
		HSCurDataInfoStatus& calc2);
	// Evaluate ����(����IDΪOperator_Evaluate
	static double Evaluate(  LPNODELEMENT a,
		HSCurDataInfoStatus* pCurDataStatus,
		CExpression* pExpression,
		double& v1,double& v2,
		HSCurDataInfoStatus& calc1,
		HSCurDataInfoStatus& calc2);
	// Function ����(����IDΪOperator_Function
	static double Function(  LPNODELEMENT a,
		HSCurDataInfoStatus* pCurDataStatus,
		CExpression* pExpression,
		double& v1,double& v2,
		HSCurDataInfoStatus& calc1,
		HSCurDataInfoStatus& calc2);
	//Express ����(����IDΪOperator_Express
	static double Express(  LPNODELEMENT a,
		HSCurDataInfoStatus* pCurDataStatus,
		CExpression* pExpression,
		double& v1,double& v2,
		HSCurDataInfoStatus& calc1,
		HSCurDataInfoStatus& calc2);
	// InportData �������ò���(����IDΪOperator_InportData
	static double InportData(  LPNODELEMENT a,
		HSCurDataInfoStatus* pCurDataStatus,
		CExpression* pExpression,
		double& v1,double& v2,
		HSCurDataInfoStatus& calc1,
		HSCurDataInfoStatus& calc2);
	// Default ����(һ���Ǵ��ڲ��������������еõ��Ѽ��������)
	static double Default(  LPNODELEMENT a,
		HSCurDataInfoStatus* pCurDataStatus,
		CExpression* pExpression,
		double& v1,double& v2,
		HSCurDataInfoStatus& calc1,
		HSCurDataInfoStatus& calc2);
	// ReturnValue ����(��������ֵ����)
	static double ReturnValue(  LPNODELEMENT a,
		HSCurDataInfoStatus* pCurDataStatus,
		CExpression* pExpression,
		double& v1,double& v2,
		HSCurDataInfoStatus& calc1,
		HSCurDataInfoStatus& calc2);
	// ReturnString�������ַ���
	static double ReturnString(  LPNODELEMENT a,
		HSCurDataInfoStatus* pCurDataStatus,
		CExpression* pExpression,
		double& v1,double& v2,
		HSCurDataInfoStatus& calc1,
		HSCurDataInfoStatus& calc2);
	

public:
	static CMapVariabile*  m_pExternExpression;      // �ⲿ���ʽ��(������ָȫ�ֵĹ�ʽ��
	static CMapVariabile*  m_pExternVariabile;		 // �ⲿ������(����û��
	static CMapVariabile*  m_pExternFunction;		 // �ⲿ������(������ָȫ�ֵ�ϵͳ������

	// ���ݺ������ִ��ⲿ�������еõ���Ӧ��ϵͳ����ָ��
	static COMPILEFUNCTION GetSysFun(CString strName);
protected:
	CMapVariabile*  m_pExpDraw;				 // �ڲ���ͼ��(��ʽ��������,һ����;Ϊһ���µĹ�ʽ,����������

	//******************************������litao����*****************************************
	CMap<CString,LPCTSTR,int,int> m_pCompNo; // ����ʱ��ʶ�����к�
	//CMap<CString,LPCTSTR,int,int> m_pExecNo; // ִ��ʱ��ʶ�����к�
	CString m_strLineObj;					 // ��ǰ�и�ֵ�ı�ʶ
	int m_nIFWHILEStart,m_nIFWHILEEnd;		 // �Ƿ���WHILEѭ���ڲ��ı���,������,��ʼΪ0��1:��ʾ��һ��ѭ����ʼ,����ѭ��Ƕ��ʱ,2,3������ȥ,ѭ�����������ݼ�

	CMap<int,int,int,int> m_nWhileFirst;					// ѭ����ͷλ��
	CMap<int,int,int,int> m_nWhileLast;						// ѭ����βλ��
	CMap<int,int,CString,CString&>  m_SaveExpDef;			// ѭ���ڱ���ʱ�ȱ���ĵ�ǰ���ַ���

	int m_nEnd;												//�������ε�������
	CArray<CLineProperty*,CLineProperty*>* m_pAyData;       //�Ӽ����ഫ��������������
	//******************************������litao����*****************************************

	CMapVariabile*  m_pParamVar;		     // �ڲ�������(��ʽ�Ĳ������þ�����������

	CMapVariabile*  m_pRefExpDraw;			 // �ڲ���ͼ��
	CMapVariabile*  m_pRefInsideVar;		 // �ڲ�������
	CMapVariabile*  m_pRefParamVar;		     // �ڲ�������

	CExpression*	m_pRefExpress;			 // �������ʽ(�ڲ���ͼ���������ʱ��ʽ����,ָ��������˹�ʽ�ĸ���ʽ

	CString			m_definitie;	// Ҫ������ַ���(��ʽ�Ķ�����
	int				m_nPozitie;		// ���ڱ���ĵ�ǰ�ַ���λ��(-1�����Ѿ�����OK
	LPNODELEMENT	m_Arbore;		// �Զ������ķ�ʽ������Ľ��
	
protected:

	CString  		m_strName;		 // ���ʽ����
	CString  		m_strDescribe;	 // ���ʽ����
	CString		  	m_strNotes;		 // ���ʽע��
	CString         m_strEidolon;	 // ��������

	CString  		m_strPWD;		 // ���ʽ����
	CString  		m_strDefAxis;	 // �Զ������꣬�ַ��������÷ֺ�(;)�ָ�

	short			m_wEstopPeriod;   // ��������
	short			m_wDefaultPeriod; // ȱʡ����

	char			m_nExpressType;   // ��ʽ����


	// ��ʽ����
protected:
	DWORD	m_dStyle;		  // ��ʽ���
	// �����ز���(���Ƿ�Ϊ��ͼ�����ʽ�ȵ�)
	//--------2009.12.16 litao���ӳ���ָ��ı�������---------------------------------------------------
	int   m_nDefSortId;      //���ù�ʽ��˳���
	//-------------------------------------------------------------------------------------------------
public:
	BOOL	IsStyle(long dStyle)		{ return (m_dStyle & dStyle); }
	void	AddStyle(long dStyle)		{ m_dStyle |= dStyle;         }
	void	RemoveStyle(long dStyle)	{ m_dStyle &= (~dStyle);      }
	void    SetStyle(long dStyle)		{ m_dStyle = dStyle;          }
	DWORD	GetStyle()					{ return m_dStyle;            }
	int		GetImageIndex(); // ���ݷ���ж��Ƿ�ΪĬ�ϵļ���ָ��,�ǵĻ�����3,���ǵĻ�����4
	void   SetParamVar(CMapVariabile*  pParamVar);//yulx add 20090316 ���ò���
	CExpression *GetRefExpress(){ return m_pRefExpress;}

	int	GetDefSortId()				{ return m_nDefSortId;         }
	void	SetDefSortId(int iSortId)	{ m_nDefSortId = iSortId;      }
protected:	
	friend CFunExpresion;
	friend CSystemFun;

public:
	// ������Ϣ����(һ����ΪMainFram)
	static CWnd* SetCurWnd(CWnd* pMsgWnd);

public:
	/******************************************************************
	*	�������ܣ�	����������ӿ�
	*	����������	CLineProperty*& pRefData	: [out] ���������
	*				int nStart					: [in] ���㿪ʼλ��
	*				int nEnd					: [in] ������λ��
	*				HSCurDataInfoStatus* pCurDataStatus = NULL : [out] ֻ���ڱ��ʽ����ʱ����,��ż�����
	*				CTValues<void>* pRefCalcValues = NULL : [out] ֻ���ڱ��ʽ����ʱ����,��ż�����λ��(��㼰�յ�)
	*	�� �� ֵ��	BOOL  : ��������ȷ(FALSE), ������Ϊ(TRUE)
	*	����������	Value()
	*	��    �ߣ�
	*	������ڣ�
	*	�޸�˵����	
	*****************************************************************/
	//******************************������litao����*****************************************
	BOOL  CalcValue(CLineProperty*& pRefData,int nStart,int nEnd,
		HSCurDataInfoStatus* pCurDataStatus = NULL,
		CTValues<void>* pRefCalcValues = NULL,CArray<CLineProperty*,CLineProperty*>* pAyData=NULL);
	//******************************������litao����*****************************************

	// �õ�ȫ�ֵ�ϵͳ������ָ��
	CSystemFun* GetSystemFunAddr();


	// ��ʽ����
public:
	

	/******************************************************************
	*	�������ܣ�	��ǰ��ʽ�Ƿ���ϲ�������
	*	����������	CString strKey	: [in] �����ִ� 
	*				int& nUnique    : [out] �Ƿ�����ƥ��
	*				DWORD dMask = FindInName | FindInDescribe : [in] ���ҷ�Χ(����,������ע�͵�)
	*	�� �� ֵ��	BOOL  : �Ƿ���ϲ�������
	*	����������	
	*	��    �ߣ�	
	*	������ڣ�	
	*	�޸�˵����	
	*****************************************************************/
	BOOL IsFind(CString strKey,int& nUnique,DWORD dMask = FindInName | FindInDescribe);

	/******************************************************************
	*	�������ܣ�	��ǰ��ʽ�Ƿ���ϲ�������
	*	����������	CString strKey	: [in] �����ִ�
	*				CString& strRet : [out] �����ǰ��ʽ��������,�򷵻�һ��ʽ��,��ʽ��������ʽע����Ϣ,����Ϊ��
	*				DWORD dMask = FindInName | FindInDescribe : [in] ���ҷ�Χ(����,������ע�͵�)
	*	�� �� ֵ��	BOOL : �Ƿ���ϲ�������
	*	����������	
	*	��    �ߣ�	
	*	������ڣ�	
	*	�޸�˵����	
	*****************************************************************/
	BOOL IsFind(CString strKey,CString& strRet,DWORD dMask = FindInName | FindInDescribe);

public:
	friend FunctionParam;

	// ����Դ
protected:
	InportDataParam* m_pDataSource;
public:
	// ��������Դ
	void			 SetData(InportDataParam* pDataSource);
	// �õ�����Դ
	CHSDataSource*   GetData();

	// image index
public:
	static class CImageList*	m_expressImages;

	enum 
	{
		HS_Exp_Root  = 27, 			// ��ʽ����
		HS_Exp_Group = 28, 			// ��
		HS_Exp_Tech  = 7,				// ����ָ��
		HS_Exp_Page = 2,   			// ҳ
		HS_Exp_Condition = 3,   		// ������ʽ
		HS_Exp_Exchange  = 4,   		// ����ϵͳ
		HS_Exp_MoreKLine = 8,   		// ���K��
		HS_Exp_TechUnion = 9,   		// ָ�����
		HS_Exp_ChuQuan = 11,    		// ��Ȩ���
		HS_Exp_CheckTech = 12,  		// ѡ��ָ��
		
		//-----2009.11.12 litao ���ӻ�ͼͼ�������-------------------------
		HS_Exp_DrawIcon0 = 18,			//Ц��
		//HS_Exp_DrawIcon1 = HS_Exp_DrawIcon0,			//Ц��
		HS_Exp_DrawIcon2 = 19,			//����
		HS_Exp_DrawIcon3 = 20,			//ƽ��
		HS_Exp_DrawIcon4 = 13,			//�ҹ�
		HS_Exp_DrawIcon5 = 14,			//����
		HS_Exp_DrawIcon6 = 15,			//б��
		HS_Exp_DrawIcon7 = 24,			//���Ϻ��ͷ
		HS_Exp_DrawIcon8 = 84,			//�����̼�ͷ
		HS_Exp_DrawIcon9 = 21,			//����
		//HS_Exp_DrawIcon10 = HS_Exp_SelectStock,		//����
		HS_Exp_DrawIcon11 = 23,		//��Բ��
		//HS_Exp_DrawIcon12 = HS_Exp_Active,			//��̫��
		HS_Exp_DrawIcon13 = 26,		//��̫��
		HS_Exp_DrawIcon14 = 27,		//������
		//------------------------------------------------------------------

		HS_Exp_SelectStock = 22, 		// ѡ��
		HS_Exp_Active		= 25, 		// ����״̬
		HS_Exp_Disk        = 31, 		// ��
		HS_Exp_Alarming    = 33, 		// ������
		HS_Exp_Alarm       = 35, 		// ����(����״̬)
		HS_Exp_WhiteSun    = 37,	    // ��̫��
		HS_Exp_Union = 9,				// ����ѡ��
		HS_Exp_Bin = 27,		
		HS_Exp_MemoInfo   = 42,			// ����¼
		HS_Exp_Union_Add = 44, 		// ���-and
		HS_Exp_Union_Sub = 45, 		// ���-sub
		HS_Exp_Close = 49,     		// �ر�
		HS_Exp_Info_Mark = 56, 		// ��Ϣ���
		HS_Exp_Cost_Move = 57,			// �ɱ��ֲ�
		HS_Exp_Cost_HuYanShan = 58,	// ����ɽ
		HS_Exp_Cons_HuoYaoDu  = 59,	// ��ҩ��

		HS_Exp_Hor_Right = 66,			// ���ұ�����
		HS_Exp_Hor_Left  = 67,			// ���������
		HS_Exp_Hor_Down  = 68,			// ���±�����
		HS_Exp_Hor_Up    = 69,			// ���ϱ�����

		HS_Exp_Drag_Chart = 70, 		// �Ϸ�ͼ��
		HS_Exp_Connect    = 71, 		// ����
		HS_Exp_Connecting = 72, 		// ���ӹ���
		HS_Exp_DisConnect = 73, 		// �Ͽ�
		HS_Exp_XiaDan     = 74, 		// �µ�
		HS_Exp_OpenInfo   = 75, 		// ����Ϣ

		HS_Exp_TableTab   = 76, 		//����TabCtrl
		HS_Exp_AlarmOn    = 77, 		//������
		Exp_Image_SelStock_Selected = 87,//ϵͳָʾ��ѡ��ͼ��
		Exright_Right = 97,
		Exright_Lift  = 98,
		Min_Icon_Ex	  = 99,
		Max_Icon_Ex	  = 100
	};

	// ���ݹ�ʽ����,�õ�ͼ��ID
	static int GetImgIndex(int nType);

	//******************************������litao����*****************************************
public:
	CString AddObjNo(CString key,int iCompExec);
	CString SubObjNo(CString key,int iCompExec);
	CString GetObjNo(CString key,int iCompExec,int nHaveID = 0);
	CString GetMaxObjNo(CString key,int iCompExec);
	//void SetMaxObjNo(CString key,int iCompExec);
	void RemoveAllNo(int iCompExec);

	LPNODELEMENT GetExpArbore()  {return m_Arbore;}

	//******************************������litao����*****************************************

};



//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(_EXPRESSION_H_)
