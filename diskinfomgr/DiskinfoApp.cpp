/*******************************************************************************
*   �ļ����� : DiskinfoApp.cpp
*   ��    �� : ��ȫ��
*   ����ʱ�� : 2015/9/12   15:59
*   �ļ����� : 
*   ��Ȩ���� : Copyright (C) 2014-2015 ���ϰ���Ƽ����޹�˾
*   �޸���ʷ : 2015/9/12   1.00 ��ʼ�汾
*******************************************************************************/

#include "DiskinfoApp.h"
#include "DirectDiskInfo.h"
#include "HardRaidDiskInfo.h"
#include "EasyLog.h"
#include "commdef.h"

CDiskinfoApp* CDiskinfoApp::m_pInstance = NULL;

/*******************************************************************************
* ��������  : Init
* ��������  : N/A
* ����      : int argc
* ����      : char * argv[]
* ����ֵ    : void
* ��ע      : N/A
* �޸�����     �޸���   �޸�����
* -----------------------------------------------
* 2015/9/12   ��ȫ��   �½�
*******************************************************************************/
void CDiskinfoApp::Init(int argc, char *argv[])
{
    //todo:��ʼ�����ã�����IDiskInfo����
	int ret = -1;
	ret = IsHardRaid();
	
	if (ret > 0)
	{
		m_pDiskInfo = new CHardRaidDiskInfo();
	} 
	else if(ret == 0)
	{
		m_pDiskInfo = new CDirectDiskInfo();
	}
	
	m_pDiskInfo->Init(argc, argv);
}

/*******************************************************************************
* ��������  : Run
* ��������  : N/A
* ����ֵ    : void
* ��ע      : N/A
* �޸�����     �޸���   �޸�����
* -----------------------------------------------
* 2015/9/12   ��ȫ��   �½�
*******************************************************************************/
void CDiskinfoApp::Run()
{
    //todo:ִ�д�����
    if (NULL != m_pDiskInfo)
    {
        m_pDiskInfo->GetDiskInfo();
        string strDst;
        m_pDiskInfo->DiskInfoToJson(strDst);

        printf("%s", strDst.c_str());
    }

}

/*******************************************************************************
* ��������  : IsHardRaid
* ��������  : N/A
* ����ֵ    : bool
* ��ע      : N/A
* �޸�����     �޸���   �޸�����
* -----------------------------------------------
* 2015/10/26   ������   �½�
*******************************************************************************/
int CDiskinfoApp::IsHardRaid()
{
	int ret = 0;
	char get_adapnum_cmd[128] = {};
	sprintf(get_adapnum_cmd, " MegaCli -pdlist -aAll | grep Adapter | wc -l");
	string num;
	ret = syscmd_result(get_adapnum_cmd, num);
	if (ret < 0)
	{
		MYTRACE(LOG_LEVEL_ERROR, "get adapter number error");
		return ret;
	}

	int adapnum = atoi(num.c_str());
	MYTRACE(LOG_LEVEL_DEBUG, "get adapter number:%d ", adapnum);

	return adapnum;
}

/*******************************************************************************
* ��������  : GetInstance
* ��������  : N/A
* ����ֵ    : CDiskinfoApp*
* ��ע      : N/A
* �޸�����     �޸���   �޸�����
* -----------------------------------------------
* 2015/9/12   ��ȫ��   �½�
*******************************************************************************/
CDiskinfoApp* CDiskinfoApp::GetInstance()
{
    if (NULL == m_pInstance)
    {
        m_pInstance = new CDiskinfoApp();
    }

    return m_pInstance;
}

/*******************************************************************************
* ��������  : ReleaseInstance
* ��������  : N/A
* ����ֵ    : void
* ��ע      : N/A
* �޸�����     �޸���   �޸�����
* -----------------------------------------------
* 2015/9/12   ��ȫ��   �½�
*******************************************************************************/
void CDiskinfoApp::ReleaseInstance()
{
    if (NULL != m_pInstance)
    {
        delete m_pInstance;
    }
}

/*******************************************************************************
* ��������  : CDiskinfoApp
* ��������  : N/A
* ����ֵ    : 
* ��ע      : N/A
* �޸�����     �޸���   �޸�����
* -----------------------------------------------
* 2015/10/09   ��ȫ��   �½�
*******************************************************************************/
CDiskinfoApp::CDiskinfoApp()
    :m_pDiskInfo(NULL)
{
}


/*******************************************************************************
* ��������  : ~CDiskinfoApp
* ��������  : N/A
* ����ֵ    : 
* ��ע      : N/A
* �޸�����     �޸���   �޸�����
* -----------------------------------------------
* 2015/10/09   ��ȫ��   �½�
*******************************************************************************/
CDiskinfoApp::~CDiskinfoApp()
{
    DELETEP(m_pDiskInfo);
}

int CDiskinfoApp::syscmd_result(char* cmd, string& resvec)
{
	FILE *pp = popen(cmd, "r"); //�����ܵ�
	if (!pp) {
		return -1;
	}
	char tmp[1024]; //����һ�����ʵĳ��ȣ��Դ洢ÿһ�����
	while (fgets(tmp, sizeof(tmp), pp) != NULL) {
		if (tmp[strlen(tmp) - 1] == '\n') {
			tmp[strlen(tmp) - 1] = '\0'; //ȥ�����з�
		}
		string s(tmp);
		resvec = s;
	}
	pclose(pp); //�رչܵ�
	return 0;
}


