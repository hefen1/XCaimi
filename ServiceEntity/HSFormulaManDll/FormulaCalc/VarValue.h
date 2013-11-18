/*************************************************************
*	Copyright (c)2010, 恒生电子股份有限公司
*	All rights reserved.
*
*	文件名称：	VarValue.h
*	文件标识：	
*	摘	  要：	声明了普通变量(数字,double)的包装类
*
*	当前版本：	1.0
*	作	  者：	
*	更新日期：	
*
***************************************************************/

#if !defined(AFX_VARVALUE_H__AC8E70D6_333D_11D2_89A8_00002145DF63__INCLUDED_)
#define AFX_VARVALUE_H__AC8E70D6_333D_11D2_89A8_00002145DF63__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

/*#include "..\comm\HSexportbase.h"*/
#include "FxjFormulaStruct.h"
#include "Value.h"

/*************************************************************
*	类 名 称：	CNumericValue
*	类 描 述：	普通变量(数字,double)的包装类
***************************************************************/
class CNumericValue : public CValue  
{
	DECLARE_DYNCREATE(CNumericValue)

public:
	void			SetValueOnly(double value);//yulx add 重新设包装的值(不删除原来的空间)
	/******************************************************************
	*	函数功能：	重新设包装的值(先删除原来的空间，重新分配后再赋值)
	*	函数参数：	double value : [in] 新值
	*	返 回 值：	void		  
	*	调用依赖：	
	*	作    者：	
	*	完成日期：	2003年4月16日
	*	修改说明：	
	*****************************************************************/
	void		    SetValue(double value);

	/******************************************************************
	*	函数功能：	返回包装的值
	*	函数参数：	HSCurDataInfoStatus* pCurDataStatus = NULL: [in] 不用
	*	返 回 值：	double  : 返回的值((dobule*)m_value);
	*	调用依赖：	
	*	作    者：	
	*	完成日期：	2003年4月16日
	*	修改说明：	
	*****************************************************************/
	virtual double  GetValue(HSCurDataInfoStatus* pCurDataStatus = NULL);

	// 设置最大值
	void		    SetMaxValue(double value);
	// 得到最大值
	virtual double  GetMaxValue(void);

	// 设置最小值
	void		    SetMinValue(double value);
	// 得到最小值
	virtual double  GetMinValue(void);

	// 设置测试步长
	void		    SetTestStepValue(double value);
	// 得到测试步长
	virtual double  GetTestStepValue(void);

	// 设置对所包装的数据的描述
	void		    SetDescribe(CString strDescribe);
	// 得到对所包装的数据的描述
	virtual CString GetDescribe();

	// 初始化
	CNumericValue();
	// 清空所占资源
	virtual ~CNumericValue();

	// 串行化
	virtual void   Serialize(CArchive &ar);
	// 从分析家公式格式转换
	virtual void   Serialize(FxjParam* pParam);
	// COPY数据,数据内存不共享
	virtual void   Copy(CNumericValue* pRefValue);

protected:
	// 最大值
	void*	 m_pMax;
	// 最小值
	void*	 m_pMin;
	// 交易系统测试步长
	void*    m_pTestStep; 
	// 注释
	CString* m_pDescribe; 
};

#endif // !defined(AFX_VARVALUE_H__AC8E70D6_333D_11D2_89A8_00002145DF63__INCLUDED_)
