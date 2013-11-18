#pragma once

//������֤��

//��ɫ���� 
#define Color_Diversity 20

class CCreateValidate
{
public:
	CCreateValidate(void);
	~CCreateValidate(void);

	/*
	* �������� ���ݴ�����ַ�������һ��λͼ
	*����1 hHwnd:   ��Ҫ��������ľ��
	*����2 strCode: ���ɵ�����
	*����3 nHeight: ͼƬ�߶�
	*����4 nWidth:  ͼƬ���
	*/
	HBITMAP CreateBMP(const HWND &hHwnd, const CString & strCode, const int nHeight, const int nWidth); 
protected:
	//�����������
	void FontDisturb(const int nHeight, const int nWidth);
	//���������ɫ
	COLORREF ColorDisturb();
private:
	//������������
	CString m_FontArray[23];
	//��������
	LOGFONT m_logFont;  
	//��ɫ
	COLORREF m_Color[10];

};
