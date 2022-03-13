#pragma once 

typedef struct
{
    int idx;
} FeatherType;

typedef struct
{
    int idx;
} FeatherVal;

typedef struct
{
    const char* name;
    FeatherType type;
    FeatherVal val;
} FeatherVariable;

typedef struct
{
    FeatherType return_type;
    FeatherVariable* variables;
    int variable_count;
} Context;

typedef struct
{
    Context* contextes;
    int context_count;
} FeatherVm;