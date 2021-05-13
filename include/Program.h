//
// Created by root on 2021/3/5.
//

#ifndef FRONTEND_PROGRAM_H
#define FRONTEND_PROGRAM_H

#endif //FRONTEND_PROGRAM_H

#include "Function.h"
#include "rose.h"
#include <map>
#include <vector>
#include "../cjson_lib/CJsonObject.hpp"

using namespace std;

class Program : public Code {
public:
    string file_path;
    string file_name;
    vector<Function*> function_list;
    SgGlobal *global;
    map<string,string> func_call_map;

    void analyze() {
        cout << "analyze cpp source file: " <<  file_path << endl << endl;

        vector<SgNode*> sgnode_list = global -> get_traversalSuccessorContainer();
        for(auto sgnode : sgnode_list) {
            if(nullptr == sgnode) {
                continue;
            }
            if(dynamic_cast<SgLocatedNode *>(sgnode) -> getFilenameString() != file_path) {
                continue;
            }
            if(auto* func_declaration = dynamic_cast<SgFunctionDeclaration*>(sgnode)) {
              if(func_declaration->get_definition()==nullptr){
                cout << "meet nullptr SgFunctionDefinition" << endl;
                continue;
              }
                Function* function = new Function(func_declaration,&func_call_map);
                function_list.push_back(function);
                function -> analyze();
            }
        }

        for(auto* function : function_list){
          if(check_recursion(function->func_name)){
            function->is_recursive = true;
            function->recursion_segment = *(new CodeSegment(function->segment));
            /*
            for(int i=0;i<function->segment.segment_list.size();i++){
              cout<<&*(function->recursion_segment.segment_list[i])<<endl;
              cout<<&*(function->segment.segment_list[i])<<endl;
            }
            */
            replace(&function->recursion_segment,function->func_name);
          }
        }
/*
        for(auto* function : function_list){
          cout << "**** func: " << function->func_name << " ****" << endl;
            function->segment.print();
        }
*/

/*
        for(auto* function : function_list){
          cout << "**** func: " << function->func_name << " ****" << endl;
          if(function->is_recursive){
            function->recursion_segment.print();
          }else{
            function->segment.print();
          }
        }
*/
        for(int i = 0 ; i < function_list.size() ; i++) {
          Function* function = function_list.at(i);
          if(function->is_recursive){
            Function* fun = new Function(function);
            generate_statement_list(&(function->recursion_segment),fun);
            cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" <<endl;
            for(SgStatement* s : fun->statement_list) {
              cout << s -> unparseToString() << endl;
            }
            fun->segment = CodeSegment(fun->statement_list, nullptr, fun->func_name, fun->func_call_map);
            fun->segment.input_list = fun->input_list;
            fun->segment.intermediate_list = fun->intermediate_list;
            fun->segment.analyze();
            fun->output_list = fun->segment.output_list;
            fun->line_info = fun->segment.line_info;
            sort(fun->line_info.begin(),fun->line_info.end());
            fun->is_recursive = true;
            function_list[i] = fun;
          }
        }

    }

    Program(const string &_name, SgGlobal *_global) : file_path(_name), global(_global) {
        // TODO file_name
    }

    void generate_statement_list(CodeSegment* cs,Function* func){
      if(cs->segment_list.size()==0){
        for (auto s : cs->statement_list) {
          func->statement_list.push_back(s);
        }
      }else{
        for(auto cs_in_cs : cs->segment_list){
          generate_statement_list(cs_in_cs,func);
        }
      }
    }

