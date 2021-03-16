//
// Created by root on 2021/3/5.
//

#ifndef FRONTEND_CODE_H
#define FRONTEND_CODE_H

#endif //FRONTEND_CODE_H

#include <string>
#include "rose.h"
#include <vector>
#include "ArrayVariable.h"
#include <sstream>
#include "Common.h"

class Code {
public:
    vector<int> line_info;
    vector<Variable*> input_list;
    vector<Variable*> output_list;
    vector<Variable*> intermediate_list;

    virtual void analyze() {};

    void to_ir_content(stringstream* ir_output);

    void add_input(Variable* v) {
//        cout << "in func add_input: " << endl;
        if(v == nullptr) {
//            cout << "v == nullptr" << endl;
            return;
        }
//        v -> print();
        bool has_variable = false;
        for(Variable* input : input_list) {
//            cout << "input: ";
            /*if(input == nullptr) {
                cout << "input == nullptr" << endl;
                continue;
            }*/
//            input -> print();
            if(v -> variable_name == input -> variable_name) {
                has_variable = true;
                break;
            }
        }
        if(!has_variable) {
            input_list.push_back(v);
            intermediate_list.push_back(v);
        }
    }

    void add_input(vector<Variable*> variable_list) {
        for(Variable* v : variable_list) {
            add_input(v);
        }
    }

    void add_output(Variable* v) {
//        cout << "add_output: " << endl;
//        v.print();

        bool has_variable = false;
        for(int i = 0 ; i < output_list.size() ; i++) {
            Variable* output = output_list.at(i);
            if(v -> variable_name == output -> variable_name) {
                output_list.at(i) -> set_expression(v -> get_expression());
                return;
            }
        }
        output_list.push_back(v);

        /*if(!has_variable) {
            output_list.push_back(v);
        }*/
    }

    void add_output(vector<Variable*> variable_list) {
        for(Variable* v : variable_list) {
            add_output(v);
        }
    }

    void add_intermediate(Variable* v) {
        int i = -1;
        for(Variable* intermediate_variable : intermediate_list) {
            i++;
            if(v -> variable_name == intermediate_variable -> variable_name) {
                intermediate_list.erase(intermediate_list.begin()+i);
                break;
            }
        }
        intermediate_list.push_back(v);

        /*cout << "after add intermediate_list:" << endl;
        for(Variable intermediate_variable : intermediate_list) {
            intermediate_variable.print();
        }
        cout << endl;*/
    }

    void add_intermediate(vector<Variable*> variable_list) {
        for(Variable* v : variable_list) {
            add_intermediate(v);
        }
    }

    void set_intermediate_variable(Variable* v) {
        add_intermediate(v);
    }

    Variable* get_intermediate_variable(string name) {
        if(&intermediate_list == nullptr || intermediate_list.size() == 0) {
            Variable* v = new Variable();
            return v;
        }
        for(Variable* variable : intermediate_list) {
            if(variable -> variable_name == name) {
                return variable;
            }
        }
        Variable* v = new Variable();
        return v;
    }

    SgExpression* handle_binary_op(SgBinaryOp* binary_op);

    SgExpression* handle_unary_op(SgUnaryOp* binary_op);

    SgExpression* handle_expression(SgExpression* expression);

    virtual string get_ir_content(int tab_num) {
        return "TODO generate ir content";
    }
};