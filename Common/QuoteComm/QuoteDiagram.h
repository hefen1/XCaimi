/*******************************************************************************
 * Copyright (c)2003, 杭州恒生信息技术有限公司
 * All rights reserved.
 *
 * 文件名称：IDrawBase.h
 * 文件标识：画图图元基类
 * 摘    要：画图图元基类
 *
 * 当前版本：1.0
 * 作    者：
 * 完成日期：2010-11-25
 *

 *******************************************************************************/
#pragma once

#include "..\hsstructnew.h"
#include "..\hsds_comudata.h"
#include "..\HSWinner.h"

#define OBJ_DIAGRAM_TECHKLINE	_T("DiagramTechKLine.DiagramTechKLine")

//鼠标消息类型结构
struct hsRefMouseMsg
{
	char m_cType;		//类型

	enum Type 
	{
		none,
		RMove,RMoveDone,	     // 移动
		RSel,RSelDone,		     // 选中
		StartPlanMove,		     // 启动平移
		CancelPlanMove,		     // 取消平移
		DBClickPlanMove,	     // 双击平移
		VerPlanMove,		     // 纵向平移
		RightHoriPlanMove,	     // 右键横向平移
		RightHoriPlanMoveDone,	 // 右键横向平移结束
		RightVerPlanMove,	     // 右键纵向平移
		RightZoomIn,		     // 右键放大
		RightZoomRect,		     // 得到右键放大区域
		ClickInExplainRect,      // 点击在图形数字显示区域
		ClickInExplainDownRect,
		NextRefresh,		     // 下次需要刷新
		LButtonMove,		     // 左键按下移动(在非图形区域)
		RButtonMove,		     // 右键按下移动(在非图形区域)
		LButtonRightOperator     // 图表标题操作
	};

	hsRefMouseMsg()
	{
		m_cType = -1;
	}
	hsRefMouseMsg(Type cType)
	{
		m_cType = (char)cType;
	}
	//判断类型
	BOOL IsType(Type cType) { return (m_cType == cType); }

};


class IRequestCollector
{
public:
	virtual void AddRequest(char* pAskData,short lAskLen) = 0;
	virtual void Release() = 0;
};

//业务窗口参数接口
struct IDrawBaseParam
{
	virtual CString WINAPI GetStringParam(CString strParamName,CString strDefault = _T("")) = 0;
	virtual int WINAPI GetIntegerParam(CString strParamName,int nDefault = 0  ) = 0;
	virtual double WINAPI GetDoubleParam(CString strParamName,double nDefault = 0 ) = 0;
	virtual POSITION GetStartPosition() = 0;
	virtual void GetNextAssoc(POSITION& rNextPosition,CString& strParamName,CString& strValue) = 0;
};

struct IRegionNode;
struct IDrawBase
{
	/********************************************************************************
	* 函数功能 : 创建图元
	* 函数参数 : pParam： 参数接口指针
	*			 
	*  返回值  : 无
	* 调用依赖 :	
	* 作    者 :
	* 完成日期 :
	* 修改记录 :
	*******************************************************************************/
	virtual BOOL Create(IDrawBaseParam *pParam) = 0;
	virtual CString GetName() = 0;
	virtual void SetName(CString strName) = 0;
	/********************************************************************************
	* 函数功能 : 设置宿主位置
	* 函数参数 : 宿主指针
	*			 
	*  返回值  : 无
	* 调用依赖 :	
	* 作    者 :
	* 完成日期 :
	* 修改记录 :
	*******************************************************************************/
	virtual void SetRegionNode(IRegionNode* pNode) = 0;
	/********************************************************************************
	* 函数功能 : 设置图元位置
	* 函数参数 : 位置坐标
	*			 
	*  返回值  : 0 成功 其他为错误号
	* 调用依赖 :	
	* 作    者 :
	* 完成日期 :
	* 修改记录 :
	*******************************************************************************/
	virtual BOOL	SetRect(CRect rc) = 0;

