//
// Created by root on 2021/3/9.
//

#ifndef FRONTEND_CODESEGMENT_H
#define FRONTEND_CODESEGMENT_H


#include "Code.h"
#include "Condition.h"

class CodeSegment : public Code {
public:
    vector<SgStatement*> statement_list;
    vector<CodeSegment*> segment_list;
    vector<CodeSegment*> other_execute_path_list;
    vector<Condition> condition_list;
    int current_ptr = 0;
    bool is_break = false;
    bool is_continue = false;
    Code* parent_node = nullptr;
    bool has_array_operation = false;

    CodeSegment() {}

    CodeSegment(vector<SgStatement*> _statement_list, Code* _parent_node) {
        for(SgStatement* s : _statement_list) {
            statement_list.push_back(s);
        }
        parent_node = _parent_node;
    }

    CodeSegment(vector<SgStatement*> _statement_list, vector<Condition> _condition_list,
                vector<Variable*> _input_list, vector<Variable*> _output_list,
                vector<Variable*> _intermediate_list, int _current_ptr, Code* _parent_node) {
        statement_list = _statement_list;
        condition_list = _condition_list;
        input_list = _input_list;
        intermediate_list = _intermediate_list;
        output_list = _output_list;
        /*for(SgStatement* s : _statement_list) {
            statement_list.push_back(s);
        }
        for(Condition c : _condition_list) {
            condition_list.push_back(c);
        }
        for(Variable* v : _input_list) {
            Variable v2(*v);
            input_list.push_back(&v2);
        }
        for(Variable* v : _output_list) {
            Variable v2(*v);
            output_list.push_back(&v2);
        }
        for(Variable* v : _intermediate_list) {
            Variable v2(*v);
            intermediate_list.push_back(&v2);
        }*/
        current_ptr = _current_ptr;
        parent_node = _parent_node;
    }

    void add_condition(Condition condition);

    virtual void print() {
        cout << "====" << get_condition_str() << "====" << endl;
        cout << "statement: " << endl;
        for(SgStatement* s : statement_list) {
            cout << s -> unparseToString() << endl;
        }

        cout << "input_list: " << endl;
        for(Variable* v : input_list) {
            v -> print();
        }
        cout << "intermediate_list: " << endl;
        for(Variable* v : intermediate_list) {
            v -> print();
        }
        cout << "output_list: " << endl;
        for(Variable* v : output_list) {
            v -> print();
        }
        if(is_break) {
            cout << "is break path" << endl;
        }
        if(is_continue) {
            cout << "is continue path" << endl;
        }
        if(has_array_operation) {
            cout << "segment contains array operation" << endl;
        }
        cout << "========" << endl;

        if(segment_list.size() > 0) {
            cout << "segment list: " << endl;
            for(CodeSegment* c : segment_list) {
                c -> print();
            }
            return;
        }/*else {
            cout << "segment_list.size() == 0" << endl;
        }*/

        if(other_execute_path_list.size() > 0) {
            for(CodeSegment* c : other_execute_path_list) {
                c -> print();
            }
        }
    }

    void analyze() {
        if(statement_list.size() == 0) {
            return;
        }
        /*cout << "analyze code segment" << endl;
        cout << "condition: " << get_condition_str() << endl;
        cout << "code:" << endl;
        for(SgStatement* s : statement_list) {
            cout << s -> unparseToString() << endl;
        }*/

        bool do_partition = partition();

        /*cout << "after partition:" << endl;
        for(CodeSegment* segment : segment_list) {
            segment -> print();
        }*/

        if(do_partition) {
            // TODO
            for (auto it = segment_list.end()-1; it >= segment_list.begin(); it--) {
                CodeSegment* segment = *it;
                segment -> analyze();
                add_output(segment -> output_list);
                add_input(segment -> input_list);
                if(it != segment_list.begin()) {
                    CodeSegment* next_segment = *(it-1);
                    next_segment -> add_output(segment -> input_list);
                }
            }
        }else {
            for( ; current_ptr < statement_list.size() ; current_ptr++) {
                SgStatement* statement = statement_list.at(current_ptr);
                handle_statement(statement);
            }

            for(CodeSegment* segment : other_execute_path_list) {
                segment -> analyze();
                add_output(segment -> output_list);
                add_input(segment -> input_list);
            }
        }

        for(Variable* v : output_list) {
            Variable* tmp = get_intermediate_variable(v -> variable_name);
            if(tmp -> get_expression() != nullptr) {
                add_output(tmp);
            }
        }

        calculate_input();

        // TODO check array
        vector<ArrayVariable*> array_list;
        for(Variable* v : intermediate_list) {
            if(v -> is_array) {
//                cout << "segment has array" << endl;
//                v.print();
                if(v -> get_expression() != nullptr) {
                    has_array_operation = true;
                    if(dynamic_cast<ArrayVariable*>(v) == nullptr) {
                        cout << v -> variable_name << " can't be casted to array variable*" << endl;
                    }else {
                        array_list.push_back(dynamic_cast<ArrayVariable*>(v));
                    }
                }
            }
        }
        // TODO do array analyze
        for(ArrayVariable* array : array_list) {
            handle_array(array);
        }
    };

    bool partition();

    void handle_statement(SgStatement* statement);

    void handle_expr_statement(SgExprStatement* statement);

    SgExpression* handle_expression(SgExpression* expression);

    void handle_variable_declaration(SgVariableDeclaration* statement);

    void handle_if_statement(SgIfStmt* statement);

    void handle_return_statement(SgReturnStmt* statement);

    void handle_break_statement(SgBreakStmt* statement);

    void handle_continue_statement(SgContinueStmt* statement);

    string get_condition_str();

    string get_ir_content(int tab_num) {
        string blank = common::get_line_start_blank(tab_num);
        stringstream ir_stream;
        if(segment_list.size() > 0) {
            for(CodeSegment* segment : segment_list) {
                ir_stream << blank << segment -> get_ir_content(tab_num+1) << endl;
            }
        }else {
            ir_stream << blank << "TODO: segment ir content" << endl;
            if(other_execute_path_list.size() > 0) {
                for(CodeSegment* segment : other_execute_path_list) {
                    ir_stream << blank << segment -> get_ir_content(tab_num+1) << endl;
                }
            }
        }
        return ir_stream.str();
    }

    void calculate_input();

private:
    SgExpression* handle_binary_op(SgBinaryOp* binary_op);

    SgFunctionCallExp* has_func_call(SgNode* statement);

    vector<Variable*> get_ref_variable_list(SgExpression* expression);

    vector<SgExpression*> get_var_ref(SgExpression* expression);

    void handle_array(ArrayVariable* array);

    Condition* get_condition_by_variable(string variable_name);

    SgExpression* get_initializer_expr(string variable_name, IndexVariable* index);
};


#endif //FRONTEND_CODESEGMENT_H
