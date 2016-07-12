#ifndef LEX_H
#define LEX_H
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <vector>

using namespace std;

enum TokenType{
    //标识的类型
    NUM,   //数字,34.0
    STRING, //字符串
    LEBRAKET,  //左括号
    RIBRAKET,   //右括号
    COMMA,  //逗号
    ADD,
    MUL,
    SUB,
    DIV,
    MOD,
    SEMICOLON, //;
    WHILE,  //while
    BREAK,  //break
    CONTINUE, //continue
    RETURN,   //return
    TRUE,  //true
    FALSE,  //false
    LC,       //{
    RC,       //}
    IF,     //if
    ELSE,   //else
    EQ,     //==
    ASSIGN, //=
    LT,     //<
    BT,     //>
    AND,    //&&
    OR,     //||
    NOT,    //'!'
    DEF,    //def
    IDENTIFIER, //lqk
    COMMENT, //#
    NEWLINE, //\n
    END //\0
};

struct Coordinate{
    int x;      //cloumn
    int y;         //lineno
};

struct Token{               //token
    Token(TokenType t,char *val):type(t),value(val){}
    ~Token()
    {
        delete value;
    }
    TokenType type;
    char *value;
};

struct CodeLine{           //one line
    vector<Token> tokens;
    int line;
};

struct CodeFile{           //file
    vector<Token*> lines;

};
CodeFile* tokenScanner(const char *path);
#endif // LEX_H
