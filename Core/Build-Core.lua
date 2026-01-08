project("Core")
kind("StaticLib")
language("C++")
cppdialect("C++20")
staticruntime("off")

files({
	"Source/**.h",
	"Source/**.hpp",
	"Source/**.cpp",
	"../deps/glad/src/**.cpp",
})

includedirs({
	"Source",
	"../deps/glad/include/",
	"../deps/GLFW/include",
})

links({
	"glfw3",
	"opengl32",
	"gdi32",
	"user32",
	"kernel32",
	"shell32",
	"advapi32",
})

libdirs({
	"../deps/GLFW/lib-mingw",
	"../deps/GLFW/lib",
})

targetdir("../Binaries/" .. OutputDir .. "/%{prj.name}")
objdir("../Binaries/Intermediates/" .. OutputDir .. "/%{prj.name}")

filter("system:windows")
systemversion("latest")
defines({})

filter("configurations:Debug")
defines({ "DEBUG" })
runtime("Debug")
symbols("On")

filter("configurations:Release")
defines({ "RELEASE" })
runtime("Release")
optimize("On")

filter("configurations:Dist")
defines({ "DIST" })
runtime("Release")
optimize("On")
symbols("Off")
