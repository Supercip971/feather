#pragma once 

#include <feather/utils/vec.h>
#include <feather/ast/expr.h>

#include <feather/ast/decl.h>
#include <feather/ast/val.h>

#define AST_ELEMENT_ENUM_ADD(v) v ,

#define FOREACH_AST(CALL) \
    FOREACH_EXPR_AST(CALL) \
    FOREACH_DECL_AST(CALL) \
    FOREACH_CONST_AST(CALL)

typedef enum 
{
    AST_NONE,
    FOREACH_AST(AST_ELEMENT_ENUM_ADD)
} FeatherASTElements;

typedef struct 
{
    int type; // type
    int ro; // right operand
    int lo; // left operand

    int _advanced_info; // used for debugging or error dumping
} FthAST;

typedef vec_t(FthAST) FthASTList;
typedef vec_t(FthVal) FthValList;


typedef struct
{
    FthASTList ast;
    FthValList val;

} FeatherAstCtx;

FthAST* ftast_node(FeatherAstCtx* ctx, int id);
FthVal* ftast_val(FeatherAstCtx* ctx, int id);

const char* ftast_type_to_str(int type);
