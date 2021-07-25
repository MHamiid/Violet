/*
 * PlatformUtils Header Windows Specific Implementation, This CPP File Should Be Only Compiled On Windows Platform
 */
#include "VIOPCH.h"
#include "Violet/Utils/PlatformUtils.h"
#include <commdlg.h>
#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32  //To get the windows handle for our window
#include <GLFW/glfw3native.h>
#include "Violet/Core/Application.h"

namespace Violet {

	std::string FileDialogs::OpenFile(const char* filter) 
	{
		OPENFILENAMEA openFileName;
		CHAR filePath[256] = { 0 };
		CHAR currentDirectory[256] = { 0 };
		ZeroMemory(&openFileName, sizeof(OPENFILENAME)); //Set the OPENFILENAME struct to 0
		openFileName.lStructSize = sizeof(OPENFILENAME);
		openFileName.hwndOwner = glfwGetWin32Window((GLFWwindow*)Application::GetApplicationInstance().getWindow().getNativeWindow());   //Get the windows window handle
		openFileName.lpstrFile = filePath;
		openFileName.nMaxFile = sizeof(filePath);
		openFileName.lpstrFilter = filter;
		openFileName.nFilterIndex = 1;
		openFileName.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

		if (GetCurrentDirectoryA(sizeof(currentDirectory), currentDirectory))  //Get the current working directory and set it in currentDirectory buffer
		{
			openFileName.lpstrInitialDir = currentDirectory; 
		}

		if (GetOpenFileNameA(&openFileName) == TRUE)
		{
			return openFileName.lpstrFile; //Return the file path
		}
		return std::string();  //Return empty string
	}
	std::string FileDialogs::SaveFile(const char* filter, const char* defaultFileName)
	{
		OPENFILENAMEA openFileName;
		CHAR filePath[256] = { 0 };
		if (defaultFileName) 
		{
			strcpy_s(filePath, sizeof(filePath),defaultFileName);  //Buffer initial value (Show to the user as the default save name)
		}
		CHAR currentDirectory[256] = { 0 };
		ZeroMemory(&openFileName, sizeof(OPENFILENAME)); //Set the OPENFILENAME struct to 0
		openFileName.lStructSize = sizeof(OPENFILENAME);
		openFileName.hwndOwner = glfwGetWin32Window((GLFWwindow*)Application::GetApplicationInstance().getWindow().getNativeWindow());   //Get the windows window handle
		openFileName.lpstrFile = filePath;
		openFileName.nMaxFile = sizeof(filePath);
		openFileName.lpstrFilter = filter;
		openFileName.nFilterIndex = 1;
		openFileName.Flags = OFN_PATHMUSTEXIST | OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR;
		openFileName.lpstrDefExt = std::strchr(filter, '\0') + 1;  //Set the default extension by extracting it from the filter


		if (GetCurrentDirectoryA(sizeof(currentDirectory), currentDirectory))  //Get the current working directory and set it in currentDirectory buffer
		{
			openFileName.lpstrInitialDir = currentDirectory;
		}

		if (GetSaveFileNameA(&openFileName) == TRUE)
		{
			return openFileName.lpstrFile; //Return the file path
		}
		return std::string();  //Return empty string
	}

}