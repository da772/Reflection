workspace "Reflection"
    architecture "x86_64"
    platforms 
    {
			"x86",
			"x64"
	}
	configurations
	{
		"Debug",
		"Release",
	}
    startproject "Reflection_Tests"
    outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"
    
    project "Reflection_Tests"
        kind "ConsoleApp"
        language "C++"
            cppdialect "C++11"
        staticruntime "off"
        targetdir ("%{prj.location}/bin/" .. outputdir .. "/%{prj.name}")
        objdir ("%{prj.location}/obj/" .. outputdir .. "/%{prj.name}")

        files 
        {
            "tests/*.cpp",
            "tests/*.hpp",
            "tests/*.h",
        }

        includedirs 
        {
       	    "include",
            "tests"
        }
        filter "system:linux"
            links
            {
                "dl"
            }
        filter "configurations:Debug"
		    runtime "Debug"
		    symbols "On"
	    filter "configurations:Release"
		    runtime "Release"
		    optimize "On"
    project "Reflection_Tests_Scripts"
        kind "SharedLib"
        language "C++"
            cppdialect "C++11"
        staticruntime "off"
        targetdir ("%{prj.location}/bin/" .. outputdir .. "/Reflection_Tests")
        objdir ("%{prj.location}/obj/" .. outputdir .. "/Reflection_Tests")
        files 
        {
            "tests/scripts/**.cpp",
            "tests/scripts/**.hpp",
            "tests/scripts/**.h",
        }

        includedirs 
        {
            "include",
            "tests",
            "tests/scripts"
        }

        filter "configurations:Debug"
            runtime "Debug"
            symbols "On"
        filter "configurations:Release"
            runtime "Release"
            optimize "On"
    