	/********************************************************************************
	* 函数功能 : 获取图元位置
	* 函数参数 : 无
	*			 
	*  返回值  : 图元位置
	* 调用依赖 :	
	* 作    者 :
	* 完成日期 :
	* 修改记录 :
	*******************************************************************************/
	virtual CRect	GetRect() = 0;

	/********************************************************************************
	* 函数功能 : 设置图元可见属性
	* 函数参数 : 是否可见
	*			 
	*  返回值  : 无
	* 调用依赖 :	
	* 作    者 :
	* 完成日期 :
	* 修改记录 :
	*******************************************************************************/
	virtual void SetVisible(BOOL bVisible) = 0;

	/********************************************************************************
	* 函数功能 : 获取图元可见属性
	* 函数参数 : 无
	*			 
	*  返回值  : 是否可见
	* 调用依赖 :	
	* 作    者 :
	* 完成日期 :
	* 修改记录 :
	*******************************************************************************/
	virtual BOOL GetVisible() = 0;

	/********************************************************************************
	* 函数功能 : 更新图元代码信息
	* 函数参数 : 新代码信息
	*			 
	*  返回值  : 原代码信息
	* 调用依赖 :	
	* 作    者 :
	* 完成日期 :
	* 修改记录 :
	*******************************************************************************/
	virtual void UpdateCodeInfo(StockUserInfo* pCodeInfoNew) = 0;

	/********************************************************************************
	* 函数功能 : 获取图元代码信息
	* 函数参数 : 无
	*			 
	*  返回值  : 代码信息
	* 调用依赖 :	
	* 作    者 :
	* 完成日期 :
	* 修改记录 :
	*******************************************************************************/
	virtual StockUserInfo* GetCodeInfo() = 0;

	/********************************************************************************
	* 函数功能 : 获取图元绘制类型
	* 函数参数 : 无
	*			 
	*  返回值  : 绘制类型
	* 调用依赖 :	
	* 作    者 :
	* 完成日期 :
	* 修改记录 :
	*******************************************************************************/
	virtual short	GetDrawType() = 0;

	/********************************************************************************
	* 函数功能 : 消息响应 包括鼠标消息、键盘消息等
	* 函数参数 : 消息参数、hsRefMouseMsg* pData = NULL, 附带的操作类型
	*			 
	*  返回值  : 成功TRUE
	* 调用依赖 :	
	* 作    者 :
	* 完成日期 :
	* 修改记录 :
	* 备注	   : 右键消息需要传入窗口参数
	*******************************************************************************/
	virtual BOOL MouseDown(UINT nFlags, CPoint point, hsRefMouseMsg* pData = NULL) = 0;
	virtual BOOL MouseDBDown(UINT nFlags, CPoint point, hsRefMouseMsg* pData = NULL) = 0;
	virtual BOOL MouseUp(UINT nFlags, CPoint point, hsRefMouseMsg* pData = NULL) = 0;
	virtual BOOL MouseMove(UINT nFlags, CPoint point, hsRefMouseMsg* pData = NULL) = 0;
	virtual BOOL RButtonDown(UINT nFlags,CPoint point, HWND hWndParent, hsRefMouseMsg* pData = NULL) = 0;
	virtual BOOL RButtonUp(UINT nFlags,CPoint point, HWND hWndParent, hsRefMouseMsg* pData = NULL) = 0;
	virtual BOOL OnCursor(UINT nFlags, CPoint point) = 0;
	virtual int	 OnKey(UINT nChar, UINT nRepCnt, UINT nFlags) = 0;
	virtual int  OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) = 0;
	/********************************************************************************
	* 函数功能 : 键盘精灵通知
	* 函数参数 : 
	*			 
	*  返回值  : TRUE:对通知已做处理 FALSE:对通知未做处理
	*******************************************************************************/
	virtual BOOL OnKeyBoardNotify(WPARAM wParam, LPARAM lParam) = 0;
	// 菜单消息
	virtual long OnMenuMsg(WPARAM wParam, LPARAM lParam) = 0;

	/********************************************************************************
	* 函数功能 : 绘图
	* 函数参数 : 设备DC
	*			 
	*  返回值  : 0 成功 其他为错误号
	*******************************************************************************/
	virtual int Draw(CDC* pDC) = 0;


	/********************************************************************************
	* 函数功能 : 获取图元请求包
	* 函数参数 : nMask 掩码； lRetLen 包长度
	*			 
	*  返回值  : 请求包字符串
	* 备注	   :
	*******************************************************************************/
	virtual BOOL GetRequestData(IRequestCollector* pReqCollector,StockUserInfo* pStockInfo, BOOL bForce = FALSE) = 0;

	/********************************************************************************
	* 函数功能 : 收到数据更新
	* 函数参数 : 
	*			 
	*  返回值  : 0 成功 其他为错误号(如不需要处理，处理错误等等)
	*******************************************************************************/
	virtual int	OnUpdateData(DataHead* pData) = 0;

	virtual void OnActiveChange(BOOL bShow) = 0;
	virtual void OnLayoutNotify(UINT nMsgId,IDrawBaseParam* pParam) = 0;

	/********************************************************************************
	* 函数功能 : 删除对象
	* 函数参数 : 无
	*			 
	*  返回值  : 0 成功 其他为错误号
	*******************************************************************************/
	virtual int Delete() = 0; 
};

