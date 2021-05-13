#include <stdio.h>
#include <windows.h>	//For Thread
#include <process.h>	//For Thread
#include <stdlib.h>	//For Rand....
#include <time.h>	//For Rand....
//#include <winsock2.h>

#include "main.h"

/*Global Variable*/
SOCKET rqSock, rpSock, flSock, frSock, fsSock, pfSock, fgSock;	//socket
HANDLE hMutex;	//MultiThread Criticial section
nodeType Usr_node; //node information
int sMode = 0;	//slient mode
int exitFlag = 1;	//thread parameter

static const unsigned char sTable[256] =
{
	0xa3,0xd7,0x09,0x83,0xf8,0x48,0xf6,0xf4,0xb3,0x21,0x15,0x78,0x99,0xb1,0xaf,0xf9,
	0xe7,0x2d,0x4d,0x8a,0xce,0x4c,0xca,0x2e,0x52,0x95,0xd9,0x1e,0x4e,0x38,0x44,0x28,
	0x0a,0xdf,0x02,0xa0,0x17,0xf1,0x60,0x68,0x12,0xb7,0x7a,0xc3,0xe9,0xfa,0x3d,0x53,
	0x96,0x84,0x6b,0xba,0xf2,0x63,0x9a,0x19,0x7c,0xae,0xe5,0xf5,0xf7,0x16,0x6a,0xa2,
	0x39,0xb6,0x7b,0x0f,0xc1,0x93,0x81,0x1b,0xee,0xb4,0x1a,0xea,0xd0,0x91,0x2f,0xb8,
	0x55,0xb9,0xda,0x85,0x3f,0x41,0xbf,0xe0,0x5a,0x58,0x80,0x5f,0x66,0x0b,0xd8,0x90,
	0x35,0xd5,0xc0,0xa7,0x33,0x06,0x65,0x69,0x45,0x00,0x94,0x56,0x6d,0x98,0x9b,0x76,
	0x97,0xfc,0xb2,0xc2,0xb0,0xfe,0xdb,0x20,0xe1,0xeb,0xd6,0xe4,0xdd,0x47,0x4a,0x1d,
	0x42,0xed,0x9e,0x6e,0x49,0x3c,0xcd,0x43,0x27,0xd2,0x07,0xd4,0xde,0xc7,0x67,0x18,
	0x89,0xcb,0x30,0x1f,0x8d,0xc6,0x8f,0xaa,0xc8,0x74,0xdc,0xc9,0x5d,0x5c,0x31,0xa4,
	0x70,0x88,0x61,0x2c,0x9f,0x0d,0x2b,0x87,0x50,0x82,0x54,0x64,0x26,0x7d,0x03,0x40,
	0x34,0x4b,0x1c,0x73,0xd1,0xc4,0xfd,0x3b,0xcc,0xfb,0x7f,0xab,0xe6,0x3e,0x5b,0xa5,
	0xad,0x04,0x23,0x9c,0x14,0x51,0x22,0xf0,0x29,0x79,0x71,0x7e,0xff,0x8c,0x0e,0xe2,
	0x0c,0xef,0xbc,0x72,0x75,0x6f,0x37,0xa1,0xec,0xd3,0x8e,0x62,0x8b,0x86,0x10,0xe8,
	0x08,0x77,0x11,0xbe,0x92,0x4f,0x24,0xc5,0x32,0x36,0x9d,0xcf,0xf3,0xa6,0xbb,0xac,
	0x5e,0x6c,0xa9,0x13,0x57,0x25,0xb5,0xe3,0xbd,0xa8,0x3a,0x01,0x05,0x59,0x2a,0x46
};
static const unsigned char dTable[256] =
{
	0x27,0xd2,0x07,0xd4,0xde,0xc7,0x67,0x18,0x42,0xed,0x9e,0x6e,0x49,0x3c,0xcd,0x43,
	0x89,0xcb,0x30,0x1f,0x8d,0xc6,0x8f,0xaa,0xc8,0x74,0xdc,0xc9,0x5d,0x5c,0x31,0xa4,
	0x70,0x88,0x61,0x2c,0x9f,0x0d,0x2b,0x87,0x50,0x82,0x54,0x64,0x26,0x7d,0x03,0x40,
	0x34,0x4b,0x1c,0x73,0xd1,0xc4,0xfd,0x3b,0xcc,0xfb,0x7f,0xab,0xe6,0x3e,0x5b,0xa5,
	0x29,0x79,0x71,0x7e,0xff,0x8c,0x0e,0xe2,0xad,0x04,0x23,0x9c,0x14,0x51,0x22,0xf0,
	0x55,0xb9,0xda,0x85,0x3f,0x41,0xbf,0xe0,0x5a,0x58,0x80,0x5f,0x66,0x0b,0xd8,0x90,
	0x35,0xd5,0xc0,0xa7,0x33,0x06,0x65,0x69,0x45,0x00,0x94,0x56,0x6d,0x98,0x9b,0x76,
	0x97,0xfc,0xb2,0xc2,0xb0,0xfe,0xdb,0x20,0xe1,0xeb,0xd6,0xe4,0xdd,0x47,0x4a,0x1d,
	0x0c,0xef,0xbc,0x72,0x75,0x6f,0x37,0xa1,0xec,0xd3,0x8e,0x62,0x8b,0x86,0x10,0xe8,
	0xca,0x2e,0x52,0x95,0xd9,0x1e,0x4e,0x38,0x44,0x28,0xe7,0x2d,0x4d,0x8a,0xce,0x4c,
	0x0a,0xdf,0x02,0xa0,0x17,0xf1,0x60,0x68,0x12,0xb7,0x7a,0xc3,0xe9,0xfa,0x3d,0x53,
	0x96,0x84,0x6b,0xba,0xf2,0x63,0x9a,0x19,0x7c,0xae,0xe5,0xf5,0xf7,0x16,0x6a,0xa2,
	0x08,0x77,0x11,0xbe,0x92,0x4f,0x24,0xc5,0x32,0x36,0x9d,0xcf,0xf3,0xa6,0xbb,0xac,
	0x5e,0x6c,0xa9,0x13,0x57,0x25,0xb5,0xe3,0xbd,0xa8,0x3a,0x01,0x05,0x59,0x2a,0x46,
	0x15,0x78,0x99,0xb1,0xaf,0xf9,0xa3,0xd7,0x09,0x83,0xf8,0x48,0xf6,0xf4,0xb3,0x21,
	0x39,0xb6,0x7b,0x0f,0xc1,0x93,0x81,0x1b,0xee,0xb4,0x1a,0xea,0xd0,0x91,0x2f,0xb8,

};


