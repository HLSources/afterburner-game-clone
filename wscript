#! /usr/bin/env python
# encoding: utf-8
# a1batross, mittorn, 2018

from __future__ import print_function
from waflib import Logs, Context, Configure
import sys
import os

VERSION = '0.99'
APPNAME = 'xash3d-fwgs'
top = '.'

Context.Context.line_just = 55 # should fit for everything on 80x26

class Subproject:
	name      = ''
	dedicated = True  # if true will be ignored when building dedicated server
	singlebin = False # if true will be ignored when singlebinary is set
	ignore    = False # if true will be ignored, set by user request

	def __init__(self, name, dedicated=True, singlebin=False):
		self.name = name
		self.dedicated = dedicated
		self.singlebin = singlebin

SUBDIRS = [
	Subproject('public',      dedicated=False),
	Subproject('game_launch', singlebin=True),
	Subproject('ref_gl'),
#	Subproject('ref_soft'),
	Subproject('mainui'),
	Subproject('vgui_support'),
	Subproject('engine', dedicated=False),
	Subproject('game_content', dedicated=False),
	Subproject('game_libs/cl_dll',   dedicated=False),
	Subproject('game_libs/dlls',   dedicated=False),
]

@Configure.conf
def get_taskgen_count(self):
	try: idx = self.tg_idx_count
	except: idx = 0 # don't set tg_idx_count to not increase counter
	return idx

def subdirs():
	return map(lambda x: x.name, SUBDIRS)

def options(opt):
	grp = opt.add_option_group('Common options')

	grp.add_option('-T', '--build-type', action='store', dest='BUILD_TYPE', default = None,
		help = 'build type: debug, release or none(custom flags)')

	grp.add_option('-d', '--dedicated', action = 'store_true', dest = 'DEDICATED', default = False,
		help = 'build Xash Dedicated Server [default: %default]')

	grp.add_option('--single-binary', action = 'store_true', dest = 'SINGLE_BINARY', default = False,
		help = 'build single "xash" binary (always enabled for dedicated) [default: %default]')

	grp.add_option('-8', '--64bits', action = 'store_true', dest = 'ALLOW64', default = False,
		help = 'allow targetting 64-bit engine(Linux/Windows/OSX x86 only) [default: %default]')

	grp.add_option('-W', '--win-style-install', action = 'store_true', dest = 'WIN_INSTALL', default = False,
		help = 'install like Windows build, ignore prefix, useful for development [default: %default]')

	grp.add_option('--enable-bsp2', action = 'store_true', dest = 'SUPPORT_BSP2_FORMAT', default = False,
		help = 'build engine and renderers with BSP2 map support(recommended for Quake, breaks compatibility!) [default: %default]')

	grp.add_option('--enable-lto', action = 'store_true', dest = 'LTO', default = False,
		help = 'enable Link Time Optimization [default: %default]')

	grp.add_option('--enable-poly-opt', action = 'store_true', dest = 'POLLY', default = False,
		help = 'enable polyhedral optimization if possible [default: %default]')

	opt.load('subproject')

	opt.add_subproject(subdirs())

	opt.load('xcompile compiler_cxx compiler_c sdl2 clang_compilation_database strip_on_install')
	if sys.platform == 'win32':
		opt.load('msvc msdev msvs')
	opt.load('reconfigure')

