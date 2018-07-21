#include "xcyHttpClient.h"
#include <thread>
#include <fstream>
#include <iostream>
#include <string.h>
#ifdef WIN32
#include<windows.h>
#else
#include<unistd.h>
#endif

xcyHttpClient::xcyHttpClient()
{
}


xcyHttpClient::~xcyHttpClient()
{
}

bool xcyHttpClient::Start(XTcp client)
{
	this->client = client;
	thread sth(&xcyHttpClient::Main, this);
	sth.detach();
	return true;
}
#define SEND_BLOCK_LEN 1024
#define SAVE_FILE_NAME_LEN 256

void xcyHttpClient::Main()
{
	char buf[SEND_BLOCK_LEN] = { 0 };
	while (1)
	{
		char bufHead[6] = { 0 };
		int len = client.Recv(bufHead, sizeof(bufHead) - 1);
		if (len <= 0)
		{
			cout << "client close1 ..." << endl;
			break;
		}
		bufHead[len] = '\0';
		string strHead(bufHead);
		if (strHead != "Begin")
		{
			cout << "recv head error ..." << endl;
			break;
		}
		else
		{
			cout << "recv head success ..." << endl;
		}
		
		int nNameLen = 0;
		int ret = client.Recv((char*)&nNameLen, 4);
		if (ret <= 0)
		{
			cout << "recv nameLen error ..." << endl;
		}

		char strFileName[SAVE_FILE_NAME_LEN] = { 0 };
		ret = client.Recv(strFileName, nNameLen);
		if (ret <= 0)
		{
			cout << "recv name error ..." << endl;
		}

		strFileName[nNameLen] = 0;
		cout << "Recv file name = " << strFileName << endl;

		FILE *m_fpOut = fopen(strFileName, "wb+");
		if (!m_fpOut)
		{
			cout << "fopen error strFileName = " << strFileName << endl;
		}

		unsigned long long file_size = 0;
		ret = client.Recv((char*)&file_size, sizeof(unsigned long long) + 1);
		if (ret <= 0)
		{
			cout << "recv dataLen error ..." << endl;
		}
		cout << "Start recv data. file size = " << file_size << endl;

		unsigned long long nRemainLen = file_size;
		int bufLen = sizeof(buf);
		int nTemp = 0;
		do
		{
			memset(buf, 0, bufLen);
			if (nRemainLen <= bufLen)
			{
				ret = client.Recv(buf, nRemainLen);
				if (ret <= 0)
				{
					cout << "recv data error1 ..." << endl;
				}

				nTemp = fwrite(buf, 1, nRemainLen, m_fpOut);
				if (nTemp != nRemainLen)
				{
					cout << "write file error nTemp = " << nTemp << endl;
				}
				cout << "write file end ... nRemainLen = " << nRemainLen << endl;

				ret = client.Send("Recv_End", 8);
				if (ret <= 0)
				{
					cout << "Send Recv_End error2 ..." << endl;
				}
				break;
			}
			else
			{
				ret = client.Recv(buf, bufLen);
				if (ret <= 0)
				{
					cout << "recv data error2 ..." << endl;
				}

				nTemp = fwrite(buf, 1, bufLen, m_fpOut);
				if (nTemp != bufLen)
				{
					cout << "write file error2 nTemp = " << nTemp << endl;
				}

				ret = client.Send("Continue", 8);
				if (ret <= 0)
				{
					cout << "Send Continue error2 ..." << endl;
				}
				
				nRemainLen -= bufLen;
			}	
		} while (1);
		fclose(m_fpOut);
		file_size = 0;
		char bufEnd[4] = { 0 };
		len = client.Recv(bufEnd, sizeof(bufEnd) - 1);
		if (len <= 0)
		{
			cout << "client close2 ..." << endl;
			break;
		}
		bufEnd[3] = '\0';
		string strEnd(bufEnd);
		if (strEnd != "End")
		{
			cout << "recv end error. strEnd = [" << strEnd << "]" << 
				" strEnd[0] = 0x" << hex << (int)strEnd[0] <<
				" strEnd[1] = 0x" << hex << (int)strEnd[1] << 
				" strEnd[2] = 0x" << hex << (int)strEnd[2] << endl;
			break;
		}
		else
		{
			cout << "recv end success ..." << endl;
			break;
		}
	}
	cout << "client close FFF ..." << endl;
//#ifdef WIN32
//	Sleep(50);
//#else
//	usleep(50);
//#endif

	client.Close();

//#ifdef WIN32
//	Sleep(100);
//#else
//	usleep(100);
//#endif

	delete this;
}

