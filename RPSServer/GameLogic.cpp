#include "stdafx.h"
#include "GameLogic.h"

namespace nwayinterview
{
	std::unordered_map<std::pair<Sign, Sign>, GameResult> const rules
	{
		{ { ROCK, ROCK }, TIE },
		{ { ROCK, PAPER }, LOSS },
		{ { ROCK, SCISSORS }, WIN },
		{ { PAPER, ROCK }, WIN },
		{ { PAPER, PAPER }, TIE },
		{ { PAPER, SCISSORS }, LOSS },
		{ { SCISSORS, ROCK }, LOSS },
		{ { SCISSORS, PAPER }, WIN },
		{ { SCISSORS, SCISSORS }, TIE },
	};

	void GameLogic::player_quit(SOCKET client_id)
	{
		sessions.erase(client_id);

		logger_.log(DEBUG, "Client disconnected:", client_id);
	}

	void GameLogic::fight(Request const &request, Response &response)
	{
		Sign npc_sign = (Sign)distribution(generator);
		logger_.log(INFO, "Client raised:", to_string(request.sign_));
		logger_.log(INFO, "NPC raised:", to_string(npc_sign));
		
		response.client_id_ = request.client_id_;
		auto &result_itor = rules.find(std::make_pair(request.sign_, npc_sign));
		response.result_ = result_itor->second;
		logger_.log(INFO, "Client ", to_string(response.result_));

		auto &status = sessions[request.client_id_];
		status.add_result(response.result_);
		status.add_sign(request.sign_);

		response.win_count = status.get_count(WIN);
		response.loss_count = status.get_count(LOSS);
		response.tie_count = status.get_count(TIE);
		response.rock_count = status.get_count(ROCK);
		response.paper_count = status.get_count(PAPER);
		response.scissors_count = status.get_count(SCISSORS);
	}
}