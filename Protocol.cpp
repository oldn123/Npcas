#include "stdafx.h"
#include "ProtocolanalysisDlg.h"
#include <stdio.h>
#include <sys/types.h>
#include <windows.h>
#include "Protocol.h"
#include "sniffer.h"
#include "pcap.h"

#include "MyAnalysiser.h"

extern HWND g_hWnd;//�����ھ��
extern bool g_StopThread;//�ж��Ƿ�ֹͣ���ػ����ݰ�
EtherHeader g_DisplayEthernet;//��·��Э����Ϣ
//PacketInformation g_packet;	//�������ݰ���Ϣ
arpheader g_DisplayARP;//ARPЭ����Ϣ
IcmpHeader g_DisplayIcmp;//ICMPЭ����Ϣ
IpHeader g_DisplayIP;//IPЭ����Ϣ
TcpHeader g_DisplayTCP;// TCPЭ����Ϣ
UdpHeader g_DisplayUDP;//UDPЭ����Ϣ

//ע���ֽ���

void PostMsg(unsigned int msg, RAW_PACKET *pRawPacket, WPARAM w = 0)
{
	LPARAM lp = (LPARAM)pRawPacket;
	if (CMyAnalysiser::GetInstance()->OnPackageCome(msg, pRawPacket->pPacketInfo, pRawPacket))
	{
		::PostMessage(g_hWnd, msg, w, lp);
	}	
}


// �������񵽵���̫����
int ParseEthernet(const unsigned char* packetdata,RAW_PACKET *pRawPacket)
{
	int NetType;
    MAC_HEADER* phMac;
	unsigned char *MAC;
	if (g_StopThread == TRUE)//ȫ�ֱ����ж�,�˳������߳�
		AfxEndThread(1, 1);
	
 	PacketInformation * pi = new PacketInformation;
 	pRawPacket->pPacketInfo = pi;
	
	//����
	sprintf(pi->SourceMac, "%s", "");
	sprintf(pi->DestinationMac, "%s", "");
	sprintf(pi->NetType, "%s", "");
	sprintf(pi->DestinationAddr, "%s", "");
	sprintf(pi->DestinationPort, "%s", "");
	sprintf(pi->SourceAddr, "%s", "");
	sprintf(pi->SourcePort, "%s", "");

	phMac = (MAC_HEADER *) packetdata;  
	NetType = ntohs(phMac->LengthOrType); //�õ��ϲ�Э���������� 
	MAC=phMac->SrcMacAddr ;
	//��̫��ԴMAC��ַ
	sprintf(g_DisplayEthernet.SourceMac, "%02X:%02X:%02X:%02X:%02X:%02X",*MAC,*(MAC+1),*(MAC+2),*(MAC+3),*(MAC+4),*(MAC+5));
	strcpy(pi->SourceMac,g_DisplayEthernet.SourceMac);
	MAC=phMac->DesMacAddr;
	//��̫��Ŀ��MAC��ַ
	sprintf(g_DisplayEthernet.DestinationMac, "%02X:%02X:%02X:%02X:%02X:%02X",*MAC,*(MAC+1),*(MAC+2),*(MAC+3),*(MAC+4),*(MAC+5));
	strcpy(pi->DestinationMac,g_DisplayEthernet.DestinationMac);
	packetdata = packetdata+sizeof(MAC_HEADER);/* ���IP���ݰ�ͷ����λ�� */
	LPARAM  lp=(LPARAM)pRawPacket;//ǿ��ת��

	if (NetType == 0x0800)
	{
		ParseIp(packetdata,pRawPacket);

		PostMsg(WM_MY_MESSAGE_COMMON, pRawPacket);	
	}
	else
	{
		delete pi;
		delete [] pRawPacket->pPktData;
		delete pRawPacket;
	}

	return 1;

	switch (NetType)
	{
	case 0x0800://ipv4
		sprintf(g_DisplayEthernet.NetType, "%s", "IPv4");
		//��������̫��Ϣ���뵽���������߳�����ϵ��Ϣ������,��ʾ��Э����Ϣ
		PostMsg(WM_MY_MESSAGE_ETHERNET, pRawPacket);
		//����IPЭ�����ݰ�
		ParseIp(packetdata,pRawPacket);

		PostMsg(WM_MY_MESSAGE_COMMON, pRawPacket);	
		return 0;
	case 0x0806://arp
	case 0x8035://rarp
		struct ARP *parp;
		parp=(struct ARP *)packetdata;
		if (ntohs(parp->OperationCode)<3 )//�ж��Ƿ�ΪARP 
		{
			sprintf(g_DisplayEthernet.NetType, "%s", "ARP");
			sprintf(pi->NetType ,"%s","ARP");
		} 
		else//����RARP
		{
			sprintf(g_DisplayEthernet.NetType, "%s", "RARP");
			sprintf(pi->NetType ,"%s","RARP");
		}
		PostMsg(WM_MY_MESSAGE_ETHERNET, pRawPacket);
		//����ARP/RARPЭ����Ϣ
		ParseArp(packetdata,pRawPacket);
		PostMsg(WM_MY_MESSAGE_COMMON, pRawPacket);		
		return 0;
	case 0x8863: //PPPOE�ķ��ֽ׶�
	case 0x8864: //PPPOE�ĻỰ�׶�
		sprintf(g_DisplayEthernet.NetType, "%s", "PPPoE");
		sprintf(pi->NetType, "%s", "PPPoE");
		PostMsg(WM_MY_MESSAGE_ETHERNET, pRawPacket);
		PostMsg(WM_MY_MESSAGE_COMMON, pRawPacket);		
		return 0;
	case 0x86dd://IPV6
		sprintf(g_DisplayEthernet.NetType, "%s", "IPV6");
		sprintf(pi->NetType, "%s", "IPV6");
		PostMsg(WM_MY_MESSAGE_ETHERNET, pRawPacket);
		PostMsg(WM_MY_MESSAGE_COMMON, pRawPacket);	
		return 0;
	default:
		sprintf(g_DisplayEthernet.NetType, "0x%x", NetType);
		sprintf(pi->NetType,  "0x%x", NetType);
		PostMsg(WM_MY_MESSAGE_ETHERNET, pRawPacket);
		PostMsg(WM_MY_MESSAGE_COMMON, pRawPacket);	
		return 0;
	}
	
	return 1;
}

