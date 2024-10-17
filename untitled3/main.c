#include <stdio.h>
#include "vm.h"
int main(void) {
    int lenght=0;
    Stat prog[]=load_code(in_file);
    start_execution(prog, lenght);
    return 0;
}
