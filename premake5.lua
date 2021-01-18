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
	
	targetdir ("bin/" .. outputDir .. "/%{prj.name}")
	objdir("bin-intermediates/" .. outputDir .. "/%{prj.name}")

	files{
	
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs{
		
		"%{prj.name}/vendor/spdlog/include"
	}

	filter "system:windows"
		cppdialect "c++17"
		staticruntime "On"
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
		staticruntime "On"
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