CMAKE_MINIMUM_REQUIRED (VERSION 2.4 FATAL_ERROR)
PROJECT (hashit)
SET (AUTHOR "David Gómez Espinosa" INTERNAL "Author")
SET (VERSION "0.9.5")

INCLUDE (FindPkgConfig)

IF (NOT PKG_CONFIG_FOUND)
	MESSAGE (FATAL_ERROR "pkg-config not found...")
ENDIF (NOT PKG_CONFIG_FOUND)

ADD_DEFINITIONS (-DDATADIR=\\\"${CMAKE_INSTALL_PREFIX}/share\\\")

ADD_LIBRARY (hashit SHARED src/hashit.c src/hfunctions.c)
ADD_DEPENDENCIES (hashit src/hashit.c src/hfunctions.c src/hashit.h src/hfunctions.h)

# CMake bug, it installs library target but doesn't
# create the symbolic links
INSTALL (TARGETS hashit LIBRARY DESTINATION lib)	
INSTALL (FILES src/hashit.h DESTINATION include)
# TODO: Generate pkg-config file (CMake supports it?)
#INSTALL (FILES libhashit.pc DESTINATION lib/pkg-config)

SET_TARGET_PROPERTIES (hashit PROPERTIES VERSION 1.0 SOVERSION 1)	
SET (LIBRARY_OUTPUT_PATH src)

ADD_CUSTOM_TARGET (distclean
	rm -f Makefile cmake_install.cmake CMakeCache.txt install_manifest.txt
	COMMAND rm -rf CMakeFiles)
