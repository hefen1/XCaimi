 /*************************************************************
*	Copyright (c)2010, 恒生电子股份有限公司
*	All rights reserved.
*
*	文件名称：	FormulaManPublic.h
*	文件标识： 	
*	摘	  要： 公式公共头文件	
*
*	当前版本：	1.0
*	作	  者：	
*	更新日期：
*
***************************************************************/

#ifndef FORMULAMANPUBLIC
#define FORMULAMANPUBLIC

#include "ConfigStruct.h"
#include "hsds_comudata.h" 
#include "hsothercoldefine.h"
#pragma pack(1) 


 //图元属性
#define HS_DRAWICON    1
#define HS_DRAWLINE    2
#define HS_DRAWTEXT    3
#define HS_POLYLINE    4
#define HS_STICKLINE   5 
#define HS_HLTHBQ	    6 
#define HS_BUYSELL	    7 
#define HS_DRAWSL	    8
 
#ifndef hx_DefMaxMin
#define hx_DefMaxMin
static long hx_DefMaxValues		 = 0x80000000;	   // 最小值
static long hx_DefMinValues		 = 0x7fffffff;	   // 最大值
static long hx_ReturnPointValues    = 0x5fffffff;     // 计算数值返回指针
#endif

// 公式属性
#define HS_EXP_DEFAULT_TECH 0x0001 // 缺省指标/常用指标
#define HS_EXP_USES		 0x0002 // 在使用中
#define HS_EXP_DELETE		 0x0004 // 彻底删除
#define HS_EXP_MAINCHART	 0x0008 // 主图叠加
#define HS_EXP_SUBCHART	 0x0010 // 副图
#define HS_EXP_USEREDIT	 0x0020 // 自编公式
#define HS_EXP_SYSTEM  	 0x0040 // 系统公式
#define HS_EXP_TEMPIN		 0x8000 // 临时引入的公式
#define HS_EXP_FINANCE		 0x10000000 // 财务数据输出的公式
#define HS_EXP_PROTECTED	 0x20000000 // 公式是否为保护的

#define HS_EXP_OUTTYPE(x)   ( (x) & 0xFF000000 ) // 公式输出mask

#define HS_HxDataSource_Left_ChuQuan	    0x00020000 // 左边除权
#define HS_HxDataSource_Back_ChuQuan		0x00040000 // 向后除权
#define HS_HxDataSource_Read_CaiWu_Data     0x00060000 // 读取财务数据

#define HX_NOTTABLE			   0x00080000  // 为绘图表达式,但是不在表格里显示
#define HX_NOTEXPRESSDRAW	   0x00100000  // 为绘图表达式,但是不绘图
#define HX_EXPRESSDRAW		   0x00200000  // 为绘图表达式
#define HX_EXPRESSDRAW_NONAME  0x00400000  // 为绘图表达式(没有名称)

#define HX_EXPRESSSENTENCE	   0x00800000  // 为语句表达式
#define HX_EXPRESSNUMBER	   0x01000000  // 为数值表达式
#define HX_EXPRESSFUNCTION	   0x02000000  // 为函数表达式
#define HX_EXPRESSREFERENCE	   0x04000000  // 为引用语句表达式
#define HX_UNION_EXPRESS	   0x08000000  // 为组合表达式
#define HX_UNKNOWFUNPARAM	   0x10000000  // 为函数表达式,其参数不定
#define HX_NOTCALCMAXMIN	   0x40000000  // 不计算最大最小值

#define hs_NameProperty_NotDrawName		0x0100 // 不绘制名称
#define hs_NameProperty_NotDrawExp		0x0200 // 不绘制参数
#define hs_NameProperty_DrawRectangle   0x0400 // 绘制文本边框
#define hs_NameProperty_DrawDescribe	0x0800 // 绘制名称描述
#define hs_NameProperty_NotCompile	    0x1000 // 当前公式没有建立/编译


#define hxOpenExpress_Edit				     0x0100    //修改公式
#define hxOpenExpress_Notes					 0x0200	   //注释
#define hxOpenExpress_Eidolon				 0x0300	   //参数

#define hxShowAlignWindow					 0x0400
#define hxShowKeyboardWindow				 0x0600

#define hxShowByExpressType					 0x0700 // 由公式类型来显示公式
#define hxShowByExpressType_AddTech			 0x0010 // 指标叠加
#define hxShowByExpressType_TechSort		 0x0020 // 指标排序
#define hxShowByExpressType_AddTechSort		 0x0030 // 添加指标排序

#define hxShowReturnByExpressType			 0x0800 // 由公式类型来显示公式,并返回选中公式

#define hxOpenExpressByConditionStockType	 0x0900 // 条件选股打开
#define hxOpenExpressByConditionDingZhi		 0x0910 // 定制选股打开
#define hxOpenExpressByConditionZhiBiao		 0x0920 // 指标选股打开
#define hxOpenExpressByConditionZhiNeng		 0x0930 // 智能选股打开
#define hxOpenExpressByConditionChaJian		 0x0940 // 插件选股打开
#define hxOpenExchangeSys            		 0x0950 // 交易系统
#define hxOpenExchangeYouSel				 0x0960	// 交易优选

#define hxOpenExpressBySortStockType		 0x0A00 // 指标排序打开
#define hxOpenBlockAnalysis					 0x0B00 // 板块分析
#define	hxOpenUniteAnalysis					 0x0C00 // 联动分析
#define	hxOpenFindDataDlg					 0x0D00 // 打开查找dlg
#define	hxOpenAlarmManagerDlg				 0x0E00 // 打开预警管理对话框
#define hxOpenAddStkToAlarm					 0x1000 // 打开加入到预警对话框

