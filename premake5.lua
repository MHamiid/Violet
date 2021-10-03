workspace "Violet"
	architecture "x86_64"
	startproject "Violet-Editor"

	configurations{
	
		"Debug",
		"Release",
		"Distribution"
	
	}

	flags
	{
		"MultiProcessorCompile"
	}

outputDir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"


project "Violet"
	location "Violet"
	kind "StaticLib"
	language "C++"
	cppdialect "c++17"
	staticruntime "on"
	
	targetdir ("bin/" .. outputDir .. "/%{prj.name}")
	objdir("bin-intermediates/" .. outputDir .. "/%{prj.name}")

	--Pre-Compiled Header
	pchheader "VIOPCH.h"
	pchsource "%{prj.name}/src/VIOPCH.cpp"
	
	files{
	
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/vendor/stb_image/**.h",				--Include stb_image to be included and compiled by the project
		"%{prj.name}/vendor/stb_image/**.cpp",		
		"%{prj.name}/vendor/ImGuizmo/ImGuizmo.h",			--Include ImGuizmo to be included and compiled by the project
		"%{prj.name}/vendor/ImGuizmo/ImGuizmo.cpp"
	}

	includedirs{
		 
		"%{prj.name}/src",									--Include dir for Violet project src folder
		"%{prj.name}/vendor/spdlog/include",				--Include dir for spdlog
		"%{prj.name}/vendor/GLFW/include",					--Include dir for GLFW
		"%{prj.name}/vendor/Glad/include",					--Include dir for Glad
		"%{prj.name}/vendor/imgui",							--Include dir for ImGui
		"%{prj.name}/vendor/glm",							--Include dir for glm
		"%{prj.name}/vendor/stb_image",						--Include dir for stb_image
		"%{prj.name}/vendor/entt/single_include/entt",	    --Include dir for entt
		"%{prj.name}/vendor/yaml-cpp/include",				--Include dir for yaml-cpp
		"%{prj.name}/vendor/ImGuizmo",						--Include dir for ImGuizmo
		"%{prj.name}/vendor/Box2D/include"					--Include dir for Box2D

	}

	links{
		"GLFW",
		"Glad",
		"ImGui",
		"yaml-cpp",
		"Box2D"
	
	}


	filter "files:Violet/vendor/ImGuizmo/**.cpp"
		flags { "NoPCH" }

	filter "system:windows"
		systemversion "latest"

		defines{

			"VIO_PLATFORM_WINDOWS",
			"GLFW_INCLUDE_NONE" --Avoid including OpenGl headers when ever including GLFW to avoid error from Glad (Glad already includes OpenGl headers)
		}

	--[[
		--When build is set to "SharedLib"
		defines{

			"VIO_SHARED_LIB",
			"VIO_BUILD_DLL"
		}
		For copying dll file for SharedLib build
		postbuildcommands{
		
			("{COPY} %{cfg.buildtarget.relpath} \"../bin/"   .. outputDir .. "/Sandbox/\"")
		}
	--]]

	filter "configurations:Debug"
		defines "VIO_DEBUG_MODE"
		symbols "on"
		runtime "Debug"
		
	filter "configurations:Release"
		defines "VIO_RELEASE_MODE"
		runtime "Release"
		optimize "on"

	filter "configurations:Distribution"
		defines "VIO_DISTRIBUTION_MODE"
		runtime "Release"
		optimize "on"

project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"
	cppdialect "c++17"
	staticruntime "on"

	targetdir("bin/" .. outputDir .. "/%{prj.name}")
	objdir("bin-intermediates/" .. outputDir .. "/%{prj.name}")

	files{
	
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs{
		
		"Violet/vendor/spdlog/include",
		"Violet/src",
		"Violet/vendor/glm",								--Include dir for glm
		"Violet/vendor/imgui",								--Include dir for ImGui
		"Violet/vendor/entt/single_include/entt"		    --Include dir for entt

	}

	links{

		"Violet"
	}

	filter "system:windows"
		systemversion "latest"

		defines{

			"VIO_PLATFORM_WINDOWS",
			--"VIO_SHARED_LIB" -When Violet build is set to "SharedLib"
		}


	filter "configurations:Debug"
		defines "VIO_DEBUG_MODE"
		runtime "Debug"
		symbols "On"

	filter "configurations:Release"
		defines "VIO_RELEASE_MODE"
		runtime "Release"
		optimize "on"

	filter "configurations:Distribution"
		defines "VIO_DISTRIBUTION_MODE"
		runtime "Release"
		optimize "on"



project "Violet-Editor"
	location "Violet-Editor"
	kind "ConsoleApp"
	language "C++"
	cppdialect "c++17"
	staticruntime "on"

	targetdir("bin/" .. outputDir .. "/%{prj.name}")
	objdir("bin-intermediates/" .. outputDir .. "/%{prj.name}")

	files{
	
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs{
		
		"Violet/vendor/spdlog/include",
		"Violet/src",
		"Violet/vendor/glm",								--Include dir for glm
		"Violet/vendor/imgui",								--Include dir for ImGui
		"Violet/vendor/entt/single_include/entt",			--Include dir for entt
		"Violet/vendor/ImGuizmo"							--Include dir for ImGuizmo

	}

	links{

		"Violet"
	}

	filter "system:windows"
		systemversion "latest"

		defines{

			"VIO_PLATFORM_WINDOWS",
			--"VIO_SHARED_LIB" -When Violet build is set to "SharedLib"
		}


	filter "configurations:Debug"
		defines "VIO_DEBUG_MODE"
		runtime "Debug"
		symbols "On"

	filter "configurations:Release"
		defines "VIO_RELEASE_MODE"
		runtime "Release"
		optimize "on"

	filter "configurations:Distribution"
		defines "VIO_DISTRIBUTION_MODE"
		runtime "Release"
		optimize "on"




--Submodules


group "Dependencies"

--Include Glad premake file
include "Violet/vendor/Glad"

project "GLFW"
	location "GLFW"
	kind "StaticLib"
	language "C"
	staticruntime "on"

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
		pic "on"
		systemversion "latest"

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



project "ImGui"
	location "ImGui"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

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
		ImGuiSrcDir .."examples/libs/glfw/include/**",
		ImGuiSrcDir .."misc/cpp/imgui_stdlib.h",        --imgui_stdlib.h + imgui_stdlib.cpp InputText() wrappers for C++ standard library (STL) type: std::string.
		ImGuiSrcDir .."misc/cpp/imgui_stdlib.cpp"

	}
	
	includedirs{
		
		ImGuiSrcDir,
		ImGuiSrcDir .."examples/libs/glfw/include",
		"Violet/vendor/Glad/include"
	
	}

	filter "system:windows"
		systemversion "latest"

	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		optimize "on"



project "yaml-cpp"
	location "yaml-cpp"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("bin/" .. outputDir .. "/%{prj.name}")
	objdir ("bin-intermediates/" .. outputDir .. "/%{prj.name}")

	YamlCppSrcDir = "Violet/vendor/yaml-cpp/"

	files
	{
		YamlCppSrcDir .."src/**.h",
        YamlCppSrcDir .."src/**.cpp",
        YamlCppSrcDir .."include/**.h"
	}
	
	includedirs{
		
		YamlCppSrcDir .."include"
	}

	filter "system:windows"
		systemversion "latest"

	filter "system:linux"
		pic "on"
		systemversion "latest"

	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		optimize "on"

project "Box2D"
	location "Box2D"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("bin/" .. outputDir .. "/%{prj.name}")
	objdir ("bin-intermediates/" .. outputDir .. "/%{prj.name}")

	Box2DSrcDir = "Violet/vendor/Box2D/"

	files
	{
		Box2DSrcDir .."src/**.h",
        Box2DSrcDir .."src/**.cpp",
        Box2DSrcDir .."include/**.h"
	}
	
	includedirs{
		
		Box2DSrcDir .."include",
		Box2DSrcDir .."src"
	}

	filter "system:windows"
		systemversion "latest"

	filter "system:linux"
		pic "on"
		systemversion "latest"

	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		optimize "on"

group "" --Dependencies
