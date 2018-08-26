// forasm.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "memory.h"

#include "asmfunc.h"

/**
(byte)ecx[0x28] = ?			//与0x40位与操作，可能用于标识

(qword)ecx[0x88] = 			//数据区地址
(dword)ecx[0x90] = 			//数据长度，单位【字节】
(dword)ecx[0x98] =			//待转长度，单位【位】
(dword)ecx[0xA0] = ?		//？长度，单位【位】
*/



/*
	00007FF7610E5B80 | 40:53                    | push rbx                                                          |
	00007FF7610E5B82 | 55                       | push rbp                                                          |
	00007FF7610E5B83 | 56                       | push rsi                                                          |
	00007FF7610E5B84 | 57                       | push rdi                                                          |
	00007FF7610E5B85 | 48:8BF9                  | mov rdi,rcx                                                       |rcx为接收数据区，首字节也做输入使用，本次为   0x80
	00007FF7610E5B88 | 41:8BD9                  | mov ebx,r9d                                                       |r9本次初始为   1 ，实际r9为变量
	00007FF7610E5B8B | 8B4C24 48                | mov ecx,dword ptr ss:[rsp+48]                                     |ss:[rsp+48]    = 0x389 = recv_size * 8 - 7
	00007FF7610E5B8F | 49:8BE8                  | mov rbp,r8                                                        |r8 = recv_data
	00007FF7610E5B92 | 8BF2                     | mov esi,edx                                                       |edx初始为0
	00007FF7610E5B94 | 85C9                     | test ecx,ecx                                                      |
	00007FF7610E5B96 | 0F84 2F020000            | je tslgame.7FF7610E5DCB                                           |
	00007FF7610E5B9C | 8BC2                     | mov eax,edx                                                       |
	00007FF7610E5B9E | 4C:896424 28             | mov qword ptr ss:[rsp+28],r12                                     |
	00007FF7610E5BA3 | 99                       | cdq                                                               |
	00007FF7610E5BA4 | 4C:896C24 30             | mov qword ptr ss:[rsp+30],r13                                     |
	00007FF7610E5BA9 | 83E2 07                  | and edx,7                                                         |
	00007FF7610E5BAC | 4C:897424 38             | mov qword ptr ss:[rsp+38],r14                                     |
	00007FF7610E5BB1 | 44:8D040E                | lea r8d,qword ptr ds:[rsi+rcx]                                    |r8d = edx + rcx = 0 + recv_size * 8 - 7
	00007FF7610E5BB5 | 4C:897C24 40             | mov qword ptr ss:[rsp+40],r15                                     |
	00007FF7610E5BBA | 44:8D2C0B                | lea r13d,qword ptr ds:[rbx+rcx]                                   |r13d = rbx + rcx = r9 + recv_size * 8 - 7
	00007FF7610E5BBE | 83E6 07                  | and esi,7                                                         |
	00007FF7610E5BC1 | 41:BF FF000000           | mov r15d,FF                                                       |r15d初始为0xff
	00007FF7610E5BC7 | 44:8D1402                | lea r10d,qword ptr ds:[rdx+rax]                                   |r10 = 0 + 0
	00007FF7610E5BCB | 8BC3                     | mov eax,ebx                                                       |
	00007FF7610E5BCD | 99                       | cdq                                                               |
	00007FF7610E5BCE | 41:C1FA 03               | sar r10d,3                                                        |
	00007FF7610E5BD2 | 83E2 07                  | and edx,7                                                         |
	00007FF7610E5BD5 | 83E3 07                  | and ebx,7                                                         |
	00007FF7610E5BD8 | 44:8D0C02                | lea r9d,qword ptr ds:[rdx+rax]                                    |
	00007FF7610E5BDC | 41:C1F9 03               | sar r9d,3                                                         |
	00007FF7610E5BE0 | 83F9 08                  | cmp ecx,8                                                         |
	00007FF7610E5BE3 | 8BCE                     | mov ecx,esi                                                       |
	00007FF7610E5BE5 | 0F8F CC000000            | jg tslgame.7FF7610E5CB7                                           |发生跳转---jmp

	00007FF7610E5CB7 | 41:8BC0                  | mov eax,r8d                                                       | ----jmp here
	00007FF7610E5CBA | 2BF3                     | sub esi,ebx                                                       |
	00007FF7610E5CBC | 99                       | cdq                                                               |
	00007FF7610E5CBD | 45:8BF7                  | mov r14d,r15d                                                     |
	00007FF7610E5CC0 | 41:D3E6                  | shl r14d,cl                                                       |
	00007FF7610E5CC3 | 83E2 07                  | and edx,7                                                         |
	00007FF7610E5CC6 | 41:8BC8                  | mov ecx,r8d                                                       |
	00007FF7610E5CC9 | 45:8BE7                  | mov r12d,r15d                                                     |
	00007FF7610E5CCC | 83E1 07                  | and ecx,7                                                         |
	00007FF7610E5CCF | 8D5E 08                  | lea ebx,qword ptr ds:[rsi+8]                                      |ebx = 8-1
	00007FF7610E5CD2 | 41:D3E4                  | shl r12d,cl                                                       |
	00007FF7610E5CD5 | 8D0C02                   | lea ecx,qword ptr ds:[rdx+rax]                                    |
	00007FF7610E5CD8 | 41:8BC5                  | mov eax,r13d                                                      |
	00007FF7610E5CDB | 99                       | cdq                                                               |
	00007FF7610E5CDC | C1F9 03                  | sar ecx,3                                                         |执行后ecx为总字节数
	00007FF7610E5CDF | 83E2 07                  | and edx,7                                                         |
	00007FF7610E5CE2 | 41:2BCA                  | sub ecx,r10d                                                      |
	00007FF7610E5CE5 | 44:8D1C02                | lea r11d,qword ptr ds:[rdx+rax]                                   |r11d 为总‘位’数
	00007FF7610E5CE9 | 41:0FB61429              | movzx edx,byte ptr ds:[r9+rbp]                                    |rbp为接收数据包， r9初始为0
	00007FF7610E5CEE | 41:C1FB 03               | sar r11d,3                                                        |r11d 为字节数
	00007FF7610E5CF2 | 45:2BD9                  | sub r11d,r9d                                                      |r9为0
	00007FF7610E5CF5 | 85F6                     | test esi,esi                                                      |esi为-1，js将跳转
	00007FF7610E5CF7 | 78 0D                    | js tslgame.7FF7610E5D06                                           |跳
	00007FF7610E5CF9 | 41:3BCB                  | cmp ecx,r11d                                                      |
	00007FF7610E5CFC | 44:0F4DD9                | cmovge r11d,ecx                                                   |
	00007FF7610E5D00 | 8BCE                     | mov ecx,esi                                                       |
	00007FF7610E5D02 | D3E2                     | shl edx,cl                                                        |
	00007FF7610E5D04 | EB 29                    | jmp tslgame.7FF7610E5D2F                                          |
	00007FF7610E5D06 | 41:8D43 FF               | lea eax,qword ptr ds:[r11-1]                                      |eax = 字节数-1
	00007FF7610E5D0A | 3BC8                     | cmp ecx,eax                                                       |与总字节数比较
	00007FF7610E5D0C | 44:8BD8                  | mov r11d,eax                                                      |
	00007FF7610E5D0F | 44:0F4DD9                | cmovge r11d,ecx                                                   |条件成立，r11d被重置为总字节数
	00007FF7610E5D13 | 8BCB                     | mov ecx,ebx                                                       |ebx=，此处为7
	00007FF7610E5D15 | D3E2                     | shl edx,cl                                                        |第一个数据<<7
	00007FF7610E5D17 | 41:FFC1                  | inc r9d                                                           |r9++,此处为0
	00007FF7610E5D1A | 44:8BC2                  | mov r8d,edx                                                       |
	00007FF7610E5D1D | 83C3 08                  | add ebx,8                                                         |ebx=7+8=0xf
	00007FF7610E5D20 | 8BCB                     | mov ecx,ebx                                                       |
	00007FF7610E5D22 | 41:0FB61429              | movzx edx,byte ptr ds:[r9+rbp]                                    |取出第二个字节数据
	00007FF7610E5D27 | D3E2                     | shl edx,cl                                                        |第二个数据<<0xf
	00007FF7610E5D29 | 41:03D0                  | add edx,r8d                                                       |第一个数据<<7+第二个数据<<0xf
	00007FF7610E5D2C | C1EA 08                  | shr edx,8                                                         |整体>>8
	00007FF7610E5D2F | 41:0FB6C6                | movzx eax,r14b                                                    |
	00007FF7610E5D33 | 41:FFC1                  | inc r9d                                                           |
	00007FF7610E5D36 | F6D0                     | not al                                                            |
	00007FF7610E5D38 | 44:22F2                  | and r14b,dl                                                       |
	00007FF7610E5D3B | 41:22043A                | and al,byte ptr ds:[r10+rdi]                                      |
	00007FF7610E5D3F | 41:0AC6                  | or al,r14b                                                        |
	00007FF7610E5D42 | 41:88043A                | mov byte ptr ds:[r10+rdi],al                                      |
	00007FF7610E5D46 | 41:FFC2                  | inc r10d                                                          |
	00007FF7610E5D49 | 41:83FB 01               | cmp r11d,1                                                        |
	00007FF7610E5D4D | 76 31                    | jbe tslgame.7FF7610E5D80                                          |
	00007FF7610E5D4F | 41:FFCB                  | dec r11d                                                          |
	00007FF7610E5D52 | 0F1F40 00                | nop dword ptr ds:[rax],eax                                        |
	00007FF7610E5D56 | 6666:0F1F8400 00000000   | nop word ptr ds:[rax+rax],ax                                      |
	00007FF7610E5D60 | 45:0FB60429              | movzx r8d,byte ptr ds:[r9+rbp]                                    |
	00007FF7610E5D65 | 8BCB                     | mov ecx,ebx                                                       |
	00007FF7610E5D67 | 41:D3E0                  | shl r8d,cl                                                        |
	00007FF7610E5D6A | 41:FFC1                  | inc r9d                                                           |
	00007FF7610E5D6D | 41:03D0                  | add edx,r8d                                                       |
	00007FF7610E5D70 | C1EA 08                  | shr edx,8                                                         |
	00007FF7610E5D73 | 41:88143A                | mov byte ptr ds:[r10+rdi],dl                                      |
	00007FF7610E5D77 | 41:FFC2                  | inc r10d                                                          |
	00007FF7610E5D7A | 49:83EB 01               | sub r11,1                                                         |
	00007FF7610E5D7E | 75 E0                    | jne tslgame.7FF7610E5D60                                          |
	00007FF7610E5D80 | 45:3BE7                  | cmp r12d,r15d                                                     | 一层转换完毕
	00007FF7610E5D83 | 74 32                    | je tslgame.7FF7610E5DB7                                           |
	00007FF7610E5D85 | 41:8D45 FF               | lea eax,qword ptr ds:[r13-1]                                      |
	00007FF7610E5D89 | C1E8 03                  | shr eax,3                                                         |
	00007FF7610E5D8C | 41:3BC1                  | cmp eax,r9d                                                       |
	00007FF7610E5D8F | 75 0D                    | jne tslgame.7FF7610E5D9E                                          |
	00007FF7610E5D91 | 45:0FB60429              | movzx r8d,byte ptr ds:[r9+rbp]                                    |
	00007FF7610E5D96 | 8BCB                     | mov ecx,ebx                                                       |
	00007FF7610E5D98 | 41:D3E0                  | shl r8d,cl                                                        |
	00007FF7610E5D9B | 41:03D0                  | add edx,r8d                                                       |
	00007FF7610E5D9E | 41:0FB6043A              | movzx eax,byte ptr ds:[r10+rdi]                                   |
	00007FF7610E5DA3 | 41:0FB6CC                | movzx ecx,r12b                                                    |
	00007FF7610E5DA7 | F6D1                     | not cl                                                            |
	00007FF7610E5DA9 | C1EA 08                  | shr edx,8                                                         |
	00007FF7610E5DAC | 22CA                     | and cl,dl                                                         |
	00007FF7610E5DAE | 41:22C4                  | and al,r12b                                                       |
	00007FF7610E5DB1 | 0AC8                     | or cl,al                                                          |
	00007FF7610E5DB3 | 41:880C3A                | mov byte ptr ds:[r10+rdi],cl                                      |
	00007FF7610E5DB7 | 4C:8B7424 38             | mov r14,qword ptr ss:[rsp+38]                                     |
	00007FF7610E5DBC | 4C:8B6C24 30             | mov r13,qword ptr ss:[rsp+30]                                     |
	00007FF7610E5DC1 | 4C:8B6424 28             | mov r12,qword ptr ss:[rsp+28]                                     |
	00007FF7610E5DC6 | 4C:8B7C24 40             | mov r15,qword ptr ss:[rsp+40]                                     |
	00007FF7610E5DCB | 5F                       | pop rdi                                                           |
	00007FF7610E5DCC | 5E                       | pop rsi                                                           |
	00007FF7610E5DCD | 5D                       | pop rbp                                                           |
	00007FF7610E5DCE | 5B                       | pop rbx                                                           |
	00007FF7610E5DCF | C3                       | ret                                                               |
*/



