//
// Created by root on 2021/3/9.
//

#include "CodeSegment.h"
#include "../cjson_lib/CJsonObject.hpp"

#ifndef FRONTEND_FUNCTION_H
#define FRONTEND_FUNCTION_H

using namespace std;

class Function : public Code {
public:
    string func_name;
    CodeSegment segment;
    CodeSegment recursion_segment;
    bool is_recursive = false;
    vector<SgStatement*> statement_list;
    SgFunctionDeclaration* function_declaration;
    map<string,string>* func_call_map;

    Function(SgFunctionDeclaration* _function_declaration, map<string,string>* _map_func_call_map) : function_declaration(_function_declaration),func_call_map(_map_func_call_map) {
        func_name = function_declaration -> get_name().getString();
    }

    Function(Function* func){
      func_name = func->func_name;
      input_list = func->input_list;
      intermediate_list = func->intermediate_list;
      func_call_map = func->func_call_map;
    }

    void analyze() {
        cout << "analyze function " << func_name << endl;

        generate_input_list();
        /*
        cout << "inputs: ";
        for(Variable v : input_list) {
            cout << v.variable_name << "(" << v.type -> unparseToCompleteString() << ")    ";
        }
        cout << endl << endl;
        */

        generate_statement_list();
        /*
        cout << "statement:" << endl;
        for (SgStatement* s : statement_list) {
            cout << s->unparseToString() << "\t\t|\t\t" << s->class_name() << endl;
        }
        cout << endl;
        */

        segment = CodeSegment(statement_list, nullptr, func_name, func_call_map);
        segment.input_list = input_list;
        segment.intermediate_list = intermediate_list;
        segment.analyze();

        generate_output_list();
        /*cout << endl << "outputs: ";
        for(Variable v : output_list) {
            cout << v.variable_name << "(" << v.type -> unparseToCompleteString() << ")    ";
        }
        cout << endl << endl;
    */
        generate_line_info();
        /*cout << endl << "lines: ";
        for(int l : line_info) {
            cout << l << "    ";
        }
        cout << endl << endl;*/

        for(CodeSegment* cs : segment.segment_list){
          cout << endl << "outputs: ";
        for(Variable* v : cs->output_list) {
            cout << v->variable_name << "(" << v->type -> unparseToCompleteString() << ")    ";
        }
        cout << endl << endl;
        }
    };

