/*************************************************************
*	Copyright (c)2010, 恒生电子股份有限公司
*	All rights reserved.
*
*	文件名称：	ExpValue.h
*	文件标识：	
*	摘	  要：	声明了公式对象的包装类
*
*	当前版本：	1.0
*	作	  者：	
*	更新日期：
*
***************************************************************/

#if !defined(AFX_EXPVALUE_H__AC8E70D5_333D_11D2_89A8_00002145DF63__INCLUDED_)
#define AFX_EXPVALUE_H__AC8E70D5_333D_11D2_89A8_00002145DF63__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include <afxmt.h>

/*#include "..\comm\HSexportbase.h"*/

#include "Value.h"
#include "FxjFormulaStruct.h"
#include "CompileInterFace.h"
//#include "UnionSelCondition.h"

/*************************************************************
*	类 名 称：	CExpValue
*	类 描 述：	公式对象的包装类
***************************************************************/
class BCGCONTROLBARDLLEXPORT CExpValue : public CValue  
{
	DECLARE_DYNCREATE(CExpValue)

public:

	/******************************************************************
	*	函数功能：	重新编译所包装的公式
	*	函数参数：	void
	*	返 回 值：	void  
	*	调用依赖：	
	*	作    者：	
	*	完成日期：
	*	修改说明：	
	*****************************************************************/
	virtual void   UpdateValue(void);
	

	/******************************************************************
	*	函数功能：	所包装的公式进行计算
	*	函数参数：	HSCurDataInfoStatus* pCurDataStatus = NULL : [out] 计算结果
	*	返 回 值：	double : 返回hx_DefMinValues表示计算不成功, 
							 返回hx_ReturnPointValues表示计算成功,结果放在pCurDataStatus中,
							 返回共它则表示返回为计算结果
	*	调用依赖：	
	*	作    者：	
	*	完成日期：
	*	修改说明：	
	*****************************************************************/
	virtual double GetValue(HSCurDataInfoStatus* pCurDataStatus = NULL);

	/******************************************************************
	*	函数功能：	所包装的公式进行计算,并将计算结果进行引用(用m_pRefCalcValues指向实现)
	*	函数参数：	CLineProperty*& pRefData: [out] 计算结果存放
	*				int nStart				: [in] 计算开始位置
	*				int nEnd				: [in] 计算结束位置
	*				HSCurDataInfoStatus* pCurDataStatus = NULL: 目前没用
	*	返 回 值：	BOOL  : 计算是否成功
	*	调用依赖：	
	*	作    者：	
	*	完成日期：
	*	修改说明：	
	*****************************************************************/
	//******************************以下是litao新增*****************************************
	virtual BOOL   CalcValue(CLineProperty*& pRefData,int nStart,int nEnd,HSCurDataInfoStatus* pCurDataStatus = NULL,
		bool bEnd = false,CArray<CLineProperty*,CLineProperty*>* pAyData=NULL);
	//******************************以上是litao新增*****************************************


	/******************************************************************
	*	函数功能：	所包装公式的公式树及内部绘图公式列表的删除,以及本类的引用结果和参数列表的删除
	*	函数参数：	（无）
	*	返 回 值：	void	  
	*	调用依赖：	
	*	作    者：	
	*	完成日期：
	*	修改说明：	
	*****************************************************************/
	virtual void	   Free();
	// 返回所包装的公式
	class CExpression* GetExp();
	// 返回联合公式
	virtual UnionSelCondition*  GetUnion();
	// 返回编译后的公式
	class CExpression* GetExpByCompile();

	// 得到公式或联合的整个名字(公式或联合名字前面加入了公式所在组的类型号)
	virtual CString GetWholeName();

	// 初始化
	CExpValue();
	// 清空占用的资源
	virtual ~CExpValue();
	
	// 串行化,保存
	virtual void   Serialize(CArchive &ar);
	virtual void   Serialize(FxjExpress*  pFxjExpress);
	virtual void   Serialize(CExpression* pExpress);

