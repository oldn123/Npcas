#include "stdafx.h"
#include <assert.h>
#include "asmfunc.h"

unsigned char * g_pDicData1 = nullptr;
unsigned char * g_pDicData2 = nullptr;
char * g_dicPath = nullptr;


void SetDicDataPath(const char * p)
{
	g_dicPath = (char*)p;
}

bool LoadDicData()
{
	int bret = 0;
	if (!g_pDicData1)
	{
		char sfile[500] = {0};
		strcat(sfile, g_dicPath);
		strcat(sfile, "dic-1.bin");
		FILE * fp = fopen(sfile, "rb");
		if (fp)
		{
			fseek(fp, 0, SEEK_END);
			int nlen = ftell(fp);
			fseek(fp, 0, SEEK_SET);

			g_pDicData1 = new unsigned char[nlen];
			fread(g_pDicData1, 1, nlen, fp);
			fclose(fp);
			bret++;
		}
	}

	if (!g_pDicData2)
	{
		char sfile[500] = {0};
		strcat(sfile, g_dicPath);
		strcat(sfile, "dic-2.bin");
		FILE * fp = fopen(sfile, "rb");
		if (fp)
		{
			fseek(fp, 0, SEEK_END);
			int nlen = ftell(fp);
			fseek(fp, 0, SEEK_SET);

			g_pDicData2 = new unsigned char[nlen];
			fread(g_pDicData2, 1, nlen, fp);
			fclose(fp);
			bret++;
		}
	}

	return bret == 2;
}


