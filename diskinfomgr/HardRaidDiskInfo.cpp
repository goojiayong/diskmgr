/*******************************************************************************
*   文件名称 : HardRaidDiskInfo.cpp
*   作    者 : 王全宝
*   创建时间 : 2015/10/9   10:31
*   文件描述 : 
*   版权声明 : Copyright (C) 2014-2015 湖南安存科技有限公司
*   修改历史 : 2015/10/9   1.00 初始版本
*******************************************************************************/

#include "HardRaidDiskInfo.h"



/*******************************************************************************
* 函数名称  : CHardRaidDiskInfo
* 函数描述  : N/A
* 返回值    : 
* 备注      : N/A
* 修改日期     修改人   修改内容
* -----------------------------------------------
* 2015/10/09   王全宝   新建
*******************************************************************************/
CHardRaidDiskInfo::CHardRaidDiskInfo()
{
}


/*******************************************************************************
* 函数名称  : ~CHardRaidDiskInfo
* 函数描述  : N/A
* 返回值    : 
* 备注      : N/A
* 修改日期     修改人   修改内容
* -----------------------------------------------
* 2015/10/09   王全宝   新建
*******************************************************************************/
CHardRaidDiskInfo::~CHardRaidDiskInfo()
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
void CHardRaidDiskInfo::Init(int argc, char *argv[])
{
	m_strRaidtype = "hard_raid";
	m_intRetCode = 0;
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
int CHardRaidDiskInfo::GetDiskInfo()
{
	int ret = 0;
	ret = SingleAdapter();
	if (ret != 0)
	{
		MYTRACE(LOG_LEVEL_ERROR, "get disk infomation error");
		m_intRetCode = -1;
	}

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
int CHardRaidDiskInfo::DiskInfoToJson(string& strJson)
{
	string x = "x";
	string ary = m_strColumn + x + m_strRow;
	Json::Value root;
	root["array"] = Json::Value(ary);
	root["raid_type"] = Json::Value(m_strRaidtype);
	root["retcod"] = Json::Value(m_intRetCode);

	map<string, tDiskInfo*>::iterator it;
	for (it = m_mapDiskInfo.begin(); it != m_mapDiskInfo.end(); it++)
	{
			Json::Value slot;
			slot["slot_number"] = Json::Value(it->second->m_strSlotNumber);
			Json::Value info;
			info["in_raid"] = Json::Value(it->second->m_strInRaid);
			info["raid_name"] = Json::Value(it->second->m_strRaidName);
			info["WWN"] = Json::Value(it->second->m_strWWN);
			info["symbol"] = Json::Value(it->second->m_strSymbol);
			info["type"] = Json::Value(it->second->m_strDiskType);
			info["capacity"] = Json::Value(it->second->m_strCapacity);
			slot["disk"] = Json::Value(info);
			root["disks"].append(slot);

	Json::FastWriter writer;
	strJson = writer.write(root).c_str();

	return 0;
}

/*******************************************************************************
* 函数名称  : SingleAdapter()
* 函数描述  : 单RAID卡的情况下获取磁盘信息
* 参数      : N/A
* 返回值    : int
* 备注      : N/A
* 修改日期     修改人   修改内容
* -----------------------------------------------
* 2015/10/23   郭家勇   新建
*******************************************************************************/
int CHardRaidDiskInfo::SingleAdapter()
{
	int ret = 0;
	vector<string> alldiskinfo;
	char get_diskinfo_cmd[128] = {};
	sprintf(get_diskinfo_cmd, " MegaCli -pdlist -a0 | grep \"Raw Size\\|Slot Number\\|Media Type\\|WWN\"");
	ret = syscmd_result(get_diskinfo_cmd, alldiskinfo);
	if (ret < 0)
	{
		MYTRACE(LOG_LEVEL_ERROR, "Hard raid get disk infomation error:%s", get_diskinfo_cmd);
		return ret;
	}

	vector<string> raidinfo;
	char get_raidinfo_cmd[128] = {};
	sprintf(get_raidinfo_cmd, " MegaCli -cfgdsply -a0 | grep  -e 'Slot Number' -e 'Name' | grep -v \"Product Name\"");
	ret = syscmd_result(get_raidinfo_cmd, raidinfo);
	if (ret < 0)
	{
		MYTRACE(LOG_LEVEL_ERROR, "Hard raid get disk infomation error:%s", get_raidinfo_cmd);
		return ret;
	}
	
	for (int i = 0; i < alldiskinfo.size(); i++)
	{
		if (alldiskinfo[i].find("Slot Number") != string::npos)
		{
			tDiskInfo* diskinfo = new tDiskInfo;
			string slot = alldiskinfo[i].substr(alldiskinfo[i].find(":")+2);
			diskinfo->m_strSlotNumber = slot;
			diskinfo->m_strSymbol = "N/A";

			string wwnstr = "wwn";
			string wwnnum = alldiskinfo[i + 1].substr(5);
			diskinfo->m_strWWN = wwnstr + wwnnum;
			
			string capa = alldiskinfo[i + 2].substr(10,alldiskinfo[i+2].find("B "));
			diskinfo->m_strCapacity = capa;

			if (alldiskinfo[i+3].find("Hard Disk Device") != string::npos )
			{
				diskinfo->m_strDiskType = "HDD";
			}
			else
			{
				diskinfo->m_strDiskType = "SSD";
			}
			diskinfo->m_strInRaid = "no";
			diskinfo->m_strRaidName = "N/A";
				
			m_mapDiskInfo[slot] = diskinfo;

		}
		
	}

	string raidname;
	for (int i = 0; i < raidinfo.size(); i++)
	{
		if (raidinfo[i].find("Name") != string::npos)
		{
			 raidname = raidinfo[i].substr(raidinfo[i].find(":")+1);
			 continue;
		}
		string slot = raidinfo[i].substr(raidinfo[i].find(":") + 2);
		map<string, tDiskInfo*>::iterator  it;
		it = m_mapDiskInfo.find(slot);
		it->second->m_strInRaid = "yes";
		if (raidname != "")
		{
			it->second->m_strRaidName = raidname;
		}

	}
}

/*******************************************************************************
* 函数名称  : MoreAdapter()
* 函数描述  : 单RAID卡的情况下获取磁盘信息
* 参数      : int adpnum RAID卡的数量
* 返回值    : int
* 备注      : N/A
* 修改日期     修改人   修改内容
* -----------------------------------------------
* 2015/10/23   郭家勇   新建
*******************************************************************************/
int CHardRaidDiskInfo::MoreAdapter(int adapnum)
{
	cout << "Function of unrealized" << endl;
}
