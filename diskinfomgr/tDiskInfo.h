/*******************************************************************************
*   �ļ����� : tDiskInfo.h
*   ��    �� : ��ȫ��
*   ����ʱ�� : 2015/10/9   10:45
*   �ļ����� : 
*   ��Ȩ���� : Copyright (C) 2014-2015 ���ϰ���Ƽ����޹�˾
*   �޸���ʷ : 2015/10/9   1.00 ��ʼ�汾
*******************************************************************************/

#pragma once

#include <string>
using std::string;

//������Ϣ�ṹ����
struct tDiskInfo
{
    string      m_strSN; //����sn
    string      m_strWWN; //wwn
    string      m_strSymbol;  //����id
    string      m_strDiskType; //�������ͣ�SSD��HDD
    string      m_strCapacity; //����
    string      m_strTemperature; //�¶�
	string      m_strInRaid; //�Ƿ�����������RAID
	string      m_strRaidName; //���ڵ�RAID������
	string      m_strSlotNumber; //�������ڵ�����λ��
	string      m_strPciAddress;
};