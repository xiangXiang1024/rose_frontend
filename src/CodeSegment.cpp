//
// Created by root on 2021/3/9.
//

#include <LoopSegment.h>
#include "CodeSegment.h"

// TODO check recursion
bool CodeSegment::partition() {
    bool do_partition = false;
    int origin_current_ptr = current_ptr;

    for( ; current_ptr < statement_list.size() ; current_ptr++) {
        SgStatement* statement = statement_list.at(current_ptr);
//        cout << "in partition(): statement: " << statement -> unparseToString() << "\t|\t" << statement -> class_name() << endl;
        if(dynamic_cast<SgForStatement*>(statement) != nullptr||dynamic_cast<SgWhileStmt*>(statement) != nullptr||dynamic_cast<SgDoWhileStmt*>(statement) != nullptr) {
//            cout << "meet for partition" << endl;
//            cout << "current_ptr: " << current_ptr << endl;
//            cout << "condition: " << get_condition_str() << endl;
//            cout << "statements: " << endl;
            /*for(int i = 0 ; i < statement_list.size() ; i++) {
                cout << i << "\t" << statement_list.at(i) -> unparseToString() << endl;
            }*/

            vector<SgStatement*> statement_list1(statement_list.begin(), statement_list.begin() + current_ptr);
            CodeSegment* segment1 = new CodeSegment(statement_list1, condition_list, input_list, output_list, intermediate_list, 0, this);
            vector<SgStatement*> statement_list2;
            statement_list2.push_back(statement);
//            cout << "generate loop segment: " << endl;
//            cout << "condition: " << get_condition_str() << endl;
            LoopSegment* segment2 = new LoopSegment(statement_list2, condition_list, input_list, output_list, intermediate_list, 0, this);
            vector<SgStatement*> statement_list3(statement_list.begin() + current_ptr + 1, statement_list.end());
            CodeSegment* segment3 = new CodeSegment(statement_list3, condition_list, input_list, output_list, intermediate_list, 0, this);

            if(segment1 -> statement_list.size() > 0 || segment1 -> is_break || segment1 -> is_continue) {
                segment_list.push_back(segment1);
            }
            segment_list.push_back(segment2);
            if(segment3 -> statement_list.size() > 0 || segment3 -> is_break || segment3 -> is_continue) {
                segment_list.push_back(segment3);
            }

            /*cout << "after partition for:" << endl;
            for(CodeSegment* s : segment_list) {
                s -> print();
            }*/

            do_partition = true;
        }else {
            SgFunctionCallExp* func_call_exp = has_func_call(statement);
            if(func_call_exp != nullptr) {
//                cout << "meet func call" << endl;
//                cout << "statement: " << statement -> unparseToString() << "\t|\t" << statement -> class_name() << endl;
//                cout << "SgFunctionCallExp: " << func_call_exp -> unparseToString() << "\t|\t" << func_call_exp -> class_name() << endl;
                /*for(SgNode* node : func_call_exp -> get_traversalSuccessorContainer()) {
                    cout << node -> unparseToString() << "\t|\t" << node -> class_name() << endl;
                }*/

                vector<SgStatement*> statement_list1(statement_list.begin(), statement_list.begin() + current_ptr);
                CodeSegment* segment1 = new CodeSegment(statement_list1, condition_list, input_list, output_list, intermediate_list, 0, this);

                SgExprListExp* expr_list = func_call_exp -> get_args();
//                cout << "SgExprListExp: " << expr_list -> unparseToString() << "\t|\t" << expr_list -> class_name() << endl;
                /*for(SgNode* node : expr_list -> get_traversalSuccessorContainer()) {
                    cout << node -> unparseToString() << "\t|\t" << node -> class_name() << endl;
                }*/
                SgExpressionPtrList&  expr_ptr_list = expr_list -> get_expressions();
//                cout << "SgExpressionPtrList&: " << endl;
                for(auto expr_ptr : expr_ptr_list) {
//                    cout << expr_ptr -> unparseToString() << "\t|\t" << expr_ptr -> class_name() << endl;
                    vector<Variable*> ref_variable_list = get_ref_variable_list(expr_ptr);
//                    cout << "ref variable: " << endl;
                    for(Variable* v : ref_variable_list) {
//                        v.print();
                        segment1 -> add_output(v);
                    }
//                    cout << endl;
                }

                SgFunctionDeclaration* ref_func_declaration = func_call_exp -> getAssociatedFunctionDeclaration();
                // TODO check recursion

                vector<SgStatement*> statement_list3(statement_list.begin() + current_ptr + 1, statement_list.end());
                CodeSegment* segment3 = new CodeSegment(statement_list3, condition_list, input_list, output_list, intermediate_list, 0, this);

                segment_list.push_back(segment1);
                segment_list.push_back(segment3);

                do_partition = true;
            }

        }
    }

    current_ptr = origin_current_ptr;
    return do_partition;
}

