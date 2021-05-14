#include <string>
#include <map>
#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

class ConstraintReader
{
private:
    string expression_file_path;
    map<string,string> constraints;
    const string HEADER="constraint";
    const string CONNECT_SYMBOL="&&";
    
    
public:
    ConstraintReader(/* args */){}
    //ConstraintReader(const string path):expression_file_path(path){}


    void  read_constraint(const string path,vector<string>& constraints_list);

    bool is_constraint_head_line(string line,const string header);
    bool is_constraint_end_line(string line);
    string get_single_contraint(string line);
    

};

