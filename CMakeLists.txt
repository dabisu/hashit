CMAKE_MINIMUM_REQUIRED (VERSION 2.4 FATAL_ERROR)
IF (COMMAND CMAKE_POLICY)	
	CMAKE_POLICY(SET CMP0005 OLD)
ENDIF (COMMAND CMAKE_POLICY)	
PROJECT (hashit)
SET (AUTHOR "David Gómez Espinosa" INTERNAL "Author")
SET (VERSION "0.9.7")

INCLUDE (FindPkgConfig)

IF (NOT PKG_CONFIG_FOUND)
	MESSAGE (FATAL_ERROR "pkg-config not found...")
ENDIF (NOT PKG_CONFIG_FOUND)

ADD_DEFINITIONS (-DDATADIR=\\\"${CMAKE_INSTALL_PREFIX}/share\\\")

ADD_LIBRARY (hashit SHARED src/hashit.c src/hfunctions.c)
ADD_DEPENDENCIES (hashit src/hashit.c src/hfunctions.c src/hashit.h src/hfunctions.h)

FILE(WRITE libhashit.pc "prefix=${CMAKE_INSTALL_PREFIX}\n")	
FILE(APPEND libhashit.pc "libdir=${CMAKE_INSTALL_PREFIX}/lib${LIB_SUFFIX}\n")	
FILE(APPEND libhashit.pc "includedir=${CMAKE_INSTALL_PREFIX}/include\n\n")	
FILE(APPEND libhashit.pc "Name: libhashit\n")	
FILE(APPEND libhashit.pc "Description: Generic hash library\n")	
FILE(APPEND libhashit.pc "Version: ${VERSION}\n")	
FILE(APPEND libhashit.pc "Libs: -L${CMAKE_INSTALL_PREFIX}/lib${LIB_SUFFIX} -lhashit\n")
FILE(APPEND libhashit.pc "Cflags: -I${CMAKE_INSTALL_PREFIX}/include\n")

INSTALL (TARGETS hashit LIBRARY DESTINATION lib${LIB_SUFFIX})	
INSTALL (FILES src/hashit.h DESTINATION include)
INSTALL (FILES libhashit.pc DESTINATION lib${LIB_SUFFIX}/pkg-config)

SET_TARGET_PROPERTIES (hashit PROPERTIES VERSION 1.0 SOVERSION 1)	
SET (LIBRARY_OUTPUT_PATH src)

ADD_CUSTOM_TARGET (distclean
	rm -f Makefile cmake_install.cmake CMakeCache.txt install_manifest.txt libhashit.pc
	COMMAND rm -rf CMakeFiles)
