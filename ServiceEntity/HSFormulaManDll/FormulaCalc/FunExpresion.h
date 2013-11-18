/*************************************************************
*	Copyright (c)2010, 恒生电子股份有限公司
*	All rights reserved.
*
*	文件名称：	FunExpresion.h
*	文件标识：	
*	摘	  要：	声明了全局系统函数的包装类
*
*	当前版本：	1.0
*	作	  者：	
*	更新日期：	
*
***************************************************************/
#if !defined(AFX_FUNEXPRESION_H__2E4DE2C5_A80A_11D4_A128_00E09833EFFA__INCLUDED_)
#define AFX_FUNEXPRESION_H__2E4DE2C5_A80A_11D4_A128_00E09833EFFA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FunExpresion.h : header file
//

/*#include "..\comm\HSexportbase.h"*/

#include "Value.h"
#include "SystemFun.h"


#define FILENAME_HS_FUNCTION_HELP_CFG	"FunctionHelp.cfg"  // 公式函数帮助文件

/*************************************************************
*	类 名 称：	CFunExpresion
*	类 描 述：	系统函数指针包装类
***************************************************************/
class CExpression;
class BCGCONTROLBARDLLEXPORT CFunExpresion : public CValue
{
	DECLARE_DYNCREATE(CFunExpresion)
public:
	// 不做任务操作
	virtual void    SetValue(void* value);
	virtual double  GetValue(void);
	virtual void    UpdateValue(void);
	// 设置当前函数参数总数
	virtual BOOL    SetParams(short nParams);
	// 设置函数名
	virtual BOOL    SetFunName(CString* pFunName);	
	// 得到参数个类
	short			GetParams()	  { return m_nParams;   }
	// 得到操作ID
	unsigned short  GetOperator() { return m_nOperator; }
	// 得到函数名
	char*		    GetFunName();
	// 设置函数用法说明
	virtual void    SetNotes();
	// 得到函数用法说明
	CString			GetNotes();

	// 得到函数描述
	CString		    GetDescribe();
	// 设置函数描述
	void			SetDescribe();
	
	// 设置函数类型,同时返回旧类型
	FunctionType    SetFunType(FunctionType nType);
	// 得到函数类型
	FunctionType    GetFunType() { return m_nFunType; }
	// 函数类型判断
	BOOL		    IsFunType(FunctionType nType) { return (m_nFunType == nType); }


	/******************************************************************
	*	函数功能：	执行封装的系统函数
	*	函数参数：	CExpression* pCompile	: [in] 公式对象,主要是为了通过他得到数据源
	*				FormulaGeneralParam* ayVars : [in] 参数列表
	*				HSCurDataInfoStatus* pCurDataStatus: [out] 计算结果输出
	*				double& dValue : [out] 返回hx_DefMinValues表示计算不成功, 
										   返回hx_ReturnPointValues表示计算成功,结果放在pCurDataStatus中,
										   返回0,一般表示对应的系统函数没实现,
										   返回共它则表示返回为计算结果
	*	返 回 值：	int : 计算错误信息,为0表示正确
	*	调用依赖：	
	*	作    者：	
	*	完成日期：
	*	修改说明：	
	*****************************************************************/
	int				Evaluate(CExpression* pCompile,FormulaGeneralParam* ayVars,
							 HSCurDataInfoStatus* pCurDataStatus,double& dValue);

	CFunExpresion();
	CFunExpresion(unsigned short nOperator,
				  CString pFunName,short nParams,COMPILEFUNCTION curFn);

	// 清除占用的私人空间
	virtual ~CFunExpresion();
public:
	// 得到定义方式(输入函数,TIP提示其参数时用)如果配置文件中没例子,则得到的就是SUM( , )格式的帮助;
	int		GetFunDefMode(CString& strFun);
	// 得到例定定义方式(输入一下函数,TIP提示其参数时用),如果配置文件中没例子,则得到的就是空;
	CString GetFunDefMode();

public:
	// 本函目前不用
	BOOL EvaluateFunction(FormulaGeneralParam* ayVars); // 

protected:
	unsigned short m_nOperator;			// 操作
	FunctionType   m_nFunType;          // 函数所属分类

	CString*	   m_pFunName;			// 函数名称
	short		   m_nParams;			// 参数总数

	CString*	   m_pNotes;		    // 函数注释
	CString*  	   m_strDescribe;	    // 函数描述

	COMPILEFUNCTION	   m_curFn;			// 函数指针

public:
	// 本函目前不用
	virtual void Serialize(CArchive& ar);

public:
	/******************************************************************
	*	函数功能：	本函数中,函数名或函数描述或说明中是否含有strKey子串.查找公式时用.
	*	函数参数：	CString strKey	: [in] 要查找的子中
	*				CString& strRet	: [out] 如果找到,则返回函数名及相关信息(描述,说明并以"-"号隔开)
	*				DWORD dMask = FindInName | FindInDescribe : [in] 查找的范围
	*	返 回 值：	BOOL : 在规定范围内找到字串则返回TRUE,否则返回FALSE
	*	调用依赖：	
	*	作    者：	
	*	完成日期：
	*	修改说明：	
	*****************************************************************/
	virtual BOOL Find(CString strKey,CString& strRet,DWORD dMask = FindInName | FindInDescribe);

public:
	// 返回封装的系统函数
	COMPILEFUNCTION	 GetCurFun()  { return m_curFn; }
};
// 全局的系统函数类对象(包含了所有定义的公式底层系统函数)
extern CSystemFun* g_pSystemFun;

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FUNEXPRESION_H__2E4DE2C5_A80A_11D4_A128_00E09833EFFA__INCLUDED_)
