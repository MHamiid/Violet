#pragma once
//ignores all warnings raised inside External headers
#pragma warning(push, 0)
#include <spdlog/spdlog.h>
#pragma warning(pop)

//Violet Core Logging
#define VIO_CORE_DEBUG(...)	   Violet::Log::GetCoreLogger()->debug(__VA_ARGS__)
#define VIO_CORE_TRACE(...)	   Violet::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define VIO_CORE_INFO(...)     Violet::Log::GetCoreLogger()->info(__VA_ARGS__)
#define VIO_CORE_WARN(...)     Violet::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define VIO_CORE_ERROR(...)    Violet::Log::GetCoreLogger()->error(__VA_ARGS__)
#define VIO_CORE_CRITICAL(...) Violet::Log::GetCoreLogger()->critical(__VA_ARGS__)

//Client Application Logging
#define VIO_DEBUG(...)		   Violet::Log::GetClientLogger()->debug(__VA_ARGS__)
#define VIO_TRACE(...)	       Violet::Log::GetClientLogger()->trace(__VA_ARGS__)
#define VIO_INFO(...)          Violet::Log::GetClientLogger()->info(__VA_ARGS__)
#define VIO_WARN(...)          Violet::Log::GetClientLogger()->warn(__VA_ARGS__)
#define VIO_ERROR(...)         Violet::Log::GetClientLogger()->error(__VA_ARGS__)
#define VIO_CRITICAL(...)      Violet::Log::GetClientLogger()->critical(__VA_ARGS__)

namespace Violet {

	class VIOLET_API Log
	{
	public:
		static void init();
		static Ref<spdlog::logger>& GetCoreLogger() { return s_coreLogger; }
		static Ref<spdlog::logger>& GetClientLogger() { return s_clientLogger; }
	private:
		static Ref<spdlog::logger> s_coreLogger;
		static Ref<spdlog::logger> s_clientLogger;

	};


}

