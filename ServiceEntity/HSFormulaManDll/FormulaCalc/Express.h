/*************************************************************
*	Copyright (c)2010, 恒生电子股份有限公司
*	All rights reserved.
*
*	文件名称：	Express.h
*	文件标识：	
*	摘	  要：	公式类的定义
*
*	当前版本：	1.0
*	作	  者：	
*	更新日期：	
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
extern UINT HX_USER_COMPILEDATA;			   // 编译
extern UINT HX_USER_UPDATEDATA;				   // 更新
extern UINT HX_USER_DRAGDATA;				   // 拖放数据
extern UINT EXPRESS_HS_FIND_DATA_MSG;         // 查找数据
extern UINT EXPRESS_ZiXun_Data_Request;        // 资讯数据请求数据
extern UINT m_HS_DOWNLOADDATA_MSG;			   // http 下载
extern UINT m_HS_DOWNLOADDATA_Other_MSG;      // http 下载资讯msg
extern UINT HS_USER_ZOOM;
extern UINT HS_USER_STRATEGY;
extern UINT EXPRESS_HS_FIND_DATA_MSG;

// 几种主图名称
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
#define HS_MAIN_VOL		  "VOL" // 成交量
//

// 公式分类名称
#define G_STRTECH			 _T("技术指标")
#define G_STRCONDITION	     _T("条件选股")
#define G_STREXCHANGE	     _T("交易系统")
#define G_STRCOLORKLINE	     _T("五彩Ｋ线")
#define G_STRRECYCLEBIN	     _T("回收站")
#define G_STRSTRATEGY		 _T("策略交易") //20090302 YJT 增加

#define G_STRUNIONCONDITION  _T("组合条件")
#define G_STRJIBEN           _T("当前<基本面/行情>选股")
//

// 公式内部字符关键字
#define EXP_REFLINE			 "REFLINE"    // 自定义坐标,形如：REFLINE:L1,L2,…LN;	
										  //                  表示在L1、L2、…LN处绘制水平坐标线。

#define EXP_ENTERLONG		 "ENTERLONG"  // 定义买入条件,在交易系统中使用
#define EXP_EXITLONG		 "EXITLONG"   // 定义卖出条件,在交易系统中使用

/*************************************************************
*	类 名 称：	CExpression
*	类 描 述：	公式类定义
***************************************************************/
class  CExpression: public CExpressionBase
{
public:
	/*************************************************************
	*	类 名 称：	HSFormulaFileHead
	*	类 描 述：	公式保存文件的文件头结构
	***************************************************************/
	class BCGCONTROLBARDLLEXPORT HSFormulaFileHead
	{
	public:
		// 公式个数
		long     m_lCount;
		// 当前版本100001001
		long     m_lVersion;
		// 头说明
		CString  m_strHead;

		// 构造,初始化处理
		HSFormulaFileHead();
		// 串行化
		void  Serialize(CArchive& ar);
		// 版本号是否正确
		BOOL  IsValid();
	};

	/*************************************************************
	*	类 名 称：	InExternal
	*	类 描 述：	引用处理的中间变量结构
	***************************************************************/
	class BCGCONTROLBARDLLEXPORT InExternal
	{
	public:
		// 引用名
		CString m_strName;
		// 引用的符号".", "#", "$", 或-1
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
		// 返回m_strName
		CString GetName();
		// 判断m_cKey值
		BOOL    IsKey(char cKey) { return m_cKey == cKey; }
	};

public:
	DECLARE_DYNCREATE(CExpression)

	CExpression();
	CExpression( CExpression& expresie );
	CExpression( CMapVariabile* pExternExpression,
		CMapVariabile* pExternVars,		         
		CMapVariabile* pExternFunVars);

	// 赋值重载
	CExpression &operator= (CExpression& expr);

	/******************************************************************
	*	函数功能：	加载外部表
	*	函数参数：	CArray<CMapVariabile*
	*				CMapVariabile*>& ayExternVar : ayExternVar[0] 为表外表达式表, ayExternVar[1] 为外部变量表, ayExternVar[2] 为外部函数表
	*	返 回 值：	
	*	调用依赖：	
	*	作    者：
	*	完成日期：
	*	修改说明：	
	*****************************************************************/
	void		AtachVariables(CArray<CMapVariabile*,CMapVariabile*>& ayExternVar);

	// 删除本公式占用的私有空间
	virtual ~CExpression();		

	// 删除本公式公式树的及内部绘图表的空间
	void	 Free();

	/******************************************************************
	*	函数功能：	拷贝当前公式的树返回
	*	函数参数：	void
	*	返 回 值：	LPNODELEMENT : 当前公式的树的copy的新树的空间指针	
	*	调用依赖：	
	*	作    者：
	*	完成日期：
	*	修改说明：	
	*****************************************************************/
	LPNODELEMENT  CloneTree(void);						// 

	/******************************************************************
	*	函数功能：	编译生成tree
	*	函数参数：	void
	*	返 回 值：	int	: 公式定义为空则返回0, 否则返回-1	  
	*	调用依赖：	
	*	作    者：
	*	完成日期：
	*	修改说明：	
	*****************************************************************/
	int			  UpdateArbore(void);					// 

	/******************************************************************
	*	函数功能：	返回本公式的树
	*	函数参数：	（无）
	*	返 回 值：	LPNODELEMENT  : 返回的树
	*	调用依赖：	
	*	作    者：
	*	完成日期：
	*	修改说明：	
	*****************************************************************/
	LPNODELEMENT  GetArbore();

	/******************************************************************
	*	函数功能：	设置公式及公式树
	*	函数参数：	CExpression* pThisExp	: [in] 目标公式数据
	*				LPNODELEMENT Arbore		: [in] 目标公式树
	*	返 回 值：	void		  
	*	调用依赖：	
	*	作    者：
	*	完成日期：
	*	修改说明：	
	*****************************************************************/
	void		  SetArbore(CExpression* pThisExp,LPNODELEMENT Arbore);

