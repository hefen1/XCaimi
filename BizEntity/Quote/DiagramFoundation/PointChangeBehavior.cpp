#include "StdAfx.h"
#include "DrawTableBase.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


//////////////////////////////////////////////////////////////////////////
// 类似屏幕坐标的坐标变换
CPoint CScreenPointChangeBehavior::IndexToPoint( int nIndex )
{
	ASSERT(m_pTableBase);
	CPoint point(-1, -1);
	
	int nTotal = m_pTableBase->GetCellCount();
	if((nIndex < 0) || (nIndex >= nTotal))
		return point;

	// 计算行坐标
	point.x = nIndex/m_pTableBase->GetRowCount();
	if (!IsCellColFixed(nIndex))
	{// 如果是非固定列 要算上水平移动的距离
		point.x -= m_pTableBase->GetMove().x;

		if (point.x < m_pTableBase->GetFixColCount())
		{// 过滤掉 已经移出的单元格
			return CPoint(point.x, -1);
		}
	}

	// 计算列坐标
	point.y = nIndex%m_pTableBase->GetRowCount();
	if (!IsCellRowFixed(nIndex))
	{// 如果是非固定行 要算上垂直移动的距离
		point.y -= m_pTableBase->GetMove().y;

		if (point.y < m_pTableBase->GetFixRowCount())
		{// 过滤掉 已经移出的单元格
			return CPoint(-1, point.y);
		}
	}
	return point;	
}

int CScreenPointChangeBehavior::PointToIndex( CPoint pos )
{
	ASSERT(m_pTableBase);
	if((pos.x >= m_pTableBase->GetColCount()) || (pos.y >= m_pTableBase->GetRowCount()))
		return -1;

	// 计算移动之前的坐标
	CPoint newPos(pos);
	// 计算行坐标
	if (!IsCellColFixed(pos))
	{// 如果是非固定列 要算上水平移动的距离
		newPos.x += m_pTableBase->GetMove().x;
	}

	// 计算列坐标
	if (!IsCellRowFixed(pos))
	{// 如果是非固定行 要算上垂直移动的距离
		newPos.y += m_pTableBase->GetMove().y;
	}
	if((newPos.x >= m_pTableBase->GetColCount()) || (newPos.y >= m_pTableBase->GetRowCount()))
		return -1;

	int nTotal = m_pTableBase->GetCellCount();
	int nIndex = newPos.x * m_pTableBase->GetRowCount() + newPos.y;
	if(nIndex >= nTotal)
 		return -1;
	return nIndex;	
}

BOOL CScreenPointChangeBehavior::IsCellRowFixed(int nIndex)
{
	ASSERT(m_pTableBase);
	if (nIndex < 0 || nIndex >= m_pTableBase->GetCellCount())
	{
		return FALSE;
	}

	int nFixedID = 0;
	for (int i = 0; i < m_pTableBase->GetFixRowCount(); i++)
	{
		nFixedID = i;
		for (int j = 0; j < m_pTableBase->GetColCount(); j++)
		{
			if (nIndex == nFixedID)
				return TRUE;
			nFixedID += m_pTableBase->GetRowCount();
		}
	}
	return FALSE;
}
BOOL CScreenPointChangeBehavior::IsCellRowFixed(CPoint pos)
{
	if((pos.x < 0) || (pos.y < 0) || (pos.x >= m_pTableBase->GetColCount()) || (pos.y >= m_pTableBase->GetRowCount()))
		return FALSE;

	if ( pos.y < m_pTableBase->GetFixRowCount() )
		return TRUE;

	return FALSE;
}
BOOL CScreenPointChangeBehavior::IsCellColFixed(int nIndex)
{
	ASSERT(m_pTableBase);
	if (nIndex < 0 || nIndex >= m_pTableBase->GetCellCount())
	{
		return FALSE;
	}

	if (nIndex < m_pTableBase->GetFixColCount() * m_pTableBase->GetRowCount())
	{
		return TRUE;
	}
	return FALSE;
}
BOOL CScreenPointChangeBehavior::IsCellColFixed(CPoint pos)
{
	if((pos.x < 0) || (pos.y < 0) || (pos.x >= m_pTableBase->GetColCount()) || (pos.y >= m_pTableBase->GetRowCount()))
		return FALSE;

	if ( pos.x < m_pTableBase->GetFixColCount() )
		return TRUE;

	return FALSE;
}

//////////////////////////////////////////////////////////////////////////
// 蛇形滚动的坐标变换
CPoint CSnakePointChangeBehavior::IndexToPoint(int nIndex)
{
	ASSERT(m_pTableBase);
	if (nIndex < 0 || nIndex > m_pTableBase->GetCellCount()
		|| m_pTableBase->GetShowRowCount() <= m_pTableBase->GetFixRowCount()
		)
	{
		return CPoint(-1,-1);
	}

	CPoint pos;
	if (m_pTableBase->IsCellRowFixed(nIndex))
	{// 固定行
		pos.x = 0;
		pos.y = nIndex;
	}
	else
	{// 滚动行
		pos.x = (nIndex - m_pTableBase->GetMove().y) / (m_pTableBase->GetShowRowCount() - m_pTableBase->GetFixRowCount());
		pos.y = (nIndex - m_pTableBase->GetMove().y) % (m_pTableBase->GetShowRowCount() - m_pTableBase->GetFixColCount());
		if (pos.y < m_pTableBase->GetFixRowCount())
		{// 过滤掉 已经移出的单元格
			return CPoint(-1, pos.y);
		}
	}
	return pos;
}

int CSnakePointChangeBehavior::PointToIndex(CPoint pos)
{
	ASSERT(m_pTableBase);
	if (pos.x < 0 || pos.x >= m_pTableBase->GetColCount() || pos.y >= m_pTableBase->GetShowRowCount()
		|| m_pTableBase->GetShowRowCount() <= m_pTableBase->GetFixRowCount()
		)
	{
		return -1;
	}

	int nIndex(-1);
	if (m_pTableBase->IsCellRowFixed(pos))
	{// 固定行
		nIndex = pos.y;
	}
	else
	{// 滚动行
		nIndex = (pos.x * (m_pTableBase->GetShowRowCount() - m_pTableBase->GetFixRowCount()) + pos.y + m_pTableBase->GetMove().y);
	}
	if (nIndex >= m_pTableBase->GetCellCount())
	{
		nIndex = -1;
	}

	return nIndex;
}

BOOL CSnakePointChangeBehavior::IsCellRowFixed(int nIndex)
{
	ASSERT(m_pTableBase);

	if (nIndex < 0 || nIndex >= m_pTableBase->GetCellCount())
	{
		return FALSE;
	}

	if (nIndex < m_pTableBase->GetFixRowCount())
		return TRUE;

	return FALSE;
}
BOOL CSnakePointChangeBehavior::IsCellRowFixed(CPoint pos)
{
	if((pos.x < 0) || (pos.y < 0) || (pos.x >= m_pTableBase->GetColCount()) || (pos.y >= m_pTableBase->GetRowCount()))
		return FALSE;

	if (pos.x < m_pTableBase->GetFixRowCount())
		return TRUE;
	
	return FALSE;
}
BOOL CSnakePointChangeBehavior::IsCellColFixed(int nIndex)
{// 蛇形滚动没有固定列
	return FALSE;
}
BOOL CSnakePointChangeBehavior::IsCellColFixed(CPoint pos)
{// 蛇形滚动没有固定列
	return FALSE;
}