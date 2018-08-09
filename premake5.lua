local function custom_project(name, type, path)
	project (name)
		kind (type)
		files (path .. "src/**.c")
		objdir (path .. "bin/%{cfg.buildcfg}/obj")
		targetdir (path .. "bin/%{cfg.buildcfg}")

		filter "Release"
			postbuildcommands {
				"upx --ultra-brute " .. path .. "bin/%{cfg.buildcfg}/%{prj.name}.exe"
			}
end

workspace "capnav"
	configurations {"Debug", "Release"}
	language "C"
	cdialect "C99"

	links {
		"kernel32",
		"user32"
	}

	linkoptions {
		"-nostdlib",
		"-Wl,-e_entry"
	}

	filter "Debug"
		symbols "On"

	filter "Release"
		optimize "Size"
		flags "LinkTimeOptimization"

custom_project("capnav", "WindowedApp", "")
custom_project("cncli", "ConsoleApp", "cli/")