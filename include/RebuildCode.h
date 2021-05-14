#ifndef FRONTEND_REBUILDCODE_H
#define FRONTEND_REBUILDCODE_H

#endif


#include "CodeSegment.h"
#include <string>
#include <vector>
#include "Variable.h"
#include <fstream>
#include "Function.h"
#include "ConstraintReader.h"
using namespace std;
//#include <dir.h>
class RebuildCode
{
    
    CodeSegment segment;
    string file_path;

    vector<SgStatement*> statement_list;
    vector<Variable*> input_list;
    vector<Variable*> output_list;
    
    vector<string> constraint_list;


public:

    RebuildCode(CodeSegment s) : segment(s){
        char path[512]="";
        getcwd(path,512);
        file_path=path;
        this->statement_list=s.statement_list;
        this->input_list=s.input_list;
        this->output_list=s.output_list;
    };
    
    RebuildCode(vector<SgStatement*> _statement_list,vector<Variable*> _input_list,vector<Variable*> _output_list){
        char path[512]="";
        getcwd(path,512);
        file_path=path;
        this->statement_list=_statement_list;
        this->input_list=_input_list;
        this->output_list=_output_list;
    };

 
    void rewrite(){
        string file_name=file_path+"/segment.cpp";
        ofstream cpp_file(file_name);

        cpp_file<<"#include \"klee/klee.h\""<<endl;
        cpp_file<<"float klee_output(const char *name, float v){return v;}"<<endl;

        //build segment function
        cpp_file<<"float segment";
        string param_str="(";
        string ref_str="(";
        for (Variable* param : input_list)
        {
            param_str=param_str+param->type->unparseToString()+" "+param->variable_name+",";
            ref_str=ref_str+param->variable_name+(",");
        }
        if(param_str.size()>1){
            param_str=param_str.substr(0,param_str.size()-1)+")";
            ref_str=ref_str.substr(0,ref_str.size()-1)+")";
        }else{
            param_str.append(")");
            ref_str.append(")");
        }
        cpp_file<<param_str<<"{";
        
        for ( SgStatement* statement:statement_list)
        {
            cpp_file<<statement->unparseToString()<<endl;
        }

        //build return statement
        SgStatement* statement=statement_list.back();
        if(dynamic_cast<SgReturnStmt*>(statement) == nullptr){
            if(output_list.size()<1){
                cpp_file<<"return 0;"<<endl;
            }else{
                cpp_file<<"return "<<output_list.front()->variable_name<<";"<<endl;
            }
        }



        cpp_file<<"}"<<endl;
        
        //build main function

        cpp_file<<"int main(){"<<endl;
        for (Variable* param : input_list)
        {
            cpp_file<<param->type->unparseToString()<<" "<<param->variable_name<<";"<<endl;
            cpp_file<<"klee_make_symbolic(&"<<param->variable_name<<", sizeof("<<param->variable_name<<"), \""<<param->variable_name<<"\");"<<endl;
        }

        cpp_file<<"klee_output(\"result\", segment"<<ref_str<<");"<<endl;

        cpp_file<<"return 0;}"<<endl;


        cpp_file.close();

        //execute run_klee.sh
        
        run_klee();
        read_expr();

    }

    void run_klee(){

        system("./run_klee.sh");
    }

    void read_expr(){
        string expr_path=file_path+"/expression.txt";
        
        ConstraintReader reader;
        reader.read_constraint(expr_path,constraint_list);

        //for (int i = 0; i < constraint_list.size(); i++)
        //{
        //    cout<<"constraint "<<i+1<<": "<<constraint_list[i]<<endl;
        //}
    }


 
};




