#pragma once

#include "sniffer.h"
#include <string>
#include <map>
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
	map<DWORD,bool> * GetPortMap(bool bUdp){return bUdp ? &m_monitorPorts_udp : &m_monitorPorts_tcp; }

	void	SetPackageType(bool bTcp, bool bUdp){m_bTcp = bTcp; m_bUdp = bUdp;}
	void	SetPackageDir(bool bSend, bool bRecv){m_bSend = bSend; m_bRecv = bRecv;}
	void	SetLocalAddr(const char * addr){m_myAddr = addr;}

	void	DispBuffer(RAW_PACKET* pPacket, char * pbuf, int & nsize);

protected:
	static CMyAnalysiser * m_pInstance;
	set<string>				m_ignoreDestIp;
	set<string>				m_ignoreSrcIp;

	map<DWORD,bool>			m_monitorPorts_tcp;
	map<DWORD,bool>			m_monitorPorts_udp;

	bool					m_bMonitorPort;
	bool					m_bSend;
	bool					m_bRecv;
	bool					m_bTcp;
	bool					m_bUdp;
	string					m_myAddr;
	char					m_dispBuffs[1024];
};

