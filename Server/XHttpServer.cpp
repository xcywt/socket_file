#include "XHttpServer.h"
#include "xcyHttpClient.h"
#include <thread>
using namespace std;
int g_clientCount = 0;
void XHttpServer::Stop()
{
	isexit = true;
}
void XHttpServer::Main()
{
	while (!isexit)
	{
		XTcp client = server.Accept();
		if (client.sock <= 0)continue;
		printf("new client connect, count:%d\n", ++g_clientCount);
		xcyHttpClient *th = new xcyHttpClient();
		th->Start(client);
	}
}
bool XHttpServer::Start(unsigned short port)
{
	isexit = false;
	server.CreateSocket();
	if (!server.Bind(port)) return false;
	thread sth(&XHttpServer::Main, this);
	sth.detach();
	return true;
}
XHttpServer::XHttpServer()
{
}


XHttpServer::~XHttpServer()
{
}
