AUTOMAKE_OPTIONS = gnu
lib_LTLIBRARIES = libghthash.la

libghthash_la_SOURCES = hash_table.c crc.c
include_HEADERS = ght_hash_table.h
noinst_HEADERS = crc.h

libghthash_la_LDFLAGS = -version-info $(LT_CURRENT):$(LT_REVISION):$(LT_AGE)

