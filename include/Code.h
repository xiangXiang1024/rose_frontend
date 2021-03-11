//
// Created by root on 2021/3/5.
//

#ifndef FRONTEND_CODE_H
#define FRONTEND_CODE_H

#endif //FRONTEND_CODE_H

#include <string>
#include "rose.h"
#include <vector>
#include "Variable.h"
#include <sstream>
#include "Common.h"

class Code {
public:
    vector<int> line_info;
    vector<Variable> input_list;
    vector<Variable> output_list;
    vector<Variable> intermediate_list;

    virtual void analyze() {};

    void to_ir_content(stringstream* ir_output);

    void add_input(Variable v) {
        bool has_variable = false;
        for(Variable input : input_list) {
            if(v.variable_name == input.variable_name) {
                has_variable = true;
                break;
            }
        }
        if(!has_variable) {
            input_list.push_back(v);
            intermediate_list.push_back(v);
        }
    }

    void add_output(Variable v) {
//        cout << "add_output: " << endl;
//        v.print();

        bool has_variable = false;
        for(Variable output : output_list) {
            if(v.variable_name == output.variable_name) {
                return;
            }
        }
        output_list.push_back(v);

        /*if(!has_variable) {
            output_list.push_back(v);
        }*/
    }

    void add_intermediate(Variable v) {
        int i = -1;
        for(Variable intermediate_variable : intermediate_list) {
            i++;
            if(v.variable_name == intermediate_variable.variable_name) {
                intermediate_list.erase(intermediate_list.begin()+i);
                break;
            }
        }
        intermediate_list.push_back(v);

        /*cout << "intermediate_list:" << endl;
        for(Variable intermediate_variable : intermediate_list) {
            intermediate_variable.print();
        }
        cout << endl;*/
    }

    void set_intermediate_variable(Variable v) {
        add_intermediate(v);
    }

    Variable get_intermediate_variable(string name) {
        if(&intermediate_list == nullptr || intermediate_list.size() == 0) {
            Variable v;
            return v;
        }
        for(Variable variable : intermediate_list) {
            if(variable.variable_name == name) {
                return variable;
            }
        }
        Variable v;
        return v;
    }

    SgExpression* handle_binary_op(SgBinaryOp* binary_op);

    SgExpression* handle_unary_op(SgUnaryOp* binary_op);

    SgExpression* handle_expression(SgExpression* expression);

    virtual string get_ir_content(int tab_num) {
        return "TODO generate ir content";
    }
};