	/******************************************************************
	*	函数功能：	内部公式计算计接口
	*	函数参数：	double& valoare		: [out] 计算结果
	*				HSCurDataInfoStatus* pCurDataStatus : [in/out] 计算结果或输入数据
	*	返 回 值：	int	: 0成功, -1不成功	  
	*	调用依赖：	vexp()
	*	作    者：
	*	完成日期：
	*	修改说明：	
	*****************************************************************/
	int			  Value(double& valoare,HSCurDataInfoStatus* pCurDataStatus);				// gets the value of the expression

	/******************************************************************
	*	函数功能：	改变公式定义
	*	函数参数：	CString& expresie : 请的公式定义体
	*	返 回 值：	int	: 编译的错误数		  
	*	调用依赖：	
	*	作    者：
	*	完成日期：
	*	修改说明：	
	*****************************************************************/
	int			  ChangeExpression(CString& expresie);	// Change expression


	// 公式文件的存取
	virtual void  Serialize(CArchive& ar);				// The serialization function
	virtual void  Serialize(FxjExpress*  pFxjExpress);  // 导入分析家
	virtual void  Serialize(CExpression* pExpress);     // copy 

	/******************************************************************
	*	函数功能：	新建公式
	*	函数参数：	CExpression* pExpress : 相关联公式
	*				ExpressParam* pParam  : 引用公式参数,其中pParam->pValue为CExpression类型
	*	返 回 值：	void  
	*	调用依赖：	
	*	作    者：
	*	完成日期：
	*	修改说明：	
	*****************************************************************/
	virtual void  NewExpress(CExpression* pExpress,ExpressParam* pParam);

	enum  ExpValuesType // 数值类型
	{	
		ExpNone,	     // 无

		ExpExternExp,	 // 外部表达式
		ExpExternVar,	 // 外部变量，包括表达式定义的参数变量		
		ExpExternFun,	 // 外部函数，所有系统已定义的函数、自定义函数		

		ExpDrawObj,		 // 绘图表达式，在窗口上绘图的表达式
		ExpInsideVar,    // 内部变量，一般指在表达式内部定义的变量
		ExpParamVar,	 // 内部参数表
		ExpDllParam		 // 外部动态库调用
	};


protected:
	// 得到函数参数(详细)
	void   GetDetailVariable( LPNODELEMENT pTree,FormulaGeneralParam* ayVars );
	// 得到一行逗号参数
	void   GetVariable( CExpValue* pValoare,LPNODELEMENT pTree,HSCurDataInfoStatus* pCurDataStatus);

	LPNODELEMENT GetVarFromTree(LPNODELEMENT pTree,CString strName);

public:

	/******************************************************************
	*	函数功能：	执行函数
	*	函数参数：	 LPNODELEMENT pTree						: [in] 函数计算树,pTree->pData包含了函数指针及参数列表指针
	*				HSCurDataInfoStatus* pCurDataStatus : [in/out] 计算结果
	*	返 回 值：	double : 计算是否成功或计算结果
	*	调用依赖：	
	*	作    者：
	*	完成日期：
	*	修改说明：	
	*****************************************************************/
	double ExeFunction( LPNODELEMENT pTree,HSCurDataInfoStatus* pCurDataStatus );  // 

	/******************************************************************
	*	函数功能：	执行引用公式
	*	函数参数：	 LPNODELEMENT pTree						: [in] 公式树
	*				HSCurDataInfoStatus* pCurDataStatus	: [in/out] 计算结果
	*	返 回 值：	double : 成功0, 失败hx_DefMinValues
	*	调用依赖：	
	*	作    者：	
	*	完成日期：	
	*	修改说明：	
	*****************************************************************/
	double ExeExpress( LPNODELEMENT pTree ,HSCurDataInfoStatus* pCurDataStatus );  

	/******************************************************************
	*	函数功能：	执行引用数据
	*	函数参数：	 LPNODELEMENT pTree : [in] 公式树
	*				HSCurDataInfoStatus* pCurDataStatus : [in/out] 计算结果
	*	返 回 值：	double  : 失败hx_DefMinValues, 返回返回引用的数据
	*	调用依赖：	
	*	作    者：	
	*	完成日期：	
	*	修改说明：	
	*****************************************************************/
	double ExeInport( LPNODELEMENT pTree ,HSCurDataInfoStatus* pCurDataStatus );   // 

	/******************************************************************
	*	函数功能：	执行赋真操作
	*	函数参数：	 LPNODELEMENT a : [in] 公式树
	*				HSCurDataInfoStatus* pCurDataStatus : [in/out] 计算结果
	*	返 回 值：	double  : 返回均为hx_DefMinValues,无意义
	*	调用依赖：	
	*	作    者：	
	*	完成日期：	
	*	修改说明：	
	*****************************************************************/
	double ExeEvaluate( LPNODELEMENT a,HSCurDataInfoStatus* pCurDataStatus );

	/******************************************************************
	*	函数功能：	执行逗号操作
	*	函数参数：	LPNODELEMENT a : [in] 公式树
	*				HSCurDataInfoStatus* pCurDataStatus : [in/out] 总的计算结果
	*				double& v1  : [out] 公式树左边的计算是否成功或计算结果
	*				double& v2	: [out] 公式树右边的计算是否成功或计算结果
	*				HSCurDataInfoStatus& calc1 : [in/out] 公式树左边的计算结果
	*				HSCurDataInfoStatus& calc2 : [in/out] 公式树右边的计算结果
	*	返 回 值：	double : 失败hx_DefMinValues, 成功hx_ReturnPointValues,否则是计算结果
	*	调用依赖：	
	*	作    者：
	*	完成日期：
	*	修改说明：	
	*****************************************************************/
	double ExeKomma(LPNODELEMENT a,HSCurDataInfoStatus* pCurDataStatus,
		double& v1,double& v2,
		HSCurDataInfoStatus& calc1,
		HSCurDataInfoStatus& calc2);

	/******************************************************************
	*	函数功能：	指行分号操作,主要是往内部函数表里加一个从一行公式编译结果产生的公式
	*	函数参数：	LPNODELEMENT a	: [in] 一棵以分号为根操作符的树
	*				HSCurDataInfoStatus* pCurDataStatus : [in] 据其m_dwStatus项判断是否将错误加入表中
	*	返 回 值：	double : 均为hx_DefMinValues,无意义
	*	调用依赖：	
	*	作    者：
	*	完成日期：
	*	修改说明：	
	*****************************************************************/
	double ExeColon(LPNODELEMENT a,HSCurDataInfoStatus* pCurDataStatus);

