#pragma once
#include "feather_map.h"
namespace fsl
{

#define LIST_DELCLARATION(list_name, element_list_name, type1, type2) \
    static fsl::feather_map<type1, type2> list_name =                 \
        fsl::feather_map<type1, type2>(element_list_name, sizeof(element_list_name) / sizeof(element_list_name[0]));

    enum feather_main_element_type
    {
        TYPE_OPERATOR = 1,
        TYPE_SPECIFIC = 2,
        TYPE_DELIMITOR = 3,
        TYPE_TOKEN = 4,
        TYPE_END_OF_LINE = 5,
        TYPE_NUMBER = 6
    };

    // ---------------- FEATHER OPERATOR ----------------

    enum feather_operator
    {
        OPERATOR_EQUAL = 1,
        OPERATOR_MULTIPLY,
        OPERATOR_ADD,
        OPERATOR_SUBSTRACT,
        OPERATOR_DIVIDE
    };
    static const fsl::feather_map_element<const char *, feather_operator> operator_list[] =
        {
            {"=", OPERATOR_EQUAL},
            {"*", OPERATOR_MULTIPLY},
            {"+", OPERATOR_ADD},
            {"-", OPERATOR_SUBSTRACT},
            {"/", OPERATOR_DIVIDE}};
    LIST_DELCLARATION(feather_operator_list, operator_list, const char *, feather_operator);

    // ---------------- FEATHER SPECIFIC ITEM ----------------

    enum feather_specific_item
    {
        NAME_FUNC = 1,
        NAME_BOOL,
        NAME_INT,
        NAME_FLOAT,
        NAME_STRING,
        NAME_CHAR,
        NAME_ADDRESS,
        NAME_IF,
        NAME_ELIF,
        NAME_ELSE,
        NAME_NULL,
        NAME_WHILE,
        NAME_FOR,
        NAME_DO_WHILE,
        NAME_IMPORT,
        NAME_RETURN,
        NAME_CLASS,
        NAME_RETURN_TYPE,
    };
    static const fsl::feather_map_element<const char *, feather_specific_item> specific_item[] =
        {
            {"func", NAME_FUNC},
            {"int", NAME_INT},
            {"string", NAME_STRING},
            {"char", NAME_CHAR},
            {"float", NAME_FLOAT},
            {"address", NAME_ADDRESS},
            {"if", NAME_IF},
            {"elif", NAME_ELIF},
            {"else", NAME_ELSE},
            {"while", NAME_WHILE},
            {"for", NAME_FOR},
            {"do_while", NAME_DO_WHILE},
            {"import", NAME_IMPORT},
            {"return", NAME_RETURN},
            {"class", NAME_CLASS},
            {"->", NAME_RETURN_TYPE}};
    LIST_DELCLARATION(feather_specific_item_list, specific_item, const char *, feather_specific_item);

    // ---------------- FEATHER DELIMITOR ----------------

    enum feather_delimitor
    {
        DELIMITOR_CODE_BLOCK_OPEN = 1,
        DELIMITOR_ARRAY_BLOCK_OPEN,
        DELIMITOR_ARGUMENT_BLOCK_OPEN,
        DELIMITOR_CODE_BLOCK_CLOSE,
        DELIMITOR_ARGUMENT_BLOCK_CLOSE,
        DELIMITOR_ARRAY_BLOCK_CLOSE,
    };

    static const fsl::feather_map_element<char, feather_delimitor> delimitor_list[] =
        {
            {'{', DELIMITOR_CODE_BLOCK_OPEN},
            {'(', DELIMITOR_ARGUMENT_BLOCK_OPEN},
            {'[', DELIMITOR_ARRAY_BLOCK_OPEN},
            {'}', DELIMITOR_CODE_BLOCK_CLOSE},
            {')', DELIMITOR_ARGUMENT_BLOCK_CLOSE},
            {']', DELIMITOR_ARRAY_BLOCK_CLOSE}};

    LIST_DELCLARATION(feather_delimitor_list, delimitor_list, char, feather_delimitor);

} // namespace fsl
