#include "stdafx.h"
#include "SelectServer.h"

namespace nwayinterview
{
	void SelectServer::send(SOCKET target, char const * const buffer, size_t size)
	{
		auto iter = sockets_.find(target);
		if (iter == sockets_.end())
		{
			logger_.log(WARNING, "Sending a package to an non-exists player.", target);
			return;
		}

		auto &socket = iter->second;
		if (size > socket->get_buffer_to_send_remained())
		{
			logger_.log(WARNING, "No room for outgoing package, discarded. socket:", target);
			return;
		}
		std::move(buffer, buffer + size, socket->buffer_to_send_ + socket->bytes_to_send_);
		socket->bytes_to_send_ += size;
	}

	void SelectServer::log_and_throw_wsa_error(char const * const msg) const
	{
		auto error = WSAGetLastError();

		logger_.log(WARNING, msg, error);
		throw std::exception(msg, error);
	}

	// Main loop for select;
	// TODO: Seperate the code into short functions.
	void SelectServer::loop(unsigned short port, ConsumerFunction buffer_consumer)
	{
		SOCKET listening_socket;
		WSADATA wsa_data;
		
		if (WSAStartup(0x0202, &wsa_data) != 0)
		{
			WSACleanup();
			log_and_throw_wsa_error("WSAStartup()");
		}

		if ((listening_socket = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED)) == INVALID_SOCKET)
		{
			log_and_throw_wsa_error("WSASocket()");
		}

		SOCKADDR_IN address_to_listen;
		address_to_listen.sin_family = AF_INET;
		address_to_listen.sin_addr.s_addr = htonl(INADDR_ANY);
		address_to_listen.sin_port = htons(port);

		if (bind(listening_socket, (PSOCKADDR)&address_to_listen, sizeof(address_to_listen)) == SOCKET_ERROR)
		{
			log_and_throw_wsa_error("bind()");
		}
		logger_.log(INFO, "Listening on *:", port);

		int const backlog = 10;
		if (listen(listening_socket, backlog))
		{
			log_and_throw_wsa_error("listen()");
		}

		unsigned long nonblocking = 1;
		if (ioctlsocket(listening_socket, FIONBIO, &nonblocking) == SOCKET_ERROR)
		{
			log_and_throw_wsa_error("ioctlsocket()");
		}

		FD_SET write_set;
		FD_SET read_set;

		while (true)
		{
			DWORD select_result;
			SOCKET accepted_socket;

			FD_ZERO(&read_set);
			FD_ZERO(&write_set);
			FD_SET(listening_socket, &read_set);
			for (auto pair : sockets_)
			{
				auto &socket = pair.second;
				if (socket->bytes_to_send_ > 0)
				{ 
					FD_SET(socket->handle_, &write_set);
				}
				
				FD_SET(socket->handle_, &read_set);
			}

			// TODO: handle error set.
			if ((select_result = select(0, &read_set, &write_set, NULL, NULL)) == SOCKET_ERROR)
			{
				log_and_throw_wsa_error("select()");
			}

			int events_count = select_result;

			if (FD_ISSET(listening_socket, &read_set))
			{
				events_count--;

				if ((accepted_socket = accept(listening_socket, NULL, NULL)) != INVALID_SOCKET)
				{
					unsigned long nonblocking = 1;
					if (ioctlsocket(accepted_socket, FIONBIO, &nonblocking) == SOCKET_ERROR)
					{
						log_and_throw_wsa_error("ioctlsocket() on accepted_socket.");
					}

					create_socket_info(accepted_socket);
				}
				else
				{
					if (WSAGetLastError() != WSAEWOULDBLOCK)
					{
						log_and_throw_wsa_error("accept() failed with error.");
					}
				}
			}

			if (events_count == 0)
				continue;
			
			std::vector<std::shared_ptr<SocketInfo>> sockets_to_remove;
			for (auto pair: sockets_)
			{
				auto &socket = pair.second;
				
				DWORD flags = 0;
				if (FD_ISSET(socket->handle_, &read_set))
				{
					events_count--;
					socket->wsa_buf_.buf = socket->buffer_received_ + socket->bytes_recieved_;
					socket->wsa_buf_.len = socket->get_buffer_received_remained();
					
					DWORD bytes_received;
					if (WSARecv(socket->handle_, &(socket->wsa_buf_) + socket->bytes_recieved_, 1,
						&bytes_received, &flags, NULL, NULL) == SOCKET_ERROR)
					{
						if (WSAGetLastError() != WSAEWOULDBLOCK)
						{
							logger_.log(WARNING, "WSARecv() failed, client disconnected:", socket->handle_);
							sockets_to_remove.push_back(socket);
						}

						continue;
					}
					else
					{
						socket->bytes_recieved_ += bytes_received;
						
						size_t bytes_consumed = buffer_consumer(*this, socket->handle_, socket->buffer_received_, socket->bytes_recieved_);
						socket->bytes_recieved_ -= bytes_consumed;

						if (socket->bytes_recieved_ != 0)
						{
							char const * remaining_data_start = socket->buffer_received_ + bytes_consumed;

							std::move(remaining_data_start, remaining_data_start + socket->bytes_recieved_,
								socket->buffer_received_);
						}

						// Connection closed;
						if (bytes_received == 0)
						{
							sockets_to_remove.push_back(socket);
							continue;
						}
					}
				}
				
				if (FD_ISSET(socket->handle_, &write_set))
				{
					events_count--;

					socket->wsa_buf_.buf = socket->buffer_to_send_;
					socket->wsa_buf_.len = socket->bytes_to_send_;

					DWORD bytes_sent;
					if (WSASend(socket->handle_, &(socket->wsa_buf_), 1,
						&bytes_sent, 0, NULL, NULL) == SOCKET_ERROR)
					{
						if (WSAGetLastError() != WSAEWOULDBLOCK)
						{
							logger_.log(WARNING, "WSASend() failed, client disconnected:", socket->handle_);
							sockets_to_remove.push_back(socket);
						}

						continue;
					}
					else
					{
						socket->bytes_to_send_ -= bytes_sent;
						// It is a litte complex to write a ring buffer in a short time,
						// So moving the remaining data ahead is much simpler.
						if (socket->bytes_to_send_ != 0)
						{
							char const * remaining_data_start = socket->buffer_to_send_ + bytes_sent;

							std::move(remaining_data_start, remaining_data_start + socket->bytes_to_send_,
								socket->buffer_to_send_);
						}
					}
				}
			} // for (i = 0; events_count > 0 && i < TotalSockets; i++)

			for (auto s : sockets_to_remove)
				free_socket_info(s);
		} // while loop
	}



	void SelectServer::create_socket_info(SOCKET s)
	{

		std::shared_ptr<SocketInfo> socket = std::make_shared<SocketInfo>();
		socket->handle_ = s;
		sockets_.insert(std::make_pair(s, socket));

		logger_.log(DEBUG, "Accepted socket:", s);
	}

	void SelectServer::free_socket_info(std::shared_ptr<SocketInfo> &socket)
	{
		closesocket(socket->handle_);
		sockets_.erase(socket->handle_);

		logger_.log(DEBUG, "Closed socket:", socket->handle_);
	}
}