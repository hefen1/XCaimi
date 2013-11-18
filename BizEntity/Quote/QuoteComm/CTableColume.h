#include "gridctrl\gridcellbase.h"
#include "..\..\..\HelperLib\GeneralHelper.h"
#include "ConfigInterface.h"
#pragma once
// for test
//	���Ҳ���������
enum FindType	
{
	none = 0x00, 
	SI_CODE = 0x01,	//	���չ�Ʊ������в���
	SI_NAME = 0x02,	//	���չ�Ʊ���ƽ��в���
	SI_PYJC = 0x04, //	����ƴ����ƽ��в���
	SI_FOREX= 0x10	//  �Ҳ������벻��000001��Ʊ
};


/*�� ���ڹ������е�*/
class CMapWordToPtrEx;
class CTableColume : public GV_ITEM
{
public:
	//������	�з��
	enum { NewColMask = 0x1,NewColFormat = 0x2 };
	CTableColume();
	CTableColume(WORD dID);
	~CTableColume();

	//��������
	CTableColume* operator=(CTableColume* pCol);


	BYTE	m_bDelete;					//����,��ע�����������, ȡֵ:NewColMask, NewColFormat
	WORD	m_dID;						//��ID
	TableColProperty* m_pProperty;		//������

	int m_nTempWidth;					// ��ʱ���

protected:
	//�ֶδ����ϣ��
	static CMapStringToPtr 		 m_mapList;
	static COLORREF*			m_colTest;	//for testing

public:
	//��������, ɾ��mask, nFormat�ڴ�
	void Delete();

	//��ȡ��m_dID�е�����ָ��, ʧ�ܷ���NULL
	TableColProperty* GetColProperty();
	//ȡ��������, ���ʧ��, ����:"��"
	CString				  GetName();
	//ȡ���������Լ����ָ��
	UINT				  GetRowMask();
	UINT				  GetRowFormat();
	//�����������Լ����	bRow  - �Ƿ�Ϊ��
	void SetRowMask(UINT nMask,BOOL bRow = TRUE);
	void SetRowFormat(UINT nFormatRef,BOOL bRow = TRUE);

	//ȡ��,�����п��	nWidth - �ַ����� * 2
	void SetColWidth(int nWidth);
	int  GetColWidth();

	//�·���һ���ֶη����ϣ��, ����ָ��
	static CMapWordToPtrEx*		GetMapColName();
	//���ֶδ����ϣ����,�����ֶη����ϣ��, ʧ�ܷ���NULL, CString strName - KEY
	static CMapWordToPtrEx*		GetMap(CString strName);
	//���ֶδ����ϣ����,�����ֶη����ϣ��, ʧ�ܷ���NULL, WORD key - KEY
	static CMapWordToPtrEx*		GetMap(WORD key);
	//�����ֶ�ID, ȡ�����������ϣ���KEY
	static CString				GetMapName(WORD key);
	//������ϣ��
	static void					InitColName();
	/********************************************************************************
	* �������� : �ֶ����Զ������ļ�
	* �������� : CWinApp* pApp - δʹ��,
	*			 WORD dNameID - �ֶ�ID,
	*			 CString& strName - ����ֵ,
	*			 CString strSection - ������
	*  ����ֵ  : BOOL
	* �������� : 
	* ��    �� : 
	* ������� : 
	* �޸ļ�¼ : 
	*******************************************************************************/
	static BOOL					GetColName(CWinApp* pApp,WORD dNameID,CString& strName,CString strSection);

	/********************************************************************************
	* �������� : �ֶ�����д�����ļ�
	* �������� : WORD wKey - �ֶ�ID, 
	*			 TableColProperty* pCol - ����ָ�� , 
	*			 LPCTSTR lpszSection ������,
	*			 LPCTSTR lpszFileName �ļ�
	*  ����ֵ  : void
	* �������� : 
	* ��    �� : 
	* ������� : 
	* �޸ļ�¼ : 
	*******************************************************************************/
	static void					WriteColName(WORD wKey, TableColProperty* pCol = NULL, LPCTSTR lpszSection = NULL,LPCTSTR lpszFileName = NULL);

	//ɾ���ֶη����ϣ��, ��д�����ļ�	strName - ����KEY
	static void					DelMapColName(CString strName);
	//�����ֶ��Ƿ����	key - ID
	static BOOL			  	    Find(WORD key);
	//ͨ���ֶ�ID���Ҹ��ֶ����Ի�������, ʧ�ܷ���NULL,"��"
	static TableColProperty*	GetByKey(WORD key);
	static CString				GetName(WORD key);
	//ͨ���ֶ�IDȡ���������������ָ��
	static UINT				GetRowMask(WORD key);
	static UINT				GetRowFormat(WORD key);
	//ͨ���ֶ�IDȡ���������������ָ��
	static UINT				GetColMask(WORD key);
	static UINT				GetColFormat(WORD key);

