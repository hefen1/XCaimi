#pragma once

#include "ConfigInterface.h"
#include "WinnerApplication.h"

//����ƹ������
enum { ScrollUp=1,ScrollDown,ScrollLeft,ScrollRight,ScrollSpeedUp };

// ͼƬ��list�е�λ��
#define BMP_TEXT_INDEX      0 
#define BMP_CAPTION_INDEX   1
#define BMP_TOOL_INDEX      2

// CScrollNewsBar �Ի���
class CScrollNewsBar : public CDialog
{
	DECLARE_DYNAMIC(CScrollNewsBar)

public:
	CScrollNewsBar(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CScrollNewsBar();
	
// �Ի�������
	enum { IDD = IDD_SCROLLNEWSBAR };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	//////////////////////�Ҽ��˵�/////////////////////// 
	afx_msg void OnFloat();    //������ʾ
	afx_msg void OnMultiline();//������ʾ
	afx_msg void OnSetting();  //�߼�����
	//����¼�
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	int MouseDown(UINT nFlags, CPoint point); //�Զ�������¼�

	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();

//
protected:
	BOOL ReadCfg();      // �������ļ�
	void WriteCfg();     // д�����ļ�
	void SetSize();      // ���û�������

	void DrawMyText( CDC* pDC);   // ���ƹ�������
	void DrawMyTool( CDC* pDC);

	void InitRect(CDC* pDC);      // ��ʼ����ʾ����
	void CalculateRect(CDC* pDC); // �����������

	BOOL HitTest(CPoint point);       // �ж����λ���Ƿ��ڻ�����������
	BOOL HitCaption(CPoint point);    // �ж����λ���Ƿ��ڱ�����������
	BOOL HitTool(CPoint point);       // �ж����λ���Ƿ��ڹ�����������
	void Scrolling( int param1 );
public:
	void DrawMyCaption( CDC* MemDC);//���Ʊ���
public:
	//�����λ������
	BOOL  bScrollFixed;       //������Ƿ�̶���״̬����
	BOOL  bScrollMultiLine;   //������Ƿ������ʾ
	int   m_nShowScrollAlone; //�������ʾ�ķ�ʽ 
    //���������Ĳ���
	CRect m_rectCaption;      //����Ʊ������
	CRect m_rectText;         //����ƹ�������
	CRect m_rectTool;         //����ƹ���������

	//״̬��ָ��
	CWnd* m_pStatuBar;
//˽�б���
private:
	CMenu menu;
	CPoint lastPoint;  //��¼����϶�ǰ��λ��

	CStringArray m_sTitleArray;    //��¼�������Ҫ��ʾ���ݵı���
	CStringArray m_sContentArray;  //��¼�������Ҫ��ʾ���ݵ�����
	CStringArray m_sUrlArray;      //��¼�������Ҫ��ʾ���ݵ�����

	CArray<CRect> m_showDataCRectArray;// ��¼��ʾ���ݵ�λ��
	int  m_nShowIndex;                 // ��ǰ��ʾ����ʼ��Ϣ���±�
	BOOL m_bIsInitRect;                // �Ƿ���й����ƾ��εĳ�ʼ��

	COLORREF m_clrBk;           // ����������ɫ
	COLORREF m_clrText;         // ����������ɫ
	int      m_nSpeed;          // �����ٶ�
	CFont*   m_pFontText; // �ı�����

	CImageList   m_lstImage; // ͼƬ��Դ
	int m_nIsUpDownRate;    // ����ǵ����Ƿ���ʾ
	int m_nIsTotal;         // ����ܶ��Ƿ���ʾ
	int m_nIsShowNewPirce;  // ��ǳɽ����Ƿ���ʾ
	int m_nIsUpDown;        // ����ǵ��Ƿ���ʾ

	int      m_nClickWay;   //��굥��ѡ�з�ʽ 0-���� 1-˫��
	int	     m_nHorizonScrollWay;  //��¼ˮƽ��������
	int      m_nVerticalScrollWay; //��¼��ֱ��������

	long     m_nTimeID;     //����ʱ��
	long     m_nTimeIDMouse;//����ж�ʱ��

	IHsColor*      m_pColorCfg;    // ��ɫ��д�ӿ�
	IHsFont*       m_pFontCfg;     // �����д�ӿ�
	IHsScrollNews* m_pScrollCfg;   // ��������ö�д�ӿ�
};