int main(int argc, char *argv[])
{
	int flag = 0, cflag = 0, test_count = 0;
	char input;
	chordHeaderType Node_Message;
	char hashing[20] = { 0 };
	int iResult = 0;	//to check return value of udp function
	int retVal;
	int optVal = 700; // 0.7 seconds


					  /*Execution error*/
	if (argc != 3) {
		printf("Usage : %s <IP> <port> \n", argv[0]);
		exit(1);
	}
	if (atoi(argv[2]) < 49152 || atoi(argv[2]) > 65535)
	{
		printf("Usage : port number -> 49152 < port < 65535 \n");
		exit(1);
	}
	/*Mutex initalize*/
	hMutex = CreateMutex(NULL, FALSE, NULL);

	/*For Socket Programming*/
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		error_message("WSAStartup() error!\n");

	/*Local(User) Node initialization*/
	memset(&Usr_node.nodeInfo.addrInfo, 0, sizeof(Usr_node.nodeInfo.addrInfo));
	Usr_node.nodeInfo.addrInfo.sin_family = AF_INET;
	Usr_node.nodeInfo.addrInfo.sin_addr.s_addr = inet_addr(argv[1]);
	Usr_node.nodeInfo.addrInfo.sin_port = htons(atoi(argv[2]));

	/*Create UDP Socket*/
	rqSock = socket(AF_INET, SOCK_DGRAM, 0);
	if (rqSock == INVALID_SOCKET)
		error_message("UDP socket creation error!");

	rpSock = socket(AF_INET, SOCK_DGRAM, 0);
	if (rpSock == INVALID_SOCKET)
		error_message("UDP socket creation error!");

	/*PingPong FixFinger Socket*/
	pfSock = socket(AF_INET, SOCK_DGRAM, 0);
	if (pfSock == INVALID_SOCKET)
		error_message("PF socket creation error!");

	fgSock = socket(AF_INET, SOCK_DGRAM, 0);
	if (fgSock == INVALID_SOCKET)
		error_message("PF socket creation error!");
	retVal = setsockopt(fgSock, SOL_SOCKET, SO_RCVTIMEO, (char*)&optVal, sizeof(optVal));

	/*Binding Socket*/
	if (bind(rpSock, (struct sockaddr *)&Usr_node.nodeInfo.addrInfo, sizeof(Usr_node.nodeInfo.addrInfo)) == INVALID_SOCKET)
		error_message("bind() error!");

	/*Initalizing node information for hasing*/
	strcpy(hashing, argv[1]);
	strcat(hashing, argv[2]);
	Usr_node.nodeInfo.ID = str_hash(hashing);	//If the same hashing value is in table, calculate one more time.


	/*To Create Menu*/
	while (flag == 0)
	{



		printf("***************************************************************\n");
		printf("*               DHT-Based P2P Protocol Controller             *\n");
		printf("*                  Ver.0.1     Dec. 12. 2018                  *\n");
		printf("*               <c> Ko, Sung-Hoon / Kim, Ki-bum               *\n");
		printf("***************************************************************\n\n");

		printf(">>> Welcome to ChordNode Program!\n");
		printf(">>> Your IP address : %s,  Port No : %d, ID : %d\n",
			inet_ntoa(Usr_node.nodeInfo.addrInfo.sin_addr),
			ntohs(Usr_node.nodeInfo.addrInfo.sin_port),
			Usr_node.nodeInfo.ID);
		printf(">>> Silent Mode is ON!\n\n");

		FHelp();
		flag = 1;
		while (flag == 1) {
			printf("CHORD> ");
			scanf(" %c", &input);
			rewind(stdin);

			if (input == 'c') {
				//Create chord network
				if (cflag == 1) {
					printf("[ERROR] You are currently in the network!; You cannot create the network!\n");
					continue;
				}

				if (create(&Usr_node) != SUCCESS)
					error_message("Create() error!!\n");
				exitFlag = 1;
				printf("CHORD> You have created a chord network!\n");
				printf("CHORD> Your finger table has been updated!\n\n");

				/*Make udp_thread*/
				HANDLE udp_thread;
				udp_thread = (HANDLE)_beginthreadex(NULL, 0, (void*)procRecvMsg, (void*)&exitFlag, 0, NULL);
				if (udp_thread == 0)
					error_message("[ERROR]procRecvMsg _beginthreadex error!\n");

				/*Make pingpong fixfinger thread*/
				HANDLE pingpong_thread;
				pingpong_thread = (HANDLE)_beginthreadex(NULL, 0, (void*)procPingPong, (void*)&exitFlag, 0, NULL);
				if (pingpong_thread == 0)
					error_message("[ERROR]pp _beginthreadex error!\n");

				/*	HANDLE fixfinger_thread;
				fixfinger_thread = (HANDLE)_beginthreadex(NULL, 0, (void*)procFixfinger, (void*)&exitFlag, 0, NULL);
				if (fixfinger_thread == 0)
				error_message("[ERROR]pp _beginthreadex error!\n");*/

			}
			else if (input == 'j') {
				//Chord네트워크 가입
				printf("CHORD> Join in the network.\n");
				printf("CHORD> You need a helper node to join the existing network.\n");
				printf("CHORD> If you want to create a network, the helper node is yourself.\n");

				exitFlag = 1;
				/*Make udp_thread*/
				HANDLE udp_thread;
				udp_thread = (HANDLE)_beginthreadex(NULL, 0, (void*)procRecvMsg, (void*)&exitFlag, 0, NULL);
				if (udp_thread == 0)
					error_message("[ERROR] udp _beginthreadex error!\n");

				/*join execution*/

				join(hashing);
				/*Make pingpong fixfinger thread*/

				HANDLE pp_thread;
				pp_thread = (HANDLE)_beginthreadex(NULL, 0, (void*)procPingPong, (void*)&exitFlag, 0, NULL);
				if (pp_thread == 0)
					error_message("[ERROR] pp _beginthreadex error!\n");

				/*HANDLE fixfinger_thread;
				fixfinger_thread = (HANDLE)_beginthreadex(NULL, 0, (void*)procFixfinger, (void*)&exitFlag, 0, NULL);
				if (fixfinger_thread == 0)
				error_message("[ERROR]pp _beginthreadex error!\n");*/

			}
			else if (input == 'q') {
				//프로그램 종료
				printf("CHORD > Exit the program. bye bye~!\n");
				flag = -2;
				break;
			}
			else if (input == 't')
			{
				unsigned int interval = 0;
				srand(time(NULL));
				TestMode();	//화면 출력
				printf("CHORD> Please set helper node's IP : 127.0.0.1 //PORT : // 50000 \n");
				printf("CHORD> Input the number of Join, Leave test : ");
				scanf("%d", &test_count);

				for (int i = 0; i < test_count; i++)
				{
					interval = rand() % 1001 + 1000;	//	1000~2000
					test_join();
					printf("TEST> Join successfully finished \n");
					Sleep(interval);
					leave();
					printf("TEST> Leave successfully finished \n");
					Sleep(7000);
				}
				FHelp();
				continue;
				
			}
			else {
				printf("[ERROR] Wrong command! Input a correct command!\n");
				continue;
			}

			flag = 2;
			SHelp();
			while (flag == 2) {
				printf("CHORD> ");
				scanf(" %c", &input);
				rewind(stdin);
				if (input == 'h') {
					SHelp();
				}
				else if (input == 'a') {
					//파일등록   
					addfile();
				}
				else if (input == 'd') {
					//파일삭제     
					deletefile();
				}
				else if (input == 's') {
					//파일검색              
					char TargetName[FNameMax] = { 0 };
					int TargetKey = 0, TargetFileNum = 0, target = 0;
					int result = 0, iResult = 0;
					int CheckFile = 0;
					char answer = 0;
					chordHeaderType message;
					nodeInfoType LookupNode;	//for lookup

					struct sockaddr_in opponent;
					int opponent_size = sizeof(opponent);

					printf("Input File name to search and download : ");
					scanf("%s", TargetName);
					rewind(stdin);
					TargetKey = str_hash(TargetName);
					//여기에 여러번 해싱한 혼자만의 키값을 타겟키에 가질 수 있도록.. 
					result = lookup(&Usr_node, TargetKey, &LookupNode);
					//찾은 노드는 LookupNode에 있음.

					/*return value check*/
					if (result == 0)
					{
						for (CheckFile = 0; CheckFile < FileMax; CheckFile++)
						{
							File_Reference_Info_Request(&message, CheckFile);
							iResult = sendto(rqSock, (char *)&message, sizeof(chordHeaderType), 0, (struct sockaddr*)&LookupNode.addrInfo, opponent_size);
							if (iResult == SOCKET_ERROR)
								printf("File Reference Info request error!\n");
							iResult = recvfrom(rqSock, (char *)&message, sizeof(chordHeaderType), 0, (struct sockaddr*)&opponent, &opponent_size);
							if (iResult == SOCKET_ERROR)
								printf("File Reference Info response error!\n");
							if (TargetKey == message.fileInfo.Key)
								break;
						}

						if (CheckFile == FileMax)
						{
							printf("There is no file!  \n");

						}
						else
						{
							printf("The file is located other node\n");
							printf("ID : %d \n", LookupNode.ID);
						}

						//다운로드 메시지 주고받기

						while (CheckFile != FileMax)
						{
							printf("Do you want to download the file?( y / n ) \n");
							scanf("%c", &answer);
							rewind(stdin);
							if ((answer == 'y') || (answer == 'n'))
								break;
						}

						if (answer == 'y')
						{
							fileRefType Recieve_arg;
							opponent = message.fileInfo.owner.addrInfo;
							FileDown_Request(&message, TargetName);
							//check socket
							iResult = sendto(rqSock, (char *)&message, sizeof(chordHeaderType), 0, (struct sockaddr*)&opponent, opponent_size);
							if (iResult == SOCKET_ERROR)
								printf("File down request error!\n");
							iResult = recvfrom(rqSock, (char *)&message, sizeof(chordHeaderType), 0, (struct sockaddr*)&opponent, &opponent_size);
							if (iResult == SOCKET_ERROR)
								printf("File down response error!\n");

							//recv에서 성공인지 처리 후 
							Recieve_arg.Key = message.fileInfo.Key;
							strcpy(Recieve_arg.Name, message.fileInfo.Name);
							Recieve_arg.owner = message.fileInfo.owner;
							Recieve_arg.refOwner = message.fileInfo.refOwner;

							printf("The file name that is downloading %s\n", Recieve_arg.Name);
							printf("Port : %d\n", ntohs(Recieve_arg.owner.addrInfo.sin_port)); //port출력 //이후삭제

							printf("IP : %s\n", inet_ntoa(Recieve_arg.owner.addrInfo.sin_addr));//ip주소출력 //이후삭제

							printf("-- Make FILE Thread --\n");
							HANDLE file_thread;
							file_thread = (HANDLE)_beginthreadex(NULL, 0, (void*)File_Recieve_Thread, (void*)&Recieve_arg, 0, NULL);
							if (file_thread == 0)
								error_message("File_Recieve_Thread _beginthreadex error!\n");
						}


					}
					else if (result == 1)
					{
						//본인 파일
						printf("The file is own file!\n");
					}
					else if (result == 2)
					{
						//본인 레퍼런스 
						printf("The fiie is own Reference file \n");
						for (target = 0; Usr_node.chordInfo.FRefInfo.fileNum; target++)
						{
							if (TargetKey == Usr_node.chordInfo.FRefInfo.fileRef[target].Key)
								break;
						}
						while (1)
						{
							printf("Do you want to download the file?( y / n ) \n");
							scanf("%c", &answer);
							rewind(stdin);
							if ((answer == 'y') || (answer == 'n'))
								break;
						}

						if (answer == 'y')
						{
							fileRefType Recieve_arg;
							opponent = Usr_node.chordInfo.FRefInfo.fileRef[target].owner.addrInfo;
							FileDown_Request(&message, TargetName);
							//check socket
							iResult = sendto(rqSock, (char *)&message, sizeof(chordHeaderType), 0, (struct sockaddr*)&opponent, opponent_size);
							if (iResult == SOCKET_ERROR)
								printf("File down request error!\n");
							iResult = recvfrom(rqSock, (char *)&message, sizeof(chordHeaderType), 0, (struct sockaddr*)&opponent, &opponent_size);
							if (iResult == SOCKET_ERROR)
								printf("File down response error!\n");

							//recv에서 성공인지 처리 후 
							Recieve_arg.Key = message.fileInfo.Key;
							strcpy(Recieve_arg.Name, message.fileInfo.Name);
							Recieve_arg.owner = message.fileInfo.owner;
							Recieve_arg.refOwner = message.fileInfo.refOwner;

							printf("다운로드 하고자 하는 파일 이름 %s\n", Recieve_arg.Name);
							printf("Port : %d\n", ntohs(Recieve_arg.owner.addrInfo.sin_port)); //port출력 //이후삭제

							printf("IP : %s\n", inet_ntoa(Recieve_arg.owner.addrInfo.sin_addr));//ip주소출력 //이후삭제

							printf("-- Make FILE Thread --\n");
							HANDLE file_thread;
							file_thread = (HANDLE)_beginthreadex(NULL, 0, (void*)File_Recieve_Thread, (void*)&Recieve_arg, 0, NULL);
							if (file_thread == 0)
								error_message("File_Recieve_Thread _beginthreadex error!\n");
						};
					}
					/*lookup check finished*/

				}
				else if (input == 'm') {
					if (sMode == 1)	//sMode is on.
					{
						sMode = 0; //off
						printf("CHORD> Silent mode is ON! \n");
					}
					else
					{
						sMode = 1;	//on
						printf("CHORD> Silent mode is OFF! \n");
					}
				}
				else if (input == 'f') {
					//핑거테이블
					printf("\t\tIP\t\tPORT\tID\n");
					printf("Predecessor\t");
					printf("%s\t", inet_ntoa(Usr_node.chordInfo.fingerInfo.Pre.addrInfo.sin_addr));//ip주소출력 
					printf("%d\t", ntohs(Usr_node.chordInfo.fingerInfo.Pre.addrInfo.sin_port)); //port출력 
					printf("%d\n", Usr_node.chordInfo.fingerInfo.Pre.ID);
					for (int i = 0; i < baseM; i++)
					{
						printf("Finger[%d]\t", i);
						printf("%s\t", inet_ntoa(Usr_node.chordInfo.fingerInfo.finger[i].addrInfo.sin_addr));//ip주소출력 
						printf("%d\t", ntohs(Usr_node.chordInfo.fingerInfo.finger[i].addrInfo.sin_port)); //port출력 
						printf("%d\n", Usr_node.chordInfo.fingerInfo.finger[i].ID);

					}

				}
				else if (input == 'i') {
					//노드정보
					printf("CHORD> My Node Information\n");
					printf("CHORD> My Node IP : %s\n", inet_ntoa(Usr_node.nodeInfo.addrInfo.sin_addr));
					printf("CHORD> My Node Port : %d\n", ntohs(Usr_node.nodeInfo.addrInfo.sin_port));
					printf("CHORD> My Node ID : %d\n", Usr_node.nodeInfo.ID);

					/*for (unsigned int i = 0; i < Usr_node.fileInfo.fileNum; i++)
					//if 해서 1st, 2nd, 3rd, 4th....
					printf("CHORD> %dst own file name : %s, Key : %d \n",
					i + 1, Usr_node.fileInfo.fileRef[i].Name, Usr_node.fileInfo.fileRef[i].Key);*/

					for (unsigned int i = 0; i < Usr_node.fileInfo.fileNum; i++)
						//if 해서 1st, 2nd, 3rd, 4th....
						printf("CHORD> %dst own file name : %s, Key : %d  ref IP : %s , Port : %d \n",
							i + 1, Usr_node.fileInfo.fileRef[i].Name, Usr_node.fileInfo.fileRef[i].Key,
							inet_ntoa(Usr_node.fileInfo.fileRef[i].refOwner.addrInfo.sin_addr),
							ntohs(Usr_node.fileInfo.fileRef[i].refOwner.addrInfo.sin_port));

					for (unsigned int i = 0; i < Usr_node.chordInfo.FRefInfo.fileNum; i++)
						//owner ID 는 좀 더 생각 해보기로.
						printf("CHORD> %dst Ref file name : %s, Key : %d , IP : %s , Port : %d \n",
							i + 1, Usr_node.chordInfo.FRefInfo.fileRef[i].Name, Usr_node.chordInfo.FRefInfo.fileRef[i].Key,
							inet_ntoa(Usr_node.chordInfo.FRefInfo.fileRef[i].owner.addrInfo.sin_addr),
							ntohs(Usr_node.chordInfo.FRefInfo.fileRef[i].owner.addrInfo.sin_port));



				}
				else if (input == 'l') {
					//탈퇴
					char answer;

					printf("CHORD > Are you sure leaveing the network? (y / n) : ");
					scanf("%c", &answer);
					rewind(stdin);

					while (answer != 'y' || answer != 'n')
					{
						if (answer == 'y' || answer == 'n')	break;

						printf("[ERROR] Wrong command! Input a correct command! \n ");
						printf("CHORD > Are you sure leaveing the network? (y / n) : ");
						scanf("%c", &answer);
						rewind(stdin);


					}

					if (answer == 'y')
					{
						printf("CHORD > Exit the program... \n");
						leave();

					}
					else
						continue;

					//exit(1);

					Sleep(7000); // Wait for closing thread....
					flag = -1;
				}
				else if (input == 'q') {
					//프로그램 종료
					printf("CHORD > Exit the program.\n");
					flag = -2;
					exitFlag = 0;
					break;
				}
				else {
					printf("[ERROR] Wrong command! Input a correct command!\n");
					continue;
				}
			}
		}
		if (flag == -1)
		{
			/*Close Socket*/
			closesocket(rqSock);
			closesocket(rpSock);
			closesocket(pfSock);
			closesocket(fgSock);
			CloseHandle(hMutex);
			WSACleanup();

			/*Mutex initalize*/
			hMutex = CreateMutex(NULL, FALSE, NULL);

			/*For Socket Programming*/
			if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
				error_message("WSAStartup() error!\n");

			/*Local(User) Node initialization*/
			memset(&Usr_node.nodeInfo.addrInfo, 0, sizeof(Usr_node.nodeInfo.addrInfo));
			Usr_node.nodeInfo.addrInfo.sin_family = AF_INET;
			Usr_node.nodeInfo.addrInfo.sin_addr.s_addr = inet_addr(argv[1]);
			Usr_node.nodeInfo.addrInfo.sin_port = htons(atoi(argv[2]));

			/*Create UDP Socket*/
			rqSock = socket(AF_INET, SOCK_DGRAM, 0);
			if (rqSock == INVALID_SOCKET)
				error_message("UDP socket creation error!");

			rpSock = socket(AF_INET, SOCK_DGRAM, 0);
			if (rpSock == INVALID_SOCKET)
				error_message("UDP socket creation error!");

			/*PingPong FixFinger Socket*/
			pfSock = socket(AF_INET, SOCK_DGRAM, 0);
			if (pfSock == INVALID_SOCKET)
				error_message("PF socket creation error!");

			fgSock = socket(AF_INET, SOCK_DGRAM, 0);
			if (fgSock == INVALID_SOCKET)
				error_message("FG socket creation error!");
			retVal = setsockopt(fgSock, SOL_SOCKET, SO_RCVTIMEO, (char*)&optVal, sizeof(optVal));

			/*Binding Socket*/
			if (bind(rpSock, (struct sockaddr *)&Usr_node.nodeInfo.addrInfo, sizeof(Usr_node.nodeInfo.addrInfo)) == INVALID_SOCKET)
				error_message("bind() error!");

			/*Initalizing node information for hasing*/
			/*strcpy(hashing, argv[1]);
			strcat(hashing, argv[2]);
			Usr_node.nodeInfo.ID = str_hash(hashing);	*///If the same hashing value is in table, calculate one more time*

			flag = 0;
		}
	}





	/*Close Socket*/
	closesocket(rqSock);
	closesocket(rpSock);
	closesocket(pfSock);
	closesocket(fgSock);
	CloseHandle(hMutex);
	WSACleanup();


	return 0;
}

