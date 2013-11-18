#pragma once

#include "..\Common\HSWinner.h"

typedef CArray<IBizWindow*,IBizWindow*>  CBizWindowsArray;

class CLinkageMsgManager
{
public:
	CLinkageMsgManager(void);
	~CLinkageMsgManager(void);

	BOOL RegisterLinkageMsg(UINT nMsg,IBizWindow *pBizWnd);
	BOOL UnregisterLinkageMsg(UINT nMsg,IBizWindow *pBizWnd);
	int DispatchLinkageMsg(UINT nMsg,WPARAM wParam,LPARAM lParam,BOOL bPost);

protected:
	CMapWordToOb       m_msgMap;
};
