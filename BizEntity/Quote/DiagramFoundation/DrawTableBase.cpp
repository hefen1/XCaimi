#include "StdAfx.h"
#include "DrawTableBase.h"
#include <zmouse.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CDrawTableBase::CDrawTableBase()
{
	m_posMove.SetPoint(0,0);
	m_nCellID = -1;
	m_wDrawOper = 0;
	m_wSignal = NEED_NOTHING;
	m_ayCell.RemoveAll();

	m_pVerScrollBar   = NULL;
	m_pHorScrollBar   = NULL;
	m_pTitle          = NULL;

	m_pPointChange = NULL;
	m_wStyle = 0;
	m_bIsShow = TRUE;

	m_nMouseRedrawTimes = 0;
	m_nFixRow = m_nFixCol = 0;
	m_nRow = m_nCol = 0;
	m_nShowRowNum = m_nShowColNum = 0;
	m_nColWidth = 0;

	m_nTipTimeID = 0; 
}

CDrawTableBase::~CDrawTableBase(void)
{
	if (m_pPointChange)
	{// 清除坐标变换操作对象
		delete m_pPointChange;
	}
	if (m_pVerScrollBar)
	{// 清除垂直滚动条对象
		delete m_pVerScrollBar;
	}
	if (m_pHorScrollBar)
	{// 清除水平滚动条对象
		delete m_pHorScrollBar;
	}
}

void CDrawTableBase::SetStyle(WORD wStyle)
{
	m_wStyle = wStyle;
	if (m_pPointChange == NULL)
	{
		if (m_wStyle & TABLE_STYLE_SCREEN)
		{
			m_pPointChange = new CScreenPointChangeBehavior(this);
		}
		else if (m_wStyle & TABLE_STYLE_SNAKE)
		{
			m_pPointChange = new CSnakePointChangeBehavior(this);
		}
	}
}

void CDrawTableBase::SetRowCount(int nRowCount)   // 设置行数
{
	ASSERT(nRowCount >= 0);
	m_nRow = nRowCount;
}

int CDrawTableBase::GetRowCount()
{
	return m_nRow;
}

void CDrawTableBase::SetFixRowCount(int nRowFixCount) // 设置固定行数
{
	ASSERT(nRowFixCount <= m_nRow && nRowFixCount >= 0);
	m_nFixRow = nRowFixCount;
}

int CDrawTableBase::GetFixRowCount()
{
	return m_nFixRow;
}

void CDrawTableBase::SetRowHeight(int nHeight)    // 设置行高
{
	ASSERT(nHeight > 0);
	m_nRowHeight = nHeight;	
}

int CDrawTableBase::GetRowHeight()
{
	return m_nRowHeight;
}

void CDrawTableBase::SetColCount(int nColCount)   // 设置列数
{
	ASSERT(nColCount >= 0);
	if (m_wStyle & TABLE_STYLE_SNAKE)
	{
		ASSERT( (nColCount == 1) );
	}
	m_nCol = nColCount;
	m_nShowColNum = m_nShowColNum > m_nCol?m_nCol:m_nShowColNum;
	m_ayColInfo.SetSize(m_nCol);
}

int CDrawTableBase::GetColCount()
{
	return m_nCol;
}

void CDrawTableBase::SetFixColCount(int nColFixCount) // 设置固定列数
{
	ASSERT(nColFixCount <= m_nCol && nColFixCount >= 0);
	m_nFixCol = nColFixCount;
}

int CDrawTableBase::GetFixColCount()
{
	return m_nFixCol;
}

int CDrawTableBase::GetShowColCount()
{
	return m_nShowColNum;
}
int CDrawTableBase::GetShowRowCount()
{
	return m_nShowRowNum;
}

CRect CDrawTableBase::GetTableRect()
{
	return m_rcTable;
}

CPoint CDrawTableBase::GetMove()
{
	return m_posMove;
}

void CDrawTableBase::SetColInfo(int nIndex, ColInfoStruct* pColInfo)   // 设置某列的列属性
{
	ASSERT(nIndex >= 0 && nIndex < m_nCol && pColInfo);
	m_ayColInfo[nIndex].Copy(pColInfo);
}

const ColInfoStruct* CDrawTableBase::GetColInfo(int nIndex)
{
	if (nIndex >= 0 && nIndex < m_nCol)
		return &m_ayColInfo[nIndex];
	else
		return NULL;
}

int CDrawTableBase::ColNumToIndex(int nColNum)
{
	if (m_wStyle & TABLE_STYLE_SNAKE)
	{
		if (nColNum >= 0 && nColNum <= m_nCol)
			return nColNum;
		else 
			return -1;
	}
	else if (m_wStyle & TABLE_STYLE_SCREEN)
	{
		if (nColNum < m_nFixCol && nColNum >= 0)
		{
			return nColNum;
		}

		if (nColNum >= m_nFixCol && nColNum <= m_nCol)
		{
			return nColNum + m_posMove.x;
		}
	}
	return -1;
}

void CDrawTableBase::SetCellCount(int nCount, BOOL bRelease)
{
	if (bRelease)
	{
		int nOldCount = m_ayCell.GetSize();
		if (nOldCount > nCount)
		{
			for (int i = nCount; i < nOldCount; i++)
			{
				CDrawCell* pCell = m_ayCell[i];
				if (pCell && !::IsBadReadPtr(pCell, 1))
					delete pCell;
			}
			m_ayCell.RemoveAt(nCount, nOldCount - nCount);
		}
	}
	ReStoreTable();
	m_ayCell.SetSize(nCount);
}

int CDrawTableBase::GetCellCount()
{
	return m_ayCell.GetSize();
}

void CDrawTableBase::CreateVerScroll(int dwStyle)
{
	if (m_pVerScrollBar)
		return;
	
	dwStyle &= ~HSSB_HORZ;
	dwStyle |=  HSSB_VERT;
	m_pVerScrollBar = new CHSScrollBar(dwStyle);
	m_pVerScrollBar->SetScrollNotify(this);
}

void CDrawTableBase::CreateHorScroll(int dwStyle)
{
	if (m_pHorScrollBar && (m_wStyle & TABLE_STYLE_SNAKE))
		return;
	
	dwStyle &= ~HSSB_VERT;
	dwStyle |=  HSSB_HORZ;
	m_pHorScrollBar = new CHSScrollBar(dwStyle);
	m_pHorScrollBar->SetScrollNotify(this);
}

void CDrawTableBase::CreateTitle()
{
	if (m_pTitle)
		return;	
	
	m_pTitle = new CHSTitle;
	m_pTitle->SetScrollNotify(this);
}

CDrawCell* CDrawTableBase::CreateCell(CArray<ItemConstructionStruct>* pAyItemStuct, DWORD dwCellMask, int nIndex)
{
	if (pAyItemStuct == NULL)
		return NULL;

	if (nIndex >= 0 && nIndex < m_ayCell.GetSize())
	{
		CDrawCell* pCellExist = m_ayCell[nIndex];
		if (pCellExist && !::IsBadReadPtr(pCellExist, 1))
		{// 如果已经创建内存 根据属性重构一个
			pCellExist->ReCreate(pAyItemStuct, dwCellMask);		
			return pCellExist;
		}
		else
		{// 不存在 重新创建
			m_ayCell[nIndex] = new CDrawCell(pAyItemStuct, dwCellMask); 
			return m_ayCell[nIndex];
		}
	}
	else
	{
		CDrawCell* pCell = new CDrawCell(pAyItemStuct, dwCellMask);
		m_ayCell.Add(pCell);
		return pCell;
	}
}

CDrawCell* CDrawTableBase::CreateCell(CString str, WORD lClr, WORD lFont, DWORD nMask, DWORD nCellMask, DWORD dwExtent, int nIndex)
{
	
	if (nIndex >= 0 && nIndex < m_ayCell.GetSize())
	{
		CDrawCell* pCellExist = m_ayCell[nIndex];
		if (pCellExist && !::IsBadReadPtr(pCellExist, 1))
		{// 如果已经创建内存 根据属性重构一个
			pCellExist->ReCreate(str, lClr, lFont, nMask, nCellMask, dwExtent);		
			return pCellExist;
		}
		else
		{// 不存在 重新创建
			m_ayCell[nIndex] = new CDrawCell(str, lClr, lFont, nMask, nCellMask, dwExtent);
			return m_ayCell[nIndex];
		}
	}
	else
	{
		CDrawCell* pCell = new CDrawCell(str, lClr, lFont, nMask, nCellMask, dwExtent);
		m_ayCell.Add(pCell);
		return pCell;
	}
}

BOOL CDrawTableBase::RemoveCol(int nStartCol, int nCount)
{
	if (nStartCol < 0 || nCount <= 0 || nStartCol + nCount > m_nCol || !(m_wStyle & TABLE_STYLE_SCREEN))
		return FALSE;
	
	// 先把要删除这部分内存释放掉
	int nStartCell = nStartCol * m_nRow;
	int nCountCell = nCount * m_nRow;
	for (int i = nStartCell; i < nCountCell; i++)
	{
		CDrawCell* pCell = m_ayCell[i];
		delete pCell;
		pCell = NULL;
	}
	// 再从一维数组中移出数据
	m_ayCell.RemoveAt(nStartCell, nCountCell);
	// 列属性数组也要删除数据
	m_ayColInfo.RemoveAt(nStartCol, nCount);
	SetColCount(m_nCol - nCount);
	// 如果删除列中包含固定列 固定列要做出相应调整
	m_nFixCol = nStartCol < m_nFixCol?nStartCol:m_nFixCol;
	
	ReStoreTable();
	return TRUE;
}

