#include "TcpServer.h"
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <string>
#include <sstream>
#include "Verify.h"

int g_ServerSocket;


bool SocketBindAndListen(char *lpszIp, int iPort)
{
	g_ServerSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (g_ServerSocket < 0)
		return false;

	struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));

	addr.sin_family = AF_INET;
	addr.sin_port = htons(iPort);
    addr.sin_addr.s_addr = inet_addr(lpszIp);
	if (0 != bind(g_ServerSocket, (sockaddr *)(&addr), sizeof(addr)))
	{
		return false;
	}
	if (0 != listen(g_ServerSocket, 1))
	{
		return false;
	}

	//::CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)RecvThreadProc, NULL, NULL, NULL);

	return true;
}

std::string get_time_str()
{
	time_t t = time(nullptr);
	struct tm* now;
	now = localtime(&t);

	std::stringstream time;
	time << now->tm_year + 1900 << "-";
	time << now->tm_mon + 1 << "-";
	time << now->tm_mday << " ";
	time << now->tm_hour << ":";
	time << now->tm_min << ":";
	time << now->tm_sec;

	return time.str();
}

void AcceptRecvMsg()
{
	struct sockaddr_in addr;
	socklen_t iLen = sizeof(addr);   
	int ClientSocket;
	char szBuf[0x100] = { 0 };

	while (1)
	{
		ClientSocket = accept(g_ServerSocket, (sockaddr*)(&addr), &iLen);
		if (ClientSocket < 0)
			continue;
		printf("[+][%s] Accept a connection from %s\n",get_time_str().c_str(), inet_ntoa(addr.sin_addr));

		int iRet = recv(ClientSocket, szBuf, sizeof(szBuf), 0);
		if (iRet <= 0)
			continue;
		VerifyData((unsigned char*)szBuf, sizeof(szBuf));
		send(ClientSocket, szBuf, sizeof(szBuf), 0);

		close(ClientSocket);
		ClientSocket = -1;
	}
}

int RecvThreadProc(void* lpVoid) 
{
	AcceptRecvMsg();
	return 0;
}
