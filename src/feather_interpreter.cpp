#include "feather_vm.h"
#include <ctype.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

namespace fsl
{
    void feather_vm::init(const char *data)
    {
        programm_length = strlen(data) + 1;
        data_copy = new char[programm_length];
        memcpy(data_copy, data, programm_length);
        printf("programm loaded : %s \n", data_copy);
        current_lexer.parse(data_copy);
        perline_information *pifo = current_lexer.generate_information();
        for (int i = 0; pifo[i].current_line != -1; i++)
        {
            printf("line %i context %i \n", i, pifo[i].line_start_context);

            if (pifo[i].litem_length != 0)
            {
                for (int j = 0; j < pifo[i].litem_length; j++)
                {
                    printf("    litem %i pos %i name %s \n", j, pifo[i].litem[j]->position, sys_type[pifo[i].litem[j]->subtype]);
                }
            }
            if (pifo[i].felment_length != 0)
            {
                for (int j = 0; j < pifo[i].felment_length; j++)
                {
                    printf("    element %i pos %i name %s \n", j, pifo[i].felment[j]->position, pifo[i].felment[j]->name);
                }
            }
        }
    }

} // namespace fsl