/*

参数r8为 buffer-size * 8 - 1


00007FF731202ED0 | 48:895C24 08             | mov qword ptr ss:[rsp+8],rbx                                      |
00007FF731202ED5 | 48:896C24 10             | mov qword ptr ss:[rsp+10],rbp                                     |
00007FF731202EDA | 48:897424 18             | mov qword ptr ss:[rsp+18],rsi                                     |
00007FF731202EDF | 57                       | push rdi                                                          |
00007FF731202EE0 | 48:83EC 20               | sub rsp,20                                                        |
00007FF731202EE4 | 49:8BD8                  | mov rbx,r8                                                        | 关注r8
00007FF731202EE7 | 48:8BEA                  | mov rbp,rdx                                                       |
00007FF731202EEA | 48:8BF9                  | mov rdi,rcx                                                       |
00007FF731202EED | E8 7EFEFFFF              | call tslgame.7FF731202D70                                         |
00007FF731202EF2 | 48:8DB7 88000000         | lea rsi,qword ptr ds:[rdi+88]                                     |
00007FF731202EF9 | 48:8D05 A85AD400         | lea rax,qword ptr ds:[7FF731F489A8]                               |
00007FF731202F00 | 48:8907                  | mov qword ptr ds:[rdi],rax                                        |
00007FF731202F03 | 33C0                     | xor eax,eax                                                       |
00007FF731202F05 | 48:8906                  | mov qword ptr ds:[rsi],rax                                        |
00007FF731202F08 | 48:8946 08               | mov qword ptr ds:[rsi+8],rax                                      | rsi+8:L"B"
00007FF731202F0C | 48:899F 98000000         | mov qword ptr ds:[rdi+98],rbx                                     |
00007FF731202F13 | 48:83C3 07               | add rbx,7                                                         |
00007FF731202F17 | 48:8987 A0000000         | mov qword ptr ds:[rdi+A0],rax                                     |
00007FF731202F1E | 8B56 08                  | mov edx,dword ptr ds:[rsi+8]                                      | rsi+8:L"B"
00007FF731202F21 | 48:C1FB 03               | sar rbx,3                                                         |
00007FF731202F25 | 8D041A                   | lea eax,qword ptr ds:[rdx+rbx]                                    |
00007FF731202F28 | 8946 08                  | mov dword ptr ds:[rsi+8],eax                                      | rsi+8:L"B"
00007FF731202F2B | 3B46 0C                  | cmp eax,dword ptr ds:[rsi+C]                                      | rsi+C:L"p"
00007FF731202F2E | 7E 08                    | jle tslgame.7FF731202F38                                          |
00007FF731202F30 | 48:8BCE                  | mov rcx,rsi                                                       |
00007FF731202F33 | E8 28209DFD              | call tslgame.7FF72EBD4F60                                         |
00007FF731202F38 | 804F 28 21               | or byte ptr ds:[rdi+28],21                                        | rdi+28:L"!"
00007FF731202F3C | 48:85ED                  | test rbp,rbp                                                      |
00007FF731202F3F | 74 36                    | je tslgame.7FF731202F77                                           |
00007FF731202F41 | 48:8B0E                  | mov rcx,qword ptr ds:[rsi]                                        |
00007FF731202F44 | 4C:8BC3                  | mov r8,rbx                                                        |
00007FF731202F47 | 48:8BD5                  | mov rdx,rbp                                                       |
00007FF731202F4A | E8 A9A17700              | call <JMP.&memcpy>                                                |
00007FF731202F4F | 48:8B87 98000000         | mov rax,qword ptr ds:[rdi+98]                                     |
00007FF731202F56 | 4C:8BC0                  | mov r8,rax                                                        |
00007FF731202F59 | 41:83E0 07               | and r8d,7                                                         |
00007FF731202F5D | 74 18                    | je tslgame.7FF731202F77                                           |
00007FF731202F5F | 48:C1F8 03               | sar rax,3                                                         |
00007FF731202F63 | 48:63D0                  | movsxd rdx,eax                                                    |
00007FF731202F66 | 48:8D05 FBE32601         | lea rax,qword ptr ds:[7FF732471368]                               | 数据末尾字节变换位置（mod.main() + 3C01368）-->(0,1b,11b,111b,1111b,11111b,111111b,1111111b)
00007FF731202F6D | 48:0316                  | add rdx,qword ptr ds:[rsi]                                        |
00007FF731202F70 | 41:0FB60400              | movzx eax,byte ptr ds:[r8+rax]                                    |
00007FF731202F75 | 2002                     | and byte ptr ds:[rdx],al                                          | *数据末尾字节变换位置*
00007FF731202F77 | 48:8B5C24 30             | mov rbx,qword ptr ss:[rsp+30]                                     |
00007FF731202F7C | 48:8BC7                  | mov rax,rdi                                                       |
00007FF731202F7F | 48:8B6C24 38             | mov rbp,qword ptr ss:[rsp+38]                                     | [rsp+38]:"愨將t"
00007FF731202F84 | 48:8B7424 40             | mov rsi,qword ptr ss:[rsp+40]                                     |
00007FF731202F89 | 48:83C4 20               | add rsp,20                                                        |
00007FF731202F8D | 5F                       | pop rdi                                                           |
00007FF731202F8E | C3                       | ret                                                               |
*/

void ConvertEndByte(unsigned char * pData, int nLen)
{
	BYTE dic[8] = {0,1,3, 7, 0xf, 0x1f,0x3f,0x7f};
	int nR8 = nLen * 8 - 1;
	nR8 &= 7;
	int nEax = dic[nR8];
	pData[nLen - 1] &= (BYTE)nEax; 
}



/************************************************************************************************************************************************************/

/***
	00007FF72FE332A0 | 4C:8B0D 99250C03         | mov r9,qword ptr ds:[7FF732EF5840]                                |
	00007FF72FE332A7 | 4D:85C9                  | test r9,r9                                                        |
	00007FF72FE332AA | 75 04                    | jne tslgame.7FF72FE332B0                                          |
	00007FF72FE332AC | 48:8BC1                  | mov rax,rcx                                                       |
	00007FF72FE332AF | C3                       | ret                                                               |
	00007FF72FE332B0 | 49:8B01                  | mov rax,qword ptr ds:[r9]                                         |
	00007FF72FE332B3 | 44:8BC2                  | mov r8d,edx                                                       |
	00007FF72FE332B6 | 48:8BD1                  | mov rdx,rcx                                                       |
	00007FF72FE332B9 | 49:8BC9                  | mov rcx,r9                                                        |
	00007FF72FE332BC | 48:FF60 28               | jmp qword ptr ds:[rax+28]                                         | to  00007FF72FE33220
*/

