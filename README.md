# BioInfo Project
The goal of this project is to implement the Burrows-Wheeler Transform: https://en.wikipedia.org/wiki/Burrows%E2%80%93Wheeler_transform  
In a nutshell it is useful while dealing with long ADN sequences, it is a compromise between space taken by the sequence and time used when we are trying to search patterns matching.

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
