#pragma once

/*Implemented Per Platform, Only The Correct Implementation For The Platform Should Be Compiled*/
namespace Violet {
	
	class VIOLET_API FileDialogs {
	public:
		/*Return Empty Strings If Canceled*/
		static std::string OpenFile(const char* filter);
		static std::string SaveFile(const char* filter, const char* defaultFileName = nullptr);
	};

}