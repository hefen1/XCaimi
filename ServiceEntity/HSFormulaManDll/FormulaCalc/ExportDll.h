/*******************************************************************************
 * Copyright (c)2003, ���ݺ�����Ϣ�������޹�˾
 * All rights reserved.
 *
 * �ļ����ƣ�ExportDll.h
 * �ļ���ʶ���궨��
 * ժ    Ҫ��������������Լ�Ctrl shift alt������״̬�����궨��
 *
 * ��ǰ�汾��1.0
 * ��    �ߣ�
 * ������ڣ�
 *
 * ȡ���汾��
 * ԭ �� �ߣ�
 * ������ڣ�
 * ��	 ע��
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
//�жϼ��Ƿ���
#define IsAtlpressed()   ( (GetKeyState(VK_MENU) & (1 << (sizeof(SHORT)*8-1))) != 0 )
#define IsSHIFTpressed() ( (GetKeyState(VK_SHIFT) & (1 << (sizeof(SHORT)*8-1))) != 0   )
#define IsCTRLpressed()  ( (GetKeyState(VK_CONTROL) & (1 << (sizeof(SHORT)*8-1))) != 0 )

#endif // _HSEXPORTBASE_H_