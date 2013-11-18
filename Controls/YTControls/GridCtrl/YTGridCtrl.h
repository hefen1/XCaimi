#pragma once

#include "GridCtrl.h"
class CYTGridCtrl: public	CGridCtrl
{
public:
	CYTGridCtrl();
	~CYTGridCtrl();

	void SetSelectClr(COLORREF clr);
};