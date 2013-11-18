/*******************************************************************************
 * Copyright(c) 2010, ���ݺ����ɷ����޹�˾
 * All rights reserved.
 *
 * �ļ����ƣ�InfoStruct.h
 * �ļ���ʶ����Ѷ��ؽṹ�ļ�
 * ժ    Ҫ����Ѷ��ؽṹ�ļ�
 *
 * ��ǰ�汾��1.0
 * ��    �ߣ�����
 * ������ڣ�2011��3��17��
 *
 *******************************************************************************/
#pragma once
#pragma	pack(push, 1)
//////////////////////////////////////////////////////////////////////////
//���Զ��ͷ��ڴ����ݽṹ
struct AutoFreeIndexData
{
	char* m_pData;	//����ָ��
	long  m_lSize;	//���ݳ���

	AutoFreeIndexData()
	{
		m_pData = NULL;
		m_lSize = 0;
	}
	~AutoFreeIndexData()
	{
		Free();
	}
	//����
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
	//�ͷ�
	void Free()
	{
		if ( m_pData != NULL )
		{
			delete[] m_pData;
			m_pData = NULL;
			m_lSize = 0;
		}
	}
	//�ж���Ч��
	BOOL IsValid() { return (m_pData != NULL && m_lSize > 0); }
};

//////////////////////////////////////////////////////////////////////////
// �����ļ���¼
struct InfoIndex
{
	char m_cType;    // Bit0�����ܱ�־ (0:������,1:����)
	// Bit1-Bit6������
	// Bit7����Ч��¼��־λ(0:��Ч��¼��1:��Ч��¼)
	// 0x00	��ͨ��Ϣ����
	// 0x01	������Ϣ����
	// 0x80	��Ч��¼
	char m_cGroup;   // Bit0-Bit3Ϊ��һ���ࣨ��֧��16�����ͣ�
	//����4BitΪ��Ѷ���̱������xx����Ϣ��
	// Bit0-3ֵ	��Ϣ����
	// 0x00	    ����������
	// 0x01	    �ƾ�����
	// 0x02	    ������ʷ��Ѷ (��˾���¼�)
	// 0x03	    ����ʵʱ��Ѷ
	// 0x04	    F10����
	// 0x05-0x0e	����
	// 0x0f	    ����	
	char m_cMarket;  // Bit0-Bit7Ϊ�г����ࣨ��֧��8���г�����
	// �ͱ���Ϣ��ص��г�Bit��Ϊ1��
	// ����Ϣ�������κ��г�ʱ��Bit0-Bit7ȫΪ0��
	// 0	�Ϻ�
	// 1	����
	// 2	��ҵ
	// 3	���
	// 4	̨��
	// 5	����
	// 6	����
	// 7	����
	long m_lDate;	    // ��*10000+��*100+��
	long m_lTime;      // ʱ*10000+��*100+��
	char m_cTitle[64]; // �ַ���������0x00��β
	// ������ʷ��Ѷ	��ظ�����ʷ��Ѷ���ݵı��⡣�������63���ֽڣ�������0x00��β��
	// ����ʵʱ��Ѷ	��ظ���ʵʱ��Ѷ���ݵı��⡣�������63���ֽڣ�������0x00��β��
	// ����F10����	��ΪF10����ҳ�ϵķ������ƣ����֧��4�����֡�
	// ��Ѷ���̵���ɫ��Ѷ	�����ɫ��Ѷ���ݵı��⡣�������63���ֽڣ�������0x00��β��

	char m_cFilePath[80]; // Դ�ļ�·�� // 130
	long m_lOffset;		  // ƫ��λ��
	long m_lLength;	      // ��Ϣ����
	char m_cCode[4][9];   // ��ش���
	// ���֧��4����ظ��ɴ��롣ÿ����ظ��ɴ���ռ9�ֽڣ����е�1�ֽ��Ǹø��������г�����(�����������)������8���ֽ�������д��ظ��ɴ��룬������0x00��

	char m_cReserved[55];  // ���� // 5
	char m_cSubType;	  // ��Ϣ�ӷ��� 0-8
	// 0 	��ʾ
	// 1	˵��
	// 2	��˾��̬
	// 3	��������
	// 4	�ֺ����
	// 5	ҵ������
	// 6 	�й�����
	// 7	��˾��������

	// 100 �ļ�
	// 101 �ļ�����

	char m_cChksum;		  // У��

	//��ȡ����,���ļ���ȡ
	char*		     ReadData(CString strPath = "",AutoFreeIndexData* pRefData = NULL);
	//����·��
	CString          GetPath();
	//ȡ����������
	CString		     GetSubName();
	//ȡ��������ɫ
	COLORREF		 GetSubColor();
	//���������,��������
	long			 RequestData(WPARAM wp,LPARAM lp);
	//��������
	BOOL Copy(InfoIndex* pRefData)
	{
		if( pRefData == NULL )
			return FALSE;

		memcpy(this,pRefData,InfoIndex::GetStructSize());
		return TRUE;
	}
	//���ؽṹ����
	static long GetStructSize() { return sizeof(InfoIndex); }

};

#pragma pack(pop)