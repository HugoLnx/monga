#! /bin/bash
# To run use: ./run tests/input.in

ROOT=/vagrant/monga

BUILD=$ROOT/build
TMP_SRC=$BUILD
TMP_COMPILED=$BUILD
DIST=$BUILD/dist

SRC=$ROOT/src
TESTS=$ROOT/tests

# Preparing
sudo rm -rf $BUILD/*
mkdir -p $TMP_SRC
mkdir -p $TMP_COMPILED
mkdir -p $DIST

# Compiling
lex --header-file=$TMP_SRC/lex.yy.h -o $TMP_SRC/lex.yy.c $SRC/monga.lex
gcc -ll $TMP_SRC/lex.yy.c -o $TMP_COMPILED/lex.yy.o -c
gcc $SRC/main.c -o $TMP_COMPILED/main.o -c
gcc $TMP_COMPILED/main.o $TMP_COMPILED/lex.yy.o -o $DIST/main

# Executing
$DIST/main < $1
