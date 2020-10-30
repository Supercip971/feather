#include "feather_lexer.h"
#include "feather_map.h"
#include "feather_type.h"
#include "feather_vm.h"
#include "feather_vector.h"
#include <stdio.h>
#define PROGRAMM_INTERPRET
/*
0 | func myfunction(){
1 |     int v = 1 + 3 * 4;
2 | }
3 |
4 | func main(){
5 |     myfunction();
6 | }
*/
#ifdef PROGRAMM_INTERPRET
#include <fstream>
#include <iostream>
int main(int argc, const char* argv[]){
    for(int i = 1; i < argc; i++){
        std::ifstream stream = std::ifstream(argv[i], std::ios::in | std::ios::binary);
        if(!stream.is_open()){
            printf("error file %s can not be opened \n", argv[i]);
        }
        stream.seekg(0, std::ios::end);
        uint64_t length = stream.tellg();
        char* dat = new char[length];
        stream.seekg(0, std::ios::beg);
        stream.read(dat, length);

        fsl::feather_virtual_machine vm = fsl::feather_virtual_machine(dat);
        vm.start();

        stream.close();
        delete[] dat;
    }


    return 0;
}


#else
int main()
{
    const char *feather_code =

        ""
        "func myanother(int a, int b){ \n"
        "   int c = b + 2 ; \n"
        "} \n"
        "func myfunction() -> int {\n"
        "    "
        "    myanother(5+10 , 20); \n"
        "    int i = 13 ; \n"
        "    int v = i-3 + 4 * 2; \n"
        "    printf(\"hello world\");\n"
        "    return v ;"
        "}\n"
        "\n"
        "func main(){\n"
        "    myfunction();\n"
        "}\n";
    fsl::feather_virtual_machine vm = fsl::feather_virtual_machine(feather_code);
    vm.start();

    return 0;
}
#endif
