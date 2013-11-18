#pragma once
#include "..\..\..\Common\QuoteComm\DrawBase.h"
#include "..\..\..\Common\QuoteComm\HSDrawBaseStruct.h"
#include ".\Graphics\HSLinear.h"
#include ".\CombComm\IndicatrixObj.h"

#define COMBANALYSISREPOR_ROW_HEIGHT       22  // 行高
class CCombAnalysisTech: public CDrawBase
{
public:
	CCombAnalysisTech(void);
	virtual~CCombAnalysisTech(void);

	virtual BOOL Create(IDrawBaseParam *pParam);
	virtual int Draw(CDC* pDC);
	virtual BOOL SetRect(CRect rc);
	virtual void OnLayoutNotify(UINT nMsgId,IDrawBaseParam* pParam);
	void RefreshDrawData();
	void SetLineName(CString strLineName) { m_strLineName = strLineName; }
	void SetIndicatrixObj(IIndicatrixObj* pObj) { m_pIndicatrixObj = pObj;}
protected:
	BOOL RecalcRect(CRect rcArea);
private:
	//绘图相关
	CRect                            m_rcGird;
	CHSLinear<float>*			m_pLinear;			         // 绘图类
	DWORD					    m_dwLinearStyle;        //线风格
	DrawPointInfo*				m_pPointPos;		        //指向绘制数据结构
	float				                m_fHoriCellWidth;		//横向单元格宽度

    //数据相关
	IIndicatrixObj*              m_pIndicatrixObj;
	CString                         m_strLineName;
	DrawIndicatrixData*       m_pDrawIndicatrixData;  //画线序列
	double                          m_dMaxValue;          //序列中的最大值
	double                          m_dMinValue;          //序列中的最小值
	int                                 m_nValidCount;        //有效数据位置
};
