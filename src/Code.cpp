//
// Created by root on 2021/3/5.
//

#include "Code.h"

SgExpression* Code::handle_binary_op(SgBinaryOp* binary_op) {
    SgExpression* left = binary_op ->get_lhs_operand();
    SgExpression* right = binary_op ->get_rhs_operand();

    if(left -> class_name() == "SgVarRefExp") {
        Variable ref_variable = get_intermediate_variable(left -> unparseToString());
        if(ref_variable.get_expression() != nullptr) {
            binary_op -> set_lhs_operand(ref_variable.get_expression());
        }
    }else {
        SgBinaryOp* left_binary_op = dynamic_cast<SgBinaryOp*>(left);
        if(left_binary_op != nullptr) {
            SgExpression* left_expr = handle_binary_op(left_binary_op);
            if(left_expr != nullptr) {
                binary_op -> set_lhs_operand(left_expr);
            }
        }
    }

    if(right -> class_name() == "SgVarRefExp") {
        Variable ref_variable = get_intermediate_variable(right -> unparseToString());
        if(ref_variable.get_expression() != nullptr) {
            binary_op -> set_rhs_operand(ref_variable.get_expression());
        }
    }else {
        SgBinaryOp* right_binary_op = dynamic_cast<SgBinaryOp*>(right);
        if(right_binary_op != nullptr) {
            SgExpression* right_expr = handle_binary_op(right_binary_op);
            if(right_expr != nullptr) {
                binary_op -> set_rhs_operand(right_expr);
            }
        }
    }

    return binary_op;
}

SgExpression* Code::handle_unary_op(SgUnaryOp* binary_op) {
    return handle_expression(binary_op);
}

SgExpression* Code::handle_expression(SgExpression* expression) {
    if(dynamic_cast<SgVarRefExp*>(expression) != nullptr) {
        Variable ref_variable = get_intermediate_variable(expression -> unparseToString());
        if(ref_variable.get_expression() != nullptr) {
            return ref_variable.get_expression();
        }
    }else if(dynamic_cast<SgBinaryOp*>(expression) != nullptr) {
        SgBinaryOp* binary_op = dynamic_cast<SgBinaryOp*>(expression);
        if(binary_op != nullptr) {
            SgExpression* expr = handle_binary_op(binary_op);
            if(expr != nullptr) {
                return expr;
            }
        }
    }else if(dynamic_cast<SgUnaryOp*>(expression) != nullptr) {
        SgUnaryOp* unary_op = dynamic_cast<SgUnaryOp*>(expression);
        if(unary_op != nullptr) {
            unary_op ->set_operand(unary_op -> get_operand());
            return unary_op;
        }
    }
    return expression;
}