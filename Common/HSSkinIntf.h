/*******************************************************
  源程序名称:HSSkinIntf.h
  软件著作权:恒生电子股份有限公司
  系统名称:  投资赢家金融理财终端V1.0
  模块名称:  皮肤
  功能说明:  
  作    者:  shenglq
  开发日期:  20110613
*********************************************************/
#pragma once

#define OBJ_HSSKIN  _T("HSSkin.HSSkinObj")

enum ESkinFormType {sfMainform, sfOnlyThisForm, sfDialog, sfDLL};

struct IHSSkinObj
{
	//释放
	virtual void Release() = 0;
	virtual void FreeCache() = 0;
	virtual BOOL LoadSkin(CString sFileName) = 0;
	virtual BOOL SetActive(BOOL bFlag) = 0;
	virtual BOOL SetSkinFormType(int nType = 0) = 0;
	virtual void SkinForm(HWND nHandle) = 0;
};