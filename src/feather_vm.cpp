
#include "feather_vm.h"
#include "feather_lexer.h"
#include "feather_math_parser.h"
#include "feather_type.h"
namespace fsl
{

    feather_virtual_machine::feather_virtual_machine()
    {
    }

    feather_virtual_machine::feather_virtual_machine(const char *code_data)
    {
        printf("loading feather virtual machine\n");
        printf("loading lexer \n");

        main_lexer = lexer(code_data);
        lexer_info = main_lexer.get_info();
    }

    uint64_t feather_virtual_machine::interpret_subcode(feather_lexer_entry *entry, uint64_t count, uint64_t end_statement)
    {
        feather_math_expression expression = feather_math_expression();

        return expression.interpret(entry, count, end_statement, this);
    }
    uint64_t feather_virtual_machine::interpret_line_specific(feather_lexer_entry *entry, uint64_t entry_id)
    {
        if (entry[entry_id].subtype < NAME_IF)
        { // func / type so it is not an if or any other thing

            if (entry[entry_id + 1].type != TYPE_TOKEN)
            {
                printf("invalid next type after entry %s, it should be a token \n", entry[entry_id + 1].data);
            }
            printf("creating variable of type %s named %s \n", entry[entry_id].data, entry[entry_id + 1].data);
            feather_variable_list &list = programm_counter.current_var_list();

            list.add_variable(0, entry[entry_id + 1].data, (feather_variable_type)entry[entry_id].subtype);

            if (entry[entry_id + 2].type != TYPE_OPERATOR)
            {
                if (entry[entry_id + 2].type == TYPE_END_OF_LINE)
                {
                    return 0;
                }
                else
                {
                    printf("invalid entry after variable definition \n");
                }
            }
            else
            {
                uint64_t r = interpret_subcode(entry, entry_id + 3, TYPE_END_OF_LINE); //
                list.find_variable(entry[entry_id + 1].data)->set_value(r);
                printf("var value %i \n", find_variable_value(entry[entry_id + 1].data)->get_value());
            }
            return 0;
        }else if(entry[entry_id].subtype == NAME_RETURN){

            uint64_t r = interpret_subcode(entry, entry_id + 1, TYPE_END_OF_LINE); //
            printf("returning %i \n", r);
            return r;
        }

        return 0;
    }
    feather_vector<function_argument> feather_virtual_machine::generate_argument_list(feather_lexer_entry *entry, uint64_t count, uint64_t start){
        feather_vector<function_argument> target;
        target.create();
        int current_offset = 0;
        bool wait_for_separator = false;
        for(int i = start; i<count; i++){
            if(entry[i].type == TYPE_DELIMITOR){
                if(entry[i].subtype == DELIMITOR_ARGUMENT_BLOCK_OPEN){
                    current_offset++;
                }
                if(entry[i].subtype == DELIMITOR_ARGUMENT_BLOCK_CLOSE){
                    current_offset--;
                    if(current_offset == 0){ // ((10+20),10)
                        return target;
                    }
                }
            }


            else if(entry[i].type == TYPE_TOKEN){
                if(wait_for_separator){
                    printf("next argument while not having a ','");
                }
                function_argument arg;
                feather_variable* var=  find_variable_value(entry[i].data);
                arg.type = var->get_type();
                arg.value = var->get_value();
                target.push(arg);
                wait_for_separator = true;
            }else if(entry[i].type == TYPE_NUMBER){
                if(wait_for_separator){
                    printf("next argument while not having a ','");
                }
                function_argument arg;
                arg.type = VAR_TYPE_INT;
                arg.value = atoi(entry[i].data);
                target.push(arg);
                wait_for_separator = true;
            }else if(entry[i].type == TYPE_SPECIFIC){
                if(entry[i].subtype == NAME_LIST_DELIMIT){
                    if(wait_for_separator){
                        wait_for_separator = false;
                    }
                }
            }

        }
        return target;
    }
    uint64_t feather_virtual_machine::interpret_multiple_entry(feather_lexer_entry *entry, uint64_t count)
    {
        uint64_t last_result = 0;
        for (uint64_t i = 0; i < count; i++)
        {
            if (entry[i].type == TYPE_TOKEN)
            {
                if (entry[i + 1].type == TYPE_DELIMITOR && entry[i + 2].type == TYPE_DELIMITOR) // function only called with no argument
                {
                    feather_function *function = find_function_definition(entry[i].data);
                    if (function == nullptr)
                    {
                        return -1;
                    }
                    uint64_t result = run_code(find_function_start(*function));
                    last_result = result;
                    programm_counter.pop();
                    return last_result;
                }else if(entry[i+1].type == TYPE_DELIMITOR){
                    printf("calling function with argument %s \n", entry[i].data);
                    feather_function *function = find_function_definition(entry[i].data);

                    if (function == nullptr)
                    {
                        printf("function not founded %s \n", entry[i].data);
                        return -1;
                    }
                    feather_vector<function_argument> list = generate_argument_list(entry, count, i+1 );

                    if(!function->set_valid_argument(list)){
                        printf("function not valid %s \n", entry[i].data);
                        return -1;
                    }
                    uint64_t start = (find_function_start(*function));

                    programm_counter.push(start); // just push next thing
                    for(size_t i = 0; i < list.get_length(); i++){
                        printf("added argument %s \n", list[i]->name);
                        programm_counter.current_var_list().add_variable(list[i]->value, list[i]->name, list[i]->type);
                    }
                    run_code_without_pushing_context(start);
                    list.destroy();
                    programm_counter.pop();
                }
            } // to thing
              // if there is a ';' after it is a variable definition
              // if there is a '=' after it is a variable assignation
              // if there is a '(' after it is a function call
            else if (entry[i].type == TYPE_SPECIFIC && entry[i].subtype != NAME_FUNC && entry[i].subtype != NAME_LIST_DELIMIT)
            { // specific can only be on the first word of a line
                if (i != 0)
                {
                    printf("specific entry isn't at the start of the line %i (%s) / %i\n", entry[i].line, entry[i].data, i);
                    printf("previous entry %s \n", entry[i - 1].data);
                }
                last_result = interpret_line_specific(entry, i);
                return last_result;
            }
        }
        return 0;
    }

