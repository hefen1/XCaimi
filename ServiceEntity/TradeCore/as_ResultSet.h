/******************************************************************************
  Դ�ļ�����as_ResultSet.h
  �����Ȩ�����ݺ������ӹɷ����޹�˾
  ϵͳ���ƣ����ڻ�������ȫ����
  ����˵����������ӿ�
  ��    �ߣ�κ����
  ����ʱ�䣺20040223
  ��    ע��
  �� �� �ߣ�
  �޸�ʱ�䣺
  �޸ļ�飺
********************************************************************************/
#ifndef ResultSetH
#define ResultSetH
#include "hs_interface.h"


const char HSQL_DATATYPE_STRING                  = 'S';  //��������Ϊstring C��׼���ַ��� 
const char HSQL_DATATYPE_CHAR                    = 'C';  //��������Ϊchar
const char HSQL_DATATYPE_INT                     = 'I';  //��������Ϊint
const char HSQL_DATATYPE_DOUBLE                  = 'D';  //��������ΪDOUBLE
const char HSQL_DATATYPE_BINARY                  = 'R';  //������������� 

/**@name �ݻٶ���� */
//@{
#ifndef DESTROY_OBJ
#define DESTROY_OBJ(obj)  if (obj) obj = (IResultSet *)obj->destroy()
#endif

///������ӿ�
struct IResultSet
{

	///ȡ�ֶ���
    /**@return �����ֶ���.
      */
    virtual int FUNCTION_CALL_MODE getColCount()=0;

    ///ȡ�ֶ���
    /** @param column:�ֶ����(��0Ϊ����)
      * @return �����ֶ���  �±�Խ�� ����NULL
      */
    virtual const char * FUNCTION_CALL_MODE getColName(int column)=0;

    //ȡ�ֶ���������
    /** @param column:�ֶ����(��0Ϊ����)
      * @return:�����泣������;�±�Խ�� ����-1
      */
    virtual char FUNCTION_CALL_MODE getColType(int column)=0;


   ///ȡ�������ֶ�С��λ��
   /** @param column:�ֶ����(��0Ϊ����)
     * @return int �����ֶ�����С��λ�� �±�Խ�� ����-1
     */
   virtual int FUNCTION_CALL_MODE getColScale(int column)=0;
  
   //ȡ�ֶ����������ݵ������.
   /** @param column:�ֶ����(��0Ϊ����)
     * @return int �����ֶο�� �±�Խ�� ����-1
     */
   virtual int FUNCTION_CALL_MODE getColWidth(int column) = 0;

   ///ȡ�ֶ�����Ӧ���ֶ����
   /**@param columnName: �ֶ���
     *@return �����ֶ����. �����ڷ���-1
     */
   virtual int  FUNCTION_CALL_MODE findColIndex(const char * columnName)=0;

    //���ֶ����(��0Ϊ����)��ȡ�ֶ�ֵ(�ַ���)
    /**@param column:�ֶ����(��0Ϊ����)
      *@return �ַ������ֶ�ֵ,�±�Խ�緵��NULL
      */
    virtual const char *  FUNCTION_CALL_MODE getString(int column)=0;

    //���ֶ�����ȡ�ֶ�ֵ(�ַ���)
    /**@param columnName: �ֶ���
      *@return �ַ������ֶ�ֵ,�����ڷ���NULL
      */
    virtual const char *  FUNCTION_CALL_MODE getString(const char * columnName)=0;

    //���ֶ����(��0Ϊ����)��ȡ�ֶ�ֵ
    /**@param column:�ֶ����(��0Ϊ����)
      *@return �ַ����ֶ�ֵ,�±�Խ�緵��'\0'.
      */
     virtual char  FUNCTION_CALL_MODE  getChar(int column)=0;

    //���ֶ�����ȡ�ֶ�ֵ
    /**@param columnName: �ֶ���
     *@return �ַ����ֶ�ֵ,�����ڷ���'\0'
     */
    virtual char   FUNCTION_CALL_MODE getChar(const char * columnName)=0;

    //���ֶ���ţ�ȡ�ֶ�ֵ
    /**@param column:�ֶ����(��0Ϊ����)
      *@return double���ֶ�ֵ,�±�Խ�緵��0
      */
    virtual double  FUNCTION_CALL_MODE getDouble( int column)=0;
    
	///���ֶ�����ȡ�ֶ�ֵ
    /**@param columnName:�ֶ���
      *@return double���ֶ�ֵ,�����ڷ���0
      */
    virtual double  FUNCTION_CALL_MODE getDouble(const char * columnName)=0;

    ///���ֶ���ţ�ȡ�ֶ�ֵ
    /**@param column:�ֶ����(��0Ϊ����)
      *@return int���ֶ�ֵ,�±�Խ�緵��0
      */
    virtual int  FUNCTION_CALL_MODE getInt(int column)=0;

    ///���ֶ�����ȡ�ֶ�ֵ
    /**@param columnName:�ֶ���
      *@return int���ֶ�ֵ,�����ڷ���0
      */
    virtual int FUNCTION_CALL_MODE getInt(const char * columnName)=0;

	///���ֶ���Ż���ֶ�ֵ,����������
	/**@param column: �ֶ����(��0Ϊ����)
	*@param   lpRawLen: [out]���ݳ���
	*@return : �����׵�ַ
	*/
	virtual void *  FUNCTION_CALL_MODE getRaw(int column,int * lpRawLen) = 0;

	///���ֶ�����ȡ�ֶ�ֵ
	/**@param columnName:�ֶ���
	*@param   lpRawLen: [out]���ݳ���
	*@return : �����׵�ַ
	*/
	virtual void *  FUNCTION_CALL_MODE getRaw(const char * columnName,int * lpRawLen) = 0;


	///���һ��ȡ���ֶ�ֵ�Ƿ�ΪNULL
   /**@return 0 �ǣ� 1����
   */
    virtual int  FUNCTION_CALL_MODE wasNull()=0;

    ///ȡ��һ����¼
    virtual void  FUNCTION_CALL_MODE next()=0;

    ///�ж��Ƿ�Ϊ��β
    /**@return 1 �ǣ�0 ����;
      */
    virtual int  FUNCTION_CALL_MODE isEOF()=0;
   
	///�ж��Ƿ�Ϊ��
    /**@return 1 �ǣ�0 ����;
      */
    virtual int  FUNCTION_CALL_MODE isEmpty()=0;

    ///�ݻٽ����
    /**@return NULL
	 *�ͷŽ����������DESTROYOBJ(obj)����ʵ��
	 *���������ͷ����ڽ������������Դ��ִ�к�����ָ�뽫��ΪҰָ�롣
	 *���Ƽ���obj = obj->destroy(); ����ִ�к� obj �� NULL.
	*/
	virtual void * FUNCTION_CALL_MODE destroy() = 0;
};
#endif
