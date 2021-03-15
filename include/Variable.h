//
// Created by root on 2021/3/5.
//

#ifndef FRONTEND_VARIABLE_H
#define FRONTEND_VARIABLE_H

#endif //FRONTEND_VARIABLE_H

#include <string>
#include "rose.h"

using namespace std;

class Variable {
public:
    string variable_name = "";
    SgType *type;
    SgExpression* init_expression = nullptr;
    string expression_str = "";

    Variable(const string &_variable_name, SgType *_type)
        : variable_name(_variable_name), type(_type) {
        init_expression = nullptr;
        expression = nullptr;
        expression_str = variable_name;
    }

    Variable() {}

    Variable(SgPntrArrRefExp* array_ref) {
//        SgInitializedName* initialize_name = var_ref -> get_symbol() -> get_declaration();
        variable_name = array_ref -> unparseToString();
        type = array_ref -> get_type();
//        type = initialize_name -> get_type();
    }

    Variable(SgVarRefExp* var_ref) {
        SgInitializedName* initialize_name = var_ref -> get_symbol() -> get_declaration();
        variable_name = initialize_name -> get_name().getString();
        type = initialize_name -> get_type();
    }

    void set_expression(SgExpression* exp) {
        expression = exp;
        if(exp != nullptr) {
            expression_str = exp -> unparseToString();
        }else {
            expression_str = "";
        }
    }

    void set_init_expression(SgExpression* exp) {
        init_expression = exp;
        expression = exp;
        expression_str = exp -> unparseToString();
    }

    SgExpression* get_expression() {
        return expression;
    }

    virtual void print() {
        cout << variable_name << "\t\t(" << type -> unparseToString() << ")\t\t" << expression_str << endl;
    }

private:
    SgExpression* expression = nullptr;
};