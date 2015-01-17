#pragma once

namespace nwayinterview
{
	class SelectServer
	{
		NONCOPYABLE(SelectServer)

	public:
		static const size_t DATA_BUFSIZE = ::nwayinterview::DATA_BUFSIZE;

		// a function consume given buffer, return the length of bytes consumed.
		// if the last parameter `len` is 0, the given socket is closed.
		typedef std::function<size_t(SelectServer&, SOCKET, char const *, size_t len)> ConsumerFunction;

		SelectServer(Logger &logger):
			logger_(logger)
		{}

		// port: port listening to.
		// buffer_consumer: a function consume given buffer, return the length of bytes consumed.
		void loop(unsigned short port, ConsumerFunction buffer_consumer);
		
		void send(SOCKET target, char const * const buffer, size_t size);

	private:

		struct SocketInfo {
			CHAR buffer_received_[DATA_BUFSIZE];
			CHAR buffer_to_send_[DATA_BUFSIZE];
			WSABUF wsa_buf_;
			SOCKET handle_;
			OVERLAPPED overlapped_;
			DWORD bytes_to_send_;
			DWORD bytes_recieved_;

			size_t get_buffer_received_remained() const { return DATA_BUFSIZE - bytes_recieved_; }
			size_t get_buffer_to_send_remained() const { return DATA_BUFSIZE - bytes_to_send_; }
		};

		void log_and_throw_wsa_error(char const * const msg) const;

		void create_socket_info(SOCKET s);
		void free_socket_info(std::shared_ptr<SocketInfo> &socket);

		Logger &logger_;

		std::unordered_map<SOCKET, std::shared_ptr<SocketInfo>> sockets_;
	};
}
