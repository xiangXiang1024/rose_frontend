//
// Created by root on 2021/3/5.
//

#ifndef FRONTEND_PROGRAM_H
#define FRONTEND_PROGRAM_H

#endif //FRONTEND_PROGRAM_H

#include "Function.h"
#include "rose.h"
#include <vector>
#include "../cjson_lib/CJsonObject.hpp"

using namespace std;

class Program : public Code {
public:
    string file_path;
    string file_name;
    vector<Function*> function_list;
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
                Function* function = new Function(func_declaration);
                function_list.push_back(function);
                function -> analyze();
            }
        }
    }

    Program(const string &_name, SgGlobal *_global) : file_path(_name), global(_global) {
        // TODO file_name
    }

    string get_ir_content(int tab_num) {
//        neb::CJsonObject program_json("");
//        program_json.Add("program_name", file_path);

        stringstream ir_stream;
        for(int i = 0 ; i < function_list.size() ; i++) {
            Function* f = function_list.at(i);
            ir_stream << f -> get_ir_content(tab_num+1);
            if(i != function_list.size() - 1) {
                ir_stream << "," << endl;
            }
        }

        cout << "---- ir content ----" << endl;
        cout << ir_stream.str() << endl;

        return ir_stream.str();

        /*cout << endl << "program json: " << endl;
        cout << program_json.ToFormattedString() << endl;
        return program_json.ToFormattedString();*/

        /*string blank = common::get_line_start_blank(tab_num);
        stringstream ir_stream;
        ir_stream << blank << "program ir content" << endl;
        for(Function f : function_list) {
            ir_stream << blank << "{" << endl;
            ir_stream << f.get_ir_content(tab_num+1) << endl;
            ir_stream << blank << "}";
        }
        return ir_stream.str();*/
    }
};