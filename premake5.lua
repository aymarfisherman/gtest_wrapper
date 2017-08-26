deps_dir = "D:/workspace/common_deps/"

workspace "Workspace"
	configurations {"Debug", "Release"}
	location "./build"

	includedirs {
        deps_dir .. "boost_1_64_0",
        deps_dir .. "googletest/googletest/include",
		deps_dir .. "googletest/googlemock/include",
	}
	
	libdirs {
        deps_dir .. "boost_1_64_0/stage/lib",
		deps_dir .. "googletest/lib",
	}
    
    filter "configurations:Debug"
		targetsuffix "_d"
		defines {"DEBUG", "_DEBUG", "_CRT_SECURE_NO_WARNINGS", "_SCL_SECURE_NO_WARNINGS", "MSVC_IMPLICIT_LINK"}
		flags {"Symbols", "Unicode"}
		
	filter "configurations:Release"        
		defines {"NDEBUG", "_CRT_SECURE_NO_WARNINGS", "_SCL_SECURE_NO_WARNINGS", "MSVC_IMPLICIT_LINK"}
		flags {"Unicode"}
		optimize "On"

project "GTestWrapper"
	kind "None"
	
	includedirs {
		"./include",
	}
	
	files {
		"./include/**.h",
		"./src/**.cpp",
	}
	
project "GTestWrapperTest"
	kind "ConsoleApp"
	targetdir "./bin"
	    
    includedirs {
        "./include",
	}
		
	files {
		"./tests/main.cpp",
	}
	
	links {
		"GTestWrapper",
	}