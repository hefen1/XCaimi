#pragma once

#include "..\Common\HSWinner.h"
#include ".\LinkageMsgManager.h"

struct HotKeyInfo
{
	UINT        m_nId;
	UINT        m_fsModifiers;
	UINT        m_vk;
	IBizWindow* m_pBizWnd;
	HWND        m_hFrame;
};


typedef CArray<HotKeyInfo*,HotKeyInfo*>  CHotKeyInfoArray;

class CHotKeyManager: public CObject
{
public:
	CHotKeyManager(void);
	~CHotKeyManager(void);

	BOOL RegisterHotKey(UINT id,UINT fsModifiers,UINT vk,IBizWindow *pBizWnd);
	BOOL UnregisterHotKey(UINT fsModifiers,UINT vk,IBizWindow *pBizWnd);

	BOOL OnHotKey(UINT fsModifiers,UINT vk);
	BOOL OnNoOperatTimeout(UINT nTimeout);
	BOOL NeedNoOperatTimeoutNotify();

protected:
	CMapStringToOb    m_hotKeyMap;
	CMapStringToOb    m_noOperatNotifyMap;
	HWND GetFrameHandle(HWND hChild);
};
