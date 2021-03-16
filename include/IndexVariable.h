//
// Created by root on 2021/3/15.
//

#ifndef FRONTEND_INDEXVARIABLE_H
#define FRONTEND_INDEXVARIABLE_H

#include "rose.h"

using namespace std;

class IndexVariable {
public:
    string variable_name = "";
    SgType *type;
    SgExpression* initializer = nullptr;
    SgExpression* constraint = nullptr;
    SgExpression* change_expr = nullptr;

    IndexVariable() {}

    IndexVariable(SgVarRefExp* var_ref) {
        SgInitializedName* initialize_name = var_ref -> get_symbol() -> get_declaration();
        variable_name = initialize_name -> get_name().getString();
        type = initialize_name -> get_type();
    }

    void print() {
        cout << variable_name << "\t\t(" << type -> unparseToString() << ")\t";
        cout << "initializer|";
        if(initializer == nullptr) {
            cout << "\t initializer == nullptr";
        }else {
            cout << "\t " << initializer -> unparseToString();
        }
        cout << "constraint|";
        if(constraint == nullptr) {
            cout << "\t constraint == nullptr";
        }else {
            cout << "\t " << constraint -> unparseToString();
        }
        cout << "change_expr|";
        if(change_expr == nullptr) {
            cout << "\t change_expr == nullptr";
        }else {
            cout << "\t " << change_expr -> unparseToString();
        }
        cout << endl;
    }
};


#endif //FRONTEND_INDEXVARIABLE_H
