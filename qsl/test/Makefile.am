SUBDIRS = .
ACLOCAL_AMFLAGS = -I m4

TESTS = mem_pool_test

check_LTLIBRARIES = libqsltestmain.la libgmockgtest.la
check_PROGRAMS = $(TESTS)


# libgmockgtest.la is gmock + gtest
libgmockgtest_la_CPPFLAGS = -Igoogletest-release-1.8.0/googletest -Igoogletest-release-1.8.0/googlemock -Igoogletest-release-1.8.0/googletest/include -Igoogletest-release-1.8.0/googlemock/include -lglog
libgmockgtest_la_SOURCES = \
	googletest-release-1.8.0/googletest/src/gtest-all.cc \
	googletest-release-1.8.0/googlemock/src/gmock-all.cc

libqsltestmain_la_CPPFLAGS = $(AM_CPPFLAGS)
libqsltestmain_la_SOURCES = TestMain.cpp
libqsltestmain_la_LIBADD = $(top_builddir)/libquackstandardlib.la libgmockgtest.la

mem_pool_test_SOURCES = MemPoolTest.cpp
mem_pool_test_LDADD = libqsltestmain.la
