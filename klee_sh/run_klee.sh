pwd=$PWD

klee_path="/home/unix-lc/klee-float/"
klee_include=klee_path"include"
klee_bin="/home/unix-lc/klee-float/cmake-build-debug/bin"

echo "pwd: "$pwd
echo "klee_path: "$klee_path
echo "klee_include: "$klee_include

clang -I $klee_include -emit-llvm -c -g ./test.cpp

cd $klee_bin

./klee -solver-backend=z3 --use-query-log=all:kquery -write-kqueries $pwd/test.bc

cp expression.txt $pwd/expression.txt