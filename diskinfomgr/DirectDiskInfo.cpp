/*******************************************************************************
*   文件名称 : DirectDiskInfo.cpp
*   作    者 : 王全宝
*   创建时间 : 2015/10/9   10:03
*   文件描述 : 
*   版权声明 : Copyright (C) 2014-2015 湖南安存科技有限公司
*   修改历史 : 2015/10/9   1.00 初始版本
*******************************************************************************/

#include "DirectDiskInfo.h"
#include "json/json.h"
#include <iostream>
using namespace std;

/*******************************************************************************
* 函数名称  : CDirectDiskInfo
* 函数描述  : N/A
* 返回值    : 
* 备注      : N/A
* 修改日期     修改人   修改内容
* -----------------------------------------------
* 2015/10/09   王全宝   新建
*******************************************************************************/
CDirectDiskInfo::CDirectDiskInfo()
{
}


/*******************************************************************************
* 函数名称  : ~CDirectDiskInfo
* 函数描述  : N/A
* 返回值    : 
* 备注      : N/A
* 修改日期     修改人   修改内容
* -----------------------------------------------
* 2015/10/09   王全宝   新建
*******************************************************************************/
CDirectDiskInfo::~CDirectDiskInfo()
{

}

/*******************************************************************************
* 函数名称  : Init
* 函数描述  : N/A
* 参数      : int argc
* 参数      : char * argv[]
* 返回值    : void
* 备注      : N/A
* 修改日期     修改人   修改内容
* -----------------------------------------------
* 2015/10/09   王全宝   新建
*******************************************************************************/
void CDirectDiskInfo::Init(int argc, char *argv[])
{
	int ret = 0;

	m_strRaidtype = "soft_raid";
	m_intRetCode = 0;

	char get_alldisk_cmd[256] = {};
	sprintf(get_alldisk_cmd, "fdisk -l 2>/dev/null |grep 'Disk /dev/sd' |awk -F ' |:' '{print $2}'|grep -v ` fdisk -l |grep /dev/sd |awk '{print $1 $2}' |grep '*' |awk '{print substr($1,0,8)}'`");
	/*sprintf(get_alldisk_cmd, "fdisk -l 2>/dev/null | grep 'Disk /dev/sd' | awk '{print $2}' | awk -F : '{print $1}'");*/
	ret = syscmd_result(get_alldisk_cmd, m_vecAllDisk);
#if 0
	for (int i = 0; i < m_vecAllDisk.size();i++)
	{
		cout << m_vecAllDisk[i] << endl;
	}
#endif

	if (ret < 0)
	{
		
		MYTRACE(LOG_LEVEL_ERROR, "get all disk error");
	}
	
}

/*******************************************************************************
* 函数名称  : GetDiskInfo
* 函数描述  : N/A
* 返回值    : int
* 备注      : N/A
* 修改日期     修改人   修改内容
* -----------------------------------------------
* 2015/10/09   王全宝   新建
*******************************************************************************/
int CDirectDiskInfo::GetDiskInfo()
{
 	int ret = 0;

	string strdisk;
	for (int i = 0; i < m_vecAllDisk.size(); i++)
	{
		tDiskInfo* diskinfo = new tDiskInfo;
		if (NULL == diskinfo)
		{
			MYTRACE(LOG_LEVEL_ERROR, "new tDiskInfo error, diskinfo is NULL");
			m_intRetCode = -1
			return -1;
		}
		strdisk = m_vecAllDisk[i];

		diskinfo->m_strSymbol = strdisk;

		ret = GetRaidName(diskinfo, strdisk);
		if (ret != 0)
		{
			MYTRACE(LOG_LEVEL_ERROR, "get raid name error:%s", strdisk.c_str());
			m_intRetCode = -1;
		}

		 ret = GetDiskWWN(diskinfo, strdisk);
		 if (ret != 0)
		 {
		 	MYTRACE(LOG_LEVEL_ERROR, "get disk WWN number error:%s", strdisk.c_str());
			m_intRetCode = -1;
		 }
		
		 ret = GetDiskType(diskinfo,strdisk);
		 if (ret != 0)
		{
		 	MYTRACE(LOG_LEVEL_ERROR, "get disk type error:%s", strdisk.c_str());
			m_intRetCode = -1;
		 }
		
		 ret = GetDiskCapacity(diskinfo, strdisk);
		 if (ret != 0)
		 {
		 	MYTRACE(LOG_LEVEL_ERROR, "get disk capacity error:%s", strdisk.c_str());
			m_intRetCode = -1;
		 }

		m_mapDiskInfo[strdisk] = diskinfo;
	}
    return ret;
}



