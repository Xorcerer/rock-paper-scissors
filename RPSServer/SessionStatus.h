#pragma once

#include <array>
#include "consts.h"


namespace nwayinterview
{
	static std::string const to_string(GameResult result)
	{
		switch (result)
		{
		case nwayinterview::TIE:
			return "Tie";
		case nwayinterview::WIN:
			return "Win";
		case nwayinterview::LOSS:
			return "Loss";
		}
	}

	static std::string const to_string(Sign sign)
	{
		switch (sign)
		{
		case nwayinterview::ROCK:
			return "Rock";
		case nwayinterview::PAPER:
			return "Paper";
		case nwayinterview::SCISSORS:
			return "Scissors";
		}
	}

	class SessionStatus
	{
		std::array<unsigned int, 3> result_status_;
		std::array<unsigned int, 3> sign_status_;

	public:
		SessionStatus() :
			result_status_({ { 0, 0, 0 } }),
			sign_status_({ {0, 0, 0} })
		{
			
		}

		void add_sign(Sign sign)
		{
			sign_status_[(size_t)sign]++;
		}

		size_t get_count(Sign sign) const
		{
			return sign_status_[(size_t)sign];
		}

		void add_result(GameResult result)
		{
			result_status_[(size_t)result]++;
		}

		size_t get_count(GameResult result) const
		{
			return result_status_[(size_t)result];
		}
	};
}