#include "stdafx.h"
#include "MyAnalysiser.h"
#include "Protocol.h"

#include <string>
using namespace std;

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

	if (m_justViewSrcPort.size())
	{
		if (!IsSrcPortInJustView(pi->SourcePort))
		{
			return false;
		}
	}

	if (m_justViewDstPort.size())
	{
		if (!IsDstPortInJustView(pi->DestinationPort))
		{
			return false;
		}
	}

	if (IsDstIpInIgnore(pi->DestinationAddr))
	{
		return false;
	}

	if (IsSrcIpInIgnore(pi->SourceAddr))
	{
		return false;
	}

	if (IsDstPortInIgnore(pi->DestinationPort))
	{
		return false;
	}

	if (IsSrcPortInIgnore(pi->SourcePort))
	{
		return false;
	}

	bool btcp = IsTcpPackage(pPacket);
	bool budp = btcp ? false : IsUdpPackage(pPacket);

	bool bsendmode = m_myAddr == pi->SourceAddr;
	bool brecvmode = m_myAddr == pi->DestinationAddr;

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
		if (m_bSend && !bsendmode)
		{
			if (!m_bRecv)
			{
				return false;
			}
		}

		if (m_bRecv && !brecvmode)
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
			if (bsendmode)
			{
				int port1 = atoi(pi->SourcePort);
				if (!m_monitorPorts_udp.count(port1))
				{
					return false;
				}
				else
				{
					if (!m_monitorPorts_udp[port1])
					{
						return false;
					}
				}
			}
			else if (brecvmode)
			{
				int port1 = atoi(pi->DestinationPort);
				if (!m_monitorPorts_udp.count(port1))
				{
					return false;
				}
				else
				{
					if (!m_monitorPorts_udp[port1])
					{
						return false;
					}
				}
			}
			else
			{
				return false;
			}
		}
		else if (btcp)
		{
			if (bsendmode)
			{
				int nport = atoi(pi->SourcePort);
				if (!m_monitorPorts_tcp.count(nport) || !m_monitorPorts_tcp[nport])
				{
					return false;
				}
			}
			else if (brecvmode)
			{
				int nport = atoi(pi->DestinationPort);
				if (!m_monitorPorts_tcp.count(nport) || !m_monitorPorts_tcp[nport])
				{
					return false;
				}
			}
			else
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

void CMyAnalysiser::hextostr(char *ptr,unsigned char *buf,int len)
{
	if (len > 999)
	{
		len = 999;
	}
	for(int i = 0; i < len; i++)
	{
		sprintf(ptr, "%02x",buf[i]);
		ptr += 2;
	}
}

RtmpPacket::RtmpDataTypes CMyAnalysiser::OnTcp(unsigned char * pbuf,	int nSize)
{
	RtmpPacket::RtmpDataTypes rdt = GetRtmpPacketType(pbuf);
	return rdt;
}


//utf±àÂë×ª»»Îªansi±àÂë 
string Utf82Ansi(const char* srcCode)
{	
	int srcCodeLen=0;
	srcCodeLen=MultiByteToWideChar(CP_UTF8,NULL,srcCode,strlen(srcCode),NULL,0);
	wchar_t* result_t=new wchar_t[srcCodeLen+1];
	MultiByteToWideChar(CP_UTF8,NULL,srcCode,strlen(srcCode),result_t,srcCodeLen);
	result_t[srcCodeLen]='/0';
	srcCodeLen=WideCharToMultiByte(CP_ACP,NULL,result_t,wcslen(result_t),NULL,0,NULL,NULL);
	char* result=new char[srcCodeLen+1];
	WideCharToMultiByte(CP_ACP,NULL,result_t,wcslen(result_t),result,srcCodeLen,NULL,NULL);
	result[srcCodeLen]='/0';
	string srcAnsiCode="";
	srcAnsiCode=(string)result;
	delete result_t;
	delete result;
	return srcAnsiCode;
}

bool CMyAnalysiser::SaveBuffer(char * sfile, map<int, RAW_PACKET* > * pPacketMap, int noffset, int ndatasize)
{
	auto iter = pPacketMap->begin();
	RAW_PACKET* pPacket = iter->second;
	FILE * fp = fopen(sfile, "wb");
	if (fp)
	{
		int nsize = ndatasize < 0 ? pPacket->PktHeader.len - noffset: 
			pPacket->PktHeader.len  - noffset < ndatasize ? pPacket->PktHeader.len  - noffset: ndatasize;

		unsigned char * pbuf = pPacket->pPktData;
		if (IsUdpPackage(pPacket))
		{
			pbuf = pPacket->pPktData + 0x2A;
			nsize = pPacket->PktHeader.len - 0x2A;
			nsize -= noffset;
			nsize = ndatasize < 0 ? nsize : 
				nsize < ndatasize ? nsize : ndatasize;
		}
		else if (IsTcpPackage(pPacket))
		{
			pbuf = pPacket->pPktData + 0x36;
			nsize = pPacket->PktHeader.len - 0x36;
			nsize -= noffset;
			nsize = ndatasize < 0 ? nsize : 
				nsize < ndatasize ? nsize : ndatasize;
		}

		fwrite(&pbuf[noffset], 1, nsize, fp);
		iter++;
		for (; iter != pPacketMap->end(); iter++)
		{
			fwrite(iter->second->pPktData, 1, iter->second->PktHeader.len, fp);
		}
		fclose(fp);

		return true;
	}

	return false;
}

bool CMyAnalysiser::SaveBuffer(char * sfile, RAW_PACKET* pPacket, int noffset, int ndatasize)
{
	FILE * fp = fopen(sfile, "wb");
	if (fp)
	{
		int nsize = ndatasize < 0 ? pPacket->PktHeader.len - noffset: 
			pPacket->PktHeader.len  - noffset < ndatasize ? pPacket->PktHeader.len  - noffset: ndatasize;

		unsigned char * pbuf = pPacket->pPktData;
		if (IsUdpPackage(pPacket))
		{
			pbuf = pPacket->pPktData + 0x2A;
			nsize = pPacket->PktHeader.len - 0x2A;
			nsize -= noffset;
			nsize = ndatasize < 0 ? nsize : 
				nsize < ndatasize ? nsize : ndatasize;
		}
		else if (IsTcpPackage(pPacket))
		{
			pbuf = pPacket->pPktData + 0x36;
			nsize = pPacket->PktHeader.len - 0x36;
			nsize -= noffset;
			nsize = ndatasize < 0 ? nsize : 
				nsize < ndatasize ? nsize : ndatasize;
		}
		
		fwrite(&pbuf[noffset], 1, nsize, fp);
		fclose(fp);

		return true;
	}

	return false;
}

bool CMyAnalysiser::DispBuffer(RAW_PACKET* pPacket, char * _pbuf, int & nsize)
{
	bool bWillbreak = false;
	int nzInput = nsize - 1;	
	if (IsUdpPackage(pPacket))
	{
		unsigned char * pbuf = pPacket->pPktData + 0x2A;
		nsize = pPacket->PktHeader.len - 0x2A;
		hextostr(_pbuf, pbuf, nsize < nzInput ? nsize : nzInput);
	}
	else if (IsTcpPackage(pPacket))
	{
		unsigned char * pbuf = pPacket->pPktData + 0x36;
		nsize = pPacket->PktHeader.len - 0x36;
		if (nsize >= 8)
		{
			RtmpPacket::RtmpDataTypes rdt = OnTcp(pbuf, nsize);
			
			string sJson;
			char * pSt = "";
			//sprintf_s(St, "0x%x:", rdt);
			switch(rdt)
			{
			case RtmpPacket::Video:pSt = "Video:";break;
			case RtmpPacket::Handshake:pSt = "Handshake:";break;
			case RtmpPacket::ChunkSize:pSt = "ChunkSize:";break;
			case RtmpPacket::Ping:pSt = "Ping:";break;
			case RtmpPacket::ServerBandwidth:pSt = "SvrBandwidth:";break;
			case RtmpPacket::ClientBandwidth:pSt = "ClentBandwidth:";break;
			case RtmpPacket::Audio:pSt = "Audio:";break;
			case RtmpPacket::Notify:pSt = "Notify:";break;
			case RtmpPacket::Invoke:pSt = "Invoke:";break;
			case RtmpPacket::AggregateMessage:pSt = "AggregateMsg:";break;
			case RtmpPacket::Unknown:pSt = ":";break;
			}

			if (rdt == RtmpPacket::Unknown)
			{
				bool bJson = ((pbuf[0] & 0xc0) >> 6) == 2 && pbuf[4] == '{';
				if (bJson)
				{
					char jsoncode[500] = {0};
					memcpy(&jsoncode[0], &pbuf[4], nsize - 4 > nzInput ? nzInput : nsize - 4);
					sJson = jsoncode;//Utf82Ansi(jsoncode);
					pSt = (char*)sJson.c_str();
				}
				else
				{
					bJson = pbuf[0]==0x81&&pbuf[1]==0x2f&&pbuf[2]=='{';
					if (bJson)
					{
						char jsoncode[500] = {0};
						memcpy(&jsoncode[0], &pbuf[2], nsize - 2 > nzInput ? nzInput : nsize - 2);
						sJson = jsoncode;//Utf82Ansi(jsoncode);
						pSt = (char*)sJson.c_str();
					}
				}

				if (bJson)
				{
					char* pfind = strstr((char*)&pbuf[2], "{\"pack_type\":5,");
					if (pfind)
					{
						bWillbreak = true;
					}
				}
			}
			strcat(_pbuf, pSt);
		}
		int n = strlen(_pbuf);
		nzInput -= n;
		hextostr(&_pbuf[n], pbuf, nsize < nzInput ? nsize : nzInput);
	}

	return bWillbreak;
}

bool CMyAnalysiser::IsDstIpInIgnore(const char * ip)
{
	if (m_ignoreDestIp.count(ip))
	{
		return true;
	}
	return false;
}

bool CMyAnalysiser::SetMonitorProcess(const char * processname)
{
	if (strlen(processname) < 1)
	{
		m_bMonitorPort = false;
		m_monitorPorts_tcp.clear();
		m_monitorPorts_udp.clear();
		return true;
	}

	DWORD dwPid = 0;
	if (strstr(processname, "pid:") != 0)
	{
		dwPid = atoi(&processname[4]);
	}

	m_bMonitorPort = true;
	TcpOrUdp tp = TcpType;
	{
		DWORD ports[100] = {0};
		if (dwPid)
		{
			GetAllPortByProcessId(tp, dwPid, ports, 100);
		}
		else
		{
			GetAllPortByProcessByName(tp, processname, ports, 100);
		}	
		m_monitorPorts_tcp.clear();
		for (int i = 0; i < 100; i++)
		{
			if (ports[i] < 1)
			{
				break;
			}
			m_monitorPorts_tcp[ports[i]] = true;
		}
	}
	
	{
		tp = UdpType;
		DWORD ports[100] = {0};
		if (dwPid)
		{
			GetAllPortByProcessId(tp, dwPid, ports, 100);
		}
		else
		{
			GetAllPortByProcessByName(tp, processname, ports, 100);
		}
		m_monitorPorts_udp.clear();
		for (int i = 0; i < 100; i++)
		{
			if (ports[i] < 1)
			{
				break;
			}
			m_monitorPorts_udp[ports[i]] = true;
		}
	}

	return m_monitorPorts_tcp.size() + m_monitorPorts_udp.size() > 0;
}

void CMyAnalysiser::AddToIgnoreDestIp(const char * ip)
{
	m_ignoreDestIp.insert(ip);
}

bool CMyAnalysiser::IsSrcPortInJustView(const char * nPort)
{
	if (m_justViewSrcPort.count(nPort))
	{
		return true;
	}
	return false;
}

bool CMyAnalysiser::IsDstPortInJustView(const char * nPort)
{
	if (m_justViewDstPort.count(nPort))
	{
		return true;
	}
	return false;
}

bool CMyAnalysiser::IsSrcPortInIgnore(const char * nPort)
{
	if (m_ignoreSrcPort.count(nPort))
	{
		return true;
	}
	return false;
}

bool CMyAnalysiser::IsDstPortInIgnore(const char * nPort)
{
	if (m_ignoreDestPort.count(nPort))
	{
		return true;
	}
	return false;
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

void CMyAnalysiser::AddToIgnoreSrcPort(const char * nPort)
{
	m_ignoreSrcPort.insert(nPort);
}

void CMyAnalysiser::AddToIgnoreDestPort(const char * nPort)
{
	m_ignoreDestPort.insert(nPort);
}

void CMyAnalysiser::ResetJustShow()
{
	m_justViewSrcPort.clear();
	m_justViewDstPort.clear();
}

void CMyAnalysiser::AddToShowOnlySrcPort(const char * nPort)
{
	m_justViewSrcPort.insert(nPort);
}

void CMyAnalysiser::AddToShowOnlyDestPort(const char * nPort)
{
	m_justViewDstPort.insert(nPort);
}