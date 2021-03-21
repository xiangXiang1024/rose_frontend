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


        for(auto* function : function_list){
          cout << "**** func: " << function->func_name << " ****" << endl;
          if(function->is_recursive){
            function->recursion_segment.print();
          }else{
            function->segment.print();
          }
        }

    }

    Program(const string &_name, SgGlobal *_global) : file_path(_name), global(_global) {
        // TODO file_name
    }

    string get_ir_content(int tab_num) {
//        neb::CJsonObject program_json("");
//        program_json.Add("program_name", file_path);

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
      for(auto c_s : code_segment->segment_list){
        if(c_s->is_func_call&&c_s->ref_func_name!=terminal_name){
          for(auto func : function_list){
            if(func->func_name==c_s->ref_func_name){
              *c_s = func->segment;
              replace(c_s,terminal_name);
            }
          }
        }
      }
      if(code_segment->segment_list.size()==0){
        if(code_segment->is_func_call&&code_segment->ref_func_name!=terminal_name){
          for(auto func : function_list){
            if(func->func_name==code_segment->ref_func_name){
              *code_segment = func->segment;
            }
          }
        }
      }
    }
};
