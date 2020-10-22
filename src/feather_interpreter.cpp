#include "feather_lexer.h"
#include "feather_type.h"
#include "feather_vm.h"
#include <ctype.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

namespace fsl
{

    feather_vm::feather_vm()
    {
    }
    feather_context::feather_context()
    {
    }
    void feather_vm::init(const char *data)
    {
        programm_length = strlen(data) + 1;
        data_copy = new char[programm_length];
        memcpy(data_copy, data, programm_length);
        printf("programm loaded : %s \n", data_copy);
        current_lexer.parse(data_copy);
        pli_info = current_lexer.generate_information();
    }
    void feather_vm::run()
    {
        main_context = feather_context(pli_info, 0, nullptr, &current_lexer);
        main_context.init_element();
    }

    feather_h_element *feather_context::find_element(const char *element_name)
    {
        for (int i = 0; i < element.length; i++)
        {
            if (strcmp(element.elements[i].name, element_name))
            {
                return &element.elements[i];
            }
        }
        // try to find element in upper context
        if (parent != nullptr)
        {
            return parent->find_element(element_name);
        }
        return nullptr;
    }
    // parent_context = nullptr when it is the main context
    feather_context::feather_context(perline_information *perline_info, uint64_t context_line_start, feather_context *parent_context, feather_lexer *lexer)
    {
        context_offset_start = lexer->get_context_at(context_line_start, 0);
        printf("feather context loading, line %i, offset %i \n", context_line_start, context_offset_start);
        lex = lexer;
        start_line = context_line_start;
        parent = parent_context;
        plifo = perline_info;
        child.length = 0;
        element.length = 0;
        // give 1 bit of memory for the realloc
        child.elements = (feather_context *)malloc(1);
        element.elements = (feather_h_element *)malloc(1);
    }
    void feather_context::init_element()
    {
        for (int i = start_line; plifo[i].current_line != -1; i++)
        {
            // check all item and find if there are context child
            // like if() {} of func myfunction(){}
            if (plifo[i].litem_length != 0)
            {
                for (int j = 0; j < plifo[i].litem_length; j++)
                {
                    if (sys_type[plifo[i].litem[j]->subtype][0] == '{')
                    {
                        child.increase_size(1);
                        child.elements[child.length - 1] = feather_context(plifo, i+1, this, lex);
                        child.elements[child.length - 1].init_element();
                    }
                }
            }
            // element
            if (plifo[i].felment_length != 0)
            {
                for (int j = 0; j < plifo[i].felment_length; j++)
                {
                    // is this element in this context ?
                    if (context_offset_start == lex->get_context_at(plifo[i].felment[j]->line, 0))
                    {
                        // increase size by 1
                        element.increase_size(1);

                        feather_h_element *target = &element.elements[element.length - 1];
                        target->name = plifo[i].felment[j]->name;
                        if (plifo[i].litem_length == 0)
                        {
                            printf("feather error, single element definition without any mean \n");
                            return;
                        }
                        for (int k = 0; k < plifo[i].litem_length; k++)
                        {
                            if (plifo[i].litem[j]->type == 1) // func or string int ... if it is a declaration
                            {
                                if (plifo[i].litem[j]->subtype == 5)
                                {
                                    // function declarations
                                    printf("function declaration line : %i\n", i);
                                    target->type = FEATHER_FUNCTION;
                                    target->value = i;
                                    break;
                                }
                                else
                                {
                                    // so variable declaration
                                    target->type = plifo[i].litem[j]->subtype;
                                    target->value = 0;
                                    printf("variable declaration line : %i / %s / name %s / offset %i \n", i, sys_type[plifo[i].litem[j]->subtype], target->name, context_offset_start);
                                    break;
                                }
                            }
                        }
                    };
                }
            }
        }
    }
    void feather_context::run()
    {
    }
}; // namespace fsl