// TODO
void CodeSegment::handle_statement(SgStatement* statement) {
//    cout << "handle statement: " << statement -> unparseToString() << "\t|\t" << statement -> class_name() << endl;
    if(dynamic_cast<SgVariableDeclaration*>(statement) != nullptr) {
        handle_variable_declaration(dynamic_cast<SgVariableDeclaration*>(statement));
    }else if(dynamic_cast<SgExprStatement*>(statement) != nullptr) {
        handle_expr_statement(dynamic_cast<SgExprStatement*>(statement));
    }else if(dynamic_cast<SgReturnStmt*>(statement) != nullptr) {
        handle_return_statement(dynamic_cast<SgReturnStmt*>(statement));
    }else if(dynamic_cast<SgIfStmt*>(statement) != nullptr) {
        handle_if_statement(dynamic_cast<SgIfStmt*>(statement));
    }else if(dynamic_cast<SgBreakStmt*>(statement) != nullptr) {
        handle_break_statement(dynamic_cast<SgBreakStmt*>(statement));
    }else if(dynamic_cast<SgContinueStmt*>(statement) != nullptr) {
        handle_continue_statement(dynamic_cast<SgContinueStmt*>(statement));
    }// TODO
}

void CodeSegment::handle_variable_declaration(SgVariableDeclaration* statement) {
//    cout << endl << "handle variable declaration statement\t\t" << statement -> unparseToString() << endl;
    SgInitializedNamePtrList& list = statement -> get_variables();
    for(SgInitializedName* n : list) {
        SgType* type = n -> get_type();
        string var_name = n -> get_name();
        Variable* variable = new Variable(var_name, type);
//        cout << "declare variable: " << var_name << "  type: " << type -> get_mangled() << endl;

        if(n -> get_initptr() == nullptr) {
//            cout << "init type: " << "nullptr" << endl;
        }else if(n -> get_initptr() -> class_name() == "SgAssignInitializer") {
//            cout << "init type: " << "SgAssignInitializer" << endl;
            auto initializer = dynamic_cast<SgAssignInitializer*>(n -> get_initptr());
//            cout << "operand: " << initializer -> get_operand() -> unparseToString() << endl;
//            cout << "operand type: " << initializer -> get_operand() -> class_name() << endl;

            SgExpression* expression = handle_expression(dynamic_cast<SgExpression*>(initializer -> get_operand()));
            variable -> set_expression(expression);
        }else {// TODO

        }

       /* cout << "declare variable: ";
        variable.print();*/
        set_intermediate_variable(variable);
    }
}

