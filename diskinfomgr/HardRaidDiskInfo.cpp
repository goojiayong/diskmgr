/*******************************************************************************
*   �ļ����� : HardRaidDiskInfo.cpp
*   ��    �� : ��ȫ��
*   ����ʱ�� : 2015/10/9   10:31
*   �ļ����� : 
*   ��Ȩ���� : Copyright (C) 2014-2015 ���ϰ���Ƽ����޹�˾
*   �޸���ʷ : 2015/10/9   1.00 ��ʼ�汾
*******************************************************************************/

#include "HardRaidDiskInfo.h"



/*******************************************************************************
* ��������  : CHardRaidDiskInfo
* ��������  : N/A
* ����ֵ    : 
* ��ע      : N/A
* �޸�����     �޸���   �޸�����
* -----------------------------------------------
* 2015/10/09   ��ȫ��   �½�
*******************************************************************************/
CHardRaidDiskInfo::CHardRaidDiskInfo()
{
}


/*******************************************************************************
* ��������  : ~CHardRaidDiskInfo
* ��������  : N/A
* ����ֵ    : 
* ��ע      : N/A
* �޸�����     �޸���   �޸�����
* -----------------------------------------------
* 2015/10/09   ��ȫ��   �½�
*******************************************************************************/
CHardRaidDiskInfo::~CHardRaidDiskInfo()
{
}

/*******************************************************************************
* ��������  : Init
* ��������  : N/A
* ����      : int argc
* ����      : char * argv[]
* ����ֵ    : void
* ��ע      : N/A
* �޸�����     �޸���   �޸�����
* -----------------------------------------------
* 2015/10/09   ��ȫ��   �½�
*******************************************************************************/
void CHardRaidDiskInfo::Init(int argc, char *argv[])
{
	m_strRaidtype = "hard_raid";
	m_intRetCode = 0;
}

/*******************************************************************************
* ��������  : GetDiskInfo
* ��������  : N/A
* ����ֵ    : int
* ��ע      : N/A
* �޸�����     �޸���   �޸�����
* -----------------------------------------------
* 2015/10/09   ��ȫ��   �½�
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
* ��������  : DiskInfoToJson
* ��������  : N/A
* ����      : string & strJson
* ����ֵ    : int
* ��ע      : N/A
* �޸�����     �޸���   �޸�����
* -----------------------------------------------
* 2015/10/09   ��ȫ��   �½�
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
* ��������  : SingleAdapter()
* ��������  : ��RAID��������»�ȡ������Ϣ
* ����      : N/A
* ����ֵ    : int
* ��ע      : N/A
* �޸�����     �޸���   �޸�����
* -----------------------------------------------
* 2015/10/23   ������   �½�
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
* ��������  : MoreAdapter()
* ��������  : ��RAID��������»�ȡ������Ϣ
* ����      : int adpnum RAID��������
* ����ֵ    : int
* ��ע      : N/A
* �޸�����     �޸���   �޸�����
* -----------------------------------------------
* 2015/10/23   ������   �½�
*******************************************************************************/
int CHardRaidDiskInfo::MoreAdapter(int adapnum)
{
	cout << "Function of unrealized" << endl;
}
