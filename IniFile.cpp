/*******************************************************************************
*   �ļ����� : IniFile.cpp
*   ��    �� : ��ȫ��
*   ����ʱ�� : 2015/10/8   16:09
*   �ļ����� : 
*   ��Ȩ���� : Copyright (C) 2014-2015 ���ϰ���Ƽ����޹�˾
*   �޸���ʷ : 2015/10/8   1.00 ��ʼ�汾
*******************************************************************************/

#include "IniFile.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <iostream>
using namespace std;

/*****************************************************************************
��������  :	CIniFile
��������  : ���캯��
�������  : strIniFilePath,ini�ļ���·��
�������  : N/A
�������  : N/A
����ֵ    : N/A
* �޸�����          �汾��       �޸���	      �޸�����
* -----------------------------------------------------
* 2012/05/21	     V1.0	     ��ȫ��	       ����
*****************************************************************************/
CIniFile::CIniFile(const string& strIniFilePath)
:m_strIniFilePath(strIniFilePath)
{

}

/*****************************************************************************
��������  :	~CIniFile
��������  : ��������
�������  : N/A
�������  : N/A
����ֵ    : N/A
��ע      :	N/A
* �޸�����          �汾��       �޸���	      �޸�����
* -----------------------------------------------------
* 2012/05/21	     V1.0	     ��ȫ��	       ����
*****************************************************************************/
CIniFile::~CIniFile(void)
{

}

/*****************************************************************************
��������  :	ReadString
��������  : ��ȡһ���ַ���ֵ
�������  : strSection���ֶ�������[]���������
�������  : strKey����������=ǰ�������
�������  : strResult�����ص�ȱʡ�ַ���
�������  : N/A
����ֵ    : ���ض�ȡ������ֵ�����section,key�������򷵻�strDefault      
��ע     : N/A
* �޸�����          �汾��       �޸���	      �޸�����
* -----------------------------------------------------
* 2012/05/21	     V1.0	     ��ȫ��	       ����
*****************************************************************************/
string CIniFile::ReadString(const string& strSection, const string& strKey, const string& strDefault /*= ""*/)
{
    char chResult[256] = {0};
    (void)GetPrivateProfileString(strSection.c_str()
                                  , strKey.c_str()
                                  , strDefault.c_str()
                                  ,chResult
                                  ,256
                                  , m_strIniFilePath.c_str());
    return (string)chResult;
}

/*****************************************************************************
��������  :	ReadInt
��������  : ��ȡһ������ֵ
�������  : strSection���ֶ�������[]���������
�������  : strKey����������=ǰ�������
�������  : iDefault�����ص�ȱʡ����ֵ
�������  : N/A
����ֵ    : ���ض�ȡ������ֵ�����section,key�������򷵻�iDefault      
��ע     : N/A
* �޸�����          �汾��       �޸���	      �޸�����
* -----------------------------------------------------
* 2012/05/21	     V1.0	     ��ȫ��	       ����
*****************************************************************************/
int CIniFile::ReadInt(const string& strSection, const string& strKey, int iDefault/* = -1*/)
{
    return GetPrivateProfileInt(strSection.c_str(), strKey.c_str(), iDefault, m_strIniFilePath.c_str());
}

/*****************************************************************************
��������  :	WriteString
��������  : ��ָ����section,key��д��һ���ַ���ֵ
�������  : strSection���ֶ�������[]���������
�������  : strKey����������=ǰ�������
�������  : strValue����д����ַ��� (=�ź�����ַ���)
�������  : N/A
����ֵ    : void
* �޸�����          �汾��       �޸���	      �޸�����
* -----------------------------------------------------
* 2012/05/21	     V1.0	     ��ȫ��	       ����
*****************************************************************************/
void CIniFile::WriteString(const string& strSection, const string& strKey, const string& strValue)
{
    WritePrivateProfileString(strSection.c_str(), strKey.c_str(), strValue.c_str(), m_strIniFilePath.c_str());
}

/*****************************************************************************
��������  :	WriteInt
��������  : ��ָ����section,key��д��һ������ֵ
�������  : strSection���ֶ�������[]���������
�������  : strKey����������=ǰ�������
�������  : iValue����д�������ֵ (=�ź��������ֵ)
�������  : N/A
����ֵ    : void       
��ע     : N/A
* �޸�����          �汾��       �޸���	      �޸�����
* -----------------------------------------------------
* 2012/05/21	     V1.0	     ��ȫ��	       ����
*****************************************************************************/
void CIniFile::WriteInt(const string& strSection, const string& strKey, int iValue)
{
    char chValue[20] = {0};
    sprintf(chValue,"%d",iValue);
    (void)WritePrivateProfileString(strSection.c_str(), strKey.c_str(), chValue, m_strIniFilePath.c_str());
}


#define MAX_FILE_SIZE 1024*16
#define LEFT_BRACE '['
#define RIGHT_BRACE ']'

int CIniFile::load_ini_file(const char *file, char *buf,int *file_size)
{
	FILE *in = NULL;
	int i=0;
	*file_size =0;

	assert(file !=NULL);
	assert(buf !=NULL);

	in = fopen(file,"r");
	if( NULL == in) {
		return 0;
	}

	buf[i]=fgetc(in);
	
	//load initialization file
	while( buf[i]!= (char)EOF) {
		i++;
		assert( i < MAX_FILE_SIZE ); //file too big, you can redefine MAX_FILE_SIZE to fit the big file 
		buf[i]=fgetc(in);
	}
	
	buf[i]='\0';
	*file_size = i;

	fclose(in);
	return 1;
}

