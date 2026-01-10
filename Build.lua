-- premake5.lua
require("export-compile-commands")
require("clean")
workspace("New Project")
architecture("x64")
configurations({ "Debug", "Release", "Dist" })
startproject("App")

language("C++")
cppdialect("C++20")
staticruntime("off")

filter("action:vs2022")
buildoptions({ "/EHsc", "/Zc:preprocessor", "/Zc:__cplusplus" })

OutputDir = "%{cfg.system}-%{cfg.architecture}/%{cfg.buildcfg}"

group("Core")
include("Core/Build-Core.lua")
group("")

include("App/Build-App.lua")
