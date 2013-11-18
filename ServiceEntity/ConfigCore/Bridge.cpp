#include "StdAfx.h"
#include "Bridge.h"
#include "HsColor.h"
#include "HsFont.h"

extern CHsColor gColor;
extern CHsFont  gFont;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CBridge::CBridge(void)
{
}

CBridge::~CBridge(void)
{
}

IHsColor* CBridge::GetHsColor()
{
	if (!gColor.IsInit())
	{
		gColor.Init();
	}
	return (IHsColor*)&gColor;
}

IHsFont* CBridge::GetHsFont()
{
	if (!gFont.IsInit())
	{
		gFont.Init();
	}
	return (IHsFont*)&gFont;
}