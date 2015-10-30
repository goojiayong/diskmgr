/*******************************************************************************
*   �ļ����� : raidmgr.cpp
*   ��    �� : ��ȫ��
*   ����ʱ�� : 2015/9/12   14:27
*   �ļ����� : 
*   ��Ȩ���� : Copyright (C) 2014-2015 ���ϰ���Ƽ����޹�˾
*   �޸���ʷ : 2015/9/12   1.00 ��ʼ�汾
*******************************************************************************/

#include "EasyLog.h"
#include "IniFile.h"
#include "CommFunc.h"

#define DISKMGR_VERSION      "v1.1.000"

CEasyLog g_ClientLog;  //ȫ����־����
CIniFile*   g_pConfig = NULL; //ȫ�������ļ�����

/*******************************************************************************
* ��������  : main
* ��������  : N/A
* ����      : int argc
* ����      : char * argv[]
* ����ֵ    : int
* ��ע      : N/A
* �޸�����     �޸���   �޸�����
* -----------------------------------------------
* 2015/9/11   ��ȫ��   �½�
*******************************************************************************/
int main(int argc, char *argv [])
{    
    //ȡ����·��
    string strFilePath;
    CCommFunc::GetProgrammeDir(strFilePath);

    string strConfigFile = strFilePath + "Config.ini";
    string strLogFile = strFilePath + "LiveStore.log";

    g_pConfig = new CIniFile(strConfigFile); //ֻ������ʽ�������ļ�����
    if (NULL == g_pConfig)
    {
        return -1;
    }
    //�������ļ��л�ȡ��־����
    int iLogLevel = g_pConfig->ReadInt(GLOABL_SECTION, KEY_LOGLEVEL, 1);

    //����־
    g_ClientLog.Open(strLogFile.c_str(), iLogLevel);

    //������־
    MYTRACE(LOG_LEVEL_INFO, "livestore version<%s>", DISKMGR_VERSION);
    return 0;
}