void FHelp()
{
	printf("CHORD> Enter a command - (c)reate : Create the chord network\n");
	printf("CHORD> Enter a command - (j)oin : Join the chord network\n");
	printf("CHORD> Enter a command - (t)est : Test the chord network\n");
	printf("CHORD> Enter a command - (q)uit : Quit the program\n");
}
void SHelp()
{
	printf("CHORD> Enter a command - (h)elp : Show the help message\n");
	printf("CHORD> Enter a command - (a)dd : Add a file to the network\n");
	printf("CHORD> Enter a command - (d)elete : Delete a file to the network\n");
	printf("CHORD> Enter a command - (s)earch : File search and download\n");
	printf("CHORD> Enter a command - (f)inger : Show the finger table\n");
	printf("CHORD> Enter a command - (i)nfo : Show the node information\n");
	printf("CHORD> Enter a command - (m)ute : Toggle the silent mode\n");
	printf("CHORD> Enter a command - (l)eave : Leave the chord network\n");
	printf("CHORD> Enter a command - (q)uit : Quit the program\n");
}
void TestMode(void)
{
	system("cls");
	printf("***************************************************************\n");
	printf("*               DHT-Based P2P Protocol Controller             *\n");
	printf("*                  Ver.0.x     Dec. xx. 2018                  *\n");
	printf("*                       Test Mode                             *\n");
	printf("***************************************************************\n\n");


	printf("CHORD> Enter a command - (s)tart : start the chord network test \n");
	printf("CHORD> Enter a command - (q)uit : Quit the test mode \n");
}
void error_message(char *Message)
{
	printf("%s\n%s\n", Message, "[ERROR] Exit the program.... enter any key");
	//getc(stdin);
	//exit(1);
	return;
}
void procRecvMsg(void *arg)//rpsock binded
{
	chordHeaderType Node_Message;
	struct sockaddr_in opponent; //to save opponent's(sender) IP
	int opponent_size = sizeof(opponent);
	int iResult; //to save return value

	while (exitFlag)
	{
		iResult =
			recvfrom(rpSock, (char *)&Node_Message, sizeof(chordHeaderType), 0, (struct sockaddr*)&opponent, &opponent_size);
		if (iResult == SOCKET_ERROR)
			error_message("UDP recieve error!\n");

		/*Message processing*/
		if (Node_Message.msgID == JOIN_INFO)
		{
			//joininfo response

			if (sMode)
				printf("CHORD> Join info message from IP : %s Port : %d \n", inet_ntoa(opponent.sin_addr), ntohs(opponent.sin_port));
			Node_Message.nodeInfo.addrInfo = opponent;
			HANDLE join_thread;
			join_thread = (HANDLE)_beginthreadex(NULL, 0, (void*)Join_Info_Thread, (void*)&Node_Message.nodeInfo, 0, NULL);
			if (join_thread == 0)
				error_message("Join_Info_Thread _beginthreadex error!\n");

		}
		else if (Node_Message.msgID == MOVE_KEYS)
		{
			//movekeys response
			if (sMode)
				printf("CHORD> move keys message from IP : %s Port : %d \n", inet_ntoa(opponent.sin_addr), ntohs(opponent.sin_port));
				
			MoveKeys_Response(&Node_Message);
			iResult = sendto(rpSock, (char *)&Node_Message, sizeof(chordHeaderType), 0, (struct sockaddr*)&opponent, opponent_size);
			if (iResult == SOCKET_ERROR)
				error_message("MoveKeys Reponse send error!\n");
		}
		else if (Node_Message.msgID == PING_PONG)
		{
			/*pingpong request response*/
			if(sMode)
				printf("CHORD> PingPong message from IP : %s Port : %d \n", inet_ntoa(opponent.sin_addr), ntohs(opponent.sin_port));

			PingPong_Response(&Node_Message);
			iResult = sendto(rpSock, (char *)&Node_Message, sizeof(chordHeaderType), 0, (struct sockaddr*)&opponent, opponent_size);
			if (iResult == SOCKET_ERROR)
				error_message("Ping Pong Reponse send error!\n");


		}
		else if (Node_Message.msgID == PREDECESSOR_INFO)
		{
			//pred info response

			if (sMode)
				printf("CHORD> pred info message from IP : %s Port : %d \n", inet_ntoa(opponent.sin_addr), ntohs(opponent.sin_port));
			Predecessor_Info_Response(&Node_Message);
			iResult = sendto(rpSock, (char *)&Node_Message, sizeof(chordHeaderType), 0, (struct sockaddr*)&opponent, opponent_size);
			if (iResult == SOCKET_ERROR)
				error_message("Predecessor Info Reponse send error!\n");

		}
		else if (Node_Message.msgID == PREDECESSOR_UPDATE)
		{
			//pred update response
			if (sMode)
				printf("CHORD> pred update message from IP : %s Port : %d \n", inet_ntoa(opponent.sin_addr), ntohs(opponent.sin_port));

			WaitForSingleObject(hMutex, INFINITE);
			Usr_node.chordInfo.fingerInfo.Pre = Node_Message.nodeInfo;
			ReleaseMutex(hMutex);

			Predecessor_Update_Response(&Node_Message);
			iResult = sendto(rpSock, (char *)&Node_Message, sizeof(chordHeaderType), 0, (struct sockaddr*)&opponent, opponent_size);
			if (iResult == SOCKET_ERROR)
				error_message("Predecessor Update Reponse send error!\n");

			//추가
			
		}
		else if (Node_Message.msgID == SUCCESSOR_INFO)
		{
			//successor info response
			if (sMode)
				printf("CHORD> successor info  message from IP : %s Port : %d \n", inet_ntoa(opponent.sin_addr), ntohs(opponent.sin_port));
			Successor_Info_Response(&Node_Message);
			iResult = sendto(rpSock, (char *)&Node_Message, sizeof(chordHeaderType), 0, (struct sockaddr*)&opponent, opponent_size);
			if (iResult == SOCKET_ERROR)
				error_message("Successor Info Reponse send error!\n");
		}
		else if (Node_Message.msgID == SUCCESSOR_UPDATE)
		{
			//sucessor update responce
			if (sMode)
				printf("CHORD> successor update message from IP : %s Port : %d \n", inet_ntoa(opponent.sin_addr), ntohs(opponent.sin_port));

			WaitForSingleObject(hMutex, INFINITE);
			Usr_node.chordInfo.fingerInfo.finger[0] = Node_Message.nodeInfo;
			ReleaseMutex(hMutex);

			Successor_Update_Response(&Node_Message);
			iResult = sendto(rpSock, (char *)&Node_Message, sizeof(chordHeaderType), 0, (struct sockaddr*)&opponent, opponent_size);
			if (iResult == SOCKET_ERROR)
				error_message("Successor Update Reponse send error!\n");

		}
		else if (Node_Message.msgID == FIND_PREDCESSOR)
		{
			//find pred response
			if (sMode)
				printf("CHORD> find predecessor message from IP : %s Port : %d \n", inet_ntoa(opponent.sin_addr), ntohs(opponent.sin_port));


			/*스레드 아닐 때*/
			nodeInfoType pred;
			pred = find_predecessor(&Usr_node, Node_Message.moreInfo);//Key
			Find_Predecessor_Response(&Node_Message, pred);
			//printf("find pred  MsgId : %d 1 IP : %s Port : %d 에게 Key : %d 보냄\n ", Node_Message.msgID, inet_ntoa(opponent.sin_addr), ntohs(opponent.sin_port), Node_Message.nodeInfo.ID);
			iResult = sendto(rpSock, (char *)&Node_Message, sizeof(chordHeaderType), 0, (struct sockaddr*)&opponent, opponent_size);
			if (iResult == SOCKET_ERROR)
				error_message("Find Predecessor Reponse send error!\n");



			/*스레드*/
			/*	nodeInfoType pred;
			pred.addrInfo = opponent;
			pred.ID = Node_Message.moreInfo;

			//Find Process... Process at Thread.
			HANDLE find_pred_thread = malloc(sizeof(HANDLE));
			find_pred_thread = (HANDLE)_beginthreadex(NULL, 0, (void*)Find_Pred_Thread, (void*)&pred, 0, NULL);
			if (find_pred_thread == 0)
			error_message("Find_Pred_Thread _beginthreadex error!\n");*/
		}
		else if (Node_Message.msgID == LEAVE_KEYS)
		{
			//LeaveKeys response
			if (sMode)
				printf("CHORD> Leavekeys message from IP : %s Port : %d \n", inet_ntoa(opponent.sin_addr), ntohs(opponent.sin_port));

			WaitForSingleObject(hMutex, INFINITE);
			Usr_node.chordInfo.FRefInfo.fileRef[Usr_node.chordInfo.FRefInfo.fileNum++] = Node_Message.fileInfo;
			ReleaseMutex(hMutex);

			LeaveKeys_Response(&Node_Message);
			iResult = sendto(rpSock, (char *)&Node_Message, sizeof(chordHeaderType), 0, (struct sockaddr*)&opponent, opponent_size);
			if (iResult == SOCKET_ERROR)
				error_message("Leave Keys Reponse send error!\n");
		}
		else if (Node_Message.msgID == FILE_REF_ADD)
		{
			//file reference add response
			if (sMode)
				printf("CHORD> File reference add message from IP : %s Port : %d \n", inet_ntoa(opponent.sin_addr), ntohs(opponent.sin_port));

			WaitForSingleObject(hMutex, INFINITE);
			Usr_node.chordInfo.FRefInfo.fileRef[Usr_node.chordInfo.FRefInfo.fileNum] = Node_Message.fileInfo;
			Usr_node.chordInfo.FRefInfo.fileNum++;
			ReleaseMutex(hMutex);

			printf("CHORD> File reference add request recieved check your reference file list! \n");
			File_Reference_Add_Response(&Node_Message);
			iResult = sendto(rpSock, (char *)&Node_Message, sizeof(chordHeaderType), 0, (struct sockaddr*)&opponent, opponent_size);
			if (iResult == SOCKET_ERROR)
				error_message("File Reference add Reponse send error!\n");
		}
		else if (Node_Message.msgID == FILE_REF_DEL)
		{
			// file reference Delete response
			if (sMode)
				printf("CHORD> File reference deldete message from IP : %s Port : %d \n", inet_ntoa(opponent.sin_addr), ntohs(opponent.sin_port));
			int target;
			for (target = 0; target < Usr_node.chordInfo.FRefInfo.fileNum; target++)
				if (Usr_node.chordInfo.FRefInfo.fileRef[target].Key == Node_Message.fileInfo.Key)
					break;
			//target은 삭제할 파일을 가리키고있음...

			for (int i = target; i < Usr_node.chordInfo.FRefInfo.fileNum; i++)
				Usr_node.chordInfo.FRefInfo.fileRef[i] = Usr_node.chordInfo.FRefInfo.fileRef[i + 1];

			WaitForSingleObject(hMutex, INFINITE);
			Usr_node.chordInfo.FRefInfo.fileNum--;
			ReleaseMutex(hMutex);
			//내용 삭제.

			File_Reference_Delete_Response(&Node_Message);
			iResult = sendto(rpSock, (char *)&Node_Message, sizeof(chordHeaderType), 0, (struct sockaddr*)&opponent, opponent_size);
			if (iResult == SOCKET_ERROR)
				error_message("File Reference Delete Reponse send error!\n");
		}
		else if (Node_Message.msgID == FILE_DOWN)
		{
			// file down response
			if (sMode)
				printf("CHORD> File down message from IP : %s Port : %d \n", inet_ntoa(opponent.sin_addr), ntohs(opponent.sin_port));
			char Filename[FNameMax] = { 0 };
			int count = 0;
			strcpy(Filename, Node_Message.fileInfo.Name);
			for (count = 0; count < Usr_node.fileInfo.fileNum; count++)
				if (strcmp(Filename, Usr_node.fileInfo.fileRef[count].Name) == 0)
					break;



			//에러처리... 파일이 있으면,
			printf("-- Make FILE Thread --\n");
			HANDLE file_thread;
			file_thread = (HANDLE)_beginthreadex(NULL, 0, (void*)File_Send_Thread, (void*)Filename, 0, NULL);
			if (file_thread == 0)
				error_message("File_Send_Thread _beginthreadex error!\n");

			FileDown_Response(&Node_Message, count);
			iResult = sendto(rpSock, (char *)&Node_Message, sizeof(chordHeaderType), 0, (struct sockaddr*)&opponent, opponent_size);
			if (iResult == SOCKET_ERROR)
				error_message("File Down Reponse send error!\n");
		}
		else if (Node_Message.msgID == FILE_REF_INFO)
		{
			//file reference info response
			if (sMode)
				printf("CHORD> File reference info message from IP : %s Port : %d \n", inet_ntoa(opponent.sin_addr), ntohs(opponent.sin_port));
			int target;
			target = Node_Message.moreInfo;
			//만약 타겟이 파일넘보다 크면 에러처리
			File_Reference_Info_Response(&Node_Message, target);
			iResult = sendto(rpSock, (char *)&Node_Message, sizeof(chordHeaderType), 0, (struct sockaddr*)&opponent, opponent_size);
			if (iResult == SOCKET_ERROR)
				error_message("File Reference Info Reponse send error!\n");
		}
		else
		{
			//error between 1~13
			printf("메시지 수신 에러\n");
		}

	}
	printf("CHORD> UDP Thread 종료 \n");
	return;
}
void procPingPong(void *arg)
{
	chordHeaderType Node_Message;
	struct sockaddr_in opponent; //to save opponent's(sender) IP
	int opponent_size = sizeof(opponent);
	int retVal;
	int optVal = 3000; // 3 seconds
	int timeVal = 0;
	int iResult = 0;
	retVal = setsockopt(pfSock, SOL_SOCKET, SO_RCVTIMEO, (char*)&optVal, sizeof(optVal));
	while (exitFlag)
	{
		if (exitFlag)
		{
			for (int i = 0; i < baseM; i++)
			{
				//내자신이라면 보내지 말기
				if (Usr_node.chordInfo.fingerInfo.finger[i].ID == Usr_node.nodeInfo.ID) continue;
				PingPong_Request(&Node_Message);

				opponent = Usr_node.chordInfo.fingerInfo.finger[i].addrInfo;	//Address of fingers
				iResult = sendto(pfSock, (char *)&Node_Message, sizeof(chordHeaderType), 0, (struct sockaddr*)&opponent, opponent_size);
				if(sMode)
					printf("CHORD> Send pingpong  message to IP : %s Port : %d \n", inet_ntoa(opponent.sin_addr), ntohs(opponent.sin_port));
				iResult = recvfrom(pfSock, (char *)&Node_Message, sizeof(chordHeaderType), 0, (struct sockaddr*)&opponent, &opponent_size);
				if ((iResult == -1) && (Usr_node.chordInfo.fingerInfo.finger[i].ID == Usr_node.chordInfo.fingerInfo.finger[0].ID))
				{
					//WaitForSingleObject(hMutex, INFINITE);
					stabilize_leave(Usr_node.chordInfo.fingerInfo.finger[i].ID);
					//ReleaseMutex(hMutex);
				}
				if ((iResult == -1) && ((i > 0) && (i < baseM - 1)))
				{
					int leaveID = Usr_node.chordInfo.fingerInfo.finger[i].ID;

					for (int j = i; j < baseM; j++)
					{
						if (Usr_node.chordInfo.fingerInfo.finger[i].ID == Usr_node.chordInfo.fingerInfo.finger[j].ID)
						{
							if (j + 1 == baseM)
							{
								for (int k = i; k < baseM; k++)
									Usr_node.chordInfo.fingerInfo.finger[k] = Usr_node.chordInfo.fingerInfo.Pre;
							}
							continue;
						}

						for (int k = i; k < j; k++)
						{
							Usr_node.chordInfo.fingerInfo.finger[k] = Usr_node.chordInfo.fingerInfo.finger[j];
						}
					}

				}

				if ((iResult == -1) && (i == baseM - 1))
				{
					Usr_node.chordInfo.fingerInfo.finger[i] = Usr_node.chordInfo.fingerInfo.Pre;
				}
			}


			HANDLE fixfinger_thread;
			fixfinger_thread = (HANDLE)_beginthreadex(NULL, 0, (void*)procFixfinger, (void*)&exitFlag, 0, NULL);
			if (fixfinger_thread == 0)
				error_message("[ERROR]pp _beginthreadex error!\n");


			/*Timer*/
			timeVal = rand() % 1001 + 5000;
			Sleep(timeVal);

		}

	}
	printf("CHORD> 핑퐁 스레드 종료\n");
	return;
}
void procFixfinger(void *arg)
{
	if(sMode)
		printf("CHORD> Run fixfinger thread\n");
	fix_finger();
	
	return;
}
void File_Send_Thread(char *arg)
{
	/*DOWNLOAD*/

	char message[fBufSize] = { 0 };
	int numread, numtotal = 0;
	int msglen = 0;
	int file_totalbytes = 0;

	fsSock = socket(AF_INET, SOCK_STREAM, 0);
	if (fsSock == INVALID_SOCKET)
		error_message("TCP socket creation error!");

	flSock = socket(AF_INET, SOCK_STREAM, 0);
	if (flSock == INVALID_SOCKET)
		error_message("TCP socket creation error!");

	if (bind(flSock, (struct sockaddr *)&Usr_node.nodeInfo.addrInfo, sizeof(Usr_node.nodeInfo.addrInfo)) == INVALID_SOCKET)
		error_message("bind() error!");

	struct sockaddr_in opponent;
	int opponent_size = 0;
	opponent_size = sizeof(opponent);


	printf("CHORD> File thread start!\n");
	printf("CHORD> Downloading file name : %s\n", arg);
	if (listen(flSock, MAX_QUEUESIZE) <0)
		error_message("listen() error!!\n");
	//함수는 accecpt에서 멈춤

	fsSock = accept(flSock, (struct sockaddr*)&opponent, &opponent_size);
	if (fsSock < 0)
		error_message("accecpt() error!!\n");

	//파일 입출력
	FILE *file = fopen(arg, "rb");

	//파일 크기 보내기
	fseek(file, 0, SEEK_END);
	file_totalbytes = ftell(file);
	printf("File size : %d \n", file_totalbytes);
	if (send(fsSock, (char*)&file_totalbytes, sizeof(file_totalbytes), 0) == SOCKET_ERROR)
	{
		printf("SEND ERROR1!\n");
	}
	rewind(file);


	while (1)
	{
		numread = fread(message, 1, fBufSize, file);
		if (numread > 0)
		{
			if (send(fsSock, message, numread, 0) == SOCKET_ERROR)
			{
				//error
				printf("SEND ERROR!\n");
				break;
			}
			numtotal += numread;
		}
		else if (numread == 0 && numtotal == file_totalbytes)
		{
			printf("File sending is finished! : %d bytes\n", numtotal);
			break;
		}
		else
		{
			printf("파일 입출력 오류");
			break;
		}
	}


	fclose(file);
	closesocket(flSock);
	closesocket(fsSock);

	return;

}
void File_Recieve_Thread(fileRefType *arg)
{
	/*DOWNLOAD*/

	struct sockaddr_in opponent;
	char message[fBufSize] = { 0 };
	int opponent_size = 0;
	opponent_size = sizeof(opponent);

	int iResult = 0;
	int file_totalbytes = 0;
	int numtotal = 0;

	frSock = socket(AF_INET, SOCK_STREAM, 0);
	if (frSock == INVALID_SOCKET)
		error_message("TCP socket creation error!");



	printf("CHORD> The file that you want to download : %s\n", arg->Name);

	opponent = arg->owner.addrInfo;
	Sleep(1000);
	if (connect(frSock, (struct sockaddr *)&opponent, opponent_size) == SOCKET_ERROR)
	{
		printf("connect error!! \n");
		//error처리
	}
	
	//파일 크기 받음
	recv(frSock, &file_totalbytes, sizeof(file_totalbytes), 0);
	printf("CHORD> file size : %d \n", file_totalbytes);

	//파일 입출력
	FILE *file = fopen(arg->Name, "wb");
	

	while (1)
	{
		iResult = recv(frSock, message, fBufSize, 0);
		if (iResult == SOCKET_ERROR)
		{
			printf("받기 실패!@#\n");
			break;
		}
		else if (iResult == 0)
		{
			break;
		}
		else
		{
			fwrite(message, 1, fBufSize, file);
			if (ferror(file))
			{
				perror("파일 입출력 오류\n");
				break;
			}
			numtotal += iResult;
		}
	}
	fclose(file);
	printf("CHORD> numtotal : %d\n", numtotal);
	printf("CHORD> file_total : %d\n", file_totalbytes);
	if (numtotal == file_totalbytes)
	{
		printf("CHORD> File send finished! \n");
	}
	else
	{
		printf("CHORD> File send failed! \n");
	}

	printf("CHORD> FIle sender's Info IP : %s PORT : %d \n",
		inet_ntoa(arg->owner.addrInfo.sin_addr), ntohs(arg->owner.addrInfo.sin_port));

	closesocket(frSock);

	return;
}
void Join_Info_Thread(nodeInfoType *arg)
{
	chordHeaderType Node_Message;
	Node_Message.nodeInfo.ID = arg->ID;
	int iResult = 0;
	struct sockaddr_in opponent = arg->addrInfo;
	int opponent_size = sizeof(struct sockaddr_in);
	
	//printf("CHORD> Recieved Join info  message from IP : %s Port : %d \n", inet_ntoa(opponent.sin_addr), ntohs(opponent.sin_port));
	Join_Info_Response(&Node_Message);
	iResult = sendto(rpSock, (char *)&Node_Message, sizeof(chordHeaderType), 0, (struct sockaddr*)&opponent, opponent_size);
	if (iResult == SOCKET_ERROR)
		error_message("Join Info Reponse send error!\n");

	//fix_finger();

	return;

}
void Find_Pred_Thread(nodeInfoType *arg)
{
	struct sockaddr_in opponent = arg->addrInfo;
	int Key = arg->ID;

	if (exitFlag != 1)
		return;


	chordHeaderType Node_Message;
	int iResult = 0;

	int opponent_size = sizeof(struct sockaddr_in);


	nodeInfoType pred;

	pred = find_predecessor(&Usr_node, Key);//Key
	Find_Predecessor_Response(&Node_Message, pred);
	
	if (sMode)
		printf("Find Predecessor Response \n");

	iResult = sendto(fgSock, (char *)&Node_Message, sizeof(chordHeaderType), 0, (struct sockaddr*)&opponent, opponent_size);
	if (iResult == SOCKET_ERROR)
		error_message("Find Predecessor Reponse send error!\n");

	return;

}
unsigned int str_hash(const char *str)  /* Hash: String to Key */
{
	unsigned int len = strlen(str);
	unsigned int hash = len, i;


	for (i = 0; i != len; i++, str++) {
		hash ^= sTable[(*str + i) & 255];
		hash = hash * PRIME_MULT;
	}

	return hash % ringSize;
}
unsigned int str_double_hash(const char *str)
{
	unsigned int len = strlen(str);
	unsigned int hash = len, i;


	for (i = 0; i != len; i++, str++) {
		hash ^= dTable[(*str + i) & 255];
		hash = hash * PRIME_MULT;
	}

	return hash % ringSize;
}
int create(nodeType *Usr_node)
{

	Usr_node->chordInfo.fingerInfo.Pre = Usr_node->nodeInfo;
	for (int i = 0; i < baseM; i++)
		Usr_node->chordInfo.fingerInfo.finger[i] = Usr_node->nodeInfo;

	return SUCCESS;
}
int join(char *HashingKey)
{
	chordHeaderType message;
	struct sockaddr_in helper;
	int iResult = 0;//to check return value;
	struct sockaddr_in opponent;
	int helper_size = sizeof(helper);
	int opponent_size = sizeof(opponent);
	char ip[15] = { 0 }, port[6] = { 0 };

	printf("CHORD> Helper's IP : ");
	scanf(" %s", &ip);
	rewind(stdin);

	printf("CHORD> Helper's PORT : ");
	scanf(" %s", &port);
	rewind(stdin);

	memset(&helper, 0, sizeof(helper));
	helper.sin_family = AF_INET;
	helper.sin_addr.s_addr = inet_addr(ip);
	helper.sin_port = htons(atoi(port));


	/*Initialize*/
	memset(&Usr_node.chordInfo.fingerInfo.Pre, 0, sizeof(Usr_node.chordInfo.fingerInfo.Pre)); //pre 초기화
																							  //보낼 메시지 초기화
	Join_Info_Request(&message);
	iResult = sendto(rqSock, (char*)&message, sizeof(chordHeaderType), 0, (struct sockaddr*) &helper, helper_size);
	if (iResult == SOCKET_ERROR)
		error_message("Join info Request send error!\n");
	if (sMode)
		printf("CHORD > JoinInfo request Massage has been sent.\n");
	//join info response
	iResult = recvfrom(rqSock, (char *)&message, sizeof(chordHeaderType), 0,
		(struct sockaddr*)&helper, &helper_size);//헬퍼한테서 석세서노드
	if (iResult == SOCKET_ERROR)
		error_message("Join info Request recvfrom error!\n");
	if (sMode)
		printf("CHORD > JoinInfo response Massage has been recieved.\n");
	Usr_node.chordInfo.fingerInfo.finger[0] = message.nodeInfo;//받은 데이터로 저장



	 /*To avoid Hashing collision - double hashing*/
	Find_Predecessor_Request(&message, Usr_node.nodeInfo.ID);

	iResult = sendto(rqSock, (char*)&message, sizeof(chordHeaderType), 0, (struct sockaddr*) &helper, helper_size);
	if (iResult == SOCKET_ERROR)
		error_message("Join info Request send error!\n");
	if (sMode)
		printf("CHORD > Find predecessor request Massage has been sent.\n");

	iResult = recvfrom(rqSock, (char *)&message, sizeof(chordHeaderType), 0,
		(struct sockaddr*)&helper, &helper_size);//헬퍼한테서 석세서노드
	if (iResult == SOCKET_ERROR)
		error_message("Join info Request recvfrom error!\n");
	if (sMode)
		printf("CHORD > Find predecessor response Massage has been recieved.\n");

	if (Usr_node.nodeInfo.ID == message.nodeInfo.ID)	//충돌
	{
		int i = 0;
		for (i = 1; i < ringSize; i++)
		{
			Usr_node.nodeInfo.ID = (str_hash(HashingKey) + i*str_double_hash(HashingKey)) % ringSize;	//새로운 Key설정

																										/*중복 확인하기*/
			Find_Predecessor_Request(&message, Usr_node.nodeInfo.ID);

			iResult = sendto(rqSock, (char*)&message, sizeof(chordHeaderType), 0, (struct sockaddr*) &helper, helper_size);
			if (iResult == SOCKET_ERROR)
				error_message("Join info Request send error!\n");
			if (sMode)
				printf("CHORD > Find predecessor request Massage has been sent.\n");

			iResult = recvfrom(rqSock, (char *)&message, sizeof(chordHeaderType), 0,
				(struct sockaddr*)&helper, &helper_size);//헬퍼한테서 석세서노드
			if (iResult == SOCKET_ERROR)
				error_message("Join info Request recvfrom error!\n");
			if (sMode)
				printf("CHORD > Find predecessor response Massage has been recieved.\n");

			if (Usr_node.nodeInfo.ID != message.nodeInfo.ID)
				break;
		}

		if (i == ringSize)
			error_message("There is no enough space you join.\n");

	}
	/*Hashing collision end*/

	/*Initialize end*/


	/*movekeys*/
	move_keys(&Usr_node, &Usr_node.chordInfo.fingerInfo.finger[0]);
	/*movekeys*/

	/*stabilize*/
	//WaitForSingleObject(hMutex, INFINITE);

	Predecessor_Info_Request(&message);
	iResult = sendto(rqSock, (char*)&message, sizeof(chordHeaderType), 0, (struct sockaddr*) &Usr_node.chordInfo.fingerInfo.finger[0].addrInfo, opponent_size);
	if (iResult == SOCKET_ERROR)
		error_message("Predecessor info Request send error!\n");
	if (sMode)
		printf("CHORD > Predecessor Info request Massage has been sent.\n");
	//printf("Main predecessor info  MsgId : %d 1 IP : %s Port : %d 에게 보냄\n ", message.msgID, inet_ntoa(Usr_node.chordInfo.fingerInfo.finger[0].addrInfo.sin_addr), ntohs(Usr_node.chordInfo.fingerInfo.finger[0].addrInfo.sin_port));
	iResult = recvfrom(rqSock, (char *)&message, sizeof(chordHeaderType), 0,
		(struct sockaddr*)&opponent, &opponent_size);
	if (iResult == SOCKET_ERROR)
		error_message("Predecessor info Request recvfrom error!\n");
	if (sMode)
		printf("CHORD > Predecessor Info response Massage has been recieved.\n");

	//printf("Main predecessor info  MsgId : %d 1 IP : %s Port : %d 에게 보냄\n ", message.msgID, inet_ntoa(opponent.sin_addr), ntohs(opponent.sin_port));
	Usr_node.chordInfo.fingerInfo.Pre = message.nodeInfo;
	opponent = message.nodeInfo.addrInfo;

	Successor_Update_Request(&message);
	iResult = sendto(rqSock, (char*)&message, sizeof(chordHeaderType), 0, (struct sockaddr*) &opponent, opponent_size);
	if (iResult == SOCKET_ERROR)
		error_message("Successor update Request send error!\n");
	if (sMode)
		printf("CHORD > Successor Update request Massage has been sent.\n");

	iResult = recvfrom(rqSock, (char *)&message, sizeof(chordHeaderType), 0,
		(struct sockaddr*)&opponent, &opponent_size);
	if (iResult == SOCKET_ERROR)
		error_message("Successor update Request recvfrom error!\n");
	if (sMode)
		printf("CHORD > Successor Update response Massage has been recieved.\n");


	Predecessor_Update_Request(&message);
	iResult = sendto(rqSock, (char*)&message, sizeof(chordHeaderType), 0, (struct sockaddr*) &Usr_node.chordInfo.fingerInfo.finger[0].addrInfo, opponent_size);
	if (iResult == SOCKET_ERROR)
		error_message("Predecessor update Request send error!\n");
	if (sMode)
		printf("CHORD > Predecessor Update request Massage has been sent.\n");
	iResult = recvfrom(rqSock, (char *)&message, sizeof(chordHeaderType), 0,
		(struct sockaddr*)&opponent, &opponent_size);
	if (iResult == SOCKET_ERROR)
		error_message("Predecessor update Request send error!\n");
	if (sMode)
		printf("CHORD > Predecessor Update response Massage has been recieved.\n");

	/*stabilize*/

	fix_finger();

	if (sMode)
		printf("CHORD > Node Join has been successfully finished. \n");


	return 0;
}
int leave(void)
{

	/*Processing at leaving Node*/
	int iResult = 0;
	struct sockaddr_in opponent;
	int opponent_size = sizeof(opponent);
	chordHeaderType message;

	/*Leave Keys Request*/
	opponent = Usr_node.chordInfo.fingerInfo.finger[0].addrInfo;	//successor
	int count = Usr_node.chordInfo.FRefInfo.fileNum;
	for (int i = 0; i < count; i++)
	{
		LeaveKeys_Request(&message);
		iResult = sendto(rqSock, (char*)&message, sizeof(chordHeaderType), 0, (struct sockaddr*) &Usr_node.chordInfo.fingerInfo.finger[0].addrInfo, opponent_size);
		if (iResult == SOCKET_ERROR)
			error_message("LeaveKeys Request send error!\n");
		if (sMode)
			printf("CHORD > LeaveKeys request Massage has been sent.\n");
		iResult = recvfrom(rqSock, (char *)&message, sizeof(chordHeaderType), 0, (struct sockaddr*)&opponent, &opponent_size);
		if (iResult == SOCKET_ERROR)
			error_message("LeaveKeys Request send error!\n");
		if (sMode)
			printf("CHORD > LeaveKeys response Massage has been recieved.\n");
		
		Usr_node.chordInfo.FRefInfo.fileNum--;
	}
	/*Leave Keys Finished*/
	count = Usr_node.fileInfo.fileNum;
	/*File Delete Request*/
	for (unsigned int i = 0; i < count; i++)
	{
		opponent = Usr_node.fileInfo.fileRef[Usr_node.fileInfo.fileNum - 1].refOwner.addrInfo;
		Usr_node.fileInfo.fileNum--;
		File_Reference_Delete_Request(&message, i);
		iResult = sendto(rqSock, (char*)&message, sizeof(chordHeaderType), 0, (struct sockaddr*) &opponent, opponent_size);
		if (iResult == SOCKET_ERROR)
			error_message("File Reference Delete Request send error!\n");
		if (sMode)
			printf("CHORD > File Reference Delete request Massage has been sent.\n");
		iResult = recvfrom(rqSock, (char *)&message, sizeof(chordHeaderType), 0, (struct sockaddr*)&opponent, &opponent_size);
		if (iResult == SOCKET_ERROR)
			error_message("File Reference Delete Request send error!\n");
		if (sMode)
			printf("CHORD > File Reference Delete response Massage has been recieved.\n");

	}
	/*File Delete Request finished*/



	exitFlag = 0; //To close thread..

	return 0;
}
void stabilize_leave(int leaveID)
{
	//At Predecessor..
	struct sockaddr_in opponent;
	int opponent_size = sizeof(opponent);
	chordHeaderType message;
	int iResult = 0;
	int opponentID = 0;

	int p_succ = 0;
	for (p_succ = 0; p_succ < baseM; p_succ++)
		if (Usr_node.chordInfo.fingerInfo.finger[0].ID != Usr_node.chordInfo.fingerInfo.finger[p_succ].ID)
			break;
	//p_ucc는 나간 노드와 가장 가까운 핑거 (내 기준)
	/*stabilize*/
	if (sMode)
		printf("CHORD >Leave stablize run\n");

	if (Usr_node.chordInfo.fingerInfo.Pre.ID == Usr_node.chordInfo.fingerInfo.finger[0].ID)
	{
		//둘이 있는경우
		for (int i = 0; i <baseM; i++)
			Usr_node.chordInfo.fingerInfo.finger[i] = Usr_node.nodeInfo;
		Usr_node.chordInfo.fingerInfo.Pre = Usr_node.nodeInfo;

		return;
	}
	else if (p_succ == baseM)
	{
		// 단 둘이 있는 것 같은경우
		Usr_node.chordInfo.fingerInfo.finger[p_succ - 1] = Usr_node.chordInfo.fingerInfo.Pre;
		//석세서 빼고.. 비교대상을 위해
		for (int i = p_succ - 1; i > 1; i--)
			Usr_node.chordInfo.fingerInfo.finger[i - 1] = Usr_node.chordInfo.fingerInfo.finger[i];

		opponent = Usr_node.chordInfo.fingerInfo.Pre.addrInfo;
		opponentID = Usr_node.chordInfo.fingerInfo.Pre.ID;
	}
	else
	{
		//여러 노드인 경우
		opponent = Usr_node.chordInfo.fingerInfo.finger[p_succ].addrInfo;
		opponentID = Usr_node.chordInfo.fingerInfo.finger[p_succ].ID;
	}
	do
	{

		Predecessor_Info_Request(&message);
		iResult = sendto(pfSock, (char*)&message, sizeof(chordHeaderType), 0, (struct sockaddr*) &opponent, opponent_size);

		if (iResult == SOCKET_ERROR)
			error_message("Predecessor Info Request send error!\n");

		iResult = recvfrom(pfSock, (char *)&message, sizeof(chordHeaderType), 0,
			(struct sockaddr*)&opponent, &opponent_size);

		if (iResult == SOCKET_ERROR)
			error_message("Predecessor Info Request send error!\n");

		if (message.nodeInfo.ID == Usr_node.chordInfo.fingerInfo.finger[0].ID)
			break;

		opponent = message.nodeInfo.addrInfo;//leave node's succ..
		opponentID = message.nodeInfo.ID;

	} while (message.nodeInfo.ID != Usr_node.chordInfo.fingerInfo.finger[0].ID);


	/*Set my Successor*/

	Usr_node.chordInfo.fingerInfo.finger[0].ID = opponentID;//opponent의 ID
	Usr_node.chordInfo.fingerInfo.finger[0].addrInfo = opponent;


	/*Set my Finger*/
	for (int i = p_succ; i > 1; i--)
		Usr_node.chordInfo.fingerInfo.finger[i - 1] = Usr_node.chordInfo.fingerInfo.finger[p_succ];


	/*Set another nods's Predecessor*/

	Predecessor_Update_Request(&message);

	iResult = sendto(pfSock, (char*)&message, sizeof(chordHeaderType), 0, (struct sockaddr*) &opponent, opponent_size);
	if (iResult == SOCKET_ERROR)
		error_message("Predecessor update Request send error!\n");
	iResult = recvfrom(pfSock, (char *)&message, sizeof(chordHeaderType), 0, (struct sockaddr*)&opponent, &opponent_size);
	if (iResult == SOCKET_ERROR)
		error_message("Predecessor update Request send error!\n");

	/*stabilize_end*/


	/*Fixfinger*/

	fix_finger();

	printf("CHORD> Leave stablize end\n");

}
void deletefile(void)
{
	char FileName[FNameMax] = { 0 };
	unsigned int target;
	int iResult = 0;
	struct sockaddr_in opponent;
	int opponent_size = sizeof(opponent);
	chordHeaderType message;

	printf("CHORD > Enter the delete file name : ");
	scanf(" %s", FileName);
	rewind(stdin);

	for (target = 0; target < Usr_node.fileInfo.fileNum; target++)
		if (strcmp(Usr_node.fileInfo.fileRef[target].Name, FileName) == 0)
			break;

	//error... if uesr enter the wrong file name. 
	if (target == Usr_node.fileInfo.fileNum)
	{
		printf("CHORD > Wrong file name! Check the file name! \n");
		return;
	}
	//Target은 삭제할 노드를 가리키고있다.

	/*Reference owner Data Delete Process*/
	opponent = Usr_node.fileInfo.fileRef[target].refOwner.addrInfo;
	File_Reference_Delete_Request(&message, target);
	iResult = sendto(rqSock, (char*)&message, sizeof(chordHeaderType), 0, (struct sockaddr*) &opponent, opponent_size);
	if (iResult == SOCKET_ERROR)
		error_message("[ERROR] File Reference Delete Request send error!\n");
	if (sMode)
		printf("CHORD > File Reference Delete request Massage has been sent.\n");

	iResult = recvfrom(rqSock, (char *)&message, sizeof(chordHeaderType), 0, (struct sockaddr*)&opponent, &opponent_size);
	if (iResult == SOCKET_ERROR)
		error_message("[ERROR] File Reference Delete Request send error!\n");
	if (sMode)
		printf("CHORD > File Reference Delete response Massage has been recieved.\n");
	if (sMode)
		printf("CHORD > Reference file is successfully removed.\n");

	/*Owner Data Information Delete Process*/
	WaitForSingleObject(hMutex, INFINITE);
	for (int i = target; i < Usr_node.fileInfo.fileNum; i++)
		Usr_node.fileInfo.fileRef[i] = Usr_node.fileInfo.fileRef[i + 1];
	Usr_node.fileInfo.fileNum--;
	ReleaseMutex(hMutex);

	if (sMode)
		printf("CHORD > File own data is successfully removed.\n");
	/*end*/
}
void addfile(void)
{
	//finished

	char FileName[FNameMax] = { 0 };
	int FileKey = 0;
	nodeInfoType FRef_succ;
	int iResult = 0;
	struct sockaddr_in opponent;
	int opponent_size = sizeof(opponent);
	chordHeaderType message;

	printf("CHORD> Files to be added must be in the same folder where this program is located\n");
	printf("CHORD> Note that the maximum file name size is %d\n", FNameMax);
	printf("CHORD> Enter the file name to add : ");
	scanf(" %s", FileName);
	rewind(stdin);

	//if it doesn't exist
	FILE *file = fopen(FileName, "rb");
	if (file == NULL)
	{
		printf("[ERROR] The file '%s' is not in the same folder where this program is!\n", FileName);
		return;
	}
	else
	{
		fclose(file);
	}


	FileKey = str_hash(FileName); //Get Key..
	printf("CHORD> Input File Name : %s, Key : %d\n", FileName, FileKey);

	FRef_succ = find_successor(&Usr_node, FileKey);//Find successor with key
	printf("CHORD> File Successor IP Addr : %s Port No : %d, ID : %d \n", inet_ntoa(FRef_succ.addrInfo.sin_addr), ntohs(FRef_succ.addrInfo.sin_port), FRef_succ.ID);

	WaitForSingleObject(hMutex, INFINITE);
	//Set own file information
	strcpy(Usr_node.fileInfo.fileRef[Usr_node.fileInfo.fileNum].Name, FileName);
	Usr_node.fileInfo.fileRef[Usr_node.fileInfo.fileNum].Key = FileKey;
	Usr_node.fileInfo.fileRef[Usr_node.fileInfo.fileNum].owner = Usr_node.nodeInfo;
	Usr_node.fileInfo.fileRef[Usr_node.fileInfo.fileNum].refOwner = FRef_succ;
	Usr_node.fileInfo.fileNum++;
	ReleaseMutex(hMutex);

	if (sMode)
		printf("CHORD> Modifying own information is finished\n");

	//send Message
	opponent = FRef_succ.addrInfo;
	File_Reference_Add_Request(&message);
	iResult = sendto(rqSock, (char*)&message, sizeof(chordHeaderType), 0, (struct sockaddr*) &opponent, opponent_size);
	if (iResult == SOCKET_ERROR)
		error_message("File Reference Add Request send error!\n");
	recvfrom(rqSock, (char *)&message, sizeof(chordHeaderType), 0, (struct sockaddr*)&opponent, &opponent_size);
	if (iResult == SOCKET_ERROR)
		error_message("File Reference Add Request send error!\n");
	if (sMode)
		printf("CHORD> File Ref Info has been sent successfully to the Successor.\n");
	if (sMode)
		printf("CHORD> File add has been successfully finished.\n\n");
}
/*void notify(nodeType *askNode, nodeType *targetNode)
{
nodeInfoType *Pre = &(askNode->chordInfo.fingerInfo.Pre);

if ((Pre == NULL) || (Pre == askNode) || modIn(ringSize, targetNode->nodeInfo.ID, Pre->ID, askNode->nodeInfo.ID, 0, 0))
askNode->chordInfo.fingerInfo.Pre = targetNode->nodeInfo;
}*/
int lookup(nodeType *curNode, int fileKey, nodeInfoType *foundNode)
{
	int i;
	int Store = 0;


	for (i = 0; i<curNode->fileInfo.fileNum; i++)
		if (fileKey == curNode->fileInfo.fileRef[i].Key)
			return 1; /* Store the File itself */

	for (i = 0; i<curNode->chordInfo.FRefInfo.fileNum; i++)
		if (fileKey == curNode->chordInfo.FRefInfo.fileRef[i].Key) {
			return 2; /* Store the File Reference itself */
		}

	*foundNode = find_successor(curNode, fileKey);

	return 0; /* Success */
}


