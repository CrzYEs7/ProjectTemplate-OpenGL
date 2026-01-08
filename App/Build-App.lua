project("App")
kind("ConsoleApp")
language("C++")
cppdialect("C++20")
targetdir("Binaries/%{cfg.buildcfg}")
staticruntime("off")

files({ "Source/**.hpp", "Source/**.h", "Source/**.cpp" })

includedirs({
	"Source",
	"../Core/Source",
	"../deps/glad/include",
	"../deps/glfw/include",
})

links({
	"Core",
	"glfw3",
	"opengl32",
	"gdi32",
	"user32",
	"kernel32",
	"shell32",
	"advapi32",
})

libdirs({
	"../Binaries/" .. OutputDir .. "/Core",
	"../deps/GLFW/lib-mingw-w64",
	"../deps/GLFW/lib",
})

filter("system:windows")
kind("ConsoleApp")
linkoptions({ "-mconsole" })

targetdir("../Binaries/" .. OutputDir .. "/%{prj.name}")
objdir("../Binaries/Intermediates/" .. OutputDir .. "/%{prj.name}")

filter({ "system:windows", "action=vs2022" })
systemversion("latest")
defines({ "WINDOWS" })

filter("configurations:Debug")
defines({ "DEBUG" })
runtime("Debug")
symbols("On")

filter("configurations:Release")
defines({ "RELEASE" })
runtime("Release")
optimize("On")
symbols("On")

filter("configurations:Dist")
defines({ "DIST" })
runtime("Release")
optimize("On")
symbols("Off")
