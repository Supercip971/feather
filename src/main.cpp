#include "feather_lexer.h"
#include "feather_map.h"
#include "feather_type.h"
#include "feather_vm.h"

#include "feather_vector.h"
#include <stdio.h>
/*


0 | func myfunction(){
1 |     int v = 1 + 3 * 4;
2 | }
3 |
4 | func main(){
5 |     myfunction();
6 | }




*/
int main()
{
    const char *feather_code =
        "func myfunction() -> int {\n"
        "    int i = 13 ; \n"
        "    int v = i - 3 + 4 * 2  ; \n"
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