def configure(conf):
	conf.env.GAMEDIR = "afterburner"

	conf.load('fwgslib reconfigure')
	conf.start_msg('Build type')
	if conf.options.BUILD_TYPE == None:
		conf.end_msg('not set', color='RED')
		conf.fatal('Please set a build type, for example "-T release"')
	elif not conf.options.BUILD_TYPE in ['fast', 'release', 'debug', 'nooptimize', 'sanitize', 'none']:
		conf.end_msg(conf.options.BUILD_TYPE, color='RED')
		conf.fatal('Invalid build type. Valid are "debug", "release" or "none"')
	conf.end_msg(conf.options.BUILD_TYPE)

	# -march=native should not be used
	if conf.options.BUILD_TYPE == 'fast':
		Logs.warn('WARNING: \'fast\' build type should not be used in release builds')

	conf.load('subproject')

	# Force XP compability, all build targets should add
	# subsystem=bld.env.MSVC_SUBSYSTEM
	# TODO: wrapper around bld.stlib, bld.shlib and so on?
	conf.env.MSVC_SUBSYSTEM = 'WINDOWS,5.01'
	conf.env.MSVC_TARGETS = ['x86'] # explicitly request x86 target for MSVC
	if sys.platform == 'win32':
		conf.load('msvc msvcfix msdev msvs')
	conf.load('xcompile compiler_c compiler_cxx gitversion clang_compilation_database strip_on_install')

	# Every static library must have fPIC
	if conf.env.DEST_OS != 'win32' and '-fPIC' in conf.env.CFLAGS_cshlib:
		conf.env.append_unique('CFLAGS_cstlib', '-fPIC')
		conf.env.append_unique('CXXFLAGS_cxxstlib', '-fPIC')

	# modify options dictionary early
	if conf.env.DEST_OS == 'android':
		conf.options.NO_VGUI = True # skip vgui
		conf.options.NANOGL = True
		conf.options.GLWES  = True
		conf.options.GL     = False

	# We restrict 64-bit builds ONLY for Win/Linux/OSX running on Intel architecture
	# Because compatibility with original GoldSrc
	if conf.env.DEST_OS in ['win32', 'linux', 'darwin'] and conf.env.DEST_CPU in ['x86_64']:
		conf.env.BIT32_MANDATORY = not conf.options.ALLOW64
		if not conf.env.BIT32_MANDATORY:
			Logs.info('WARNING: will build engine for 32-bit target')
	else:
		conf.env.BIT32_MANDATORY = False
	conf.load('force_32bit')

	linker_flags = {
		'common': {
			'msvc':  ['/DEBUG', '/WX'], # always create PDB, doesn't affect result binaries
			'gcc':   ['-Wl,--no-undefined']
		},
		'sanitize': {
			'clang': ['-fsanitize=undefined', '-fsanitize=address'],
			'gcc':   ['-fsanitize=undefined', '-fsanitize=address'],
		}
	}

	compiler_c_cxx_flags = {
		'common': {
			# disable thread-safe local static initialization for C++11 code, as it cause crashes on Windows XP
			'msvc':    ['/D_USING_V110_SDK71_', '/Zi', '/FS', '/Zc:threadSafeInit-', '/WX', '/wd4005', '/EHsc'],
			'clang': [
				'-g',
				'-gdwarf-2',
				'-fvisibility=hidden',
				'-Werror',
				'-Wno-format-truncation'
			],
			'gcc': [
				'-g',
				'-fvisibility=hidden',
				'-fdiagnostics-color=always',
				'-Werror',
				'-Wno-format-truncation'
			]
		},
		'fast': {
			'msvc':    ['/O2', '/Oy', '/MT', '/DNDEBUG'], #todo: check /GL /LTCG
			'gcc':     ['-Ofast', '-march=native', '-funsafe-math-optimizations', '-funsafe-loop-optimizations', '-fomit-frame-pointer', '-DNDEBUG'],
			'clang':   ['-Ofast', '-march=native', '-DNDEBUG'],
			'default': ['-O3', '-DNDEBUG']
		},
		'release': {
			'msvc':    ['/O2', '/DNDEBUG', '/MT'],
			'default': ['-O3', '-DNDEBUG']
		},
		'debug': {
			'msvc':    ['/O1', '/D_DEBUG', '/MTd'],
			'gcc':     ['-Og', '-D_DEBUG'],
			'default': ['-O1', '-D_DEBUG']
		},
		'sanitize': {
			'msvc':    ['/Od', '/RTC1', '/MT', '/DNDEBUG'],
			'gcc':     ['-Og', '-fsanitize=undefined', '-fsanitize=address', '-DNDEBUG'],
			'clang':   ['-O0', '-fsanitize=undefined', '-fsanitize=address', '-DNDEBUG'],
			'default': ['-O0', '-DNDEBUG']
		},
		'nooptimize': {
			'msvc':    ['/Od', '/MT', '/DNDEBUG'],
			'default': ['-O0', '-DNDEBUG']
		}
	}

	compiler_optional_flags = [
		'-fdiagnostics-color=always',
		'-Werror=return-type',
		'-Werror=parentheses',
		'-Werror=vla',
		'-Werror=tautological-compare',
		'-Werror=duplicated-cond',
		'-Werror=duplicated-branches', # BEWARE: buggy
		'-Werror=bool-compare',
		'-Werror=bool-operation',
		'-Wdouble-promotion',
		'-Wstrict-aliasing',
	]

	c_compiler_optional_flags = [
		'-Werror=implicit-function-declaration',
		'-Werror=int-conversion',
		'-Werror=implicit-int',
		'-Werror=strict-prototypes',
		'-Werror=old-style-declaration',
		'-Werror=old-style-definition',
		'-Werror=declaration-after-statement'
	]

	linkflags = conf.get_flags_by_type(linker_flags, conf.options.BUILD_TYPE, conf.env.COMPILER_CC)
	cflags    = conf.get_flags_by_type(compiler_c_cxx_flags, conf.options.BUILD_TYPE, conf.env.COMPILER_CC)

	# Here we don't differentiate C or C++ flags
	if conf.options.LTO:
		lto_cflags = {
			'msvc':  ['/GL'],
			'gcc':   ['-flto'],
			'clang': ['-flto']
		}

		lto_linkflags = {
			'msvc':  ['/LTCG'],
			'gcc':   ['-flto'],
			'clang': ['-flto']
		}
		cflags    += conf.get_flags_by_compiler(lto_cflags, conf.env.COMPILER_CC)
		linkflags += conf.get_flags_by_compiler(lto_linkflags, conf.env.COMPILER_CC)

	if conf.options.POLLY:
		polly_cflags = {
			'gcc':   ['-fgraphite-identity'],
			'clang': ['-mllvm', '-polly']
			# msvc sosat :(
		}

		cflags   += conf.get_flags_by_compiler(polly_cflags, conf.env.COMPILER_CC)

	# And here C++ flags starts to be treated separately
	cxxflags = list(cflags)
	if conf.env.COMPILER_CC != 'msvc':
		conf.check_cc(cflags=cflags, msg= 'Checking for required C flags')
		conf.check_cxx(cxxflags=cflags, msg= 'Checking for required C++ flags')

		cxxflags += conf.filter_cxxflags(compiler_optional_flags, cflags)
		cflags += conf.filter_cflags(compiler_optional_flags + c_compiler_optional_flags, cflags)

	conf.env.append_unique('CFLAGS', cflags)
	conf.env.append_unique('CXXFLAGS', cxxflags)
	conf.env.append_unique('LINKFLAGS', linkflags)

	# check if we can use C99 tgmath
	if conf.check_cc(header_name='tgmath.h', mandatory=False):
		tgmath_usable = conf.check_cc(fragment='''#include<tgmath.h>
			int main(void){ return (int)sin(2.0f); }''',
			msg='Checking if tgmath.h is usable', mandatory=False)
		conf.define_cond('HAVE_TGMATH_H', tgmath_usable)
	else:
		conf.undefine('HAVE_TGMATH_H')

	conf.env.DEDICATED     = conf.options.DEDICATED
	# we don't need game launcher on dedicated
	conf.env.SINGLE_BINARY = conf.options.SINGLE_BINARY or conf.env.DEDICATED
	if conf.env.DEST_OS == 'linux':
		conf.check_cc( lib='dl' )

	if conf.env.DEST_OS != 'win32':
		if not conf.env.LIB_M: # HACK: already added in xcompile!
			conf.check_cc( lib='m' )

		if conf.env.DEST_OS != 'android': # Android has pthread directly in libc
			conf.check_cc( lib='pthread' )
	else:
		# Common Win32 libraries
		# Don't check them more than once, to save time
		# Usually, they are always available
		# but we need them in uselib
		a = map(lambda x: {
			# 'features': 'c',
			# 'message': '...' + x,
			'lib': x,
			# 'uselib_store': x.upper(),
			# 'global_define': False,
		}, [
			'user32',
			'shell32',
			'gdi32',
			'advapi32',
			'dbghelp',
			'psapi',
			'ws2_32'
		])

		for i in a:
			conf.check_cc(**i)

		# conf.multicheck(*a, run_all_tests = True, mandatory = True)

	# indicate if we are packaging for Linux/BSD
	if(not conf.options.WIN_INSTALL and
		conf.env.DEST_OS not in ['win32', 'darwin', 'android']):
		conf.env.LIBDIR = conf.env.BINDIR = '${PREFIX}/lib/xash3d'
	else:
		conf.env.LIBDIR = conf.env.BINDIR = conf.env.PREFIX

	conf.define('XASH_BUILD_COMMIT', conf.env.GIT_VERSION if conf.env.GIT_VERSION else 'notset')
	conf.define('AFTERBURNER_ENGINE', 1)

	for i in SUBDIRS:
		if conf.env.SINGLE_BINARY and i.singlebin:
			continue

		if conf.env.DEST_OS == 'android' and i.singlebin:
			continue

		if conf.env.DEDICATED and i.dedicated:
			continue

		conf.add_subproject(i.name)

def build(bld):
	for i in SUBDIRS:
		if bld.env.SINGLE_BINARY and i.singlebin:
			continue

		if bld.env.DEST_OS == 'android' and i.singlebin:
			continue

		if bld.env.DEDICATED and i.dedicated:
			continue

		bld.add_subproject(i.name)