// TODO  ++  +=
// TODO 定位矩阵运算
void CodeSegment::handle_expr_statement(SgExprStatement* statement) {
//    cout << endl << "handle expr statement: " << statement -> unparseToString() << "\t|\t" << statement -> class_name() << endl;
    vector<SgNode*> node_list = statement -> get_traversalSuccessorContainer();
    for(SgNode* n : node_list) {
//        cout << n->unparseToString() << "\t" << n->class_name() << endl;
        if(dynamic_cast<SgAssignOp*>(n) != nullptr) {
//            cout << "handle SgAssignOp" << endl;
            SgAssignOp* assign_op = dynamic_cast<SgAssignOp*>(n);
            SgNode* var_node = assign_op -> get_traversalSuccessorByIndex(0);
            if(dynamic_cast<SgVarRefExp*>(var_node) != nullptr) {
                SgVarRefExp* var_ref = dynamic_cast<SgVarRefExp*>(var_node);
                Variable* v = new Variable(var_ref);
                SgNode* expression_node = assign_op -> get_traversalSuccessorContainer().back();
                SgExpression* expression = dynamic_cast<SgExpression*>(expression_node);
                v -> set_expression(handle_expression(expression));
                set_intermediate_variable(v);
            }else if(dynamic_cast<SgPntrArrRefExp*>(var_node) != nullptr) {
//                    cout << "meet array: " << var_node -> unparseToString() << "\t|\t" << var_node -> class_name() << endl;
                    SgPntrArrRefExp* arr_ref = dynamic_cast<SgPntrArrRefExp*>(var_node);

//                    cout << "generate array variable: " << endl;
                    ArrayVariable* array_variable = new ArrayVariable(arr_ref);
//                    array_variable.print();

                    SgNode* expression_node = assign_op -> get_traversalSuccessorContainer().back();
                    SgExpression* expression = dynamic_cast<SgExpression*>(expression_node);
                    array_variable -> set_expression(handle_expression(expression));

//                    cout << "after analyze: " << endl;
//                    array_variable.print();
                    set_intermediate_variable(array_variable);
                    /*cout << "after add in handle_expr_statement() intermediate_list:" << endl;
                    for(Variable intermediate_variable : intermediate_list) {
                        intermediate_variable.print();
                    }
                    cout << endl;*/
            }
        }else if(dynamic_cast<SgUnaryOp*>(n) != nullptr) {// TODO
//            cout << n->unparseToString() << "transfer to SgUnaryOp" << endl;
            SgUnaryOp* unary_op = dynamic_cast<SgUnaryOp*>(n);
//            cout << "unary_op -> get_operand(): " << unary_op -> get_operand() -> unparseToString() << endl;
//            cout << "unary_op -> get_operand_i(): " << unary_op -> get_operand_i() -> unparseToString() << endl;
//            cout << "unary_op -> get_operand() -> get_type(): " << unary_op -> get_operand() -> get_type() -> unparseToString() << endl;

            SgType* sg_type = unary_op -> get_operand() -> get_type();
            SgNode* var_node = n -> get_traversalSuccessorByIndex(0);
            SgVarRefExp* var_ref = dynamic_cast<SgVarRefExp*>(var_node);
            SgIntVal* int_val = new SgIntVal(1, "");
            string class_name = n -> class_name();
            if(class_name.find("Plus") != class_name.npos) {
                SgAddOp* add_op = new SgAddOp(var_ref, int_val, sg_type);
                add_op -> set_startOfConstruct(unary_op->get_operand() -> get_startOfConstruct());
                add_op -> set_endOfConstruct(unary_op->get_operand() -> get_endOfConstruct());
//                dynamic_cast<SgExpression*>(n) -> replace_expression(unary_op, add_op);
                unary_op -> replace_expression(unary_op->get_operand(), add_op);
//                cout << "unary_op: " << unary_op -> unparseToString() << endl;
                /*cout << "todo add_op set_file_info" << endl;
                add_op -> set_file_info(n -> get_file_info());
                cout << "todo add_op set_startOfConstruct" << endl;
                add_op -> set_startOfConstruct(n -> get_startOfConstruct());*/

                Sg_File_Info* file_info = n -> get_endOfConstruct();
                /*cout << "endOfConstruct: " << file_info << endl;
                if(file_info == nullptr) {
                    cout << "file_info == nullptr" << endl;
                }*/

//                cout << "todo add_op set_endOfConstruct" << endl;
//                add_op -> set_endOfConstruct(statement -> get_endOfConstruct());
               /* if(add_op -> get_endOfConstruct() == nullptr) {
                    cout << "add_op -> get_endOfConstruct() == nullptr" << endl;
                }*/

                /*for(SgNode* node : add_op -> get_traversalSuccessorContainer()) {
                    cout << node -> unparseToString() << "\t|\t" << node -> class_name() << endl;
                }*/
            }
        }else if(n -> class_name().find("Assign")) {
            SgNode* var_node = n -> get_traversalSuccessorByIndex(0);
            SgVarRefExp* var_ref = dynamic_cast<SgVarRefExp*>(var_node);
            Variable* v = new Variable(var_ref);
            set_intermediate_variable(v);
        }else {
            // TODO
        }
    }
}

