#pragma once

#ifdef VIO_PLATFORM_WINDOWS
	#ifdef VIO_SHARED_LIB
		#ifdef VIO_BUILD_DLL
			#define VIOLET_API __declspec(dllexport)
		#else
			#define VIOLET_API __declspec(dllimport)
		#endif // VIO_BUILD_DLL
	#else
		#define VIOLET_API	
	#endif //VIO_SHARED_LIB
#else
	#error No Platform specified!

#endif //VIO_PLATFORM_WINDOWS


//To be defined in debug mode
#ifdef VIO_DEBUG_MODE
	#ifdef VIO_PLATFORM_WINDOWS
		#define VIO_DEBUG_BREAK() __debugbreak()
	#elif VIO_PLATFORM_LINUX
		#include <signal.h>
		#define VIO_DEBUG_BREAK() raise(SIGTRAP)
	#else
		#error "Platform debugbreak not supported"
	#endif  //Setting VIO_DEBUG_BREAK()
	#define VIO_ENABLE_ASSERTS
#else
	#define VIO_DEBUG_BREAK()
#endif  //DEBUG_DEBUG_MODE


//Debug Assertion
#ifdef VIO_ENABLE_ASSERTS
#define VIO_ASSERT(x,...) {if(!(x)){ VIO_ERROR("Assertion Failed: {0}" , __VA_ARGS__); VIO_DEBUG_BREAK();}}
#define VIO_CORE_ASSERT(x,...) {if(!(x)){ VIO_CORE_ERROR("Assertion Failed: {0}" , __VA_ARGS__); VIO_DEBUG_BREAK();}}


#else
#define VIO_BREAK
#define VIO_CORE_BREAK
#define VIO_ASSERT(x,...)
#define VIO_CORE_ASSERT(x,...)

#endif //VIO_ENABLE_ASSERTS

//Defines
#define VIO_BIND_EVENT_MEMBER_FUNCTION(functionName) [this](auto&&... args) -> bool { return this->functionName(std::forward<decltype(args)>(args)...); }  //Called inside a class to bind a member function

#include <memory> //Make sure memory is included first
namespace Violet {

	template <typename T>
	using Scoped = std::unique_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Scoped<T> CreateScope(Args&& ... args)
	{
		return std::make_unique<T>(std::forward<Args>(args)...);
	}



	template <typename T>
	using Ref = std::shared_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Ref<T> CreateRef(Args&& ... args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}
}