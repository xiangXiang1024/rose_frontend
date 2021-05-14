#include "ConstraintReader.h"

void ConstraintReader::read_constraint(const string path,vector<string>& constraints_list){
    const char* path_chs=path.data();
    ifstream expression_file(path_chs);
    if (!expression_file.is_open())
    {
        cout<<"Failed to open file!"<<endl;
    }

    string line;
    bool is_in_constraint=false;
    bool is_completed_constraints=false;
    string constraints="";
    while (getline(expression_file,line))
    {
        if (is_in_constraint)
        {
            if (is_constraint_end_line(line))
            {
                is_in_constraint=false;
                is_completed_constraints=true;
                line=line.substr(0,line.length()-1);
            }
            constraints.append(CONNECT_SYMBOL);
            constraints.append(get_single_contraint(line));
            
        }
        
        if (is_constraint_head_line(line,HEADER))
        {
            constraints="";
            if (!is_constraint_end_line(line))
            {
                is_in_constraint=true;
            }else{
                is_completed_constraints=true;
                line=line.substr(0,line.length()-1);
            }
            constraints.append(line.substr(line.find("[")+1));
            
        }

        if (is_completed_constraints)
        {
            is_completed_constraints=false;
            constraints_list.push_back(constraints);
        }
        
        
    }

    expression_file.close();
    
    
}

bool ConstraintReader::is_constraint_head_line(string line,const string header){
    if (line.length()<header.length())
    {
        return false;
    }
    
    if (line.substr(0,header.length()).compare(header)!=0)
    {
        return false;
    }
    
    return true;
}

bool ConstraintReader::is_constraint_end_line(string line){
    if (line.substr(line.length()-1,1).compare("]")==0)
    {
        return true;
    }
    
    return false;
}

string ConstraintReader::get_single_contraint(string line){
    const char* line_chs=line.data();
    int space_num=0;
    for (int i = 0; i < line.length(); i++)
    {
        if (line_chs[i]==' ')
        {
            space_num++;
        }
        else break;
    }
    
    return line.substr(space_num);
}


