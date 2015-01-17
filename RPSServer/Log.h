#pragma once

#include <string>


namespace nwayinterview
{
	enum LogLevel
	{
		DEBUG,
		INFO,
		WARNING,
		CRITICAL,
	};

	static std::string const to_string(LogLevel level)
	{
		switch (level)
		{
		case nwayinterview::DEBUG:
			return "[Debug]";
		case nwayinterview::INFO:
			return "[Info]";
		case nwayinterview::WARNING:
			return "[Warning]";
		case nwayinterview::CRITICAL:
			return "[Critical]";
		}
	}

	class Logger
	{
		Logger(Logger const &_) = delete;
		Logger &operator=(Logger const &_) = delete;

	public:
		Logger(std::ostream &out, LogLevel level = DEBUG) :
			out_(out),
			log_level_(level)
		{

		}

		template<typename T1>
		void log(LogLevel level, T1 const &msg1) const
		{
			if (level >= log_level_)
			{
				out_ << to_string(level) << ' ' << msg1 << std::endl;
			}
		}

		template<typename T1, typename T2>
		void log(LogLevel level, T1 const &msg1, T2 const &msg2) const
		{
			if (level >= log_level_)
			{
				out_ << to_string(level) << ' ' << msg1 << ' ' << msg2 << std::endl;
			}
		}

	private:
		LogLevel log_level_;
		std::ostream & out_;
	};
}
