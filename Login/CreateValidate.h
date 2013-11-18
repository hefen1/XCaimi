#pragma once

//生成验证码

//颜色差异 
#define Color_Diversity 20

class CCreateValidate
{
public:
	CCreateValidate(void);
	~CCreateValidate(void);

	/*
	* 函数功能 根据传入的字符串生成一副位图
	*参数1 hHwnd:   需要创建窗体的句柄
	*参数2 strCode: 生成的文字
	*参数3 nHeight: 图片高度
	*参数4 nWidth:  图片宽度
	*/
	HBITMAP CreateBMP(const HWND &hHwnd, const CString & strCode, const int nHeight, const int nWidth); 
protected:
	//生成随机字体
	void FontDisturb(const int nHeight, const int nWidth);
	//生成随机颜色
	COLORREF ColorDisturb();
private:
	//字体名称数组
	CString m_FontArray[23];
	//字体属性
	LOGFONT m_logFont;  
	//颜色
	COLORREF m_Color[10];

};
