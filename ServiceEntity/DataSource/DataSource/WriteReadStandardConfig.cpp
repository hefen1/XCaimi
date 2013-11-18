
#include "stdafx.h"

#include "writereadstandardconfig.h"


#include <stdio.h>
#include <io.h>
#include <sys/stat.h>
#include "dos.h"

#include <math.h>

#include  <stdlib.h>
#include <direct.h>

//added by hlw 2002.0815

CString YlsGetProfileText(LPCTSTR lpAppName,        // section name
						  LPCTSTR lpKeyName,        // key name
						  LPCTSTR lpDefault,        // default string
						  LPCTSTR lpFileName)       // initialization file name
{
	CString str;
	int nLen = 128;
	int nRes;
	do
	{
		nLen *= 2;
		str.GetBufferSetLength(nLen);
		::GetPrivateProfileString(lpAppName,lpKeyName,lpDefault,str.GetBuffer(),nLen,lpFileName);
		nRes = lstrlen(str);
	} while (nRes == nLen-1);
	str.ReleaseBuffer();

	return str;
}

CWriteReadStandardConfig::CWriteReadStandardConfig(void)
{
}

CWriteReadStandardConfig::~CWriteReadStandardConfig(void)
{
}

BOOL CWriteReadStandardConfig::ReadConfigSectionEntry(const char* strSection, 
													  CStringArray& strArray,const char* strFileName)
{	
	strArray.RemoveAll();

	if(strFileName == NULL)
		return NULL;
	if(_access(strFileName, 0) != 0)
	{
		return FALSE;
	}
	struct _stat buf;
	memset(&buf, 0, sizeof(buf));
	_stat(strFileName, &buf);
	int nSize = (int)buf.st_size;
	if(nSize <= 0)
	{
		return FALSE;
	}
	char* strBuf = new char[nSize+1];
	memset(strBuf,0,nSize+1);

	::GetPrivateProfileString(strSection,NULL,"",strBuf,nSize,strFileName);
	char* p = strBuf;

	int nLen;
	int nTotal = 0;

	while(*p != '\0')
	{
		strArray.Add(p);
		nLen = strlen(p) + 1;
		
		nTotal += nLen;
		if( nTotal >= nSize )
			break;

		p += nLen;
	}  
	delete[] strBuf;

	return TRUE;
}

/******************************************************************
*	�������ܣ����ڴ��зֽ��,
*			  1,���pstrSectionΪNULL,�򷵻����ж�����,������pstrArray
*			  2,���pstrSection��ΪNULL��pstrEntryΪNULL,�򷵻ظö����м���,������pstrArray
*			  3,���pstrSection��ΪNULL��pstrEntry��ΪNULL,�򷵻ظöθü�ֵ,������pstrItem
*	����������	
*	�� �� ֵ��	BOOL					�������ɹ�����TRUE��ʧ��ʱ����FALSE
*	����������	
*	��    �ߣ�	���ӱ�
*	������ڣ�	2004-8-27
*	�޸�˵����	
*****************************************************************/
BOOL CWriteReadStandardConfig::ReadConfigSectionEntry(const char* pstrSection, 
													  const char* pstrEntry, 
													  CString* pstrItem, 
													  CStringArray* pstrArray, 
													  const char* pData, 
													  const int nLength)
{
	// DEBUG��ϲ�������, ���ʹ�õ��ò�����ȷ
	if(pData == NULL || nLength <= 0)
	{
		return FALSE;
	}

	if(pstrArray)
	{
		pstrArray->RemoveAll();
	}

	if(pstrItem)
	{
		pstrItem->Empty();
	}

	BOOL bResult = FALSE;
	TRY
	{
		CString strData = pData;
		strData.Trim();

		int nCurLen = strData.GetLength();

		CString	strLine;
		CString	strText;
		CString	strTmp;	

		int i		= 0; 
		int nFind	= -1;
		BOOL bFindSection = FALSE;
		while(i < nLength && i < nCurLen)
		{
			nFind	= strData.Find("\n", i);
			if(nFind == -1)
			{
				nFind	= strData.Find("\r", i);
				if(nFind == -1)
					nFind = nLength - 1;

				/*if(i + 1 < nLength && pData[i + 1] == '\n')
				{
				i ++;
				}*/
			}

			strLine = strData.Mid(i, nFind - i);
			i  += strLine.GetLength() + 1;

			strLine.Trim();
			strLine.TrimRight("\r\n");

			if(strLine.IsEmpty())
			{
				continue;
			}

			if(strLine[0] == ';' || strLine[0] == ',' ||
				strLine[0] == '#' || strLine[0] == '$') // ע����
			{
				continue;
			}
			else if(strLine[0] == '[') // ��
			{
				nFind = strLine.Find(']', 1);
				// �ݴ�ȱ']',����']'����ע���ַ�
				if(nFind == -1)
				{
					strText = strLine.Right(strLine.GetLength() - 1);
				}
				else
				{
					strText = strLine.Mid(1, nFind - 1);
				}

				strText.Trim();
				if(strText.IsEmpty())
				{
					continue;
				}

				if(pstrSection == NULL)
				{
					if(pstrArray)
					{
						pstrArray->Add(strText);
					}

					continue;
				}

				// ���Ҷ���
				if(strText.Compare( pstrSection ) != 0)
				{
					if(bFindSection == TRUE)
					{
						break;
					}

					continue; 					
				}

				bFindSection = TRUE;
			}
			else // ��
			{
				if(bFindSection == FALSE)
				{
					continue;
				}

				nFind = strLine.Find('=');
				if(nFind == -1)// û�з��� = ,���ǺϷ��ļ����
				{
					continue;
				}

				strText = strLine.Left(nFind );
				strText.Trim();
				if(strText.IsEmpty())
				{
					continue;
				}

				if(pstrArray)
				{
					pstrArray->Add(strText);
				}
				// ���Ҽ�
				if( pstrEntry && pstrItem && strText.Compare(pstrEntry) == 0)
				{
					strTmp = strLine.Right(strLine.GetLength() - nFind - 1);
					strTmp.Trim();

					if(!strTmp.IsEmpty())
						pstrItem->Format("%s", strTmp);					

					if(pstrArray == NULL)
						break;
				}				
			}
		}

		bResult = (pstrArray && pstrArray->GetSize() || 
			pstrItem && !pstrItem->IsEmpty());
	}
	CATCH(CFileException, pEx)
	{
		//HS_FILE_EXCEPTION(pEx);
		//pEx->ReportError(MB_ICONERROR|MB_OK);
	}
	AND_CATCH(CMemoryException, pEx)
	{
		//WRITE_LOG(__FILE__, __LINE__,"MemoryException");
		//pEx->ReportError(MB_ICONERROR|MB_OK);
	}	
	END_CATCH;
	return bResult;
}