void CodeSegment::handle_if_statement(SgIfStmt* statement) {
//    cout << endl << "handle if statement\t\t" << statement -> unparseToString() << endl;

    SgStatement* condition_statement = statement ->	get_conditional();
    SgExpression* condition_expr = dynamic_cast<SgExprStatement*>(condition_statement) -> get_expression();
    SgExpression* expr = handle_expression(condition_expr);

    CodeSegment* false_segment = new CodeSegment(statement_list, condition_list, input_list, output_list, intermediate_list, current_ptr, this);

    Condition true_condition(expr);
    add_condition(true_condition);

    SgStatement* true_body = statement -> get_true_body();
    SgBasicBlock* true_block = dynamic_cast<SgBasicBlock*>(true_body);
    vector<SgStatement*> true_statement_list;
    for(auto s : true_block -> get_traversalSuccessorContainer()) {
        true_statement_list.push_back(dynamic_cast<SgStatement*>(s));
//        cout << s -> unparseToString() << endl;
    }
    statement_list.erase(statement_list.begin() + current_ptr);
    statement_list.insert(statement_list.begin() + current_ptr, true_statement_list.begin(), true_statement_list.end());
    current_ptr--;

    SgStatement* false_body = statement -> get_false_body();
    vector<SgStatement*> false_statement_list;
    if(false_body != nullptr) {
        if(false_body -> class_name() == "SgBasicBlock") {
            for(auto s : false_body -> get_traversalSuccessorContainer()) {
                false_statement_list.push_back(dynamic_cast<SgStatement*>(s));
            }
        }else if(false_body -> class_name() == "SgIfStmt") {
            false_statement_list.push_back(false_body);
        }
    }
    false_segment -> statement_list.erase(false_segment -> statement_list.begin() + false_segment -> current_ptr);
    if(false_body != nullptr) {
        false_segment -> statement_list.insert(false_segment -> statement_list.begin() + false_segment -> current_ptr, false_statement_list.begin(), false_statement_list.end());
    }
    false_segment -> current_ptr--;
    Condition false_condition(expr, true);
    false_segment -> add_condition(false_condition);

//    cout << "false segment: " << endl;
//    false_segment -> print();

    other_execute_path_list.push_back(false_segment);
}

void CodeSegment::handle_return_statement(SgReturnStmt* statement) {
//    cout << endl << "handle return statement\t\t" << statement -> unparseToString() << endl;
    SgExpression* expression = statement -> get_expression();

//    cout << "expr: " << expression -> unparseToString()  << "\t" << expression -> class_name() << "\t" << endl;
//    cout << "get_traversalSuccessorContainer:" << endl;

    if(dynamic_cast<SgVarRefExp*>(expression) != nullptr) {
        Variable* output = get_intermediate_variable(dynamic_cast<SgVarRefExp*>(expression) -> get_symbol() -> get_declaration() -> get_name().getString());
        if(output -> variable_name == "") {
            output = new Variable(dynamic_cast<SgVarRefExp*>(expression));
        }
//        Variable variable(dynamic_cast<SgVarRefExp*>(expression));
        add_output(output);
    }else if(dynamic_cast<SgPntrArrRefExp*>(expression) != nullptr) {
        SgPntrArrRefExp* array_ref = dynamic_cast<SgPntrArrRefExp*>(expression);
        Variable* output = get_intermediate_variable(array_ref -> unparseToString());
        if(output -> variable_name == "") {
            output = new Variable(array_ref);
        }
//        cout << "get array output: ";
//        output.print();
        add_output(output);
    }else {
        for(SgNode* node : expression -> get_traversalSuccessorContainer()) {
//            cout << node -> class_name() << endl;
            if(node -> class_name() == "SgVarRefExp") {
                SgVarRefExp* var_ref_exp = dynamic_cast<SgVarRefExp*>(node);
                Variable* output = get_intermediate_variable(var_ref_exp -> get_symbol() -> get_declaration() -> get_name().getString());
//                Variable variable(var_ref_exp);
                add_output(output);
            }
        }
    }

    while(statement_list.size() > current_ptr + 1) {
        statement_list.pop_back();
    }
}

