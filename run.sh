# To run use: ./run tests/input.in

ROOT=/vagrant/monga

BUILD=$ROOT/build
TMP_SRC=$BUILD/src
TMP_COMPILED=$BUILD/compiled

SRC=$ROOT/src
TESTS=$ROOT/tests

# Preparing
sudo rm -rf $BUILD/*
mkdir -p $TMP_SRC
mkdir -p $TMP_COMPILED

# Compiling
lex --header-file=$TMP_SRC/lex.yy.h -o $TMP_SRC/lex.yy.c $SRC/monga.lex
gcc -ll $TMP_SRC/lex.yy.c -o $TMP_COMPILED/lex.yy.o -c
gcc $SRC/main.c -o $TMP_COMPILED/main.o -c
gcc $TMP_COMPILED/main.o $TMP_COMPILED/lex.yy.o -o $BUILD/main

# Executing
$BUILD/main < $1
