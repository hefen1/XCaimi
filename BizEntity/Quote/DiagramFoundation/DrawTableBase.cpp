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
	{// �������任��������
		delete m_pPointChange;
	}
	if (m_pVerScrollBar)
	{// �����ֱ����������
		delete m_pVerScrollBar;
	}
	if (m_pHorScrollBar)
	{// ���ˮƽ����������
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

void CDrawTableBase::SetRowCount(int nRowCount)   // ��������
{
	ASSERT(nRowCount >= 0);
	m_nRow = nRowCount;
}

int CDrawTableBase::GetRowCount()
{
	return m_nRow;
}

void CDrawTableBase::SetFixRowCount(int nRowFixCount) // ���ù̶�����
{
	ASSERT(nRowFixCount <= m_nRow && nRowFixCount >= 0);
	m_nFixRow = nRowFixCount;
}

int CDrawTableBase::GetFixRowCount()
{
	return m_nFixRow;
}

void CDrawTableBase::SetRowHeight(int nHeight)    // �����и�
{
	ASSERT(nHeight > 0);
	m_nRowHeight = nHeight;	
}

int CDrawTableBase::GetRowHeight()
{
	return m_nRowHeight;
}

void CDrawTableBase::SetColCount(int nColCount)   // ��������
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

void CDrawTableBase::SetFixColCount(int nColFixCount) // ���ù̶�����
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

void CDrawTableBase::SetColInfo(int nIndex, ColInfoStruct* pColInfo)   // ����ĳ�е�������
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
		{// ����Ѿ������ڴ� ���������ع�һ��
			pCellExist->ReCreate(pAyItemStuct, dwCellMask);		
			return pCellExist;
		}
		else
		{// ������ ���´���
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
		{// ����Ѿ������ڴ� ���������ع�һ��
			pCellExist->ReCreate(str, lClr, lFont, nMask, nCellMask, dwExtent);		
			return pCellExist;
		}
		else
		{// ������ ���´���
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
	
	// �Ȱ�Ҫɾ���ⲿ���ڴ��ͷŵ�
	int nStartCell = nStartCol * m_nRow;
	int nCountCell = nCount * m_nRow;
	for (int i = nStartCell; i < nCountCell; i++)
	{
		CDrawCell* pCell = m_ayCell[i];
		delete pCell;
		pCell = NULL;
	}
	// �ٴ�һά�������Ƴ�����
	m_ayCell.RemoveAt(nStartCell, nCountCell);
	// ����������ҲҪɾ������
	m_ayColInfo.RemoveAt(nStartCol, nCount);
	SetColCount(m_nCol - nCount);
	// ���ɾ�����а����̶��� �̶���Ҫ������Ӧ����
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
		// ���治��Ҫɾ���ĵ�Ԫ��
		for (int nRow = 0; nRow < nStartRow; nRow++)
			ayTempCell[nNewCellIndex++] = m_ayCell[nOldCellIndex++];
		// ɾ����Ԫ��
		for (int nRow = nStartRow; nRow < nStartRow + nCount; nRow++)
		{
			CDrawCell* pCell = m_ayCell[nOldCellIndex++];
			delete pCell;
		}
		// ���治��Ҫɾ���ĵ�Ԫ��
		for (int nRow = nStartRow + nCount; nRow < m_nRow; nRow++)
			ayTempCell[nNewCellIndex++] = m_ayCell[nOldCellIndex++];
	}

	m_ayCell.RemoveAll();
	m_ayCell.Copy(ayTempCell);
	ayTempCell.RemoveAll();
	// �����µĲ���
	m_nRow = nNewRow;
	m_nShowRowNum = m_nShowRowNum > m_nRow?m_nRow:m_nShowRowNum;
	m_nFixRow = nStartRow < m_nFixRow?nStartRow:m_nFixRow;

	ReStoreTable();
	return TRUE;
}


void CDrawTableBase::InsertCol(int nIndex, BOOL bFixRow, CArray<ItemConstructionStruct>* ayItemStuct, DWORD dwCellMask)
{// ��ǰֻ֧����ͨ���
	if (nIndex < 0 || nIndex > m_nCol || m_nRow < 1 || !(m_wStyle & TABLE_STYLE_SCREEN))
	{// �Ϸ��Ĳ���λ��0~m_nCol �Լ���ǰ�����������0
		return;
	}
	
	// ����Ҫ���뵥Ԫ�������
	CDrawCellArray ayNewCell;
	ayNewCell.SetSize(m_nRow);
	// �����µ��ڴ�ռ�
	if (bFixRow)
	{// �������̶��б��ָ�ʽ
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

	// �����뵽��Ԫ��������
	m_ayCell.InsertAt(nIndex*m_nRow, &ayNewCell);
	SetColCount(m_nCol+1);

	ayNewCell.RemoveAll();
}
void CDrawTableBase::InsertCol(int nIndex,BOOL bFixRow, CString str, WORD wClr, WORD wFont, DWORD dwMask, DWORD dwCellMask)
{// ��ǰֻ֧����ͨ���
	if (nIndex < 0 || nIndex > m_nCol || m_nRow < 1 || !(m_wStyle & TABLE_STYLE_SCREEN))
	{// �Ϸ��Ĳ���λ��0~m_nCol �Լ���ǰ�����������0
		return;
	}

	// ����Ҫ���뵥Ԫ�������
	CDrawCellArray ayNewCell;
	ayNewCell.SetSize(m_nRow);
	// �����µ��ڴ�ռ�

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
	// �����뵽��Ԫ��������
	m_ayCell.InsertAt(nIndex*m_nRow, &ayNewCell);
	// ���һ���µ�������ֵ
	ColInfoStruct pColStruct;
	m_ayColInfo.InsertAt(nIndex, pColStruct);
	// ������һ
	m_nCol++;

	ayNewCell.RemoveAll();
}

void CDrawTableBase::CreateRowCell( int nRowCount )
{// ��ǰֻ֧����ͨ���
	if (nRowCount < 1 || nRowCount == m_nRow || m_nCol == 0 || !(m_wStyle & TABLE_STYLE_SCREEN))
		return;
	
	if (nRowCount < m_nRow)
	{// ��Ҫɾ������

		CDrawCellArray ayCell;
		ayCell.SetSize(nRowCount * m_nCol);
		int nNewIndex(0);

		DWORD nBegin = ::GetTickCount();
		int nOldIndex(0);
		for (int x = 0; x < m_nCol; x++)
		{
//			DWORD Begin = ::GetTickCount();
			// ��Ӿɵ�����
			for (int y = 0; y < nRowCount; y++)
			{
				//nOldIndex = PointToIndex(CPoint(x,y));
				CDrawCell* pCell = m_ayCell[nOldIndex++];
				ayCell[nNewIndex++] = pCell;
			}
//			DWORD End = ::GetTickCount();
//			TRACE1("CreateRowCell���� ɾ���в����� ��Ӿɵ����� ��ʱ %d \n", End - Begin);
//			Begin = ::GetTickCount();
			// ɾ������Ŀռ�
			for (int y = nRowCount; y < m_nRow; y++)
			{
				//nOldIndex = PointToIndex(CPoint(x,y));
				CDrawCell* pCell = m_ayCell[nOldIndex++];
				delete pCell;
			}
//			End = ::GetTickCount();
//			TRACE1("CreateRowCell���� ɾ���в����� ɾ���ɵ����� ��ʱ %d \n\n", End - Begin);
		}
		DWORD nEnd = ::GetTickCount();
		TRACE1("CreateRowCell���� ɾ���в��� ��ʱ%d \n\n", nEnd - nBegin);
		m_ayCell.RemoveAll();
		m_ayCell.Copy(ayCell);	
		ayCell.RemoveAll();
	}
	else
	{// ��Ҫ��������ڴ�
		
		CDrawCellArray ayCell;
		ayCell.SetSize(nRowCount * m_nCol);

		CArray<ItemConstructionStruct> ayItemStruct;
		DWORD  dwCellMask(0);
		int nNewIndex(0);

		DWORD nBegin = ::GetTickCount();
		int nOldIndex(0);
		for (int x = 0; x < m_nCol; x++)
		{
			// ��Ӿɵ�����
			for (int y = 0; y < m_nRow; y++)
			{
				//nOldIndex = PointToIndex(CPoint(x,y));
				CDrawCell* pCell = m_ayCell[nOldIndex++];
				ayCell[nNewIndex++] = pCell;
			}
			ayItemStruct.RemoveAll();
			m_ayCell[nOldIndex-1]->GetItemStyle(ayItemStruct);
			dwCellMask = m_ayCell[nOldIndex-1]->m_dwStyle;
			// �����µ��ڴ�ռ�
			for (int y = m_nRow; y < nRowCount; y++)
			{
				CDrawCell* pCell = new CDrawCell(&ayItemStruct, dwCellMask); 
				pCell->SetEmpty();  // ��Ҫ��һЩ���Ը������
				ayCell[nNewIndex++] = pCell;
			}
		}
		DWORD nEnd = ::GetTickCount();
		TRACE1("CreateRowCell���� ����в��� ��ʱ%d \n\n", nEnd - nBegin);
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
	///////////////////////////////������///////////////////////////////////////////
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
	// ����������Ŀ��
	nColInfoIndex = ColNumToIndex(pos.x);
	if (nColInfoIndex == -1)
	{
		rect.SetRectEmpty();
		return FALSE;
	}
	rect.right = rect.left + m_ayColInfo.GetAt(nColInfoIndex).nWidth;
	//////////////////////////////����߶�////////////////////////////////////////////
	for (int i = 0; i < pos.y; i++)
		rect.top += m_nRowHeight;
	
	// ����������ĸ߶�
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
			// ������Ҫ�ػ������
			CRect rect;
			GetCellRect(nCellID, rect);
			// ˢ������Ϊ��������
			rect.InflateRect(rect.Width(),0,rect.Width(),0);
			// �淶������
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
	{// �����Сû�иı� ���ҿɼ����ж���Ϊ0 ����Ѿ��������� ����Ҫ���ظ������С
		return FALSE;
	}
	m_rcTable = m_rect;
	CalCulateSize();
	return TRUE;
}

void CDrawTableBase::CalCulateSize()
{
	// �������Ϊ0 ����Ҫ����
	if (m_rect.IsRectEmpty())
	{
		m_nShowColNum = m_nShowRowNum = 0;
		return;
	}
	CRect rc = m_rect;
	// ����б���� ��Ԥ�����������
	if (m_pTitle)
		rc = m_pTitle->SetRect(m_rect);

	// ���������һ��Ϊ0 ֱ�ӷ���
	if (m_nCol <= 0 || m_nRow <= 0)
		return;

	// ����д�ֱ������ ��Ԥ���������������� 
	CRect rcVScrollResidue(0,0,0,0);
	if (m_pVerScrollBar && m_pVerScrollBar->GetAutoSize())
	{
		rcVScrollResidue = m_pVerScrollBar->SetRect(rc);	
		m_rcTable = rcVScrollResidue ;
	}
	// �����ˮƽ������ ��Ԥ����ˮƽ������������
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
		int nVerScrollRange(0);  // ��ֱ������������Χ
		int nHorScrollRange(0);  // ˮƽ������������Χ
		if (m_wStyle & TABLE_STYLE_SCREEN)
		{// ��ͨ���

			// ���㵱ǰ�ɼ���
			if ( (m_wStyle & TABLE_HEIGHT_CHANGABLE) && (m_pVerScrollBar == NULL))
			{// �������и߿ɱ� ���ҹ�����������		
				m_nRowHeight = m_rcTable.Height() / m_nRow; // �и�Ϊ��ǰ����߶� ���� ������
				m_nShowRowNum = m_nRow; // ȫ��������ʾ
			}
			else
			{
				m_nShowRowNum = m_rcTable.Height() / m_nRowHeight;
				m_nShowRowNum = (m_nShowRowNum > m_nRow)?m_nRow:m_nShowRowNum;
			}
			// ���㵱ǰ�ɼ���
			if (m_wStyle & TABLE_WIDTH_CHANGABLE)
			{// �������п�ɱ� ���ǹ̶��е��п����޷��ı� ��Ȼʹ�ó�ʼֵ
				
				ColInfoStruct* pColInfo = NULL;
				int nTotalWidth = 0;
				for (int i = 0; i < m_nFixCol; i++)
				{
					pColInfo = &m_ayColInfo.GetAt(i);
					nTotalWidth += pColInfo->nWidth;

					if (nTotalWidth >= m_rcTable.Width()) // �ܿ�Ȳ��ܳ�������
					{
						m_nShowColNum = i;
						break;
					}
				}

				// �ǹ̶��е��п� ʣ�������ƽ������
				if (nTotalWidth < m_rcTable.Width())
				{
					int nWidth = (m_rcTable.Width()-nTotalWidth) / (m_nCol - m_nFixCol);
					for (int i = m_nFixCol; i < m_nCol - 1; i++)
					{
						pColInfo = &m_ayColInfo.GetAt(i);
						pColInfo->nWidth = nWidth;

						nTotalWidth += nWidth;
					}
					// ���һ�п���ǵ�ǰͼԪ��ȼ�ȥ֮ǰ�����еĿ��
					m_ayColInfo.GetAt(m_nCol-1).nWidth = m_rcTable.Width() - nTotalWidth; 
					m_nShowColNum = m_nCol;
				}
			}
			else
			{
				int nWidth(0);
				m_nShowColNum = m_nCol;
				// ��Ϊ�п�ɱ� ������㷨������������ʾһ������
				// ��Ҫ��ˮƽ���������ó� m_nCol - m_nFixCol - 1
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
				// ��ǰ����ʾ������ ������Ҳ����Ҫ��
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
		{// ��������η���� ��������п���붼һ�� �����п������һ����ʼֵ ��Ϊm_nColWidth�ͼ�¼��ʼ���

			m_nFixCol = 0; // �̶���һ��Ϊ0
			// ����
			m_nRow = m_ayCell.GetSize();
			m_nShowRowNum = m_rcTable.Height() / m_nRowHeight;
//			m_nRow = m_nShowRowNum;
			// ����
			if (m_nColWidth == 0)
				m_nColWidth = m_ayColInfo.GetAt(0).nWidth;
			int nWidth = m_nColWidth;
			m_nShowColNum = m_rcTable.Width() / nWidth;
			m_nCol = m_nShowColNum;
			// ���µ�����ǰ�ɼ��������������Լ��п�
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
				// ��ǰ����ʾ������ ������Ҳ����Ҫ��
				nVerScrollRange = 0;
			}
			else
			{
				nVerScrollRange = m_ayCell.GetSize() - (m_nShowRowNum-m_nFixRow)*m_nShowColNum - m_nFixRow;
			}
		}
		CRect rcTemp;
		if (m_pVerScrollBar)
		{// ���ݼ�������� ��������Χ ���¼������������ 
			if (m_pVerScrollBar->GetAutoSize())
			{
				rcTemp = m_pVerScrollBar->SetRect(rc, nVerScrollRange);
				// ������¼��������֮�� ����������ı� ����Ҫ���¼���
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


	// �������Ƶ�Ĭ��λ��
	if (m_pHorScrollBar)
		m_pHorScrollBar->SetBlockToDefaultPosition();
	if (m_pVerScrollBar)
		m_pVerScrollBar->SetBlockToDefaultPosition();
}

int CDrawTableBase::Draw( CDC* pDC )
{
	if (m_bVisable == FALSE || m_rect.IsRectEmpty())
	{// ��ǰͼԪ���ɼ�
		return FALSE;
	}

	// ������
	DrawTableBK(pDC);

	//////////////////////////////////////////////////////////////////////////
	// ����Ԫ��
	BOOL bRet = FALSE;
	for (int x = 0; x < m_nShowColNum+1 && x < m_nCol; x++)
	{
		for (int y = 0; y < m_nShowRowNum+1 && y < m_nRow; y++)
		{
			CPoint pos(x,y);
			if (IsCellRowFixed(pos) || IsCellColFixed(pos))
				bRet = DrawCell(pDC, pos);
			else
			{// ������ǹ̶�����Ҫ ��ʾ�ܱ�ʶ������
				if (m_bIsShow)
				{
					bRet = DrawCell(pDC, pos);
				}
			}
		}
	}
	//////////////////////////////////////////////////////////////////////////
	// ������ ����Ĭ���������һֱ��ʾ����
	if (m_pTitle)
	{
		m_pTitle->Draw(pDC);
	}

	//////////////////////////////////////////////////////////////////////////
	// �������ͱ�� �ܱ�ʶ������
	// ��������
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
	{// �ı��С
		DrawLine(m_posNow, pDC);
	}
	else if (m_wDrawOper & NEED_MOVE)
	{// �ƶ�
		int nCursor;
		int nLeft;
		int nCellID = GetCellID(m_posNow, nCursor, nLeft); // ��굱ǰ���ڵ�Ԫ��
		CPoint CellPos;
		if (nCellID != -1)
		{
			CellPos = IndexToPoint(nCellID);
			if (m_wSignal & NEED_MOVE)
			{// ��Ԫ�����ƶ� ������Ҫ���ߵ�λ��
				if (CellPos.x >= m_nFixCol)
				{// �ǹ̶���
					CString strText = m_ayCell[m_nCellID]->GetItemText(-1);
					DrawRect(m_posNow, strText, pDC);
					CRect CellRect;
					GetCellRect(nCellID, CellRect);
					// ���
					if ( (m_posNow.x - nLeft) < CellRect.Width() / 2)
						DrawLine(CPoint(CellRect.left, CellRect.top), pDC);
					// �ұ�
					else
						DrawLine(CPoint(CellRect.right, CellRect.top), pDC);
				}
		    }	
			else
			{// Item���ƶ� ������Ҫ���ߵ�λ��
				if (m_nCellID == nCellID)
				{// û���Ƴ���ǰ��Ԫ��
					CDrawCell* pCell = m_ayCell[nCellID];
					int nDiff;
					// ��ǰ����ƶ�����Item
					int nItemID = pCell->GetItemID(m_posNow, m_ayColInfo.GetAt(CellPos.x).nWidth, nLeft, nDiff); 
					if (nItemID != -1)
					{
						CString strText = m_ayCell[m_nCellID]->GetItemText(m_ayCell[m_nCellID]->m_nItemID);
						DrawRect(m_posNow, strText, pDC);
						// ��ȡItem����
						CRect CellRect;
						GetCellRect(nCellID, CellRect);
						CRect ItemRect = pCell->GetItemRect(CellRect, nItemID);
						// ���
						if ( nDiff < ItemRect.Width() / 2)
							DrawLine(CPoint(ItemRect.left, ItemRect.top), pDC);
						// �ұ�
						else
							DrawLine(CPoint(ItemRect.right, ItemRect.top), pDC);
					}
				}
			}
	    }
	}
	else if (m_wDrawOper & NEED_TIP)
	{// ��ʾTip
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
	// ��ȡ��ɫ����
	COLORREF clrFG = DrawTool::GetColor(TEXT_COLOR_TIPFG);
	COLORREF clrBK = DrawTool::GetColor(TEXT_COLOR_TIPBK);
	CFont* pFont = DrawTool::GetFont(TEXT_FONT_TIP);

	//////////////////////////////////////////////////////////////////////////
	// ��������Ҫ�ľ��δ�С
	CFont* pOldFont = pDC->SelectObject(pFont);
	CRect rcNeed(0,0,0,0);
	pDC->DrawText(strContent, &rcNeed, DT_CALCRECT); // ��ȡ������������Ҫ�Ĵ�С
	CSize sz(rcNeed.Width()+6, rcNeed.Height()+4);
	// �淶�ߴ�
	CPoint pos(m_posNow);
	// ƫ��λ��
	pos.x += OFFSET_CX; 
	pos.y += OFFSET_CY;
	//::ClientToScreen(m_pRegion->GetHostHandle(), &pos);
	
	// ��ȡ��ǰ��������С
	int nCXScreen = m_rcTable.right;//::GetSystemMetrics(SM_CXSCREEN);
	int nCYScreen = m_rcTable.bottom;//::GetSystemMetrics(SM_CYSCREEN);
	pos.x = pos.x < 0?0:pos.x;  pos.x = pos.x > (nCXScreen - sz.cx)?(nCXScreen-sz.cx):pos.x;
	pos.y = pos.y < 0?0:pos.y; 	pos.y = pos.y > (nCYScreen - sz.cy)?(nCYScreen-sz.cy):pos.y;
	//::ScreenToClient(m_pRegion->GetHostHandle(), &pos);

	// �������
	CRect rc(pos, sz);
	pDC->FillSolidRect(&rc, RGB(238,198,16));
	rc.DeflateRect(1,1,1,1);
	pDC->FillSolidRect(&rc, clrBK);

	COLORREF clrOld = pDC->SetTextColor(clrFG);
	int nOldBkMode = pDC->SetBkMode(TRANSPARENT);
	
	pDC->DrawText(strContent, strContent.GetLength(), &rc, DT_VCENTER);

	// ��ԭCDC
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
	int nDiffx = 0;  // ������߿������

	/////////////////////////////////��ɨ��///////////////////////////////////////
	// �ȴӹ̶��� Ϊ�˵õ���ȷ��ǰ������
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
	// ��ɨ��ʧ��
	if (nCellPosX == -1)
		return -1;

	int nHeight = m_rcTable.top;
	int nCellPosY = -1;
	int nDiffy = 0; // �����ϱ߿������
	//////////////////////////////////��ɨ��////////////////////////////////////////	
	// �ȴӹ̶���
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
	// ��ɨ��ʧ��
	if (nCellPosY == -1)
		return -1;

	nIndex = PointToIndex(CPoint(nCellPosX,nCellPosY));
	if (nIndex != -1)
	{
		CDrawCell* pCell = m_ayCell[nIndex];
		if (pCell)
		{
			if ( (nDiffx < pCell->m_logPen.lopnWidth.x + 3) && (pCell->m_dwStyle & CELL_RESIZABLEWE) )
			{// ��߿�
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
	// ��ԭ״̬
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
	int nDiffx = 0;  // ������߿������
	int nLeft(0);    // ��߿�ĺ�����
	int nCursor(0);  // �������ʽ
	int nSignal(0);  // ��Ҫ�����ı�ʶ
	
	/////////////////////////////////��ɨ��///////////////////////////////////////
	// �ȴӹ̶��� Ϊ�˵õ���ȷ��ǰ������
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
				nWidth = m_ayColInfo.GetAt(nColInfoIndex).nWidth;   // nWidth��󱣴�Ϊ������ڵ�Ԫ��Ŀ��
				break;
			}
		}
	}
	// ��ɨ��ʧ��
	if (nCellPosX == -1)
		return -1;

	int nHeight = m_rcTable.top;
	int nCellPosY = -1;
	int nDiffy = 0; // �����ϱ߿������
	//////////////////////////////////��ɨ��////////////////////////////////////////	
	// �ȴӹ̶���
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
	// ��ɨ��ʧ��
	if (nCellPosY == -1)
		return -1;

	nIndex = PointToIndex(CPoint(nCellPosX,nCellPosY));
	if (nIndex != -1)
	{
		CDrawCell* pCell = m_ayCell[nIndex];
		if (pCell)
		{// �ҵ�������ڵĵ�Ԫ��

			//////////////////////////////////////////////////////////////////////////
			// �ж��Ƿ�Ҫ��ʾTip
			if ( (pCell->m_dwStyle & CELL_TIP) )
			{
				m_nTipTimeID = ::SetTimer(m_pRegion->GetHostHandle(), (long)this+TIMER_TIP, DELAY_SHOWTIP_TIMESPAN, &CDrawTableBase::ShowTipProc);
			}
			//////////////////////////////////////////////////////////////////////////

			if ( (nDiffx < pCell->m_logPen.lopnWidth.x + 3) && (pCell->m_dwStyle & CELL_RESIZABLEWE) && nCellPosX > m_nFixCol)
			{// ��߿�
				nCursor = MOUSE_SIZEWE;
				nSignal = NEED_RESIZE;
			}
			else
			{// ��Ԫ���ڲ�
				nCursor = pCell->GetItemSignal(point, nWidth, nLeft);
				
				if (nCursor & MOUSE_SIZEWE)
				{// ��ԪС����Ա��ı��С
				}
				else if ( nCursor & MOUSE_HAND )
				{// ��ԪС�����ݿɱ�ѡ�� ��ǰֻ���Ը�������ʽչʾ����
					nSignal = NEED_TEXTHIGHLIGHT;
					SetRedrawTimes(2); // ����Ҫ�ػ����� һ�θ�����ʾ ��һ�θ���ȥ��
				}
				else 
				{
					if ( (pCell->m_dwStyle & CELL_MOVABLE) && (nCellPosX >= m_nFixCol))
					{// ��Ԫ�����ƶ�λ��
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
	// ��Ϊ�����¼��Ĵ��ڻ��߲˵����� �ᵼ����굥���¼�������ƶ��¼�֮�� ��굱ǰ�ĵ�Ԫ����ܷ����ı� 
	// �����ڻ�ȡһ����굱ǰ���ڵĵ�Ԫ�� �Լ����
	m_posNow = point;
	int nCursor(0);
	int nSignal(0);
	m_nCellID = GetCellID(point, &nCursor, NULL, &nSignal);
	m_wSignal = nSignal;
	SetMouseCursor(nCursor);

	//////////////////////////////////////////////////////////////////////////
	// �ȴ������������¼�
	if (m_pVerScrollBar)
	{// ������������� ���������
		if (m_pVerScrollBar->MouseDown(nFlags, point))
		{// ����������ڹ������� ��ֱ�ӷ���
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
	// �ٴ���������¼�
	// ����
	if (m_nCellID == -1)
	{
		return FALSE;
	}
	if ((m_wSignal & NEED_NOTHING) && (m_ayCell[m_nCellID]->m_wSignal & NEED_NOTHING) )
	{
		return FALSE;
	}
	//////////////////////////////////////////////////////////////////////////
	// �ж��Ƿ���ѡ����
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
	// �����ǲ������̵Ļ��� ���磺�ƶ����ı��С
	m_wDrawOper = m_wSignal | m_ayCell[m_nCellID]->m_wSignal;
	HWND hwnd = m_pRegion->GetHostHandle();

	CPoint prePos(-1,-1);
	int    nDiff = 0;

	//////////////////////////////////////////////////////////////////////////
	// �޶�����ƶ���Χ
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
				::ScreenToClient(hwnd, &m_posNow); // Ҫת��Ϊ��Ļ����

				nDiff = m_posNow.x - point.x;
				if(msg.wParam != MK_LBUTTON)
				{// ������û�а�ס ��ֱ���˳�
					bEixstLoop = TRUE;
					break;
				}
				CRect rectNeedRedraw = m_rcTable;
				if (m_wDrawOper & NEED_RESIZE)
				{// �ı��С
					if (prePos.x != -1)
					{// ������Ҫ�ػ������				
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
				{// �����ƶ�
					CRect rectNew;
					CRect rectOld;
					if (m_wSignal & NEED_MOVE)
					{// ��Ԫ���ƶ�
						rectNew = GetDrawRect(m_posNow,TRUE);
						rectOld = GetDrawRect(prePos, TRUE);
					}
					else
					{// Item�ƶ�
						rectNew = GetDrawRect(m_posNow,FALSE);
						rectOld = GetDrawRect(prePos, FALSE);
					}

					rectNeedRedraw = m_rcTable;
					if (prePos.x != -1)
					{// ������Ҫ�ػ������
						rectNeedRedraw.left = (rectNew.left<rectOld.left)?rectNew.left:rectOld.left;
						rectNeedRedraw.right = (rectNew.right>rectOld.right)?rectNew.right:rectOld.right;
					}
					else
					{
						rectNeedRedraw.left = rectNew.left;
						rectNeedRedraw.right = rectNew.right;
					}
					// Ҫ�������ߵ�����
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
				{// û���ƶ���
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
	{// ��Ҫ�ı��С
		if ( (m_wSignal & NEED_RESIZE) )
		{// ��Ԫ��֮��ı��С
			CPoint posRight = IndexToPoint(m_nCellID);
			ReSize(posRight.x-1, nDiff);
		}
		else
		{// ��Ԫ���� Item֮��ı��С
			CPoint posNowCell = IndexToPoint(m_nCellID);
			int nItemLeft = m_ayCell[m_nCellID]->m_nItemID - 1;
			int nItemRight = nItemLeft + 1;
			int nColInfoIndex = ColNumToIndex(posNowCell.x);

			// ͬ�еĵ�Ԫ��������Ĳ�ͬ����ͬ
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
	{// ��Ҫ����λ��
		int nCursor;
		int nLeft;
		int nCellID = GetCellID(m_posNow, nCursor, nLeft); // ��굱ǰ���ڵ�Ԫ��
		CPoint CellPos = IndexToPoint(nCellID);
		if (nCellID != -1)
		{
			CellPos = IndexToPoint(nCellID);
			int nColInfoIndex = ColNumToIndex(CellPos.x);
			if (m_wSignal & NEED_MOVE)
			{// ��Ԫ�����ƶ� 
				if (CellPos.x >= m_nFixCol)
				{// �ǹ̶���
					int nColInsert(-1); // ��Ҫ�������λ��
					// ���
					if ( (m_posNow.x - nLeft) < m_ayColInfo.GetAt(nColInfoIndex).nWidth / 2)
						nColInsert = CellPos.x;
					// �ұ�
					else
						nColInsert = CellPos.x + 1;

					CellPos = IndexToPoint(m_nCellID);
					MoveCol(CellPos.x, nColInsert);
				}
			}	
			else
			{// Item���ƶ� ������Ҫ���ߵ�λ��
				if (m_nCellID == nCellID)
				{// û���Ƴ���ǰ��Ԫ��
					CDrawCell* pCell = m_ayCell[nCellID];
					int nDiff;
					// ��ǰ����ƶ�����Item
					int nItemID = pCell->GetItemID(m_posNow, m_ayColInfo.GetAt(nColInfoIndex).nWidth, nLeft, nDiff); 
					if (nItemID != -1)
					{
						int nItemInsert(-1); // ��Ҫ�������λ��;
						// ��ȡItem����
						CRect CellRect;
						GetCellRect(nCellID, CellRect);
						CRect ItemRect = pCell->GetItemRect(CellRect, nItemID);

						// ���
						if ( nDiff < ItemRect.Width() / 2)
							nItemInsert = nItemID;
						// �ұ�
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
	// ��Ϊ���ڹ����¼� ���Ե�ǰ������ڵĵ�Ԫ����Ҫ���¼���һ��
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
	// ��̬�����Ըı� ���磺ѡ��
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
	//	TRACE1("��ǰ������Ϊ %d \n", nActiveRow);
	int nPreAcitveRow = nActiveRow;
	if (nChar == VK_UP)
		nActiveRow--;
	else if (nChar == VK_DOWN)
		nActiveRow++;
	
	if (nPreAcitveRow != nActiveRow)
	{
		//////////////////////////////////////////////////////////////////////////
		// �淶��������
		nActiveRow = nActiveRow < m_nFixRow?m_nFixRow:nActiveRow;
		nActiveRow = nActiveRow > m_nRow-m_nFixRow?m_nRow-m_nFixRow:nActiveRow;
		if (nPreAcitveRow == nActiveRow)
			return FALSE;

		if (nActiveRow - m_posMove.y < m_nFixRow)
		{
			Scroll(nActiveRow-m_nFixRow-GetMove().y, TRUE, FALSE);
			// 		// ��Ϊ������Ϲ�����һ�� ��ǰ�����к�֮ǰ�ļ����ж������ƶ�һ��
			// 		nActiveRow++;
			// 		nPreAcitveRow++;
		}
		else if (nActiveRow - m_posMove.y >= m_nShowRowNum)
		{
			Scroll(nActiveRow-m_nShowRowNum-GetMove().y+1, TRUE, FALSE);
			// 		// ��Ϊ������¹�����һ�� ��ǰ�����к�֮ǰ�ļ����ж������ƶ�һ��
			// 		nActiveRow--;
			// 		nPreAcitveRow--;
		}

		RemoveActiveRow(nPreAcitveRow);
		SetActiveRow(nActiveRow);
		//	TRACE1("����֮��ļ�����Ϊ %d \n", nActiveRow);
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
		{// ��Ԫ�����ƶ�
			rect = GetDrawRect(pos, TRUE);
		}
		else
		{// ��Ԫ���ڲ�Item�ƶ�
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
	{// ֻ����ͨ���ſ���ʵ��
		return;
	}
	if (nBegin < m_nFixCol || nBegin >= m_nCol || nEnd < m_nFixCol || nEnd > m_nCol)
	{// �������ݷǷ�
		return;
	}

	if (nBegin == nEnd || nBegin + 1 == nEnd)
	{// λ�ò��䲻��Ҫ�ƶ�
		return;
	}

	// ת���ɶ�Ӧ����ID
	int nColIndexBegin = ColNumToIndex(nBegin);
	int nColIndexEnd = ColNumToIndex(nEnd);

	// ��nBegin�е����е�Ԫ�񱸷�
	CDrawCellArray ayTemp;
	ayTemp.SetSize(m_nRow);
	int nCellID = nColIndexBegin * m_nRow;
	for (int i = 0; i < m_nRow; i++, nCellID++)
	{
		CDrawCell* pCell = m_ayCell[nCellID];
		ayTemp.SetAt(i, pCell);
	}

	if (nBegin < nEnd)
	{// ���Ҳ�����
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

		// ��Ӧ��������ҲҪ�����ı�

		ColInfoStruct colInfo;
		colInfo.Copy(&m_ayColInfo.GetAt(nColIndexBegin));
		m_ayColInfo.InsertAt(nColIndexEnd, colInfo);
		m_ayColInfo.RemoveAt(nColIndexBegin);
	}
	else
	{// ���������
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

		// ��Ӧ��������ҲҪ�����仯
		ColInfoStruct colInfo;
		colInfo.Copy(&m_ayColInfo.GetAt(nColIndexBegin));
		m_ayColInfo.InsertAt(nColIndexEnd, colInfo);
		m_ayColInfo.RemoveAt(nColIndexBegin + 1); // ��ΪnEnd���뵽����� ԭ����nBegin��������һ��λ��
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
{// ������ û��ִ�����
	return;
}

void CDrawTableBase::OnFixedRowClick()
{// ������ û��ִ�����
	return;
}

CString CDrawTableBase::GetTipText(int nCellID, int nItemID)
{// ������ û��ִ����� 
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

void CDrawTableBase::InValidate(CRect* pRect, BOOL bErase/* = TRUE*/) // ˢ������ ���ⲿ����
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

	// ������Χ
	if ( nDiff > nRange)
		nDiff = nRange;
	else if ( nDiff < nRange * (-1))
		nDiff = nRange * (-1);

	if (nDiff > 0)
	{// ����ƽ��	
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
		// ���˵�ͬʱҲ�Ǽ�����	
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
	{// �ȴӵ�һ�� ��ɨ��
		int nCellID = y;
		CDrawCell* pCell = m_ayCell[nCellID];
		if (pCell->m_dwStyle & CELL_BK_HIGHLIGHT)
		{// �ҵ���һ��������Ԫ ����ɨ��
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
		// ���˵�ͬʱҲ�Ǽ�����	
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
		{// �ȴӵ�һ�п�ʼ ��ɨ��
			int nCellID = x * m_nRow;
			CDrawCell* pCell = m_ayCell[nCellID];
			if (pCell->m_dwStyle & CELL_BK_HIGHLIGHT)
			{
				int y(0);
				for (; y < m_nRow; y++)
				{// ����ɨ��
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
	{// ���ƶ���

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
	{// ��������ֱ

		if (nPosition < 0)
			nPosition = 0;
		else if (nPosition > nRange)
			nPosition = nRange;
		
		if (m_posMove.y == nPosition)
			return FALSE;
		m_posMove.y = nPosition;
	}
	else
	{// ������ˮƽ
		
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