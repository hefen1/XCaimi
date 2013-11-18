#pragma once
/*******************************************************************************
* Copyright (c)2010, ���ݺ������ӹɷ����޹�˾
* All rights reserved.
*
* �ļ����ƣ�HsFileManager.h
* ժ    Ҫ���ļ�����ʵ����
*
* ��ǰ�汾��
* ��    �ߣ�����
* ������ڣ�2010-12-10
* ��	 ע��
*******************************************************************************/
#include "ConfigBase.h"

class CHsFileManager
{
public:
	CHsFileManager();
	~CHsFileManager();

	CConfigBase* GetColorCfg();  // ��ȡ��ɫ�����ļ�������ָ��
	CConfigBase* GetFontCfg();   // ��ȡ���������ļ�������ָ��	
	CConfigBase* GetSystemCfg(); // ��ȡϵͳ�����ļ�������ָ��
	CConfigBase* GetDispColumnCfg();   // ��ȡDispColumn.xml�ļ�ָ��
	CConfigBase* GetReportColumnCfg(); // ��ȡReportColumn.xml�ļ�ָ��
	CConfigBase* GetReportTabCfg();    // ��ȡReportTab.xml�ļ�ָ��
	CConfigBase* GetServerCfg();       // ��ȡͨѶ�����ļ�ָ��
	CConfigBase* GetUserDataCfg();     // ��ȡ�������û������ļ�ָ��
	CConfigBase* GetKeyboardCfg();     // ��ȡ�����м��̾����ȼ��ļ�ָ��
	CConfigBase* GetKLineTab();         //��ȡK��tab������
	CConfigBase* GetKLinePostionCtrl();	//��ȡK��λ�ÿ���
private:
	CConfigBase* m_pColorCfg;  // ���������ļ�
	CConfigBase* m_pFontCfg;   // ��ɫ�����ļ�
	CConfigBase* m_pSystemCfg; // ϵͳ�����ļ�
	CConfigBase* m_pDispColumnCfg;    // DispColumn.xml�ļ�
	CConfigBase* m_pReportColumnCfg;  // ReportColumn.xml�ļ�
	CConfigBase* m_pReportTabCfg;     // ReportTab.xml�ļ�
	CConfigBase* m_pServerCfg;        // ͨѶ�����ļ�
	CConfigBase* m_pUserDataCfg;      // �������û������ļ�
	CConfigBase* m_pKeyboardCfg;      // ���̾��������ļ�
	CConfigBase* m_pKLineTab;         // ��ȡK��tab������
	CConfigBase* m_pKLinePostionCtrol;// ��ȡK��tab������

};