int CIniFile::newline(char c)
{
	return ('\n' == c ||  '\r' == c )? 1 : 0;
}
int CIniFile::end_of_string(char c)
{
	return '\0'==c? 1 : 0;
}
int CIniFile::left_barce(char c)
{
	return LEFT_BRACE == c? 1 : 0;
}
int CIniFile::isright_brace(char c )
{
	return RIGHT_BRACE == c? 1 : 0;
}
int CIniFile::parse_file(const char *section, const char *key, const char *buf,int *sec_s,int *sec_e,
					  int *key_s,int *key_e, int *value_s, int *value_e)
{
	const char *p = buf;
	int i=0;

	assert(buf!=NULL);
	assert(section != NULL && strlen(section));
	assert(key != NULL && strlen(key));

	*sec_e = *sec_s = *key_e = *key_s = *value_s = *value_e = -1;

	while( !end_of_string(p[i]) ) {
		//find the section
		if( ( 0==i ||  newline(p[i-1]) ) && left_barce(p[i]) )
		{
			int section_start=i+1;

			//find the ']'
			do {
				i++;
			} while( !isright_brace(p[i]) && !end_of_string(p[i]));

			if( 0 == strncmp(p+section_start,section, i-section_start)) {
				int newline_start=0;

				i++;

				//Skip over space char after ']'
				while(isspace(p[i])) {
					i++;
				}

				//find the section
				*sec_s = section_start;
				*sec_e = i;

				while( ! (newline(p[i-1]) && left_barce(p[i])) 
				&& !end_of_string(p[i]) ) {
					int j=0;
					//get a new line
					newline_start = i;

					while( !newline(p[i]) &&  !end_of_string(p[i]) ) {
						i++;
					}
					
					//now i  is equal to end of the line
					j = newline_start;

					if(';' != p[j]) //skip over comment
					{
						while(j < i && p[j]!='=') {
							j++;
							if('=' == p[j]) {
								if(strncmp(key,p+newline_start,strlen(key))==0)
								{
									//find the key ok
									*key_s = newline_start;
									*key_e = j-1;

									*value_s = j+1;
									*value_e = i;

									return 1;
								}
							}
						}
					}

					i++;
				}
			}
		}
		else
		{
			i++;
		}
	}
	return 0;
}

int CIniFile::GetPrivateProfileString( const char  *section, const char  *key,const char  *default_value,char  *value, int size, const char  *file)
{
	char buf[MAX_FILE_SIZE]={0};
	int file_size;
	int sec_s,sec_e,key_s,key_e, value_s, value_e;

	//check parameters
	assert(section != NULL && strlen(section));
	assert(key != NULL && strlen(key));
	assert(value != NULL);
	assert(size > 0);
	assert(file !=NULL &&strlen(key));

	if(!load_ini_file(file,buf,&file_size))
	{
		if(default_value!=NULL)
		{
			strncpy(value,default_value, size);
		}
		return 0;
	}

	if(!parse_file(section,key,buf,&sec_s,&sec_e,&key_s,&key_e,&value_s,&value_e))
	{
		if(default_value!=NULL)
		{
			strncpy(value,default_value, size);
		}
		return 0; //not find the key
	}
	else
	{
		int cpcount = value_e -value_s;

		if( size-1 < cpcount)
		{
			cpcount =  size-1;
		}
	
		memset(value, 0, size);
		memcpy(value,buf+value_s, cpcount );
		value[cpcount] = '\0';

		return 1;
	}
}

int CIniFile::GetPrivateProfileInt( const char *section, const char *key,int default_value, 
				const char *file)
{
	char value[32] = {0};
	if(!GetPrivateProfileString(section,key,NULL,value, sizeof(value),file))
	{
		return default_value;
	}
	else
	{
		return atoi(value);
	}
}

int CIniFile::WritePrivateProfileString(const char *section, const char *key,
					const char *value, const char *file)
{
	char buf[MAX_FILE_SIZE]={0};
	char w_buf[MAX_FILE_SIZE]={0};
	int sec_s,sec_e,key_s,key_e, value_s, value_e;
	int value_len = (int)strlen(value);
	int file_size;
	FILE *out;

	//check parameters
	assert(section != NULL && strlen(section));
	assert(key != NULL && strlen(key));
	assert(value != NULL);
	assert(file !=NULL &&strlen(key));

	if(!load_ini_file(file,buf,&file_size))
	{
		sec_s = -1;
	}
	else
	{
		parse_file(section,key,buf,&sec_s,&sec_e,&key_s,&key_e,&value_s,&value_e);
	}

	if( -1 == sec_s)
	{
		if(0==file_size)
		{
			sprintf(w_buf+file_size,"[%s]\n%s=%s\n",section,key,value);
		}
		else
		{
			//not find the section, then add the new section at end of the file
			memcpy(w_buf,buf,file_size);
			sprintf(w_buf+file_size,"\n[%s]\n%s=%s\n",section,key,value);
		}
	}
	else if(-1 == key_s)
	{
		//not find the key, then add the new key=value at end of the section
		memcpy(w_buf,buf,sec_e);
		sprintf(w_buf+sec_e,"%s=%s\n",key,value);
		sprintf(w_buf+sec_e+strlen(key)+strlen(value)+2,buf+sec_e, file_size - sec_e);
	}
	else
	{
		//update value with new value
		memcpy(w_buf,buf,value_s);
		memcpy(w_buf+value_s,value, value_len);
		memcpy(w_buf+value_s+value_len, buf+value_e, file_size - value_e);
	}
	
	out = fopen(file,"w");
	if(NULL == out)
	{
		return 0;
	}
	
	if(-1 == fputs(w_buf,out) )
	{
		fclose(out);
		return 0;
	}

	fclose(out);
	return 1;
}

