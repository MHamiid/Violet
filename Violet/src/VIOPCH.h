#pragma once
//Pre-Compiled Header

#include <iostream>
#include <memory>
#include <utility>
#include <algorithm>
#include <functional>


//Data Structures
#include <string>
#include <sstream>
#include <array>
#include <vector>
#include <unordered_map>
#include <unordered_set>


//Violet Defined includes
#include "Violet/Core/Core.h"
#include "Violet/Core/Log.h"


//Headers for Windows
#ifdef VIO_PLATFORM_WINDOWS   //Defined in premake as a global compiler macro
	#include <Windows.h>
#endif // VIO_PLATFORM_WINDOWS 

