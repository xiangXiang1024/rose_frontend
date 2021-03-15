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
    SgExpression* constraint = nullptr;
    SgExpression* change_expr = nullptr;

    IndexVariable() {}

    IndexVariable(SgVarRefExp* var_ref) {
        SgInitializedName* initialize_name = var_ref -> get_symbol() -> get_declaration();
        variable_name = initialize_name -> get_name().getString();
        type = initialize_name -> get_type();
    }

    void print() {
        cout << variable_name << "\t\t(" << type -> unparseToString() << ")" << endl;
    }
};


#endif //FRONTEND_INDEXVARIABLE_H
