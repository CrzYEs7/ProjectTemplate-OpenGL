if _ACTION == "clean" then
	os.rmdir("bin")
	os.rmdir("obj")
	os.rmdir(".vs")
	os.remove("*.sln")
	os.remove("*.vcxproj")
	os.remove("*.ini")
	os.remove("**.filter")
	os.remove("**.filters")
	os.remove("**.user")
	os.remove("Makefile")
	-- etc
end


include("Core/clean.lua")
include("App/clean.lua")
