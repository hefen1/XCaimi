/*************************************************************
*	Copyright (c)2002, ���ݺ�����Ϣ�������޹�˾
*	All rights reserved.
*
*	�ļ����ƣ�	HxCompileOut.h
*	�ļ���ʶ��	
*	ժ	  Ҫ��	��ģ�������ӿ�
*
*	��ǰ�汾��	2.0
*	��	  �ߣ�	����ʯ
*	�������ڣ�	2003��4��18��
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

#include "..\HSInfoLib\InfoGetOut.h"       // ��Ϣ����

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
