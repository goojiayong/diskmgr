/*******************************************************************************
*   �ļ����� : IniFile.h
*   ��    �� : ��ȫ��
*   ����ʱ�� : 2015/10/8   16:08
*   �ļ����� : 
*   ��Ȩ���� : Copyright (C) 2014-2015 ���ϰ���Ƽ����޹�˾
*   �޸���ʷ : 2015/10/8   1.00 ��ʼ�汾
*******************************************************************************/

#pragma once

#include <string>
using std::string;

class CIniFile
{
public:
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
    CIniFile(const string& strIniFilePath);

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
    ~CIniFile(void);
    
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
    string ReadString(const string& strSection, const string& strKey, const string& strDefault = "");

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
    int ReadInt(const string& strSection, const string& strKey, int iDefault = -1);

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
    void WriteString(const string& strSection, const string& strKey, const string& strValue);

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
    void WriteInt(const string& strSection, const string& strKey, int iValue);
protected:
    int GetPrivateProfileString( const char  *section, const char  *key,const char  *default_value, char * constvalue, int size, const char  * file);
    int GetPrivateProfileInt( const char *section, const char *key,int default_value, const char *file);
    int WritePrivateProfileString( const char *section, const char *key,const char *value, const char *file);

private:
    int load_ini_file(const char *file, char *buf,int *file_size);
    int newline(char c);
    int end_of_string(char c);
    int left_barce(char c);
    int isright_brace(char c );
    int parse_file(const char *section, const char *key, const char *buf,int *sec_s,int *sec_e,
        int *key_s,int *key_e, int *value_s, int *value_e);

private:
    // ini�ļ�·��
    string                        m_strIniFilePath;

};

extern CIniFile*   g_pConfig;  //����һ��ȫ�ֶ���

#define GLOABL_SECTION   "Gloabl"      //section
#define KEY_DURATION     "duration"    //duration
#define KEY_LOGLEVEL     "LogLevel"    //LogLevel
#define KEY_MSG_HOST     "msgIp"       //��Ϣ����IP
#define KEY_MSG_PORT     "msgPort"     //��Ϣ���߶˿ں�
#define KEY_TRANS_TYPE   "TransType"   //���ݴ���Э������
#define KEY_DIRECT_C     "DirectCamera" //ȡ����ʽ
#define KEY_INTERVAL     "ReconnInterval" //��������ʱ����
#define KEY_RETIMES      "ReconnetTimes" //������������
#define KEY_DATABASEIP   "DatabaseIp" //���ݿ�IP
#define KEY_DATABASEPORT "DatabasePort"
#define KEY_DATABASENAME "DatabaseName"
#define KEY_USERNAME     "UserName"
#define KEY_PWD          "Password"
#define KEY_S_IP         "SocketIp"
#define KEY_S_PORT       "SocketPort"

#define KEY_CHANNEL_NUM   "Channel"
#define KEY_CHANNEL_START "StartNum"
#define KEY_CHANNEL_SRC   "ChannelSrc"
#define KEY_DISK_PATH     "DiskPath"