int move_keys(nodeType *JoinNode, nodeInfoType *SuccNode)
{
	chordHeaderType message;
	struct sockaddr_in opponent; //to save opponent's(sender) IP
	int opponent_size = sizeof(opponent);	//
	int iResult = 0;
	int i, KeysCount = 0;

	MoveKeys_Request(&message);

	iResult = sendto(rqSock, (char*)&message, sizeof(chordHeaderType), 0, (struct sockaddr*) &Usr_node.chordInfo.fingerInfo.finger[0].addrInfo, opponent_size);
	if (iResult == SOCKET_ERROR)
		error_message("MoveKeys Request send error!\n");
	if (sMode)
		printf("CHORD > MoveKeys request Massage has been sent.\n");
	recvfrom(rqSock, (char *)&message, sizeof(chordHeaderType), 0, (struct sockaddr*)&opponent, &opponent_size);
	if (iResult == SOCKET_ERROR)
		error_message("MoveKeys Request recieve error!\n");
	if (sMode)
		printf("CHORD > MoveKeys response Massage has been recieved.\n");
	KeysCount = message.moreInfo; //Success Node의 Keys개수

	for (i = 0; i < KeysCount; i++)
	{
		File_Reference_Info_Request(&message, i);
		iResult = sendto(rqSock, (char*)&message, sizeof(chordHeaderType), 0, (struct sockaddr*) &Usr_node.chordInfo.fingerInfo.finger[0].addrInfo, opponent_size);
		if (iResult == SOCKET_ERROR)
			error_message("File Reference Info Request send error!\n");
		if (sMode)
			printf("CHORD > File Reference Info request Massage has been sent.\n");
		iResult = recvfrom(rqSock, (char *)&message, sizeof(chordHeaderType), 0, (struct sockaddr*)&opponent, &opponent_size);
		if (iResult == SOCKET_ERROR)
			error_message("File Reference Info Request send error!\n");
		if (sMode)
			printf("CHORD > File Reference Info response Massage has been recieved.\n");

		if (modIn(ringSize, JoinNode->nodeInfo.ID, message.fileInfo.Key, SuccNode->ID, 1, 0))
		{
			if (JoinNode->fileInfo.fileNum + 1 > nodefmax)
			{
				printf("==== Error: nodefmax(=%d) limit exceeded! ====\n", nodefmax);
				return -1;
			}
			JoinNode->chordInfo.FRefInfo.fileRef[JoinNode->chordInfo.FRefInfo.fileNum]
				= message.fileInfo;
			JoinNode->chordInfo.FRefInfo.fileNum++;

			File_Reference_Delete_Request(&message, i);
			iResult = sendto(rqSock, (char*)&message, sizeof(chordHeaderType), 0, (struct sockaddr*) &Usr_node.chordInfo.fingerInfo.finger[0].addrInfo, opponent_size);
			if (iResult == SOCKET_ERROR)
				error_message("File Reference Delete Request send error!\n");
			if (sMode)
				printf("CHORD > File Reference Delete request Massage has been sent.\n");
			iResult = recvfrom(rqSock, (char *)&message, sizeof(chordHeaderType), 0, (struct sockaddr*)&opponent, &opponent_size);
			if (iResult == SOCKET_ERROR)
				error_message("File Reference Delete Request send error!\n");
			if (sMode)
				printf("CHORD > File Reference Delete reponse Massage has been recieved.\n");
		}
	}

	return 0;
}