BOOL CDrawTableBase::RemoveRow(int nStartRow, int nCount)
{
	if (nStartRow < 0 || nCount <= 0 || nStartRow + nCount > m_nRow || !(m_wStyle & TABLE_STYLE_SCREEN))
		return FALSE;
	
	int nNewRow = m_nRow - nCount;
	CDrawCellArray ayTempCell;
	ayTempCell.SetSize(m_nCol * nNewRow);

	int nNewCellIndex(0), nOldCellIndex(0);
	for (int nCol = 0; nCol < m_nCol; nCol++)
	{
		// 保存不需要删除的单元格
		for (int nRow = 0; nRow < nStartRow; nRow++)
			ayTempCell[nNewCellIndex++] = m_ayCell[nOldCellIndex++];
		// 删除单元格
		for (int nRow = nStartRow; nRow < nStartRow + nCount; nRow++)
		{
			CDrawCell* pCell = m_ayCell[nOldCellIndex++];
			delete pCell;
		}
		// 保存不需要删除的单元格
		for (int nRow = nStartRow + nCount; nRow < m_nRow; nRow++)
			ayTempCell[nNewCellIndex++] = m_ayCell[nOldCellIndex++];
	}

	m_ayCell.RemoveAll();
	m_ayCell.Copy(ayTempCell);
	ayTempCell.RemoveAll();
	// 设置新的参数
	m_nRow = nNewRow;
	m_nShowRowNum = m_nShowRowNum > m_nRow?m_nRow:m_nShowRowNum;
	m_nFixRow = nStartRow < m_nFixRow?nStartRow:m_nFixRow;

	ReStoreTable();
	return TRUE;
}


void CDrawTableBase::InsertCol(int nIndex, BOOL bFixRow, CArray<ItemConstructionStruct>* ayItemStuct, DWORD dwCellMask)
{// 当前只支持普通表格
	if (nIndex < 0 || nIndex > m_nCol || m_nRow < 1 || !(m_wStyle & TABLE_STYLE_SCREEN))
	{// 合法的插入位置0~m_nCol 以及当前行数必须大于0
		return;
	}
	
	// 申请要插入单元格的数量
	CDrawCellArray ayNewCell;
	ayNewCell.SetSize(m_nRow);
	// 分配新的内存空间
	if (bFixRow)
	{// 仅仅给固定列保持格式
		for (int i = 0; i < m_nFixRow; i++)
		{
			CDrawCell* pCell = new CDrawCell(ayItemStuct, dwCellMask);		
			pCell->SetEmpty();
			ayNewCell[i] = pCell;
		}
		for (int i = m_nFixRow; i < m_nRow; i++)
		{
			CDrawCell* pCell = new CDrawCell(ayItemStuct, dwCellMask);		
			ayNewCell[i] = pCell;
		}
	}
	else
	{
		for (int i = 0; i < m_nRow; i++)
		{
			CDrawCell* pCell = new CDrawCell(ayItemStuct, dwCellMask);		
			ayNewCell[i] = pCell;
		}
	}

	// 最后加入到单元格数组中
	m_ayCell.InsertAt(nIndex*m_nRow, &ayNewCell);
	SetColCount(m_nCol+1);

	ayNewCell.RemoveAll();
}
void CDrawTableBase::InsertCol(int nIndex,BOOL bFixRow, CString str, WORD wClr, WORD wFont, DWORD dwMask, DWORD dwCellMask)
{// 当前只支持普通表格
	if (nIndex < 0 || nIndex > m_nCol || m_nRow < 1 || !(m_wStyle & TABLE_STYLE_SCREEN))
	{// 合法的插入位置0~m_nCol 以及当前行数必须大于0
		return;
	}

	// 申请要插入单元格的数量
	CDrawCellArray ayNewCell;
	ayNewCell.SetSize(m_nRow);
	// 分配新的内存空间

	if (bFixRow)
	{
		for (int i = 0; i < m_nFixRow; i++)
		{
			CDrawCell* pCell = new CDrawCell(str, wClr, wFont, dwMask, dwCellMask);		
			ayNewCell[i] = pCell;
		}
		for (int i = m_nFixRow; i < m_nRow; i++)
		{
			CDrawCell* pCell = new CDrawCell(str, wClr, wFont, dwMask, dwCellMask);		
			pCell->SetEmpty();
			ayNewCell[i] = pCell;
		}
	}
	else
	{
		for (int i = 0; i < m_nRow; i++)
		{
			CDrawCell* pCell = new CDrawCell(str, wClr, wFont, dwMask, dwCellMask);		
			ayNewCell[i] = pCell;
		}

	}
	// 最后加入到单元格数组中
	m_ayCell.InsertAt(nIndex*m_nRow, &ayNewCell);
	// 添加一个新的列属性值
	ColInfoStruct pColStruct;
	m_ayColInfo.InsertAt(nIndex, pColStruct);
	// 行数加一
	m_nCol++;

	ayNewCell.RemoveAll();
}

void CDrawTableBase::CreateRowCell( int nRowCount )
{// 当前只支持普通表格
	if (nRowCount < 1 || nRowCount == m_nRow || m_nCol == 0 || !(m_wStyle & TABLE_STYLE_SCREEN))
		return;
	
	if (nRowCount < m_nRow)
	{// 需要删除操作

		CDrawCellArray ayCell;
		ayCell.SetSize(nRowCount * m_nCol);
		int nNewIndex(0);

		DWORD nBegin = ::GetTickCount();
		int nOldIndex(0);
		for (int x = 0; x < m_nCol; x++)
		{
//			DWORD Begin = ::GetTickCount();
			// 添加旧的内容
			for (int y = 0; y < nRowCount; y++)
			{
				//nOldIndex = PointToIndex(CPoint(x,y));
				CDrawCell* pCell = m_ayCell[nOldIndex++];
				ayCell[nNewIndex++] = pCell;
			}
//			DWORD End = ::GetTickCount();
//			TRACE1("CreateRowCell函数 删除行操作中 添加旧的内容 耗时 %d \n", End - Begin);
//			Begin = ::GetTickCount();
			// 删除多余的空间
			for (int y = nRowCount; y < m_nRow; y++)
			{
				//nOldIndex = PointToIndex(CPoint(x,y));
				CDrawCell* pCell = m_ayCell[nOldIndex++];
				delete pCell;
			}
//			End = ::GetTickCount();
//			TRACE1("CreateRowCell函数 删除行操作中 删除旧的内容 耗时 %d \n\n", End - Begin);
		}
		DWORD nEnd = ::GetTickCount();
		TRACE1("CreateRowCell函数 删除行操作 耗时%d \n\n", nEnd - nBegin);
		m_ayCell.RemoveAll();
		m_ayCell.Copy(ayCell);	
		ayCell.RemoveAll();
	}
	else
	{// 需要额外分配内存
		
		CDrawCellArray ayCell;
		ayCell.SetSize(nRowCount * m_nCol);

		CArray<ItemConstructionStruct> ayItemStruct;
		DWORD  dwCellMask(0);
		int nNewIndex(0);

		DWORD nBegin = ::GetTickCount();
		int nOldIndex(0);
		for (int x = 0; x < m_nCol; x++)
		{
			// 添加旧的内容
			for (int y = 0; y < m_nRow; y++)
			{
				//nOldIndex = PointToIndex(CPoint(x,y));
				CDrawCell* pCell = m_ayCell[nOldIndex++];
				ayCell[nNewIndex++] = pCell;
			}
			ayItemStruct.RemoveAll();
			m_ayCell[nOldIndex-1]->GetItemStyle(ayItemStruct);
			dwCellMask = m_ayCell[nOldIndex-1]->m_dwStyle;
			// 分配新的内存空间
			for (int y = m_nRow; y < nRowCount; y++)
			{
				CDrawCell* pCell = new CDrawCell(&ayItemStruct, dwCellMask); 
				pCell->SetEmpty();  // 需要将一些属性给清除掉
				ayCell[nNewIndex++] = pCell;
			}
		}
		DWORD nEnd = ::GetTickCount();
		TRACE1("CreateRowCell函数 添加行操作 耗时%d \n\n", nEnd - nBegin);
		m_ayCell.RemoveAll();
		m_ayCell.Copy(ayCell);
		ayCell.RemoveAll();
	}
	m_nRow = nRowCount;
}

CPoint CDrawTableBase::IndexToPoint( int nIndex )
{
	ASSERT(m_pPointChange);
	return m_pPointChange->IndexToPoint(nIndex);
}

int CDrawTableBase::PointToIndex( CPoint pos )
{
	ASSERT(m_pPointChange);
	return m_pPointChange->PointToIndex(pos);
}

BOOL CDrawTableBase::IsCellRowFixed(int nIndex)
{
	ASSERT(m_pPointChange);
	return m_pPointChange->IsCellRowFixed(nIndex);
}

BOOL CDrawTableBase::IsCellRowFixed(CPoint pos)
{
	ASSERT(m_pPointChange);
	return m_pPointChange->IsCellRowFixed(pos);
}

BOOL CDrawTableBase::IsCellColFixed(int nIndex)
{
	ASSERT(m_pPointChange);
	return m_pPointChange->IsCellColFixed(nIndex);
}

BOOL CDrawTableBase::IsCellColFixed(CPoint pos)
{
	ASSERT(m_pPointChange);
	return m_pPointChange->IsCellColFixed(pos);
}

int CDrawTableBase::GetColumn(int nIndex)
{
	return GetColumn(IndexToPoint(nIndex));
}
int CDrawTableBase::GetColumn(CPoint pos)
{
	if (m_wStyle & TABLE_STYLE_SNAKE)
	{
		if (pos.x >= 0 && pos.x < m_nCol)
			return pos.x;
		else 
			return -1;
	}
	else if (m_wStyle & TABLE_STYLE_SCREEN)
	{
		if (pos.x < m_nFixCol && pos.x >= 0)
		{
			return pos.x;
		}

		if (pos.x >= m_nFixCol && pos.x < m_nCol)
		{
			return pos.x + m_posMove.x;
		}
	}
	return -1;
}

int CDrawTableBase::GetRow(int nIndex)
{
	return GetRow(IndexToPoint(nIndex));
}
int CDrawTableBase::GetRow(CPoint pos)
{
	if (pos.y < m_nFixRow && pos.x >= 0)
	{
		return pos.y;
	}

	if (pos.y >= m_nFixRow && pos.y < m_nRow)
	{
		return pos.y + m_posMove.y;
	}
	return -1;
}

BOOL CDrawTableBase::GetCellRect( int index, CRect& rect )
{
	if (index < 0 || index > m_ayCell.GetSize())
	{
		rect.SetRectEmpty();
		return FALSE;
	}

	return GetCellRect(IndexToPoint(index), rect);
}

