CMAKE_MINIMUM_REQUIRED (VERSION 2.4 FATAL_ERROR)
PROJECT (hashit)
SET (AUTHOR "David Gómez Espinosa" INTERNAL "Author")
SET (VERSION "0.9.5")
MESSAGE("${VERSION}")


INCLUDE (FindPkgConfig)

IF (NOT PKG_CONFIG_FOUND)
	MESSAGE (FATAL_ERROR "pkg-config not found...")
ENDIF (NOT PKG_CONFIG_FOUND)

ADD_DEFINITIONS (-DVERSION=\\\"${VERSION}\\\")
ADD_DEFINITIONS (-DDATADIR=\\\"${CMAKE_INSTALL_PREFIX}/share\\\")

ADD_LIBRARY (libhashit SHARED src/hashit.c src/hfunctions.c)
ADD_DEPENDENCIES (libhashit src/hashit.c src/hfunctions.c src/hashit.h src/hfunctions.h)

#INSTALL (TARGETS sakura
#		 RUNTIME DESTINATION bin)	

SET_DIRECTORY_PROPERTIES (PROPERTIES
	ADDITIONAL_MAKE_CLEAN_FILES "${MO_FILES};po/sakura.pot")

ADD_CUSTOM_TARGET (distclean
	rm -f Makefile cmake_install.cmake CMakeCache.txt install_manifest.txt
	COMMAND rm -rf CMakeFiles po/CMakeFiles)
