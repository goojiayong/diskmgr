/*******************************************************************************
*   �ļ����� : CommFunc.cpp
*   ��    �� : ��ȫ��
*   ����ʱ�� : 2015/10/8   16:07
*   �ļ����� : 
*   ��Ȩ���� : Copyright (C) 2014-2015 ���ϰ���Ƽ����޹�˾
*   �޸���ʷ : 2015/10/8   1.00 ��ʼ�汾
*******************************************************************************/

#include "CommFunc.h"

#ifdef _WINDOWS
#include <Windows.h>
#include <Shlwapi.h>
#else
#include <unistd.h>
#include <sys/stat.h>
#endif

/*******************************************************************************
* ��������  : IsDirectoryExist
* ��������  : N/A
* ����      : string & strPath
* ����ֵ    : bool
* ��ע      : N/A
* �޸�����     �޸���   �޸�����
* -----------------------------------------------
* 2013/12/30   ��ȫ��   �½�
*******************************************************************************/
bool CCommFunc::IsDirectoryExist( const string& strPath )
{
    if (strPath.empty())
    {
        return false;
    }
    
#ifdef _WINDOWS
    DWORD dw;
    dw = GetFileAttributesA(strPath.c_str());
    if (((dw!=INVALID_FILE_ATTRIBUTES)&&(dw&FILE_ATTRIBUTE_DIRECTORY)) == FALSE)
    {
        return false;
    }    
#else
    if(access(strPath.c_str(), F_OK) == -1) //������
    {
        return false;
    }
#endif

    return true;
}

bool CCommFunc::MyCreateDirectory( string& strPath )
{
#ifdef _WINDOWS
    if (CreateDirectory(strPath.c_str(), NULL) == FALSE)
    {
        return false;
    }    
#else
    if(mkdir(strPath.c_str(), 0777) == -1) //ʧ��
    {
        return false;
    }
#endif

    return true;
}

/*******************************************************************************
* ��������  : GetProgrammeDir
* ��������  : ��ȡ����ǰ·��
* �������  : itype :  0: ����"\"; 1:������
* �������  : N/A
* ����ֵ    : N/A
* ��ע      : N/A
* �޸�����     �޸���   �޸�����
* -----------------------------------------------
* 2013/01/05    ��ȫ��   �½�
*******************************************************************************/
void CCommFunc::GetProgrammeDir(string& strPath, int itype /*= 0*/)
{
#ifdef _WINDOWS
    //ȡ����·��
    char szFilePath[MAX_PATH] = {0};
    (void)GetModuleFileName(NULL,szFilePath,MAX_PATH);
    strPath = szFilePath;
    if (itype == 0)
    {
        strPath = strPath.substr(0, strPath.rfind("\\")+1);
    }
    else
    {
        strPath = strPath.substr(0, strPath.rfind("\\"));
    }
#else   
    char buf[256];
    int count = readlink( "/proc/self/exe", buf, 256 ); 
    if ( count < 0 ) 
    { 
         buf[0] = '\0';
         return ; 
    } 
    strPath = buf;
    if (itype == 0)
    {
        strPath = strPath.substr(0, strPath.rfind("/")+1);
    }
    else
    {
        strPath = strPath.substr(0, strPath.rfind("/"));
    }

#endif
}

void CCommFunc::trim(string& strSrc)
{
    //left
    strSrc.erase(0, strSrc.find_first_not_of(' '));
    //right
    strSrc.erase(strSrc.find_last_not_of(' ')+1);
}
