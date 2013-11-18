/*************************************************************
*	Copyright(c) 2010, ���ݺ����ɷ����޹�˾
*	All rights reserved.
*
*	�ļ����ƣ�	CDiagramComm.h
*	�ļ���ʶ��
*	ժ	  Ҫ��  ͼԪ��ͬʹ�õ�һЩ����
*	
*	��ǰ�汾��
*	��	  �ߣ�	����
*	������ڣ�  2011-3-21
*
*	��	  ע��
*	�޸ļ�¼��
*	��	  �ߣ�
*	�޸����ڣ�
*	�޸����ݣ�
*
****************************************************************/
#pragma once
#include "..\..\..\Common\QuoteComm\QuoteDefine.h"

#define INFO_PATH_KEY_F10	    "%sF10��Ѷ·��"   // F10��Ѷ
#define INFO_PATH_KEY_TREND		"%s��ʱ��Ѷ·��"  // ��ʱ��Ѷ
#define INFO_PATH_KEY_TECH		"%s�̺���Ѷ·��"  // �̺���Ѷ
#define INFO_PATH_KEY_REPORT	"%s������Ѷ·��"  // ������Ѷ
#define INFO_PATH_KEY_SPEC		"%s��ɫ��Ѷ·��"  // ��ɫ��Ѷ

#define HTMLFORMAT_PATH         "InfoMine.template" // ���html��ʽ���ļ����ļ���
#define HTMLCONTENT_PATH        "InfoMine.show"   // ���ڴ����Ҫ��ʾ��html�ļ�

//////////////////////////////////////////////////////////////////////////
// html��ʽ�滻�ؼ���
#define HTML_REPLACE_TITLE      "ARTICLE_TITLE"      
#define HTML_REPLACE_CONTENT    "ARTICLE_CONTENT"
#define HTML_REPLACE_AUTHOR     "ARTICLE_AUTHOR"
#define HTML_REPLACE_TIME       "ARTICLE_TIME"
#define HTML_REPLACE_ORIGIN		"ARTICLE_ORIGIN"
//////////////////////////////////////////////////////////////////////////

#define GET_SRVTYPE_BYNAME(name)	((!name.CompareNoCase(SECRET_TREE_INI_FILE) \
	|| !name.CompareNoCase(BASE_INFO_TREE_INI_FILE)) ? CEV_Connect_ZX : CEV_Connect_HQ)