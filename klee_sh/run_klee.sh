pwd=$PWD

#klee路径，请修改
klee_path="/home/unix-lc/klee-float"
klee_include="/home/unix-lc/klee-float/include"
klee_bin="/home/unix-lc/klee-float/cmake-build-debug/bin"

echo "pwd: "$pwd
echo "klee_path: "$klee_path
echo "klee_include: "$klee_include

clang -I $klee_include -emit-llvm -c -g ./segment.cpp

cd $klee_bin

./klee -solver-backend=z3 --use-query-log=all:kquery -write-kqueries $pwd/segment.bc

cp expression.txt $pwd/expression.txt