	// 改名操作返回
	enum ReNameState { Fail/*参数不正确*/,Equal/*同名*/,Exist/*名字已存在*/,SysExist/*名字已存在于全局的外部函公式中*/,NoFind/*旧公式不在全局的外部函公式列表中*/,Succ /*成功*/};

	/******************************************************************
	*	函数功能：	更改所包装的公式名
	*	函数参数：	CString strNewName: [in] 新的公式名
	*				CMapVariabile* pExternExpression: [in] 公式所在的表
	*	返 回 值：	int : 改名结果(enum ReNameState)
	*	调用依赖：	
	*	作    者：	
	*	完成日期：	2003年4月16日
	*	修改说明：	
	*****************************************************************/
	int ReName( CString strNewName,CMapVariabile* pExternExpression );

	void RemoveAllNo(int iCompExec);

protected:
	CTValues<void>		m_pRefCalcValues; // 表达式已计算的结果(主要用于间接表达式)
	CMapVariabile*		m_pParam;		  // 相关参数表
										/* 规则：参数1 -> 图形绘制方式(却省为曲线)
												 参数2 -> 图形绘制颜色(却省为自动取)
												 参数3 -> 图形绘制的线风格(却省为线)
												 参数4 -> 图形绘制的线宽度(却省为自动取)											
										*/

public:
	// 返回&m_pRefCalcValues
	CTValues<void>*	GetRefCalcValues();
	// 返回m_pParam// 相关参数表
	CMapVariabile*  GetParam();		 	
	// 加一项参数到相关参数表
	void			AddParam(CValue* pExpress);
	CString	 m_LineName;       //当前赋值语句的标识
	bool	 m_InWhile;		   //当前赋值语句是否在循环内	
	bool     m_CurExecInWhile;  //当前执行语句是否在循环内了，防止循环内的语句在循环后又被调用。因为while函数在循环内语句之前。
	int		 m_WhileLevel;		//当前编译语句是第几层次WHILE循环
	int		 m_CurExecWhileLevel;		//当前编译语句是第几层次WHILE循环

public:
	void SetMaxObjNo(CString key,int iCompExec);

public:
	// 清空占用的资源
	void Clean();

public:
	/******************************************************************
	*	函数功能：	包含的公式中,公式名、公式描述或说明中是否含有strKey子串.查找公式时用.
	*	函数参数：	CString strKey	: [in] 要查找的子中
	*				CString& strRet	: [out] 如果找到,则返回函数名及相关信息(描述,说明并以"-"号隔开)
	*				DWORD dMask = FindInName | FindInDescribe : [in] 查找的范围
	*	返 回 值：	BOOL : 在规定范围内找到字串则返回TRUE,否则返回FALSE
	*	调用依赖：	CExpression::IsFind()
	*	作    者：	
	*	完成日期：
	*	修改说明：	
	*****************************************************************/
	virtual BOOL Find(CString strKey,CString& strRet,DWORD dMask = FindInName | FindInDescribe);

	/******************************************************************
	*	函数功能：	返回全局的指标比较方式（大于, 小于， 等于...）
	*	函数参数：	int& nCount: [out] 比较方式的个数
	*	返 回 值：	char** : 比较方式的文本
	*	调用依赖：	
	*	作    者：	
	*	完成日期：	2003年4月16日
	*	修改说明：	
	*****************************************************************/
	static char** GetConditionStr(int& nCount);
};

extern void    AddPeriodNameToWnd(CComboBox* pBox);
extern int     GetPeriodNumByName(CString strPeriodName);
extern CString GetPeriodStr(WORD dPeriod,int* nPos = NULL);

CMap<CString,LPCTSTR,LPEXECOBJNO,LPEXECOBJNO> m_pCompNo; // 执行时标识的序列号
CMap<CString,LPCTSTR,LPEXECOBJNO,LPEXECOBJNO> m_pExecNo; // 执行时标识的序列号
#endif // !defined(AFX_EXPVALUE_H__AC8E70D5_333D_11D2_89A8_00002145DF63__INCLUDED_)
