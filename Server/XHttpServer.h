#ifndef _XHTTPSERVER_H__
#define _XHTTPSERVER_H__
#include "XTcp.h"
class XHttpServer
{
public:
	bool Start(unsigned short port);
	XHttpServer();
	~XHttpServer();
	XTcp server;
	void Main();
	void Stop();
	bool isexit = false;
};
#endif // _XHTTPSERVER_H__