BOOL CWriteReadStandardConfig::ReadConfigSectionString(const char* strSection, 
													   CStringArray& strArray,const char* strFileName)
{
	strArray.RemoveAll();

	if(strFileName == NULL)
		return NULL;;
	if(_access(strFileName, 0) != 0)
	{
		return FALSE;
	}
	struct _stat buf;
	memset(&buf, 0, sizeof(buf));
	_stat(strFileName, &buf);
	int nSize = (int)buf.st_size;
	if(nSize <= 0)
	{
		return FALSE;
	}
	char* strBuf = new char[nSize+1];
	memset(strBuf,0,nSize+1);

	::GetPrivateProfileString(strSection,NULL,"",strBuf,nSize,strFileName);
	char* p = strBuf;

	int nLen;
	int nTotal = 0;

	//char strItem1[128];
	CString strItem;
	while(*p != '\0')
	{
		//::GetPrivateProfileString(strSection,p,"",strItem1,127,strFileName);
		strItem = YlsGetProfileText(strSection,p,"",strFileName);

		strArray.Add(strItem);
		nLen = strlen(p) + 1;

		nTotal += nLen;
		if( nTotal >= nSize )
			break;

		p += nLen;
	}  
	delete[] strBuf;

	return TRUE;
}

BOOL CWriteReadStandardConfig::ReadConfigString(const char* strSection,const char* strEntry,
												CString& strItem,const char* strFileName)
{
	if(strFileName == NULL)
		return NULL;;
	if(_access(strFileName, 0) != 0)
	{
		return FALSE;
	}

	//char strBuf[128];   
	//::GetPrivateProfileString(strSection,strEntry,"",strBuf,127,strFileName);
	//if(strBuf[0] == '\0')
	//   return FALSE;
	//strItem = strBuf;

	strItem = YlsGetProfileText(strSection,strEntry,"",strFileName);
	if( strItem.IsEmpty() )
		return FALSE;

	return TRUE;
}

BOOL CWriteReadStandardConfig::ReadConfigInt(const char* strSection,const char* strEntry,
											 int& nValue,const char* strFileName)
{
	if(strFileName == NULL)
		return NULL;;
	if(_access(strFileName, 0) != 0)
	{
		return FALSE;
	}

	nValue = ::GetPrivateProfileInt(strSection,strEntry, -1, strFileName);
	if( nValue == -1 )
	{
		nValue = 0;
		return FALSE;
	}

	return TRUE;
}

BOOL CWriteReadStandardConfig::WriteConfigString(const char* strSection,const char* strEntry, 
												 const char* strItem,const char* strFileName)
{
	if(strFileName == NULL)
		return NULL;;

	::WritePrivateProfileString(strSection,strEntry, strItem, strFileName);
	return TRUE;
}

