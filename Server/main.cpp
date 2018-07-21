#pragma warning(disable:4996)
#include "XTcp.h"
#include "XHttpServer.h"
#include <iostream>
#ifdef WIN32
#include <windows.h>
#else
#include<unistd.h>
#endif
using namespace std;

int main()
{
	cout << "Json Server +++" << endl;
	XHttpServer server;
	server.Start(8080);
	while (1)
	{
	#ifdef WIN32
		Sleep(1000);
	#else
		sleep(1);
	#endif
	}
	return 0;
}