// ���������ARP��Ϣ
void ParseArp(const unsigned char* packetdata,RAW_PACKET* pRawPacket)
{
	unsigned short Protocol;
	unsigned short Hardware;
	unsigned short Operation;
	struct ARP *Arp; 
	MAC_HEADER* pMacHdr = (MAC_HEADER*)pRawPacket->pPktData;
	struct ARP* pARPHdr = (struct ARP *)((BYTE*)pMacHdr+sizeof(MAC_HEADER));
	// �ȶ�arp��Ϣ����ֵ
	sprintf(g_DisplayARP.HardwareLength, "%s", "");
	sprintf(g_DisplayARP.Hardware, "%s", "");
	sprintf(g_DisplayARP.OperationCode, "%s", "");
	sprintf(g_DisplayARP.ProtocolLength, "%s", "");
	sprintf(g_DisplayARP.Protocol, "%s", "");
	sprintf(g_DisplayARP.OperationInformatin, "%s", "");
	sprintf(g_DisplayARP.SourceAddr, "%s", "");
	sprintf(g_DisplayARP.DestinationAddr, "%s", "");
	Arp = (struct ARP *) packetdata;		                   
	Hardware = ntohs(Arp->Hardware);
	Protocol = ntohs(Arp->Protocol);
	Operation = ntohs(Arp->OperationCode);
	sprintf(g_DisplayARP.HardwareLength, "%d", pARPHdr->HardwareLength);
	sprintf(g_DisplayARP.ProtocolLength, "%d", pARPHdr->ProtocolLength);
	sprintf(g_DisplayARP.Hardware, "%d", Hardware);
	sprintf(g_DisplayARP.Protocol, "%d", Protocol);
	sprintf(g_DisplayARP.OperationCode, "%d", Operation);  
	//ARPͷ���ӽڵ㣺ԴIP��ַ
	in_addr ipAddr;
	memcpy(&ipAddr, pARPHdr->scrIpAddr, sizeof(in_addr));
	sprintf(g_DisplayARP.SourceAddr, "%s",inet_ntoa(ipAddr));
	//ARPͷ���ӽڵ㣺Ŀ��IP��ַ
	memcpy(&ipAddr, pARPHdr->destIpAddr, sizeof(in_addr));
	sprintf(g_DisplayARP.DestinationAddr, "%s",inet_ntoa(ipAddr));
	LPARAM  lp=(LPARAM)pRawPacket;//ǿ��ת��

	switch (Operation)
	{
	case 1:
		sprintf(g_DisplayARP.OperationInformatin, "%s", "ARP����");
		PostMsg(WM_MY_MESSAGE_ARP, pRawPacket);
		break;
	case 2:
		sprintf(g_DisplayARP.OperationInformatin, "%s", "ARPӦ��");
		PostMsg(WM_MY_MESSAGE_ARP, pRawPacket);
		break;
	case 3:
		sprintf(g_DisplayARP.OperationInformatin, "%s", "RARP����");
		PostMsg(WM_MY_MESSAGE_ARP, pRawPacket);
		break;
	case 4:
		sprintf(g_DisplayARP.OperationInformatin, "%s", "RARPӦ��");
		PostMsg(WM_MY_MESSAGE_ARP, pRawPacket);
		break;
	default:
		sprintf(g_DisplayARP.OperationInformatin, "%s", "δ֪");
		PostMsg(WM_MY_MESSAGE_ARP, pRawPacket);
		return ;
	}
}