BOOL CDrawTableBase::GetCellRect( CPoint pos, CRect& rect )
{
	if((pos.x < 0) || (pos.y < 0) || (pos.x >= m_nCol) || (pos.y >= m_nRow))
	{
		rect.SetRectEmpty();
		return FALSE;
	}
	rect = m_rcTable;

	int nColInfoIndex(0);
	///////////////////////////////计算宽度///////////////////////////////////////////
	for (int i = 0; i < pos.x; i++)
	{
		nColInfoIndex = ColNumToIndex(i);
		if (nColInfoIndex == -1)
		{
			rect.SetRectEmpty();
			return FALSE;
		}
		rect.left += m_ayColInfo.GetAt(nColInfoIndex).nWidth;
	}
	// 最后加上自身的宽度
	nColInfoIndex = ColNumToIndex(pos.x);
	if (nColInfoIndex == -1)
	{
		rect.SetRectEmpty();
		return FALSE;
	}
	rect.right = rect.left + m_ayColInfo.GetAt(nColInfoIndex).nWidth;
	//////////////////////////////计算高度////////////////////////////////////////////
	for (int i = 0; i < pos.y; i++)
		rect.top += m_nRowHeight;
	
	// 最后加上自身的高度
	rect.bottom = rect.top + m_nRowHeight;
	return TRUE;
}

CDrawCell* CDrawTableBase::SetCellText( CPoint pos, int nItemID, const char* strText, WORD lClr, WORD lFont, DWORD lMark, DWORD lDelMark, BOOL bNeedRedraw)
{
	int nCellID = PointToIndex(pos);
	if (nCellID >= 0 && nCellID < m_ayCell.GetSize())
	{
		return SetCellText(nCellID, nItemID, strText, lClr, lFont, lMark, lDelMark,bNeedRedraw);
	}
	return NULL;
}

CDrawCell* CDrawTableBase::SetCellText(int nCellID, int nItemID, const char* strText, WORD lClr, WORD lFont, DWORD lMark, DWORD lDelMark, BOOL bNeedRedraw)
{
	if (nCellID >= 0 && nCellID < m_ayCell.GetSize())
	{
		CDrawCell* pCell = m_ayCell[nCellID];
		pCell->SetItemText(nItemID, strText, lClr, lFont, lMark, lDelMark);
		
		if (bNeedRedraw)
		{
			//////////////////////////////////////////////////////////////////////////
			// 计算需要重绘的区域
			CRect rect;
			GetCellRect(nCellID, rect);
			// 刷性区域为左右两个
			rect.InflateRect(rect.Width(),0,rect.Width(),0);
			// 规范化区域
			rect.left = (rect.left < m_rcTable.left)?m_rcTable.left:rect.left;
			rect.right = (rect.right > m_rcTable.right)?m_rcTable.right:rect.right;
			m_pRegion->Invalidate(&rect, FALSE);
		}
		return pCell;
	}
	return NULL;
}

BOOL CDrawTableBase::SetRect(CRect rc)
{
	if (!__super::SetRect(rc) && (m_nShowRowNum != 0) && (m_nShowColNum != 0))
	{// 如果大小没有改变 并且可见行列都不为0 如果已经经过计算 不需要再重复计算大小
		return FALSE;
	}
	m_rcTable = m_rect;
	CalCulateSize();
	return TRUE;
}

void CDrawTableBase::CalCulateSize()
{
	// 如果区域为0 不需要计算
	if (m_rect.IsRectEmpty())
	{
		m_nShowColNum = m_nShowRowNum = 0;
		return;
	}
	CRect rc = m_rect;
	// 如果有标题框 先预留标题框区域
	if (m_pTitle)
		rc = m_pTitle->SetRect(m_rect);

	// 如果行列有一个为0 直接返回
	if (m_nCol <= 0 || m_nRow <= 0)
		return;

	// 如果有垂直滚动条 先预留出滚动条的区域 
	CRect rcVScrollResidue(0,0,0,0);
	if (m_pVerScrollBar && m_pVerScrollBar->GetAutoSize())
	{
		rcVScrollResidue = m_pVerScrollBar->SetRect(rc);	
		m_rcTable = rcVScrollResidue ;
	}
	// 如果有水平滚动条 再预留出水平滚动条的区域
	CRect rcHScrollResidue(0,0,0,0);
	if (m_pHorScrollBar && m_pHorScrollBar->GetAutoSize())
	{
		rcHScrollResidue = m_pHorScrollBar->SetRect(m_rcTable);
		m_rcTable = rcHScrollResidue;
	}
	BOOL bNeedReCalculate = FALSE;
	do 
	{
		bNeedReCalculate = FALSE;
		int nVerScrollRange(0);  // 垂直滚动条滚动范围
		int nHorScrollRange(0);  // 水平滚动条滚动范围
		if (m_wStyle & TABLE_STYLE_SCREEN)
		{// 普通表格

			// 计算当前可见行
			if ( (m_wStyle & TABLE_HEIGHT_CHANGABLE) && (m_pVerScrollBar == NULL))
			{// 如果表格行高可变 并且滚动条不存在		
				m_nRowHeight = m_rcTable.Height() / m_nRow; // 行高为当前区域高度 除以 总行数
				m_nShowRowNum = m_nRow; // 全都可以显示
			}
			else
			{
				m_nShowRowNum = m_rcTable.Height() / m_nRowHeight;
				m_nShowRowNum = (m_nShowRowNum > m_nRow)?m_nRow:m_nShowRowNum;
			}
			// 计算当前可见列
			if (m_wStyle & TABLE_WIDTH_CHANGABLE)
			{// 如果表格列宽可变 但是固定列的列宽是无法改变 仍然使用初始值
				
				ColInfoStruct* pColInfo = NULL;
				int nTotalWidth = 0;
				for (int i = 0; i < m_nFixCol; i++)
				{
					pColInfo = &m_ayColInfo.GetAt(i);
					nTotalWidth += pColInfo->nWidth;

					if (nTotalWidth >= m_rcTable.Width()) // 总宽度不能超过区域
					{
						m_nShowColNum = i;
						break;
					}
				}

				// 非固定列的列宽 剩余的区域平均分配
				if (nTotalWidth < m_rcTable.Width())
				{
					int nWidth = (m_rcTable.Width()-nTotalWidth) / (m_nCol - m_nFixCol);
					for (int i = m_nFixCol; i < m_nCol - 1; i++)
					{
						pColInfo = &m_ayColInfo.GetAt(i);
						pColInfo->nWidth = nWidth;

						nTotalWidth += nWidth;
					}
					// 最后一列宽度是当前图元宽度减去之前所有列的宽度
					m_ayColInfo.GetAt(m_nCol-1).nWidth = m_rcTable.Width() - nTotalWidth; 
					m_nShowColNum = m_nCol;
				}
			}
			else
			{
				int nWidth(0);
				m_nShowColNum = m_nCol;
				// 因为列宽可变 下面的算法会出现最后几列显示一半的情况
				// 需要将水平滚动项设置成 m_nCol - m_nFixCol - 1
				for (int i = 0; i < m_ayColInfo.GetCount(); i++)
				{
					nWidth += m_ayColInfo.GetAt(i).nWidth;
					if (nWidth >= m_rcTable.Width())
					{
						m_nShowColNum = i;
						break;
					}
				}
			}
			if (m_nShowRowNum == 0 || m_nShowColNum == 0)
			{
				// 当前都显示不出来 滚动条也不需要了
				nVerScrollRange = 0;
				nHorScrollRange = 0;
			}
			else
			{
				nVerScrollRange = m_nRow - m_nShowRowNum;
				if(m_wStyle & TABLE_HSCROLL_NO_ENDING )
					nHorScrollRange = m_nCol - m_nFixCol - 1;
				else
					nHorScrollRange = m_nCol - m_nShowColNum;
				
			}
		}
		else if (m_wStyle & TABLE_STYLE_SNAKE)
		{// 如果是蛇形风格表格 这种情况列宽必须都一样 而且列宽必须有一个初始值 因为m_nColWidth就记录初始宽度

			m_nFixCol = 0; // 固定列一定为0
			// 行数
			m_nRow = m_ayCell.GetSize();
			m_nShowRowNum = m_rcTable.Height() / m_nRowHeight;
//			m_nRow = m_nShowRowNum;
			// 列数
			if (m_nColWidth == 0)
				m_nColWidth = m_ayColInfo.GetAt(0).nWidth;
			int nWidth = m_nColWidth;
			m_nShowColNum = m_rcTable.Width() / nWidth;
			m_nCol = m_nShowColNum;
			// 重新调整当前可见列数、总列数以及列宽
			m_ayColInfo.SetSize(m_nCol);

			nWidth = m_rcTable.Width() / m_nShowColNum;
			int nTemp(0);
			for (int i = 0; i < m_ayColInfo.GetCount() - 1; i++)
			{
				nTemp += nWidth;
				m_ayColInfo.GetAt(i).nWidth = nWidth;
			}
			m_ayColInfo.GetAt(m_ayColInfo.GetCount()-1).nWidth = m_rcTable.Width() - nTemp;
			
			if (m_nShowColNum == 0 || m_nShowRowNum == 0)
			{			
				// 当前都显示不出来 滚动条也不需要了
				nVerScrollRange = 0;
			}
			else
			{
				nVerScrollRange = m_ayCell.GetSize() - (m_nShowRowNum-m_nFixRow)*m_nShowColNum - m_nFixRow;
			}
		}
		CRect rcTemp;
		if (m_pVerScrollBar)
		{// 根据计算出来的 滚动条范围 重新计算滚动条区域 
			if (m_pVerScrollBar->GetAutoSize())
			{
				rcTemp = m_pVerScrollBar->SetRect(rc, nVerScrollRange);
				// 如果重新计算滚动条之后 表格区域发生改变 则需要重新计算
				if (rcTemp != rcVScrollResidue)
				{
					m_rcTable = rcTemp;
					rcVScrollResidue = rcTemp;
					bNeedReCalculate = TRUE;
				}
			}
			else
				m_pVerScrollBar->SetRange(nVerScrollRange);
		}
		if (m_pHorScrollBar)
		{
			if (m_pHorScrollBar->GetAutoSize())
			{
				rcTemp = m_pHorScrollBar->SetRect(rcTemp, nHorScrollRange);
				if (rcTemp != rcHScrollResidue)
				{
					m_rcTable = rcTemp;
					rcHScrollResidue = rcTemp;
					bNeedReCalculate = TRUE;
				}
			}
			else
				m_pHorScrollBar->SetRange(nHorScrollRange);	
		}
	} while (bNeedReCalculate);


	// 将滑块移到默认位置
	if (m_pHorScrollBar)
		m_pHorScrollBar->SetBlockToDefaultPosition();
	if (m_pVerScrollBar)
		m_pVerScrollBar->SetBlockToDefaultPosition();
}

