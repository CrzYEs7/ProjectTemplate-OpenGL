if _ACTION == "clean" then
	os.rmdir("bin")
	os.rmdir("Binaries")
	os.rmdir("obj")
	os.rmdir(".vs")
	os.rmdir("compile_commands")
	os.rmdir(".cache")
	os.remove("*.sln")
	os.remove("*.vcxproj")
	os.remove("*.ini")
	os.remove("**.filter")
	os.remove("**.filters")
	os.remove("**.user")
	os.remove("Makefile")
	os.remove("compile_commands.json")
end

include("Core/clean.lua")
include("App/clean.lua")