    uint64_t feather_virtual_machine::run_code_without_pushing_context(uint64_t from){
        printf("jumping at %i \n", from);
        uint64_t last_result = 0;
                uint64_t current_context = 1;
                uint64_t current_line_entry = 0;
                feather_lexer_entry *current_line = new feather_lexer_entry[64]; // max 64 entry in a 'line'
        while (current_context != 0)
        {

            //printf("running %i / %s / %i \n", programm_counter.current(), lexer_info->entry[programm_counter.current()].data, current_line_entry);
            if (current_line_entry != 0 || lexer_info->entry[programm_counter.current()].type != TYPE_END_OF_LINE)
            {
                current_line[current_line_entry] = lexer_info->entry[programm_counter.current()];

                current_line_entry++;
            }
            programm_counter.current()++;
            uint64_t next = programm_counter.current();
            if (lexer_info->entry[next].type == TYPE_END_OF_LINE)
            {
                current_line[current_line_entry++] = lexer_info->entry[programm_counter.current()];

                last_result = interpret_multiple_entry(current_line, current_line_entry);
                current_line_entry = 0;
            }
            else if (lexer_info->entry[next].type == TYPE_DELIMITOR)
            {
                if (lexer_info->entry[next].subtype == DELIMITOR_CODE_BLOCK_OPEN)
                {
                    current_context++;
                }
                else if (lexer_info->entry[next].subtype == DELIMITOR_CODE_BLOCK_CLOSE)
                {
                    current_context--;
                    if (current_context == 0)
                    {

                        return last_result;
                    }
                }
            }
        }
        return 0;
    }
    uint64_t feather_virtual_machine::run_code(uint64_t from)
    {
        programm_counter.push(from);
        return run_code_without_pushing_context(from);
    }
    void feather_virtual_machine::init_function_list(){

        function_list.create();
        for (uint64_t i = 0; i < lexer_info->entry_count; i++)
        {
            feather_lexer_entry *entry = &lexer_info->entry[i];

            if (entry->type == TYPE_SPECIFIC && entry->subtype == NAME_FUNC)
            { // that mean that the next should be the function name
                function_list.push(feather_function(lexer_info->entry,i));
            }
        }
    }
    void feather_virtual_machine::start()
    {
        printf("starting feather virtual machine \n");
        init_function_list();
        programm_counter.current() = 0;
        feather_function *main_entry = find_function_definition("main");
        if (main_entry == nullptr)
        {
            printf("no main function founded  \n");
            return;
        }
        printf("main function founded at line %x \n", main_entry->get_line());
        programm_counter.current_var_list().create();
        run_code(main_entry->get_start());
    }

    uint64_t feather_virtual_machine::find_function_start(feather_function function)
    {

        return function.get_start();
    }

    feather_function *feather_virtual_machine::find_function_definition(const char *name)
    {
        bool verify_next = false;

        for(int i =0; i< function_list.get_length();i++){
            if(strcmp(name, function_list[i]->get_name()) == 0){
                return function_list[i];
            }
        }
        return nullptr;
    }

    feather_variable *feather_virtual_machine::find_variable_value(const char *name)
    {
        return programm_counter.current_var_list().find_variable(name);
    }
} // namespace fsl
