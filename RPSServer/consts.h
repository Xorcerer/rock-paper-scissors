#pragma once

namespace nwayinterview
{
	enum Sign : byte {
		ROCK = 0,
		PAPER = 1,
		SCISSORS = 2,
	};

	enum GameResult : byte {
		TIE = 0,
		WIN = 1,
		LOSS = 2,
	};

	const size_t DATA_BUFSIZE = 1024;
}
