#include "klee/klee.h"
float klee_output(const char *name, float v){return v;}
int ff(int a,float b){
    return a;
}
int main(){
int a;
klee_make_symbolic(&a, sizeof(a), "a");
float b;
klee_make_symbolic(&b, sizeof(b), "b");
klee_output("result", ff(a,b));
return 0;}
