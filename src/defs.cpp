#include "defs.h"
std::string token::to_str(){
    switch (_value)
    {
    case token_type::NULL_TOKEN:
        return "null token";
        break;
    case token_type::PLUS:
        return "+";
        break;
    
    case token_type::MINUS:
        return "-";
        break;
    case token_type::STAR:
        return "*";
        break;
    case token_type::SLASH:
        return "/";
        break;
    case token_type::INT_VALUE:
        return std::to_string(_raw_value);
        break;
    case token_type::END_OF_STREAM:
        return "end of stream";
        break;
    default:
        break;
    }
}

ast_type ast_from_token(token_type type){

    switch (type)
    {
    case token_type::NULL_TOKEN:
        return ast_type::NULL_TYPE;
        break;
    case token_type::PLUS:
        return ast_type::ADD;
        break;
    
    case token_type::MINUS:
        return ast_type::SUBSTRACT;
        break;
    case token_type::STAR:
        return ast_type::MULTIPLY;
        break;
    case token_type::SLASH:
        return ast_type::DIVIDE;
        break;
    case token_type::INT_VALUE:
        return ast_type::INT_VALUE;
        break;
    case token_type::END_OF_STREAM:
        return ast_type::NULL_TYPE;
        break;
    default:
        return ast_type::NULL_TYPE;
        break;
    }
}
std::string ast_to_str(ast_type type){
    switch(type){
        case ast_type::ADD:{
            return " + ";
        }
        case ast_type::SUBSTRACT:{
            return " - ";
        }
        case ast_type::DIVIDE:{
            return " / ";
        }
        case ast_type::INT_VALUE:{
            return " int ";
        }
        case ast_type::MULTIPLY:{
            return " * ";
        }
        case ast_type::NULL_TYPE:{
            return " null ";
        }
        default: {
            return " undefined ";

        }
    };
};

void ast_node::print(){
    
    if(left){
        left->print();
    }

    printf("ast: %s ", ast_to_str(type).c_str());
    if(type == ast_type::INT_VALUE){
        printf(" = %i \n", value);
    }else{
        printf("\n");
    }

    if(right){
        right->print();
    }
}
void ast_node::destroy(){
    if(left){
        left->destroy();
    }if(right){
        right->destroy();
    }
    delete this;
}
int ast_node::generate(){
    int left_register;
    int right_register;
    if(left){
        left_register = left->generate();
    }
    if(right){
        right_register = right->generate();
    }

    switch(type){
        case ast_type::DIVIDE:{
            return left_ret / right_ret;
        }
        case ast_type::MULTIPLY:{
            return left_ret * right_ret;
        }
        case ast_type::ADD:{
            return left_ret + right_ret;
        }
        case ast_type::SUBSTRACT:{
            return left_ret - right_ret;
        }
        case ast_type::INT_VALUE:{
            return value;
        }
        case ast_type::NULL_TYPE:{
            printf("using null type in ast interpret \n");
            
            exit(-1);
            return 0;
        }
        default: {
            printf("using undefined type in ast interpret \n");
            
            exit(-21);
            return 0;

        }
    };
}

int ast_node::generate(){
    int left_ret;
    int right_ret;
    if(left){
        left_ret = left->interpret();
    }
    if(right){
        right_ret = right->interpret();
    }

    switch(type){
        case ast_type::DIVIDE:{
            return left_ret / right_ret;
        }
        case ast_type::MULTIPLY:{
            return left_ret * right_ret;
        }
        case ast_type::ADD:{
            return left_ret + right_ret;
        }
        case ast_type::SUBSTRACT:{
            return left_ret - right_ret;
        }
        case ast_type::INT_VALUE:{
            return value;
        }
        case ast_type::NULL_TYPE:{
            printf("using null type in ast interpret \n");
            
            exit(-1);
            return 0;
        }
        default: {
            printf("using undefined type in ast interpret \n");
            
            exit(-21);
            return 0;

        }
    };
}
int tok_table[] = {
    [(int)ast_type::NULL_TYPE] = 0,
    [(int)ast_type::ADD] = 10,
    [(int)ast_type::SUBSTRACT] =10,
    [(int)ast_type::MULTIPLY] = 20,
    [(int)ast_type::DIVIDE] = 20,
    [(int)ast_type::INT_VALUE] = 0,
};
int precedence(ast_type type){
    if(tok_table[(int)type] != 0){
        return tok_table[(int)type];
    }
    else{
        printf("not usable token for precedence: %i", (int)type);
        exit(0);
        return 0;
    }
}
int fprecedence(ast_type type){
    return tok_table[(int)type];

}