#pragma warning(disable:4996)
#include "XTcp.h"
#include<iostream>
#include<fstream>
#ifdef WIN32
#include<windows.h>
#else
#include<string.h>
#include<unistd.h>
#endif
using namespace std;
#define SEND_BLOCK_LEN 1024

int fopenTest()
{
	cout << "main() +++" << endl;
	XTcp cClient;
	cClient.CreateSocket();
	while (1)
	{
		//if (cClient.Connect("127.0.0.1", 8080))
		//if (cClient.Connect("192.168.1.11", 8080))
		//if (cClient.Connect("192.168.1.34", 8080))
		if (cClient.Connect("192.168.1.9", 8080))
		{
			break;
		}
	}

	bool error = false;
	// Primer��������İ�.pdf
	// ���SDK.rar
	// �й���ͼ.jpg
	// demo2.txt
	// qt-opensource-windows-x86-msvc2013_64-5.8.0.exe
	// C++_Primer_Five.pdf
	// zhongguoditu.jpg
	// Win7_x64c_8.4.gho
	string m_strfilename  = "2.txt";
	const char *strFileName = "2.txt";

	FILE *m_fp = fopen(m_strfilename.c_str(), "rb");
	if (m_fp == NULL)
	{
		printf("open file %s failed!\n", m_strfilename.c_str());
		return 0;
	}
	//��ȡ�ļ���С
	fseek(m_fp, 0, SEEK_END);
	unsigned long long filesize = ftell(m_fp);
	if (0 == filesize)
	{
		printf("File %s get size failed!\n", m_strfilename.c_str());
		fclose(m_fp);
		cClient.Close();
		getchar();
		return 0;
	}
	if (filesize > 80 * 1024 * 1024)
	{
		cout << "file to lager +++" << endl;
	}
	fseek(m_fp, 0, 0);
	
	int ret = cClient.Send("Begin", 5);
	if (ret <= 0)
	{
		error = true;
		cout << "Send head error +++" << endl;
	}

	// �������ֳ���
	int nFileNameLen = strlen(strFileName);
	ret = cClient.Send((char*)&nFileNameLen, 4);
	if (ret <= 0)
	{
		error = true;
		cout << "Send name len error +++" << endl;
	}
	// ��������
	ret = cClient.Send(strFileName, nFileNameLen);
	if (ret <= 0)
	{
		error = true;
		cout << "Send name error +++" << endl;
	}

	// �����ļ��ܳ���
	cout << "send file_size = " << filesize << endl;
	ret = cClient.Send((char*)&filesize, sizeof(unsigned long long) + 1);
	if (ret <= 0)
	{
		error = true;
		cout << "Send file data len error +++" << endl;
	}

	//  ѭ�������ļ�
	char buf[SEND_BLOCK_LEN] = { 0 };
	int size = sizeof(buf);

	int nSendSize = 0;
	int sendcount = 0;
	for (;;)
	{
		int len = fread(buf, 1, size, m_fp); // �Ƚ��ļ����ݶ�ȡ��buf��
		if (len < 0)
		{
			error = true;
			break;
		}
		if (len == 0)
		{
			cout << "Get file end +++" << endl;
			break;
		}

		if (cClient.Send(buf, len) <= 0)
		{
			cout << "Send file error1 +++" << endl;
			error = true;
			break;
		}

		char continueBuf[9] = { 0 };
		if (cClient.Recv(continueBuf, sizeof(continueBuf) - 1) <= 0)
		{
#ifdef WIN32
			cout << "Recv continue error1 +++ LastError = " << ::GetLastError() << endl;
#else
			cout << "Recv continue error1 +++  " << endl;
#endif
			error = true;
			break;
		}
		continueBuf[8] = 0;
		string strTemp1 = "Continue";
		string strTemp2 = "Recv_End";
		if (continueBuf == strTemp1)
		{
			// 1024 = 1024 * 1024 / SEND_BLOCK_LEN
			if (sendcount % 1024 == 0) // 1024 * 1024 �պ���1��
			{
				cout << "Has been sent " << ++nSendSize << " Mb ......" << endl;
			}
			sendcount++;
		}

		if (continueBuf == strTemp2)
		{
			cout << "Server recv data end ..." << endl;
		}
	}

	fclose(m_fp);

	ret = cClient.Send("End", 3);
	if (ret <= 0)
	{
		cout << "Send end error +++" << endl;
	}

	{
		//char Buf[9] = { 0 };
		//if (cClient.Recv(Buf, sizeof(Buf) - 1) <= 0)
		//{
		//	cout << "Recv continue error1 +++" << endl;
		//}

		//string strTemp2 = "ServerOk";
		//if (Buf == strTemp2)
		//{
		//	cout << "It is good. Server recv file success " << endl;
		//}
		//else
		//{
		//	cout << "Oh no. Server recv file failed. Buf = [" << Buf << "]" << endl;
		//}
	}
#ifdef WIN32
	//Sleep(100);
#else
	//usleep(20);
#endif
	cClient.Close();
	if (error)
	{
		cout << "Send file Failed +++" << endl;
	}
	else
	{
		cout << "Send file success +++" << endl;
	}
	
	getchar();
	return 0;
}

int main()
{
	fopenTest();
	return 0;
}