/***
	读字典

	00007FF72FE33220 | 33C9                     | xor ecx,ecx                                                       |  r8?    rdx?
	00007FF72FE33222 | 41:83F8 10               | cmp r8d,10                                                        |
	00007FF72FE33226 | 8BC1                     | mov eax,ecx                                                       |
	00007FF72FE33228 | 0F96C0                   | setbe al                                                          |
	00007FF72FE3322B | 48:81FA F07F0000         | cmp rdx,7FF0                                                      |
	00007FF72FE33232 | 0F96C1                   | setbe cl                                                          |
	00007FF72FE33235 | 85C1                     | test ecx,eax                                                      |
	00007FF72FE33237 | 74 27                    | je tslgame.7FF72FE33260                                           |
	00007FF72FE33239 | 48:8D42 0F               | lea rax,qword ptr ds:[rdx+F]                                      |
	00007FF72FE3323D | B9 2C000000              | mov ecx,2C                                                        | 2C:','
	00007FF72FE33242 | 48:C1E8 04               | shr rax,4                                                         |
	00007FF72FE33246 | 48:8D15 B3CDA3FE         | lea rdx,qword ptr ds:[7FF72E870000]                               |
	00007FF72FE3324D | 0FB68410 10174C04        | movzx eax,byte ptr ds:[rax+rdx+44C1710]                           |
	00007FF72FE33255 | 2BC8                     | sub ecx,eax                                                       |
	00007FF72FE33257 | 0FB7844A A0164C04        | movzx eax,word ptr ds:[rdx+rcx*2+44C16A0]                         |
	00007FF72FE3325F | C3                       | ret                                                               |
*/

int fun_7ff72fe33220_readDic(int nR8, int nRdx) {
	
	LoadDicData();	
	bool b1 = nR8 <= 0x10;
	bool b2 = nRdx <= 0x7ff0;
	if (b1 & b2) {
		int rcx5 = 0x2C - g_pDicData1[(nRdx + 0xf) >> 4];
		int rax7 = g_pDicData2[rcx5 * 2];
		return rax7;
	}
	assert(false);
	return -1;
}

