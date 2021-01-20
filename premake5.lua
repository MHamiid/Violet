workspace "Violet"
	architecture "x64"
	startproject "Sandbox"

	configurations{
	
		"Debug",
		"Release",
		"Distribution"
	
	}

outputDir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"



--Include Project for GLFW
project "GLFW"
	location "GLFW"
	kind "StaticLib"
	language "C"

	targetdir ("bin/" .. outputDir .. "/%{prj.name}")
	objdir ("bin-intermediates/" .. outputDir .. "/%{prj.name}")

	GLFWSrcDir = "Violet/vendor/GLFW/"

	files
	{
		GLFWSrcDir .."include/GLFW/glfw3.h",
		GLFWSrcDir .."include/GLFW/glfw3native.h",
		GLFWSrcDir .."src/glfw_config.h",
		GLFWSrcDir .."src/context.c",
		GLFWSrcDir .."src/init.c",
		GLFWSrcDir .."src/input.c",
		GLFWSrcDir .."src/monitor.c",
		GLFWSrcDir .."src/vulkan.c",
		GLFWSrcDir .."src/window.c"
	}
	filter "system:linux"
		pic "On"

		systemversion "latest"
		staticruntime "Off"

		files
		{
			GLFWSrcDir .."src/x11_init.c",
			GLFWSrcDir .."src/x11_monitor.c",
			GLFWSrcDir .."src/x11_window.c",
			GLFWSrcDir .."src/xkb_unicode.c",
			GLFWSrcDir .."src/posix_time.c",
			GLFWSrcDir .."src/posix_thread.c",
			GLFWSrcDir .."src/glx_context.c",
			GLFWSrcDir .."src/egl_context.c",
			GLFWSrcDir .."src/osmesa_context.c",
			GLFWSrcDir .."src/linux_joystick.c"
		}

		defines
		{
			"_GLFW_X11"
		}

	filter "system:windows"
		systemversion "latest"
		staticruntime "Off"

		files
		{
			GLFWSrcDir .."src/win32_init.c",
			GLFWSrcDir .."src/win32_joystick.c",
			GLFWSrcDir .."src/win32_monitor.c",
			GLFWSrcDir .."src/win32_time.c",
			GLFWSrcDir .."src/win32_thread.c",
			GLFWSrcDir .."src/win32_window.c",
			GLFWSrcDir .."src/wgl_context.c",
			GLFWSrcDir .."src/egl_context.c",
			GLFWSrcDir .."src/osmesa_context.c"
		}

		defines 
		{ 
			"_GLFW_WIN32",
			"_CRT_SECURE_NO_WARNINGS"
		}

	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		optimize "on"

project "Violet"
	location "Violet"
	kind "SharedLib"
	language "C++"
	
	targetdir ("bin/" .. outputDir .. "/%{prj.name}")
	objdir("bin-intermediates/" .. outputDir .. "/%{prj.name}")

	--Pre-Compiled Header
	pchheader "VIOPCH.h"
	pchsource "%{prj.name}/src/VIOPCH.cpp"

	files{
	
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs{
		 
		"%{prj.name}/vendor/spdlog/include", --include dir for spdlog
		"%{prj.name}/src",					 --Include dir for Violet project src folder
		"%{prj.name}/vendor/GLFW/include"		 --include dir for GLFW
	}

	links{
		"GLFW"
	}

	filter "system:windows"
		cppdialect "c++17"
		staticruntime "Off"
		systemversion "latest"

		defines{

			"VIO_PLATFORM_WINDOWS",
			"VIO_BUILD_DLL"
		}

		postbuildcommands{
		
			("{COPY} %{cfg.buildtarget.relpath} ../bin/"   .. outputDir .. "/Sandbox")

		}


	filter "configurations:Debug"
		defines "VIO_DEBUG_MODE"
		symbols "On"
		defines{ "VIO_ENABLE_ASSERTS"}

	filter "configurations:Release"
		defines "VIO_RELEASE_MODE"
		optimize "On"

	filter "configurations:Distribution"
		defines "VIO_DISTRIBUTION_MODE"
		optimize "On"

project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"

	targetdir("bin/" .. outputDir .. "/%{prj.name}")
	objdir("bin-intermediates/" .. outputDir .. "/%{prj.name}")

	files{
	
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs{
		
		"Violet/vendor/spdlog/include",
		"Violet/src"
	}

	links{

		"Violet"
	}

	filter "system:windows"
		cppdialect "c++17"
		staticruntime "Off"
		systemversion "latest"

		defines{

			"VIO_PLATFORM_WINDOWS"
		}


	filter "configurations:Debug"
		defines "VIO_DEBUG_MODE"
		symbols "On"

	filter "configurations:Release"
		defines "VIO_RELEASE_MODE"
		optimize "On"

	filter "configurations:Distribution"
		defines "VIO_DISTRIBUTION_MODE"
		optimize "On"