	/******************************************************************
	*	函数功能：	执行黑认操作,一般是从又计算的项中得取数据(如绘图函数及内部变量)
	*	函数参数：	LPNODELEMENT a	: [in] 公式树
	*				HSCurDataInfoStatus* pCurDataStatus : [out] 计算结果
	*	返 回 值：	double : hx_ReturnPointValues处理成功,结果在pCurDataStatus中,hx_DefMinValues处理失败, 其它是指直接返回的数据
	*	调用依赖：	
	*	作    者：
	*	完成日期：
	*	修改说明：	
	*****************************************************************/
	double ExeDefault(LPNODELEMENT a,HSCurDataInfoStatus* pCurDataStatus);

	double ExeWhile(LPNODELEMENT a,HSCurDataInfoStatus* pCurDataStatus);

protected:
	BOOL   IsOperInTree(LPNODELEMENT pTree,CArray<unsigned char,unsigned char>& ayOperatie,int nSize);

protected:	
	// ;  按行编译
	LPNODELEMENT	LineOp();			  
	// := 赋值操作,在表达式里定义变量
	LPNODELEMENT	EvaluateOp();		  
	// "," 操作	
	LPNODELEMENT	KommaOp();		     
	// || or
	BOOL			IsOrlogic();
	LPNODELEMENT	OrlogicalOp();        
	// && and
	BOOL			IsAndlogic();
	LPNODELEMENT	AndlogicalOp();       
	// "< > = == >= <= <> !=" 操作
	LPNODELEMENT	logicalOp();         
	// !		
	LPNODELEMENT	sgOp();               
	// 跳过空格
	void			SkipSpaces();		
	// 跳过注释
	BOOL			SkipNote();   
	// 得到当前之前的ID名
	int				GetDefinedVarPos();	  // 
	// 是否为负号
	int				IsNegativeSign();
	// 是否为有效的参数(主要是参数个数是否相等)
	BOOL IsValidateParam(CFunExpresion* pFun,LPNODELEMENT pTree,FormulaGeneralParam* ayVars); // 参数是否有效
	BOOL IsValidateParam(CExpression* pExpression,LPNODELEMENT pTree,FormulaGeneralParam* ayVars); // 引用参数是否有效
public:

	/******************************************************************
	*	函数功能：	按树a进行解析计算(内部计算主要接,更底层)
	*	函数参数：	 LPNODELEMENT a							: [in] 公式树
	*				HSCurDataInfoStatus* pCurDataStatus : [in/out] 输入计算条件参数及输出计算结果 
	*	返 回 值：	double : hx_ReturnPointValues处理成功,结果在pCurDataStatus中,hx_DefMinValues处理失败, 其它是指直接返回的数据 
	*	调用依赖：	
	*	作    者：
	*	完成日期：
	*	修改说明：	
	*****************************************************************/
	virtual double vexp ( LPNODELEMENT a,HSCurDataInfoStatus* pCurDataStatus);       // 

	// 编译相关
protected:
	// - () | | 
	LPNODELEMENT factor (void);		
	// 数值、函数、变量
	LPNODELEMENT identificator(void);  
	// ^
	LPNODELEMENT putere(void);            
	// * /
	LPNODELEMENT termen(void);            
	// + -
	LPNODELEMENT expresie(void);          
	// 删除 m_Arbore
	void   elibmem ( LPNODELEMENT a);     
	// 复制
	LPNODELEMENT clone(LPNODELEMENT arb); 

	//******************************以上是litao新增*****************************************
	//while函数参数 及 循环体内赋值语句，返回while函数公式树跟节点
	LPNODELEMENT whiledo(CValue* valoare,int nEnd);
	//循环函数
	HSDouble WHILE (CExpression* pCompile, FormulaGeneralParam* ayVars,
		HSCurDataInfoStatus* pCurDataStatus, int& nError);

	//返回WHILE循环之间的语句
	CString GetWhileDefinitie(CString strSource);
	//在WHILE之前加WHILE赋值
	CString AddWHILE(CString strSource);
	//******************************以上是litao新增*****************************************

	// 是否编译到文本的最后
	BOOL IsEnd();

	// 以下为符号判断
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

	//******************************以下是litao新增*****************************************
	int IsBigBracketLeft(BYTE cChar,const CString& strSource,int& nPos);   // {
	int IsBigBracketRight(BYTE cChar,const CString& strSource,int& nPos);  // }	
	//******************************以上是litao新增*****************************************

	int IsSemicolon(BYTE cChar,const CString& strSource,int& nPos);		// ;
	int IsKomma(BYTE cChar,const CString& strSource,int& nPos);			// ,
	int IsColon(BYTE cChar,const CString& strSource,int& nPos);			// :

	int IsGreatLess(BYTE cChar,const CString& strSource,int& nPos);		// >
	int IsLessThan(BYTE cChar,const CString& strSource,int& nPos);		// <
	int IsEqual(BYTE cChar,const CString& strSource,int& nPos);			// =

	int IsDot(BYTE cChar,const CString& strSource,int& nPos);			// .
	int Is$(BYTE cChar,const CString& strSource,int& nPos);				// $
	int IsWell(BYTE cChar,const CString& strSource,int& nPos);			// #
	int IsChineseSpace(BYTE cChar,const CString& strSource,int& nPos);	// 全角空格
	int IsInvertedComma(BYTE cChar,const CString& strSource,int& nPos);	// " 引用
	int IsText(BYTE cChar,const CString& strSource,int& nPos);			// ' 字符串

protected:
	// 是否为数学函数,如果是则返回包含其参数的函数树
	LPNODELEMENT IsNumFun(CString id);

public:

	/******************************************************************
	*	函数功能：	加一个内部参数表参数
	*	函数参数：	CString strName	: [in] 参数名
	*				CValue* pExpress: [in] 参数精灵
	*	返 回 值：	void 
	*	调用依赖：	
	*	作    者：
	*	完成日期：	2003年4月16日
	*	修改说明：	
	*****************************************************************/
	void AddParamer(CString strName,CValue* pExpress);

