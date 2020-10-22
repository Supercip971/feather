#include "feather_vm.h"
#include <stdio.h>

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

    fsl::feather_vm vm;
    vm.init(feather_code);
    vm.run();
    return 0;
}
