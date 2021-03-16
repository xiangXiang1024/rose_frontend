//
// Created by root on 2021/3/9.
//

#ifndef FRONTEND_LOOPSEGMENT_H
#define FRONTEND_LOOPSEGMENT_H


#include "CodeSegment.h"

class LoopSegment : public CodeSegment{
public:
    CodeSegment* initializer = nullptr;
    vector<CodeSegment*> loop_segment_list;

    LoopSegment() {}

    /*LoopSegment(vector<SgStatement*> _statement_list) {
        for(SgStatement* s : _statement_list) {
            statement_list.push_back(s);
        }
    }*/

    /*LoopSegment(vector<SgStatement*> _statement_list, vector<Condition> _condition_list) {
        for(SgStatement* s : _statement_list) {
            statement_list.push_back(s);
        }

        for(Condition c : _condition_list) {
            condition_list.push_back(c);
        }
    }*/

    /*LoopSegment(vector<SgStatement*> _statement_list, vector<Condition> _condition_list, int _current_ptr) {
        for(SgStatement* s : _statement_list) {
            statement_list.push_back(s);
        }

        for(Condition c : _condition_list) {
            condition_list.push_back(c);
        }
        current_ptr = _current_ptr;
    }*/

    LoopSegment(vector<SgStatement *> _statement_list, vector<Condition> _condition_list,
            vector<Variable> _input_list, vector<Variable> _output_list,
            vector<Variable> _intermediate_list, int _current_ptr, Code* _parent_node) {
        for(SgStatement* s : _statement_list) {
            statement_list.push_back(s);
        }
        for(Condition c : _condition_list) {
            condition_list.push_back(c);
        }
        for(Variable v : _input_list) {
            Variable v2(v);
            input_list.push_back(v2);
        }
        for(Variable v : _output_list) {
            Variable v2(v);
            output_list.push_back(v2);
        }
        for(Variable v : _intermediate_list) {
            Variable v2(v);
            intermediate_list.push_back(v2);
        }
        current_ptr = _current_ptr;
        parent_node = _parent_node;
    }


    void analyze() {
//        cout << "analyze loop segment" << endl;
        SgStatement* statement = statement_list.front();
//        cout << "statement: " << statement -> unparseToString() << "\t|\t" << statement -> class_name() << endl;
        if(dynamic_cast<SgForStatement*>(statement) != nullptr) {
            handle_for_statement(dynamic_cast<SgForStatement*>(statement));
        }

        if(dynamic_cast<SgWhileStmt*>(statement) != nullptr) {
            handle_while_statment(dynamic_cast<SgWhileStmt*>(statement));
        }

        if(dynamic_cast<SgDoWhileStmt*>(statement) != nullptr) {
            handle_do_while_statement(dynamic_cast<SgDoWhileStmt*>(statement));
        }

//        cout << "loop_segment_list size: " << loop_segment_list.size() << endl;

        for (auto it = loop_segment_list.end()-1; it >= loop_segment_list.begin(); it--) {
            CodeSegment* segment = *it;
//            cout << "in loop to analyze segment: " << endl;
//            segment -> print();
            segment -> analyze();
            (*it) -> intermediate_list = segment -> intermediate_list;
        }

        /*cout << "loop body after analyze: " << endl;
        for(CodeSegment segment : loop_segment_list) {
            segment.print();
        }*/
    }

    void print() {
        cout << "====" << get_condition_str() << "==== loop" << endl;
//    cout << "loop_segment_list.size: loop_segment_list.size: " << loop_segment_list.size() << endl;
        for(SgStatement* s : statement_list) {
            cout << s -> unparseToString() << endl;
        }

        cout << "initializer: " << endl;
        if(initializer == nullptr) {
            cout << "initializer == nullptr" << endl;
        }else {
            initializer -> print();
        }

        if(loop_segment_list.size() > 0) {
            for(CodeSegment* c : loop_segment_list) {
                c -> print();
            }
            return;
        }/*else {
            cout << "loop_segment_list.size() == 0" << endl;
        }*/
    }

    void handle_for_statement(SgForStatement* statement);

    void handle_while_statment(SgWhileStmt* statement);

    void handle_do_while_statement(SgDoWhileStmt* statement);

    string get_ir_content(int tab_num) {
        string blank = common::get_line_start_blank(tab_num);
        stringstream ir_stream;
        ir_stream << blank << "TODO: loop ir content" << endl;
        for(CodeSegment* c : loop_segment_list) {
            ir_stream << c -> get_ir_content(tab_num+1) << endl;
        }
        return ir_stream.str();
    }
};


#endif //FRONTEND_LOOPSEGMENT_H
