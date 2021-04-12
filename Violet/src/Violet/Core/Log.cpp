#include "VIOPCH.h"
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>

namespace Violet {
	//Defining static members.
	Ref<spdlog::logger> Log::s_coreLogger;
	Ref<spdlog::logger> Log::s_clientLogger;
	void Log::init()
	{	
		std::vector<spdlog::sink_ptr> logSinks;
		logSinks.emplace_back(CreateRef<spdlog::sinks::stdout_color_sink_mt>());
		logSinks.emplace_back(CreateRef<spdlog::sinks::basic_file_sink_mt>("Violet.log", true));

		logSinks[0]->set_pattern("%^[%T] %n: %v%$");
		logSinks[1]->set_pattern("[%D] [%T] [%l] %n: %v");

		s_coreLogger = CreateRef<spdlog::logger>("VIOLET", begin(logSinks), end(logSinks));
		spdlog::register_logger(s_coreLogger);
		s_coreLogger->set_level(spdlog::level::trace);
		s_coreLogger->flush_on(spdlog::level::trace);

		s_clientLogger = CreateRef<spdlog::logger>("APP", begin(logSinks), end(logSinks));
		spdlog::register_logger(s_clientLogger);
		s_clientLogger->set_level(spdlog::level::trace);
		s_clientLogger->flush_on(spdlog::level::trace);
	}

}