//response의 moreinfo에 성공실패 넣기.
void Join_Info_Request(chordHeaderType *Node_Message)
{
	Node_Message->msgID = JOIN_INFO;
	Node_Message->msgType = REQUEST;
	Node_Message->nodeInfo = Usr_node.nodeInfo;
	Node_Message->moreInfo = 0;
	Node_Message->bodySize = 0;
}
void Join_Info_Response(chordHeaderType *Node_Message)
{
	nodeInfoType successor;
	successor = find_successor(&Usr_node, Node_Message->nodeInfo.ID);

	Node_Message->msgID = JOIN_INFO;
	Node_Message->msgType = RESPONSE;	
	Node_Message->moreInfo = SUCCESS;	
	Node_Message->nodeInfo = successor;
	Node_Message->bodySize = 0;
}
void MoveKeys_Request(chordHeaderType *Node_Message)
{
	Node_Message->msgID = MOVE_KEYS;
	Node_Message->msgType = REQUEST;
	Node_Message->nodeInfo = Usr_node.nodeInfo;
	Node_Message->moreInfo = 0;
	Node_Message->bodySize = 0;
}
void MoveKeys_Response(chordHeaderType *Node_Message)
{
	Node_Message->msgID = MOVE_KEYS;
	Node_Message->msgType = RESPONSE;
	Node_Message->nodeInfo = Usr_node.nodeInfo;
	Node_Message->moreInfo = Usr_node.chordInfo.FRefInfo.fileNum;
	Node_Message->bodySize = 0;
}
void PingPong_Request(chordHeaderType *Node_Message)
{
	Node_Message->msgID = PING_PONG;
	Node_Message->msgType = REQUEST;
	Node_Message->moreInfo = FAILURE;
	Node_Message->nodeInfo = Usr_node.nodeInfo;
	Node_Message->bodySize = 0;
}
void PingPong_Response(chordHeaderType *Node_Message)
{
	Node_Message->msgID = PING_PONG;
	Node_Message->msgType = RESPONSE;
	Node_Message->nodeInfo = Usr_node.nodeInfo;
	Node_Message->moreInfo = SUCCESS;
	Node_Message->bodySize = 0;
}
void Predecessor_Info_Request(chordHeaderType *Node_Message)
{
	Node_Message->msgID = PREDECESSOR_INFO;
	Node_Message->msgType = REQUEST;
	Node_Message->nodeInfo = Usr_node.nodeInfo;
	Node_Message->moreInfo = 0;
	Node_Message->bodySize = 0;
}
void Predecessor_Info_Response(chordHeaderType *Node_Message)
{
	Node_Message->msgID = PREDECESSOR_INFO;
	Node_Message->msgType = RESPONSE;
	Node_Message->nodeInfo = Usr_node.chordInfo.fingerInfo.Pre;
	Node_Message->moreInfo = 0;
	Node_Message->bodySize = 0;
}
void Predecessor_Update_Request(chordHeaderType *Node_Message)
{
	Node_Message->msgID = PREDECESSOR_UPDATE;
	Node_Message->msgType = REQUEST;
	Node_Message->nodeInfo = Usr_node.nodeInfo;
	Node_Message->moreInfo = 0;
	Node_Message->bodySize = 0;
}
void Predecessor_Update_Response(chordHeaderType *Node_Message)
{
	Node_Message->msgID = PREDECESSOR_UPDATE;
	Node_Message->msgType = RESPONSE;
	Node_Message->nodeInfo = Usr_node.nodeInfo;
	Node_Message->moreInfo = SUCCESS;
	Node_Message->bodySize = 0;
}
void Successor_Info_Request(chordHeaderType *Node_Message)
{
	Node_Message->msgID = SUCCESSOR_INFO;
	Node_Message->msgType = REQUEST;
	Node_Message->nodeInfo = Usr_node.nodeInfo;
	Node_Message->moreInfo = 0;
	Node_Message->bodySize = 0;
}
void Successor_Info_Response(chordHeaderType *Node_Message)
{
	Node_Message->msgID = SUCCESSOR_INFO;
	Node_Message->msgType = RESPONSE;
	Node_Message->nodeInfo = Usr_node.chordInfo.fingerInfo.finger[0];
	Node_Message->moreInfo = 0;
	Node_Message->bodySize = 0;
}
void Successor_Update_Request(chordHeaderType *Node_Message)
{
	Node_Message->msgID = SUCCESSOR_UPDATE;
	Node_Message->msgType = REQUEST;
	Node_Message->nodeInfo = Usr_node.nodeInfo;
	Node_Message->moreInfo = 0;
	Node_Message->bodySize = 0;
}
void Successor_Update_Response(chordHeaderType *Node_Message)
{
	Node_Message->msgID = SUCCESSOR_UPDATE;
	Node_Message->msgType = RESPONSE;
	Node_Message->nodeInfo = Usr_node.nodeInfo;
	Node_Message->moreInfo = SUCCESS;
	Node_Message->bodySize = 0;
}
void Find_Predecessor_Request(chordHeaderType *Node_Message, int Key)
{
	Node_Message->msgID = FIND_PREDCESSOR;
	Node_Message->msgType = REQUEST;
	Node_Message->nodeInfo = Usr_node.nodeInfo;
	Node_Message->moreInfo = Key;	
	Node_Message->bodySize = 0;
}
void Find_Predecessor_Response(chordHeaderType *Node_Message, nodeInfoType pred)
{
	Node_Message->msgID = FIND_PREDCESSOR;
	Node_Message->msgType = RESPONSE;
	Node_Message->nodeInfo = pred;
	Node_Message->moreInfo = Usr_node.nodeInfo.ID;
	Node_Message->bodySize = 0;
}
void LeaveKeys_Request(chordHeaderType *Node_Message)
{
	Node_Message->msgID = LEAVE_KEYS;
	Node_Message->msgType = REQUEST;
	Node_Message->nodeInfo = Usr_node.nodeInfo;
	Node_Message->moreInfo = Usr_node.chordInfo.FRefInfo.fileNum;
	Node_Message->fileInfo = Usr_node.chordInfo.FRefInfo.fileRef[Usr_node.chordInfo.FRefInfo.fileNum - 1];
	Node_Message->bodySize = 0;
}
void LeaveKeys_Response(chordHeaderType *Node_Message)
{
	Node_Message->msgID = LEAVE_KEYS;
	Node_Message->msgType = RESPONSE;
	Node_Message->nodeInfo = Usr_node.nodeInfo;
	Node_Message->moreInfo = SUCCESS;
	Node_Message->bodySize = 0;
}
void File_Reference_Add_Request(chordHeaderType *Node_Message)
{
	Node_Message->msgID = FILE_REF_ADD;
	Node_Message->msgType = REQUEST;
	Node_Message->nodeInfo = Usr_node.nodeInfo;
	Node_Message->moreInfo = Usr_node.fileInfo.fileNum;
	Node_Message->fileInfo = Usr_node.fileInfo.fileRef[Usr_node.fileInfo.fileNum - 1];
	Node_Message->bodySize = 0;
}
void File_Reference_Add_Response(chordHeaderType *Node_Message)
{
	Node_Message->msgID = FILE_REF_ADD;
	Node_Message->msgType = RESPONSE;
	Node_Message->nodeInfo = Usr_node.nodeInfo;
	Node_Message->moreInfo = SUCCESS;
	Node_Message->bodySize = 0;
}
void File_Reference_Delete_Request(chordHeaderType *Node_Message, int target)
{
	Node_Message->msgID = FILE_REF_DEL;
	Node_Message->msgType = REQUEST;//request
	Node_Message->nodeInfo = Usr_node.nodeInfo;
	Node_Message->moreInfo = target;
	Node_Message->fileInfo = Usr_node.fileInfo.fileRef[target];
	Node_Message->bodySize = 0;
}
void File_Reference_Delete_Response(chordHeaderType *Node_Message)
{
	Node_Message->msgID = FILE_REF_DEL;
	Node_Message->msgType = RESPONSE;
	Node_Message->nodeInfo = Usr_node.nodeInfo;
	Node_Message->moreInfo = SUCCESS;
	Node_Message->bodySize = 0;
}
void FileDown_Request(chordHeaderType *Node_Message, char *Filename)
{
	Node_Message->msgID = FILE_DOWN;
	Node_Message->msgType = REQUEST;
	Node_Message->nodeInfo = Usr_node.nodeInfo;
	strcpy(Node_Message->fileInfo.Name, Filename);
	Node_Message->bodySize = 0;
}
void FileDown_Response(chordHeaderType *Node_Message, int target)
{
	Node_Message->msgID = FILE_DOWN;
	Node_Message->msgType = RESPONSE;
	Node_Message->nodeInfo = Usr_node.nodeInfo;
	Node_Message->moreInfo = SUCCESS;
	Node_Message->fileInfo = Usr_node.fileInfo.fileRef[target];
	Node_Message->bodySize = 0;
}
void File_Reference_Info_Request(chordHeaderType *Node_Message, int target)
{
	Node_Message->msgID = FILE_REF_INFO;
	Node_Message->msgType = REQUEST;
	Node_Message->nodeInfo = Usr_node.nodeInfo;
	Node_Message->moreInfo = target;
	Node_Message->bodySize = 0;
}
void File_Reference_Info_Response(chordHeaderType *Node_Message, int target)
{
	Node_Message->msgID = FILE_REF_INFO;
	Node_Message->msgType = RESPONSE;
	Node_Message->nodeInfo = Usr_node.nodeInfo;
	Node_Message->moreInfo = Usr_node.chordInfo.FRefInfo.fileNum;
	Node_Message->fileInfo = Usr_node.chordInfo.FRefInfo.fileRef[target];
	Node_Message->bodySize = 0;
}

