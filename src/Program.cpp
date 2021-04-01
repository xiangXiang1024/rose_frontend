//
// Created by root on 2021/3/5.
//

#include "Program.h"

using namespace std;

using namespace SageBuilder;
using namespace SageInterface;

void Program::generate_ref_variable_list(vector<Variable*> &ref_list,SgFunctionCallExp* exp){
    SgExprListExp* expr_list = exp -> get_args();
    SgExpressionPtrList&  expr_ptr_list = expr_list -> get_expressions();
//    cout<<"call ref variables:"<<endl;
    for(auto expr_ptr : expr_ptr_list) {
//                    cout << expr_ptr -> unparseToString() << "\t|\t" << expr_ptr -> class_name() << endl;
        Variable* v=new Variable(expr_ptr->unparseToString(),expr_ptr->get_type());
//        vector<Variable*> ref_variable_list = get_ref_variable_list(expr_ptr);
//        for(Variable* v:ref_variable_list){
//            v->print();
            ref_list.push_back(v);
//        }
//                    cout << "ref variable: " << endl;
//                    for(Variable* v : ref_variable_list) {
//                        v.print();
//                        segment1 -> add_output(v);
//                    }
//                    cout << endl;

                }
//                cout<<endl;

}

void Program::generate_func_parameter_list(vector<Variable*> &parameter_list,SgFunctionDeclaration* func_declaration){
    //cout<<"function parameters: "<<endl;
    SgFunctionParameterList* params = func_declaration -> get_parameterList();
    for (SgInitializedName* param : params -> get_args()) {
        string name = param -> get_name().getString();
        SgType* type = param -> get_type();
        Variable* variable = new Variable(name, type);
        //variable->print();
        parameter_list.push_back(variable);
    }
    //cout<<endl;


}

void Program::generate_assignment_statement_list(vector<SgStatement*>&statement_list,vector<Variable*> ref_variable_list,vector<Variable*> func_parameter_list,SgNode* node){

    SgBasicBlock* block=isSgBasicBlock(node);

    if(block!=NULL&&ref_variable_list.size()==func_parameter_list.size()){
        for (int  i = 0; i < ref_variable_list.size(); i++)
        {
            Variable* ref=ref_variable_list.at(i);
            Variable* param=func_parameter_list.at(i);
            if(ref->variable_name!=param->variable_name){

                SgVariableDeclaration* variableDeclaration=buildVariableDeclaration(param->variable_name,param->type,buildAssignInitializer(buildVarRefExp(ref->variable_name)));

                prependStatement(variableDeclaration,block);

                statement_list.push_back(dynamic_cast<SgStatement*>(variableDeclaration));
            }
        }



    }
}



SgFunctionCallExp* Program::has_func_call(SgNode* statement) {
//    cout << "check has_func_call node: " << endl;
//    cout << "statement: " << statement -> unparseToString() << "\t|\t" << statement -> class_name() << endl;
    SgFunctionCallExp* exp = dynamic_cast<SgFunctionCallExp*>(statement);
    if(exp != nullptr&&static_cast<int>((exp->unparseToString()).find("<<"))!=-1){
      return nullptr;
    }


    if(dynamic_cast<SgFunctionCallExp*>(statement) != nullptr) {
//        cout << "meet func call statement: " << statement -> unparseToString() << endl;
        SgFunctionCallExp* exp = dynamic_cast<SgFunctionCallExp*>(statement);
        cout << exp->unparseToString() <<endl;
        return dynamic_cast<SgFunctionCallExp*>(statement);
    }

    vector<pair<SgNode*, string>> pair_list = statement -> returnDataMemberPointers();
    for(pair<SgNode*, string> p : pair_list) {
        if(p.first == nullptr) {
            break;
        }
        /*cout << "pair: " << endl;
        cout << "node: " << p.first -> unparseToString() << "\t|\t" << p.first -> class_name() << endl;
        cout << "string: " << p.second << endl;*/
        if(dynamic_cast<SgStatement*>(p.first) != nullptr || dynamic_cast<SgExpression*>(p.first) != nullptr) {
            SgFunctionCallExp* exp = has_func_call(p.first);
            if(exp != nullptr) {
//                cout << "meet func call statement: " << exp -> unparseToString() << endl;
                return exp;
            }
        }
    }

    return nullptr;
}
