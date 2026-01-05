if _ACTION == "clean" then
	os.rmdir("bin")
	os.rmdir("obj")
	os.remove("*.sln")
	os.remove("*.vcxproj")
	os.remove("*.ini")
	os.remove("Makefile")
	-- etc
end