// �������񵽵�IP���ݰ�
void ParseIp(const unsigned char* packetdata,RAW_PACKET* pRawPacket)
{
	struct IPV4* ip;
	unsigned int  HeaderLength;
	unsigned int 	Length;
	unsigned int 	off;
	unsigned char* pIP_data = NULL;
	unsigned int Off;
	unsigned char Tos;
	unsigned short Checksum;
	LPARAM  lp=(LPARAM)pRawPacket;//ǿ��ת��
	ip = (struct IPV4 *) packetdata;//ǿ��ת��ΪIp�ṹ�İ�
	/* �������ֽ�����ת���������ֽ����� */
	Length = ntohs(ip->Length);
	Checksum = ntohs(ip->Checksum);
	HeaderLength = ip->HeaderLength * 4;//�ײ���
	sprintf(g_DisplayIP.Version, "%d", ip->Version);
	sprintf(g_DisplayIP.HeaderLength, "%d", HeaderLength);
	Tos = ip->Tos;
	sprintf(g_DisplayIP.Tos, "%d", Tos);
	/* �������ֽ�����ת���������ֽ����� */
	sprintf(g_DisplayIP.Length, "%d", ntohs(ip->Length));
	sprintf(g_DisplayIP.Ident, "%d", ntohs(ip->Ident));
	/* �������ֽ�����ת���������ֽ����� */
	Off = ntohs(ip->Flags_Offset);
	sprintf(g_DisplayIP.Flags, "%d", (Off >>13) );
	sprintf(g_DisplayIP.Offset, "%d", (Off & 0x1fff) * 8);
	sprintf(g_DisplayIP.TTL, "%d", ip->TTL);
	sprintf(g_DisplayIP.Protocol, "%d", ip->Protocol);
	sprintf(g_DisplayIP.Checksum, "%d", Checksum);
	SOCKADDR_IN addr;
	addr.sin_addr.s_addr=ip->SourceAddr;
	sprintf(g_DisplayIP.SourceAddr, "%s", inet_ntoa(addr.sin_addr));
	addr.sin_addr.s_addr=ip->DestinationAddr;
	sprintf(g_DisplayIP.DestinationAddr, "%s", inet_ntoa(addr.sin_addr));

	if (pRawPacket && pRawPacket->pPacketInfo)
	{
		strcpy(pRawPacket->pPacketInfo->SourceAddr ,g_DisplayIP.SourceAddr);
		strcpy(pRawPacket->pPacketInfo->DestinationAddr ,g_DisplayIP.DestinationAddr);	
	}

	PostMsg(WM_MY_MESSAGE_IP, pRawPacket);

	

	Length =Length-HeaderLength;
	off = ntohs(ip->Flags_Offset);
	if ((off & 0x1fff) == 0)
	{
		pIP_data = (unsigned char *) ip + HeaderLength;//ip���ݲ���,����������ݰ�
		switch (ip->Protocol)
		{
		case 6:
			sprintf(pRawPacket->pPacketInfo->NetType, "%s", "TCP");
			//����TCPЭ����Ϣ
			ParseTcp(pIP_data,pRawPacket);
			break;
		case 17:
			sprintf(pRawPacket->pPacketInfo->NetType, "%s", "UDP");
			//����UDPЭ����Ϣ
			ParseUdp(pIP_data,pRawPacket);
			break;
		case 1:
			sprintf(pRawPacket->pPacketInfo->NetType, "%s", "ICMP");
			//����ICMPЭ����Ϣ
			ParseIcmp(pIP_data,pRawPacket);
			break;
		case 2:
			sprintf(pRawPacket->pPacketInfo->NetType, "%s", "IGMP");
			break;
		default:
			sprintf(pRawPacket->pPacketInfo->NetType, "%s", "--");
			break;
		}
	}
}

