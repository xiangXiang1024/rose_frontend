//
// Created by root on 2021/3/9.
//

#include "Function.h"

void Function::generate_input_list() {
    SgFunctionParameterList* params = function_declaration -> get_parameterList();
    for (SgInitializedName* param : params -> get_args()) {
        string name = param -> get_name().getString();
        SgType* type = param -> get_type();
        Variable* variable = new Variable(name, type);
        add_input(variable);
        add_intermediate(variable);
    }
}

void Function::generate_statement_list() {
    SgFunctionDefinition *func_def = function_declaration->get_definition();
    SgBasicBlock *func_body = func_def->get_body();
    SgStatementPtrList &stmt_list = func_body->get_statements();
    for (auto s : stmt_list) {
        statement_list.push_back(dynamic_cast<SgStatement*>(s));
    }
}

void Function::generate_output_list() {
    output_list = segment.output_list;
}

void Function::generate_line_info() {
    line_info = segment.line_info;
    sort(line_info.begin(), line_info.end());
}