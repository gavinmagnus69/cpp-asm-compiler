#pragma once

#include "token/token.h"
#include "../error_handle/error_handle.h"
#include <vector>
#include <fstream>

class Lexer{
private:    
    std::string file_path;
    std::string code;
    std::vector<Token> tokens;
    std::size_t current_token_index;
public:
    explicit Lexer(const std::string& file_path) : file_path(file_path) {
        current_token_index = 0;
        this->open();
    }

    void parse(){
        split();
        merge();
        check();
    }

    void print(){
        for( auto& token : tokens){
            token.print();
        }
    }

    Token current_token(){
        return tokens[current_token_index];
    }

    token_type current_token_type(){
        return current_token().type;
    }

    void next_token(){
        ++current_token_index;
    }

private:
    void open(){
        std::ifstream in(file_path, std::ios::binary);

        if(!in.is_open()){
            error_handle::raise(error_handle_type::LEXER, "File not found!");
        }

        std::size_t size  = in.seekg(0, std::ios::end).tellg();
        in.seekg(0);

        code.resize(size);
        in.read(code.data(), size);
        in.close();

    }

    static bool is_complex_token(char symbol1, char symbol2){
        switch (symbol1)
        {
            case '<':
            {
                return symbol2 == '=';
            }
            case '>':
            {
                return symbol2 == '=';
            }
            case '+':
            {
                return symbol2 == '=' || symbol2 == '+';
            }
            case '-':
            {
                return symbol2 == '=' || symbol2 == '-';
            }
            case '=':
            {
                return symbol2 == '=';
            }
            case '!':
            {
                return symbol2 == '=';
            }
            case '&':
            {
                return symbol2 == '&';
            }
            case '|':
            {
                return symbol2 == '|';
            }
            case ':':
            {
                return symbol2 == ':';
            }
            case '/':
            {
                return symbol2 == '*';
            }
            case '*':
            {
                return symbol2 == '/';
            }

            default:
                return false;
        }
    }
    void merge(){
        std::vector<Token> temp_tokens;

        for(int i = 0; i < tokens.size(); ++i){

            if(tokens[i].type == token_type::INTEGER_CONST){

                if(i + 2 < tokens.size() && tokens[i + 1].type == token_type::POINT && tokens[i + 2].type == token_type::INTEGER_CONST){
                    
                    std::string new_lexeme = tokens[i].lexeme + tokens[i + 1].lexeme + tokens[i + 2].lexeme;
                    temp_tokens.push_back(Token(new_lexeme));
                    i += 2;
                    continue;
                }
            }

            if(i + 1 < tokens.size() && is_complex_token(tokens[i].lexeme[0], tokens[i + 1].lexeme[0])){
                std::string new_lexeme = tokens[i].lexeme + tokens[i + 1].lexeme;
                temp_tokens.push_back(Token(new_lexeme));
                i += 1;
                continue;
            }

            temp_tokens.push_back(tokens[i]);
        }

        tokens = temp_tokens;
        temp_tokens.clear();
    }
    
    static bool is_correct_identifier(const std::string& lexeme){
        if(!isalpha(lexeme[0]) && lexeme[0] == '_'){
            return false;
        }

        for(int i = 1; i < lexeme.size(); ++i){
            if(!isalpha(lexeme[i]) && !isdigit(lexeme[i]) && lexeme[i] != '_'){
                return false;
            }
        }

        return true;
    }

    void check(){
        for(auto& token : tokens){
            if(token.type == token_type::IDENTIFIER){
                if(!is_correct_identifier(token.lexeme)){
                    error_handle::raise(error_handle_type::LEXER, "Incorrect identifier!");
                }
            }
        }
    }

    void split(){

        std::string tmp_lexeme;

        for(const auto& symbol : code){

            if(is_separate_symbol(symbol)){
                if(!tmp_lexeme.empty()){
                    tokens.push_back(Token(tmp_lexeme));
                    tmp_lexeme.clear();
                }

                if(symbol != ' ' && symbol != '\n' && symbol != '\r'){

                    tokens.push_back(Token(std::string(1, symbol)));
                
                }

                continue;
            }

            tmp_lexeme += symbol;
        }
    }



    

    static bool is_separate_symbol(char symbol){
         return  symbol == ':' || symbol == ';' ||
                symbol == ',' || symbol == '.' ||
                symbol == '{' || symbol == '}' ||
                symbol == '(' || symbol == ')' ||
                symbol == '[' || symbol == ']' ||
                symbol == '*' || symbol == '/' ||
                symbol == '+' || symbol == '-' ||
                symbol == '&' || symbol == '|' ||
                symbol == '=' || symbol == '!' ||
                symbol == '<' || symbol == '>' ||
                symbol == '\''|| symbol == '"' ||
                symbol == '^' || symbol == '?' ||
                symbol == '%' || symbol == '\\'||
                symbol == '~' || symbol == ' ' ||
                symbol == '\r' ||symbol == '\n'||
                symbol == '#';
    }


};