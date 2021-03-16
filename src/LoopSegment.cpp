//
// Created by root on 2021/3/9.
//

#include "LoopSegment.h"

void LoopSegment::handle_for_statement(SgForStatement* statement) {
//    cout << "TODO handle_for_statement" << statement -> unparseToString() << endl;

    SgForInitStatement* init_block = statement -> get_for_init_stmt();
    SgStatement* test_statement = statement -> get_test();
    SgExpression* increment_expression = statement -> get_increment();
    SgStatement* loop_body = statement -> get_loop_body();

    if(init_block == nullptr) {
//        cout << "init_block == nullptr" << endl;
    }else {
//        cout << "init_block: " << "|\t" << init_block -> class_name() << endl;

        vector<SgStatement*> statement_list;
        for(SgNode* n : init_block -> get_traversalSuccessorContainer()) {
//            cout << n -> unparseToString() << "\t|\t" << n -> class_name() << endl;
            if(dynamic_cast<SgStatement*>(n) != nullptr) {
                statement_list.push_back(dynamic_cast<SgStatement*>(n));
            }
        }
        initializer = new CodeSegment(statement_list, condition_list, input_list, output_list, intermediate_list, 0, this);
    }
    /*if(test_statement == nullptr) {
        cout << "test_statement == nullptr" << endl;
    }else {
        cout << "test_statement: " << test_statement -> unparseToString() << endl;
    }
    if(increment_expression == nullptr) {
        cout << "increment_expression == nullptr" << endl;
    }else {
        cout << "increment_expression: " << increment_expression -> unparseToString() << endl;
    }
    if(loop_body == nullptr) {
        cout << "loop_body == nullptr" << endl;
    }else {
        cout << "loop_body: " << loop_body -> unparseToString() << endl;
    }*/

    initializer -> analyze();
//    cout << "after analyze initializer: " << endl;
//    initializer.print();

    if(test_statement != nullptr) {
//        cout << "test_statement -> class_name(): " << test_statement -> class_name() << "\t|\t" << test_statement -> unparseToString() << endl;
        vector<SgStatement*> _statement_list;
        CodeSegment* test_segment = new CodeSegment(_statement_list, condition_list, input_list, output_list, intermediate_list, 0, this);
        Condition test_break_condition(dynamic_cast<SgExprStatement*>(test_statement) -> get_expression(), true);
        test_segment -> add_condition(test_break_condition);
        test_segment -> is_break = true;
        loop_segment_list.push_back(test_segment);
//        cout << "test_segment: " << endl;
//        test_segment.print();
    }

    vector<SgStatement*> body_statement_list;
//    cout << "loop_body -> class_name(): " << loop_body -> class_name() << endl;
    SgStatementPtrList &stmt_list = dynamic_cast<SgBasicBlock*>(loop_body) -> get_statements();
    for (auto s : stmt_list) {
        body_statement_list.push_back(dynamic_cast<SgStatement*>(s));
    }

    if(increment_expression != nullptr) {
//        cout << "increment_expression -> class_name(): " << increment_expression -> class_name() << endl;
        SgExprStatement* expr_statement = new SgExprStatement(dynamic_cast<SgExpression*>(increment_expression));
        if(expr_statement -> get_startOfConstruct() == nullptr) {
            expr_statement->set_startOfConstruct(increment_expression -> get_startOfConstruct());
            expr_statement->set_parent(statement);
        }
        body_statement_list.push_back(expr_statement);
    }

    CodeSegment* body = new CodeSegment(body_statement_list, condition_list, input_list, output_list, intermediate_list, 0, this);
    if(test_statement != nullptr) {
        Condition test_pass_condition(dynamic_cast<SgExprStatement*>(test_statement) -> get_expression());
        body -> add_condition(test_pass_condition);
    }

    loop_segment_list.push_back(body);

    /*cout << "test body segment: " << endl;
    for(SgStatement* s : body -> statement_list) {
        cout << s -> unparseToString() << endl;
    }
    cout << endl;*/
}

/*
void LoopSegment::print() {
    cout << "====" << get_condition_str() << "==== loop" << endl;
//    cout << "loop_segment_list.size: loop_segment_list.size: " << loop_segment_list.size() << endl;
    for(SgStatement* s : statement_list) {
        cout << s -> unparseToString() << endl;
    }

    cout << "TODO print loop" << endl;

    cout << "initializer: " << endl;
    initializer.print();

    if(loop_segment_list.size() > 0) {
        for(CodeSegment c : loop_segment_list) {
            c.print();
        }
        return;
    }
}
*/
void LoopSegment::handle_while_statment(SgWhileStmt* statement){
    SgStatement* test_statement = statement->get_condition();
    SgStatement* loop_body = statement->get_body();

    if(test_statement != nullptr) {
          vector<SgStatement*> _statement_list;
          CodeSegment* test_segment = new CodeSegment(_statement_list, condition_list, input_list, output_list, intermediate_list, 0, this);
          Condition test_break_condition(dynamic_cast<SgExprStatement*>(test_statement) -> get_expression(), true);
          test_segment -> add_condition(test_break_condition);
          test_segment -> is_break = true;
          loop_segment_list.push_back(test_segment);
    }

    vector<SgStatement*> body_statement_list;
    SgStatementPtrList &stmt_list = dynamic_cast<SgBasicBlock*>(loop_body) -> get_statements();
    for (auto s : stmt_list) {
        body_statement_list.push_back(dynamic_cast<SgStatement*>(s));
    }

    CodeSegment* body = new CodeSegment(body_statement_list, condition_list, input_list, output_list, intermediate_list, 0, this);
    if(test_statement != nullptr) {
        Condition test_pass_condition(dynamic_cast<SgExprStatement*>(test_statement) -> get_expression());
        body -> add_condition(test_pass_condition);
    }

    loop_segment_list.push_back(body);
}

void LoopSegment::handle_do_while_statement(SgDoWhileStmt* statement){
    SgStatement* test_statement = statement->get_condition();
    SgStatement* loop_body = statement->get_body();

    vector<SgStatement*> body_statement_list;
    SgStatementPtrList &stmt_list = dynamic_cast<SgBasicBlock*>(loop_body) -> get_statements();
    for (auto s : stmt_list) {
        body_statement_list.push_back(dynamic_cast<SgStatement*>(s));
    }

    CodeSegment* body = new CodeSegment(body_statement_list, condition_list, input_list, output_list, intermediate_list, 0, this);

    loop_segment_list.push_back(body);

    if(test_statement != nullptr) {
      vector<SgStatement*> _statement_list;
      CodeSegment* test_segment = new CodeSegment(_statement_list, condition_list, input_list, output_list, intermediate_list, 0, this);
      Condition test_break_condition(dynamic_cast<SgExprStatement*>(test_statement) -> get_expression(), true);
      test_segment -> add_condition(test_break_condition);
      test_segment -> is_break = true;
      loop_segment_list.push_back(test_segment);
    }
}