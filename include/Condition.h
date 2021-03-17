//
// Created by root on 2021/3/8.
//

#include "rose.h"
#include <string>

#ifndef FRONTEND_CONDITION_H
#define FRONTEND_CONDITION_H

using namespace std;

class Condition {
public:
    SgExpression* expression;
    bool is_false = false;

    Condition(SgExpression* _expression) : expression(_expression) {}

    Condition(SgExpression* _expression, bool _is_false) : expression(_expression), is_false(_is_false) {}

    string get_string();
};

#endif //FRONTEND_CONDITION_H