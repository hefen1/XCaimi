/*************************************************************
*	Copyright (c)2002, 杭州恒生信息技术有限公司
*	All rights reserved.
*
*	文件名称：	HxCompileOut.h
*	文件标识：	
*	摘	  要：	本模块的输出接口
*
*	当前版本：	2.0
*	作	  者：	俞礼石
*	更新日期：	2003年4月18日
*
***************************************************************/

#if !defined(_HXCOMPILEOUT_H_)
#define _HXCOMPILEOUT_H_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "..\comm\systemDefine.h"

#ifndef __AFXTEMPL_H__
#include <afxtempl.h>
#endif

#ifdef _AFXDLL
	#ifdef _DEBUG
		#ifdef _UNICODE
			#pragma comment(lib,"BCGCB462UD.lib") 
			#pragma message("Automatically linking with BCGCB462UD.dll")
		#else
			#pragma comment(lib,"..\\lib\\debug\\YlsCompile.lib") 
			#pragma message("Automatically linking with ..\\lib\\debug\\YlsCompile.lib")
		#endif
	#else
		#ifdef _UNICODE
			#pragma comment(lib,"BCGCB462U.lib") 
			#pragma message("Automatically linking with BCGCB462U.dll") 
		#else
			#pragma comment(lib,"..\\lib\\release\\YlsCompile.lib") 
			#pragma message("Automatically linking with ..\\lib\\release\\YlsCompile.lib") 
		#endif
	#endif
#else

	#ifdef _DEBUG
		#ifdef _UNICODE
			#pragma comment(lib,"BCGCB462Staticud.lib") 
			#pragma message("Automatically linking with BCGCB462Staticud.lib") 
		#else
			#ifdef  VC_60
				#pragma comment(lib,"..\\lib60\\debug\\YlsCompile.lib") 
				#pragma message("Automatically linking with ..\\lib60\\debug\\YlsCompile.lib") 
			#else
				#pragma comment(lib,"..\\lib\\debug\\YlsCompile.lib") 
				#pragma message("Automatically linking with ..\\lib\\debug\\YlsCompile.lib") 
			#endif
		#endif
	#else
		#ifdef _UNICODE
			#pragma comment(lib,"BCGCB462StaticU.lib") 
			#pragma message("Automatically linking with BCGCB462StaticU.lib") 
		#else
			#ifdef  VC_60
				#pragma comment(lib,"..\\lib60\\Release\\YlsCompile.lib") 
				#pragma message("Automatically linking with ..\\lib60\\Release\\YlsCompile.lib")
			#else
				#pragma comment(lib,"..\\lib\\Release\\YlsCompile.lib") 
				#pragma message("Automatically linking with ..\\lib\\Release\\YlsCompile.lib")
			#endif
		#endif
	#endif

#endif

#include "..\HSInfoLib\InfoGetOut.h"       // 信息数据

#include "..\comm\pageid.h"
#include "..\comm\fxjFormulaStruct.h"

#include "..\HSFormulaBaseLib\MfcBaseOut.h"

#include "pubStruct.h"
#include "FormulaCalc.h"

/*
#include "ExpPublic.h"
#include "Express.h"
#include "ExpValue.h"
#include "FunExpresion.h"
#include "Value.h"
#include "VarValue.h"
#include "HSDataSource.h"
#include "SystemFun.h"
#include "UnionSelCondition.h"
#include "FormulaCalc.h"
*/

#endif // _HXCOMPILEOUT_H_