/***

00007FF72FE338A0 | 48:895C24 08             | mov qword ptr ss:[rsp+8],rbx                                      |r8 = 0,rdx = ?
00007FF72FE338A5 | 48:897424 10             | mov qword ptr ss:[rsp+10],rsi                                     |
00007FF72FE338AA | 57                       | push rdi                                                          |
00007FF72FE338AB | 48:83EC 20               | sub rsp,20                                                        |
00007FF72FE338AF | 48:8BF1                  | mov rsi,rcx                                                       |
00007FF72FE338B2 | 41:8BD8                  | mov ebx,r8d                                                       |
00007FF72FE338B5 | 48:8B0D 841F0C03         | mov rcx,qword ptr ds:[7FF732EF5840]                               |
00007FF72FE338BC | 48:8BFA                  | mov rdi,rdx                                                       |rdx->rdi
00007FF72FE338BF | 48:85C9                  | test rcx,rcx                                                      |
00007FF72FE338C2 | 75 0C                    | jne tslgame.7FF72FE338D0                                          |
00007FF72FE338C4 | E8 C76BFFFF              | call tslgame.7FF72FE2A490                                         |
00007FF72FE338C9 | 48:8B0D 701F0C03         | mov rcx,qword ptr ds:[7FF732EF5840]                               |
00007FF72FE338D0 | 48:8B01                  | mov rax,qword ptr ds:[rcx]                                        |
00007FF72FE338D3 | 44:8BCB                  | mov r9d,ebx                                                       |
00007FF72FE338D6 | 4C:8BC7                  | mov r8,rdi                                                        |rdi->r8
00007FF72FE338D9 | 48:8BD6                  | mov rdx,rsi                                                       |
00007FF72FE338DC | 48:8B5C24 30             | mov rbx,qword ptr ss:[rsp+30]                                     |
00007FF72FE338E1 | 48:8B7424 38             | mov rsi,qword ptr ss:[rsp+38]                                     |
00007FF72FE338E6 | 48:83C4 20               | add rsp,20                                                        |
00007FF72FE338EA | 5F                       | pop rdi                                                           |
00007FF72FE338EB | 48:FF60 18               | jmp qword ptr ds:[rax+18]                                         |00007FF72FE33560


00007FF72FE33560 | 48:895C24 08             | mov qword ptr ss:[rsp+8],rbx                                      |
00007FF72FE33565 | 48:896C24 10             | mov qword ptr ss:[rsp+10],rbp                                     |
00007FF72FE3356A | 48:897424 18             | mov qword ptr ss:[rsp+18],rsi                                     |
00007FF72FE3356F | 57                       | push rdi                                                          |
00007FF72FE33570 | 41:54                    | push r12                                                          |
00007FF72FE33572 | 41:55                    | push r13                                                          |
00007FF72FE33574 | 41:56                    | push r14                                                          |
00007FF72FE33576 | 41:57                    | push r15                                                          |
00007FF72FE33578 | 48:83EC 20               | sub rsp,20                                                        |
00007FF72FE3357C | 45:8BE1                  | mov r12d,r9d                                                      |
00007FF72FE3357F | 49:8BF8                  | mov rdi,r8                                                        | 关注r8
00007FF72FE33582 | 48:8BDA                  | mov rbx,rdx                                                       |
00007FF72FE33585 | 4C:8BE9                  | mov r13,rcx                                                       |
00007FF72FE33588 | 49:81F8 F07F0000         | cmp r8,7FF0                                                       |
00007FF72FE3358F | 0F87 89010000            | ja tslgame.7FF72FE3371E                                           |
00007FF72FE33595 | 41:83F9 10               | cmp r9d,10                                                        |
00007FF72FE33599 | 0F87 7F010000            | ja tslgame.7FF72FE3371E                                           |
00007FF72FE3359F | 8B0D DFE0EF02            | mov ecx,dword ptr ds:[7FF732D31684]                               | 00007FF732D31684:L"+"
00007FF72FE335A5 | 85C9                     | test ecx,ecx                                                      |
00007FF72FE335A7 | 0F84 71010000            | je tslgame.7FF72FE3371E                                           |
00007FF72FE335AD | FF15 0DA3CA01            | call qword ptr ds:[<&TlsGetValueStub>]                            |
00007FF72FE335B3 | 4C:8BF8                  | mov r15,rax                                                       |
00007FF72FE335B6 | 48:85C0                  | test rax,rax                                                      |
00007FF72FE335B9 | 0F84 5F010000            | je tslgame.7FF72FE3371E                                           |
00007FF72FE335BF | 48:85DB                  | test rbx,rbx                                                      |
00007FF72FE335C2 | 74 0C                    | je tslgame.7FF72FE335D0                                           |
00007FF72FE335C4 | 0FB7CB                   | movzx ecx,bx                                                      |
00007FF72FE335C7 | 48:85C9                  | test rcx,rcx                                                      |
00007FF72FE335CA | 0F84 4E010000            | je tslgame.7FF72FE3371E                                           |
00007FF72FE335D0 | 45:33C0                  | xor r8d,r8d                                                       |
00007FF72FE335D3 | 48:8D15 26CAA3FE         | lea rdx,qword ptr ds:[7FF72E870000]                               |
00007FF72FE335DA | 41:8BE8                  | mov ebp,r8d                                                       |
00007FF72FE335DD | 45:8BF0                  | mov r14d,r8d                                                      |
00007FF72FE335E0 | 48:85DB                  | test rbx,rbx                                                      |
00007FF72FE335E3 | 74 7E                    | je tslgame.7FF72FE33663                                           | 跳
00007FF72FE335E5 | 48:8BC3                  | mov rax,rbx                                                       |
00007FF72FE335E8 | 48:25 0000FFFF           | and rax,FFFFFFFFFFFF0000                                          |
00007FF72FE335EE | 0FB728                   | movzx ebp,word ptr ds:[rax]                                       |
00007FF72FE335F1 | 44:0FB670 02             | movzx r14d,byte ptr ds:[rax+2]                                    |
00007FF72FE335F6 | 0FB648 03                | movzx ecx,byte ptr ds:[rax+3]                                     |
00007FF72FE335FA | 48:85FF                  | test rdi,rdi                                                      |
00007FF72FE335FD | 74 30                    | je tslgame.7FF72FE3362F                                           |
00007FF72FE335FF | 80F9 E3                  | cmp cl,E3                                                         |
00007FF72FE33602 | 0F85 16010000            | jne tslgame.7FF72FE3371E                                          |
00007FF72FE33608 | 48:3BFD                  | cmp rdi,rbp                                                       |
00007FF72FE3360B | 77 2B                    | ja tslgame.7FF72FE33638                                           |
00007FF72FE3360D | 45:85F6                  | test r14d,r14d                                                    |
00007FF72FE33610 | 74 15                    | je tslgame.7FF72FE33627                                           |
00007FF72FE33612 | B8 2D000000              | mov eax,2D                                                        | 2D:'-'
00007FF72FE33617 | 41:2BC6                  | sub eax,r14d                                                      |
00007FF72FE3361A | 0FB78442 A0164C04        | movzx eax,word ptr ds:[rdx+rax*2+44C16A0]                         |
00007FF72FE33622 | 48:3BF8                  | cmp rdi,rax                                                       |
00007FF72FE33625 | 76 11                    | jbe tslgame.7FF72FE33638                                          |
00007FF72FE33627 | 48:8BC3                  | mov rax,rbx                                                       |
00007FF72FE3362A | E9 00010000              | jmp tslgame.7FF72FE3372F                                          |
00007FF72FE3362F | 80F9 E3                  | cmp cl,E3                                                         |
00007FF72FE33632 | 0F85 E6000000            | jne tslgame.7FF72FE3371E                                          |
00007FF72FE33638 | 49:8BC6                  | mov rax,r14                                                       |
00007FF72FE3363B | 48:C1E0 05               | shl rax,5                                                         |
00007FF72FE3363F | 4E:394438 10             | cmp qword ptr ds:[rax+r15+10],r8                                  |
00007FF72FE33644 | 74 1D                    | je tslgame.7FF72FE33663                                           |
00007FF72FE33646 | 42:8B4C38 08             | mov ecx,dword ptr ds:[rax+r15+8]                                  |
00007FF72FE3364B | 83F9 40                  | cmp ecx,40                                                        | 40:'@'
00007FF72FE3364E | 0F83 CA000000            | jae tslgame.7FF72FE3371E                                          |
00007FF72FE33654 | 0FAFCD                   | imul ecx,ebp                                                      |
00007FF72FE33657 | 81F9 00000100            | cmp ecx,10000                                                     |
00007FF72FE3365D | 0F83 BB000000            | jae tslgame.7FF72FE3371E                                          |

00007FF72FE33663 | 48:85FF                  | test rdi,rdi                                                      |
00007FF72FE33666 | 74 68                    | je tslgame.7FF72FE336D0                                           |
00007FF72FE33668 | 48:8D47 0F               | lea rax,qword ptr ds:[rdi+F]                                      |
00007FF72FE3366C | 48:C1E8 04               | shr rax,4                                                         |
00007FF72FE33670 | 0FB68C10 10174C04        | movzx ecx,byte ptr ds:[rax+rdx+44C1710]                           |
00007FF72FE33678 | 48:C1E1 05               | shl rcx,5                                                         |
00007FF72FE3367C | 49:03CF                  | add rcx,r15                                                       |
00007FF72FE3367F | 48:8B31                  | mov rsi,qword ptr ds:[rcx]                                        |
00007FF72FE33682 | 48:85F6                  | test rsi,rsi                                                      |
00007FF72FE33685 | 75 21                    | jne tslgame.7FF72FE336A8                                          |
00007FF72FE33687 | 4C:3941 10               | cmp qword ptr ds:[rcx+10],r8                                      |
00007FF72FE3368B | 0F84 8D000000            | je tslgame.7FF72FE3371E                                           |
00007FF72FE33691 | 0F1041 10                | movups xmm0,xmmword ptr ds:[rcx+10]                               |
00007FF72FE33695 | 0F1101                   | movups xmmword ptr ds:[rcx],xmm0                                  |
00007FF72FE33698 | 4C:8941 10               | mov qword ptr ds:[rcx+10],r8                                      |
00007FF72FE3369C | 44:8941 18               | mov dword ptr ds:[rcx+18],r8d                                     |
00007FF72FE336A0 | 48:8B31                  | mov rsi,qword ptr ds:[rcx]                                        |
00007FF72FE336A3 | 48:85F6                  | test rsi,rsi                                                      |
00007FF72FE336A6 | 74 76                    | je tslgame.7FF72FE3371E                                           |
00007FF72FE336A8 | FF49 08                  | dec dword ptr ds:[rcx+8]                                          |
00007FF72FE336AB | 48:8B06                  | mov rax,qword ptr ds:[rsi]                                        |
00007FF72FE336AE | 48:8901                  | mov qword ptr ds:[rcx],rax                                        |
00007FF72FE336B1 | 48:85DB                  | test rbx,rbx                                                      |
00007FF72FE336B4 | 74 63                    | je tslgame.7FF72FE33719                                           |
00007FF72FE336B6 | 44:8BC5                  | mov r8d,ebp                                                       |
00007FF72FE336B9 | 48:8BD3                  | mov rdx,rbx                                                       |
00007FF72FE336BC | 49:3BF8                  | cmp rdi,r8                                                        |
00007FF72FE336BF | 48:8BCE                  | mov rcx,rsi                                                       |
00007FF72FE336C2 | 4C:0F46C7                | cmovbe r8,rdi                                                     |
00007FF72FE336C6 | E8 2D9AB401              | call <JMP.&memcpy>                                                |
00007FF72FE336CB | 45:33C0                  | xor r8d,r8d                                                       |
00007FF72FE336CE | EB 03                    | jmp tslgame.7FF72FE336D3                                          |
00007FF72FE336D0 | 49:8BF0                  | mov rsi,r8                                                        |
00007FF72FE336D3 | 48:85DB                  | test rbx,rbx                                                      |
00007FF72FE336D6 | 74 41                    | je tslgame.7FF72FE33719                                           |
00007FF72FE336D8 | 41:8BD6                  | mov edx,r14d                                                      |
00007FF72FE336DB | 48:C1E2 05               | shl rdx,5                                                         |
00007FF72FE336DF | 49:03D7                  | add rdx,r15                                                       |
00007FF72FE336E2 | 8B42 08                  | mov eax,dword ptr ds:[rdx+8]                                      |
00007FF72FE336E5 | 83F8 40                  | cmp eax,40                                                        | 40:'@'
00007FF72FE336E8 | 73 0A                    | jae tslgame.7FF72FE336F4                                          |
00007FF72FE336EA | 0FAFC5                   | imul eax,ebp                                                      |
00007FF72FE336ED | 3D 00000100              | cmp eax,10000                                                     |
00007FF72FE336F2 | 72 15                    | jb tslgame.7FF72FE33709                                           |
00007FF72FE336F4 | 48:837A 10 00            | cmp qword ptr ds:[rdx+10],0                                       |
00007FF72FE336F9 | 75 1E                    | jne tslgame.7FF72FE33719                                          |
00007FF72FE336FB | 0F1002                   | movups xmm0,xmmword ptr ds:[rdx]                                  |
00007FF72FE336FE | 0F1142 10                | movups xmmword ptr ds:[rdx+10],xmm0                               |
00007FF72FE33702 | 4C:8902                  | mov qword ptr ds:[rdx],r8                                         |
00007FF72FE33705 | 44:8942 08               | mov dword ptr ds:[rdx+8],r8d                                      |
00007FF72FE33709 | 48:8B0A                  | mov rcx,qword ptr ds:[rdx]                                        |
00007FF72FE3370C | 48:890B                  | mov qword ptr ds:[rbx],rcx                                        |
00007FF72FE3370F | 4C:8943 08               | mov qword ptr ds:[rbx+8],r8                                       |
00007FF72FE33713 | FF42 08                  | inc dword ptr ds:[rdx+8]                                          |
00007FF72FE33716 | 48:891A                  | mov qword ptr ds:[rdx],rbx                                        |
00007FF72FE33719 | 48:8BC6                  | mov rax,rsi                                                       | 赋值
00007FF72FE3371C | EB 11                    | jmp tslgame.7FF72FE3372F                                          |
00007FF72FE3371E | 45:8BCC                  | mov r9d,r12d                                                      |
00007FF72FE33721 | 4C:8BC7                  | mov r8,rdi                                                        |
00007FF72FE33724 | 48:8BD3                  | mov rdx,rbx                                                       |
00007FF72FE33727 | 49:8BCD                  | mov rcx,r13                                                       |
00007FF72FE3372A | E8 C1010000              | call tslgame.7FF72FE338F0                                         |
00007FF72FE3372F | 48:8B5C24 50             | mov rbx,qword ptr ss:[rsp+50]                                     |
00007FF72FE33734 | 48:8B6C24 58             | mov rbp,qword ptr ss:[rsp+58]                                     |
00007FF72FE33739 | 48:8B7424 60             | mov rsi,qword ptr ss:[rsp+60]                                     |
00007FF72FE3373E | 48:83C4 20               | add rsp,20                                                        |
00007FF72FE33742 | 41:5F                    | pop r15                                                           |
00007FF72FE33744 | 41:5E                    | pop r14                                                           |
00007FF72FE33746 | 41:5D                    | pop r13                                                           |
00007FF72FE33748 | 41:5C                    | pop r12                                                           |
00007FF72FE3374A | 5F                       | pop rdi                                                           |
00007FF72FE3374B | C3                       | ret                                                               |
*/