void DecodeData(unsigned char * pBuf, int nLen, unsigned char * pBuf_dest, int iR9)
{
	if (nLen == 0)
	{
		return;
	}

	ConvertEndByte(pBuf, nLen);

	int nRet = fun_7ff72ebd4f60_readDic(nLen);
	*(int*)pBuf_dest = nRet;

	int nEdx = 0;

	int nTotalBits = nEdx + (nLen * 8 - 7);

	int nOffsetBits = iR9 + (nLen * 8 - 7);

	int nR10 = nEdx + nEdx;

	unsigned char b15 = 0xff;

	/*
	mov eax,ebx                                                       |
	cdq                                                               |
	sar r10d,3                                                        |
	and edx,7                                                         |
	and ebx,7                                                         |
	lea r9d,qword ptr ds:[rdx+rax]                                    |
	sar r9d,3   
	*/
	int nVal = 0;
	{
		nR10 >>= 3;
		int iEax = iR9;
		if (iR9 < 0)
		{
			int n1		= iR9 & 0x7;
			int n2		= -1 & 0x7;
			nVal		= n1 + n2;
		}
		else
		{
			int n1		= iR9 & 0x7;
			int n2		= 0 & 0x7;
			nVal		= n1 + n2;
		}	
		nVal >>= 3;
	}

	if (nLen > 8)
	{
		/*
		mov eax,r8d                                                       | ----jmp here
		sub esi,ebx                                                       |
		cdq                                                               |
		mov r14d,r15d                                                     |
		shl r14d,cl                                                       |
		*/
		int nEax = nTotalBits;
		int nt1 = nEdx & 0x7;
		int nt2 = iR9 & 0x7;
		int nEsi = nt1 - nt2;
		int n14 = b15 << ((unsigned char)nEdx & 0x7);


		/*
		and edx,7                                                         |
		mov ecx,r8d                                                       |

		and ecx,7                                                         |
		lea ebx,qword ptr ds:[rsi+8]                                      |ebx = 8-1
		*/
		int nEdx1 = nTotalBits < 0 ? 0x7 : 0;
		int nEcx1 = nTotalBits & 0x7;
		int nEbx1 = 8 + nEsi;

		/*
		mov r12d,r15d                                                     |

		shl r12d,cl                                                       |
		lea ecx,qword ptr ds:[rdx+rax]                                    |
		mov eax,r13d                                                      |
		cdq                                                               |
		sar ecx,3                                                         |执行后ecx为总字节数
		and edx,7                                                         |
		sub ecx,r10d                                                      |
		*/
		int n12 = b15 << nEcx1;
		nEcx1 = nEdx1 + nEax;
		nEcx1 >>= 3;
		nEcx1 -= nR10;
		int n13 = nOffsetBits;
		nEax = n13;
		nEdx1 = nEax < 0 ? 0x7 : 0;


		/*
		lea r11d,qword ptr ds:[rdx+rax]                                   |r11d 为总‘位’数
		movzx edx,byte ptr ds:[r9+rbp]                                    |rbp为接收数据包， r9初始为0
		sar r11d,3                                                        |r11d 为字节数
		sub r11d,r9d                                                      |r9为0
		test esi,esi                                                      |esi为-1，js将跳转
		*/
		int nR11 = nEax + nEdx1;
		nEdx1 = pBuf[iR9];
		nR11 >>= 3;
		nR11 -= iR9;
		if (nEsi < 0)
		{
			/*
			lea eax,qword ptr ds:[r11-1]                                      |eax = 字节数-1
			cmp ecx,eax                                                       |与总字节数比较
			mov r11d,eax                                                      |
			cmovge r11d,ecx                                                   |条件成立，r11d被重置为总字节数
			mov ecx,ebx                                                       |ebx=，此处为7
			shl edx,cl                                                        |第一个数据<<7
			inc r9d                                                           |r9++,此处为0
			mov r8d,edx                                                       |
			add ebx,8                                                         |ebx=7+8=0xf
			mov ecx,ebx                                                       |
			movzx edx,byte ptr ds:[r9+rbp]                                    |取出第二个字节数据
			shl edx,cl                                                        |第二个数据<<0xf
			add edx,r8d                                                       |第一个数据<<7+第二个数据<<0xf
			shr edx,8                                                         |整体>>8
			*/
			nEdx1 = nR11 - 1;
			nR11 = nEax;
			if (nEax >= nEcx1)
			{
				nR11 = nEcx1;
			}
			nEcx1 = nEbx1;
			nEdx1 <<= (BYTE)nEcx1;
			iR9++;
			int nR8d = nEdx1;
			nEbx1 += 8;
			nEcx1 = nEbx1;

			nEdx1 = pBuf[iR9];
			nEdx1 <<= (BYTE)nEcx1;
			nEdx1 += nR8d;
			nEdx1 >>= 8;


			/*
			movzx eax,r14b                                                    |
			inc r9d                                                           |
			not al                                                            |
			and r14b,dl                                                       |
			and al,byte ptr ds:[r10+rdi]                                      |
			or al,r14b                                                        |
			mov byte ptr ds:[r10+rdi],al                                      |
			inc r10d                                                          |
			*/
			nEax = (BYTE)n14;
			iR9++;
			nEax = ~(nEax & 0xf);
			n14 &= (BYTE)nEdx1;
			nEax &= pBuf_dest[nR10];
			nEax |= (BYTE)n14;
			pBuf_dest[nR10] = (BYTE)nEax;
			nR10++;

			/*
			cmp r11d,1                                                        |
			jbe tslgame.7FF7610E5D80                                          |
			dec r11d                                                          |
7FF7610E5D60:movzx r8d,byte ptr ds:[r9+rbp]                                    |
			mov ecx,ebx                                                       |
			shl r8d,cl                                                        |
			inc r9d                                                           |
			add edx,r8d                                                       |
			shr edx,8                                                         |
			mov byte ptr ds:[r10+rdi],dl                                      |
			inc r10d                                                          |
			sub r11,1                                                         |
			jne tslgame.7FF7610E5D60                                          |
			*/

			if (nR11 > 1)
			{
				nR11--;
				do
				{
					nR8d = pBuf[iR9];
					nEcx1 = nEbx1;
					nR8d <<= nEcx1;
					iR9++;
					nEdx1 += nR8d;
					nEdx1 >>= 8;
					pBuf_dest[nR10] = (BYTE)nEdx1;
					nR10++;
					nR11--;
				}
				while(nR11 != 0);
			}

			/*
			cmp r12d,r15d                                                     | 一层转换完毕
			je tslgame.7FF7610E5DB7                                           |
			lea eax,qword ptr ds:[r13-1]                                      |
			shr eax,3                                                         |
			cmp eax,r9d                                                       |
			jne tslgame.7FF7610E5D9E                                          |
			movzx r8d,byte ptr ds:[r9+rbp]                                    |
			mov ecx,ebx                                                       |
			shl r8d,cl                                                        |
			add edx,r8d                                                       |
			*/
			if (n12 != b15)
			{
				nEax = n13 - 1;
				nEax >>= 3;
				if (nEax == iR9)
				{
					nR8d = pBuf[iR9];
					nEcx1= nEbx1;
					nR8d <<= (BYTE)nEcx1;
					nEdx1 += nR8d;
				}

				/*
				movzx eax,byte ptr ds:[r10+rdi]                                   |
				movzx ecx,r12b                                                    |
				not cl                                                            |
				shr edx,8                                                         |
				and cl,dl                                                         |
				and al,r12b                                                       |
				or cl,al                                                          |
				mov byte ptr ds:[r10+rdi],cl                                      |
				*/
				nEax = pBuf_dest[nR10];
				nEcx1 = n12;
				nEcx1 = ~(nEcx1 & 0xf);
				nEdx1 >>= 8;
				nEcx1 &= (BYTE)nEdx1;
				nEax &= (BYTE)n12;
				nEcx1 |= nEax;
				pBuf_dest[nR10] = (BYTE)nEcx1;
			}
		}
	}
}


int do_test()
{
	unsigned char * pbuf = new unsigned char[128];
	unsigned char * pbuf_dst = new unsigned char[128];
	memset(pbuf, 0x1a, 128);
	memset(pbuf_dst, 0, 128);
	DecodeData(pbuf, 128, pbuf_dst, 1);

	return 0;
}

