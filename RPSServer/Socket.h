#pragma once


namespace nwayinterview
{
	class Socket
	{
	public:
		Socket(SOCKET socket) :
			handle_(socket)
		{
		}

		virtual ~Socket();

		void Close();

		SOCKET get_socket() const { return handle_; }

	private:
		SOCKET handle_;
		std::atomic<bool> closed_;
	};

}