	/******************************************************************
	*	函数功能：	加一个公式到内部绘图函数(编译一行";",一般就要加一个)
	*	函数参数：	LPNODELEMENT pTree : 新生成的公式由此树产生
	*	返 回 值：	void 
	*	调用依赖：	AddExternDraw(CString strName,CValue* pExpress);
	*	作    者：
	*	完成日期：	2003年4月16日
	*	修改说明：	
	*****************************************************************/
	void AddExternDraw(LPNODELEMENT pTree);

	/******************************************************************
	*	函数功能：	加一个公式到内部绘图函数(编译一行";",一般就要加一个)
	*	函数参数：	CString strName : [in] 公式名
	*				CValue* pExpress: [in] 公式体
	*	返 回 值：	void 
	*	调用依赖：	
	*	作    者：
	*	完成日期：	2003年4月16日
	*	修改说明：	
	*****************************************************************/
	void AddExternDraw(CString strName,CValue* pExpress);

	// 当前公式是否已经编译
	BOOL IsCompile();
	// 删除上一次错误,然后进行公式的重新编译
	int  Compile();

public:	
	// 从参数列表中得到参数精灵,有如param#1 .. param#9 ... param#参数个数
	CStringArray* GetDefEidolon();

	// 得到公式类型
	char    GetExpressType()    { return m_nExpressType; }
	// 表达式名称
	CString GetName()		    { return m_strName;}	 
	// 全局的表达式名称(与上区别是在最前面加了一个类型号)
	CString GetWholeName();								
	// 表达式描述
	CString GetDescribe()		{ return m_strDescribe; }
	// 表达式注释
	CString GetNotes()			{ return m_strNotes; }   
	// 参数精灵
	CString GetEidolon()		{ return m_strEidolon; } 
	// 得到口令
	CString GetPWD()			{ return m_strPWD; }
	// 要编译的字符串
	CString	GetSource()	 		{ return m_definitie; }
	int		SetSource(CString definitie,void* pData = NULL);
	// 自定义坐标，字符串坐标用分号(;)分割,目前好象不用
	CString GetDefAxis() 		{ return m_strDefAxis; }	 

	// 设置公式类型
	void SetExpressType(char nExpressType){ m_nExpressType = nExpressType; }
	// 公式类型判断
	BOOL IsExpressType(char nExpressType) { return (m_nExpressType == nExpressType); }

	// 设置表达式名称
	void SetName(CString strName)		  { m_strName.Format("%s",strName);}		
	// 设置表达式描述
	void SetDescribe(CString strDescribe) { m_strDescribe.Format("%s",strDescribe);}
	// 设置表达式注释
	void SetNotes(CString strNotes)       { m_strNotes.Format("%s",strNotes);}      
	// 设置参数精灵
	void SetEidolon(CString strEidolon)   { m_strEidolon.Format("%s",strEidolon);}  
	// 设置口令字串
	void SetPWD(CString strPWD)			  { m_strPWD.Format("%s",strPWD);}

	// 得到内部参数表
	CMapVariabile* GetParamVar();			
	// 得到内部参数表的参数总数
	int			   GetParamVarCount();						 

	// 内部绘图函数表
	CMapVariabile* GetExpDraw()    { return m_pExpDraw;   }	 
	void	SetExpDraw(CMapVariabile *pMap){ m_pExpDraw = pMap;}
	// 由名称得到内部绘图函数
	CValue*		   GetDrawExpress(CString strName);		
	// 由位置得到内部绘图函数
	CValue*		   GetDrawExpress(int nPos);                 

	// 口令校对
	BOOL IsValidPWD(CString strPwd);

	// 得到禁用周期,目前不在采用此功能
	short* GetEstopPeriod()   { return &m_wEstopPeriod;   }   
	// 得到缺省周期
	short* GetDefaultPeriod() { return &m_wDefaultPeriod; }   
	// 得到公式pExpress的参数当前值表达字串,如"1,10,20"
	CString GetParamString(CExpression* pExpress);
	// 得到内部绘图表的第一个绘图函数的参数当前值表达字串
	CString GetParamString();
	// 

	/******************************************************************
	*	函数功能：	得到当前内部绘图函数的第一个输出的参数表达字串及所有绘图输出的画线名称及相关的画线属性:线颜色,线风格,线宽度等
	*	函数参数：	CString& strParam		   : [out] 内部绘图函数的第一个输出的参数表达字串
	*				CArray<CFormulaDrawParam*  
	*				CFormulaDrawParam*>& ayDraw : [out] 所有绘图输出的画线名称及相关的画线属性数组
	*	返 回 值：	int : 输出绘图函数的个数
	*	调用依赖：	
	*	作    者：
	*	完成日期：	2003年4月16日
	*	修改说明：	
	*****************************************************************/
	int		GetDraw(CString& strParam,CArray<CFormulaDrawParam*,CFormulaDrawParam*>& ayDraw);

public:
	/******************************************************************
	*	函数功能：	根据名字,得到其在外部表达式表,外部变量表,外部函数表,内部绘图表,内部参数表以及其相关联公式里的值
	*	函数参数：	CString strName	: [in] 键值
	*				short& nRet		: [out] 1:外部变量表
	*										2:外部函数表
	*										3:外部绘图表
	*										4:内部变量表
	*										5:内部参数表
	*				int nGetType = -1: [in] -1: 不在外部表达式表中查找, 否则在外部表达式表中查找
	*	返 回 值：	CValue* : 找到的对象指针,找不到则为NULL
	*	调用依赖：	
	*	作    者：
	*	完成日期：	2003年4月16日
	*	修改说明：	
	*****************************************************************/
	CValue* GetVariable(CString& strName,short& nRet,int nGetType = -1,int nHaveID = 0);

	//******************************以下是litao新增*****************************************
	//执行时得到前一标识的公式
	CValue* GetVariableExec(CString strName,short& nRet,int nGetType = -1);

	CValue* GetVariable2(CString& strName,short& nRet,int nGetType = -1);
	//******************************以上是litao新增*****************************************

	// 周期判断
	BOOL	IsPeriod(CString strPeriod);

