#pragma once

#include "sniffer.h"
#include <string>
#include <set>
using namespace std;

struct PortInfo;

class CMyAnalysiser
{
public:
	CMyAnalysiser(void);
	~CMyAnalysiser(void);

	static CMyAnalysiser * GetInstance();

public:
	bool	OnPackageCome(int nPackageType, PacketInformation* pi, RAW_PACKET* pPacket);
	
	void	AddToIgnoreSrcIp(const char * ip);
	void	AddToIgnoreDestIp(const char * ip);
	
	bool	IsSrcIpInIgnore(const char * ip);
	bool	IsDstIpInIgnore(const char * ip);

	void	SetMonitorProcess(const char *);

	void	SetPackageType(bool bTcp, bool bUdp){m_bTcp = bTcp; m_bUdp = bUdp;}
	void	SetPackageDir(bool bSend, bool bRecv){m_bSend = bSend; m_bRecv = bRecv;}
	void	SetLocalAddr(const char * addr){m_myAddr = addr;}

protected:
	static CMyAnalysiser * m_pInstance;
	set<string>				m_ignoreDestIp;
	set<string>				m_ignoreSrcIp;

	set<DWORD>				m_monitorPorts_tcp;
	set<DWORD>				m_monitorPorts_udp;

	bool					m_bMonitorPort;
	bool					m_bSend;
	bool					m_bRecv;
	bool					m_bTcp;
	bool					m_bUdp;
	string					m_myAddr;
};

