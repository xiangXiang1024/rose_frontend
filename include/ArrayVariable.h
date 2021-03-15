//
// Created by root on 2021/3/11.
//

#ifndef FRONTEND_ARRAYVARIABLE_H
#define FRONTEND_ARRAYVARIABLE_H


#include "Variable.h"
#include "IndexVariable.h"

class ArrayVariable : public Variable {
public:
    vector<IndexVariable*> index_list;

    ArrayVariable() {}

    ArrayVariable(SgPntrArrRefExp* arr_ref) {
        arr_ref_exp = arr_ref;
        index_list = get_index_list(arr_ref);
        variable_name = index_list.at(0) -> variable_name;
        type = index_list.at(0) -> type;
        index_list.erase(index_list.begin());
    }

    void print() {
        cout << "array: " << variable_name << "\t\t(" << type -> unparseToString() << ")\t\t" << expression_str << endl;
        cout << "index list: " << endl;
        for(IndexVariable* index : index_list) {
            index -> print();
        }
    }

private:
    SgPntrArrRefExp* arr_ref_exp;

    vector<IndexVariable*> get_index_list(SgPntrArrRefExp* arr_ref);
};


#endif //FRONTEND_ARRAYVARIABLE_H
