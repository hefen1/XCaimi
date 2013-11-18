/*************************************************************
*	Copyright (c)2010, 恒生电子股份有限公司
*	All rights reserved.
*
*	文件名称：	KeyBoardImp.h
*	文件标识：	键盘精灵接口实现类
*	摘	  要：	
*
*	当前版本：	1.0
*	作	  者：	
*	更新日期：	
*
*	修改说明：	
*
***************************************************************/

#include "KeyboardInterface.h"
#include "WndKeyboard.h"

class CKeyBoard : public IKeyboard
{
public:
	CKeyBoard();
	~CKeyBoard();

	/******************************************************************
	*	函数功能：	创建键盘精灵,看CKeyboardDataList::InitKeyboard函数说明
	*	函数参数：	HWND hParent
	*				
	*	返 回 值：	void				  
	*	调用依赖：	
	*	作    者：	
	*	完成日期：	
	*	修改说明：	
	*****************************************************************/
	void   CreateKeyboard(HWND hParent=NULL);

	/******************************************************************
	*	函数功能：	初始化键盘精灵,看CKeyboardDataList::InitKeyboard函数说明
	*	函数参数：	
	*				
	*	返 回 值：	void				  
	*	调用依赖：	
	*	作    者：	
	*	完成日期：	
	*	修改说明：	
	*****************************************************************/
	void  InitKeyboard();
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
	void ShowKeyboard(char nChar, HSShowKeyboardInfo* pKeyInfo=NULL, HWND hParent=NULL);

private:
	CWndKeyboard* m_pWndKeyboard; 
};