int CDrawTableBase::Draw( CDC* pDC )
{
	if (m_bVisable == FALSE || m_rect.IsRectEmpty())
	{// 当前图元不可见
		return FALSE;
	}

	// 画背景
	DrawTableBK(pDC);

	//////////////////////////////////////////////////////////////////////////
	// 画单元格
	BOOL bRet = FALSE;
	for (int x = 0; x < m_nShowColNum+1 && x < m_nCol; x++)
	{
		for (int y = 0; y < m_nShowRowNum+1 && y < m_nRow; y++)
		{
			CPoint pos(x,y);
			if (IsCellRowFixed(pos) || IsCellColFixed(pos))
				bRet = DrawCell(pDC, pos);
			else
			{// 如果不是固定行列要 显示受标识符控制
				if (m_bIsShow)
				{
					bRet = DrawCell(pDC, pos);
				}
			}
		}
	}
	//////////////////////////////////////////////////////////////////////////
	// 画标题 标题默认情况下是一直显示出来
	if (m_pTitle)
	{
		m_pTitle->Draw(pDC);
	}

	//////////////////////////////////////////////////////////////////////////
	// 滚动条和表格 受标识符控制
	// 画滚动条
	if (m_pVerScrollBar && m_bIsShow)
	{
		m_pVerScrollBar->Draw(pDC);
	}

	if (m_pHorScrollBar && m_bIsShow)
	{
		m_pHorScrollBar->Draw(pDC);
	}
	//////////////////////////////////////////////////////////////////////////

	if (m_wDrawOper & NEED_RESIZE)
	{// 改变大小
		DrawLine(m_posNow, pDC);
	}
	else if (m_wDrawOper & NEED_MOVE)
	{// 移动
		int nCursor;
		int nLeft;
		int nCellID = GetCellID(m_posNow, nCursor, nLeft); // 鼠标当前所在单元格
		CPoint CellPos;
		if (nCellID != -1)
		{
			CellPos = IndexToPoint(nCellID);
			if (m_wSignal & NEED_MOVE)
			{// 单元格列移动 计算需要画线的位置
				if (CellPos.x >= m_nFixCol)
				{// 非固定列
					CString strText = m_ayCell[m_nCellID]->GetItemText(-1);
					DrawRect(m_posNow, strText, pDC);
					CRect CellRect;
					GetCellRect(nCellID, CellRect);
					// 左边
					if ( (m_posNow.x - nLeft) < CellRect.Width() / 2)
						DrawLine(CPoint(CellRect.left, CellRect.top), pDC);
					// 右边
					else
						DrawLine(CPoint(CellRect.right, CellRect.top), pDC);
				}
		    }	
			else
			{// Item列移动 计算需要画线的位置
				if (m_nCellID == nCellID)
				{// 没有移出当前单元格
					CDrawCell* pCell = m_ayCell[nCellID];
					int nDiff;
					// 当前鼠标移动到的Item
					int nItemID = pCell->GetItemID(m_posNow, m_ayColInfo.GetAt(CellPos.x).nWidth, nLeft, nDiff); 
					if (nItemID != -1)
					{
						CString strText = m_ayCell[m_nCellID]->GetItemText(m_ayCell[m_nCellID]->m_nItemID);
						DrawRect(m_posNow, strText, pDC);
						// 获取Item矩形
						CRect CellRect;
						GetCellRect(nCellID, CellRect);
						CRect ItemRect = pCell->GetItemRect(CellRect, nItemID);
						// 左边
						if ( nDiff < ItemRect.Width() / 2)
							DrawLine(CPoint(ItemRect.left, ItemRect.top), pDC);
						// 右边
						else
							DrawLine(CPoint(ItemRect.right, ItemRect.top), pDC);
					}
				}
			}
	    }
	}
	else if (m_wDrawOper & NEED_TIP)
	{// 显示Tip
		if (m_nCellID >= 0 && m_nCellID < m_ayCell.GetSize())
		{		
			CDrawCell* pCell = m_ayCell[m_nCellID];
			if (pCell->m_dwStyle & CELL_TIP)
			{
				CString strContent = GetTipText(m_nCellID, pCell->m_nItemID);
				DrawTip(pDC, strContent);
			}
		}
	}

	return TRUE;
}

BOOL CDrawTableBase::DrawTableBK(CDC* pDC)
{
	pDC->FillSolidRect(&m_rcTable, RGB(0,0,0));
	return TRUE;
}

BOOL CDrawTableBase::DrawSelf(CDC* pDC, CPoint pos, int nItemID, CRect& rcItem)
{
	return FALSE;
}

BOOL CDrawTableBase::DrawCell(CDC* pDC, CPoint pos)
{
	int nIndex = PointToIndex(pos);

	if (nIndex == -1)
		return FALSE;
		
	CRect rect;
	GetCellRect(pos, rect);
	CDrawCell* pCell = m_ayCell[nIndex];
	if (pCell)
		return pCell->DrawCell(pDC, rect, pos, this);
	else
		return FALSE;
	
}

BOOL CDrawTableBase::DrawTip(CDC* pDC, const CString& strContent)
{
	if (strContent.IsEmpty())
		return TRUE;
	
	//TRACE("strContent %s \n", strContent);
	//////////////////////////////////////////////////////////////////////////
	// 获取颜色字体
	COLORREF clrFG = DrawTool::GetColor(TEXT_COLOR_TIPFG);
	COLORREF clrBK = DrawTool::GetColor(TEXT_COLOR_TIPBK);
	CFont* pFont = DrawTool::GetFont(TEXT_FONT_TIP);

	//////////////////////////////////////////////////////////////////////////
	// 计算所需要的矩形大小
	CFont* pOldFont = pDC->SelectObject(pFont);
	CRect rcNeed(0,0,0,0);
	pDC->DrawText(strContent, &rcNeed, DT_CALCRECT); // 获取绘制字体所需要的大小
	CSize sz(rcNeed.Width()+6, rcNeed.Height()+4);
	// 规范尺寸
	CPoint pos(m_posNow);
	// 偏移位置
	pos.x += OFFSET_CX; 
	pos.y += OFFSET_CY;
	//::ClientToScreen(m_pRegion->GetHostHandle(), &pos);
	
	// 获取当前表格区域大小
	int nCXScreen = m_rcTable.right;//::GetSystemMetrics(SM_CXSCREEN);
	int nCYScreen = m_rcTable.bottom;//::GetSystemMetrics(SM_CYSCREEN);
	pos.x = pos.x < 0?0:pos.x;  pos.x = pos.x > (nCXScreen - sz.cx)?(nCXScreen-sz.cx):pos.x;
	pos.y = pos.y < 0?0:pos.y; 	pos.y = pos.y > (nCYScreen - sz.cy)?(nCYScreen-sz.cy):pos.y;
	//::ScreenToClient(m_pRegion->GetHostHandle(), &pos);

	// 构造矩形
	CRect rc(pos, sz);
	pDC->FillSolidRect(&rc, RGB(238,198,16));
	rc.DeflateRect(1,1,1,1);
	pDC->FillSolidRect(&rc, clrBK);

	COLORREF clrOld = pDC->SetTextColor(clrFG);
	int nOldBkMode = pDC->SetBkMode(TRANSPARENT);
	
	pDC->DrawText(strContent, strContent.GetLength(), &rc, DT_VCENTER);

	// 还原CDC
	pDC->SetBkMode(nOldBkMode);
	pDC->SetTextColor(clrOld);
	pDC->SelectObject(pOldFont);

	return TRUE;
}

void CDrawTableBase::RestoreOper()
{
	m_wDrawOper &= ~NEED_TIP;
	KillTipTimer();
}

BOOL CDrawTableBase::MouseMove( UINT nFlags, CPoint point, hsRefMouseMsg* pData /*= NULL*/ )
{	
	if (!m_rcTable.PtInRect(point))
	{
		return FALSE;
	}
	m_posNow = point;
	
	int nCursor(0);
	int nSignal(0);
	m_nCellID = GetCellID(point, &nCursor, NULL, &nSignal);
	m_wSignal = nSignal;
	SetMouseCursor(nCursor);

	if (m_nMouseRedrawTimes != 0)
	{
		InValidate(NULL, FALSE);
		m_nMouseRedrawTimes--;
	}

	return TRUE;
}

