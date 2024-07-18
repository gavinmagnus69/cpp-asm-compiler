#include <iostream>
#include "../src/lexer/lexer.h" 


int main(){
    try{
        Lexer lex("test.cpp");
        lex.parse();
        lex.print();
    }
    catch(std::logic_error& error){
        std::cout << error.what() << '\n';
    }
    return 0;
}