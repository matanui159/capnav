workspace "capscroll"
	configurations { "Debug", "Release" }

project "capscroll"
	kind "WindowedApp"
	language "C"
	cdialect "C99"
	files "src/main.c"
	objdir "bin/%{cfg.buildcfg}/obj"
	targetdir "bin/%{cfg.buildcfg}"

	linkoptions {
		"-nostdlib",
		"-Wl,-e_sys_entry"
	}

	filter "system:windows"
		files "src/sys/win32.c"
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
			"upx --ultra-brute bin/%{cfg.buildcfg}/%{prj.name}.exe"
		}