#ifndef __XCYHTTPCLIENT_H__
#define __XCYHTTPCLIENT_H__

#include "XTcp.h"
#include<string>
using namespace std;

class xcyHttpClient
{
public:
	xcyHttpClient();
	~xcyHttpClient();

	bool Start(XTcp client);
	void Main();
private:
	XTcp client;
};

#endif // __XCYHTTPCLIENT_H__