	// 基本运算符操作(+-*/,数学函数,逻辑,系统函数及公式-数据引用操作
public:
	// 基本运算符操作中,参数完全相同,意义与"加操作"也类似,在此只详细写了"加操作"的参数函义
	// 后面的参数意义可以参数这里
	/******************************************************************
	*	函数功能：	加操作
	*	函数参数：	LPNODELEMENT a : [in] 加法操作的公式树
	*				HSCurDataInfoStatus* pCurDataStatus : [out] 计算结果, 不能为NULL,否则会死机
	*				CExpression* pExpression : [in] 公式指针,主要是为了调用里面的计算函数, 不能为NULL,否则会死机
	*				double& v1 : [out] 操作符左边计算结果
										hx_ReturnPointValues: 操作符左边计算成功, 结果在calc1中,
										hx_DefMinValues: 处理失败
										其它值: 计算结果数据 
	*				double& v2 : [out] 操作符右边计算结果
										hx_ReturnPointValues: 操作符右边计算成功, 结果在calc2中,
										hx_DefMinValues: 处理失败
										其它值: 计算结果数据 
	*				HSCurDataInfoStatus& calc1 : [out] 操作符左边计算结果
	*				HSCurDataInfoStatus& calc2 : [out] 操作符右边计算结果
	*	返 回 值：	double : 加操作计算结果
								hx_ReturnPointValues: 计算成功, 结果在pCurDataStatus中,
								hx_DefMinValues: 处理失败
								其它值: 计算结果数据 
	*	调用依赖：	
	*	作    者：
	*	完成日期：	2003年4月17日
	*	修改说明：	
	*****************************************************************/
	// +操作
	static double Add(  LPNODELEMENT a,
		HSCurDataInfoStatus* pCurDataStatus,
		CExpression* pExpression,
		double& v1,double& v2,
		HSCurDataInfoStatus& calc1,
		HSCurDataInfoStatus& calc2);
	// -操作
	static double Sub(  LPNODELEMENT a,
		HSCurDataInfoStatus* pCurDataStatus,
		CExpression* pExpression,
		double& v1,double& v2,
		HSCurDataInfoStatus& calc1,
		HSCurDataInfoStatus& calc2);
	// *操作
	static double Ride(  LPNODELEMENT a,
		HSCurDataInfoStatus* pCurDataStatus,
		CExpression* pExpression,
		double& v1,double& v2,
		HSCurDataInfoStatus& calc1,
		HSCurDataInfoStatus& calc2);
	// /操作
	static double Divide(  LPNODELEMENT a,
		HSCurDataInfoStatus* pCurDataStatus,
		CExpression* pExpression,
		double& v1,double& v2,
		HSCurDataInfoStatus& calc1,
		HSCurDataInfoStatus& calc2);
	// |操作
	static double Or(  LPNODELEMENT a,
		HSCurDataInfoStatus* pCurDataStatus,
		CExpression* pExpression,
		double& v1,double& v2,
		HSCurDataInfoStatus& calc1,
		HSCurDataInfoStatus& calc2);
	// ^操作(几次方操作)
	static double Pow(  LPNODELEMENT a,
		HSCurDataInfoStatus* pCurDataStatus,
		CExpression* pExpression,
		double& v1,double& v2,
		HSCurDataInfoStatus& calc1,
		HSCurDataInfoStatus& calc2);
	// <操作
	static double LessThan(  LPNODELEMENT a,
		HSCurDataInfoStatus* pCurDataStatus,
		CExpression* pExpression,
		double& v1,double& v2,
		HSCurDataInfoStatus& calc1,
		HSCurDataInfoStatus& calc2);
	// >操作
	static double GreatLess(  LPNODELEMENT a,
		HSCurDataInfoStatus* pCurDataStatus,
		CExpression* pExpression,
		double& v1,double& v2,
		HSCurDataInfoStatus& calc1,
		HSCurDataInfoStatus& calc2);