void CodeSegment::handle_break_statement(SgBreakStmt* statement) {
    is_break = true;
    while(statement_list.size() > current_ptr + 1) {
        statement_list.pop_back();
    }
}

void CodeSegment::handle_continue_statement(SgContinueStmt* statement) {
    is_continue = true;
    while(statement_list.size() > current_ptr + 1) {
        statement_list.pop_back();
    }
}

SgExpression* CodeSegment::handle_expression(SgExpression* expression) {
//    cout << "handle_expression: " << expression -> unparseToString() << "\t|\t" << expression -> class_name() << endl;

    if(dynamic_cast<SgVarRefExp*>(expression) != nullptr) {
        Variable* ref_variable = get_intermediate_variable(expression -> unparseToString());
        if(ref_variable -> get_expression() != nullptr) {
            return ref_variable -> get_expression();
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
            unary_op -> set_operand(unary_op -> get_operand());
            return unary_op;
        }
    }
    return expression;
}

SgExpression* CodeSegment::handle_binary_op(SgBinaryOp* binary_op) {
    SgExpression* left = binary_op ->get_lhs_operand();
    SgExpression* right = binary_op ->get_rhs_operand();

    if(left -> class_name() == "SgVarRefExp") {
        Variable* ref_variable = get_intermediate_variable(left -> unparseToString());
        if(ref_variable -> get_expression() != nullptr) {
            binary_op -> set_lhs_operand(ref_variable -> get_expression());
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

//    cout << "right class: " << right -> class_name() << endl;
//    cout << "right getFilenameString: " << right -> getFilenameString() << endl;
    if(right -> class_name() == "SgVarRefExp") {
        Variable* ref_variable = get_intermediate_variable(right -> unparseToString());
        if(ref_variable -> get_expression() != nullptr) {
            binary_op -> set_rhs_operand(ref_variable -> get_expression());
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

void CodeSegment::add_condition(Condition condition) {
    condition_list.push_back(condition);
}

string CodeSegment::get_condition_str() {
    string s = "";

    if(condition_list.size() == 0) {
        return "true";
    }

    for(Condition c : condition_list) {
        s.append(c.get_string());
        s.append(" && ");
    }

    s = s.substr(0, s.length() - 4);

    return s;
}

SgFunctionCallExp* CodeSegment::has_func_call(SgNode* statement) {
//    cout << "check has_func_call node: " << endl;
//    cout << "statement: " << statement -> unparseToString() << "\t|\t" << statement -> class_name() << endl;

    if(dynamic_cast<SgFunctionCallExp*>(statement) != nullptr) {
//        cout << "meet func call statement: " << statement -> unparseToString() << endl;
        return dynamic_cast<SgFunctionCallExp*>(statement);
    }

    vector<pair<SgNode*, string>> pair_list = statement -> returnDataMemberPointers();
    for(pair<SgNode*, string> p : pair_list) {
        if(p.first == nullptr) {
            break;
        }
        /*cout << "pair: " << endl;
        cout << "node: " << p.first -> unparseToString() << "\t|\t" << p.first -> class_name() << endl;
        cout << "string: " << p.second << endl;*/
        if(dynamic_cast<SgStatement*>(p.first) != nullptr || dynamic_cast<SgExpression*>(p.first) != nullptr) {
            SgFunctionCallExp* exp = has_func_call(p.first);
            if(exp != nullptr) {
//                cout << "meet func call statement: " << exp -> unparseToString() << endl;
                return exp;
            }
        }
    }

    return nullptr;
}

vector<Variable*> CodeSegment::get_ref_variable_list(SgExpression* expression) {
//    cout << "get ref variables (expression: " << expression -> unparseToString() << " )" << endl;
    vector<Variable*> ref_variable_list;

//    cout << "get_traversalSuccessorContainer: " << endl;
    for(SgNode* n : expression -> get_traversalSuccessorContainer()) {
//        cout << n -> unparseToString() << "\t|\t" << n -> class_name() << endl;
        if(dynamic_cast<SgValueExp*>(n) != nullptr) {
            continue;
        }
        if(dynamic_cast<SgVarRefExp*>(n) != nullptr) {
//            cout << "find variable ref" << endl;
            Variable* ref_v = new Variable(dynamic_cast<SgVarRefExp*>(n));
            bool has_variable = false;
            for(Variable* v : ref_variable_list) {
                if(v -> variable_name == ref_v -> variable_name) {
                    has_variable = true;
                    break;
                }
            }
            if(!has_variable) {
                ref_variable_list.push_back(ref_v);
            }
        }
        if(dynamic_cast<SgExpression*>(n) != nullptr) {
            vector<Variable*> tmp_vector = get_ref_variable_list(dynamic_cast<SgExpression*>(n));
            for(Variable* ref_v : tmp_vector) {
                bool has_variable = false;
                for(Variable* v : ref_variable_list) {
                    if(v -> variable_name == ref_v -> variable_name) {
                        has_variable = true;
                        break;
                    }
                }
                if(!has_variable) {
                    ref_variable_list.push_back(ref_v);
                }
            }
        }
    }

    return ref_variable_list;
}

void CodeSegment::calculate_input() {
//    cout << "calculate inputs:" << endl;

//    print();

    vector<Variable*> inputs;

    for(Variable* v : output_list) {
        SgExpression* expression = get_intermediate_variable(v -> variable_name) -> get_expression();
        if(expression != nullptr) {
//            cout << v.variable_name << "parse expression: " << expression -> unparseToString() << endl;
            vector<SgExpression*> var_refs = get_var_ref(expression);
            for(SgExpression* ref : var_refs) {
                Variable* v = get_intermediate_variable(ref -> unparseToString());
                if(v -> variable_name != "") {
                    inputs.push_back(v);
                }
            }
        }else {
//            cout << v.variable_name << "is input" << endl;
            if(v -> variable_name != "") {
                    inputs.push_back(v);
            }
        }
    }

    /*for(Variable v : inputs) {
        cout << "get inputs: ";
        v.print();
    }*/

    add_input(inputs);
}

vector<SgExpression*> CodeSegment::get_var_ref(SgExpression* expression) {
    vector<SgExpression*> var_refs;

    if(expression == nullptr) {
        return var_refs;
    }else if(dynamic_cast<SgPntrArrRefExp*>(expression) != nullptr || dynamic_cast<SgVarRefExp*>(expression) != nullptr) {
        var_refs.push_back(expression);
        return var_refs;
    }else if(dynamic_cast<SgValueExp*>(expression) != nullptr) {
        return var_refs;
    }else {
        for(SgNode* n : expression -> get_traversalSuccessorContainer()) {
            if(dynamic_cast<SgExpression*>(expression) != nullptr) {
                vector<SgExpression*> tmp = get_var_ref(dynamic_cast<SgExpression*>(n));
                var_refs.insert(var_refs.end(), tmp.begin(), tmp.end());
            }
        }
    }

    return var_refs;
}

void CodeSegment::handle_array(ArrayVariable* array) {
//    cout << "handle array" << endl;
//    array -> print();
    vector<IndexVariable*> index_list = array -> index_list;
    for(IndexVariable* index : index_list) {
        string variable_name = index -> variable_name;
        SgExpression* expr = nullptr;
        Code* node = this;
        while(node != nullptr && dynamic_cast<CodeSegment*>(node) != nullptr) {
            Variable* v = node -> get_intermediate_variable(variable_name);
            if(v != nullptr && v -> get_expression() != nullptr) {
                expr = v -> get_expression();
                break;
            }
            for(CodeSegment* segment : dynamic_cast<CodeSegment*>(node) -> segment_list) {
                Variable* v = segment -> get_intermediate_variable(variable_name);
                if(v != nullptr && v -> get_expression() != nullptr) {
                    expr = v -> get_expression();
                    break;
                }
            }
            node = dynamic_cast<CodeSegment*>(node) -> parent_node;
        }
        if(expr != nullptr) {
//            cout << "find index " << variable_name << " expr: " << expr -> unparseToString() << endl;
            index -> change_expr = expr;
        }
        Condition* condition = get_condition_by_variable(variable_name);
        if(condition != nullptr) {
//            cout << "find " << index -> variable_name << " condition: " << condition -> get_string() << endl;
            index -> constraint = condition;
        }

        SgExpression* init_expr = get_initializer_expr(variable_name, index);
        if(init_expr != nullptr) {
//            cout << "find " << variable_name << " init_expr: " << init_expr -> unparseToString() << endl;
            index -> initializer = init_expr;
        }
    }

//    cout << "---- after handle array ----" << endl;
//    array -> print();
}

Condition* CodeSegment::get_condition_by_variable(string variable_name) {
//    cout << "in func get_condition_by_variable\t\tvariable: " << variable_name << endl;

    Code* node = this;
    while(node != nullptr && dynamic_cast<CodeSegment*>(node) != nullptr) {
        vector<Condition> condition_list = dynamic_cast<CodeSegment*>(node) -> condition_list;
        for(Condition c : condition_list) {
            SgExpression* expression = c.expression;
//            cout << "expression: " << expression -> unparseToString() << endl;
            bool find_var = false;
            for(SgNode* node : expression -> get_traversalSuccessorContainer()) {
//                cout << "\tnode: " << node -> unparseToString() << "\t|\t" << node -> class_name() << endl;
                if(dynamic_cast<SgVarRefExp*>(node) != nullptr && node -> unparseToString() == variable_name) {
                    find_var = true;
                    break;
                }
            }
            if(find_var) {
                Condition* condition = new Condition(c.expression, c.is_false);
                return condition;
            }
        }
        node = dynamic_cast<CodeSegment*>(node) -> parent_node;
    }

    return nullptr;
}

SgExpression* CodeSegment::get_initializer_expr(string variable_name, IndexVariable* index) {
//    cout << "in func get_initializer_expr\t\tvariable: " << variable_name << endl;

    vector<SgExpression*> initializer_list;
    Code* node = this;
    Code* pre_node = nullptr;
    while(node != nullptr && dynamic_cast<CodeSegment*>(node) != nullptr) {
        bool has_initializer = false;
        Variable* v = node -> get_intermediate_variable(variable_name);
//        if(v != nullptr && v -> variable_name != "") {
//            v -> print();
//        }
        if(v != nullptr && v -> variable_name != "" && v -> get_expression() != nullptr && v -> get_expression() -> unparseToString() != index -> change_expr -> unparseToString()) {
            SgExpression* expr = v -> get_expression();
            initializer_list.push_back(expr);
            has_initializer = true;
            break;
        }
        if(dynamic_cast<LoopSegment*>(node) != nullptr) {
            CodeSegment* init_segment = dynamic_cast<LoopSegment*>(node) -> initializer;
            Variable* v = init_segment -> get_intermediate_variable(variable_name);
//            if(v != nullptr && v -> variable_name != "") {
//                v -> print();
//            }
            if(v != nullptr && v -> variable_name != "" && v -> get_expression() != nullptr && v -> get_expression() -> unparseToString() != index -> change_expr -> unparseToString()) {
                SgExpression* expr = v -> get_expression();
                initializer_list.push_back(expr);
                has_initializer = true;
            }
        }
        if(has_initializer) {
            return initializer_list.back();
        }

        for(CodeSegment* segment : dynamic_cast<CodeSegment*>(node) -> segment_list) {
            if(pre_node == segment) {
                break;
            }
            Variable* v = node -> get_intermediate_variable(variable_name);
//            if(v != nullptr && v -> variable_name != "") {
//                v -> print();
//            }
            if(v != nullptr && v->variable_name != "" && v -> get_expression() != nullptr && v -> get_expression() -> unparseToString() != index -> change_expr -> unparseToString()) {
                SgExpression* expr = v -> get_expression();
                initializer_list.push_back(expr);
                has_initializer = true;
            }
            if(has_initializer) {
                return initializer_list.back();
            }
        }
        pre_node = node;
        node = dynamic_cast<CodeSegment*>(node) -> parent_node;
    }

    return nullptr;
}
