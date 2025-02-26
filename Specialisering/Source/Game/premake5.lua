include "../../Premake/extensions.lua"

workspace "Game"
	location "../../"
	startproject "Editor"
	architecture "x64"

	configurations {
		"Debug",
		"Release",
		"Retail"
	}

-- include for common stuff 
include "../../Premake/common.lua"

include (dirs.external)
include (dirs.engine)


-------------------------------------------------------------
project "Game"
	location (dirs.projectfiles)
	dependson { "External", "Engine" }
		
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"

	debugdir "%{dirs.bin}"
	targetdir ("%{dirs.bin}")
	targetname("%{prj.name}_%{cfg.buildcfg}")
	objdir ("%{dirs.temp}/%{prj.name}/%{cfg.buildcfg}")

	links {"External", "Engine"}

	includedirs { 
		dirs.external, 
		dirs.engine,
		dirs.game
	}

	files {
		"source/**.h",
		"source/**.cpp",
		"source/**.hpp"
	}

	libdirs { 
		dirs.lib, 
		dirs.dependencies,
	}
	verify_or_create_settings("Game")
		--Debug
	filter "configurations:Debug"
		defines {"_DEBUG"}
		runtime "Debug"
		symbols "on"
		files {"tools/**"}
		includedirs {"tools/"}
		links {}
		-- Release
	filter "configurations:Release"
		defines "_RELEASE"
		runtime "Release"
		optimize "on"
		files {"tools/**"}
		includedirs {"tools/"}
		links {}
		--Retail
	filter "configurations:Retail"
		defines "_RETAIL"
		runtime "Release"
		optimize "on"
		includedirs {"tools/"}
		links {}
	filter "system:windows"
	--		kind "StaticLib"
		staticruntime "off"
		symbols "On"		
		systemversion "latest"
		warnings "Extra"
		--conformanceMode "On"
		--buildoptions { "/permissive" }
		flags { 
		--	"FatalWarnings", -- would be both compile and lib, the original didn't set lib
			"FatalCompileWarnings",
			"MultiProcessorCompile"
		}
		
		defines {
			"WIN32",
			"_LIB", 
			"TGE_SYSTEM_WINDOWS" 
		}
project "GameLauncher"
	location (dirs.projectfiles)
	dependson { "External", "Engine", "Game" }
		
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"

	debugdir "%{dirs.bin}"
	targetdir ("%{dirs.bin}")
	targetname("%{prj.name}_%{cfg.buildcfg}")
	objdir ("%{dirs.temp}/%{prj.name}/%{cfg.buildcfg}")

	links {"External", "Engine","Game"}

	includedirs { 
		dirs.external, 
		dirs.engine,
		"../Game/Source"
	}

	files {
		"../GameLauncher/**.h",
		"../GameLauncher/**.cpp",
		"../GameLauncher/**.hpp"
	}

	libdirs { 
		dirs.lib, 
		dirs.dependencies,
	}
	verify_or_create_settings("Game")
		--Debug
	filter "configurations:Debug"
		defines {"_DEBUG"}
		runtime "Debug"
		symbols "on"
		files {"tools/**"}
		includedirs {"tools/"}
		links {}
		-- Release
	filter "configurations:Release"
		defines "_RELEASE"
		runtime "Release"
		optimize "on"
		files {"tools/**"}
		includedirs {"tools/"}
		links {}
		--Retail
	filter "configurations:Retail"
		defines "_RETAIL"
		runtime "Release"
		optimize "on"
		includedirs {"tools/"}
		links {}
	filter "system:windows"
	--	kind "StaticLib"
		staticruntime "off"
		symbols "On"		
		systemversion "latest"
		warnings "Extra"
		--conformanceMode "On"
		--buildoptions { "/permissive" }
		flags { 
		--	"FatalWarnings", -- would be both compile and lib, the original didn't set lib
			"FatalCompileWarnings",
			"MultiProcessorCompile"
		}
		
		defines {
			"WIN32",
			"_LIB", 
			"TGE_SYSTEM_WINDOWS" 
		}
project "Editor"
	location (dirs.projectfiles)
	dependson { "External", "Engine","Game", "GameLauncher" }
		
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"

	debugdir "%{dirs.bin}"
	targetdir ("%{dirs.bin}")
	targetname("%{prj.name}_%{cfg.buildcfg}")
	objdir ("%{dirs.temp}/%{prj.name}/%{cfg.buildcfg}")

	links {"External", "Engine","Game"}

	verify_or_create_settings("Editor")

	includedirs { 
		dirs.external, 
		dirs.engine,
		dirs.editor,
		dirs.game

	}

	files {
		"../Editor/**.h",
		"../Editor/**.cpp",
		"../Editor/**.hpp"
	}

	libdirs { 
		dirs.lib, 
		dirs.dependencies,
	}
		--Debug
	filter "configurations:Debug"
		defines {"_DEBUG"}
		runtime "Debug"
		symbols "on"
		files {"tools/**"}
		includedirs {"tools/"}
		links {}
		-- Release
	filter "configurations:Release"
		defines "_RELEASE"
		runtime "Release"
		optimize "on"
		files {"tools/**"}
		includedirs {"tools/"}
		links {}
		--Retail
	filter "configurations:Retail"
		defines "_RETAIL"
		runtime "Release"
		optimize "on"
		includedirs {"tools/"}
		links {}
	filter "system:windows"
	--		kind "StaticLib"
		staticruntime "off"
		symbols "On"		
		systemversion "latest"
		warnings "Extra"
		--conformanceMode "On"
		--buildoptions { "/permissive" }
		flags { 
		--	"FatalWarnings", -- would be both compile and lib, the original didn't set lib
			"FatalCompileWarnings",
			"MultiProcessorCompile"
		}
		
		defines {
			"WIN32",
			"_LIB", 
			"TGE_SYSTEM_WINDOWS" 
		}