	// =操作
	static double Equal(  LPNODELEMENT a,
		HSCurDataInfoStatus* pCurDataStatus,
		CExpression* pExpression,
		double& v1,double& v2,
		HSCurDataInfoStatus& calc1,
		HSCurDataInfoStatus& calc2);
	// !操作
	static double Not(  LPNODELEMENT a,
		HSCurDataInfoStatus* pCurDataStatus,
		CExpression* pExpression,
		double& v1,double& v2,
		HSCurDataInfoStatus& calc1,
		HSCurDataInfoStatus& calc2);
	// sin 操作(操作ID为Operator_SIN)
	static double Sin(  LPNODELEMENT a,
		HSCurDataInfoStatus* pCurDataStatus,
		CExpression* pExpression,
		double& v1,double& v2,
		HSCurDataInfoStatus& calc1,
		HSCurDataInfoStatus& calc2);
	// Cos 操作(操作ID为Operator_COS
	static double Cos(  LPNODELEMENT a,
		HSCurDataInfoStatus* pCurDataStatus,
		CExpression* pExpression,
		double& v1,double& v2,
		HSCurDataInfoStatus& calc1,
		HSCurDataInfoStatus& calc2);
	// exp 操作(操作ID为Operator_EXP
	static double Exp(  LPNODELEMENT a,
		HSCurDataInfoStatus* pCurDataStatus,
		CExpression* pExpression,
		double& v1,double& v2,
		HSCurDataInfoStatus& calc1,
		HSCurDataInfoStatus& calc2);
	// SQRT 操作(操作ID为Operator_SQRT
	static double Sqrt(  LPNODELEMENT a,
		HSCurDataInfoStatus* pCurDataStatus,
		CExpression* pExpression,
		double& v1,double& v2,
		HSCurDataInfoStatus& calc1,
		HSCurDataInfoStatus& calc2);
	// log 操作(操作ID为Operator_LOG
	static double Log(  LPNODELEMENT a,
		HSCurDataInfoStatus* pCurDataStatus,
		CExpression* pExpression,
		double& v1,double& v2,
		HSCurDataInfoStatus& calc1,
		HSCurDataInfoStatus& calc2);
	// tg 操作(操作ID为Operator_TG
	static double Tg(  LPNODELEMENT a,
		HSCurDataInfoStatus* pCurDataStatus,
		CExpression* pExpression,
		double& v1,double& v2,
		HSCurDataInfoStatus& calc1,
		HSCurDataInfoStatus& calc2);
	// ctg 操作(操作ID为Operator_CTG
	static double Ctg(  LPNODELEMENT a,
		HSCurDataInfoStatus* pCurDataStatus,
		CExpression* pExpression,
		double& v1,double& v2,
		HSCurDataInfoStatus& calc1,
		HSCurDataInfoStatus& calc2);
	//ASIN 操作(操作ID为Operator_ASIN
	static double Asin(  LPNODELEMENT a,
		HSCurDataInfoStatus* pCurDataStatus,
		CExpression* pExpression,
		double& v1,double& v2,
		HSCurDataInfoStatus& calc1,
		HSCurDataInfoStatus& calc2);
	// ACOS 操作(操作ID为Operator_ACOS
	static double Acos(  LPNODELEMENT a,
		HSCurDataInfoStatus* pCurDataStatus,
		CExpression* pExpression,
		double& v1,double& v2,
		HSCurDataInfoStatus& calc1,
		HSCurDataInfoStatus& calc2);
	// ATG 操作(操作ID为Operator_ATG
	static double ATG(  LPNODELEMENT a,
		HSCurDataInfoStatus* pCurDataStatus,
		CExpression* pExpression,
		double& v1,double& v2,
		HSCurDataInfoStatus& calc1,
		HSCurDataInfoStatus& calc2);
	// LargerEqual 操作(操作ID为Operator_LargerEqual
	static double LargerEqual(  LPNODELEMENT a,
		HSCurDataInfoStatus* pCurDataStatus,
		CExpression* pExpression,
		double& v1,double& v2,
		HSCurDataInfoStatus& calc1,
		HSCurDataInfoStatus& calc2);
	// SmallEqual 操作(操作ID为Operator_SmallEqual
	static double SmallEqual(  LPNODELEMENT a,
		HSCurDataInfoStatus* pCurDataStatus,
		CExpression* pExpression,
		double& v1,double& v2,
		HSCurDataInfoStatus& calc1,
		HSCurDataInfoStatus& calc2);
	// NotEqual 操作(操作ID为Operator_NotEqual
	static double NotEqual(  LPNODELEMENT a,
		HSCurDataInfoStatus* pCurDataStatus,
		CExpression* pExpression,
		double& v1,double& v2,
		HSCurDataInfoStatus& calc1,
		HSCurDataInfoStatus& calc2);
	// || 操作(操作ID为Operator_OR
	static double DoubleOR(  LPNODELEMENT a,
		HSCurDataInfoStatus* pCurDataStatus,
		CExpression* pExpression,
		double& v1,double& v2,
		HSCurDataInfoStatus& calc1,
		HSCurDataInfoStatus& calc2);
	// AND && 操作(操作ID为Operator_AND
	static double AND(  LPNODELEMENT a,
		HSCurDataInfoStatus* pCurDataStatus,
		CExpression* pExpression,
		double& v1,double& v2,
		HSCurDataInfoStatus& calc1,
		HSCurDataInfoStatus& calc2);
	// Semicolon ; 操作(操作ID为Operator_Semicolon
	static double Semicolon(  LPNODELEMENT a,
		HSCurDataInfoStatus* pCurDataStatus,
		CExpression* pExpression,
		double& v1,double& v2,
		HSCurDataInfoStatus& calc1,
		HSCurDataInfoStatus& calc2);
	// Error 操作(操作ID为Operator_Error
	static double Error(  LPNODELEMENT a,
		HSCurDataInfoStatus* pCurDataStatus,
		CExpression* pExpression,
		double& v1,double& v2,
		HSCurDataInfoStatus& calc1,
		HSCurDataInfoStatus& calc2);
	// Komma , 操作(操作ID为Operator_Komma
	static double Komma(  LPNODELEMENT a,
		HSCurDataInfoStatus* pCurDataStatus,
		CExpression* pExpression,
		double& v1,double& v2,
		HSCurDataInfoStatus& calc1,
		HSCurDataInfoStatus& calc2);
	// Colon : 操作(操作ID为Operator_Colon
	static double Colon(  LPNODELEMENT a,
		HSCurDataInfoStatus* pCurDataStatus,
		CExpression* pExpression,
		double& v1,double& v2,
		HSCurDataInfoStatus& calc1,
		HSCurDataInfoStatus& calc2);
	// Evaluate 操作(操作ID为Operator_Evaluate
	static double Evaluate(  LPNODELEMENT a,
		HSCurDataInfoStatus* pCurDataStatus,
		CExpression* pExpression,
		double& v1,double& v2,
		HSCurDataInfoStatus& calc1,
		HSCurDataInfoStatus& calc2);
	// Function 操作(操作ID为Operator_Function
	static double Function(  LPNODELEMENT a,
		HSCurDataInfoStatus* pCurDataStatus,
		CExpression* pExpression,
		double& v1,double& v2,
		HSCurDataInfoStatus& calc1,
		HSCurDataInfoStatus& calc2);
	//Express 操作(操作ID为Operator_Express
	static double Express(  LPNODELEMENT a,
		HSCurDataInfoStatus* pCurDataStatus,
		CExpression* pExpression,
		double& v1,double& v2,
		HSCurDataInfoStatus& calc1,
		HSCurDataInfoStatus& calc2);
	// InportData 数据引用操作(操作ID为Operator_InportData
	static double InportData(  LPNODELEMENT a,
		HSCurDataInfoStatus* pCurDataStatus,
		CExpression* pExpression,
		double& v1,double& v2,
		HSCurDataInfoStatus& calc1,
		HSCurDataInfoStatus& calc2);
	// Default 操作(一般是从内部函数表及参数表中得到已计算的数据)
	static double Default(  LPNODELEMENT a,
		HSCurDataInfoStatus* pCurDataStatus,
		CExpression* pExpression,
		double& v1,double& v2,
		HSCurDataInfoStatus& calc1,
		HSCurDataInfoStatus& calc2);
	// ReturnValue 操作(返回数据值操作)
	static double ReturnValue(  LPNODELEMENT a,
		HSCurDataInfoStatus* pCurDataStatus,
		CExpression* pExpression,
		double& v1,double& v2,
		HSCurDataInfoStatus& calc1,
		HSCurDataInfoStatus& calc2);
	// ReturnString　返回字符串
	static double ReturnString(  LPNODELEMENT a,
		HSCurDataInfoStatus* pCurDataStatus,
		CExpression* pExpression,
		double& v1,double& v2,
		HSCurDataInfoStatus& calc1,
		HSCurDataInfoStatus& calc2);
	

public:
	static CMapVariabile*  m_pExternExpression;      // 外部表达式表(这里是指全局的公式表
	static CMapVariabile*  m_pExternVariabile;		 // 外部变量表(这里没用
	static CMapVariabile*  m_pExternFunction;		 // 外部函数表(这里是指全局的系统函数表

