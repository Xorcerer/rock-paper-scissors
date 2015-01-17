#include "Messages.h"
#include "SessionStatus.h"


namespace nwayinterview
{
	class GameLogic
	{
		GameLogic(GameLogic const &_) = delete;
		GameLogic &operator=(GameLogic const &_) = delete;

	public:
		GameLogic(Logger const &logger) :
			logger_(logger),
			generator(),
			distribution(0, 2)
		{}

		void player_quit(SOCKET client_id);
		void fight(Request const &request, Response &response);

	private:
		std::default_random_engine generator;
		std::uniform_int_distribution<int> distribution;

		// player id: session statistic.
		std::unordered_map <unsigned long, SessionStatus> sessions;

		Logger const & logger_;
	};
}

namespace std {
	template <> struct hash<std::pair<nwayinterview::Sign, nwayinterview::Sign>>
	{
		size_t operator()(std::pair<nwayinterview::Sign, nwayinterview::Sign> const & x) const
		{
			return x.first * 10 + x.second;
		}
	};
}