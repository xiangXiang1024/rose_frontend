#include "rose.h"
#include <iostream>
#include <vector>
#include <Program.h>

using namespace std;

int main(int argc,char**argv){
    SgProject* project=frontend(argc,argv);
    RoseAst ast(project);
    vector<SgNode*> nodes = project -> get_fileList_ptr() -> get_traversalSuccessorContainer();
    for (auto n : nodes) {
        if (auto src = dynamic_cast<SgSourceFile *>(n)) {
            Program program(src->getFileName(), src->get_globalScope());
            program.generate_main_function();
        }
    }
        
    

}
