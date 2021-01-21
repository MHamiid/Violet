project "Glad"
	kind "StaticLib"
	language "C"

	targetdir ("bin/" .. outputDir .. "/%{prj.name}")
	objdir ("bin-intermediates/" .. outputDir .. "/%{prj.name}")

	files
	{	
		"include/glad/glad.h",
		"KHR/khrplatform.h",
		"src/glad.c"

	}
	
	includedirs{
		"include"
	}

	filter "system:windows"
		systemversion "latest"
		staticruntime "Off"


	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		optimize "on"