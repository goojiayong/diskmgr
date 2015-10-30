/*******************************************************************************
*   �ļ����� : EasyLog.cpp
*   ��    �� : ��ȫ��
*   ����ʱ�� : 2015/10/8   16:08
*   �ļ����� : 
*   ��Ȩ���� : Copyright (C) 2014-2015 ���ϰ���Ƽ����޹�˾
*   �޸���ʷ : 2015/10/8   1.00 ��ʼ�汾
*******************************************************************************/


#include "EasyLog.h"
#include <string>
using std::string;

#ifdef _WINDOWS
#pragma warning (disable:4996)
#endif

// ��־�ļ���С�����1000K,�����ʹ��xxx.bak�ļ�
#define MAX_LOGFILE_LEN 1000 * 1024

/*******************************************************************************
* ��������  : CEasyLog
* ��������  : ���캯��
* �������  : N/A
* �������  : N/A
* ����ֵ    : N/A
* ��ע      : N/A
* �޸�����     �޸���   �޸�����
* -----------------------------------------------
* 2012/11/05    ��ȫ��   �½�
*******************************************************************************/
CEasyLog::CEasyLog()
#ifdef _WINDOWS
    :m_hMutex(NULL)    
#else //windows
    :m_pMutex(PTHREAD_MUTEX_INITIALIZER)
#endif    
    ,m_iLogLevel(LOG_LEVEL_ERROR)
{
    memset(m_szLogFilePath, 0, sizeof(m_szLogFilePath));
}

/*******************************************************************************
* ��������  : ~CEasyLog
* ��������  : ��������
* �������  : N/A
* �������  : N/A
* ����ֵ    : N/A
* ��ע      : N/A
* �޸�����     �޸���   �޸�����
* -----------------------------------------------
* 2012/11/05    ��ȫ��   �½�
*******************************************************************************/
CEasyLog::~CEasyLog(void)
{
    Close();
}

/*******************************************************************************
* ��������  : Open
* ��������  : ������־ϵͳ
* �������  : pszLogFilePath����־�ļ���
                iLogLevel: ��־����,Ĭ��error����
* �������  : N/A
* ����ֵ    : N/A
* ��ע      : N/A
* �޸�����     �޸���   �޸�����
* -----------------------------------------------
* 2012/11/05    ��ȫ��   �½�
*******************************************************************************/
void CEasyLog::Open(const char* pszLogFilePath, int iLogLevel /*= LOG_LEVEL_ERROR*/)
{
    strncpy(m_szLogFilePath, pszLogFilePath, FILENAME_MAX - 1);
    m_szLogFilePath[FILENAME_MAX - 1] = '\0';
 
#ifdef _WINDOWS
    //ֱ�����ļ��ľ���·����MUTEX�����֣��ᴴ��ʧ�ܣ���fd
    string strMutexName = m_szLogFilePath;
    strMutexName = strMutexName.substr(strMutexName.rfind("\\") + 1) +" log_mutex";   
    m_hMutex = CreateMutex(NULL, FALSE, strMutexName.c_str()); //�����ļ��������������
#else //windows
    pthread_mutex_init(&m_pMutex,NULL);
#endif
    m_iLogLevel = iLogLevel;
}
 
/*******************************************************************************
* ��������  : Close
* ��������  : �ر���־ϵͳ
* �������  : N/A
* �������  : N/A
* ����ֵ    : N/A
* ��ע      : N/A
* �޸�����     �޸���   �޸�����
* -----------------------------------------------
* 2012/11/05    ��ȫ��   �½�
*******************************************************************************/
void CEasyLog::Close()
{
#ifdef _WINDOWS
    if (m_hMutex != NULL)
    {
        CloseHandle(m_hMutex);
        m_hMutex = NULL;
    }
#else 
    pthread_mutex_destroy(&m_pMutex);

#endif
}

/*******************************************************************************
* ��������  : WriteLog
* ��������  : д��־
* �������  : iLevel:  ��־����
             pszFmt: ������־��Ϣ
* �������  : N/A
* ����ֵ    : N/A
* ��ע      : N/A
* �޸�����     �޸���   �޸�����
* -----------------------------------------------
* 2012/11/05    ��ȫ��   �½�
*******************************************************************************/
void CEasyLog::WriteLog(int iLevel, const char* pszFile, int iLine, const char* pszFmt, ...)
{
    if (m_iLogLevel >= iLevel)
    {
#ifdef _WINDOWS
       WaitForSingleObject(m_hMutex, INFINITE); 
#else 
        pthread_mutex_lock(&m_pMutex);        
#endif        

        CheckFileSize(); //������С����
        FILE* fp = fopen(m_szLogFilePath, "a");
        if (fp != NULL)
        {
            char szBuff[1024] = {0};
            va_list argptr;
            va_start(argptr, pszFmt);
            vsnprintf(szBuff, 1023, pszFmt, argptr);
            va_end(argptr);
#ifdef _WINDOWS
            SYSTEMTIME st;
            GetLocalTime(&st);

            fprintf(fp, "%s(%d)[%u][%04d-%d-%d %02d:%02d:%02d:%03d]%s\n", 
                pszFile, iLine,
                GetCurrentThreadId(),
                st.wYear, st.wMonth, st.wDay,st.wHour, st.wMinute, st.wSecond, st.wMilliseconds,
                szBuff);

#else 
            time_t t =time(NULL);
            struct tm local = { 0 };
            localtime_r(&t, &local);
            //struct tm *local;
            //time_t t;
            //t=time(NULL);
            //local=localtime(&t);
            fprintf(fp, "%s(%d)[%x][%04d-%d-%d %02d:%02d:%02d]%s\n", 
                pszFile, iLine,
                (unsigned)pthread_self(),
                1900+local.tm_year, 1+local.tm_mon, local.tm_mday,local.tm_hour, local.tm_min, local.tm_sec,
                szBuff);

#endif
            fclose(fp);
        }
#ifdef _WINDOWS
       ReleaseMutex(m_hMutex); 
#else 
       pthread_mutex_unlock(&m_pMutex);
        
#endif
        
    }
}

/*******************************************************************************
* ��������  : CheckFileSize
* ��������  : �����־�ļ���С��������С��ת��
* �������  : N/A
* �������  : N/A
* ����ֵ    : N/A
* ��ע      : �����С�������������xx.bak��,������־��������պ��ԭ��־�ļ�д
* �޸�����     �޸���   �޸�����
* -----------------------------------------------
* 2012/11/05    ��ȫ��   �½�
*******************************************************************************/
void CEasyLog::CheckFileSize()
{
    FILE* fp = fopen(m_szLogFilePath, "rb");
    if (fp != NULL)
    {
        // �Ȼ�ȡ�ļ���С
        fseek(fp, 0, SEEK_END);
        long size = ftell(fp);
        fclose(fp);

        // �ļ������������Ƶ������ļ�
        if (size > MAX_LOGFILE_LEN)
        {
            // �����ļ�
            char szBakFile[FILENAME_MAX] = {0};
            sprintf(szBakFile, "%s.bak", m_szLogFilePath);
            FILE* fpbak =fopen(szBakFile, "rb");
            if (fpbak != NULL) //����ļ��Ѵ���
            {
                fclose(fpbak);
                remove(szBakFile);  //ɾ���ļ�
            }

            rename(m_szLogFilePath, szBakFile); //�������ļ�
        }
    }
}

