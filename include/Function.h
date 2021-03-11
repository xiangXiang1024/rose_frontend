//
// Created by root on 2021/3/9.
//

#include "CodeSegment.h"

#ifndef FRONTEND_FUNCTION_H
#define FRONTEND_FUNCTION_H

using namespace std;

class Function : public Code {
public:
    string func_name;
    CodeSegment segment;
    vector<SgStatement*> statement_list;
    SgFunctionDeclaration* function_declaration;

    Function(SgFunctionDeclaration* _function_declaration) : function_declaration(_function_declaration) {
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

        segment = CodeSegment(statement_list);
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


        cout << "**** func: " << func_name << " ****" << endl;
        segment.print();
    };

    string get_ir_content(int tab_num) {
        string blank = common::get_line_start_blank(tab_num);
        stringstream ir_stream;
        ir_stream << blank << "function: " << func_name << " ir content" << endl;
        ir_stream << blank << "{" << endl;
        ir_stream << segment.get_ir_content(tab_num+1) << endl;
        ir_stream << blank << "}";
        return ir_stream.str();
    }

private:
    void generate_input_list();

    void generate_statement_list();

    void generate_output_list();

    void generate_line_info();
};

#endif //FRONTEND_FUNCTION_H

