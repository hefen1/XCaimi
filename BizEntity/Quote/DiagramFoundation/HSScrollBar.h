/*************************************************************
*	Copyright(c) 2010, ���ݺ����ɷ����޹�˾
*	All rights reserved.
*
*	�ļ����ƣ�	CHsScrollBar.h
*	�ļ���ʶ��
*	ժ	  Ҫ��  ������
*	
*	��ǰ�汾��
*	��	  �ߣ�	����
*	������ڣ�  2011-01-05
*
*	��	  ע��
*	�޸ļ�¼��
*	��	  �ߣ�
*	�޸����ڣ�
*	�޸����ݣ�
*
****************************************************************/
#pragma once
#include "DiagramFoundationDef.h"
#include "DrawTableStruct.h"

#define HSSB_VERT         0X0001 // ��������ֱ
#define HSSB_HORZ         0X0002 // ������ˮƽ

#define HSSB_LEFT         0X0010 // ��������� ��Թ�������ֱ
#define HSSB_RIGHT        0X0020 // �������ұ�
#define HSSB_UP           0X0010 // �������ϱ� ��Թ�����ˮƽ
#define HSSB_DOWN         0X0020 // �������±�

// ����m_bBlockNeedBackΪTrue�Ż�ִ��
#define HSSB_BLOCK_ALWAYSTOP     0x0100  // ����Ĭ��λ���ڶ���
#define HSSB_BLOCK_ALWAYBOTTOM   0x0200  // ����Ĭ��λ���ڵײ�
#define HSSB_BLOCK_ALWAYLEFT     0x0100  // ����Ĭ��λ������
#define HSSB_BLOCK_ALWAYRIGHT    0x0200  // ����Ĭ��λ�����Ҳ�

#define HSSB_SIZE          10    // ���������(����������ֱ) �������߶�(��������ˮƽ)
#define HSSB_BLOCK_MINSIZE 4     // ������С�ĳߴ�

interface IScrollNotify 
{
	// ��������
	//	virtual void  OnScroll(int nDiff, BOOL bIsVert) = 0; �������ܲ��뵽SetPosition������
	// ����λ�ò���
	virtual BOOL  SetPosition(int nPosition, BOOL bIsVert, BOOL bIsRedraw = TRUE) = 0;
	// ˢ�²���
	virtual void  ScrollInvalidate(CRect* pRect, BOOL bErase = TRUE) = 0;
	// ��ȡ�������
	virtual HWND  GetHandle() = 0;
};


class HS_EXT_CLASS CHSScrollBar
{
public:
	CHSScrollBar(DWORD Style); 
	~CHSScrollBar(void);
	DWORD   GetStyle() {return m_nStyle;}  // ��ȡ���������
	void    SetStyle(DWORD nStyle){m_nStyle = nStyle;} // ���ù��������

	BOOL	ReadFile(); // �������ļ�
	void    Reset();    // ����һ�ι�������
    // ���������Ĵ�С(rc)�Լ����������(m_nStyle)��������Լ���λ�÷�������ʣ�µ�����
	CRect   SetRect(CRect rc);
	CRect   SetRect(CRect rc, int nRange);
	CRect   GetRect();

	// ���������ù������Ĵ�С ��������ֱ���ǹ������Ŀ�� ��������ˮƽ���ǹ������ĸ߶� û�и���ΪĬ��ֵ
	void    SetSize(int nSize);  
	
	void    SetAutoSize(BOOL bSize) {m_bAutoSize = bSize;}
	BOOL    GetAutoSize() {return m_bAutoSize;}

	BOOL    SetRange(int nRange);
	int     GetRange(){return m_nRange;}

	void    SetBlockToDefaultPosition();
	// ���ƹ�����
	void	Draw(CDC* pDC);
	void	DrawBorder(CDC* pDC);  // ���߿�
	void	DrawArrow(CDC* pDC);   // �����¼�ͷ�������Ҽ�ͷ
	void    DrawBlock(CDC* pDC);   // ������
	
	BOOL	MouseDown(UINT nFlags, CPoint point);  // ����¼�
	void    SetScrollNotify(IScrollNotify* pNotify) {m_iNotify = pNotify;}

	void    OnScroll(int nMoveDiff);   // ���ⲿ���� ��������������
	void    SetTop();                  // �������ö�
	void    SetBottom();               // �������õ�
protected:
	// ���㻬��λ�� ����С�ڻ�����С�ߴ� �����һ���ƶ������Ĵ���
	// ���ػ���Ĵ�С(ˮƽ���߶ȣ���ֱ�����)
	int     CalcBlock(int nMaxSize); 
	BOOL    SetBlockRect();        // �������ƶ���ǰλ��(����m_nPosition��ֵ�����ƻ���)

	enum HitPlace{NONE = 0, Up = 1, Down, Block, UpBlank, DownBlank};  // �����ѡ�еĵط��ֱ��Ƿ�Χ֮�⣬�ϼ�ͷ���¼�ͷ�����飬��������հף���������հ�
    void     MouseEvent(HitPlace eHit, CPoint point); // ��괦���¼�
	void     Scrolling(int nDiff);   // �����¼� ������ʾ(���ϣ�����) ������ʾ(���£�����)
	HitPlace GetHitPlace(CPoint pt); // �ж��������λ��
private:
	DWORD          m_nStyle;        // ���������

	int            m_nRange;        // �ⲿͼԪ���ƶ���Χ
	int            m_nScrollRange;  // �����������ƶ���Χ
	int            m_nPosition;     // ��ǰ����λ��
	int            m_nPrePosition;  // ֮ǰ�Ļ���λ��
	int            m_nMoveDistance; // ����һ���ƶ�������(Ĭ��Ϊ3������,��Ϊ����ƶ��¼�ÿ3������ϵͳ��׽һ��)
	int            m_nScrollDiff;   // ����һ���ƶ������Ĵ���
	HsLOGPEN	   m_logpen;        // ����

	CRect          m_rect;          // ��������������
	CRect          m_rcUpArrow;     // �������ϼ�ͷ(���ͷ)����
    CRect          m_rcDownArrow;   // �������¼�ͷ(�Ҽ�ͷ)����
	CRect          m_rcBlock;       // ��������������
	int            m_nSize;         // ��������С
	BOOL           m_bAutoSize;     // �Ƿ���Ҫ�Զ�������С

	IHsColor*      m_iColor;        // ��ɫ��д�ӿ� 
	COLORREF       m_clrBK;         // ����ɫ
	COLORREF       m_clrBlock;      // ������ɫ
	COLORREF       m_clrArrow;      // ��ͷ��ɫ

	double         m_dUserDiff;     // �������һ�δ�����ƶ���С������ʹ�������ƶ�һ�� �򽫸�С����¼����
	IScrollNotify* m_iNotify;       // �ص�ָ��  
};