	// 根据函数名字从外部函数表中得到相应的系统函数指针
	static COMPILEFUNCTION GetSysFun(CString strName);
protected:
	CMapVariabile*  m_pExpDraw;				 // 内部绘图表(公式编译后产生,一般以;为一个新的公式,但不做保存

	//******************************以下是litao新增*****************************************
	CMap<CString,LPCTSTR,int,int> m_pCompNo; // 编译时标识的序列号
	//CMap<CString,LPCTSTR,int,int> m_pExecNo; // 执行时标识的序列号
	CString m_strLineObj;					 // 当前行赋值的标识
	int m_nIFWHILEStart,m_nIFWHILEEnd;		 // 是否是WHILE循环内部的编译,整数型,初始为0。1:表示第一个循环开始,当有循环嵌套时,2,3递增上去,循环变异结束后递减

	CMap<int,int,int,int> m_nWhileFirst;					// 循环内头位置
	CMap<int,int,int,int> m_nWhileLast;						// 循环内尾位置
	CMap<int,int,CString,CString&>  m_SaveExpDef;			// 循环内编译时先保存的当前的字符串

	int m_nEnd;												//计算线形的周期数
	CArray<CLineProperty*,CLineProperty*>* m_pAyData;       //从计算类传过来的线型数组
	//******************************以上是litao新增*****************************************

	CMapVariabile*  m_pParamVar;		     // 内部参数表(公式的参数设置就是设置这里

	CMapVariabile*  m_pRefExpDraw;			 // 内部绘图表
	CMapVariabile*  m_pRefInsideVar;		 // 内部变量表
	CMapVariabile*  m_pRefParamVar;		     // 内部参数表

	CExpression*	m_pRefExpress;			 // 相关联公式(内部绘图表里面的临时公式有用,指向其产生此公式的父公式

	CString			m_definitie;	// 要编译的字符串(公式的定义体
	int				m_nPozitie;		// 正在编译的当前字符串位置(-1表是已经编译OK
	LPNODELEMENT	m_Arbore;		// 以二叉树的方式表达编译的结果
	
protected:

	CString  		m_strName;		 // 表达式名称
	CString  		m_strDescribe;	 // 表达式描述
	CString		  	m_strNotes;		 // 表达式注释
	CString         m_strEidolon;	 // 参数精灵

	CString  		m_strPWD;		 // 表达式密码
	CString  		m_strDefAxis;	 // 自定义坐标，字符串坐标用分号(;)分割

	short			m_wEstopPeriod;   // 禁用周期
	short			m_wDefaultPeriod; // 缺省周期

	char			m_nExpressType;   // 公式类型


	// 公式属性
protected:
	DWORD	m_dStyle;		  // 公式风格
	// 风格相关操作(如是否为绘图输出公式等等)
	//--------2009.12.16 litao增加常用指标的保存排序---------------------------------------------------
	int   m_nDefSortId;      //常用公式的顺序号
	//-------------------------------------------------------------------------------------------------
public:
	BOOL	IsStyle(long dStyle)		{ return (m_dStyle & dStyle); }
	void	AddStyle(long dStyle)		{ m_dStyle |= dStyle;         }
	void	RemoveStyle(long dStyle)	{ m_dStyle &= (~dStyle);      }
	void    SetStyle(long dStyle)		{ m_dStyle = dStyle;          }
	DWORD	GetStyle()					{ return m_dStyle;            }
	int		GetImageIndex(); // 根据风格判断是否为默认的技术指标,是的话返回3,不是的话返回4
	void   SetParamVar(CMapVariabile*  pParamVar);//yulx add 20090316 设置参数
	CExpression *GetRefExpress(){ return m_pRefExpress;}

	int	GetDefSortId()				{ return m_nDefSortId;         }
	void	SetDefSortId(int iSortId)	{ m_nDefSortId = iSortId;      }
protected:	
	friend CFunExpresion;
	friend CSystemFun;

public:
	// 设置消息窗口(一般设为MainFram)
	static CWnd* SetCurWnd(CWnd* pMsgWnd);

public:
	/******************************************************************
	*	函数功能：	对外计算主接口
	*	函数参数：	CLineProperty*& pRefData	: [out] 计算结果存放
	*				int nStart					: [in] 计算开始位置
	*				int nEnd					: [in] 计算结果位置
	*				HSCurDataInfoStatus* pCurDataStatus = NULL : [out] 只有在表达式引用时有用,存放计算结果
	*				CTValues<void>* pRefCalcValues = NULL : [out] 只有在表达式引用时有用,存放计算结果位置(起点及终点)
	*	返 回 值：	BOOL  : 参数不正确(FALSE), 其它均为(TRUE)
	*	调用依赖：	Value()
	*	作    者：
	*	完成日期：
	*	修改说明：	
	*****************************************************************/
	//******************************以下是litao新增*****************************************
	BOOL  CalcValue(CLineProperty*& pRefData,int nStart,int nEnd,
		HSCurDataInfoStatus* pCurDataStatus = NULL,
		CTValues<void>* pRefCalcValues = NULL,CArray<CLineProperty*,CLineProperty*>* pAyData=NULL);
	//******************************以上是litao新增*****************************************

	// 得到全局的系统函数类指针
	CSystemFun* GetSystemFunAddr();


	// 公式类型
public:
	