#define hsOpenExpress_Create				 0x0F00 // 快速新建技术指标 

#define hxSaveLoadTree						 0x2000 // 保存公式树
#define hxSettingExpressParent				 0x3000 // 设置公式dll父窗口

#define hsOpenStrategyTestSetting			 0x4000	// 打开策略交易参数设定窗口


enum HSAllocType {charType,InfoIndexType,InfoIndexSubType,HSDrawOtherDataType,HSDoubleType,HSCString,HSCStringCopy};
enum ExpressType { none,
		Tech = 0x01,Condition = 0x02,Exchange = 0x04,MoreKLine = 0x08,
		ExpressBin = 0x10, Strategy = 0x20, Union,JiBenMian};

typedef void* ( *HSExternAlloc )( long nCurSize,int nType  );
typedef void  ( *HSExternFree )( void* pData, int nType  );



struct HSDrawOtherData
{
 	BYTE	 m_bType1;
 	BYTE	 m_bType2;
 	BYTE	 m_bType3;
 	BYTE	 m_bType4; 
	double	 m_dValue;    // 数字 
	char     m_cText[50];
	
 	HSDrawOtherData()
 	{
 		Empty();
	}

 	~HSDrawOtherData()
 	{
 	}
 	void Copy( HSDrawOtherData* pData );
 	void Empty()
 	{
 		m_bType1 = 0;
 		m_bType2 = 0;
 		m_bType3 = 0;
 		m_bType4 = 0; 
 		m_dValue = 0;
		memset(m_cText,0,sizeof(m_cText));
 	}
 };
 
 struct HSDrawData
 {
	 // 数据
	 double   m_dData;       
	 HSDrawOtherData*   m_pOtherData;  
	 static HSExternAlloc m_pHSExternAlloc;
	 static HSExternFree  m_pHSExternFree;

	 //构造
	 HSDrawData()
	 {
		 m_dData = hx_DefMinValues;
		 m_pOtherData = NULL;
	 }
	 //构造 复制类型数据
	 HSDrawData(HSDrawData& sData)
	 {
		 m_dData = sData.m_dData;
		 m_pOtherData = NULL;
	 }

	 //构造 复制double类型数据
	 HSDrawData(double dData)
	 {
		 m_dData   = dData;
		 m_pOtherData = NULL;
	 }

	 ~HSDrawData()
	 {
		FreeOther();
	 }
	 void init()
	 {
		 m_dData = hx_DefMinValues;
	 }
	 void FreeOther();
	 HSDrawOtherData* AllocOther(HSDrawOtherData* pData,BOOL bForce = FALSE);

	 void Copy(HSDrawData* pData,BOOL bMsg = FALSE);

#ifdef VC_7_0
	 //构造 复制long类型数据
	 HSDrawData(long lData)
	 {
		 m_dData   = lData;
		 m_pOtherData = NULL;
	 }
	 //构造 复制int类型数据
	 HSDrawData(int dData)
	 {
		 m_dData   = dData;
		 m_pOtherData = NULL;
	 }
	 //构造 复制BYTE类型数据
	 HSDrawData(BYTE dData)
	 {
		 m_dData   = dData;
		 m_pOtherData = NULL;
	 }
	 //构造 复制unsigned long类型数据
	 HSDrawData(unsigned long dData)
	 {
		 m_dData   = dData;
		 m_pOtherData = NULL;
	 }
#endif

	 //操作符= 将复制double 类型数据
	 const HSDrawData& operator=(double dData)
	 {
		 m_dData = dData;
		 //m_pOtherData = NULL;
		 return *this;
	 }

	 //操作符=, 将复制HSDrawData 类型数据
	 const HSDrawData& operator=(HSDrawData& dData)
	 {
		 m_dData	= dData.m_dData;
		 //m_pOtherData = NULL;
		 return *this;
	 }

 #ifdef VC_7_0

	 //操作符=, 将复制long 类型数据
	 const HSDrawData& operator=(long lData)
	 {
		 m_dData = lData;
		 //m_pOtherData = NULL;
		 return *this;
	 }
	 //操作符=, 将复制int 类型数据
	 const HSDrawData& operator=(int nData)
	 {
		 m_dData = nData;
		 //m_pOtherData = NULL;
		 return *this;
	 }
	 //操作符=, 将复制BYTE 类型数据
	 const HSDrawData& operator=(BYTE bData)
	 {
		 m_dData = bData;
		 //m_pOtherData = NULL;
		 return *this;
	 }

	 const HSDrawData& operator=(unsigned long bData)
	 {
		 m_dData = bData;
		 //m_pOtherData = NULL;
		 return *this;
	 }

#endif

	 ///////////
	 //操作符 +运算:m_dData += dData.m_dData;
	 const HSDrawData& operator+=(const HSDrawData& dData)
	 {
		 m_dData += dData.m_dData;
		 return *this;
	 }
	 //操作符 +
	 const HSDrawData& operator+=(double dData)
	 {
		 m_dData += dData;
		 return *this;
	 }


	 //操作符  - 运算:m_dData -= dData.m_dData;
	 const HSDrawData& operator-=(const HSDrawData& dData)
	 {
		 m_dData -= dData.m_dData;
		 return *this;
	 }
	 const HSDrawData& operator-=(double dData)
	 {
		 m_dData -= dData;
		 return *this;
	 }

	 //操作符 * 运算:m_dData *= dData.m_dData;
	 const HSDrawData& operator*=(const HSDrawData& dData)
	 {
		 m_dData *= dData.m_dData;
		 return *this;
	 }
	 const HSDrawData& operator*=(double dData)
	 {
		 m_dData *= dData;
		 return *this;
	 }

	 //操作符 / 运算:m_dData /= dData.m_dData;
	 const HSDrawData& operator/=(const HSDrawData& dData)
	 {
		 m_dData /= dData.m_dData;
		 return *this;
	 }
	 const HSDrawData& operator/=(double dData)
	 {
		 m_dData /= dData;
		 return *this;
	 }

	 bool operator!=(double dData)
	 {
		 return (m_dData != dData);
	 }

	 bool operator!=(HSDrawData dData)
	 {
		 return (m_dData != dData.m_dData);
	 }

	 //操作符 数据类型转化
	 operator double()
	 {
		 return m_dData;
	 }
	 //操作符 数据类型转化
	 operator long()
	 {
		 return (long)m_dData;
	 }
	 //操作符 数据类型转化
	 operator int()
	 {
		 return (int)m_dData;
	 }
	 //操作符 数据类型转化
	 operator BYTE()
	 {
		 return (BYTE)m_dData;
	 }
	 //操作符 数据类型转化
	 operator bool()
	 {
		 return m_dData?true:false;
	 }
	 operator unsigned long()
	 {
		 return (unsigned long)m_dData;
	 }	
 };

