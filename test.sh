#!/bin/bash

test_passed=0
test_failed=0

rm -rf build

mkdir build
cd build
cmake ..
make

echo -ne "\n\n-------- TESTSUITE ---------\n\n"

./bw-build ../data/cmv.fasta cmv.fasta.idx 6
diff cmv.fasta.idx ../data/cmv.fasta.idx
if [ $(echo $?) -eq 0 ]
then
    test_passed=$((test_passed+1))
    echo "cmv.fasta to cmv.fasta.idx passed"
else
    test_failed=$((test_failed+1))
    echo "cmv.fasta to cmv.fasta.idx failed"
fi

./bw-build --compress ../data/cmv.fasta cmv.fasta.idxc 5
diff cmv.fasta.idxc ../data/cmv.fasta.idxc
if [ $(echo $?) -eq 0 ]
then
    test_passed=$((test_passed+1))
    echo "cmv.fasta to cmv.fasta.idxc passed"
else
    test_failed=$((test_failed+1))
    echo "cmv.fasta to cmv.fasta.idxc failed"
fi

./bw-build ../data/gfp.fasta gfp.fasta.idx 1
diff gfp.fasta.idx ../data/gfp.fasta.idx
if [ $(echo $?) -eq 0 ]
then
    test_passed=$((test_passed+1))
    echo "gfp.fasta to gfp.fasta.idx passed"
else
    test_failed=$((test_failed+1))
    echo "gfp.fasta to gfp.fasta.idx failed"
fi

./bw-build --compress ../data/gfp.fasta gfp.fasta.idxc 33
diff gfp.fasta.idxc ../data/gfp.fasta.idxc
if [ $(echo $?) -eq 0 ]
then
    test_passed=$((test_passed+1))
    echo "gfp.fasta to gfp.fasta.idxc passed"
else
    test_failed=$((test_failed+1))
    echo "gfp.fasta to gfp.fasta.idxc failed"
fi

./bw-build ../data/calreticulin.fasta calreticulin.fasta.idx 15
diff calreticulin.fasta.idx ../data/calreticulin.fasta.idx
if [ $(echo $?) -eq 0 ]
then
    test_passed=$((test_passed+1))
    echo "calreticulin.fasta to calreticulin.fasta.idx passed"
else
    test_failed=$((test_failed+1))
    echo "calreticulin.fasta to calreticulin.fasta.idx failed"
fi

./bw-build --compress ../data/calreticulin.fasta calreticulin.fasta.idxc 15
diff calreticulin.fasta.idxc ../data/calreticulin.fasta.idxc
if [ $(echo $?) -eq 0 ]
then
    test_passed=$((test_passed+1))
    echo "calreticulin.fasta to calreticulin.fasta.idxc passed"
else
    test_failed=$((test_failed+1))
    echo "calreticulin.fasta to calreticulin.fasta.idxc failed"
fi

./bw-build -y ../data/rtn4.fasta rtn4.fasta.idx 72
diff rtn4.fasta.idx ../data/rtn4.fasta.idx
if [ $(echo $?) -eq 0 ]
then
    test_passed=$((test_passed+1))
    echo "rtn4.fasta to rtn4.fasta.idx passed"
else
    test_failed=$((test_failed+1))
    echo "rtn4.fasta to rtn4.fasta.idx failed"
fi

./bw-build --compress ../data/rtn4.fasta rtn4.fasta.idxc 18
diff rtn4.fasta.idxc ../data/rtn4.fasta.idxc
if [ $(echo $?) -eq 0 ]
then
    test_passed=$((test_passed+1))
    echo "rtn4.fasta to rtn4.fasta.idxc passed"
else
    test_failed=$((test_failed+1))
    echo "rtn4.fasta to rtn4.fasta.idxc failed"
fi

echo -ne "\n\n"

echo "nb test passed: $test_passed"
echo "nb test failed: $test_failed"

echo -ne "\n\n\n"
