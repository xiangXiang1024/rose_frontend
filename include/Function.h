//
// Created by root on 2021/3/9.
//

#include "CodeSegment.h"
#include "../cjson_lib/CJsonObject.hpp"

#ifndef FRONTEND_FUNCTION_H
#define FRONTEND_FUNCTION_H

using namespace std;

class Function : public Code {
public:
    string func_name;
    CodeSegment segment;
    CodeSegment recursion_segment;
    bool is_recursive = false;
    vector<SgStatement*> statement_list;
    SgFunctionDeclaration* function_declaration;
    map<string,string>* func_call_map;

    Function(SgFunctionDeclaration* _function_declaration, map<string,string>* _map_func_call_map) : function_declaration(_function_declaration),func_call_map(_map_func_call_map) {
        func_name = function_declaration -> get_name().getString();
    }

    void analyze() {
//        cout << "analyze function " << func_name << endl;

        generate_input_list();
        /*cout << "inputs: ";
        for(Variable v : input_list) {
            cout << v.variable_name << "(" << v.type -> unparseToCompleteString() << ")    ";
        }
        cout << endl << endl;*/

        generate_statement_list();
        /*cout << "statement:" << endl;
        for (SgStatement* s : statement_list) {
            cout << s->unparseToString() << "\t\t|\t\t" << s->class_name() << endl;
        }
        cout << endl;*/

        segment = CodeSegment(statement_list, this, func_name, func_call_map);
        segment.input_list = input_list;
        segment.analyze();

        generate_output_list();
        /*cout << endl << "outputs: ";
        for(Variable v : output_list) {
            cout << v.variable_name << "(" << v.type -> unparseToCompleteString() << ")    ";
        }
        cout << endl << endl;
    */
        generate_line_info();
        /*cout << endl << "lines: ";
        for(int l : line_info) {
            cout << l << "    ";
        }
        cout << endl << endl;*/
    };

    string get_ir_content(int tab_num) {
        /*string blank = common::get_line_start_blank(tab_num);
        stringstream ir_stream;
        ir_stream << blank << "function: " << func_name << " ir content" << endl;
        ir_stream << blank << "{" << endl;
        ir_stream << segment.get_ir_content(tab_num+1) << endl;
        ir_stream << blank << "}";*/
//        return ir_stream.str();

        neb::CJsonObject function_json("");
        function_json.Add("program_name", "TODO program name");
        function_json.Add("function_name", func_name);

        function_json.AddEmptySubObject("variables");
        map<string, string> variable_map;
        for(Variable* v : input_list) {
            variable_map[v -> variable_name] = v -> type -> unparseToString();
        }
        for(Variable* v : output_list) {
            variable_map[v -> variable_name] = v -> type -> unparseToString();
        }
        auto iter = variable_map.begin();
        while(iter != variable_map.end()) {
            function_json["variables"].Add(iter -> first, iter -> second);
            iter++;
        }

        function_json.AddEmptySubArray("input_variables");
        for(Variable* v : input_list) {
            function_json["input_variables"].Add(v -> variable_name);
        }

        // TODO
        function_json.AddEmptySubObject("return");

        return function_json.ToFormattedString();
    }

private:
    void generate_input_list();

    void generate_statement_list();

    void generate_output_list();

    void generate_line_info();
};

#endif //FRONTEND_FUNCTION_H

