// RPSServer.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "GameLogic.h"
#include "SelectServer.h"
#include <thread>
#include <fstream>


using namespace nwayinterview;


size_t handle_request(GameLogic &logic, SelectServer &server, SOCKET s, char const * const buffer, size_t len)
{
	if (len == 0)
	{
		logic.player_quit(s);
		return 0;
	}

	Request request;
	Response response;

	char response_buffer[SelectServer::DATA_BUFSIZE];

	char const *package = buffer;

	size_t bytes_remained = len;
	while (bytes_remained > 0)
	{
		request.client_id_ = s;
		request.type_ = PLAYER_RAISED_HAND;

		size_t consumed = request.deserialize_from(buffer, len);
		if (consumed == 0)
			break;

		bytes_remained -= consumed;
		package += consumed;

		logic.fight(request, response);

		size_t used = response.serialize_to(response_buffer, SelectServer::DATA_BUFSIZE);
		server.send(response.client_id_, response_buffer, used);
	}
	return len - bytes_remained;
};

int _tmain(int argc, _TCHAR* argv[])
{
	auto has_params = [&](_TCHAR const *param_str)
	{
		return std::any_of(&argv[1], &argv[argc], [&](_TCHAR *arg) { return std::wstring(arg) == param_str; });
	};

	bool verbose = has_params(_T("-v"));

	LogLevel log_level = verbose ? DEBUG : INFO;
	
	std::ofstream sessions_log_stream("sessions.log", std::ofstream::out);
	std::ofstream server_log_stream("server.log", std::ofstream::out);

	Logger session_log(sessions_log_stream, log_level);
	Logger server_log(server_log_stream, log_level);

	SelectServer server(session_log);
	GameLogic logic(server_log);
	
	using namespace std::placeholders;
	auto enqueue_consumer = std::bind(handle_request, std::ref(logic), _1, _2, _3, _4);

	auto echo_consumer = [](SOCKET s, char const * const buffer, size_t len) -> size_t
	{
		std::cout << std::string(buffer, buffer + len) << std::endl;
		return len;
	};

	std::thread select_server_thread([&]() { server.loop(10010, enqueue_consumer); });
	
	select_server_thread.join();
	
	return 0;
}

