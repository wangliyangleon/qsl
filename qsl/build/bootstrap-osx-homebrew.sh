#!/bin/bash -x
# The only prerequisite should be homebrew. If something doesn't work out of
# the box with just homebrew, let's fix it.

# fail fast
set -e

BASE_DIR="$(cd "$(dirname -- "$0")"/.. ; pwd)"  # qsl/qsl
cd "$BASE_DIR"

brewget() {
    brew install $@ || brew upgrade $@
}

# tool dependencies: autotools and scons (for double-conversion)
brewget autoconf automake libtool

# dependencies
brewget glog gflags
test -d GSL && mv GSL GSL_`date +%s`
git clone https://github.com/Microsoft/GSL.git GSL

#autoreconf -i
#./configure

pushd test
test -e gtest-1.7.0.zip || {
    curl -O https://googletest.googlecode.com/files/gtest-1.7.0.zip
    unzip gtest-1.7.0.zip
}
popd
