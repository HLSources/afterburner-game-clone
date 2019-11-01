def options(opt):
	pass

def configure(conf):
	conf.env.append_unique("DEFINES", "GAME_LIBRARIES")

	if conf.env.DEST_OS == 'win32':
		conf.env.append_unique("DEFINES",
		[
			"_WIN32",
			"WIN32",
			"NOMINMAX"
		])

		# Needed to avoid warnings with MiniUTL.
		# 4005 = Macro redefinition
		# 4244 = Possible loss of data on type cast
		conf.env.append_unique("CXXFLAGS",
		[
			"/WX",
			"/wd4005",
			"/wd4244"
		])
	else:
		conf.env.append_unique("DEFINES",
		[
			"_LINUX",	# Apparently this covers all non-Windows platforms adequately.
			"stricmp=strcasecmp",
			"strnicmp=strncasecmp",
			"_snprintf=snprintf",
			"_vsnprintf=vsnprintf"
		])

		conf.env.append_unique("CXXFLAGS",
		[
			"-fno-exceptions",
			"-Wno-invalid-offsetof"
		])