//
// Created by root on 2021/3/6.
//

#ifndef FRONTEND_COMMON_H
#define FRONTEND_COMMON_H

#include "rose.h"

namespace common {
    extern const std::string TAB;

    extern const std::string FILE_NAME_STRING;

    extern std::map<VariantT, std::string> operator_str_map;

    void init_operator_str_map();

    std::string get_line_start_blank(int tab_num);

    std::string get_operator_str(VariantT type);
}

#endif //FRONTEND_COMMON_H