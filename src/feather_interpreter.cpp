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
        main_context.run();
    }

    feather_context *feather_context::find_context(int line)
    {
        for (int i = 0; i < child.length; i++)
        {
            if (child.elements[i].current_line == line)
            {
                return &child.elements[i];
            }
        }
        // try to find element in upper context
        if (parent != nullptr)
        {
            return parent->find_context(line);
        }
        return nullptr;
    }
    feather_h_element *feather_context::find_element(const char *element_name)
    {
        for (int i = 0; i < element.length; i++)
        {
            if (strcmp(element.elements[i].name, element_name) == 0)
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
                        child.elements[child.length - 1] = feather_context(plifo, i + 1, this, lex);
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
                                    target->type = 5;
                                    target->value = i;
                                    printf("function declaration line : %i | offset %i | name %s | type %i \n", i, context_offset_start, target->name, target->type);
                                    
                                    break;
                                }
                                else
                                {
                                    // so variable declaration
                                    target->type = plifo[i].litem[j]->subtype;
                                    target->value = 0;
                                    printf("variable declaration line : %i | %s | name %s | offset %i \n", i, sys_type[plifo[i].litem[j]->subtype], target->name, context_offset_start);
                                    break;
                                }
                            }
                        }
                    };
                }
            }
        }
    }
    void feather_context::interpret_line()
    {
        bool variable_definition = false;
        bool function_call = false;
        bool special_statement = false;
        if (plifo[current_line].litem_length != 0)
        {
            for (int j = 0; j < plifo[current_line].litem_length; j++)
            {
                //
                if (sys_type[plifo[current_line].litem[j]->subtype][0] == '=') // so this mean that there is an variable declaration
                {
                    variable_definition = true;
                    printf("%i | there is an variable definition \n", current_line);
                }
                else if (sys_type[plifo[current_line].litem[j]->subtype][0] == '(') //
                {
                    function_call = true;
                    printf("%i | there is an function call \n", current_line);
                }
                else if (plifo[current_line].litem[j]->type == 4)
                {
                    special_statement = false;
                }
            }
        }
        else
        {
            // do nothing
        }

        if (variable_definition)
        {
        }
        else if (function_call)
        {
            if (plifo[current_line].litem_length != 0)
            {
                for (int j = 0; j < plifo[current_line].felment_length; j++)
                {

                    printf("%i | searching for %s \n", current_line, plifo[current_line].felment[j]->name);
                    feather_h_element *el = find_element(plifo[current_line].felment[j]->name);
                    if (el != nullptr)
                    {
                        
                            printf("%i | founded function %s \n", current_line, el->name);
                            auto context = find_context(el->value);
                            if(context != nullptr){
                                printf("%i | founded context %i \n", current_line, context->current_line);
                                context->run();
                            }
                       
                    }else{
                        printf("%i |  no element founded :^(", current_line);
                    }
                }
            }
        }
        else if (special_statement)
        {
        }
    }
    void feather_context::run()
    {
        if (parent == nullptr)
        {
            printf("\n\n == feather context RUN ==\n\n");
            auto main_function = find_element("main");
            if (main_function == nullptr)
            {
                printf("no main function founded :^( \n");
                return;
            }
            uint64_t main_line = main_function->value;
            printf("%s function on line %i \n", main_function->name, main_function->value);
            for (int i = 0; i < child.length; i++)
            {
                if (main_line + 1 == child.elements[i].start_line)
                {
                    printf("running child context main\n");
                    child.elements[i].run(); // run the context with main()
                }
            }
        }
        else
        {
            current_line = start_line;
            for (current_line = start_line; lex->get_context_at(current_line, 0) >= context_offset_start; current_line = next_line)
            {
                next_line = current_line + 1;
                printf("%i | interpreting line %i \n", current_line, current_line);
                interpret_line();
            }
        }
    }
}; // namespace fsl