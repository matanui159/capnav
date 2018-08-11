local function custom_project(name, type, path)
	project (name)
		kind (type)
		files (path .. "src/**.c")
		objdir (path .. "bin/%{cfg.buildcfg}/obj")
		targetdir (path .. "bin/%{cfg.buildcfg}")
end

workspace "capnav"
	configurations {"Debug", "Release"}
	language "C"
	cdialect "C99"

	buildoptions {
		"-Wall",
		"-mno-stack-arg-probe"
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
			"upx --ultra-brute bin/%{cfg.buildcfg}/capnav.exe",
			"upx --ultra-brute cli/bin/%{cfg.buildcfg}/cncli.exe"
		}

custom_project("capnav", "WindowedApp", "")
custom_project("cncli", "ConsoleApp", "cli/")