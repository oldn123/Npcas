#pragma once
#include "rtmp/RtmpPacket.h"
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
	static void hextostr(char *ptr,unsigned char *buf,int len);
	static int FindData(LPBYTE pData, int nDataSize, LPBYTE pFind, int nFindSize, vector<int>& posArr);
	static int FindData(RAW_PACKET* pPacket, LPBYTE pFind, int nFindSize, vector<int>& posArr);

public:
	bool	OnPackageCome(int nPackageType, PacketInformation* pi, RAW_PACKET* pPacket);
	
	void	AddToIgnoreSrcIp(const char * ip);
	void	AddToIgnoreDestIp(const char * ip);

	void	AddToIgnoreSrcPort(const char * sPort);
	void	AddToIgnoreDestPort(const char * sPort);

	void	ResetJustShow();
	void	AddToShowOnlySrcPort(const char * sPort);
	void	AddToShowOnlyDestPort(const char * sPort);

	bool	IsSrcPortInJustView(const char * sPort);
	bool	IsDstPortInJustView(const char * sPort);

	bool	IsSrcPortInIgnore(const char * sPort);
	bool	IsDstPortInIgnore(const char * sPort);

	bool	IsSrcIpInIgnore(const char * ip);
	bool	IsDstIpInIgnore(const char * ip);

	bool	IsNeedSend(){return m_bSend;}
	bool	IsNeedRecv(){return m_bRecv;}

	bool	SetMonitorProcess(const char *);
	map<DWORD,bool> * GetPortMap(bool bUdp){return bUdp ? &m_monitorPorts_udp : &m_monitorPorts_tcp; }

	void	SetPackageType(bool bTcp, bool bUdp){m_bTcp = bTcp; m_bUdp = bUdp;}
	void	SetPackageDir(bool bSend, bool bRecv){m_bSend = bSend; m_bRecv = bRecv;}
	void	SetLocalAddr(const char * addr){m_myAddr = addr;}

	bool	DispBuffer(RAW_PACKET* pPacket, char * pbuf, int & nsize);
	bool	SaveBuffer(char * sfile, RAW_PACKET* pPacket, int noffset, int ndatasize);
	bool	SaveBuffer(char * sfile, map<int, RAW_PACKET* > * pPacket, int noffset, int ndatasize);
protected:
	RtmpPacket::RtmpDataTypes OnTcp(unsigned char * pbuf,	int nSize);


protected:
	static CMyAnalysiser * m_pInstance;
	set<string>				m_ignoreDestIp;
	set<string>				m_ignoreSrcIp;

	set<string>				m_ignoreDestPort;
	set<string>				m_ignoreSrcPort;

	set<string>				m_justViewDstPort;
	set<string>				m_justViewSrcPort;

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

