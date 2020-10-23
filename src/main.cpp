#include "feather_lexer.h"
#include "feather_map.h"
#include "feather_type.h"
#include "feather_vm.h"
#include <bits/stdint-uintn.h>
#include <stdio.h>
/*


0 | func myfunction(){
1 |     int v = 1+1
2 | }
3 |
4 | func main(){
5 |     myfunction()
6 | }

so the lexer has to do this 

[func] special 
[myfunction] token
[(] argument zone
[)] argument zone end
[{] code start
[int] type
[v] token
[=] operator
[1] number
[+] operator
[1] number
[}] end code
[func] special
[main] token

[(] argument zone
[)] argument zone end
[{] code start
[myfunction] token

[(] argument zone
[)] argument zone end
[}] code end

*/
int main()
{
    const char *feather_code =
        "func myfunction(){\n"
        "    int v = 1 + 1 ; \n"
        "}\n"
        "\n"
        "func main(){\n"
        "    myfunction() ;\n"
        "}\n";

    printf("list entry %i get %s \n", 3, fsl::feather_operator_list.find_from_co(3));
    fsl::lexer lex = fsl::lexer(feather_code);

    return 0;
}
