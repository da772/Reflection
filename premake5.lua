workspace "Reflection"
    architecture "x86_64"
    platforms 
    {
        "x86",
        "x86_64"
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
            "tests/src/*.cpp",
            "tests/src/*.hpp",
            "tests/src/*.h",
        }

        includedirs 
        {
       	    "include",
            "tests",
            "tests/include"
        }

        filter "system:linux"
            links
            {
                "dl"
            }
        filter "system:windows"
            defines
            {
                "MS_BUILD_BIN=\"$(MSBuildBinPath)\""
            }
        filter "platforms:x86"
            architecture "x86"
            defines 
            {
                "BUILD_ARCHITECTURE=\"x86\""
            }

        filter "platforms:x86_64"
            architecture "x86_64"
            defines 
            {
                "BUILD_ARCHITECTURE=\"x86_64\""
            }

        filter "configurations:Debug"
		    runtime "Debug"
		    symbols "On"
            defines 
            {
                "BUILD_CONFIG=\"Debug\""
            }
	    filter "configurations:Release"
		    runtime "Release"
		    optimize "On"
            defines 
            {
                "BUILD_CONFIG=\"Release\""
            }
    
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
            "tests/include",
            "tests/scripts"
        }
        filter "system:windows" 
            postbuildcommands
            {
               -- "XCOPY /I /E /S /Y /F \"$(TargetDir)%{prj.name}.dll\" \"bin/" .. outputdir .. "/Reflection_Tests/\"",
                --"XCOPY /I /E /S /Y /F \"$(TargetDir)%{prj.name}.pdb\" \"bin/" .. outputdir .. "/Reflection_Tests/\"",
                --"del /F \"$(TargetDir)%{prj.name}.pdb\""
            }
    
        filter "platforms:x86"
            architecture "x86"
    
        filter "platforms:x86_64"
            architecture "x86_64"
    
    
        filter "configurations:Debug"
            runtime "Debug"
            symbols "On"
            defines
            {
    
            }
        filter "configurations:Release"
            runtime "Release"
            optimize "On"
            defines
            {
                
            }
    