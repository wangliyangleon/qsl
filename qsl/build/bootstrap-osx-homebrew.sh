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
# get gitignore
git clone https://github.com/github/gitignore.git gitignore
cat qsl.gitignore > .gitignore
cat gitignore/C++.gitignore >> .gitignore
cat gitignore/Autotools.gitignore >> .gitignore

# install GSL
test -d GSL && mv GSL GSL_`date +%s`
git clone https://github.com/Microsoft/GSL.git GSL
pushd GSL
mkdir build
cd build
cmake -G "Unix Makefiles" ../
cmake --build . --config Debug
ctest -C Debug
test $? -eq 0 || {
  echo "BE CAREFUL!!! GSL is not fully supported in your env!"
}
popd

autoreconf -i
./configure

pushd test
test -e gtest-1.8.0.zip || {
    wget https://github.com/google/googletest/archive/release-1.8.0.zip -O gtest-1.8.0.zip
    unzip gtest-1.8.0.zip
}
popd
