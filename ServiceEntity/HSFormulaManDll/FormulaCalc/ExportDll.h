/*******************************************************************************
 * Copyright (c)2003, 杭州恒生信息技术有限公司
 * All rights reserved.
 *
 * 文件名称：ExportDll.h
 * 文件标识：宏定义
 * 摘    要：连接输出定义以及Ctrl shift alt键按下状态函数宏定义
 *
 * 当前版本：1.0
 * 作    者：
 * 完成日期：
 *
 * 取代版本：
 * 原 作 者：
 * 完成日期：
 * 备	 注：
 *******************************************************************************/

#ifndef _HSEXPORTBASE_H_
#define _HSEXPORTBASE_H_
 
#ifdef _AFXDLL
	#ifdef _BCGCONTROLBAR_
	   #define BCGCONTROLBARDLLEXPORT  _declspec(dllexport)
	#else
	   #define BCGCONTROLBARDLLEXPORT  //_declspec(dllimport) // AFX_EXT_CLASS
	#endif
#else
	#define BCGCONTROLBARDLLEXPORT	   //__declspec(novtable)
#endif
//判断键是否按下
#define IsAtlpressed()   ( (GetKeyState(VK_MENU) & (1 << (sizeof(SHORT)*8-1))) != 0 )
#define IsSHIFTpressed() ( (GetKeyState(VK_SHIFT) & (1 << (sizeof(SHORT)*8-1))) != 0   )
#define IsCTRLpressed()  ( (GetKeyState(VK_CONTROL) & (1 << (sizeof(SHORT)*8-1))) != 0 )

#endif // _HSEXPORTBASE_H_