/*************************************************************
*	Copyright (c)2010, �������ӹɷ����޹�˾
*	All rights reserved.
*
*	�ļ����ƣ�	keyboardinterface.h
*	�ļ���ʶ��	���̾���ӿ�
*	ժ	  Ҫ��	
*
*	��ǰ�汾��	1.0
*	��	  �ߣ�	
*	�������ڣ�	
*
*	�޸�˵����	
*
***************************************************************/
#include "KeyBoardStruct.h"
#define OBJ_KEYBOARD _T("KeyBoard.KeyBoard")
interface IKeyboard
{
	/******************************************************************
	*	�������ܣ�	��ʼ�����̾���,��CKeyboardDataList::InitKeyboard����˵��
	*	����������
	*				
	*	�� �� ֵ��	void				  
	*	����������	CKeyboardDataList::InitKeyboard
	*	��    �ߣ�	
	*	������ڣ�	2003��5��6��
	*	�޸�˵����	
	*****************************************************************/
	virtual void  InitKeyboard()=0;
	/******************************************************************
	*	�������ܣ�	��ʾ�������ؼ��̾��鴰��
	*	����������	char nChar [in]  �����ַ�
	*				HWND hParent[in] �����ھ�� 
	*				HSShowKeyboardInfo* pKeyInfo  [in]  ���̾��鴰��λ��
	*				
	*	�� �� ֵ��	void				  
	*	����������	
	*	��    �ߣ�	
	*	������ڣ�	
	*	�޸�˵����	
	*****************************************************************/
	virtual void ShowKeyboard(char nChar,HSShowKeyboardInfo* pKeyInfo=NULL, HWND hParent=NULL)=0;
};