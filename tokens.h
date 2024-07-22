#ifndef TOKENS_H
#define TOKENS_H

enum yytokentype {
    NUMBER,
    ASSIGN,
    END,
    OR,
    ASSIGNMENT,
    COMMA,
    GE,
    GT,
    LE,
    LT,
    NE,
    LBRACE,
    LPAREN,
    LBRACKET,
    MINUS,
    PLUS,
    RBRACE,
    RPAREN,
    RBRACKET,
    HASH,
    DIV,
    MUL,
    EXP,
    COMMENT,
    SINGLE_QUOTE,
    DOUBLE_QUOTE,
    ID,
    RESERVED,
    SECTION,
    IF,
    WRITE
};

typedef union YYSTYPE {
    int num;
    char *str;
} YYSTYPE;

#define MAX_TOKENS 1000
#define MAX_TOKEN_LENGTH 100

typedef struct Token {
    enum yytokentype type;
    char text[MAX_TOKEN_LENGTH];
} Token;

extern Token tokens[MAX_TOKENS];
extern int token_count;

#endif
