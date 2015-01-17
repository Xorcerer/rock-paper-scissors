// RPSClient.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <string>
#include <iostream>
#include <WinSock2.h>

#include <Messages.h>
#include <consts.h>

SOCKET connect_to(char* address, unsigned short port)
{
	WSADATA wsadata;

	int error = WSAStartup(0x0202, &wsadata);

	if (error)
		return INVALID_SOCKET;

	if (wsadata.wVersion != 0x0202)
	{
		WSACleanup();
		return INVALID_SOCKET;
	}

	SOCKADDR_IN target;

	target.sin_family = AF_INET;
	target.sin_port = htons(port);
	target.sin_addr.s_addr = inet_addr(address);

	SOCKET s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (s == INVALID_SOCKET)
	{
		return INVALID_SOCKET;
	}

	if (connect(s, (SOCKADDR *)&target, sizeof(target)) == SOCKET_ERROR)
	{
		return INVALID_SOCKET;
	}

	return s;
}

using namespace nwayinterview;

int _tmain(int argc, _TCHAR* argv[])
{
	SOCKET s = ::connect_to("127.0.0.1", 10010);
	if (s == INVALID_SOCKET)
		return -1;

	char buffer[DATA_BUFSIZE];

	int selection;

	while (true)
	{
		std::cout << "Select 1: Rock, 2: Paper, 3: Scissors. Any key else to quit." << std::endl;
		std::cin >> selection;
		if (selection < 1 || selection > 3)
		{
			std::cout << "Bye Bye." << std::endl;
			break;
		}
		Sign sign = (Sign)(byte)(selection - 1);

		buffer[0] = (byte)sign;
		int result = send(s, buffer, 1, 0);
		if (result < 0)
		{
			std::cerr << "Send failed." << std::endl;
			return -1;
		}

		result = recv(s, buffer, DATA_BUFSIZE, 0);
		if (result <= 0)
		{
			std::cerr << "Recv failed." << std::endl;
			return -1;
		}

		Response response;
		result = response.deserialize_from(buffer, result);
		if (result <= 0)
		{
			std::cerr << "Deserializing failed." << std::endl;
			return -1;
		}

		std::cout << "Result: " << to_string(response.result_) << std::endl
			<< "Win: " << response.win_count << std::endl
			<< "Loss: " << response.loss_count << std::endl
			<< "Tie: " << response.tie_count << std::endl
			<< "Rock: " << response.rock_count << std::endl
			<< "Paper: " << response.paper_count << std::endl
			<< "Scissors: " << response.scissors_count << std::endl
			<< "---------------------------------------------------------" << std::endl;
	}
	return 0;
}

