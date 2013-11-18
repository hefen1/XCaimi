/*************************************************************
*	Copyright(c) 2010, 杭州恒生信息技术有限公司
*	All rights reserved.
*
*	文件名称：	RequestResponceDataManager.h
*	文件标识：
*	摘	  要：	请求(应答)列表管理类
*	
*	当前版本：
*	作	  者：	蒋国宏
*	完成日期：  2010-9-1
*
*	备	  注：
*	修改记录：
*	作	  者：
*	修改日期：
*	修改内容：
*
****************************************************************/

#pragma once 

#define HS_REQUEST_IGNORE_INTERVAL			1000		//请求忽略间隔时间 - 1秒

enum eDataSourceRequestResponceStauts				//数据请求、应答包处理状态
{
	eDataSourceRequestResponceStauts_jypacknofiy		= 0x0001,	//交易数据同步通知
	eDataSourceRequestResponceStauts_CommNotify         = 0x0002,   //通讯通知

	eDataSourceRequestResponceStauts_notprocess			= 0x0011,	//未分析
	eDataSourceRequestResponceStauts_processing			= 0x0012,	//正在分析
	eDataSourceRequestResponceStauts_sended				= 0x0013,	//已发送到服务器

	eDataSourceRequestResponceStauts_partreaddisk       = 0x0021,   //部分读取缓存
	eDataSourceRequestResponceStauts_allreaddisk        = 0x0022,   //全部读取缓存 
	 
	eDataSourceRequestResponceStauts_recvnotPorcess		= 0x0101,	//已接收未处理
	eDataSourceRequestResponceStauts_recvPorcess		= 0x0102,	//接收后处理中
	eDataSourceRequestResponceStauts_complete			= 0x0103,	//处理完成并返回给客户端

	eDataSourceRequestResponceStauts_loacal				= 0,		//本地处理数据

	eDataSourceRequestResponceStauts_masksend			= 0x00F0,	//掩码--发送数据
	eDataSourceRequestResponceStauts_maskrecv			= 0x0F00,	//掩码--接收数据

};

struct CDataSourceReqestResponceData
{
	UINT		m_uStatus;						//数据包处理状态
	long		m_lDataSinkID;					//回调
	UINT		m_uOption;						//数据包参数
	int			m_nTimeOut;						//超时时间
	int			m_nAskDataLen;					//请求/应答包原始长度
	char*		m_pRequestResponceData;			//请求/应答包内容指针
	CDataSourceReqestResponceData()
	{
		memset(this, 0, sizeof(CDataSourceReqestResponceData));
	}
};
