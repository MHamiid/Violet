#pragma once

#ifdef VIO_PLATFORM_WINDOWS
	#ifdef VIO_BUILD_DLL
		#define VIOLET_API __declspec(dllexport)
	#else
		#define VIOLET_API __declspec(dllimport)
	#endif // VIO_BUILD_DLL

#else
	#error No Platform specified!

#endif


//Debug Assertion
#ifdef VIO_ENABLE_ASSERTS

#define VIO_ASSERT(x,...) {if(!(x)){ VIO_ERROR("Assertion Failed: {0}" , __VA_ARGS__); __debugbreak();}}
#define VIO_CORE_ASSERT(x,...) {if(!(x)){ VIO_CORE_ERROR("Assertion Failed: {0}" , __VA_ARGS__); __debugbreak();}}


#else
#define VIO_ASSERT(x,...)
#define VIO_CORE_ASSERT(x,...)

#endif //VIO_ENABLE_ASSERTS