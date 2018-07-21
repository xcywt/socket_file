#ifndef XTCP_H
#define XTCP_H
#pragma warning(disable:4996)

#include <string>
std::string GetIpByHost(std::string host);

class  XTcp
{
public:
	int CreateSocket();
	bool Bind(unsigned short port);
	XTcp Accept();
	void Close();
	int Recv(char *buf,int bufsize);
	int Send(const char *buf,int sendsize);
	bool Connect(const char *ip,unsigned short port,int timeoutms=1000);
	bool SetBlock(bool isblock);
	XTcp();
	virtual ~XTcp();
	int sock = 0;
	unsigned short port = 0;
	char ip[16];

};

#endif