unsigned char * GetTlsVal()
{
	return 0;
}

__int64 fun_7ff72fe338a0_conv(int nR8, int nRdx)
{
	int nRax = nR8 + 0x0f;
	nRax >>= 4;

	int nEcx = g_pDicData1[nRax];
	nEcx <<= 5;

	unsigned char * pTlsVal = GetTlsVal();
	__int64 i64val = (__int64)pTlsVal[nEcx];

	return i64val;
}


/***
参数dword ptr ds:[rcx+8]  为buffer长度

00007FF72EBD4F60 | 48:895C24 08             | mov qword ptr ss:[rsp+8],rbx                                      |
00007FF72EBD4F65 | 57                       | push rdi                                                          |
00007FF72EBD4F66 | 48:83EC 20               | sub rsp,20                                                        |
00007FF72EBD4F6A | 48:6379 08               | movsxd rdi,dword ptr ds:[rcx+8]                                   |
00007FF72EBD4F6E | 48:8BD9                  | mov rbx,rcx                                                       |
00007FF72EBD4F71 | B9 04000000              | mov ecx,4                                                         |
00007FF72EBD4F76 | 837B 0C 00               | cmp dword ptr ds:[rbx+C],0                                        |
00007FF72EBD4F7A | 75 04                    | jne tslgame.7FF72EBD4F80                                          |
00007FF72EBD4F7C | 3BF9                     | cmp edi,ecx                                                       |
00007FF72EBD4F7E | 76 0F                    | jbe tslgame.7FF72EBD4F8F                                          |
00007FF72EBD4F80 | 48:8D047F                | lea rax,qword ptr ds:[rdi+rdi*2]                                  | rdi
00007FF72EBD4F84 | 48:C1E8 03               | shr rax,3                                                         |
00007FF72EBD4F88 | 48:8D4F 10               | lea rcx,qword ptr ds:[rdi+10]                                     |
00007FF72EBD4F8C | 48:03C8                  | add rcx,rax                                                       |
00007FF72EBD4F8F | 33D2                     | xor edx,edx                                                       | rcx, rdx
00007FF72EBD4F91 | E8 0AE32501              | call tslgame.7FF72FE332A0                                         | 跟进、找返回值   00007FF72FE332A0
00007FF72EBD4F96 | 3BF8                     | cmp edi,eax                                                       |
00007FF72EBD4F98 | B9 FFFFFF7F              | mov ecx,7FFFFFFF                                                  |
00007FF72EBD4F9D | 0F4FC1                   | cmovg eax,ecx                                                     | eax < edi时赋值
00007FF72EBD4FA0 | 8943 0C                  | mov dword ptr ds:[rbx+C],eax                                      |
00007FF72EBD4FA3 | 48:8B0B                  | mov rcx,qword ptr ds:[rbx]                                        |
00007FF72EBD4FA6 | 48:85C9                  | test rcx,rcx                                                      |
00007FF72EBD4FA9 | 75 04                    | jne tslgame.7FF72EBD4FAF                                          |
00007FF72EBD4FAB | 85C0                     | test eax,eax                                                      |
00007FF72EBD4FAD | 74 0E                    | je tslgame.7FF72EBD4FBD                                           |
00007FF72EBD4FAF | 48:63D0                  | movsxd rdx,eax                                                    | rdx
00007FF72EBD4FB2 | 45:33C0                  | xor r8d,r8d                                                       |
00007FF72EBD4FB5 | E8 E6E82501              | call tslgame.7FF72FE338A0                                         | 关注返回值   00007FF72FE338A0
00007FF72EBD4FBA | 48:8903                  | mov qword ptr ds:[rbx],rax                                        |
00007FF72EBD4FBD | 48:8B5C24 30             | mov rbx,qword ptr ss:[rsp+30]                                     |
00007FF72EBD4FC2 | 48:83C4 20               | add rsp,20                                                        |
00007FF72EBD4FC6 | 5F                       | pop rdi                                                           |
00007FF72EBD4FC7 | C3                       | ret    
*/


int fun_7ff72ebd4f60_readDic(int nRdx) {
	if (nRdx <= 4)
	{
		assert(false);
	}
	int nRax = nRdx + nRdx * 2;
	nRax >>= 3;
	nRax += (nRdx + 0x10);
	int nRcx = nRax;
	int nR8 = 0;
	nRax = fun_7ff72fe33220_readDic(nR8, nRcx);
	if (nRax < nRdx)
	{
		nRax = 0x7ffffff;	
		assert(0);
	}
	nR8 = 0;
	__int64 i64Ret = fun_7ff72fe338a0_conv(nR8, nRax);
	return (int)i64Ret;
}

//H:\game\Steam\steamapps\common\PUBG\TslGame\Binaries\Win64\TslGame_BE.exe -LobbyUrl=https://prod-live-front.playbattlegrounds.com/index.html