    void make_ir_content(neb::CJsonObject& res) {
        /*string blank = common::get_line_start_blank(tab_num);
        stringstream ir_stream;
        ir_stream << blank << "function: " << func_name << " ir content" << endl;
        ir_stream << blank << "{" << endl;
        ir_stream << segment.get_ir_content(tab_num+1) << endl;
        ir_stream << blank << "}";*/
//        return ir_stream.str();
        if(segment.segment_list.size()!=0){
          for(CodeSegment* cs : segment.segment_list){
            neb::CJsonObject general_json("");
            general_json.AddEmptySubArray("line_no");
            general_json["line_no"].Add(cs->get_begin_line_no());
            general_json["line_no"].Add(cs->get_end_line_no());
            general_json.AddEmptySubArray("traces");
            if(cs->traces.size()==0){
              neb::CJsonObject trace_json("");
              trace_json.Add("constraint",cs->get_condition_str());
              trace_json.Add("break",cs->is_break);
              trace_json.Add("continue",cs->is_continue);
              if(cs->segment_list.size()==0){
                if(cs->type!="loop"){
                  bool find = false;
                  string double_array = "[";
                  /*
                  if(cs->statement_list.size()!=0){
                    for(SgStatement* statement:cs->statement_list){
                    if(dynamic_cast<SgExprStatement*>(statement) != nullptr) {
                      vector<SgNode*> node_list = statement -> get_traversalSuccessorContainer();
                      for(SgNode* n : node_list) {
                        if(dynamic_cast<SgAssignOp*>(n)!=nullptr){
                          SgAssignOp* assign_op = dynamic_cast<SgAssignOp*>(n);
                        SgNode* var_node = assign_op -> get_traversalSuccessorByIndex(0);
                        if(dynamic_cast<SgVarRefExp*>(var_node) != nullptr) {
                        SgVarRefExp* var_ref = dynamic_cast<SgVarRefExp*>(var_node);
                        Variable* v = new Variable(var_ref);
                        SgNode* expression_node = assign_op -> get_traversalSuccessorContainer().back();
                        SgExpression* expression = dynamic_cast<SgExpression*>(expression_node);
                        double_array.append("[");
                        double_array.append(v->variable_name);
                        double_array.append(",");
                        if(expression!=nullptr){
                          double_array.append(expression->unparseToString());
                        }
                        double_array.append("]");
                        double_array.append(",");
                        find = true;
                        }
                        }

                      }
                    }
                  }
                  if(find){
                    double_array.erase(double_array.end()-1);
                  }
                    }
                    */
                    for(Variable* v : cs->intermediate_list){
                      double_array.append("[");
                      double_array.append(v->variable_name);
                      double_array.append(",");
                      double_array.append(v->expression_str);
                      double_array.append("]");
                      double_array.append(",");
                      find = true;
                    }
                    if(find){
                      double_array.erase(double_array.end()-1);
                    }
                    double_array.append("]");
                    trace_json.Add("content",double_array);
                }else{
                  trace_json.AddEmptySubArray("content");
                  trace_json["content"].Add(cs->get_ir_content());
                }
              }else{
                trace_json.AddEmptySubArray("content");
                for(CodeSegment* cs_in_cs : cs->segment_list){
                  trace_json["content"].Add(cs_in_cs->get_ir_content());
                }
              }
              if(cs->critical_string!=""){
                trace_json.Add("return",cs->critical_string);
              }
              general_json["traces"].Add(trace_json);
            }else{
              for(CodeSegment* trace : cs->traces){
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
                if(trace->critical_string!=""){
                  trace_json.Add("return",trace->critical_string);
                }
                general_json["traces"].Add(trace_json);
              }
            }
            general_json.AddEmptySubArray("irrelevant");
            for(int no : cs->unrelated_lines){
              general_json["irrelevant"].Add(no);
            }
            res["sections"].Add(general_json);
          }
        }else{
          neb::CJsonObject general_json("");
          general_json.AddEmptySubArray("line_no");
          general_json["line_no"].Add(segment.get_begin_line_no());
          general_json["line_no"].Add(segment.get_end_line_no());
          general_json.AddEmptySubArray("traces");
          if(segment.traces.size()==0){
            neb::CJsonObject trace_json("");
            trace_json.Add("constraint",segment.get_condition_str());
            trace_json.Add("break",segment.is_break);
            trace_json.Add("continue",segment.is_continue);
            if(segment.segment_list.size()==0){
              if(segment.type!="loop"){
                bool find = false;
                  string double_array = "[";
                  /*
                  if(segment.statement_list.size()!=0){
                    for(SgStatement* statement:segment.statement_list){
                    if(dynamic_cast<SgExprStatement*>(statement) != nullptr) {
                      vector<SgNode*> node_list = statement -> get_traversalSuccessorContainer();
                      for(SgNode* n : node_list) {
                        if(dynamic_cast<SgAssignOp*>(n)!=nullptr){
                          SgAssignOp* assign_op = dynamic_cast<SgAssignOp*>(n);
                        SgNode* var_node = assign_op -> get_traversalSuccessorByIndex(0);
                        if(dynamic_cast<SgVarRefExp*>(var_node) != nullptr) {
                        SgVarRefExp* var_ref = dynamic_cast<SgVarRefExp*>(var_node);
                        Variable* v = new Variable(var_ref);
                        SgNode* expression_node = assign_op -> get_traversalSuccessorContainer().back();
                        SgExpression* expression = dynamic_cast<SgExpression*>(expression_node);
                        double_array.append("[");
                        double_array.append(v->variable_name);
                        double_array.append(",");
                        if(expression!=nullptr){
                          double_array.append(expression->unparseToString());
                        }
                        double_array.append("]");
                        double_array.append(",");
                        find = true;
                        }
                        }

                      }
                    }
                  }
                  if(find){
                    double_array.erase(double_array.end()-1);
                  }
                  }
                  */
                  for(Variable* v : segment.intermediate_list){
                    double_array.append("[");
                    double_array.append(v->variable_name);
                    double_array.append(",");
                    double_array.append(v->expression_str);
                    double_array.append("]");
                    double_array.append(",");
                    find = true;
                  }
                  if(find){
                    double_array.erase(double_array.end()-1);
                  }
                  double_array.append("]");
                  trace_json.Add("content",double_array);
              }else{
                trace_json.AddEmptySubArray("content");
                trace_json["content"].Add(segment.get_ir_content());
              }
            }else{
              trace_json.AddEmptySubArray("content");
              for(CodeSegment* cs_in_cs : segment.segment_list){
                  trace_json["content"].Add(cs_in_cs->get_ir_content());
              }
            }
            if(segment.critical_string!=""){
              trace_json.Add("return",segment.critical_string);
            }
            general_json["traces"].Add(trace_json);
          }else{
            for(CodeSegment* trace : segment.traces){
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
              if(trace->critical_string!=""){
                trace_json.Add("return",trace->critical_string);
              }
              general_json["traces"].Add(trace_json);
            }
          }
          general_json.AddEmptySubArray("irrelevant");
          for(int no : segment.unrelated_lines){
            general_json["irrelevant"].Add(no);
          }
          res["sections"].Add(general_json);
        }
        /*
        neb::CJsonObject function_json("");
        function_json.Add("program_name", "TODO program name");
        function_json.Add("function_name", func_name);

        function_json.AddEmptySubObject("variables");
        map<string, string> variable_map;
        for(Variable* v : input_list) {
            variable_map[v -> variable_name] = v -> type -> unparseToString();
        }
        for(Variable* v : output_list) {
            variable_map[v -> variable_name] = v -> type -> unparseToString();
        }
        auto iter = variable_map.begin();
        while(iter != variable_map.end()) {
            function_json["variables"].Add(iter -> first, iter -> second);
            iter++;
        }

        function_json.AddEmptySubArray("input_variables");
        for(Variable* v : input_list) {
            function_json["input_variables"].Add(v -> variable_name);
        }

        // TODO
        function_json.AddEmptySubObject("return");

        return function_json.ToFormattedString();
        */
    }

private:
    void generate_input_list();

    void generate_statement_list();

    void generate_output_list();

    void generate_line_info();
};

#endif //FRONTEND_FUNCTION_H


