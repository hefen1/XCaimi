/*************************************************************
*	Copyright (c)2010, �������ӹɷ����޹�˾
*	All rights reserved.
*
*	�ļ����ƣ�	KeyBoardImp.h
*	�ļ���ʶ��	���̾���ӿ�ʵ����
*	ժ	  Ҫ��	
*
*	��ǰ�汾��	1.0
*	��	  �ߣ�	
*	�������ڣ�	
*
*	�޸�˵����	
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
	*	�������ܣ�	�������̾���,��CKeyboardDataList::InitKeyboard����˵��
	*	����������	HWND hParent
	*				
	*	�� �� ֵ��	void				  
	*	����������	
	*	��    �ߣ�	
	*	������ڣ�	
	*	�޸�˵����	
	*****************************************************************/
	void   CreateKeyboard(HWND hParent=NULL);

	/******************************************************************
	*	�������ܣ�	��ʼ�����̾���,��CKeyboardDataList::InitKeyboard����˵��
	*	����������	
	*				
	*	�� �� ֵ��	void				  
	*	����������	
	*	��    �ߣ�	
	*	������ڣ�	
	*	�޸�˵����	
	*****************************************************************/
	void  InitKeyboard();
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
	void ShowKeyboard(char nChar, HSShowKeyboardInfo* pKeyInfo=NULL, HWND hParent=NULL);

private:
	CWndKeyboard* m_pWndKeyboard; 
};
