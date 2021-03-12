//
// Created by root on 2021/3/11.
//

#ifndef FRONTEND_ARRAYVARIABLE_H
#define FRONTEND_ARRAYVARIABLE_H


#include "Variable.h"

class ArrayVariable : public Variable {
    vector<Variable> index_list;
};


#endif //FRONTEND_ARRAYVARIABLE_H
