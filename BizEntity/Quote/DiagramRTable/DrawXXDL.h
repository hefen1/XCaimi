/*************************************************************
*	Copyright(c) 2010, 杭州恒生股份有限公司
*	All rights reserved.
*
*	文件名称：	CDrawXXDL.h
*	文件标识：
*	摘	  要：  信息地雷图元
*	
*	当前版本：
*	作	  者：	王超
*	完成日期：  2011-3-21
*
*	备	  注：
*	修改记录：
*	作	  者：
*	修改日期：
*	修改内容：
*
****************************************************************/
#pragma once
#include "DrawTableBase.h"
#include "QuoteComm\InfoStruct.h"
#include "DHtmlShowDlg.h"

#define XXDL_ROW_HEIGHT   20   // 行高
class CDrawXXDL :
	public CDrawTableBase
{
public:
	CDrawXXDL(IHsColor* pColor, IHsFont* pFont, IHsInfo* pInfo);
	~CDrawXXDL(void);

	void    CreateSelf();
	void    DelAllInfoIndex();           // 删除所有数据信息
	CString GetTitleString(InfoIndex* pInfoIndex); // 从信息地雷中提取标题文本
	void    RequestText(int nAyIndex);      // 获取信息地雷资讯内容
//interface
	virtual BOOL    DrawSelf(CDC* pDC, CPoint pos, int nItemID, CRect& rcItem);	
	virtual	BOOL	GetRequestData(IRequestCollector* pReqCollector,StockUserInfo* pStockInfo, BOOL bForce = FALSE);
	virtual int		OnUpdateData(DataHead* pData);
	virtual BOOL    MouseDBDown(UINT nFlags, CPoint point, hsRefMouseMsg* pData = NULL);
private:
	IHsColor*  m_pColor;         // 颜色接口指针
	IHsFont*   m_pFont;          // 字体接口指针
	IHsInfo*   m_pInfo;          // 资讯接口指针
 
	CString    m_strInfoPath;    // 咨询路径
	int        m_nActiveID;      // 双击选中的单元格
	long       m_lTextIndexCount;// 信息地雷索引数据数量
	BOOL       m_bRequst;        // 是否有发送请求

	CArray<InfoIndex*>  m_ayInfoIndex; // 咨询索引对象
	CDHtmlShowDlg*      m_pDlgShowHtml;// html对话框
};
