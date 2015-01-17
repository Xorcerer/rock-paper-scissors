#pragma once

#include "SessionStatus.h"


namespace nwayinterview
{
	enum RequestType : byte {
		PLAYER_QUIT,
		PLAYER_RAISED_HAND,
	};

	struct Request
	{
		RequestType type_;

		// The following fields may be useless depends on `type`.
		Sign sign_;
		SOCKET client_id_;

		size_t serialize_to(char *buffer, size_t len)
		{
			const size_t total_size = sizeof(Sign);
			if (len < total_size)
				return 0;

			*(byte *)buffer = (byte)sign_;
			
			return total_size;
		}

		size_t deserialize_from(char const *buffer, size_t len)
		{
			if (len < sizeof(char))
				return 0;

			sign_ = (Sign)*buffer;
			return sizeof(char);
		}
	};
	
	static_assert(std::is_pod<Request>::value, "Request should be POD");

	struct Response
	{
		SOCKET client_id_;
		GameResult result_;

		unsigned int win_count;
		unsigned int loss_count;
		unsigned int tie_count;

		unsigned int rock_count;
		unsigned int paper_count;
		unsigned int scissors_count;

		size_t deserialize_from(char *buffer, size_t len)
		{
			const size_t total_size = (sizeof(unsigned int) * 3 + sizeof(GameResult));
			if (len < total_size)
				return 0;

			result_ = *(GameResult *)buffer;
			buffer += sizeof(GameResult);
			win_count = *(unsigned int *)buffer;
			buffer += sizeof(unsigned int);
			loss_count = *(unsigned int *)buffer;
			buffer += sizeof(unsigned int);
			tie_count = *(unsigned int *)buffer;
			buffer += sizeof(unsigned int);
			rock_count = *(unsigned int *)buffer;
			buffer += sizeof(unsigned int);
			paper_count = *(unsigned int *)buffer;
			buffer += sizeof(unsigned int);
			scissors_count = *(unsigned int *)buffer;
			buffer += sizeof(unsigned int);

			return total_size;
		}

		size_t serialize_to(char *buffer, size_t len)
		{
			const size_t total_size = (sizeof(unsigned int) * 6 + sizeof(GameResult));
			if (len < total_size)
				return 0;

			*(byte *)buffer = (byte)result_;
			buffer += sizeof(byte);
			*(unsigned int *)buffer = win_count;
			buffer += sizeof(unsigned int);
			*(unsigned int *)buffer = loss_count;
			buffer += sizeof(unsigned int);
			*(unsigned int *)buffer = tie_count;
			buffer += sizeof(unsigned int);
			*(unsigned int *)buffer = rock_count;
			buffer += sizeof(unsigned int);
			*(unsigned int *)buffer = paper_count;
			buffer += sizeof(unsigned int);
			*(unsigned int *)buffer = scissors_count;
			buffer += sizeof(unsigned int);

			return total_size;
		}
	};

	static_assert(std::is_pod<Response>::value, "Response should be POD");
}
