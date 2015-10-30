/*******************************************************************************
*   文件名称 : DiskinfoApp.cpp
*   作    者 : 王全宝
*   创建时间 : 2015/9/12   15:59
*   文件描述 : 
*   版权声明 : Copyright (C) 2014-2015 湖南安存科技有限公司
*   修改历史 : 2015/9/12   1.00 初始版本
*******************************************************************************/

#include "DiskinfoApp.h"
#include "DirectDiskInfo.h"
#include "HardRaidDiskInfo.h"
#include "EasyLog.h"
#include "commdef.h"

CDiskinfoApp* CDiskinfoApp::m_pInstance = NULL;

/*******************************************************************************
* 函数名称  : Init
* 函数描述  : N/A
* 参数      : int argc
* 参数      : char * argv[]
* 返回值    : void
* 备注      : N/A
* 修改日期     修改人   修改内容
* -----------------------------------------------
* 2015/9/12   王全宝   新建
*******************************************************************************/
void CDiskinfoApp::Init(int argc, char *argv[])
{
    //todo:初始化设置，构造IDiskInfo对象
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
* 函数名称  : Run
* 函数描述  : N/A
* 返回值    : void
* 备注      : N/A
* 修改日期     修改人   修改内容
* -----------------------------------------------
* 2015/9/12   王全宝   新建
*******************************************************************************/
void CDiskinfoApp::Run()
{
    //todo:执行处理函数
    if (NULL != m_pDiskInfo)
    {
        m_pDiskInfo->GetDiskInfo();
        string strDst;
        m_pDiskInfo->DiskInfoToJson(strDst);

        printf("%s", strDst.c_str());
    }

}

/*******************************************************************************
* 函数名称  : IsHardRaid
* 函数描述  : N/A
* 返回值    : bool
* 备注      : N/A
* 修改日期     修改人   修改内容
* -----------------------------------------------
* 2015/10/26   郭家勇   新建
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
* 函数名称  : GetInstance
* 函数描述  : N/A
* 返回值    : CDiskinfoApp*
* 备注      : N/A
* 修改日期     修改人   修改内容
* -----------------------------------------------
* 2015/9/12   王全宝   新建
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
* 函数名称  : ReleaseInstance
* 函数描述  : N/A
* 返回值    : void
* 备注      : N/A
* 修改日期     修改人   修改内容
* -----------------------------------------------
* 2015/9/12   王全宝   新建
*******************************************************************************/
void CDiskinfoApp::ReleaseInstance()
{
    if (NULL != m_pInstance)
    {
        delete m_pInstance;
    }
}

/*******************************************************************************
* 函数名称  : CDiskinfoApp
* 函数描述  : N/A
* 返回值    : 
* 备注      : N/A
* 修改日期     修改人   修改内容
* -----------------------------------------------
* 2015/10/09   王全宝   新建
*******************************************************************************/
CDiskinfoApp::CDiskinfoApp()
    :m_pDiskInfo(NULL)
{
}


/*******************************************************************************
* 函数名称  : ~CDiskinfoApp
* 函数描述  : N/A
* 返回值    : 
* 备注      : N/A
* 修改日期     修改人   修改内容
* -----------------------------------------------
* 2015/10/09   王全宝   新建
*******************************************************************************/
CDiskinfoApp::~CDiskinfoApp()
{
    DELETEP(m_pDiskInfo);
}

int CDiskinfoApp::syscmd_result(char* cmd, string& resvec)
{
	FILE *pp = popen(cmd, "r"); //建立管道
	if (!pp) {
		return -1;
	}
	char tmp[1024]; //设置一个合适的长度，以存储每一行输出
	while (fgets(tmp, sizeof(tmp), pp) != NULL) {
		if (tmp[strlen(tmp) - 1] == '\n') {
			tmp[strlen(tmp) - 1] = '\0'; //去除换行符
		}
		string s(tmp);
		resvec = s;
	}
	pclose(pp); //关闭管道
	return 0;
}


