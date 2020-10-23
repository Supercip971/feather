#include "feather_map.h"
#include "feather_type.h"
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
    printf("list entry %i get %s", 3, fsl::feather_operator_list.find_from_co(3));
    return 0;
}