	/******************************************************************
	*	函数功能：	当前公式是否符合查找条件
	*	函数参数：	CString strKey	: [in] 条件字串 
	*				int& nUnique    : [out] 是否整字匹配
	*				DWORD dMask = FindInName | FindInDescribe : [in] 查找范围(名字,描述及注释等)
	*	返 回 值：	BOOL  : 是否符合查找条件
	*	调用依赖：	
	*	作    者：	
	*	完成日期：	
	*	修改说明：	
	*****************************************************************/
	BOOL IsFind(CString strKey,int& nUnique,DWORD dMask = FindInName | FindInDescribe);

	/******************************************************************
	*	函数功能：	当前公式是否符合查找条件
	*	函数参数：	CString strKey	: [in] 条件字串
	*				CString& strRet : [out] 如果当前公式符合条件,则返回一公式名,公式描述及公式注释信息,否则为空
	*				DWORD dMask = FindInName | FindInDescribe : [in] 查找范围(名字,描述及注释等)
	*	返 回 值：	BOOL : 是否符合查找条件
	*	调用依赖：	
	*	作    者：	
	*	完成日期：	
	*	修改说明：	
	*****************************************************************/
	BOOL IsFind(CString strKey,CString& strRet,DWORD dMask = FindInName | FindInDescribe);

public:
	friend FunctionParam;

	// 数据源
protected:
	InportDataParam* m_pDataSource;
public:
	// 设置数据源
	void			 SetData(InportDataParam* pDataSource);
	// 得到数据源
	CHSDataSource*   GetData();

	// image index
public:
	static class CImageList*	m_expressImages;

	enum 
	{
		HS_Exp_Root  = 27, 			// 公式根更
		HS_Exp_Group = 28, 			// 组
		HS_Exp_Tech  = 7,				// 技术指标
		HS_Exp_Page = 2,   			// 页
		HS_Exp_Condition = 3,   		// 条件公式
		HS_Exp_Exchange  = 4,   		// 交易系统
		HS_Exp_MoreKLine = 8,   		// 五采K线
		HS_Exp_TechUnion = 9,   		// 指标组合
		HS_Exp_ChuQuan = 11,    		// 除权标记
		HS_Exp_CheckTech = 12,  		// 选择指标
		
		//-----2009.11.12 litao 增加绘图图标的数量-------------------------
		HS_Exp_DrawIcon0 = 18,			//笑脸
		//HS_Exp_DrawIcon1 = HS_Exp_DrawIcon0,			//笑脸
		HS_Exp_DrawIcon2 = 19,			//哭脸
		HS_Exp_DrawIcon3 = 20,			//平嘴
		HS_Exp_DrawIcon4 = 13,			//右拐
		HS_Exp_DrawIcon5 = 14,			//方！
		HS_Exp_DrawIcon6 = 15,			//斜！
		HS_Exp_DrawIcon7 = 24,			//向上红箭头
		HS_Exp_DrawIcon8 = 84,			//向下绿箭头
		HS_Exp_DrawIcon9 = 21,			//红烛
		//HS_Exp_DrawIcon10 = HS_Exp_SelectStock,		//旗帜
		HS_Exp_DrawIcon11 = 23,		//红圆点
		//HS_Exp_DrawIcon12 = HS_Exp_Active,			//红太阳
		HS_Exp_DrawIcon13 = 26,		//绿太阳
		HS_Exp_DrawIcon14 = 27,		//蓝房子
		//------------------------------------------------------------------

		HS_Exp_SelectStock = 22, 		// 选股
		HS_Exp_Active		= 25, 		// 激活状态
		HS_Exp_Disk        = 31, 		// 盘
		HS_Exp_Alarming    = 33, 		// 报警中
		HS_Exp_Alarm       = 35, 		// 报警(正常状态)
		HS_Exp_WhiteSun    = 37,	    // 白太阳
		HS_Exp_Union = 9,				// 联合选股
		HS_Exp_Bin = 27,		
		HS_Exp_MemoInfo   = 42,			// 备忘录
		HS_Exp_Union_Add = 44, 		// 组合-and
		HS_Exp_Union_Sub = 45, 		// 组合-sub
		HS_Exp_Close = 49,     		// 关闭
		HS_Exp_Info_Mark = 56, 		// 信息标记
		HS_Exp_Cost_Move = 57,			// 成本分布
		HS_Exp_Cost_HuYanShan = 58,	// 火焰山
		HS_Exp_Cons_HuoYaoDu  = 59,	// 火药度

		HS_Exp_Hor_Right = 66,			// 向右边收宿
		HS_Exp_Hor_Left  = 67,			// 向左边收宿
		HS_Exp_Hor_Down  = 68,			// 向下边收宿
		HS_Exp_Hor_Up    = 69,			// 向上边收宿

		HS_Exp_Drag_Chart = 70, 		// 拖放图形
		HS_Exp_Connect    = 71, 		// 连接
		HS_Exp_Connecting = 72, 		// 连接过程
		HS_Exp_DisConnect = 73, 		// 断开
		HS_Exp_XiaDan     = 74, 		// 下单
		HS_Exp_OpenInfo   = 75, 		// 打开信息

		HS_Exp_TableTab   = 76, 		//报表TabCtrl
		HS_Exp_AlarmOn    = 77, 		//报警中
		Exp_Image_SelStock_Selected = 87,//系统指示等选中图标
		Exright_Right = 97,
		Exright_Lift  = 98,
		Min_Icon_Ex	  = 99,
		Max_Icon_Ex	  = 100
	};

	// 根据公式类型,得到图标ID
	static int GetImgIndex(int nType);

	//******************************以下是litao新增*****************************************
public:
	CString AddObjNo(CString key,int iCompExec);
	CString SubObjNo(CString key,int iCompExec);
	CString GetObjNo(CString key,int iCompExec,int nHaveID = 0);
	CString GetMaxObjNo(CString key,int iCompExec);
	//void SetMaxObjNo(CString key,int iCompExec);
	void RemoveAllNo(int iCompExec);

	LPNODELEMENT GetExpArbore()  {return m_Arbore;}

	//******************************以上是litao新增*****************************************

};



//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(_EXPRESSION_H_)
