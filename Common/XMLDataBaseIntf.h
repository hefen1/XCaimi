/*******************************************************
  Դ��������:XMLDataBaseIntf.h
  �������Ȩ:�������ӹɷ����޹�˾
  ϵͳ����:  Ͷ��Ӯ�ҽ�������ն�V1.0
  ģ������:  ��������
  ����˵��:  XML�����ļ��������ݽӿڶ���
  ��    ��:  shenglq
  ��������:  20100926
*********************************************************/
#pragma once
typedef CString CXML;

struct INodeData 
{
	//�ͷ�
	virtual void Release() = 0;
	//��������������ֵ
	virtual CString GetValue(const CString sPropName) = 0;
	virtual CString Name() = 0;
	virtual CString Value() = 0;
	virtual CString ValueEx() = 0;
};
//////////////////////////////////////////////////////////////////////////
struct INodeTreeData : virtual public INodeData
{
	virtual int GetCount() = 0;
	virtual INodeData* GetItems(const int iIndex) = 0;
	virtual INodeData* GetItemsByName(const CString sName) = 0;
};

//////////////////////////////////////////////////////////////////////////

struct INodeDataList
{
	//�ͷ�
	virtual void Release() = 0;
	//ͳ�ƽӵ����
	virtual int GetCount() = 0;
	virtual INodeData* GetItems(const int iIndex) = 0;
	virtual INodeData* GetItemsByName(const CString sName) = 0;
	virtual INodeTreeData* GetNodeTreeData(const CString sName) = 0;
};