#include "stdafx.h"
#include "DrawBase.h"

void CDrawBaseParam::AddParam(CString strParamName,CString strValue)
{
	m_valueMap.SetAt(strParamName,strValue);
}

CString CDrawBaseParam::GetStringParam(CString strParamName,CString strDefault /*= _T("")*/)
{
	CString strValue;
	if ( m_valueMap.Lookup(strParamName,strValue) )
		return strValue;
	else
		return strDefault;
}

int CDrawBaseParam::GetIntegerParam(CString strParamName,int nDefault /* = 0*/)
{
	CString strValue;
	if ( m_valueMap.Lookup(strParamName,strValue) )
	{
		if(!strValue.IsEmpty())
			return atoi(strValue);
	}
	return nDefault;
}

double CDrawBaseParam::GetDoubleParam(CString strParamName,double nDefault/* = 0*/)
{
	CString strValue;
	if ( m_valueMap.Lookup(strParamName,strValue) )
	{
		if(!strValue.IsEmpty())
			return atof(strValue);
	}
	return nDefault;
}

POSITION CDrawBaseParam::GetStartPosition()
{
	return m_valueMap.GetStartPosition();
}

void CDrawBaseParam::GetNextAssoc(POSITION& rNextPosition,CString& strParamName,CString& strValue)
{
	return m_valueMap.GetNextAssoc(rNextPosition,strParamName,strValue);
}

CDrawBase::CDrawBase()
{
	m_bVisable = TRUE;
	m_rect.SetRectEmpty();
	m_stkInfo.SetEmpty();
	m_pRegion = NULL;
	m_strName = _T("");
	m_nDrawType = DrawType_UnKnow;
}

void CDrawBase::SetRegionNode(IRegionNode* pNode)
{
	m_pRegion = pNode;
}

BOOL CDrawBase::SetRect(CRect rc)
{
	if (m_rect == rc)
	{
		return FALSE;
	}
	else
	{
		m_rect = rc;
		return TRUE;
	}
}

CRect	CDrawBase::GetRect()
{
	return m_rect;
}

void CDrawBase::UpdateCodeInfo(StockUserInfo* pCodeInfoNew)
{
	if(pCodeInfoNew)
		m_stkInfo.Copy(pCodeInfoNew);
	else
		m_stkInfo.SetEmpty();
}

StockUserInfo* CDrawBase::GetCodeInfo()
{
	return &m_stkInfo;
}
short	CDrawBase::GetDrawType()
{
	return 0;
}
//鼠标消息
BOOL CDrawBase::MouseDown(UINT nFlags, CPoint point, hsRefMouseMsg* pData /*= NULL*/)
{
	return TRUE;
}
BOOL CDrawBase::MouseDBDown(UINT nFlags, CPoint point, hsRefMouseMsg* pData /*= NULL*/)
{
	return TRUE;
}
BOOL CDrawBase::MouseUp(UINT nFlags, CPoint point, hsRefMouseMsg* pData /*= NULL*/)
{
	return TRUE;
}
BOOL CDrawBase::MouseMove(UINT nFlags, CPoint point, hsRefMouseMsg* pData/*= NULL*/)
{
	return TRUE;
}
BOOL CDrawBase::RButtonDown(UINT nFlags,CPoint point, HWND hWndParent, hsRefMouseMsg* pData/*= NULL*/)
{
	return TRUE;
}
BOOL CDrawBase::RButtonUp(UINT nFlags,CPoint point, HWND hWndParent, hsRefMouseMsg* pData/*= NULL*/)
{
	return TRUE;
}
BOOL CDrawBase::OnCursor(UINT nFlags, CPoint point)
{
	return TRUE;
}
int  CDrawBase::OnKey(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	return 0;
}
int CDrawBase::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	return 0;
}
BOOL CDrawBase::OnKeyBoardNotify(WPARAM wParam, LPARAM lParam)
{
	return FALSE;
}
// 菜单消息
long CDrawBase::OnMenuMsg(WPARAM wParam, LPARAM lParam)
{
	return 0;
}
int CDrawBase::Draw(CDC* pDC)
{
	return 0;
}
BOOL CDrawBase::GetRequestData(IRequestCollector* pReqCollector,StockUserInfo* pStockInfo, BOOL bForce)
{
	return TRUE;
}
int	CDrawBase::OnUpdateData(DataHead* pData)
{
	return TRUE;
}

void CDrawBase::OnActiveChange(BOOL bShow)
{

}

void CDrawBase::OnLayoutNotify(UINT nMsgId,IDrawBaseParam* pParam)
{

}

int CDrawBase::Delete()
{
	delete this;
	return 0;
}