typedef  HSDrawData HSDouble;

 //操作符重载 == 返回true or false
 inline bool AFXAPI operator==(const HSDrawData& s1, const HSDrawData& s2)
 {
 	return (s1.m_dData == s2.m_dData);
 }
 //操作符重载 == 返回true or false
 inline bool AFXAPI operator==(const HSDrawData& s1, double s2)
 {
 	return (s1.m_dData == s2);
 }
 //操作符重载 == 返回true or false
 inline bool AFXAPI operator==(double s1, const HSDrawData& s2)
 {
 	return (s1 == s2.m_dData);
 }
 //操作符重载 == 返回true or false
 inline bool AFXAPI operator==(const HSDrawData& s1, long s2)
 {
 	return (s1.m_dData == s2);
 }
 inline bool AFXAPI operator==(const HSDrawData& s1, unsigned long s2)
 {
 	return (s1.m_dData == s2);
 }
 //操作符重载 == 返回true or false
 inline bool AFXAPI operator==(long s1, const HSDrawData& s2)
 {
 	return (s1 == s2.m_dData);
 }
 //操作符重载 == 返回true or false
 inline bool AFXAPI operator==(const HSDrawData& s1, int s2)
 {
 	return (s1.m_dData == s2);
 }
 //操作符重载 == 返回true or false
 inline bool AFXAPI operator==(int s1, const HSDrawData& s2)
 {
 	return (s1 == s2.m_dData);
 }
 //操作符重载 == 返回true or false
 inline bool AFXAPI operator==(const HSDrawData& s1, BYTE s2)
 {
 	return (s1.m_dData == s2);
 }
 //操作符重载 == 返回true or false
 inline bool AFXAPI operator==(BYTE s1, const HSDrawData& s2)
 {
 	return (s1 == s2.m_dData);
 }
 
 //操作符重载 != 返回true or false
 inline bool AFXAPI operator!=(const HSDrawData& s1, const HSDrawData& s2)
 {
 	return (s1.m_dData != s2.m_dData);
 }
 //操作符重载 != 返回true or false
 inline bool AFXAPI operator!=(const HSDrawData& s1, double s2)
 {
 	return (s1.m_dData != s2);
 }
 //操作符重载 != 返回true or false
 inline bool AFXAPI operator!=(double s1, const HSDrawData& s2)
 {
 	return (s1 != s2.m_dData);
 }
 //操作符重载 != 返回true or false
 inline bool AFXAPI operator!=(const HSDrawData& s1, long s2)
 {
 	return (s1.m_dData != s2);
 }
 inline bool AFXAPI operator!=(const HSDrawData& s1, unsigned long s2)
 {
 	return (s1.m_dData != s2);
 }
 //操作符重载 != 返回true or false
 inline bool AFXAPI operator!=(long s1, const HSDrawData& s2)
 {
 	return (s1 != s2.m_dData);
 }
 //操作符重载 != 返回true or false
 inline bool AFXAPI operator!=(const HSDrawData& s1, int s2)
 {
 	return (s1.m_dData != s2);
 }
 //操作符重载 != 返回true or false
 inline bool AFXAPI operator!=(int s1, const HSDrawData& s2)
 {
 	return (s1 != s2.m_dData);
 }
 //操作符重载 != 返回true or false
 inline bool AFXAPI operator!=(const HSDrawData& s1, BYTE s2)
 {
 	return (s1.m_dData != s2);
 }
 //操作符重载 != 返回true or false
 inline bool AFXAPI operator!=(BYTE s1, const HSDrawData& s2)
 {
 	return (s1 != s2.m_dData);
 }
 
 //操作符重载 < 返回true or false
 inline bool AFXAPI operator<(const HSDrawData& s1, const HSDrawData& s2)
 {
 	return (s1.m_dData < s2.m_dData);
 }
 //操作符重载 < 返回true or false
 inline bool AFXAPI operator<(const HSDrawData& s1, double s2)
 {
 	return (s1.m_dData < s2);
 }
 //操作符重载 < 返回true or false
 inline bool AFXAPI operator<(double s1, const HSDrawData& s2)
 {
 	return (s1 < s2.m_dData);
 }
 //操作符重载 < 返回true or false
 inline bool AFXAPI operator<(const HSDrawData& s1, long s2)
 {
 	return (s1.m_dData < s2);
 }
 inline bool AFXAPI operator<(const HSDrawData& s1,unsigned long s2)
 {
 	return (s1.m_dData < s2);
 }
 //操作符重载 < 返回true or false
 inline bool AFXAPI operator<(long s1, const HSDrawData& s2)
 {
 	return (s1 < s2.m_dData);
 }
 //操作符重载 < 返回true or false
 inline bool AFXAPI operator<(const HSDrawData& s1, int s2)
 {
 	return (s1.m_dData < s2);
 }
 //操作符重载 < 返回true or false
 inline bool AFXAPI operator<(int s1, const HSDrawData& s2)
 {
 	return (s1 < s2.m_dData);
 }
 //操作符重载 < 返回true or false
 inline bool AFXAPI operator<(const HSDrawData& s1, BYTE s2)
 {
 	return (s1.m_dData < s2);
 }
 //操作符重载 < 返回true or false
 inline bool AFXAPI operator<(BYTE s1, const HSDrawData& s2)
 {
 	return (s1 < s2.m_dData);
 }
 
 //操作符重载 > 返回true or false
 inline bool AFXAPI operator>(const HSDrawData& s1, const HSDrawData& s2)
 {
 	return (s1.m_dData > s2.m_dData);
 }
 //操作符重载 > 返回true or false
 inline bool AFXAPI operator>(const HSDrawData& s1, double s2)
 {
 	return (s1.m_dData > s2);
 }
 //操作符重载 > 返回true or false
 inline bool AFXAPI operator>(double s1, const HSDrawData& s2)
 {
 	return (s1 > s2.m_dData);
 }
 //操作符重载 > 返回true or false
 inline bool AFXAPI operator>(const HSDrawData& s1, long s2)
 {
 	return (s1.m_dData > s2);
 }
 inline bool AFXAPI operator>(const HSDrawData& s1,unsigned long s2)
 {
 	return (s1.m_dData > s2);
 }
 //操作符重载 > 返回true or false
 inline bool AFXAPI operator>(long s1, const HSDrawData& s2)
 {
 	return (s1 > s2.m_dData);
 }
 //操作符重载 > 返回true or false
 inline bool AFXAPI operator>(const HSDrawData& s1, int s2)
 {
 	return (s1.m_dData > s2);
 }
 //操作符重载 > 返回true or false
 inline bool AFXAPI operator>(int s1, const HSDrawData& s2)
 {
 	return (s1 > s2.m_dData);
 }
 //操作符重载 > 返回true or false
 inline bool AFXAPI operator>(const HSDrawData& s1, BYTE s2)
 {
 	return (s1.m_dData > s2);
 }
 //操作符重载 > 返回true or false
 inline bool AFXAPI operator>(BYTE s1, const HSDrawData& s2)
 {
 	return (s1 > s2.m_dData);
 }
 
 ///操作符重载 <= 返回true or false
 inline bool AFXAPI operator<=(const HSDrawData& s1, const HSDrawData& s2)
 {
 	return (s1.m_dData <= s2.m_dData);
 }
 ///操作符重载 <= 返回true or false
 inline bool AFXAPI operator<=(const HSDrawData& s1, double s2)
 {
 	return (s1.m_dData <= s2);
 }
 ///操作符重载 <= 返回true or false
 inline bool AFXAPI operator<=(double s1, const HSDrawData& s2)
 {
 	return (s1 <= s2.m_dData);
 }
 ///操作符重载 <= 返回true or false
 inline bool AFXAPI operator<=(const HSDrawData& s1, long s2)
 {
 	return (s1.m_dData <= s2);
 }
 inline bool AFXAPI operator<=(const HSDrawData& s1,unsigned long s2)
 {
 	return (s1.m_dData <= s2);
 }
 ///操作符重载 <= 返回true or false
 inline bool AFXAPI operator<=(long s1, const HSDrawData& s2)
 {
 	return (s1 <= s2.m_dData);
 }
 ///操作符重载 <= 返回true or false
 inline bool AFXAPI operator<=(const HSDrawData& s1, int s2)
 {
 	return (s1.m_dData <= s2);
 }
 ///操作符重载 <= 返回true or false
 inline bool AFXAPI operator<=(int s1, const HSDrawData& s2)
 {
 	return (s1 <= s2.m_dData);
 }
 ///操作符重载 <= 返回true or false
 inline bool AFXAPI operator<=(const HSDrawData& s1, BYTE s2)
 {
 	return (s1.m_dData <= s2);
 }
 ///操作符重载 <= 返回true or false
 inline bool AFXAPI operator<=(BYTE s1, const HSDrawData& s2)
 {
 	return (s1 <= s2.m_dData);
 }
 
 //操作符重载 >= 返回true or false
 inline bool AFXAPI operator>=(const HSDrawData& s1, const HSDrawData& s2)
 {
 	return (s1.m_dData >= s2.m_dData);
 }
 //操作符重载 >= 返回true or false
 inline bool AFXAPI operator>=(const HSDrawData& s1, double s2)
 {
 	return (s1.m_dData >= s2);
 }
 //操作符重载 >= 返回true or false
 inline bool AFXAPI operator>=(double s1, const HSDrawData& s2)
 {
 	return (s1 >= s2.m_dData);
 }
 //操作符重载 >= 返回true or false
 inline bool AFXAPI operator>=(const HSDrawData& s1, long s2)
 {
 	return (s1.m_dData >= s2);
 }
 inline bool AFXAPI operator>=(const HSDrawData& s1,unsigned long s2)
 {
 	return (s1.m_dData >= s2);
 }
 //操作符重载 >= 返回true or false
 inline bool AFXAPI operator>=(long s1, const HSDrawData& s2)
 {
 	return (s1 >= s2.m_dData);
 }
 //操作符重载 >= 返回true or false
 inline bool AFXAPI operator>=(const HSDrawData& s1, int s2)
 {
 	return (s1.m_dData >= s2);
 }
 //操作符重载 >= 返回true or false
 inline bool AFXAPI operator>=(int s1, const HSDrawData& s2)
 {
 	return (s1 >= s2.m_dData);
 }
 //操作符重载 >= 返回true or false
 inline bool AFXAPI operator>=(const HSDrawData& s1, BYTE s2)
 {
 	return (s1.m_dData >= s2);
 }
 //操作符重载 >= 返回true or false
 inline bool AFXAPI operator>=(BYTE s1, const HSDrawData& s2)
 {
 	return (s1 >= s2.m_dData);
 }
 
 //操作符重载 + 返回HSDrawData
 inline HSDrawData AFXAPI operator+(const HSDrawData& string1,const HSDrawData& string2)
 {
 	return HSDrawData(string1.m_dData + string2.m_dData);
 }
 //操作符重载 + 返回HSDrawData
 inline HSDrawData AFXAPI operator+(const HSDrawData& string, double ch)
 {
 	return HSDrawData(string.m_dData + ch);
 }
 //操作符重载 + 返回HSDrawData
 inline HSDrawData AFXAPI operator+(double ch, const HSDrawData& string)
 {
 	return HSDrawData(ch + string.m_dData);
 }
 //操作符重载 + 返回HSDrawData
 inline HSDrawData AFXAPI operator+(const HSDrawData& string, long lpsz)
 {
 	return HSDrawData(string.m_dData + lpsz);
 }
 inline HSDrawData AFXAPI operator+(const HSDrawData& string,unsigned long lpsz)
 {
 	return HSDrawData(string.m_dData + lpsz);
 }
 //操作符重载 + 返回HSDrawData
 inline HSDrawData AFXAPI operator+(long lpsz, const HSDrawData& string)
 {
 	return HSDrawData(lpsz + string.m_dData);
 }
 
 //操作符重载 - 返回HSDrawData
 inline HSDrawData AFXAPI operator-(const HSDrawData& string1,const HSDrawData& string2)
 {
 	return HSDrawData(string1.m_dData - string2.m_dData);
 }
 //操作符重载 - 返回HSDrawData
 inline HSDrawData AFXAPI operator-(const HSDrawData& string, double ch)
 {
 	return HSDrawData(string.m_dData - ch);
 }
 //操作符重载 - 返回HSDrawData
 inline HSDrawData AFXAPI operator-(double ch, const HSDrawData& string)
 {
 	return HSDrawData(ch - string.m_dData);
 }
 //操作符重载 - 返回HSDrawData
 inline HSDrawData AFXAPI operator-(const HSDrawData& string, long lpsz)
 {
 	return HSDrawData(string.m_dData - lpsz);
 }
 inline HSDrawData AFXAPI operator-(const HSDrawData& string,unsigned long lpsz)
 {
 	return HSDrawData(string.m_dData - lpsz);
 }
 //操作符重载 - 返回HSDrawData
 inline HSDrawData AFXAPI operator-(long lpsz, const HSDrawData& string)
 {
 	return HSDrawData(lpsz - string.m_dData);
 }
 
 //操作符重载 * 返回HSDrawData
 inline HSDrawData AFXAPI operator*(const HSDrawData& string1,const HSDrawData& string2)
 {
 	return HSDrawData(string1.m_dData * string2.m_dData);
 }
 //操作符重载 * 返回HSDrawData
 inline HSDrawData AFXAPI operator*(const HSDrawData& string, double ch)
 {
 	return HSDrawData(string.m_dData * ch);
 }
 //操作符重载 * 返回HSDrawData
 inline HSDrawData AFXAPI operator*(double ch, const HSDrawData& string)
 {
 	return HSDrawData(ch * string.m_dData);
 }
 //操作符重载 * 返回HSDrawData
 inline HSDrawData AFXAPI operator*(const HSDrawData& string, long lpsz)
 {
 	return HSDrawData(string.m_dData * lpsz);
 }
 inline HSDrawData AFXAPI operator*(const HSDrawData& string,unsigned long lpsz)
 {
 	return HSDrawData(string.m_dData * lpsz);
 }
 //操作符重载 * 返回HSDrawData
 inline HSDrawData AFXAPI operator*(long lpsz, const HSDrawData& string)
 {
 	return HSDrawData(lpsz * string.m_dData);
 }
 
 //操作符重载 / 返回HSDrawData
 inline HSDrawData AFXAPI operator/(const HSDrawData& string1,const HSDrawData& string2)
 {
 	return HSDrawData(string1.m_dData / string2.m_dData);
 }
 //操作符重载 / 返回HSDrawData
 inline HSDrawData AFXAPI operator/(const HSDrawData& string, double ch)
 {
 	return HSDrawData(string.m_dData / ch);
 }
 //操作符重载 / 返回HSDrawData
 inline HSDrawData AFXAPI operator/(double ch, const HSDrawData& string)
 {
 	return HSDrawData(ch / string.m_dData);
 }
 //操作符重载 / 返回HSDrawData
 inline HSDrawData AFXAPI operator/(const HSDrawData& string, long lpsz)
 {
 	return HSDrawData(string.m_dData / lpsz);
 }
 inline HSDrawData AFXAPI operator/(const HSDrawData& string,unsigned long lpsz)
 {
 	return HSDrawData(string.m_dData / lpsz);
 }
 //操作符重载 / 返回HSDrawData
 inline HSDrawData AFXAPI operator/(long lpsz, const HSDrawData& string)
 {
 	return HSDrawData(lpsz / string.m_dData);
 }
 
 ///////////////
 //操作符重载 ! 返回true or false
 inline bool AFXAPI operator!(const HSDrawData& s1)
 {
 	return !s1.m_dData;
 }
 
 //操作符重载 || 返回true or false
 inline bool AFXAPI operator||(const HSDrawData& s1, const HSDrawData& s2)
 {
 	return (s1.m_dData || s2.m_dData);
 }
 //操作符重载 || 返回true or false
 inline bool AFXAPI operator||(const HSDrawData& s1, double s2)
 {
 	return (s1.m_dData || s2);
 }
 //操作符重载 || 返回true or false
 inline bool AFXAPI operator||(double s1, const HSDrawData& s2)
 {
 	return (s1 || s2.m_dData);
 }
 //操作符重载 || 返回true or false
 inline bool AFXAPI operator||(const HSDrawData& s1, long s2)
 {
 	return (s1.m_dData || s2);
 }
 inline bool AFXAPI operator||(const HSDrawData& s1,unsigned long s2)
 {
 	return (s1.m_dData || s2);
 }
 //操作符重载 || 返回true or false
 inline bool AFXAPI operator||(long s1, const HSDrawData& s2)
 {
 	return (s1 || s2.m_dData);
 }
 //操作符重载 || 返回true or false
 inline bool AFXAPI operator||(const HSDrawData& s1, int s2)
 {
 	return (s1.m_dData || s2);
 }
 //操作符重载 || 返回true or false
 inline bool AFXAPI operator||(int s1, const HSDrawData& s2)
 {
 	return (s1 || s2.m_dData);
 }
 //操作符重载 || 返回true or false
 inline bool AFXAPI operator||(const HSDrawData& s1, BYTE s2)
 {
 	return (s1.m_dData || s2);
 }
 //操作符重载 || 返回true or false
 inline bool AFXAPI operator||(BYTE s1, const HSDrawData& s2)
 {
 	return (s1 || s2.m_dData);
 }
 
 //操作符重载 && 返回true or false
 inline bool AFXAPI operator&&(const HSDrawData& s1, const HSDrawData& s2)
 {
 	return (s1.m_dData && s2.m_dData);
 }
 //操作符重载 && 返回true or false
 inline bool AFXAPI operator&&(const HSDrawData& s1, double s2)
 {
 	return (s1.m_dData && s2);
 }
 //操作符重载 && 返回true or false
 inline bool AFXAPI operator&&(double s1, const HSDrawData& s2)
 {
 	return (s1 && s2.m_dData);
 }
 //操作符重载 && 返回true or false
 inline bool AFXAPI operator&&(const HSDrawData& s1, long s2)
 {
 	return (s1.m_dData && s2);
 }
 inline bool AFXAPI operator&&(const HSDrawData& s1,unsigned long s2)
 {
 	return (s1.m_dData && s2);
 }
 //操作符重载 && 返回true or false
 inline bool AFXAPI operator&&(long s1, const HSDrawData& s2)
 {
 	return (s1 && s2.m_dData);
 }
 //操作符重载 && 返回true or false
 inline bool AFXAPI operator&&(const HSDrawData& s1, int s2)
 {
 	return (s1.m_dData && s2);
 }
 //操作符重载 && 返回true or false
 inline bool AFXAPI operator&&(int s1, const HSDrawData& s2)
 {
 	return (s1 && s2.m_dData);
 }
 //操作符重载 && 返回true or false
 inline bool AFXAPI operator&&(const HSDrawData& s1, BYTE s2)
 {
 	return (s1.m_dData && s2);
 }
 //操作符重载 && 返回true or false
 inline bool AFXAPI operator&&(BYTE s1, const HSDrawData& s2)
 {
 	return (s1 && s2.m_dData);
 }
 
