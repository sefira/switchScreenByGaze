/*
The MIT License(MIT)

Copyright(c) 2015 Bu Xingyuan

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files(the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and / or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions :

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/

#include "computer.h"
#include "evaluatemedia.h"

#include <iostream>
#include <Winsock2.h>

#pragma comment(lib, "Ws2_32.lib")

using namespace std;

ComputerInfo::ComputerInfo()
{
	memset(hostname,0,sizeof(hostname));
	memset(IP, 0, sizeof(IP));
	evaluate_point = 1000;
	num = 0;
}

int ComputerInfo::ToString()
{
	cout << "num:" << num << endl;
	cout << "hostname:" << hostname << endl;
	cout << "IP:" << IP << endl;
	cout << "evaulate:" << evaluate_point << endl;

	return 0;
}

Computer::Computer()
{
	
}

Computer::~Computer()
{

}

int Computer::QueryLocalHostname()
{
	struct hostent *m_hostent;

	WSADATA wsaData; 

	// Initialize Winsock
	int result;
	result = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (result != 0) {
		printf("WSAStartup failed: %d\n", result);
		return 1;
	}

	result = gethostname(local_hostname, sizeof(local_hostname));
	if (result != 0) {
		printf("gethostname failed: %d\n", result);
		return 1;
	}

	WSACleanup();

	return 0;
}

int Computer::QueryHostIPbyName(char * hostname, ComputerInfo &m_computerinfo)
{

	//-----------------------------------------
	// Declare and initialize variables
	WSADATA wsaData;
	int result;

	DWORD dwError;
	int i = 0;

	struct hostent *remotehost;
	struct in_addr addr;

	char **pAlias;

	// Initialize Winsock
	result = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (result != 0)
	{
		printf("WSAStartup failed: %d\n", result);
		return 1;
	}

	printf("Calling gethostbyname with %s\n", hostname);
	remotehost = gethostbyname(hostname);

	if (remotehost == NULL)
	{
		dwError = WSAGetLastError();
		if (dwError != 0) {
			if (dwError == WSAHOST_NOT_FOUND)
			{
				printf("Host not found\n");
				return 1;
			}
			else if (dwError == WSANO_DATA)
			{
				printf("No data record found\n");
				return 1;
			}
			else
			{
				printf("Function failed with error: %ld\n", dwError);
				return 1;
			}
		}
	}
	else
	{
		strcpy(m_computerinfo.hostname, remotehost->h_name);

		if (remotehost->h_addr_list[0] != 0)
		{
			addr.s_addr = *(u_long *)remotehost->h_addr_list[i++];
			strcpy(m_computerinfo.IP, inet_ntoa(addr));
		}
		else
		{
			return 1;
		}
		return 0;
	}
}

double Computer::QueryDeviation()
{
	double dev = EvaluateMedia::GetDeviation();

	return dev;
}

int Computer::SendHostname()
{
	//TODO socket send , a really send action
	QueryLocalHostname();
	cout << local_hostname << endl;
	
	return 0;
}

int Computer::SendDeviation()
{
	//TODO socket send , a really send action
	double deviation = QueryDeviation();
	cout << deviation << endl;

	return 0;
}