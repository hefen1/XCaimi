/*************************************************************
*	Copyright(c) 2010, ���ݺ�����Ϣ�������޹�˾
*	All rights reserved.
*
*	�ļ����ƣ�	BlockManger.h
*	�ļ���ʶ��
*	ժ	  Ҫ��	��������
*	
*	��ǰ�汾��
*	��	  �ߣ�	������
*	������ڣ�  2010-9-1
*
*	��	  ע�� ��ʱ�������治������2010-9-21������
*	�޸ļ�¼��
*	��	  �ߣ�
*	�޸����ڣ�
*	�޸����ݣ�
*
****************************************************************/

#pragma once

#define MAX_BLOCKNAME					32



class CBlockManger
{
protected:
	//���ػ����г����ݶ���
	std::list<BourseInfo*>				m_listBlockInfo;
	//�����г����ݻ�����
	CMutex								m_mutexBlock;	
public:
	//	���캯������������
	CBlockManger();
	~CBlockManger();

public:


};