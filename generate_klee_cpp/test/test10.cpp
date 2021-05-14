#include "klee/klee.h"
int ff(int a,float b){
    return a;
}
int main(){
int a;
klee_make_symbolic(&a, sizeof(a), "a");
float b;
klee_make_symbolic(&b, sizeof(b), "b");
ff(a,b);
return 0;}
