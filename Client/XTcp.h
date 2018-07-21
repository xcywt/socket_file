#ifndef XTCP_H
#define XTCP_H
#pragma warning(disable:4996)

#include <string>
std::string GetIpByHost(std::string host);
/*
这里是封装的socket类。Recv可以接收指定字节的数据。Send可以发送指定字节的数据。
*/
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
