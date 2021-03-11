//
// Created by root on 2021/3/8.
//

#include "Condition.h"

string Condition::get_string() {
    if(is_false) {
        string s = "!(";
        s.append(expression -> unparseToString());
        s.append(")");
        return s;
    }else {
        return expression -> unparseToString();
    }
}