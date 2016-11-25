SUBDIRS = . test
ACLOCAL_AMFLAGS = -I m4

lib_LTLIBRARIES = libquackstandardlib.la

libquackstandardlibdir = $(includedir)
nobase_libquackstandardlib_HEADERS = \
    CommonInclude.h \
    Exception.h \
    pool/AutoMemPool.h \
    pool/InflatePool.h \
    pool/MemPool.h

libquackstandardlib_la_SOURCES = \
    pool/AutoMemPool.cpp \
    pool/InflatePool.cpp
