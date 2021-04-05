#include "klee/klee.h"

float klee_output(const char *name, float v)
{
    return v;
}

float max(float a, float b) {
    klee_make_symbolic(&a, sizeof(a), "a");
    klee_make_symbolic(&b, sizeof(b), "b");
    float result;
    if(a > b) {
        result = a;
    }else if(a < b){
        result = b;
    }
    klee_output("result", result);
    return result;
}

int main() {
    float a, b;
    max(a, b);
    return 0;
}