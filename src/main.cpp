#include <iostream>
#include <sstream>
#include <fstream>
#include "rose.h"
#include <vector>
#include "Program.h"
#include "Common.h"

using namespace std;

void analyze(int argc, char **argv, string ir_file_path);

string get_ir_content(Program program);

int main(int argc, char **argv) {
    common::init_operator_str_map();

    string ir_file_path = argv[argc-1];

    analyze(2, argv, ir_file_path);

    return 0;
}

void analyze(int argc, char **argv, string ir_file_path) {
    SgProject* project = frontend(argc, argv);
    RoseAst ast(project);
    vector<SgNode*> nodes = project -> get_fileList_ptr() -> get_traversalSuccessorContainer();
    for (auto n : nodes) {
        if (auto src = dynamic_cast<SgSourceFile *>(n)) {
            Program program(src->getFileName(), src->get_globalScope());
            program.analyze();

            ofstream file_stream;
            file_stream.open(ir_file_path, ios::trunc);
            file_stream << get_ir_content(program);
            file_stream.close();
        }
    }
}

string get_ir_content(Program program) {
    stringstream ir_stream;
    ir_stream << program.get_ir_content() << endl;
    return ir_stream.str();
}
