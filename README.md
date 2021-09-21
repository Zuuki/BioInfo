# BioInfo
Bio Info Project

## How to build this project
mkdir build
cd build
cmake ..
make

You have now access to 2 exectuables:

bw-build
bw-search

## bw-build arguments

### mandatory:
    infile       : path to the input file
    outfile      : path to the output file
    f            : frequency for the creation of the index

### optional:
    --compress   : the BWT will be returned compressed

## bw-search arguments

### mandatory:
    infile       : path to the input file
    q            : sequence you wanna search

### optional:
    --count-only : print only the number of matching
