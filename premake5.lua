workspace "capnav"
	configurations {"Debug", "Release"}

project "capnav"
	kind "WindowedApp"
	files "src/**.c"
	objdir "bin/%{cfg.buildcfg}/obj"
	targetdir "bin/%{cfg.buildcfg}"
	language "C"
	cdialect "C99"

	buildoptions {
		"-Wall"
	}

	linkoptions {
		"-nostdlib",
		"-Wl,-e_entry",
	}

	links {
		"kernel32",
		"user32"
	}

	filter "Debug"
		symbols "On"

	filter "Release"
		optimize "Size"
		flags "LinkTimeOptimization"
		postbuildcommands {
			"upx --ultra-brute bin/%{cfg.buildcfg}/capnav.exe"
		}