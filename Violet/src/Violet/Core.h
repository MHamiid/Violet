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