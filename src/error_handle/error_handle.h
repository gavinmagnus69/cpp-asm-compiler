#pragma once
#include <string>
#include <iostream>

enum class error_handle_type{
    LEXER, 
    PARSER,
    SEMANTIC,
    CODE_GENERATION
};

class error_handle{
public:
    static void raise(error_handle_type error_type, const std::string& msg){
        
        std::string error_begin;
        
        switch (error_type)
        {
        case error_handle_type::LEXER:
            error_begin = "Lexical error!\n";
            break;
        case error_handle_type::PARSER:
            error_begin = "Parser error!\n";
            break;
        case error_handle_type::SEMANTIC:
            error_begin = "Semantic error!\n";
            break;
        case error_handle_type::CODE_GENERATION:
            error_begin = "Code_generation error!\n";
            break;
        
        }

        std::string error_message = error_begin + msg;

        std::cout << error_message << '\n';

        throw std::logic_error(error_message);


    }


};