    string get_ir_content() {
//        neb::CJsonObject program_json("");
//        program_json.Add("program_name", file_path);
      neb::CJsonObject res("");
      res.Add("file_name",file_path.substr(file_path.find_last_of('/')+1,file_path.size()));
      res.Add("file_path",file_path);
      res.AddEmptySubArray("functions");
      for(int i = 0 ; i < function_list.size() ; i++) {
        Function* f = function_list.at(i);
        neb::CJsonObject func_json("");
        if(f->is_recursive){
          func_json.Add("type","recursive");
        }else{
          func_json.Add("type","normal");
        }
        func_json.AddEmptySubArray("line_no");
        func_json["line_no"].Add(f->segment.get_begin_line_no());
        func_json["line_no"].Add(f->segment.get_end_line_no());
        func_json.Add("function_name",f->func_name);
        neb::CJsonObject input_json("");
        for(Variable* v : f->input_list){
          input_json.Add(v->variable_name,v->type->unparseToString());
        }
        func_json.Add("input",input_json);
        neb::CJsonObject variables_json("");
        for(Variable* v : f->intermediate_list){
          variables_json.Add(v->variable_name,v->type->unparseToString());
        }
        func_json.Add("variables",variables_json);
        neb::CJsonObject output_json("");
        for(Variable* v : f->output_list){
          output_json.Add(v->variable_name,v->type->unparseToString());
        }
        func_json.Add("output",output_json);
        func_json.AddEmptySubArray("sections");
            f -> make_ir_content(func_json);
            res["functions"].Add(func_json);
        }
        cout << res.ToFormattedString() <<endl;
        cout << res.ToString() << endl;
        return res.ToFormattedString();
/*
        stringstream ir_stream;
        for(int i = 0 ; i < function_list.size() ; i++) {
            Function* f = function_list.at(i);
            ir_stream << f -> get_ir_content(tab_num+1);
            if(i != function_list.size() - 1) {
                ir_stream << "," << endl;
            }
        }

        cout << "---- ir content ----" << endl;
        cout << ir_stream.str() << endl;

        return ir_stream.str();

        /*cout << endl << "program json: " << endl;
        cout << program_json.ToFormattedString() << endl;
        return program_json.ToFormattedString();*/

        /*string blank = common::get_line_start_blank(tab_num);
        stringstream ir_stream;
        ir_stream << blank << "program ir content" << endl;
        for(Function f : function_list) {
            ir_stream << blank << "{" << endl;
            ir_stream << f.get_ir_content(tab_num+1) << endl;
            ir_stream << blank << "}";
        }
        return ir_stream.str();*/
    }

    bool check_recursion(string func_name){
      map<string, string>::iterator iter;
      iter = func_call_map.find(func_name);
      if(iter==func_call_map.end()){
        return false;
      }
      while(iter!=func_call_map.end()&&iter->second!=func_name){
        iter = func_call_map.find(iter->second);
      }
      if(iter==func_call_map.end()){
        return false;
      }else{
        return true;
      }
    }

    void replace(CodeSegment* code_segment, string terminal_name){
      for(int i=0;i<code_segment->segment_list.size();i++){
        CodeSegment* c_s = code_segment->segment_list.at(i);
        if(c_s->is_func_call&&c_s->ref_func_name!=terminal_name){
          for(auto func : function_list){
            if(func->func_name==c_s->ref_func_name){
              //cout<<"resc func"<<endl;
              SgStatement* statement=c_s->statement_list.front();
              //cout<<statement->unparseToString()<<endl;

              SgFunctionCallExp* exp = Program::has_func_call(statement);

              vector<Variable*> ref_list;
              generate_ref_variable_list(ref_list,exp);

              vector<Variable*>parameter_list;
              generate_func_parameter_list(parameter_list,exp->getAssociatedFunctionDeclaration());

              vector<SgStatement*> assignment_statement_list;
              SgFunctionDeclaration* func_dec=func->function_declaration;
              SgFunctionDefinition* func_def=func_dec->get_definition();
              SgBasicBlock* block=func_def->get_body();
              generate_assignment_statement_list(assignment_statement_list,ref_list,parameter_list,block);
              /*
              cout<<"new statements: "<<endl;
              for(SgStatement* s:assignment_statement_list){
                cout<<s->unparseToString()<<endl;
              }
            */


              *c_s = *(new CodeSegment(func->segment));
              CodeSegment* pre = code_segment->segment_list.at(i-1);
              for(SgStatement* s:assignment_statement_list){
                pre->statement_list.push_back(s);
              }
              replace(c_s,terminal_name);
            }
          }
        }
      }
      if(code_segment->segment_list.size()==0){
        if(code_segment->is_func_call&&code_segment->ref_func_name!=terminal_name){
          for(auto func : function_list){
            if(func->func_name==code_segment->ref_func_name){
              *code_segment = *(new CodeSegment(func->segment));
            }
          }
        }
      }
    }



    void generate_assignment_statement_list(vector<SgStatement*>&statement_list,vector<Variable*> ref_variable_list,vector<Variable*> func_parameter_list,SgNode* node);

    void generate_ref_variable_list(vector<Variable*> &ref_list,SgFunctionCallExp* exp);

    void generate_func_parameter_list(vector<Variable*> &parameter_list,SgFunctionDeclaration* func_declaration);

    SgFunctionCallExp* has_func_call(SgNode* statement) ;


};

