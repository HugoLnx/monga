#!/bin/bash
echo "Running lexical Tests"
python scripts/lexical.py test_all

echo
echo "Running syntax Tests"
python scripts/syntax.py test_all

echo
echo "Running ast-tree Tests"
python scripts/ast-tree.py test_all

echo
echo "Running ids and types Tests"
python scripts/ids-and-types.py test_all

echo
echo "Running monga Tests"
python scripts/monga.py test_all