int CDrawTableBase::GetCellID( CPoint point, int &nCursor, int &nLeft)
{
	if (!m_rcTable.PtInRect(point))
		return -1;
	
	int nIndex = 0;
	int nWidth = m_rcTable.left;
	int nCellPosX = -1;
	int nDiffx = 0;  // 距离左边框的像素

	/////////////////////////////////列扫描///////////////////////////////////////
	// 先从固定列 为了得到正确当前列属性
	for (int x = 0; x < m_nShowColNum && x < m_nCol; x++)
	{
		int nColInfoIndex = ColNumToIndex(x);
		if (nColInfoIndex != -1)
		{
			nWidth += m_ayColInfo.GetAt(nColInfoIndex).nWidth;
			if (nWidth > point.x)
			{
				nCellPosX = x;
				nDiffx = m_ayColInfo.GetAt(nColInfoIndex).nWidth - (nWidth - point.x);
				nLeft  = nWidth - m_ayColInfo.GetAt(nColInfoIndex).nWidth;
				break;
			}
		}
	}
	// 行扫描失败
	if (nCellPosX == -1)
		return -1;

	int nHeight = m_rcTable.top;
	int nCellPosY = -1;
	int nDiffy = 0; // 距离上边框的像素
	//////////////////////////////////行扫描////////////////////////////////////////	
	// 先从固定行
	for (int y = 0; y < m_nShowRowNum && y < m_nRow; y++)
	{
		nHeight += m_nRowHeight;
		if (nHeight > point.y)
		{
			nCellPosY = y;
			nDiffy = m_nRowHeight - (nHeight - point.y);
			break;
		}
	}
	// 列扫描失败
	if (nCellPosY == -1)
		return -1;

	nIndex = PointToIndex(CPoint(nCellPosX,nCellPosY));
	if (nIndex != -1)
	{
		CDrawCell* pCell = m_ayCell[nIndex];
		if (pCell)
		{
			if ( (nDiffx < pCell->m_logPen.lopnWidth.x + 3) && (pCell->m_dwStyle & CELL_RESIZABLEWE) )
			{// 左边框
				nCursor = MOUSE_SIZEWE;
			}
			else
			{
				nCursor = MOUSE_ARROW;
			}
		}
		else 
			return -1;
	}
	return nIndex;
}

int CDrawTableBase::GetCellID(CPoint point, int* pCursor, int* pLeft, int* pSignal)
{
	if (!m_rcTable.PtInRect(point))
		return -1;

	//////////////////////////////////////////////////////////////////////////
	// 还原状态
	m_wDrawOper = 0;
	m_wSignal = NEED_NOTHING; 
	KillTipTimer();
	if (m_nCellID >= 0 && m_nCellID < m_ayCell.GetSize())
	{
		CDrawCell* pPreCell = m_ayCell[m_nCellID];
		pPreCell->ResetItem();
	}
	//////////////////////////////////////////////////////////////////////////

	int nIndex = 0;
	int nWidth = m_rcTable.left;
	int nCellPosX = -1;
	int nDiffx = 0;  // 距离左边框的像素
	int nLeft(0);    // 左边框的横坐标
	int nCursor(0);  // 鼠标光标样式
	int nSignal(0);  // 需要操作的标识
	
	/////////////////////////////////列扫描///////////////////////////////////////
	// 先从固定列 为了得到正确当前列属性
	for (int x = 0; x < m_nShowColNum && x < m_nCol; x++)
	{
		int nColInfoIndex = ColNumToIndex(x);
		if (nColInfoIndex >= 0 && nColInfoIndex < m_ayColInfo.GetCount())
		{
			nWidth += m_ayColInfo.GetAt(nColInfoIndex).nWidth;
			if (nWidth > point.x)
			{
				nCellPosX = x;
				nDiffx = m_ayColInfo.GetAt(nColInfoIndex).nWidth - (nWidth - point.x);
				nLeft  = nWidth - m_ayColInfo.GetAt(nColInfoIndex).nWidth;
				nWidth = m_ayColInfo.GetAt(nColInfoIndex).nWidth;   // nWidth最后保存为鼠标所在单元格的宽度
				break;
			}
		}
	}
	// 行扫描失败
	if (nCellPosX == -1)
		return -1;

	int nHeight = m_rcTable.top;
	int nCellPosY = -1;
	int nDiffy = 0; // 距离上边框的像素
	//////////////////////////////////行扫描////////////////////////////////////////	
	// 先从固定行
	for (int y = 0; y < m_nShowRowNum && y < m_nRow; y++)
	{
		nHeight += m_nRowHeight;
		if (nHeight > point.y)
		{
			nCellPosY = y;
			nDiffy = m_nRowHeight - (nHeight - point.y);
			break;
		}
	}
	// 列扫描失败
	if (nCellPosY == -1)
		return -1;

	nIndex = PointToIndex(CPoint(nCellPosX,nCellPosY));
	if (nIndex != -1)
	{
		CDrawCell* pCell = m_ayCell[nIndex];
		if (pCell)
		{// 找到鼠标所在的单元格

			//////////////////////////////////////////////////////////////////////////
			// 判断是否要显示Tip
			if ( (pCell->m_dwStyle & CELL_TIP) )
			{
				m_nTipTimeID = ::SetTimer(m_pRegion->GetHostHandle(), (long)this+TIMER_TIP, DELAY_SHOWTIP_TIMESPAN, &CDrawTableBase::ShowTipProc);
			}
			//////////////////////////////////////////////////////////////////////////

			if ( (nDiffx < pCell->m_logPen.lopnWidth.x + 3) && (pCell->m_dwStyle & CELL_RESIZABLEWE) && nCellPosX > m_nFixCol)
			{// 左边框
				nCursor = MOUSE_SIZEWE;
				nSignal = NEED_RESIZE;
			}
			else
			{// 单元格内部
				nCursor = pCell->GetItemSignal(point, nWidth, nLeft);
				
				if (nCursor & MOUSE_SIZEWE)
				{// 单元小格可以被改变大小
				}
				else if ( nCursor & MOUSE_HAND )
				{// 单元小格内容可被选中 当前只能以高亮的形式展示出来
					nSignal = NEED_TEXTHIGHLIGHT;
					SetRedrawTimes(2); // 高亮要重绘两次 一次高亮显示 另一次高亮去除
				}
				else 
				{
					if ( (pCell->m_dwStyle & CELL_MOVABLE) && (nCellPosX >= m_nFixCol))
					{// 单元格列移动位置
						nSignal = NEED_MOVE;
					}
				}
			}
		}
	}
	if (pCursor)
		*pCursor = nCursor;
	if (pLeft)
		*pLeft = nLeft;
	if (pSignal)
		*pSignal = nSignal;
	
	return nIndex;
}