// 计算时传入外部参数
 struct CCalcExternParam
 {
 	long		m_nBeginDate; // 开始日期,如19901020
 	long	    m_nEndDate;   
 	CodeInfo    m_sCode;	  // 代码
 	char 		m_szBlock[BLOCK_NAME_LENGTH]; // 板块名称
 
 	CCalcExternParam()
 	{
 		EmptyBlock();
 	}
 
 	void    EmptyBlock() { memset(m_szBlock,0,sizeof(m_szBlock)); }
 	CString GetBlockName() { return CString(m_szBlock).Left(BLOCK_NAME_LENGTH); }
 	void    SetBlockName(CString pData)
 	{
 		EmptyBlock();
 		if( pData.IsEmpty() )
 			return;
 
 		strncpy(m_szBlock,pData,sizeof(m_szBlock));
 	}
 };

/// *************************************************************
// *	类 名 称：	CLineProperty
// *	类 描 述：	绘图数据(在盘后分析里面用到)
// ***************************************************************/
 struct CLineProperty
 {
 public:
 	CCalcExternParam* m_pCalcParam; // 外部指针，本类不作删除
 public:
 	HSDouble *		m_pData;    		// 数据
 	int				m_nSize;			// 数据长度
 
	char		    m_strName[20];  		// 名称 
 	DWORD		    m_dwStyle;  		// 绘制风格
 	ColorProperty   m_lColor;   		// 当为技术线时的颜色
 
 	HSDouble		m_lMinValue;		// 最小
 	HSDouble		m_lMaxValue;		// 最大
 
  	void*		    m_pCurVal;			 // 当前的公式
  	void*	        m_pDataSource;		 // 数据包装 
 	int				m_nColID;			 // 列id，在公式表格里使用 
 	char			m_strFormat[64];		 // 表格数据使用，输出格式串

	void			init();
 	void			FreeStr();
 	void			AllocStr(CString* pStr);
 	CString			GetFormat(HSDouble& dValue);
 
 public:
 	// 初始化
 	CLineProperty();
 	// 清空所占用的资源
 	~CLineProperty();
 
 	/******************************************************************
 	*	函数功能：	返回m_pData[nDataPos]的BOOL值
 	*	函数参数：	int nDataPos : [in] 要求返回BOOL值的数据位置
 	*	返 回 值：	BOOL : 对应点的数据是否为真
 	*	调用依赖：	
 	*	作    者：	
 	*	完成日期：
 	*	修改说明：	
 	*****************************************************************/
 	BOOL IsTrue(int nDataPos) ;
 	BOOL IsEmpty();
 
 	/******************************************************************
 	*	函数功能：	分配nCurSize*sizeof(HSDouble)个空间给m_pData
 	*	函数参数：	int nCurSize: [in] 新的长度
 	*				int nOldSize: [in] 旧的长度
 	*	返 回 值：	void 
 	*	调用依赖：	
 	*	作    者：	
 	*	完成日期：
 	*	修改说明：	
 	*****************************************************************/
 	void Alloc(int nCurSize,int nOldSize);
 
 	void Free();
 	// 置默认的最大最小值
 	void DefMinMax();
 
 public:
 	// 风格判断
 	BOOL	IsStyle(long dwStyle);
 	// 加风格
 	void	AddStyle(long dwStyle);
 	// 删除风格
 	void	RemoveStyle(long dwStyle);
 	// 设置风格
 	void    SetStyle(long dwStyle);
 	// 计算最大最小百分比
 	// 注意：使用此函数前必须先判断nBegin及nEnd的合法性
 	BOOL CalcUpDownPercent(int nBegin, int nEnd, double& dUp, double& dDown, double dBaseIntput = (double)hx_DefMinValues);
 };


 /*************************************************************
 *	类 名 称：	CTValues
 *	类 描 述：	数组变量的类的封装的模板
 ***************************************************************/
 template <class TValues>
 struct CTValues
 {
	 TValues* m_pValue;	// 指向外部空间,这里不做清除
	 long	 m_lStart;	// 开始节点
	 long	 m_lSize;	// 总长度

	 CTValues()									     { m_pValue = NULL;m_lStart = 0;m_lSize = 0; }
	 CTValues(TValues* pValue,long lSize)		     { m_pValue = pValue;m_lStart = 0;m_lSize = lSize; }
	 CTValues(TValues* pValue,long lStart,long lSize) { m_pValue = pValue;m_lStart = lStart;m_lSize = lSize; }
	 void operator = (const CTValues& values)	     { m_pValue = values.m_pValue;m_lStart = values.m_lStart;m_lSize = values.m_lSize; }
	 void Empty()									 { m_pValue = NULL;m_lStart = 0;m_lSize = 0; }
	 BOOL IsEmpty()									 { return (m_pValue != NULL && m_lSize > 0); }	
 };


 // 日线数据返回
 struct RetDiskData
 {
	 void* m_pData;
	 long  m_lSize;

	 RetDiskData()
	 {
		 memset(this,0,sizeof(RetDiskData));
	 }

	 ~RetDiskData()
	 {
		 Free();
	 }

	 BOOL IsValid() { return ( m_pData != NULL && m_lSize > 0 ); }
	 BOOL IsEmpty() { return ((m_pData == NULL) || (m_lSize <= 0)); }

	 void Alloc( int nCurSize  )
	 {
		 if( nCurSize <= 0 )
		 {
			 Free();
			 return;
		 }		
		 if( m_lSize != nCurSize )
		 {
			 Free();
			 m_pData = new char[sizeof(StockDay)*nCurSize];
			 m_lSize = nCurSize;
		 }
		 memset( m_pData,0,sizeof(StockDay)*m_lSize);
	 }
	 void Free()
	 {
		 if ( m_pData != NULL )
		 {
			 delete[] m_pData;
			 m_pData = NULL;
		 }
		 m_lSize = 0;
	 }
 };

 struct HSRetFileData
 {
	 char* m_pData;
	 long  m_lSize;

	 HSRetFileData()
	 {
		 memset(this,0,sizeof(HSRetFileData));
	 }
	 ~HSRetFileData()
	 {
		 Free();
	 }

	 inline void Alloc( int nCurSize  )
	 {		
		 if( nCurSize <= 0 )
		 {
			 Free();
			 return;
		 }		
		 if( m_lSize != nCurSize )
		 {
			 Free();

			 if( HSDrawData::m_pHSExternAlloc )
			 {
				 m_pData = (char*)(*HSDrawData::m_pHSExternAlloc)
					 ( (long)nCurSize,charType );
				 if( m_pData )
				 {
					 m_lSize = nCurSize;
				 }
			 }
			 else
			 {
				 m_pData = new char[nCurSize];
				 memset( m_pData,0,m_lSize);
				 m_lSize = nCurSize;
			 }
		 }
		 else
		 {
			 memset( m_pData,0,m_lSize);
		 }
	 }

	 inline void Free()
	 {
		 if( HSDrawData::m_pHSExternFree )
		 {
			 (*HSDrawData::m_pHSExternFree)(m_pData,charType);
			 m_pData = NULL;
			 m_lSize = 0;
		 }
		 else if ( m_pData != NULL )
		 {
			 delete[] m_pData;
			 m_pData = NULL;
			 m_lSize = 0;
		 }
	 }
	 inline BOOL IsValid() { return (m_pData != NULL && m_lSize > 0); }

	 inline void Append(HSRetFileData* pRetData) //追加数据,Xiongzb,2002-03-15
	 {
		 if(!pRetData )
			 return;
		 Append(pRetData->m_pData, pRetData->m_lSize);
	 }

	 void Append(char* pData, long lDataSize) //追加数据,Xiongzb,2004-03-30
	 {
		 if(!pData || lDataSize <= 0)
			 return;

		 if(IsValid())
		 {
			 long lSize = m_lSize;

			 char* pTotalData = new char[lSize];
			 memcpy(pTotalData, m_pData, lSize);

			 Alloc(lSize + lDataSize);

			 memcpy(m_pData, pTotalData, lSize);
			 memcpy(m_pData + lSize, pData, lDataSize);

			 delete[] pTotalData;
			 return;
		 }

		 Alloc(lDataSize);
		 memcpy(m_pData, pData, m_lSize);
	 }
 };

 struct   RealTimeDataSource
 {
	 StockUserInfo	  m_stStockInfo;
	 StockOtherData    m_sOthData;
	 union
	 {
		 HSIndexRealTime  m_stIndexData;
		 HSStockRealTime  m_stStockData;
		 HSQHRealTime	 m_qhNowData;
	 };
 };