	// �����ֶδ����ϣ��ָ��
	static CMapStringToPtr*		GetList() { return &m_mapList; }
	//����ֶη����ϣ��		CString strName - KEY ,CMapWordToPtrEx* pValue - �����ϣ��ָ��
	static void					AddToList(CString strName,CMapWordToPtrEx* pValue);
	//ɾ���ֶδ����ϣ�� �ֶ�����д�����ļ�
	static void					DelList();
	//ɾ��һ�ֶ� д�����ļ� δʹ��
	static void					DelColumn(WORD key);

	//ͨ���ֶ�IDȡ�������	
	static CString				GetNameByColID(WORD dID);		//����
	static COLORREF				GetBkClrByColID(WORD dID);		//����ɫ
	static COLORREF				GetFgClrByColID(WORD dID);		//ǰ��ɫ
	static UINT					GetStyleByColID(WORD dID);		//���
	static UINT					GetDataTypeByColID(WORD dID);	//��������
};

class CMapWordToPtrEx : public CMapWordToPtr
{
protected:
	/*���ͷ��	��16λ,�������	����:MPEXSϵ�� ��λ���	*
	*	8 - 15λ,����,	����:MPEXTϵ�� �Ƚ�ֵ
	*	0 - 7λ,����*/
	DWORD m_dwStyle;
public:
	CMapWordToPtrEx()
	{
		m_dwStyle = MAKELONG( 0, MPEXS_RWCONFIG);		
	}
public:
	//���ù�ϣ��
	void SetAt(WORD key, void* newValue,CWinApp* pApp,CString& strName,CString& strSection)
	{
		TableColProperty* pCol = (TableColProperty*)newValue;

		if(pCol != NULL)
		{
			if( IsStyle(MPEXS_RWCONFIG) && 
				CTableColume::GetColName(pApp,key,strName,strSection) )
			{
				CStringArray ayRet;
				if(CGeneralHelper::AnalyseStr(strName,";",ayRet) >= 7)
				{
					if( !IsStyle(MPEXS_COLNAME_NORWCFG) )
					{
						pCol->m_strName.Format("%s",ayRet[0]);
					}
					//pCol->m_nRowMask   = atol(ayRet[1]);
					//pCol->m_nRowFormat = atol(ayRet[2]);
					pCol->m_nColMask   = atol(ayRet[3]);
					pCol->m_nColFormat = atol(ayRet[4]);
					pCol->m_TextColor  = atol(ayRet[5]);
					pCol->m_cWidth     = (char)atol(ayRet[6]);
					pCol->m_cConfigWidth = pCol->m_cWidth;
				}
				else
				{
					pCol->m_strName.LoadString(atoi(pCol->m_strName));
				}
			}
			else
			{
				pCol->m_strName.LoadString(atoi(pCol->m_strName));
			}
		}
		CMapWordToPtr::SetAt(key,newValue);
	}
	void SetAt(WORD key, void* newValue)
	{
		void* pValue;
		if( CMapWordToPtr::Lookup(key,pValue) ) // �Ѿ�����
		{
			delete (CTableColume*)pValue;
			RemoveKey(key);
			return;
		}
		CMapWordToPtr::SetAt(key,newValue);
	}
public:
	//�жϷ��
	BOOL IsStyle(WORD wStyle){return wStyle & HIWORD(m_dwStyle);}
	//���÷��
	void ModifyStyle(WORD wAddStyle, WORD wRemoveStyle)
	{
		m_dwStyle |= MAKELONG(0, wAddStyle);
		m_dwStyle &= ~ MAKELONG(0, wRemoveStyle);
	}
public:
	//ȡ�û�����������
	BYTE GetType(){ return HIBYTE( LOWORD(m_dwStyle) );}
	void SetType(BYTE tType)
	{
		m_dwStyle &= 0xFFFF00FF;
		m_dwStyle |= MAKELONG(MAKEWORD(0, tType), 0);
	} 
public:
	//����
	BYTE GetReserve(){ return LOBYTE( LOWORD(m_dwStyle) );}
	void SetReserve(BYTE tType)
	{
		m_dwStyle &= 0xFFFFFF00;
		m_dwStyle |= MAKELONG(MAKEWORD(tType, 0), 0);
	} 

};