BOOL CDrawTableBase::MouseDown( UINT nFlags, CPoint point, hsRefMouseMsg* pData /*= NULL*/ )
{
	
	//////////////////////////////////////////////////////////////////////////
	// 因为滚轮事件的存在或者菜单弹出 会导致鼠标单击事件与鼠标移动事件之间 鼠标当前的单元格可能发生改变 
	// 重新在获取一下鼠标当前所在的单元格 以及标记
	m_posNow = point;
	int nCursor(0);
	int nSignal(0);
	m_nCellID = GetCellID(point, &nCursor, NULL, &nSignal);
	m_wSignal = nSignal;
	SetMouseCursor(nCursor);

	//////////////////////////////////////////////////////////////////////////
	// 先处理滚动条鼠标事件
	if (m_pVerScrollBar)
	{// 如果滚动条存在 处理滚动条
		if (m_pVerScrollBar->MouseDown(nFlags, point))
		{// 如果单击是在滚动条上 则直接返回
			return TRUE;
		}
	}
	if (m_pHorScrollBar)
	{
		if (m_pHorScrollBar->MouseDown(nFlags, point))
		{
			return TRUE;
		}
	}
	//////////////////////////////////////////////////////////////////////////
	// 再处理表格鼠标事件
	// 过滤
	if (m_nCellID == -1)
	{
		return FALSE;
	}
	if ((m_wSignal & NEED_NOTHING) && (m_ayCell[m_nCellID]->m_wSignal & NEED_NOTHING) )
	{
		return FALSE;
	}
	//////////////////////////////////////////////////////////////////////////
	// 判断是否有选中行
	if (m_wStyle & TABLE_ROW_SELECT)
	{	
		int nPreActiveRow = GetActiveRow();
		int nRow = GetRow(m_nCellID);
		if (nRow >= m_nFixRow)
		{
			RemoveActiveRow(nPreActiveRow);
			SetActiveRow(nRow);
			SetRedrawTimes(1);
		}
	}
	//////////////////////////////////////////////////////////////////////////
	// 以下是操作过程的绘制 比如：移动，改变大小
	m_wDrawOper = m_wSignal | m_ayCell[m_nCellID]->m_wSignal;
	HWND hwnd = m_pRegion->GetHostHandle();

	CPoint prePos(-1,-1);
	int    nDiff = 0;

	//////////////////////////////////////////////////////////////////////////
	// 限定鼠标移动范围
	CRect selfRect = m_rcTable;
	CPoint originPos(0,0);
	::ClientToScreen(hwnd, &originPos);
	selfRect.left += originPos.x;	selfRect.right += originPos.x;
	selfRect.top += originPos.y;    selfRect.bottom += originPos.y;
	::ClipCursor(&selfRect);
	//

	::SetCapture(hwnd);
	while(TRUE)
	{	
		MSG msg;
		VERIFY(::GetMessage(&msg, NULL, 0, 0));

		BOOL bEixstLoop = FALSE;
		switch (msg.message) 
		{
		case WM_MOUSEMOVE:
			{
				m_posNow.SetPoint(msg.pt.x, msg.pt.y);
				::ScreenToClient(hwnd, &m_posNow); // 要转换为屏幕坐标

				nDiff = m_posNow.x - point.x;
				if(msg.wParam != MK_LBUTTON)
				{// 如果左键没有按住 则直接退出
					bEixstLoop = TRUE;
					break;
				}
				CRect rectNeedRedraw = m_rcTable;
				if (m_wDrawOper & NEED_RESIZE)
				{// 改变大小
					if (prePos.x != -1)
					{// 计算需要重绘的区域				
						rectNeedRedraw.left = (prePos.x<m_posNow.x)?prePos.x:m_posNow.x;
						rectNeedRedraw.right = (prePos.x>m_posNow.x)?prePos.x:m_posNow.x;
					}
					else
					{
						rectNeedRedraw.left = m_posNow.x;
						rectNeedRedraw.right = rectNeedRedraw.left + 2;
					}
					rectNeedRedraw.InflateRect(3,0,3,0);
					m_pRegion->Invalidate(&rectNeedRedraw, FALSE);
				}
				else if (m_wDrawOper & NEED_MOVE)
				{// 左右移动
					CRect rectNew;
					CRect rectOld;
					if (m_wSignal & NEED_MOVE)
					{// 单元格移动
						rectNew = GetDrawRect(m_posNow,TRUE);
						rectOld = GetDrawRect(prePos, TRUE);
					}
					else
					{// Item移动
						rectNew = GetDrawRect(m_posNow,FALSE);
						rectOld = GetDrawRect(prePos, FALSE);
					}

					rectNeedRedraw = m_rcTable;
					if (prePos.x != -1)
					{// 计算需要重绘的区域
						rectNeedRedraw.left = (rectNew.left<rectOld.left)?rectNew.left:rectOld.left;
						rectNeedRedraw.right = (rectNew.right>rectOld.right)?rectNew.right:rectOld.right;
					}
					else
					{
						rectNeedRedraw.left = rectNew.left;
						rectNeedRedraw.right = rectNew.right;
					}
					// 要包含画线的区域
					CRect rectCell;
					GetCellRect(m_nCellID, rectCell);
					rectNeedRedraw.left = (rectNeedRedraw.left < rectCell.left)?rectNeedRedraw.left:rectCell.left;
					rectNeedRedraw.right = (rectNeedRedraw.right > rectCell.right)?rectNeedRedraw.right:rectCell.right;
					rectNeedRedraw.InflateRect(1,0,1,0);
					m_pRegion->Invalidate(&rectNeedRedraw, FALSE);
				}
				prePos = m_posNow;
				break;
			}			
		case WM_LBUTTONUP:
		case WM_RBUTTONDOWN:
			{
				if (m_posNow == point)
				{// 没有移动过
					if ( IsCellColFixed(m_nCellID) )
					{
						OnFixedColumnClick();
					}
					if (IsCellRowFixed(m_nCellID))
					{
						OnFixedRowClick();
					}
				}
			}
		case WM_KEYDOWN:
			bEixstLoop = TRUE;
			break;
		default:
			DispatchMessage(&msg);
		}
		if(bEixstLoop)
			break;
	}

	::ReleaseCapture();
	::ClipCursor(NULL);
	if (m_wDrawOper & NEED_RESIZE)
	{// 需要改变大小
		if ( (m_wSignal & NEED_RESIZE) )
		{// 单元格之间改变大小
			CPoint posRight = IndexToPoint(m_nCellID);
			ReSize(posRight.x-1, nDiff);
		}
		else
		{// 单元格内 Item之间改变大小
			CPoint posNowCell = IndexToPoint(m_nCellID);
			int nItemLeft = m_ayCell[m_nCellID]->m_nItemID - 1;
			int nItemRight = nItemLeft + 1;
			int nColInfoIndex = ColNumToIndex(posNowCell.x);

			// 同列的单元格随表格风格的不同而不同
			if (m_wStyle & TABLE_STYLE_SCREEN)
			{
				for (int y = 0; y < m_nRow; y++)
				{
					int nIndex = PointToIndex(CPoint(posNowCell.x, y));
					m_ayCell[nIndex]->ReSize(nItemLeft, nItemRight, m_ayColInfo.GetAt(nColInfoIndex).nWidth, nDiff);
				}
			}
			else if (m_wStyle & TABLE_STYLE_SNAKE)
			{
				for (int i = 0; i < m_ayCell.GetSize(); i++)
				{
					posNowCell = IndexToPoint(i);
					m_ayCell[i]->ReSize(nItemLeft, nItemRight, m_ayColInfo.GetAt(nColInfoIndex).nWidth, nDiff);
				}
			}
		}
	}
	else if (m_wDrawOper & NEED_MOVE)
	{// 需要调整位置
		int nCursor;
		int nLeft;
		int nCellID = GetCellID(m_posNow, nCursor, nLeft); // 鼠标当前所在单元格
		CPoint CellPos = IndexToPoint(nCellID);
		if (nCellID != -1)
		{
			CellPos = IndexToPoint(nCellID);
			int nColInfoIndex = ColNumToIndex(CellPos.x);
			if (m_wSignal & NEED_MOVE)
			{// 单元格列移动 
				if (CellPos.x >= m_nFixCol)
				{// 非固定列
					int nColInsert(-1); // 需要被插入的位置
					// 左边
					if ( (m_posNow.x - nLeft) < m_ayColInfo.GetAt(nColInfoIndex).nWidth / 2)
						nColInsert = CellPos.x;
					// 右边
					else
						nColInsert = CellPos.x + 1;

					CellPos = IndexToPoint(m_nCellID);
					MoveCol(CellPos.x, nColInsert);
				}
			}	
			else
			{// Item列移动 计算需要画线的位置
				if (m_nCellID == nCellID)
				{// 没有移出当前单元格
					CDrawCell* pCell = m_ayCell[nCellID];
					int nDiff;
					// 当前鼠标移动到的Item
					int nItemID = pCell->GetItemID(m_posNow, m_ayColInfo.GetAt(nColInfoIndex).nWidth, nLeft, nDiff); 
					if (nItemID != -1)
					{
						int nItemInsert(-1); // 需要被插入的位置;
						// 获取Item矩形
						CRect CellRect;
						GetCellRect(nCellID, CellRect);
						CRect ItemRect = pCell->GetItemRect(CellRect, nItemID);

						// 左边
						if ( nDiff < ItemRect.Width() / 2)
							nItemInsert = nItemID;
						// 右边
						else
							nItemInsert = nItemID + 1;

						int nBegin = pCell->m_nItemID;
						if (m_wStyle & TABLE_STYLE_SCREEN)
						{
							for (int y = 0; y < m_nRow; y++)
							{
								int nIndex = PointToIndex(CPoint(CellPos.x, y));
								m_ayCell[nIndex]->Move(nBegin, nItemInsert);
							}
						}
						else if (m_wStyle & TABLE_STYLE_SNAKE)
						{
							for (int i = 0; i < m_ayCell.GetSize(); i++)
								m_ayCell[i]->Move(nBegin, nItemInsert);
						}
					}
				}
			}
		}
	}

	if (m_wDrawOper != NEED_NOTHING)
		InValidate(NULL, FALSE);	
	return TRUE;
}

BOOL CDrawTableBase::MouseDBDown(UINT nFlags, CPoint point, hsRefMouseMsg* pData)
{
	// 因为存在滚轮事件 所以当前鼠标所在的单元格需要重新计算一下
	int nCursor(0);
	int nSignal(0);
	m_nCellID = GetCellID(point, &nCursor, NULL, &nSignal);
	m_wSignal = nSignal;
	SetMouseCursor(nCursor);

	return TRUE;
}

int CDrawTableBase::OnMouseWheel( UINT nFlags, short zDelta, CPoint pt )
{
	if (m_pVerScrollBar && m_pVerScrollBar->GetRange() != 0)
	{
		int nDiff = (-1) * zDelta / WHEEL_DELTA;
		Scroll(nDiff, TRUE, TRUE);
		return TRUE;
	}
	return FALSE;
}

BOOL CDrawTableBase::RButtonDown(UINT nFlags,CPoint point, HWND hWndParent, hsRefMouseMsg* pData)
{
	int nCursor(0);
	int nSignal(0);
	m_nCellID = GetCellID(point, &nCursor, NULL, &nSignal);
	m_wSignal = nSignal;
	SetMouseCursor(nCursor);

	//////////////////////////////////////////////////////////////////////////
	// 静态的属性改变 比如：选中
	if (m_nCellID != -1)
	{
		if (m_wStyle & TABLE_ROW_SELECT)
		{	
			int nPreActiveRow = GetActiveRow();
			int nRow = GetRow(m_nCellID);
			if (nRow >= m_nFixRow)
			{
				RemoveActiveRow(nPreActiveRow);
				SetActiveRow(nRow);
				SetRedrawTimes(1);
			}
		}

	}
	
	if (m_nMouseRedrawTimes)
		InValidate(NULL, FALSE);

	return TRUE;
}

void CDrawTableBase::ReSize(int nCellCol, int nDiff )
{
	if (nCellCol < 0 || nCellCol > m_nCol)
	{
		return;
	}

	int nColIndexLeft = ColNumToIndex(nCellCol);
	if (m_ayColInfo.GetAt(nColIndexLeft).nWidth + nDiff < CELL_MIN_WIDTH)
		nDiff = CELL_MIN_WIDTH - m_ayColInfo.GetAt(nColIndexLeft).nWidth;

	m_ayColInfo.GetAt(nColIndexLeft).nWidth += nDiff;
	CalCulateSize();
}

int	CDrawTableBase::OnKey(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if ( !(m_wStyle & TABLE_ROW_SELECT))
		return FALSE;

	int nActiveRow = GetActiveRow();
	//	TRACE1("当前激活行为 %d \n", nActiveRow);
	int nPreAcitveRow = nActiveRow;
	if (nChar == VK_UP)
		nActiveRow--;
	else if (nChar == VK_DOWN)
		nActiveRow++;
	
	if (nPreAcitveRow != nActiveRow)
	{
		//////////////////////////////////////////////////////////////////////////
		// 规范法激活行
		nActiveRow = nActiveRow < m_nFixRow?m_nFixRow:nActiveRow;
		nActiveRow = nActiveRow > m_nRow-m_nFixRow?m_nRow-m_nFixRow:nActiveRow;
		if (nPreAcitveRow == nActiveRow)
			return FALSE;

		if (nActiveRow - m_posMove.y < m_nFixRow)
		{
			Scroll(nActiveRow-m_nFixRow-GetMove().y, TRUE, FALSE);
			// 		// 因为表格往上滚动了一行 当前激活行和之前的激活行都往下移动一行
			// 		nActiveRow++;
			// 		nPreAcitveRow++;
		}
		else if (nActiveRow - m_posMove.y >= m_nShowRowNum)
		{
			Scroll(nActiveRow-m_nShowRowNum-GetMove().y+1, TRUE, FALSE);
			// 		// 因为表格往下滚动了一行 当前激活行和之前的激活行都往上移动一行
			// 		nActiveRow--;
			// 		nPreAcitveRow--;
		}

		RemoveActiveRow(nPreAcitveRow);
		SetActiveRow(nActiveRow);
		//	TRACE1("滚动之后的激活行为 %d \n", nActiveRow);
		InValidate(NULL, FALSE);
		return TRUE;
	}	
	return FALSE;
}

void CDrawTableBase::DrawLine( CPoint pos, CDC* pDC )
{
	CPen pen(PS_SOLID, 1, RGB(255, 255, 255));
	CPen* pOldPen = pDC->SelectObject(&pen);

	if (pos != CPoint(-1, -1))
	{
		pDC->MoveTo(pos.x, m_rcTable.top);
		pDC->LineTo(pos.x, m_rcTable.bottom);
	}
	pDC->SelectObject(pOldPen);
}

