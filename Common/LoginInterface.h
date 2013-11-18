/*******************************************************************************
* Copyright (c)2010, ���ݺ������ӹɷ����޹�˾
* All rights reserved.
*
* �ļ����ƣ�LoginInterface.h
* ժ    Ҫ����¼�ӿ������ļ�
*
* ��ǰ�汾��
* ��    �ߣ�����
* ������ڣ�2010-12-10
* ��	 ע��
*******************************************************************************/
#pragma once

#define OBJ_LOGIN	      _T("Login.LoginObj")		

enum LoginType{LoginType_Normal = 1, LoginType_OnlyTrade};
enum LoginRet {LoginRet_Error = 1, LoginRet_Quote, LoginRet_Trade, LoginRet_QuoteAndTrade};
interface IDoLogin
{
	virtual LoginRet Login(LoginType eType) = 0;  // ��¼����
	virtual void TestSpeed() = 0;                  // ����
	virtual void ConnManager() = 0;                // ͨѶ����
};
