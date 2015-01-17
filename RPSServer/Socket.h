#pragma once

#include <WinSock2.h>


namespace nwayinterview
{
	class Socket
	{
	public:
		Socket(SOCKET socket) :
			ref_count_(1),
			socket_fd_(socket)
		{
		}

		virtual ~Socket();

		void Close();

		void AddRef();
		void Release();

		SOCKET get_socket() const { return socket_fd_; }

	private:
		unsigned int ref_count_;
		SOCKET socket_fd_;
	};

}