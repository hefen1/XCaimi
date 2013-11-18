/*************************************************************
*	Copyright (c)2010, 恒生电子股份有限公司
*	All rights reserved.
*
*	文件名称：	MfcBaseOut.h
*	文件标识：	公式编译器模块输出接口
*	摘	  要：	包含了本模块所有的头文件
*
*	当前版本：	1.0
*	作	  者：	
*	更新日期：
*
***************************************************************/

#if !defined(_MFCBASE_H_)
#define _MFCBASE_H_

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
		#else
			#ifdef  VC_60
				#pragma comment(lib,"..\\lib60\\debug\\MFCBase.lib") 
				#pragma message("Automatically linking with ..\\lib60\\debug\\MFCBase.lib") 
			#else
				#pragma comment(lib,"..\\lib\\debug\\MFCBase.lib") 
				#pragma message("Automatically linking with ..\\lib\\debug\\MFCBase.lib")  
			#endif
		#endif
	#else
		#ifdef _UNICODE
		#else
			#ifdef  VC_60
				#pragma comment(lib,"..\\lib60\\Release\\MFCBase.lib") 
				#pragma message("Automatically linking with ..\\lib60\\Release\\MFCBase.lib")
			#else
				#pragma comment(lib,"..\\lib\\Release\\MFCBase.lib") 
				#pragma message("Automatically linking with ..\\lib\\Release\\MFCBase.lib")  
			#endif
		#endif
	#endif
#else
	#ifdef _DEBUG
		#ifdef _UNICODE
		#else
			#ifdef  VC_60
				#pragma comment(lib,"..\\lib60\\debug\\MFCBase.lib") 
				#pragma message("Automatically linking with ..\\lib60\\debug\\MFCBase.lib") 
			#else
				#pragma comment(lib,"..\\lib\\debug\\MFCBase.lib") 
				#pragma message("Automatically linking with ..\\lib\\debug\\MFCBase.lib")  
			#endif
		#endif
	#else
		#ifdef _UNICODE
		#else
			#ifdef  VC_60
				#pragma comment(lib,"..\\lib60\\Release\\MFCBase.lib") 
				#pragma message("Automatically linking with ..\\lib60\\Release\\MFCBase.lib")
			#else
				#pragma comment(lib,"..\\lib\\Release\\MFCBase.lib") 
				#pragma message("Automatically linking with ..\\lib\\Release\\MFCBase.lib")  
			#endif
		#endif
	#endif

#endif

#include "..\HSInfoLib\InfoGetOut.h"       // 信息数据

#include "..\comm\pageid.h"
#include "..\comm\fxjFormulaStruct.h"

#include "CompileInterFace.h"
#include "ExpPublic.h"
#include "Express.h"
#include "ExpValue.h"
#include "FunExpresion.h"
#include "Value.h"
#include "VarValue.h"
#include "HSDataSource.h"
#include "SystemFun.h"
//#include "UnionSelCondition.h"
//#include "FormulaCalc.h"

#endif // _MFCBASE_H_
