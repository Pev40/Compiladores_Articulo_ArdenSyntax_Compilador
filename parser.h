#ifndef PARSER_H
#define PARSER_H

typedef struct ASTNode {
    enum { NODE_SECTION, NODE_STATEMENT, NODE_ASSIGNMENT, NODE_EXPRESSION, NODE_CONDITION, NODE_ACTION } type;
    union {
        struct { char *name; struct ASTNode *statements; } section;
        struct { struct ASTNode *next; struct ASTNode *stmt; } statement_list;
        struct { char *var; struct ASTNode *expr; } assignment;
        struct { int value; char *id; } expression;
        struct { struct ASTNode *condition; struct ASTNode *stmt; } condition;
        struct { struct ASTNode *expr; } action;
    } data;
} ASTNode;

ASTNode *parse();
void free_ast(ASTNode *node);
void print_ast(ASTNode *node, int indent);

#endif
