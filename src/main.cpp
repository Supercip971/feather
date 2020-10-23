#include "feather_vm.h"
#include <stdio.h>
/*


0 | func myfunction(){
1 |     int v = 1+1
2 | }
3 |
4 | func main(){
5 |     myfunction()
6 | };


*/

int main()
{
    const char *feather_code =
        "func myfunction(){\n"
        "    int v = 1+1 \n"
        "}\n"
        "\n"
        "func main(){\n"
        "    myfunction()\n"
        "}\n";

    return 0;
}
