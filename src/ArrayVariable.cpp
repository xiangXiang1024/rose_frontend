//
// Created by root on 2021/3/11.
//

#include "ArrayVariable.h"

vector<IndexVariable*> ArrayVariable::get_index_list(SgPntrArrRefExp* arr_ref) {
//    cout << "TODO get array index list" << endl;
    vector<IndexVariable*> v;

    for(SgNode* node : arr_ref -> get_traversalSuccessorContainer()) {
        if(dynamic_cast<SgPntrArrRefExp*>(node) != nullptr) {
            vector<IndexVariable*> tmp = get_index_list(dynamic_cast<SgPntrArrRefExp*>(node));
            v.insert(v.end(), tmp.begin(), tmp.end());
        }else if(dynamic_cast<SgVarRefExp*>(node) != nullptr) {
            v.push_back(new IndexVariable(dynamic_cast<SgVarRefExp*>(node)));
        }
    }

    /*for(SgNode* n : arr_ref -> get_traversalSuccessorContainer()) {
        cout << n -> unparseToString() << "\t|\t" << n -> class_name() << endl;
        if(dynamic_cast<SgPntrArrRefExp*>(n) != nullptr) {
            for(SgNode* node : n -> get_traversalSuccessorContainer()) {
                cout << "    " << node -> unparseToString() << "\t|\t" << node -> class_name() << endl;
                if(dynamic_cast<SgPntrArrRefExp*>(node) != nullptr) {
                    for(SgNode* n2 : node -> get_traversalSuccessorContainer()) {
                        cout << "       " << n2 -> unparseToString() << "\t|\t" << n2 -> class_name() << endl;
                    }
                }
            }
        }
    }*/
    return v;
}