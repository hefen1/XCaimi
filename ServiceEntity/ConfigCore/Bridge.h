/*******************************************************************************
* Copyright (c)2010, 杭州恒生电子股份有限公司
* All rights reserved.
*
* 文件名称：Bridge.h
* 摘    要：用于在ConfigCore内部各个接口实现类之间的相互调用
*
* 当前版本：
* 作    者：王超
* 完成日期：2010-12-22
* 备	 注：
*******************************************************************************/

#pragma once
#include "ConfigInterface.h"

class CBridge
{
public:
	CBridge(void);
	~CBridge(void);

	static IHsColor* GetHsColor();
	static IHsFont*  GetHsFont();
};
