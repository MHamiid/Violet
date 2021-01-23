workspace "Violet"
	architecture "x64"
	startproject "Sandbox"

	configurations{
	
		"Debug",
		"Release",
		"Distribution"
	
	}

outputDir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"


project "Violet"
	location "Violet"
	kind "SharedLib"
	language "C++"
	staticruntime "Off"
	
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
		 
		"%{prj.name}/src",					 --Include dir for Violet project src folder
		"%{prj.name}/vendor/spdlog/include", --include dir for spdlog
		"%{prj.name}/vendor/GLFW/include",	 --include dir for GLFW
		"%{prj.name}/vendor/Glad/include",	 --include dir for Glad
		"%{prj.name}/vendor/imgui"	 --include dir for Glad
	}

	links{
		"GLFW",
		"Glad",
		"ImGui"
	
	}

	filter "system:windows"
		cppdialect "c++17"
		systemversion "latest"

		defines{

			"VIO_PLATFORM_WINDOWS",
			"VIO_BUILD_DLL",
			"GLFW_INCLUDE_NONE" --Avoid including OpenGl headers when ever including GLFW to avoid error from Glad (Glad already includes OpenGl headers)
		}

		postbuildcommands{
		
			("{COPY} %{cfg.buildtarget.relpath} ../bin/"   .. outputDir .. "/Sandbox")

		}


	filter "configurations:Debug"
		defines "VIO_DEBUG_MODE"
		symbols "On"
		runtime "Debug"
		
	filter "configurations:Release"
		defines "VIO_RELEASE_MODE"
		runtime "Release"
		optimize "On"

	filter "configurations:Distribution"
		defines "VIO_DISTRIBUTION_MODE"
		runtime "Release"
		optimize "On"

project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"
	staticruntime "Off"

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
		systemversion "latest"

		defines{

			"VIO_PLATFORM_WINDOWS"
		}


	filter "configurations:Debug"
		defines "VIO_DEBUG_MODE"
		runtime "Debug"
		symbols "On"

	filter "configurations:Release"
		defines "VIO_RELEASE_MODE"
		runtime "Release"
		optimize "On"

	filter "configurations:Distribution"
		defines "VIO_DISTRIBUTION_MODE"
		runtime "Release"
		optimize "On"



--Submodules


group "Dependencies"

--Include Glad premake file
include "Violet/vendor/Glad"

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



--Include Project for GLFW
project "ImGui"
	location "ImGui"
	kind "StaticLib"
	language "C++"

	targetdir ("bin/" .. outputDir .. "/%{prj.name}")
	objdir ("bin-intermediates/" .. outputDir .. "/%{prj.name}")

	ImGuiSrcDir = "Violet/vendor/imgui/"

	files
	{
		ImGuiSrcDir .."imconfig.h",
        ImGuiSrcDir .."imgui.h",
        ImGuiSrcDir .."imgui.cpp",
        ImGuiSrcDir .."imgui_draw.cpp",
        ImGuiSrcDir .."imgui_internal.h",
        ImGuiSrcDir .."imgui_widgets.cpp",
        ImGuiSrcDir .."imstb_rectpack.h",
        ImGuiSrcDir .."imstb_textedit.h",
        ImGuiSrcDir .."imstb_truetype.h",
        ImGuiSrcDir .."imgui_demo.cpp",
		ImGuiSrcDir .."imgui_tables.cpp",
		ImGuiSrcDir .."backends/imgui_impl_opengl3.h",
		ImGuiSrcDir .."backends/imgui_impl_opengl3.cpp",
		ImGuiSrcDir .."backends/imgui_impl_glfw.h",
		ImGuiSrcDir .."backends/imgui_impl_glfw.cpp",
		ImGuiSrcDir .."examples/libs/glfw/include/**"

	}
	
	includedirs{
		
		ImGuiSrcDir,
		ImGuiSrcDir .."examples/libs/glfw/include",
		"Violet/vendor/Glad/include"
	
	}

	filter "system:windows"
		systemversion "latest"
		cppdialect "C++17"
		staticruntime "Off"


	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		optimize "on"


group "" --Dependencies