BOOL CWriteReadStandardConfig::ReadSectionInt(const char *strSection,const char* strEntry,
											  CUIntArray &UIntArray,const char* strFileName)
{
	UIntArray.RemoveAll();

	if(strFileName == NULL)
		return NULL;;
	char *token;
	char DivideStr[]=";";   

	CString paramString;

	if(!ReadConfigString(strSection, strEntry, paramString))
		return FALSE;
	token = strtok(paramString.GetBuffer(paramString.GetLength()),DivideStr);

	while( token != NULL )
	{
		UIntArray.Add(atoi(token));
		token = strtok( NULL, DivideStr );
	}
	return TRUE;
}	 

BOOL CWriteReadStandardConfig::WriteSectionInt(const char *strSection,const char* strEntry,
											   long lValue,const char* strFileName /*= NULL*/)
{
	if(strFileName == NULL)
		return NULL;;

	CString paramString;
	paramString.Format("%d", lValue);

	return WriteConfigString(strSection, strEntry,paramString,strFileName);
}

BOOL CWriteReadStandardConfig::WriteSectionInt(const char *strSection,const char* strEntry,
											   CUIntArray &UIntArray,const char* strFileName)
{	
	if(strFileName == NULL)
		return NULL;;
	CStringArray entryArray;
	char rBuffer[6];
	CString paramString = "";

	for(int i = 0; i < UIntArray.GetSize(); i++)
	{
		sprintf(rBuffer,"%d;",UIntArray[i]);
		paramString += rBuffer; 
	} 

	return WriteConfigString(strSection, strEntry,paramString,strFileName);
}  

BOOL CWriteReadStandardConfig::ChangeSection(const char *strSectionNew,const char* strSectionOld,
											 const char* strFileName /*= NULL*/)
{	

	if(strFileName == NULL)
		return FALSE;

	CStringArray strItemArray;

	CWriteReadStandardConfig::ReadConfigSectionEntry(strSectionOld,strItemArray,strFileName);

	CString strEntry,strItem;
	for(int i = 0;i < strItemArray.GetSize();i++)
	{
		strEntry = strItemArray.GetAt(i);

		CWriteReadStandardConfig::ReadConfigString(strSectionOld,strEntry,strItem,strFileName);

		CWriteReadStandardConfig::WriteConfigString(strSectionNew,strEntry,strItem,strFileName);

	}
	CWriteReadStandardConfig::WriteConfigString(strSectionOld,NULL,NULL,strFileName);

	return TRUE;
}

CString CWriteReadStandardConfig::GetContentsByHelpID(CString strFile,CString strHelpID)
{  
	CString strContents ;	//����

	if(!strHelpID.IsEmpty())
	{
		//�����ļ�ȫ·�����ļ���
		//CString  strFile = CHSFilePath::GetPath(CHSFilePath::Setting) + FILENAME_HS_HELPID_CFG;
		CFile fHelpFile;
		if(fHelpFile.Open(strFile, CFile::modeReadWrite|CFile::modeRead|
			CFile::shareDenyWrite|CFile::modeNoTruncate))
		{
			DWORD lFileLen = (DWORD)fHelpFile.GetLength();
			//�����ڴ�
			char * pData = new char[lFileLen + 1];

			memset(pData, 0, lFileLen + 1);
			try
			{
				//��ȡ����
				fHelpFile.Read(pData, lFileLen);
				//Ѱ�Ҷ���
				char* pSection = strstr(pData, strHelpID);
				if(pSection)
				{
					//Ѱ����һ������
					char* pNextSection = strtok(pSection, strHelpID.Left(1));
					if(pNextSection == NULL)
					{
						pNextSection = 	pSection;
					}
					strContents.Format("%s", pNextSection);
					//���ҽ���λ��
					int nLastChPos = strContents.Find(strHelpID.Right(1));
					if(nLastChPos != -1)
					{
						strContents = strContents.Right(strContents.GetLength() 
							- nLastChPos - 1);
					}
					else
					{
						strContents.Empty();
					}
					strContents.TrimLeft("\r\n");						
				}
				else/* û�иö�������д��ö�*/
				{
					strHelpID += "\r\n";
					strHelpID.Insert(0, "\r\n");
					fHelpFile.Write(strHelpID, strHelpID.GetLength());
				}				
			}
			catch(...)
			{
				ASSERT(FALSE);
			}
			fHelpFile.Close();
			//ɾ���ڴ�
			delete[] pData;			
		}
	}
	//����ȱʡ������
	if(strContents.IsEmpty())
	{
		//strContents = HSTXT_WHAT_THIS;
	}
	return strContents;
}


