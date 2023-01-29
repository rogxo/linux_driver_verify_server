#include <stdio.h>
#include <stdlib.h>
#include "TcpServer.h"


int main(int argc, char* argv[])
{
	//system("sysctl -w net.ipv4.tcp_syncookie=1");
	//system("sysctl -w net.ipv4.tcp_tw_reuse=1");
	//system("sysctl -w net.ipv4.tcp_tw_recycle=1");
	//system("sysctl -w net.ipv4.tcp_fin_timeout=5");

	if (false == SocketBindAndListen((char*)"0.0.0.0", 31828))
	{
		printf("[-] SocketBindAndListen Error.\n");
	}
	printf("[+] SocketBindAndListen OK.\n");

	AcceptRecvMsg();

	return 0;
}