/*******************************************************************************
* * 函数名称  : IsSasDisk
* * 函数描述  : 判断磁盘是否为SAS盘
* * 输入参数  : string&   strdisk: 磁盘的设备名称
* * 输出参数  : N/A
* * 返回值    : ture/false
* * 备注      : N/A
* * 修改日期     修改人   修改内容
* * -----------------------------------------------
* * 2015/10/16    郭家勇   新建
* *******************************************************************************/
bool CDirectDiskInfo::IsSasDisk(string& strdisk)
{

	return true;
}

/*******************************************************************************
* * 函数名称  : GetRaidName()
* * 函数描述  : 获取Raid组的名称
* * 输入参数  : string& strdisk: 磁盘的设备名称
* * 输出参数  : tDiskInfo* pDiskInfo
* * 返回值    : 0:success  other:failed
* * 备注      : N/A
* * 修改日期     修改人   修改内容
* * -----------------------------------------------
* * 2015/10/16    郭家勇   新建
* *******************************************************************************/
int CDirectDiskInfo::GetRaidName(tDiskInfo *pDiskInfo, string& strdisk)
{
	int ret = 0;

	string disk = strdisk.substr(5, 10);
	char get_raidname_cmd[128] = {};
	sprintf(get_raidname_cmd,
		"mdadm --detail  /dev/` cat /proc/mdstat | grep  %s| awk '{print $1}'` 2> /dev/null | grep Name | awk   '{print $3}' ", 
		disk.c_str());
	ret = syscmd_result(get_raidname_cmd, pDiskInfo->m_strRaidName);
	if (ret < 0)
	{
		MYTRACE(LOG_LEVEL_ERROR, "get raid name error:%s", get_raidname_cmd);
		return ret;
	}
	if (!pDiskInfo->m_strRaidName.empty())
	{
		pDiskInfo->m_strInRaid = "yes";
	}
	else
	{
		pDiskInfo->m_strInRaid = "no";
		pDiskInfo->m_strRaidName = "N/A";
	}

	return ret;
}

/*******************************************************************************
* * 函数名称  : GetDiskCapacity()
* * 函数描述  : 获取磁盘的容量
* * 输入参数  : string strdisk: 磁盘的设备名称
* * 输出参数  : tDiskInfo* pDiskInfo
* * 返回值    : 0:success  other:failed
* * 备注      : N/A
* * 修改日期     修改人   修改内容
* * -----------------------------------------------
* * 2015/10/16    郭家勇   新建
* *******************************************************************************/
int CDirectDiskInfo::GetDiskCapacity(tDiskInfo *pDiskInfo, string& strdisk)
{
	int ret = 0;

	char get_capacity_cmd[128] = {};
	sprintf(get_capacity_cmd, "fdisk -l 2>/dev/null | grep 'Disk %s' | awk -F ':|,' '{print $2}'", strdisk.c_str());
	ret = syscmd_result(get_capacity_cmd, pDiskInfo->m_strCapacity);
	if (ret < 0)
	{
		MYTRACE(LOG_LEVEL_ERROR, "get disk capacity error:%s", get_capacity_cmd);
		return ret;
	}
	return ret;
}

/*******************************************************************************
* * 函数名称  : GetDiskWWN()
* * 函数描述  : 获取磁盘的WWN号
* * 输入参数  : string strdisk: 磁盘的设备名称
* * 输出参数  : tDiskInfo* pDiskInfo
* * 返回值    : 0:success  other:failed
* * 备注      : N/A
* * 修改日期     修改人   修改内容
* * -----------------------------------------------
* * 2015/10/16    郭家勇   新建
* *******************************************************************************/
int CDirectDiskInfo::GetDiskWWN(tDiskInfo *pDiskInfo, string& strdisk)
{
	int ret = 0;
	char get_wwn_cmd[128] = {};
	string disk = strdisk.substr(5, 10);
	sprintf(get_wwn_cmd, "ls -l /dev/disk/by-id | grep %s | grep wwn | awk '{print $9}'", disk.c_str());
	ret = syscmd_result(get_wwn_cmd, pDiskInfo->m_strWWN);
	if (ret < 0)
	{
		MYTRACE(LOG_LEVEL_ERROR, "get disk WWN number error:%s", get_wwn_cmd);
		return ret;
	}

	return ret;
}