int modIn(int modN, int targNum, int range1, int range2, int leftmode, int rightmode)
{
	// leftmode, rightmode: 0 => range boundary not included, 1 => range boundary included   
	int result = 0;

	if (range1 == range2) {
		if ((leftmode == 0) || (rightmode == 0))
			return 0;
	}

	if (modPlus(ringSize, range1, 1) == range2) {
		if ((leftmode == 0) && (rightmode == 0))
			return 0;
	}

	if (leftmode == 0)
		range1 = modPlus(ringSize, range1, 1);
	if (rightmode == 0)
		range2 = modMinus(ringSize, range2, 1);

	if (range1  < range2) {
		if ((targNum >= range1) && (targNum <= range2))
			result = 1;
	}
	else if (range1 > range2) {
		if (((targNum >= range1) && (targNum < modN))
			|| ((targNum >= 0) && (targNum <= range2)))
			result = 1;
	}
	else if ((targNum == range1) && (targNum == range2))
		result = 1;

	return result;
}
int twoPow(int power)
{
	int i;
	int result = 1;

	if (power >= 0)
		for (i = 0; i<power; i++)
			result *= 2;
	else
		result = -1;

	return result;
}
int modMinus(int modN, int minuend, int subtrand)
{
	if (minuend - subtrand >= 0)
		return minuend - subtrand;
	else
		return (modN - subtrand) + minuend;
}
int modPlus(int modN, int addend1, int addend2)
{
	if (addend1 + addend2 < modN)
		return addend1 + addend2;
	else
		return (addend1 + addend2) - modN;
}