struct IRegionNode 
{
	/********************************************************************************
	* 函数功能 : 获取宿主窗口句柄
	* 函数参数 : 
	*			 
	*  返回值  : 
	*******************************************************************************/
	virtual HWND GetHostHandle() = 0;
	/********************************************************************************
	* 函数功能 : 激活图元
	* 函数参数 : strDiagramName 图元名称
	*			 
	*  返回值  : 
	*******************************************************************************/
	virtual BOOL ActiveDiagram(CString strDiagramName) = 0;

	/********************************************************************************
	* 函数功能 : 刷新区域
	* 函数参数 : 
	*			 
	*  返回值  : 
	*******************************************************************************/
	virtual BOOL Invalidate(CRect* pRC = NULL,BOOL bErase = TRUE) = 0;

	/********************************************************************************
	* 函数功能 : 设置光标
	* 函数参数 : 
	*			 
	*  返回值  : 
	*******************************************************************************/
	virtual BOOL SetCursor(LPCSTR lpCursorName) = 0;

	/********************************************************************************
	* 函数功能 : 获得请求收集器，获取后调用IRequestCollector::Release释放
	* 函数参数 : 
	*			 
	*  返回值  : 
	*******************************************************************************/
	virtual IRequestCollector* GetRequestCollector() = 0;

	/********************************************************************************
	* 函数功能 : 发送请求
	* 函数参数 : 
	*			 
	*  返回值  : 
	*******************************************************************************/
	virtual BOOL SendRequest(IRequestCollector* pReqCollector, BOOL bAutoClear = TRUE) = 0;

	/********************************************************************************
	* 函数功能 : 图层内部通知
	* 函数参数 : nMsgId:通知消息id,pParam:通知参数
	*			 
	*  返回值  : 
	*******************************************************************************/
	virtual void SendNotifyMsg(UINT nMsgId,IDrawBaseParam* pParam) = 0;
	/********************************************************************************
	* 函数功能 : 向业务
	* 函数参数 : nMsgId:通知消息id,pParam:通知参数
	*			 
	*  返回值  : 
	*******************************************************************************/
	virtual void SendLinkageMsg(UINT nMsg,IDrawBaseParam* pParam,
		CString strPanelName = _T(""),CString strLayout = _T(""),CString strOwner = PUBLIC_OWNER ) = 0;
};

struct IDiagramLayer
{
	
};