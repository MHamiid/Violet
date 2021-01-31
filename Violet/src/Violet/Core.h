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
	#define VIO_ENABLE_ASSERTS
#endif  //DEBUG_DEBUG_MODE


//Debug Assertion
#ifdef VIO_ENABLE_ASSERTS

#define VIO_ASSERT(x,...) {if(!(x)){ VIO_ERROR("Assertion Failed: {0}" , __VA_ARGS__); __debugbreak();}}
#define VIO_CORE_ASSERT(x,...) {if(!(x)){ VIO_CORE_ERROR("Assertion Failed: {0}" , __VA_ARGS__); __debugbreak();}}


#else
#define VIO_ASSERT(x,...)
#define VIO_CORE_ASSERT(x,...)

#endif //VIO_ENABLE_ASSERTS

//Defines
#define VIO_BIND_EVENT_FUNCTION(functionName) std::bind(&functionName, this, std::placeholders::_1)