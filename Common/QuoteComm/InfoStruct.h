/*******************************************************************************
 * Copyright(c) 2010, 杭州恒生股份有限公司
 * All rights reserved.
 *
 * 文件名称：InfoStruct.h
 * 文件标识：资讯相关结构文件
 * 摘    要：资讯相关结构文件
 *
 * 当前版本：1.0
 * 作    者：王超
 * 完成日期：2011年3月17日
 *
 *******************************************************************************/
#pragma once
#pragma	pack(push, 1)
//////////////////////////////////////////////////////////////////////////
//可自动释放内存数据结构
struct AutoFreeIndexData
{
	char* m_pData;	//数据指针
	long  m_lSize;	//数据长度

	AutoFreeIndexData()
	{
		m_pData = NULL;
		m_lSize = 0;
	}
	~AutoFreeIndexData()
	{
		Free();
	}
	//分配
	void Alloc( int nCurSize  )
	{		
		if( nCurSize <= 0 )
		{
			Free();
			return;
		}		
		if( m_lSize != nCurSize )
		{
			Free();
			m_pData = new char[nCurSize];
			m_lSize = nCurSize;
		}
		memset( m_pData,0,m_lSize);
	}
	//释放
	void Free()
	{
		if ( m_pData != NULL )
		{
			delete[] m_pData;
			m_pData = NULL;
			m_lSize = 0;
		}
	}
	//判断有效性
	BOOL IsValid() { return (m_pData != NULL && m_lSize > 0); }
};

//////////////////////////////////////////////////////////////////////////
// 索引文件记录
struct InfoIndex
{
	char m_cType;    // Bit0：加密标志 (0:不加密,1:加密)
	// Bit1-Bit6：保留
	// Bit7：无效记录标志位(0:有效记录，1:无效记录)
	// 0x00	普通信息索引
	// 0x01	加密信息索引
	// 0x80	无效记录
	char m_cGroup;   // Bit0-Bit3为第一大类（共支持16种类型）
	//（此4Bit为资讯厂商必须告诉xx的信息）
	// Bit0-3值	信息类型
	// 0x00	    交易所公告
	// 0x01	    财经新闻
	// 0x02	    个股历史资讯 (公司大事记)
	// 0x03	    个股实时资讯
	// 0x04	    F10资料
	// 0x05-0x0e	保留
	// 0x0f	    其它	
	char m_cMarket;  // Bit0-Bit7为市场分类（共支持8个市场），
	// 和本信息相关的市场Bit置为1，
	// 当信息不属于任何市场时，Bit0-Bit7全为0。
	// 0	上海
	// 1	深圳
	// 2	创业
	// 3	香港
	// 4	台湾
	// 5	美国
	// 6	保留
	// 7	其它
	long m_lDate;	    // 年*10000+月*100+日
	long m_lTime;      // 时*10000+分*100+秒
	char m_cTitle[64]; // 字符串必须用0x00结尾
	// 个股历史资讯	相关个股历史资讯内容的标题。内容最多63个字节，必须用0x00结尾。
	// 个股实时资讯	相关个股实时资讯内容的标题。内容最多63个字节，必须用0x00结尾。
	// 个股F10资料	即为F10资料页上的分类名称，最多支持4个汉字。
	// 资讯厂商的特色资讯	相关特色资讯内容的标题。内容最多63个字节，必须用0x00结尾。

	char m_cFilePath[80]; // 源文件路径 // 130
	long m_lOffset;		  // 偏移位置
	long m_lLength;	      // 信息长度
	char m_cCode[4][9];   // 相关代码
	// 最多支持4个相关个股代码。每个相关个股代码占9字节，其中第1字节是该个股所在市场种类(编码见附表三)，其它8个字节用于填写相关个股代码，不足填0x00。

	char m_cReserved[55];  // 保留 // 5
	char m_cSubType;	  // 信息子分类 0-8
	// 0 	提示
	// 1	说明
	// 2	公司动态
	// 3	分析点评
	// 4	分红配股
	// 5	业绩报告
	// 6 	招股上市
	// 7	公司其它公告

	// 100 文件
	// 101 文件索引

	char m_cChksum;		  // 校验

	//读取数据,从文件读取
	char*		     ReadData(CString strPath = "",AutoFreeIndexData* pRefData = NULL);
	//返回路径
	CString          GetPath();
	//取得索引名称
	CString		     GetSubName();
	//取得索引颜色
	COLORREF		 GetSubColor();
	//生成请求包,发送请求
	long			 RequestData(WPARAM wp,LPARAM lp);
	//复制数据
	BOOL Copy(InfoIndex* pRefData)
	{
		if( pRefData == NULL )
			return FALSE;

		memcpy(this,pRefData,InfoIndex::GetStructSize());
		return TRUE;
	}
	//返回结构长度
	static long GetStructSize() { return sizeof(InfoIndex); }

};

#pragma pack(pop)