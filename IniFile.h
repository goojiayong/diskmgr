/*******************************************************************************
*   文件名称 : IniFile.h
*   作    者 : 王全宝
*   创建时间 : 2015/10/8   16:08
*   文件描述 : 
*   版权声明 : Copyright (C) 2014-2015 湖南安存科技有限公司
*   修改历史 : 2015/10/8   1.00 初始版本
*******************************************************************************/

#pragma once

#include <string>
using std::string;

class CIniFile
{
public:
    /*****************************************************************************
    函数名称  :	CIniFile
    函数描述  : 构造函数
    输入参数  : strIniFilePath,ini文件的路径
    输入参数  : N/A
    输出参数  : N/A
    返回值    : N/A
    * 修改日期          版本号       修改人	      修改内容
    * -----------------------------------------------------
    * 2012/05/21	     V1.0	     王全宝	       创建
    *****************************************************************************/
    CIniFile(const string& strIniFilePath);

    /*****************************************************************************
    函数名称  :	~CIniFile
    函数描述  : 析构函数
    输入参数  : N/A
    输出参数  : N/A
    返回值    : N/A
    备注      :	N/A
    * 修改日期          版本号       修改人	      修改内容
    * -----------------------------------------------------
    * 2012/05/21	     V1.0	     王全宝	       创建
    *****************************************************************************/
    ~CIniFile(void);
    
    /*****************************************************************************
    函数名称  :	ReadString
    函数描述  : 读取一个字符串值
    输入参数  : strSection，字段名，即[]里面的名称
    输入参数  : strKey，键名，即=前面的名称
    输入参数  : strResult，返回的缺省字符串
    输出参数  : N/A
    返回值    : 返回读取的整数值，如果section,key不存在则返回strDefault      
    备注     : N/A
    * 修改日期          版本号       修改人	      修改内容
    * -----------------------------------------------------
    * 2012/05/21	     V1.0	     王全宝	       创建
    *****************************************************************************/
    string ReadString(const string& strSection, const string& strKey, const string& strDefault = "");

    /*****************************************************************************
    函数名称  :	ReadInt
    函数描述  : 读取一个整数值
    输入参数  : strSection，字段名，即[]里面的名称
    输入参数  : strKey，键名，即=前面的名称
    输入参数  : iDefault，返回的缺省整数值
    输出参数  : N/A
    返回值    : 返回读取的整数值，如果section,key不存在则返回iDefault      
    备注     : N/A
    * 修改日期          版本号       修改人	      修改内容
    * -----------------------------------------------------
    * 2012/05/21	     V1.0	     王全宝	       创建
    *****************************************************************************/
    int ReadInt(const string& strSection, const string& strKey, int iDefault = -1);

    /*****************************************************************************
    函数名称  :	WriteString
    函数描述  : 在指定的section,key处写入一个字符串值
    输入参数  : strSection，字段名，即[]里面的名称
    输入参数  : strKey，键名，即=前面的名称
    输入参数  : strValue，待写入的字符串 (=号后面的字符串)
    输出参数  : N/A
    返回值    : void
    * 修改日期          版本号       修改人	      修改内容
    * -----------------------------------------------------
    * 2012/05/21	     V1.0	     王全宝	       创建
    *****************************************************************************/
    void WriteString(const string& strSection, const string& strKey, const string& strValue);

    /*****************************************************************************
    函数名称  :	WriteInt
    函数描述  : 在指定的section,key处写入一个整数值
    输入参数  : strSection，字段名，即[]里面的名称
    输入参数  : strKey，键名，即=前面的名称
    输入参数  : iValue，待写入的整数值 (=号后面的整数值)
    输出参数  : N/A
    返回值    : void       
    备注     : N/A
    * 修改日期          版本号       修改人	      修改内容
    * -----------------------------------------------------
    * 2012/05/21	     V1.0	     王全宝	       创建
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
    // ini文件路径
    string                        m_strIniFilePath;

};

extern CIniFile*   g_pConfig;  //声明一个全局对象

#define GLOABL_SECTION   "Gloabl"      //section
#define KEY_DURATION     "duration"    //duration
#define KEY_LOGLEVEL     "LogLevel"    //LogLevel
#define KEY_MSG_HOST     "msgIp"       //消息总线IP
#define KEY_MSG_PORT     "msgPort"     //消息总线端口号
#define KEY_TRANS_TYPE   "TransType"   //数据传输协议类型
#define KEY_DIRECT_C     "DirectCamera" //取流方式
#define KEY_INTERVAL     "ReconnInterval" //断线重连时间间隔
#define KEY_RETIMES      "ReconnetTimes" //断线重连次数
#define KEY_DATABASEIP   "DatabaseIp" //数据库IP
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