//���񵽵�ICMP���ݰ�
void ParseIcmp(const unsigned char* packetdata,RAW_PACKET* pRawPacket)
{
	LPARAM  lp=(LPARAM)pRawPacket;//ǿ��ת��
	struct ICMP* Icmp;
	Icmp = (struct ICMP *) packetdata;
	sprintf(g_DisplayIcmp.type, "%d", Icmp->Type);
	switch (Icmp->Type)
	{
	case 0:
		sprintf(g_DisplayIcmp.information, "%s", "����Ӧ��");
		break;
	case 8:
		sprintf(g_DisplayIcmp.information, "%s", "��������");
		break;
	case 13:
		sprintf(g_DisplayIcmp.information, "%s", "ʱ�������");
		break;
	case 14:
		sprintf(g_DisplayIcmp.information, "%s", "ʱ���Ӧ��");
		break;
	case 17:
		sprintf(g_DisplayIcmp.information, "%s", "��ַ��������");
		break;
	case 18:
		sprintf(g_DisplayIcmp.information, "%s", "��ַ����Ӧ��");
		break;
	default:
		sprintf(g_DisplayIcmp.information, "%s", "ICMP����δ֪");
		break;
	}
	sprintf(g_DisplayIcmp.code, "%d", Icmp->Code);
	sprintf(g_DisplayIcmp.checksum, "%d", ntohs(Icmp->Checksum));
	//������ICMPЭ����Ϣ���뵽���������߳�����ϵ��Ϣ������,��ʾ��Э����Ϣ
	::PostMessage(g_hWnd, WM_MY_MESSAGE_ICMP, 0, lp);
	return;
}
//�������񵽵� TCP����
int ParseTcp(const unsigned char* packetdata,RAW_PACKET* pRawPacket)
{
	LPARAM  lp=(LPARAM)pRawPacket;//ǿ��ת��
	struct TCP* ptcp;
	ptcp = (struct TCP *) packetdata;
	unsigned short SourcePort;
	SourcePort = ntohs(ptcp->SrcPort);
	unsigned short DestinationPort;
	DestinationPort = ntohs(ptcp->DstPort);
	int HeaderLength;
	// hdrlen_flags 4λ�ײ����� + 6λ���� + 6λ��־
	HeaderLength = (ntohs(ptcp->hdrlen_flags)>>12)*4;//����12λ����tcp�ײ��� 6bit
	unsigned int  SequenceNum;
	SequenceNum = ntohl(ptcp->SequenceNum);
	unsigned int  Acknowledgment;
	Acknowledgment = ntohl(ptcp->Acknowledgment);
	unsigned short AdvertisedWindow;
	AdvertisedWindow = ntohs(ptcp->AdvertisedWindow);	
	unsigned short UrgPtr;
	UrgPtr = ntohs(ptcp->UrgPtr);
	unsigned char Flags;
	Flags = ntohs(ptcp->hdrlen_flags)&0x003f;//��־λ ��6λ
	sprintf(g_DisplayTCP.SrcPort, "%d", SourcePort);
	sprintf(g_DisplayTCP.DstPort, "%d", DestinationPort);
// 	strcpy(g_packet.DestinationPort ,g_DisplayTCP.DstPort);
// 	strcpy(g_packet.SourcePort ,g_DisplayTCP.SrcPort);
	sprintf(g_DisplayTCP.SequenceNum, "%u", SequenceNum);
	sprintf(g_DisplayTCP.Acknowledgment, "%u", Acknowledgment);
	sprintf(g_DisplayTCP.HdrLen, "%d", HeaderLength);
	//����4λ�ײ����� + 6λ���� + 6λ��־
	sprintf(g_DisplayTCP.Zero, "%d", (ntohs(ptcp->hdrlen_flags)>>6) & 0x003f );

	if (pRawPacket && pRawPacket->pPacketInfo)
	{
		strcpy(pRawPacket->pPacketInfo->SourcePort ,g_DisplayTCP.SrcPort);
		strcpy(pRawPacket->pPacketInfo->DestinationPort ,g_DisplayTCP.DstPort);	
	}


	char myflags[1024];
	strcpy(myflags, "");
	if (Flags & 0x02)
	{
		strcat(myflags, "SYN ");
	}
	if (Flags & 0x01)
	{
		strcat(myflags, "FIN ");
	}
	if (Flags & 0x04)
	{
		strcat(myflags, "RST ");
	}
	if (Flags & 0x08)
	{
		strcat(myflags, "PSH ");
	}
	if (Flags & 0x10)
	{
		strcat(myflags, "ACK ");
	}
	if (Flags & 0x20)
	{
		strcat(myflags, "URG ");
	}
	sprintf(g_DisplayTCP.Flags, "%s", myflags);
	sprintf(g_DisplayTCP.AdvertisedWindow, "%d", AdvertisedWindow);
	sprintf(g_DisplayTCP.Checksum, "%d", ntohs(ptcp->Checksum));
	sprintf(g_DisplayTCP.UrgPtr, "%d", UrgPtr);
	//������TCP��Ϣ���뵽���������߳�����ϵ��Ϣ������,��ʾ��Э����Ϣ
	::PostMsg(WM_MY_MESSAGE_TCP, pRawPacket);

	//�����Ǻ���HTTPЭ��
	bool bhttp = ParseHttp(pRawPacket);

	return 0;
}
//�����Ƿ񲶻� HTTP
bool ParseHttp(RAW_PACKET* pRawPacket)
{
	bool bfind = false;
	LPARAM  lp=(LPARAM)pRawPacket;//ǿ��ת��
	int find_http = false;
	 /* ���ipͷ */
	struct IPV4 *ih = (struct IPV4 *)(pRawPacket->pPktData+sizeof(MAC_HEADER));
	/*��һ���޷��Ŷ��������������ֽ�˳��ת��Ϊ�����ֽ�˳��,�˴�����Ҫ*/
	int len = ntohs(ih->Length);//����ipͷ�����ݲ���
	char *pip_data = (char *)(ih);
	int n = 0;
	for(;n<len; n++)
	{
		/* http get or post request */
		if(!find_http &&((n+3<len &&strncmp(pip_data+n,"GET",strlen("GET")) ==0 )
			|| (n+4<len && strncmp(pip_data+n,"POST",strlen("POST")) == 0)) )
			find_http = true;
		/* http response */
		if(!find_http && n+8<len && strncmp(pip_data+n,"HTTP/1.1",strlen("HTTP/1.1"))==0)
			find_http = true;
		if(find_http)//�ж��Ƿ��ҵ�http
		{   
			//������HTTP��Ϣ���뵽���������߳�����ϵ��Ϣ������,��ʾ��Э����Ϣ
			PostMsg(WM_MY_MESSAGE_HTTP, pRawPacket, n);//n���ipƫ������http��ͷ
			bfind = true;
			break;
		}
	}

	return bfind;
}
// �������񵽵�UDP���ݣ����ҽ������Ƿ����DNSЭ�������
int ParseUdp(const unsigned char* packetdata,RAW_PACKET* pRawPacket)
{
	
	LPARAM  lp=(LPARAM)pRawPacket;//ǿ��ת��
	struct UDP* udp;
	udp = (struct UDP *) packetdata;
	unsigned short SourcePort; 
	SourcePort = ntohs(udp->SrcPort);	
	unsigned short DestinationPort; 
	DestinationPort = ntohs(udp->DstPort); 
	unsigned short Length;
	Length = ntohs(udp->Length);
	sprintf(g_DisplayUDP.SrcPort, "%d", SourcePort);
	sprintf(g_DisplayUDP.DstPort, "%d", DestinationPort);
// 	strcpy(g_packet.DestinationPort ,g_DisplayUDP.DstPort);
// 	strcpy(g_packet.SourcePort ,g_DisplayUDP.SrcPort);
	sprintf(g_DisplayUDP.Length, "%d", Length);
	sprintf(g_DisplayUDP.Checksum, "%d", ntohs(udp->Checksum));

	if (pRawPacket && pRawPacket->pPacketInfo)
	{
		strcpy(pRawPacket->pPacketInfo->SourcePort ,g_DisplayUDP.SrcPort);
		strcpy(pRawPacket->pPacketInfo->DestinationPort ,g_DisplayUDP.DstPort);	
	}
	
	PostMsg(WM_MY_MESSAGE_UDP, pRawPacket);
	//�Ƿ���DNSЭ�� ͨ���ж϶˿ں��Ƿ�Ϊ 53
	if (SourcePort==53 || DestinationPort==53)
	{
		//������UDP��Ϣ���뵽���������߳�����ϵ��Ϣ������,��ʾ��Э����Ϣ
		PostMsg(WM_MY_MESSAGE_DNS, pRawPacket);
	}


	return 0;
}
