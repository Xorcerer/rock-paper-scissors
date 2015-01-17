#include "stdafx.h"
#include "Socket.h"

namespace nwayinterview
{

	Socket::~Socket()
	{
		Close();
	}

	void Socket::Close()
	{
		if (closed_)
			return;

		::closesocket(handle_);
	}

}