nodeInfoType find_successor(nodeType *curNode, int IDKey)
{
	nodeInfoType predNode;
	chordHeaderType message;
	struct sockaddr_in opponent;
	int opponent_size = sizeof(opponent);
	int iResult;
	//curnode 는 Usr

	if (curNode->nodeInfo.ID == IDKey)
		return curNode->nodeInfo;

	else {
		predNode = find_predecessor(curNode, IDKey);
		if (predNode.ID != Usr_node.nodeInfo.ID)
		{
			opponent = predNode.addrInfo;
			Successor_Info_Request(&message);


			iResult = sendto(fgSock, (char*)&message, sizeof(chordHeaderType), 0,
				(struct sockaddr*) &opponent, opponent_size);
			if (sMode)
				printf("CHORD > Successor Info request Massage has been sent.\n");
			if (iResult == SOCKET_ERROR)
				error_message("Successor Info Request send error!\n");

			iResult = recvfrom(fgSock, (char *)&message, sizeof(chordHeaderType), 0,
				(struct sockaddr*)&opponent, &opponent_size);
			if (sMode)
				printf("CHORD > Successor Info response Massage has been recieved.\n");
			if (iResult == SOCKET_ERROR)
			{
				error_message("Successor info Reponse send error!\n");
				return curNode->nodeInfo;
			}
			return message.nodeInfo;
		}
		return curNode->chordInfo.fingerInfo.finger[0];
	}
}

