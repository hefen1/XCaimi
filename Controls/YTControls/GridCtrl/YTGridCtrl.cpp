#include "stdafx.h"
#include "YTGridCtrl.h"

CYTGridCtrl::CYTGridCtrl()
{

}

CYTGridCtrl::~CYTGridCtrl()
{

}

void CYTGridCtrl::SetSelectClr(COLORREF clr)
{
	for (int row = 0; row < GetRowCount(); row++)
	{
		for(int col =0; col < GetColumnCount(); col++)
		{
			CGridCellBase* pCell = GetCell(row, col);
			pCell->SetSelectColor(clr);
		}
	}
}