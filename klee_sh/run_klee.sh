pwd=$PWD

read -p "Please enter the cpp file:"
cpp_file=${REPLY}

rewriter_path=$PWD
reader_path=$PWD

cd $rewriter_path

./generate_main ${cpp_file}

cpp_file=${cpp_file/.cpp/1.cpp}

cd $pwd

#klee路径，请修改
klee_path="/home/unix-lc/klee-float"
klee_include="/home/unix-lc/klee-float/include"
klee_bin="/home/unix-lc/klee-float/cmake-build-debug/bin"

echo "pwd: "$pwd
echo "klee_path: "$klee_path
echo "klee_include: "$klee_include

clang -I $klee_include -emit-llvm -c -g ./${cpp_file}

cd $klee_bin

array=(${cpp_file//\// })

./klee -solver-backend=z3 --use-query-log=all:kquery -write-kqueries $pwd/${array[-1]/.cpp/.bc}

cp expression.txt $pwd/expression.txt

cd $reader_path

./expr_reader $pwd/expression.txt