void CDrawTableBase::DrawRect(CPoint pos, CString strText, CDC* pDC)
{
	CPen pen(PS_SOLID, 1, RGB(255,0,0));
	CPen* pOldPen = pDC->SelectObject(&pen);

	if (pos != CPoint(-1, -1))
	{
		CRect rect;
		if (m_wSignal & NEED_MOVE)
		{// 单元格列移动
			rect = GetDrawRect(pos, TRUE);
		}
		else
		{// 单元格内部Item移动
			rect = GetDrawRect(pos, FALSE);
		}
		pDC->Rectangle(&rect);
		int nOldBkMode = pDC->SetBkMode(TRANSPARENT);
		COLORREF clrOldFg = pDC->SetTextColor(RGB(58,58,58));
		
		pDC->DrawText(strText, strText.GetLength(), &rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		
		pDC->SetTextColor(clrOldFg);
		pDC->SetBkMode(nOldBkMode);
	}

	pDC->SelectObject(pOldPen);
	pen.DeleteObject();
}

CRect CDrawTableBase::GetDrawRect( CPoint pos,BOOL bIsCell)
{
	CRect rect;
	if (pos.x == -1 || pos.y == -1)
	{
		rect.SetRectEmpty();
	}
	else
	{
		CPoint posCell = IndexToPoint(m_nCellID);
		int nCellWidth = m_ayColInfo.GetAt(posCell.x).nWidth;
		if (bIsCell)
		{
			rect.left   = pos.x - nCellWidth / 2;
			rect.right  = pos.x + nCellWidth / 2;
			rect.top    = pos.y - m_nRowHeight / 2;
			rect.bottom = pos.y + m_nRowHeight / 2;
		}
		else
		{
			return m_ayCell[m_nCellID]->GetDrawRect(pos, nCellWidth, m_nRowHeight);
		}

	}
	return rect;
}

void CDrawTableBase::MoveCol( int nBegin, int nEnd )
{
	if (!m_wStyle & TABLE_STYLE_SCREEN)
	{// 只有普通表格才可以实现
		return;
	}
	if (nBegin < m_nFixCol || nBegin >= m_nCol || nEnd < m_nFixCol || nEnd > m_nCol)
	{// 传入数据非法
		return;
	}

	if (nBegin == nEnd || nBegin + 1 == nEnd)
	{// 位置不变不需要移动
		return;
	}

	// 转化成对应的列ID
	int nColIndexBegin = ColNumToIndex(nBegin);
	int nColIndexEnd = ColNumToIndex(nEnd);

	// 将nBegin列的所有单元格备份
	CDrawCellArray ayTemp;
	ayTemp.SetSize(m_nRow);
	int nCellID = nColIndexBegin * m_nRow;
	for (int i = 0; i < m_nRow; i++, nCellID++)
	{
		CDrawCell* pCell = m_ayCell[nCellID];
		ayTemp.SetAt(i, pCell);
	}

	if (nBegin < nEnd)
	{// 向右插入列
		for (int x = nColIndexBegin + 1; x < nColIndexEnd; x++)
		{
			for (int y = 0; y < m_nRow; y++)
			{
				int nIndex = x * m_nRow + y;
				CDrawCell* pCell = m_ayCell[nIndex];

				nIndex = (x-1) * m_nRow + y;
				m_ayCell[nIndex] = pCell;
			}
		}

		for (int y = 0; y < m_nRow; y++)
		{
			int nIndex = (nColIndexEnd-1) * m_nRow + y;
			CDrawCell* pCell = ayTemp[y];
			m_ayCell[nIndex] = pCell;
		}

		// 对应的列属性也要发生改变

		ColInfoStruct colInfo;
		colInfo.Copy(&m_ayColInfo.GetAt(nColIndexBegin));
		m_ayColInfo.InsertAt(nColIndexEnd, colInfo);
		m_ayColInfo.RemoveAt(nColIndexBegin);
	}
	else
	{// 向左插入列
		for (int x = nColIndexBegin - 1; x >= nColIndexEnd; x--)
		{
			for (int y = 0; y < m_nRow; y++)
			{
				int nIndex = x * m_nRow + y;
				CDrawCell* pCell = m_ayCell[nIndex];

				nIndex = (x+1) * m_nRow + y;
				m_ayCell[nIndex] = pCell;
			}
		}

		for (int y = 0; y < m_nRow; y++)
		{
			int nIndex = nColIndexEnd * m_nRow + y;
			CDrawCell* pCell = ayTemp[y];
			m_ayCell[nIndex] = pCell;
		}

		// 对应的列属性也要发生变化
		ColInfoStruct colInfo;
		colInfo.Copy(&m_ayColInfo.GetAt(nColIndexBegin));
		m_ayColInfo.InsertAt(nColIndexEnd, colInfo);
		m_ayColInfo.RemoveAt(nColIndexBegin + 1); // 因为nEnd插入到了左边 原来的nBegin向右移了一个位置
	}
	ayTemp.RemoveAll();
}

int CDrawTableBase::GetItemID(int nCellID, CString str)
{
	if (nCellID < 0 || nCellID >= m_ayCell.GetSize())
	{
		return -1;
	}

	return m_ayCell[nCellID]->GetItemID(str);
}

BOOL CDrawTableBase::GetItemStyle(int nCellID, int nItemID, ItemConstructionStruct& sConstructionStruct)
{
	if (nCellID < 0 || nCellID >= m_ayCell.GetSize())
	{
		return FALSE;
	}
	return m_ayCell[nCellID]->GetItemStyle(nItemID, sConstructionStruct);
}

int CDrawTableBase::GetActiveCellID()
{
	return m_nCellID;
}

int CDrawTableBase::GetScrollRange(int bIsVert)
{	
	int nRange(0);
	if (bIsVert)
	{
		if (m_wStyle & TABLE_STYLE_SCREEN)
			nRange = m_nRow - m_nShowRowNum;
		else if (m_wStyle & TABLE_STYLE_SNAKE)
			nRange = m_ayCell.GetSize() - (m_nShowRowNum-m_nFixRow)*m_nShowColNum - m_nFixRow;
	}
	else
	{
		//nRange = m_nCol - m_nShowColNum;
		nRange = m_nCol - m_nFixCol - 1;
	}
	return nRange;
}

void CDrawTableBase::OnFixedColumnClick()
{// 基类表格 没有执行语句
	return;
}

void CDrawTableBase::OnFixedRowClick()
{// 基类表格 没有执行语句
	return;
}

CString CDrawTableBase::GetTipText(int nCellID, int nItemID)
{// 基类表格 没有执行语句 
	return "";
}

void CDrawTableBase::SetMouseCursor(int nCursor)
{
	if (nCursor & MOUSE_SIZEWE)
	{
		m_pRegion->SetCursor(IDC_SIZEWE);
	}
	else if (nCursor & MOUSE_ARROW)
	{
		m_pRegion->SetCursor(IDC_ARROW);
	}
	else if (nCursor & MOUSE_HAND)
	{
		m_pRegion->SetCursor(IDC_HAND);
	}
}

void CDrawTableBase::ReStoreTable()
{
	m_nMouseRedrawTimes = 0;
	m_nCellID = -1;
	m_posNow.SetPoint(0,0);
	m_wDrawOper = NEED_NOTHING;
	m_wSignal = NEED_NOTHING;
	KillTipTimer();
}

void CDrawTableBase::ResetCell(CDrawCell* pCell)
{
	if (pCell)
	{
		pCell->ResetItem();
	}
}

void CDrawTableBase::ResetCell(int nCellID)
{
	if (nCellID >= 0 && nCellID < m_ayCell.GetSize())
	{
		m_ayCell[nCellID]->ResetItem();
	}
}

void CDrawTableBase::ResetCell(CPoint pos)
{
	ResetCell(PointToIndex(pos));
}

void CDrawTableBase::InValidate(CRect* pRect, BOOL bErase/* = TRUE*/) // 刷新区域 给外部调用
{
	if (pRect == NULL)
	{
		CRect rc = m_rect;
		if (m_pVerScrollBar)
		{
			CRect rcVer = m_pVerScrollBar->GetRect();
			rc.UnionRect(&rc, &rcVer);
		}
		if (m_pHorScrollBar)
		{
			CRect rcHor = m_pHorScrollBar->GetRect();
			rc.UnionRect(&rc, &rcHor);
		}
		m_pRegion->Invalidate(&rc, bErase);
	}
	else
	{
		m_pRegion->Invalidate(pRect, bErase);
	}
}

CDrawCell* CDrawTableBase::GetCell(CPoint pos)
{
	int nIndex = PointToIndex(pos);
	return GetCell(nIndex);
}

CDrawCell* CDrawTableBase::GetCell(int nCellID)
{
	if (nCellID < 0 || nCellID >= m_ayCell.GetSize())
	{
		return NULL;
	}

	return m_ayCell[nCellID];
}

CDrawItem* CDrawTableBase::GetItem(CPoint pos, int nItem)
{
	return GetItem(PointToIndex(pos), nItem);
}

CDrawItem* CDrawTableBase::GetItem(int nCellID, int nItem)
{
	if (nCellID >= 0 && nCellID < m_ayCell.GetSize())
	{
		return GetItem(m_ayCell[nCellID], nItem);
	}
	return NULL;
}

CDrawItem* CDrawTableBase::GetItem(CDrawCell* pCell, int nItem)
{
	if (pCell && nItem >=0 && nItem < pCell->m_ayItem.GetCount())
	{
		return pCell->m_ayItem[nItem];
	}
	return NULL;
}

void CDrawTableBase::GetCellText(CPoint pos, int nItem, CString& str)
{
	GetCellText(PointToIndex(pos), nItem, str);
}
void CDrawTableBase::GetCellText(int nCellID, int nItem, CString& str)
{
	if (nCellID < 0 || nCellID >= m_ayCell.GetSize())
		return;

	str = m_ayCell[nCellID]->GetItemText(nItem);
}	

void CDrawTableBase::RemoveAllCell()
{
	m_ayCell.DeleteAll();
	m_nRow = m_nCol = 0;
	m_nShowRowNum = m_nShowColNum = 0;
	m_nFixRow = m_nFixCol = 0;

	m_ayColInfo.RemoveAll();
	ReStoreTable();
}

void CDrawTableBase::CellMove(int nDiff)
{
	int nRange(0);
	if (m_wStyle & TABLE_STYLE_SCREEN)
		nRange = m_nRow - m_nFixRow;
	else if (m_wStyle & TABLE_STYLE_SNAKE)
		nRange = m_ayCell.GetSize() - m_nFixRow;

	// 超过范围
	if ( nDiff > nRange)
		nDiff = nRange;
	else if ( nDiff < nRange * (-1))
		nDiff = nRange * (-1);

	if (nDiff > 0)
	{// 向上平移	
		for (int y = m_nFixRow; y + nDiff < m_nRow; y++)
		{
			for (int x = 0; x < m_nCol; x++)
			{
				int nPreIndex = PointToIndex(CPoint(x,y));
				int nIndex = PointToIndex(CPoint(x, y+nDiff));
				m_ayCell[nPreIndex]->Copy(m_ayCell[nIndex]);
			}
		}
		for (int y = m_nRow - nDiff; y < m_nRow; y++)
		{
			for (int x = 0; x < m_nCol; x++)
			{
				m_ayCell[PointToIndex(CPoint(x, y))]->SetEmpty();
			}
		}
	}
	else if (nDiff < 0)
	{
		int nAbsDiff = -1 * nDiff;
		for (int y = m_nRow - 1; y - nAbsDiff >= m_nFixRow; y--)
		{
			for (int x = 0; x < m_nCol; x++)
			{
				int nPreIndex = PointToIndex(CPoint(x,y));
				int nIndex = PointToIndex(CPoint(x, y-nAbsDiff));
				m_ayCell[nPreIndex]->Copy(m_ayCell[nIndex]);
			}
		}
		for (int y = m_nFixRow + nAbsDiff; y >= m_nFixRow; y--)
		{
			for (int x = 0; x < m_nCol; x++)
			{
				m_ayCell[PointToIndex(CPoint(x, y))]->SetEmpty();
			}
		}
	}
//	m_pRegion->Invalidate(&m_rcTable, FALSE);
}

BOOL CDrawTableBase::SetActiveRow(int nRow)
{
	if (nRow < 0 || nRow >= m_nRow)
		return FALSE;

	ASSERT(m_wStyle & TABLE_STYLE_SCREEN);
	for (int x = 0; x < m_nCol; x++)
	{
		int nCellID = x * m_nRow + nRow;
		CDrawCell* pCell = m_ayCell[nCellID];
		pCell->m_dwStyle |= CELL_BK_HIGHLIGHT;
	}
	return TRUE;
}

BOOL CDrawTableBase::RemoveActiveRow(int nRow)
{
	if (nRow < 0 || nRow >= m_nRow)
		return FALSE;
	
	ASSERT(m_wStyle & TABLE_STYLE_SCREEN);
	int nActiveCol = GetActiveCol();
	for (int x = 0; x < m_nCol; x++)
	{
		if (x == nActiveCol)
		// 过滤掉同时也是激活列	
			continue;

		int nCellID = x * m_nRow + nRow;
		CDrawCell* pCell = m_ayCell[nCellID];
		pCell->m_dwStyle &= ~CELL_BK_HIGHLIGHT;
	}
	return TRUE;
}

int CDrawTableBase::GetActiveRow()
{
	ASSERT(m_wStyle & TABLE_STYLE_SCREEN);
	for (int y = 0; y < m_nRow; y++)
	{// 先从第一列 行扫描
		int nCellID = y;
		CDrawCell* pCell = m_ayCell[nCellID];
		if (pCell->m_dwStyle & CELL_BK_HIGHLIGHT)
		{// 找到第一个高亮单元 再行扫描
			int x(0);
			for ( ; x < m_nCol; x++)
			{	
				nCellID = x * m_nRow + y;
				pCell = m_ayCell[nCellID];
				if ( !(pCell->m_dwStyle & CELL_BK_HIGHLIGHT) )
					break;
			}
			if (x == m_nCol)
				return y;
		}
	}
	return -1;
}

BOOL CDrawTableBase::SetActiveCol(int nCol)
{
	if (nCol < 0 || nCol >= m_nCol)
		return FALSE;

	ASSERT(m_wStyle & TABLE_STYLE_SCREEN);	
	for (int y = 0; y < m_nRow; y++)
	{
		int nCellID = nCol * m_nRow + y;
		CDrawCell* pCell = m_ayCell[nCellID];
		pCell->m_dwStyle |= CELL_BK_HIGHLIGHT;
	}
	return TRUE;
}

BOOL CDrawTableBase::RemoveActiveCol(int nCol)
{
	if (nCol < 0 || nCol >= m_nCol)
		return FALSE;

	ASSERT(m_wStyle & TABLE_STYLE_SCREEN);
	int nActiveRow = GetActiveRow();
	for (int y = 0; y < m_nRow; y++)
	{
		if (y == nActiveRow)
		// 过滤掉同时也是激活行	
			continue;

		int nCellID = nCol * m_nRow + y;
		CDrawCell* pCell = m_ayCell[nCellID];
		pCell->m_dwStyle &= ~CELL_BK_HIGHLIGHT;
	}
	return TRUE;
}

int  CDrawTableBase::GetActiveCol()
{
	if (m_wStyle & TABLE_STYLE_SCREEN)
	{
		for (int x = 0; x < m_nCol; x++)
		{// 先从第一行开始 行扫描
			int nCellID = x * m_nRow;
			CDrawCell* pCell = m_ayCell[nCellID];
			if (pCell->m_dwStyle & CELL_BK_HIGHLIGHT)
			{
				int y(0);
				for (; y < m_nRow; y++)
				{// 再列扫描
					nCellID = x * m_nRow + y;
					pCell = m_ayCell[nCellID];
					if ( !(pCell->m_dwStyle & CELL_BK_HIGHLIGHT) )
						break;
				}
				if (y == m_nRow)
					return x;
			}
		}
	}
	return -1;
}

BOOL CDrawTableBase::Scroll(int nDiff, BOOL bIsVert /*= TRUE*/, BOOL bReDraw /*= TRUE*/)
{
	CPoint point(m_posMove);
	int nRange = GetScrollRange(bIsVert);
	if (bIsVert == TRUE)
	{
		m_posMove.y += nDiff;
		if (m_posMove.y < 0)
		{
			m_posMove.y = 0;
		}
		else if (m_posMove.y > nRange)
		{
			m_posMove.y = nRange;
		}
	}
	else
	{
		m_posMove.x += nDiff;
		if (m_posMove.x < 0)
		{
			m_posMove.x = 0;
		}
		else if (m_posMove.x > nRange)
		{
			m_posMove.x = nRange;
		}
	}

	TRACE1("m_posMove.y = %d \n", m_posMove.y);
	if (point != m_posMove)
	{// 有移动过

		if (bIsVert)
		{
			if (m_pVerScrollBar)
				m_pVerScrollBar->OnScroll(m_posMove.y - point.y);
		}
		else 
		{
			if (m_pHorScrollBar)
				m_pHorScrollBar->OnScroll(m_posMove.x - point.x);

		}

		int nCursor(0);
		int nSignal(0);
		m_nCellID = GetCellID(m_posNow, &nCursor, NULL, &nSignal);
		m_wSignal = nSignal;
		SetMouseCursor(nCursor);	
		
		if (bReDraw)
			InValidate(&m_rcTable, FALSE);
		return TRUE;
	}
	else
		return FALSE;
}

BOOL CDrawTableBase::SetPosition(int nPosition, BOOL bIsVert, BOOL bIsRedraw)
{
	int nRange = GetScrollRange(bIsVert);
	if (bIsVert)
	{// 滚动条垂直

		if (nPosition < 0)
			nPosition = 0;
		else if (nPosition > nRange)
			nPosition = nRange;
		
		if (m_posMove.y == nPosition)
			return FALSE;
		m_posMove.y = nPosition;
	}
	else
	{// 滚动条水平
		
		if (nPosition < 0)
			nPosition = 0;
		else if (nPosition > nRange)
			nPosition = nRange;
		
		if (m_posMove.x == nPosition)
			return FALSE;
		m_posMove.x = nPosition;
	}
	
	if (bIsRedraw)
	{
		int nCursor(0);
		int nSignal(0);
		m_nCellID = GetCellID(m_posNow, &nCursor, NULL, &nSignal);
		m_wSignal = nSignal;
		SetMouseCursor(nCursor);

		InValidate(&m_rcTable, FALSE);
	}
	return TRUE;
}

void CDrawTableBase::ScrollInvalidate( CRect* pRect, BOOL bErase /*= TRUE*/ )
{
	InValidate(pRect, bErase);
}

HWND CDrawTableBase::GetHandle()
{
	if (m_pRegion)
	{
		return m_pRegion->GetHostHandle();
	}
	return (HWND)(-1);
}

void CDrawTableBase::TitleInvalidate(CRect* pRect, BOOL bErase /*= TRUE*/)
{
	InValidate(pRect, bErase);
}

void CDrawTableBase::KillTipTimer()
{
	if (m_nTipTimeID != 0)
	{
		KillTimer(m_pRegion->GetHostHandle(), m_nTipTimeID);
		m_nTipTimeID = 0;
	}
}

VOID CALLBACK CDrawTableBase::ShowTipProc( HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime )
{
	CDrawTableBase* pThis = (CDrawTableBase*)(idEvent-TIMER_TIP);
	KillTimer(hwnd, idEvent);

	CPoint pos(0,0);
	::GetCursorPos(&pos);
	::ScreenToClient(pThis->m_pRegion->GetHostHandle(), &pos);

	CRect rc = pThis->GetTableRect();
	if (rc.PtInRect(pos))
	{
		pThis->SetDrawOper(NEED_TIP);
		pThis->SetRedrawTimes(1);
		pThis->InValidate(NULL, FALSE);
	}
}