/*******************************************************
  Դ��������:HSSkinIntf.h
  �������Ȩ:�������ӹɷ����޹�˾
  ϵͳ����:  Ͷ��Ӯ�ҽ�������ն�V1.0
  ģ������:  Ƥ��
  ����˵��:  
  ��    ��:  shenglq
  ��������:  20110613
*********************************************************/
#pragma once

#define OBJ_HSSKIN  _T("HSSkin.HSSkinObj")

enum ESkinFormType {sfMainform, sfOnlyThisForm, sfDialog, sfDLL};

struct IHSSkinObj
{
	//�ͷ�
	virtual void Release() = 0;
	virtual void FreeCache() = 0;
	virtual BOOL LoadSkin(CString sFileName) = 0;
	virtual BOOL SetActive(BOOL bFlag) = 0;
	virtual BOOL SetSkinFormType(int nType = 0) = 0;
	virtual void SkinForm(HWND nHandle) = 0;
};