/*******************************************************************************
* * 函数名称  : GetDiskType()
* * 函数描述  : 获取磁盘的类型
* * 输入参数  : string strdisk: 磁盘的设备名称
* * 输出参数  : tDiskInfo* pDiskInfo
* * 返回值    : 0:success  other:failed
* * 备注      : N/A
* * 修改日期     修改人   修改内容
* * -----------------------------------------------
* * 2015/10/16    郭家勇   新建
* *******************************************************************************/
int CDirectDiskInfo::GetDiskType(tDiskInfo *pDiskInfo, string& strdisk)
{
	int ret = 0;
// 	string get_type_cmd = "smartctl -i  | grep  SuperSSpeed >/dev/null; if [[ $? == 0 ]];then echo SSD ; else echo HDD ;fi";
// 	get_type_cmd.insert(12, strdisk);
// 	vector<std::string> type_vect;
	char get_type_cmd[128] = {};
	sprintf(get_type_cmd, "smartctl -i %s | grep  SuperSSpeed >/dev/null; if [[ $? == 0 ]];then echo SSD ; else echo HDD ;fi", strdisk.c_str());
	ret = syscmd_result(get_type_cmd, pDiskInfo->m_strDiskType);
	if (ret < 0)
	{
		MYTRACE(LOG_LEVEL_ERROR, "get disk type error: %s", get_type_cmd);
		return ret;
	}

	return ret;
}

/*******************************************************************************
* * 函数名称  : GetPciAddrMap
* * 函数描述  : 获取服务器的“pci地址”和槽号的对应关系
* * 输入参数  : string strmode: 服务器型号
* * 输出参数  : map<string, string>& pcimap
* * 返回值    : N/A
* * 备注      : N/A
* * 修改日期     修改人   修改内容
* * -----------------------------------------------
* * 2015/10/17    郭家勇   新建
* *******************************************************************************/
int CDirectDiskInfo::GetPciAddrMap(string& strmode, std::map<string, string>& pcimap)
{
	int ret = 0;
	Json::Reader reader;
	Json::Value root;
	string modepath = "/usr/local/etc/gemfs/mode/";
	string modename = modepath + strmode;
	const char* filename = modename.c_str();

	std::ifstream is;
	is.open(filename);
	if (!is)
	{
		MYTRACE(LOG_LEVEL_ERROR, "open %s mode file error", filename);
		ret = -1;
		return ret;
	}

	if (reader.parse(is, root, false)) {
		int arr_size = root["pciarr"].size();
		for (unsigned int i = 0; i < arr_size; i++) {
			Json::Value val_pciaddr = root["pciarr"];
			string pciaddr = val_pciaddr[i]["pciaddr"].asString();
			string slotnum = val_pciaddr[i]["slotnum"].asString();
	
			pcimap[pciaddr] = slotnum;
		}
	}
	is.close();
	return ret;
}

/*******************************************************************************
* 函数名称  : DiskInfoToJson
* 函数描述  : N/A
* 参数      : string & strJson
* 返回值    : int
* 备注      : N/A
* 修改日期     修改人   修改内容
* -----------------------------------------------
* 2015/10/09   王全宝   新建
*******************************************************************************/
int CDirectDiskInfo::DiskInfoToJson(string& strJson)
{
	Json::Value root;
	root["raid_type"] = Json::Value(m_strRaidtype);
	root["retcod"] = Json::Value(m_intRetCode);

	map<string, tDiskInfo*>::iterator it;
	for (it = m_mapDiskInfo.begin(); it != m_mapDiskInfo.end(); it++)
	{

			Json::Value slot;
			Json::Value info;
			info["in_raid"] = Json::Value(it->second->m_strInRaid);
			info["raid_name"] = Json::Value(it->second->m_strRaidName);
			info["WWN"] = Json::Value(it->second->m_strWWN);
			info["symbol"] = Json::Value(it->second->m_strSymbol);
			info["type"] = Json::Value(it->second->m_strDiskType);
			info["capacity"] = Json::Value(it->second->m_strCapacity);
			slot["disk"] = Json::Value(info);
			root["disks"].append(slot);

	}

		Json::FastWriter writer;
		strJson = writer.write(root).c_str();

    return 0;
}