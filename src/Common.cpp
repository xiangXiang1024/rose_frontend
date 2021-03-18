//
// Created by root on 2021/3/6.
//
#include "Common.h"

namespace common {
    const std::string TAB = "    ";

    const std::string FUNCTION_IR_TEMPLATE = "{\"program_name\":\"PROGRAM_NAME\"}";

    std::map<VariantT, std::string> operator_str_map;

    void init_operator_str_map() {
        operator_str_map[V_SgEqualityOp] = "==";
        operator_str_map[V_SgLessThanOp] = "<";
        operator_str_map[V_SgGreaterThanOp] = ">";
        operator_str_map[V_SgNotEqualOp] = "!=";
        operator_str_map[V_SgLessOrEqualOp] = "<=";
        operator_str_map[V_SgGreaterOrEqualOp] = ">=";
        operator_str_map[V_SgAddOp] = "+";
        operator_str_map[V_SgSubtractOp] = "-";
        operator_str_map[V_SgMultiplyOp] = "*";
        operator_str_map[V_SgDivideOp] = "/";
        operator_str_map[V_SgIntegerDivideOp] = "/";
        operator_str_map[V_SgModOp] = "%";
        operator_str_map[V_SgAndOp] = "&&";
        operator_str_map[V_SgOrOp] = "||";
        operator_str_map[V_SgBitXorOp] = "^";
        operator_str_map[V_SgBitAndOp] = "&";
        operator_str_map[V_SgBitOrOp] = "|";
        operator_str_map[V_SgCommaOpExp] = ",";
        operator_str_map[V_SgLshiftOp] = "<<";
        operator_str_map[V_SgRshiftOp] = ">>";
        operator_str_map[V_SgNotOp] = "!";
        operator_str_map[V_SgAssignOp] = "=";
        operator_str_map[V_SgPointerDerefExp] = "*";
        operator_str_map[V_SgAddressOfOp] = "&";
        operator_str_map[V_SgPlusAssignOp] = "+=";
        operator_str_map[V_SgMinusAssignOp] = "-=";
        operator_str_map[V_SgAndAssignOp] = "&&=";
        operator_str_map[V_SgIorAssignOp] = "||=";
        operator_str_map[V_SgMultAssignOp] = "*=";
        operator_str_map[V_SgDivAssignOp] = "/=";
        operator_str_map[V_SgModAssignOp] = "%=";
        operator_str_map[V_SgXorAssignOp] = "^=";
        operator_str_map[V_SgLshiftAssignOp] = "<<=";
    }

    std::string get_operator_str(VariantT type) {
        auto op = operator_str_map.find(type);
        return op -> second;
    }

    std::string get_line_start_blank(int tab_num) {
        std::string blank = "";
        for(int i = 0 ; i < tab_num ; i++) {
            blank += TAB;
        }
        return blank;
    }
}