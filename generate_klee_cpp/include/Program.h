#include "rose.h"
#include <vector>
#include <Variable.h>
#include <fstream>

using namespace std;


class Program{
public:
    string file_path;
    SgGlobal *global;
    int func_count;
    Program(const string &_name, SgGlobal *_global) : file_path(_name), global(_global) {func_count=0;}



    void generate_func_parameter_list(vector<Variable*> &parameter_list,SgFunctionDeclaration* func_declaration){
      SgFunctionParameterList* params = func_declaration -> get_parameterList();
      for (SgInitializedName* param : params -> get_args()) {
        string name = param -> get_name().getString();
        SgType* type = param -> get_type();
        Variable* variable = new Variable(name, type);
        parameter_list.push_back(variable);
      }
    }

    void rewrite_file(SgFunctionDeclaration* func_declaration){
      vector<Variable*> parameter_list;
      generate_func_parameter_list(parameter_list,func_declaration);  

      string name=func_declaration->get_name();
      
      ifstream input_file(file_path);

      string num=to_string(func_count);
      string out_path=file_path.substr(0,file_path.size()-4)+num+".cpp";

      ofstream output_file(out_path);

      output_file<<"#include \"klee/klee.h\""<<endl;
      output_file<<"float klee_output(const char *name, float v){return v;}"<<endl;

      string line;

      while(getline(input_file,line)){
        output_file<<line<<endl;
      }
      

      output_file<<"int main(){"<<endl;

      string func_ref="(";
      for(Variable* param:parameter_list){
        output_file<<param->type->unparseToString()<<" "<<param->variable_name<<";"<<endl;
        output_file<<"klee_make_symbolic(&"<<param->variable_name<<", sizeof("<<param->variable_name<<"), \""<<param->variable_name<<"\");"<<endl;
        func_ref.append(param->variable_name);
        func_ref.append(",");
      }
      func_ref=func_ref.substr(0,func_ref.size()-1);
      func_ref.append(")");

      output_file<<"klee_output(\"result\", "<<name<<func_ref<<");"<<endl;

      //output_file<<name<<func_ref<<";"<<endl;
      

      




      output_file<<"return 0;}"<<endl;

      input_file.close();
      output_file.close();


    }
    
    void generate_main_function(){
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
              }else{
                func_count++;
                rewrite_file(func_declaration);


              }
            }
        }
    }




};