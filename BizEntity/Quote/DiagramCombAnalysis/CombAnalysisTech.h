#pragma once
#include "..\..\..\Common\QuoteComm\DrawBase.h"
#include "..\..\..\Common\QuoteComm\HSDrawBaseStruct.h"
#include ".\Graphics\HSLinear.h"
#include ".\CombComm\IndicatrixObj.h"

#define COMBANALYSISREPOR_ROW_HEIGHT       22  // �и�
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
	//��ͼ���
	CRect                            m_rcGird;
	CHSLinear<float>*			m_pLinear;			         // ��ͼ��
	DWORD					    m_dwLinearStyle;        //�߷��
	DrawPointInfo*				m_pPointPos;		        //ָ��������ݽṹ
	float				                m_fHoriCellWidth;		//����Ԫ����

    //�������
	IIndicatrixObj*              m_pIndicatrixObj;
	CString                         m_strLineName;
	DrawIndicatrixData*       m_pDrawIndicatrixData;  //��������
	double                          m_dMaxValue;          //�����е����ֵ
	double                          m_dMinValue;          //�����е���Сֵ
	int                                 m_nValidCount;        //��Ч����λ��
};
