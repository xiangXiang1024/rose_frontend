//
// Created by root on 2021/3/5.
//

#ifndef FRONTEND_PROGRAM_H
#define FRONTEND_PROGRAM_H

#endif //FRONTEND_PROGRAM_H

#include "Function.h"
#include "rose.h"
#include <vector>

using namespace std;

class Program : public Code {
public:
    string file_path;
    string file_name;
    vector<Function> function_list;
    SgGlobal *global;

    void analyze() {
        cout << "analyze cpp source file: " <<  file_path << endl << endl;

        vector<SgNode*> sgnode_list = global -> get_traversalSuccessorContainer();
        for(auto sgnode : sgnode_list) {
            if(nullptr == sgnode) {
                continue;
            }
            if(dynamic_cast<SgLocatedNode *>(sgnode) -> getFilenameString() != file_path) {
                continue;
            }
            if(auto* func_declaration = dynamic_cast<SgFunctionDeclaration*>(sgnode)) {
                Function function(func_declaration);
                function_list.push_back(function);
                function.analyze();
            }
        }
    }

    Program(const string &_name, SgGlobal *_global) : file_path(_name), global(_global) {
        // TODO file_name
    }

    string get_ir_content(int tab_num) {
        string blank = common::get_line_start_blank(tab_num);
        stringstream ir_stream;
        ir_stream << blank << "program ir content" << endl;
        for(Function f : function_list) {
            ir_stream << blank << "{" << endl;
            ir_stream << f.get_ir_content(tab_num+1) << endl;
            ir_stream << blank << "}";
        }
        return ir_stream.str();
    }
};