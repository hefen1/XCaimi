/*************************************************************
*	Copyright (c)2010, 恒生电子股份有限公司
*	All rights reserved.
*
*	文件名称：	keyboardinterface.h
*	文件标识：	键盘精灵接口
*	摘	  要：	
*
*	当前版本：	1.0
*	作	  者：	
*	更新日期：	
*
*	修改说明：	
*
***************************************************************/
#include "KeyBoardStruct.h"
#define OBJ_KEYBOARD _T("KeyBoard.KeyBoard")
interface IKeyboard
{
	/******************************************************************
	*	函数功能：	初始化键盘精灵,看CKeyboardDataList::InitKeyboard函数说明
	*	函数参数：
	*				
	*	返 回 值：	void				  
	*	调用依赖：	CKeyboardDataList::InitKeyboard
	*	作    者：	
	*	完成日期：	2003年5月6日
	*	修改说明：	
	*****************************************************************/
	virtual void  InitKeyboard()=0;
	/******************************************************************
	*	函数功能：	显示或是隐藏键盘精灵窗口
	*	函数参数：	char nChar [in]  输入字符
	*				HWND hParent[in] 父窗口句柄 
	*				HSShowKeyboardInfo* pKeyInfo  [in]  键盘精灵窗口位置
	*				
	*	返 回 值：	void				  
	*	调用依赖：	
	*	作    者：	
	*	完成日期：	
	*	修改说明：	
	*****************************************************************/
	virtual void ShowKeyboard(char nChar,HSShowKeyboardInfo* pKeyInfo=NULL, HWND hParent=NULL)=0;
};