#include "stdafx.h"
#include "MyAnalysiser.h"
#include "Protocol.h"

bool IsTcpPackage(RAW_PACKET* prp)
{
	if(((MAC_HEADER *)prp->pPktData)->LengthOrType == 0x0008)
	{
		return ((struct IPV4 *)(prp->pPktData + sizeof(MAC_HEADER)))->Protocol == 6;
	}
	return false;
}

bool IsUdpPackage(RAW_PACKET* prp)
{
	if(((MAC_HEADER *)prp->pPktData)->LengthOrType == 0x0008)
	{
		return ((struct IPV4 *)(prp->pPktData + sizeof(MAC_HEADER)))->Protocol == 17;
	}
	return false;
}


CMyAnalysiser * CMyAnalysiser::m_pInstance = NULL;

CMyAnalysiser::CMyAnalysiser(void)
{
	m_bMonitorPort = false;

	m_bSend	= false;
	m_bRecv	= false;

	m_bTcp = true;
	m_bUdp = true;
}


CMyAnalysiser::~CMyAnalysiser(void)
{
}

CMyAnalysiser * CMyAnalysiser::GetInstance()
{
	if (!m_pInstance)
	{
		m_pInstance = new CMyAnalysiser;
	}
	return m_pInstance;
}

bool CMyAnalysiser::OnPackageCome(int nPackageType, PacketInformation* pi, RAW_PACKET* pPacket)
{
	if (WM_MY_MESSAGE_COMMON != nPackageType &&
		WM_MY_MESSAGE_TCP != nPackageType &&
		WM_MY_MESSAGE_UDP != nPackageType)
	{
		return false;
	}

	if (IsDstIpInIgnore(pi->DestinationAddr))
	{
		return false;
	}

	if (IsSrcIpInIgnore(pi->SourceAddr))
	{
		return false;
	}

	bool btcp = IsTcpPackage(pPacket);
	bool budp = btcp ? false : IsUdpPackage(pPacket);

	if (m_bUdp && !budp)
	{
		if (!m_bTcp)
		{
			return false;
		}	
	}
	if (m_bTcp && !btcp)
	{
		if (!m_bUdp)
		{
			return false;
		}
	}

	if (m_bSend || m_bRecv)
	{
		if (m_bSend && m_myAddr != pi->SourceAddr)
		{
			if (!m_bRecv)
			{
				return false;
			}
		}

		if (m_bRecv && m_myAddr != pi->DestinationAddr)
		{
			if (!m_bSend)
			{
				return false;
			}
		}
	}

	if (m_bMonitorPort)
	{
		if (budp)
		{
			if (!m_monitorPorts_udp.count(atoi(pi->DestinationPort)))
			{
				return false;
			}
		}
		else if (btcp)
		{
			if (!m_monitorPorts_tcp.count(atoi(pi->DestinationPort)))
			{
				return false;
			}
		}	
		else
		{
			return false;
		}
	}

	return true;
}

bool CMyAnalysiser::IsDstIpInIgnore(const char * ip)
{
	if (m_ignoreDestIp.count(ip))
	{
		return true;
	}
	return false;
}

void CMyAnalysiser::SetMonitorProcess(const char * processname)
{
	if (strlen(processname) < 1)
	{
		m_bMonitorPort = false;
		m_monitorPorts_tcp.clear();
		m_monitorPorts_udp.clear();
		return;
	}

	m_bMonitorPort = true;
	TcpOrUdp tp = TcpType;
	{
		DWORD ports[100] = {0};
		GetAllPortByProcessByName(tp, processname, ports, 100);
		m_monitorPorts_tcp.clear();
		for (int i = 0; i < 100; i++)
		{
			if (ports[i] < 1)
			{
				break;
			}
			m_monitorPorts_tcp.insert(ports[i]);
		}
	}
	
	{
		tp = UdpType;
		DWORD ports[100] = {0};
		GetAllPortByProcessByName(tp, processname, ports, 100);
		m_monitorPorts_tcp.clear();
		for (int i = 0; i < 100; i++)
		{
			if (ports[i] < 1)
			{
				break;
			}
			m_monitorPorts_udp.insert(ports[i]);
		}
	}
}

void CMyAnalysiser::AddToIgnoreDestIp(const char * ip)
{
	m_ignoreDestIp.insert(ip);
}

bool CMyAnalysiser::IsSrcIpInIgnore(const char * ip)
{
	if (m_ignoreSrcIp.count(ip))
	{
		return true;
	}
	return false;
}

void CMyAnalysiser::AddToIgnoreSrcIp( const char * ip )
{
	m_ignoreSrcIp.insert(ip);
}
