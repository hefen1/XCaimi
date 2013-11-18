/*******************************************************************************
* Copyright (c)2010, 杭州恒生电子股份有限公司
* All rights reserved.
*
* 文件名称：HsUserDataCfg.h
* 摘    要：运行中读写用户数据配置实现类
*
* 当前版本：
* 作    者：王超
* 完成日期：2010-12-29
* 备	 注：因为效率需要，在初始化的时候就直接将数据导入
*******************************************************************************/
#pragma once
#include "ConfigInterface.h"
#include "ConfigStruct.h"
#include "ConfigBase.h"

class CHsUserDataCfg : IHsUserDataCfg
{
public:
	CHsUserDataCfg(void);
	~CHsUserDataCfg(void);
	void Init();
	BOOL IsInit() {return m_bInited;}

	BOOL Release();
	BOOL SaveToFile();

private:
	void LoadFileData(); // 从文件读取导入到map中 
// 实现接口
public:
	virtual const HsCodeMark* SearchMark(IN CString& strKeyWord); //根据关键字查找标记内容
	virtual BOOL DelMark(IN CString& strKeyWord);		 // 删除指定标记
	virtual BOOL SetMark(IN const HsCodeMark& CodeMark); // 设定标记
private:
	BOOL         m_bInited;
	BOOL         m_bSave;
	CConfigBase* m_pConfigBase;

//	CCodeMarkArray	m_ayCodeMark;
	CCodeMarkMap	m_mapCodeMark;
};
