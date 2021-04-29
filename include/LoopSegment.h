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

    LoopSegment(vector<SgStatement *> _statement_list, vector<Condition> _condition_list,
            vector<Variable*> _input_list, vector<Variable*> _output_list,
            vector<Variable*> _intermediate_list, int _current_ptr, Code* _parent_node, string _func_name, map<string,string>* _func_call_map) {
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
        statement_list = _statement_list;
        condition_list = _condition_list;
        input_list = _input_list;
        intermediate_list = _intermediate_list;
        output_list = _output_list;
        current_ptr = _current_ptr;
        parent_node = _parent_node;
        func_name = _func_name;
        func_call_map = _func_call_map;
    }


    void analyze() {
        cout << "analyze loop segment" << endl;
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

        if(has_array_operation) {
            cout << "segment contains array operation" << endl;
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

    neb::CJsonObject get_ir_content() {
        neb::CJsonObject general_json("");
        general_json.Add("type",type);
        general_json.AddEmptySubArray("line_no");
        general_json["line_no"].Add(get_begin_line_no());
        general_json["line_no"].Add(get_end_line_no());
        neb::CJsonObject input_json("");
        for(Variable* v : input_list){
          input_json.Add(v->variable_name,v->type->unparseToString());
        }
        general_json.Add("input",input_json);
        neb::CJsonObject output_json("");
        for(Variable* v : output_list){
          output_json.Add(v->variable_name,v->type->unparseToString());
        }
        general_json.Add("output",output_json);
        general_json.AddEmptySubArray("variables");
        neb::CJsonObject variables_json("");
        if(initializer!=nullptr){
          for(Variable* v : initializer->intermediate_list){
            if(v->variable_name!=""){
              variables_json.Add(v->variable_name,v->type->unparseToString());
            }
          }
        }
        general_json["variables"].Add(variables_json);
        general_json.AddEmptySubArray("initialzie");
        neb::CJsonObject initialzie_json("");
        if(initializer!=nullptr){
          for(Variable* v : initializer->intermediate_list){
            if(v->variable_name!=""){
              initialzie_json.Add(v->variable_name,v->expression_str);
            }
          }
        }
        general_json["initialzie"].Add(initialzie_json);
        general_json.AddEmptySubArray("traces");
        auto ls_1 = loop_segment_list.end()-1;
        auto ls_0 = loop_segment_list.begin();
        CodeSegment* segment_1 = *ls_1;
        CodeSegment* segment_0 = *ls_0;
        CodeSegment* segment;
        if(segment_1->statement_list.size()!=0){
          segment = segment_1;
        }else{
          segment = segment_0;
        }
        if(segment->traces.size()!=0){
          if(segment_0->statement_list.size()==0){
            neb::CJsonObject test_json("");
            test_json.Add("constraint",segment_0->get_condition_str());
            test_json.Add("break",segment_0->is_break);
            test_json.Add("continue",segment_0->is_continue);
            test_json.AddEmptySubArray("content");
            test_json["content"].Add(segment_0->get_ir_content());
            general_json["traces"].Add(test_json);
          }
          for(CodeSegment* trace : segment->traces){
              neb::CJsonObject trace_json("");
              trace_json.Add("constraint",trace->get_condition_str());
              trace_json.Add("break",trace->is_break);
              trace_json.Add("continue",trace->is_continue);
              trace_json.AddEmptySubArray("content");
              if(trace->segment_list.size()==0){
                trace_json["content"].Add(trace->get_ir_content());
              }else{
                for(CodeSegment* cs_in_trace : trace->segment_list){
                  trace_json["content"].Add(cs_in_trace->get_ir_content());
                }
              }
              general_json["traces"].Add(trace_json);
          }
          if(segment_1->statement_list.size()==0){
            neb::CJsonObject test_json("");
            test_json.Add("constraint",segment_1->get_condition_str());
            test_json.Add("break",segment_1->is_break);
            test_json.Add("continue",segment_1->is_continue);
            test_json.AddEmptySubArray("content");
            test_json["content"].Add(segment_1->get_ir_content());
            general_json["traces"].Add(test_json);
          }
        }
        general_json.AddEmptySubArray("irrelevant");
        for(int no : unrelated_lines){
            general_json["irrelevant"].Add(no);
        }
        return general_json;
      }
/*
    string get_ir_content(int tab_num) {
        string blank = common::get_line_start_blank(tab_num);
        stringstream ir_stream;
        ir_stream << blank << "TODO: loop ir content" << endl;
        for(CodeSegment* c : loop_segment_list) {
            ir_stream << c -> get_ir_content(tab_num+1) << endl;
        }
        return ir_stream.str();
    }
    */
};


#endif //FRONTEND_LOOPSEGMENT_H
