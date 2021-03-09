workspace "Reflection"
    architecture "x86_64"
	configurations
	{
		"Debug",
		"Release",
	}

    outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"
    project "Reflection_Tests"
        kind "ConsoleApp"
        language "C++"
            cppdialect "C++11"
        staticruntime "on"

        targetdir ("%{prj.location}/bin/" .. outputdir .. "/%{prj.name}")
        objdir ("%{prj.location}/obj/" .. outputdir .. "/%{prj.name}")

        files 
        {
            "src/**.cpp",
            "src/**.c",
            "tests/**.cpp",
            "tests/**.hpp",
            "tests/**.h",
        }

        includedirs 
        {
       	    "include"
        }

        filter "configurations:Debug"
		runtime "Debug"
		symbols "On"
	filter "configurations:Release"
		runtime "Release"
		optimize "On"