nodeInfoType find_predecessor(nodeType *curNode, int IDKey)
{
	struct sockaddr_in opponent; //to save opponent's(sender) IP
	int opponent_size = sizeof(opponent);
	int iResult;
	nodeType tempNode = *curNode;
	chordHeaderType message;
	if (tempNode.nodeInfo.ID == tempNode.chordInfo.fingerInfo.finger[0].ID) // special case: the initial node
		return tempNode.nodeInfo;

	if (tempNode.nodeInfo.ID == IDKey)	//해싱 충돌시 검사. 해당 IDKey가 존재 하는지 검사
		return tempNode.nodeInfo;

	while (!modIn(ringSize, IDKey, tempNode.nodeInfo.ID, tempNode.chordInfo.fingerInfo.finger[0].ID, 0, 1))
	{
		tempNode.nodeInfo = find_closest_predecessor(&tempNode, IDKey);

		Successor_Info_Request(&message);
		opponent = tempNode.nodeInfo.addrInfo;
		iResult = sendto(fgSock, (char*)&message, sizeof(chordHeaderType), 0,
			(struct sockaddr*) &opponent, opponent_size);
		iResult = recvfrom(fgSock, (char *)&message, sizeof(chordHeaderType), 0,
			(struct sockaddr*)&opponent, &opponent_size);
		tempNode.chordInfo.fingerInfo.finger[0].ID = message.nodeInfo.ID;
	}


	return tempNode.nodeInfo;
}

nodeInfoType find_closest_predecessor(nodeType *curNode, int IDKey)
{
	int i;
	struct sockaddr_in opponent; //to save opponent's(sender) IP
	int opponent_size = sizeof(opponent);
	chordHeaderType message;
	int iResult;
	for (i = baseM - 1; i >= 0; i--)
	{
		if (curNode->chordInfo.fingerInfo.finger[i].ID == 0) // Actually not necessary
			continue;

		if (modIn(ringSize, curNode->chordInfo.fingerInfo.finger[i].ID, curNode->nodeInfo.ID, IDKey, 0, 0))
		{
			if (curNode->chordInfo.fingerInfo.finger[i].ID != Usr_node.nodeInfo.ID)
			{
				int leaveID = curNode->chordInfo.fingerInfo.finger[i].ID;

				opponent = curNode->chordInfo.fingerInfo.finger[i].addrInfo;
				Find_Predecessor_Request(&message, IDKey);

				iResult = sendto(fgSock, (char*)&message, sizeof(chordHeaderType), 0,
					(struct sockaddr*) &opponent, opponent_size);
				if (sMode)
					printf("CHORD > Find Predecessor request Massage has been sent.\n");
				if (iResult == SOCKET_ERROR)
					error_message("Find Predecessor Request send error!\n");

				iResult = recvfrom(fgSock, (char *)&message, sizeof(chordHeaderType), 0,
					(struct sockaddr*)&opponent, &opponent_size);

				if (sMode)
					printf("CHORD > Find Predecessor response Massage has been recieved.\n");
				if (iResult == SOCKET_ERROR)
				{
					for (int j = i; j > 0; j--)
					{
						if (leaveID == Usr_node.chordInfo.fingerInfo.finger[j].ID)
						{
							if (j != baseM - 1)
								Usr_node.chordInfo.fingerInfo.finger[j] = Usr_node.chordInfo.fingerInfo.finger[j + 1];
							if (j == baseM - 1)
							{
								Usr_node.chordInfo.fingerInfo.finger[j] = Usr_node.chordInfo.fingerInfo.Pre;
							}
						}
					}
					printf("find pred  MsgId : %d 1 IP : %s Port : %d 에게 Key : %d 안옴\n ", message.msgID, inet_ntoa(opponent.sin_addr), ntohs(opponent.sin_port), message.nodeInfo.ID);
					error_message("Thread Find Predecessor Reponse recieve error!\n");
					return curNode->chordInfo.fingerInfo.finger[i];
				}


				return message.nodeInfo;


			}
		}
	}
	/*actually don't used*/
	return curNode->nodeInfo;
}

int fix_finger(void)
{
	int i;


	if (Usr_node.nodeInfo.ID == 0)
		return -1;

	for (i = 0; i < baseM; i++)
	{
		Usr_node.chordInfo.fingerInfo.finger[i]
			= find_successor(&Usr_node, modPlus(ringSize, Usr_node.nodeInfo.ID, twoPow(i)));
	}

	for (i = 0; i < Usr_node.fileInfo.fileNum; i++)
	{
		Usr_node.fileInfo.fileRef[i].refOwner
			= find_successor(&Usr_node, modPlus(ringSize, Usr_node.nodeInfo.ID, Usr_node.fileInfo.fileRef[i].refOwner.ID));
	}



	return 0;
}

void test_join(void)
{
	chordHeaderType message;
	struct sockaddr_in helper;
	int iResult = 0;//to check return value;
	struct sockaddr_in opponent;
	int helper_size = sizeof(helper);
	int opponent_size = sizeof(opponent);
	char ip[15] = { "127.0.0.1" }, port[6] = {"50000" };

	/*printf("CHORD> Helper's IP : ");
	scanf(" %s", &ip);
	rewind(stdin);

	printf("CHORD> Helper's PORT : ");
	scanf(" %s", &port);
	rewind(stdin);*/

	memset(&helper, 0, sizeof(helper));
	helper.sin_family = AF_INET;
	helper.sin_addr.s_addr = inet_addr(ip);
	helper.sin_port = htons(atoi(port));


	/*Initialize*/
	memset(&Usr_node.chordInfo.fingerInfo.Pre, 0, sizeof(Usr_node.chordInfo.fingerInfo.Pre)); //pre 초기화
																							  //보낼 메시지 초기화
	Join_Info_Request(&message);
	iResult = sendto(rqSock, (char*)&message, sizeof(chordHeaderType), 0, (struct sockaddr*) &helper, helper_size);
	if (iResult == SOCKET_ERROR)
		error_message("Join info Request send error!\n");
	if (sMode)
		printf("CHORD > JoinInfo request Massage has been sent.\n");
	//join info response
	iResult = recvfrom(rqSock, (char *)&message, sizeof(chordHeaderType), 0,
		(struct sockaddr*)&helper, &helper_size);//헬퍼한테서 석세서노드
	if (iResult == SOCKET_ERROR)
		error_message("Join info Request recvfrom error!\n");
	if (sMode)
		printf("CHORD > JoinInfo response Massage has been recieved.\n");
	Usr_node.chordInfo.fingerInfo.finger[0] = message.nodeInfo;//받은 데이터로 저장



															   /*To avoid Hashing collision - double hashing*/
	Find_Predecessor_Request(&message, Usr_node.nodeInfo.ID);

	iResult = sendto(rqSock, (char*)&message, sizeof(chordHeaderType), 0, (struct sockaddr*) &helper, helper_size);
	if (iResult == SOCKET_ERROR)
		error_message("Join info Request send error!\n");
	if (sMode)
		printf("CHORD > Find predecessor request Massage has been sent.\n");

	iResult = recvfrom(rqSock, (char *)&message, sizeof(chordHeaderType), 0,
		(struct sockaddr*)&helper, &helper_size);//헬퍼한테서 석세서노드
	if (iResult == SOCKET_ERROR)
		error_message("Join info Request recvfrom error!\n");
	if (sMode)
		printf("CHORD > Find predecessor response Massage has been recieved.\n");

	

	/*Initialize end*/


	/*movekeys*/
	move_keys(&Usr_node, &Usr_node.chordInfo.fingerInfo.finger[0]);
	/*movekeys*/

	/*stabilize*/
	//WaitForSingleObject(hMutex, INFINITE);

	Predecessor_Info_Request(&message);
	iResult = sendto(rqSock, (char*)&message, sizeof(chordHeaderType), 0, (struct sockaddr*) &Usr_node.chordInfo.fingerInfo.finger[0].addrInfo, opponent_size);
	if (iResult == SOCKET_ERROR)
		error_message("Predecessor info Request send error!\n");
	if (sMode)
		printf("CHORD > Predecessor Info request Massage has been sent.\n");
	//printf("Main predecessor info  MsgId : %d 1 IP : %s Port : %d 에게 보냄\n ", message.msgID, inet_ntoa(Usr_node.chordInfo.fingerInfo.finger[0].addrInfo.sin_addr), ntohs(Usr_node.chordInfo.fingerInfo.finger[0].addrInfo.sin_port));
	iResult = recvfrom(rqSock, (char *)&message, sizeof(chordHeaderType), 0,
		(struct sockaddr*)&opponent, &opponent_size);
	if (iResult == SOCKET_ERROR)
		error_message("Predecessor info Request recvfrom error!\n");
	if (sMode)
		printf("CHORD > Predecessor Info response Massage has been recieved.\n");

	//printf("Main predecessor info  MsgId : %d 1 IP : %s Port : %d 에게 보냄\n ", message.msgID, inet_ntoa(opponent.sin_addr), ntohs(opponent.sin_port));
	Usr_node.chordInfo.fingerInfo.Pre = message.nodeInfo;
	opponent = message.nodeInfo.addrInfo;

	Successor_Update_Request(&message);
	iResult = sendto(rqSock, (char*)&message, sizeof(chordHeaderType), 0, (struct sockaddr*) &opponent, opponent_size);
	if (iResult == SOCKET_ERROR)
		error_message("Successor update Request send error!\n");
	if (sMode)
		printf("CHORD > Successor Update request Massage has been sent.\n");

	iResult = recvfrom(rqSock, (char *)&message, sizeof(chordHeaderType), 0,
		(struct sockaddr*)&opponent, &opponent_size);
	if (iResult == SOCKET_ERROR)
		error_message("Successor update Request recvfrom error!\n");
	if (sMode)
		printf("CHORD > Successor Update response Massage has been recieved.\n");


	Predecessor_Update_Request(&message);
	iResult = sendto(rqSock, (char*)&message, sizeof(chordHeaderType), 0, (struct sockaddr*) &Usr_node.chordInfo.fingerInfo.finger[0].addrInfo, opponent_size);
	if (iResult == SOCKET_ERROR)
		error_message("Predecessor update Request send error!\n");
	if (sMode)
		printf("CHORD > Predecessor Update request Massage has been sent.\n");
	iResult = recvfrom(rqSock, (char *)&message, sizeof(chordHeaderType), 0,
		(struct sockaddr*)&opponent, &opponent_size);
	if (iResult == SOCKET_ERROR)
		error_message("Predecessor update Request send error!\n");
	if (sMode)
		printf("CHORD > Predecessor Update response Massage has been recieved.\n");

	/*stabilize*/

	fix_finger();

	if (sMode)
		printf("CHORD > Node Join has been successfully finished. \n");


	return;
}