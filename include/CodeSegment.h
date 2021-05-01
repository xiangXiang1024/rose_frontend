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
      vector<CodeSegment*> traces;
      vector<Condition> condition_list;
      int current_ptr = 0;
      bool is_break = false;
      bool is_continue = false;
      bool has_array_operation = false;
      bool is_func_call = false;
      string ref_func_name = "";
      string func_name = "";
      map<string,string>* func_call_map;
      string type = "procedure";

      CodeSegment() {}

      CodeSegment(CodeSegment& c_s){
        this->statement_list = c_s.statement_list;
        for(auto cs : c_s.segment_list){
          this->segment_list.push_back(new CodeSegment(*cs));
        }
        this->traces = c_s.traces;
        this->condition_list = c_s.condition_list;
        this->current_ptr = c_s.current_ptr;
        this->is_break = c_s.is_break;
        this->is_continue = c_s.is_continue;
        this->parent_node = c_s.parent_node;
        this->has_array_operation = c_s.has_array_operation;
        this->is_func_call = c_s.is_func_call;
        this->ref_func_name = c_s.ref_func_name;
        this->func_name = c_s.func_name;
        this->func_call_map = c_s.func_call_map;
      }

      CodeSegment(vector<SgStatement*> _statement_list, Code* _parent_node, string _func_name, map<string,string>* _func_call_map) {
          for(SgStatement* s : _statement_list) {
              statement_list.push_back(s);
          }
          parent_node = _parent_node;
          func_name = _func_name;
          func_call_map = _func_call_map;
      }

      CodeSegment(vector<SgStatement*> _statement_list, vector<Condition> _condition_list,
                  vector<Variable*> _input_list, vector<Variable*> _output_list,
                  vector<Variable*> _intermediate_list, int _current_ptr, Code* _parent_node, string _func_name, map<string,string>* _func_call_map) {
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
          func_name = _func_name;
          func_call_map = _func_call_map;
      }

      CodeSegment(vector<Condition> _condition_list,
                  vector<Variable*> _input_list, vector<Variable*> _output_list,
                  vector<Variable*> _intermediate_list, int _current_ptr, Code* _parent_node, string _func_name, map<string,string>* _func_call_map) {
          condition_list = _condition_list;
          input_list = _input_list;
          intermediate_list = _intermediate_list;
          output_list = _output_list;
          current_ptr = _current_ptr;
          parent_node = _parent_node;
          func_name = _func_name;
          func_call_map = _func_call_map;
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
          cout << "unrelated_lines: " << endl;
          for(int n : unrelated_lines) {
              cout << n << " ";
          }
          cout << endl;
          if(is_break) {
              cout << "is break path" << endl;
          }
          if(is_continue) {
              cout << "is continue path" << endl;
          }
          if(has_array_operation) {
              cout << "segment contains array operation" << endl;
          }
          if(is_func_call){
            cout << "is func call path" << endl;
          }
          /*
          if(segment_list.size() > 0) {
            cout << "===========================" << endl;
              cout << "segment list: " << endl;
              for(CodeSegment* c : segment_list) {
                  c -> print();
              }
              cout << "===========================" << endl;
          }
          */
          /*else {
              cout << "segment_list.size() == 0" << endl;
          }*/
          /*
          if(traces.size() > 0) {
            cout << "========" << endl;
            cout << "traces: " << endl;
              for(CodeSegment* c : traces) {
                  c -> print();
              }
              cout << "========" << endl;
          }
          */
      }

      void analyze() {
          if(statement_list.size() == 0) {
              return;
          }
/*
          cout << "analyze code segment" << endl;
          cout << "condition: " << get_condition_str() << endl;
          cout << "code:" << endl;
          for(SgStatement* s : statement_list) {
              cout << s -> unparseToString() << endl;
          }
*/
          bool do_partition = partition();

/*
          cout << "after partition:" << endl;
          for(CodeSegment* segment : segment_list) {
              segment -> print();
          }
*/
          if(do_partition) {
              // TODO
              for (auto it = segment_list.end()-1; it >= segment_list.begin(); it--) {
                  CodeSegment* segment = *it;
                  cout << "before analyze:" <<endl;
                  segment->print();
                  segment -> analyze();
                  cout << "after analyze:" <<endl;
                  segment->print();
                  add_output(segment -> output_list);
                  add_input(segment -> input_list);
                  /*
                  if(it != segment_list.begin()) {
                      CodeSegment* next_segment = *(it-1);
                      next_segment -> add_output(segment -> input_list);
                  }
                  */
              }
          }else {
              bool is_continue_statement = false;
              for( ; current_ptr < statement_list.size() ; current_ptr++) {
                  SgStatement* statement = statement_list.at(current_ptr);
                  handle_statement(statement);
                  if(dynamic_cast<SgIfStmt*>(statement) != nullptr||dynamic_cast<SgBreakStmt*>(statement) != nullptr||dynamic_cast<SgContinueStmt*>(statement) != nullptr||dynamic_cast<SgReturnStmt*>(statement) != nullptr){
                    if(dynamic_cast<SgContinueStmt*>(statement) != nullptr){
                      is_continue_statement = true;
                    }
                    break;
                  }
              }
              if(is_continue_statement){
                handle_statement(statement_list.at(current_ptr));
              }

              cout << traces.size()<<endl;

              for(CodeSegment* segment : traces) {
                  cout << "before analyze:" <<endl;
                  segment->print();
                  segment -> analyze();
                  cout << "after analyze:" <<endl;
                  segment->print();
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

      void handle_continue_statement(SgContinueStmt* statement){
        is_continue = true;
        while(statement_list.size() > current_ptr + 1) {
          statement_list.pop_back();
        }
        //cout << "ffffffffffffffffffffffffff" <<endl;
        Code* node = parent_node;
        while(critical_statement==nullptr&&node!=nullptr){
          critical_statement = node->critical_statement;
          node = node->parent_node;
        }
        if(critical_statement!=nullptr){
          //cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"<<endl;
          vector<SgStatement*> critical_statement_list;
          critical_statement_list.push_back(critical_statement);
          statement_list.insert(statement_list.begin()+current_ptr,critical_statement_list.begin(),critical_statement_list.end());
        }
        //cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"<<endl;
      };

      string get_condition_str();

      virtual neb::CJsonObject get_ir_content(){
        neb::CJsonObject general_json("");
        general_json.Add("type",type);
        if(type=="func_call"){
          general_json.Add("function_name",ref_func_name);
        }
        if(statement_list.size()!=0){
          general_json.AddEmptySubArray("line_no");
          general_json["line_no"].Add(get_begin_line_no());
          general_json["line_no"].Add(get_end_line_no());
        }
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
        general_json.AddEmptySubArray("traces");
        if(traces.size()!=0){
          for(CodeSegment* trace : traces){
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
        }else{
          neb::CJsonObject trace_json("");
          trace_json.Add("constraint","true");
          trace_json.Add("break",false);
          trace_json.Add("continue",false);
          if(segment_list.size()==0){
            cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" <<endl;
            print();
            cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" <<endl;
            bool find = false;
            string double_array = "[";
            /*
            if(statement_list.size()!=0){
              for(SgStatement* statement:statement_list){
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
            for(Variable* v : intermediate_list){
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
            for(CodeSegment* cs : segment_list){
              trace_json["content"].Add(cs->get_ir_content());
            }
          }
          general_json["traces"].Add(trace_json);

        }
        general_json.AddEmptySubArray("irrelevant");
        for(int no : unrelated_lines){
            general_json["irrelevant"].Add(no);
        }
        /*
          string blank = common::get_line_start_blank(tab_num);
          stringstream ir_stream;
          if(segment_list.size() > 0) {
              for(CodeSegment* segment : segment_list) {
                  ir_stream << blank << segment -> get_ir_content(tab_num+1) << endl;
              }
          }else {
              ir_stream << blank << "TODO: segment ir content" << endl;
              if(traces.size() > 0) {
                  for(CodeSegment* segment : traces) {
                      ir_stream << blank << segment -> get_ir_content(tab_num+1) << endl;
                  }
              }
          }
          return ir_stream.str();
        */
        return general_json;
      };

      void calculate_input();

      int get_begin_line_no(){
        return statement_list.front()->get_file_info()->get_line();
      }

      int get_end_line_no(){
        return statement_list.back()->get_file_info()->get_line();
      }

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