//公式属性 
 struct ExpPropery
 {
	 CString		    m_strName;		// 名称
	 CString		    m_strDescribe;  // 名称描述
	 char				m_dExpType;     // 公式类型
	 DWORD				m_bStyle;       //公式分割 （主副图叠加）
 };
 //除权后价格 = 除权前价格 * m_fMulit + m_fAdd
 struct ChuQuanData
 {
	 enum { NotCalc,Calc/* 计算*/ };

	 char	m_cMask;		//选项

	 unsigned long	m_lDate;	//除权日  
	 float	m_fMulit;			//相乘的数据
	 float	m_fAdd;				//相加的数据
	 float	m_fSGBL;			//送股比率
	 float	m_fPGBL;			//配股比率
	 float	m_fPGJ;				//配股价
	 float	m_fXJHL;			//现金红利
 };


 // 最大最小值
 struct CDispMaxMinValue
 {
	 long	  m_nPos; // 位置
	 HSDouble  m_lValue; // 最大或最小的值
	 CDispMaxMinValue();
	 void Empty();
	 BOOL IsFind(long nPos);
 };

 /*************************************************************
 *	类 名 称：	CNameProperty
 *	类 描 述：	名字属性
 ***************************************************************/
 struct CNameProperty
 {
 public:
	 CString		    m_strName;		// 名称
	 CString		    m_strDescribe;  // 名称描述
	 CString		    m_strExp;		// 参数
	 WORD				m_dStyle;		// 绘制风格
	 char				m_dExpType;     // 公式类型
	 ColorProperty		m_lColor;		// 颜色

	 CNameProperty();

	 // 名字是否为空
	 BOOL IsEmpty();
	 // 名字比较
	 int  CompareNoCase( LPCTSTR lpsz );

	 // 风格操作
 public:
	 BOOL	IsStyle(WORD dStyle);
	 void	AddStyle(WORD dStyle);
	 void	RemoveStyle(WORD dStyle);
	 void   SetStyle(WORD dStyle);
 };


 class CHSFinanceData : public CurrentFinanceData
 {
 public:
	 CHSFinanceData();
	 CHSFinanceData(CurrentFinanceData* pFinanceData);

	 //根据财务数据的ID号返回相对应的财务数据
	 HSDouble GetDataByID(UINT nID);
	 //根据财务数据的相对位置返回相对应的财务数据
	 HSDouble GetDataByPos(int nPos);

	 HSDouble CAPITAL(CodeInfo* pCode);

	 //清空本结构
	 void Empty()   {	memset(this,0,sizeof(HisFinanceData));	 }

	 //检测数据是否为空
	 BOOL IsEmpty() { return (m_nDate == 0) ; }
 };

