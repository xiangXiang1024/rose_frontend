#!/bin/bash
output_dir=$PWD"/output"
if [ ! -d ${output_dir} ]; then
    echo "output_dir(${output_dir}) is not a directory or doesn't exist"
    exit 1;
else
    if [ ${output_dir: -1} != "/" ]; then
        output_dir=${output_dir}"/"
    fi
    rm -rf ${output_dir}
    mkdir ${output_dir}
fi

read -p "Please enter the directory path:"
dir_path=${REPLY}

is_same_path() {
    path_1=$1
    path_2=$2

    if [ ${path_1} = ${path_2} -o ${path_1}"/" = ${path_2} -o ${path_1} = ${path_2}"/" ] ; then
        return 0
    fi

    return 1
}

analyze_cpp_file() {
    analyze_path=$1

    cpp_file=${analyze_path#${dir_path}}
    ir_file=${cpp_file%.*}".ir"

    if [ ${ir_file:0:1} = "/" ] ; then
        ir_file=${ir_file#/}
    fi

    ir_path=${output_dir}${ir_file}

    mkdir -p ${ir_path%/*}
    echo "" > ${ir_path}
#    touch ${ir_path}
    ./cmake-build-debug/frontend ${cpp_path} ${ir_path}
}

analyze() {
    analyze_dir=$1

    if [ ${analyze_dir: -1} != "/" ]; then
        analyze_dir=${analyze_dir}"/"
    fi

    if [ ! -d ${analyze_dir} ] ; then
        echo "${analyze_dir} is not a directory or doesn't exist"
        exit 1
    else
        for analyze_path in `find ${analyze_dir}`
        do
            if [ -d ${analyze_path} ] ; then
                is_same_path ${analyze_path} ${analyze_dir}
                if [ $? -gt 0 ] ; then
                    analyze ${analyze_path}
                fi
            elif [ ${analyze_path: -4} == ".cpp" ]; then
                analyze_cpp_file ${analyze_path}
            fi
        done
    fi
}

analyze ${dir_path}

if [ $? -eq 0 ]; then
    echo "The result is stored in the directory: ${output_dir}"
fi