//	股票的实时数据
class CRealTimeDataSource : public RealTimeDataSource
{
public:
	//	构造函数
	CRealTimeDataSource();
	

	/******************************************************************
	*	函数功能：	根据数据的位置返回该实时数据
	*	函数参数：	int			nPos		：指定的数据位置
	*				double		nPriceUnit	：价格精度单位
	*				char		cType		：证券类型
	*	返 回 值：	HSDouble：待返回的实时数据
	*	调用依赖：	
	*	作    者：	
	*	完成日期：	
	*	修改说明：	
	*****************************************************************/
	HSDouble GetDataByPos(int nPos,double nPriceUnit,char cType = 0);
	
	//	清空数据
	void Empty()	
	{	
		memset(this,0,sizeof(RealTimeDataSource));

		m_dHuanShoulv = 0; // 换手率
		m_dShiYinglv  = 0;  // 市盈率
		m_nTolalTime  = 0;  // 总时间
	}

	//	检测是否为空数据
	BOOL IsEmpty();

	// 计算其他参数
public:
	double m_dHuanShoulv; // 换手率
	double m_dShiYinglv;  // 市盈率
	int    m_nTolalTime;  // 总时间
	double m_dQuarter;	  // 季度 //20090605 YJT 修改市盈率算法

};
#pragma pack() 
 #endif // _DRAWDATA_H_

