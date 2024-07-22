#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"
#include "tokens.h"

extern int yylex();
extern char *yytext;
extern FILE *yyin;

ASTNode *parse_sections();
ASTNode *parse_section();
ASTNode *parse_statements();
ASTNode *parse_statement();
ASTNode *parse_assignment();
ASTNode *parse_expression();
ASTNode *parse_condition();
ASTNode *parse_action();

void yyerror(const char *s) {
    fprintf(stderr, "Error: %s\n", s);
    exit(1);
}

ASTNode *create_node(int type) {
    ASTNode *node = (ASTNode *)malloc(sizeof(ASTNode));
    if (!node) {
        yyerror("No se pudo asignar memoria");
    }
    node->type = type;
    return node;
}

ASTNode *parse() {
    return parse_sections();
}

ASTNode *parse_sections() {
    printf("Inicio del análisis de secciones\n");  // Mensaje de depuración
    ASTNode *head = NULL;
    ASTNode **current = &head;
    while (1) {
        ASTNode *section = parse_section();
        if (!section) break;
        *current = create_node(NODE_SECTION);
        (*current)->data.section.name = section->data.section.name;
        (*current)->data.section.statements = section->data.section.statements;
        current = &(*current)->data.statement_list.next;
        free(section);
    }
    printf("Fin del análisis de secciones\n");  // Mensaje de depuración
    return head;
}

ASTNode *parse_section() {
    printf("Inicio del análisis de una sección\n");  // Mensaje de depuración
    ASTNode *node = create_node(NODE_SECTION);
    int token = yylex();
    if (yytext) {
        printf("Token: %d, Texto: %s\n", token, yytext);  // Mensaje de depuración
    } else {
        printf("Token: %d, Texto: (null)\n", token);  // Mensaje de depuración
    }

    if (token != SECTION) {
        free(node);  // Evitar fugas de memoria
        return NULL;
    }
    node->data.section.name = strdup(yytext);
    printf("Sección: %s\n", node->data.section.name);  // Mensaje de depuración
    if (yylex() != ASSIGN) yyerror("Se esperaba ':'");
    node->data.section.statements = parse_statements();
    if (yylex() != END) yyerror("Se esperaba ';'");
    printf("Fin del análisis de una sección\n");  // Mensaje de depuración
    return node;
}

ASTNode *parse_statements() {
    printf("Inicio del análisis de declaraciones\n");  // Mensaje de depuración
    ASTNode *head = NULL;
    ASTNode **current = &head;
    while (1) {
        ASTNode *stmt = parse_statement();
        if (!stmt) break;
        *current = create_node(NODE_STATEMENT);
        (*current)->data.statement_list.stmt = stmt;
        current = &(*current)->data.statement_list.next;
    }
    printf("Fin del análisis de declaraciones\n");  // Mensaje de depuración
    return head;
}

ASTNode *parse_statement() {
    int token = yylex();
    if (yytext) {
        printf("Declaración Token: %d, Texto: %s\n", token, yytext);  // Mensaje de depuración
    } else {
        printf("Declaración Token: %d, Texto: (null)\n", token);  // Mensaje de depuración
    }

    switch (token) {
        case ID:
            return parse_assignment();
        case IF:
            return parse_condition();
        case WRITE:
            return parse_action();
        default:
            return NULL;
    }
}

ASTNode *parse_assignment() {
    printf("Inicio del análisis de asignación\n");  // Mensaje de depuración
    ASTNode *node = create_node(NODE_ASSIGNMENT);
    node->data.assignment.var = strdup(yytext);
    printf("Asignación: %s\n", node->data.assignment.var);  // Mensaje de depuración
    if (yylex() != ASSIGN) yyerror("Se esperaba '='");
    node->data.assignment.expr = parse_expression();
    if (yylex() != END) yyerror("Se esperaba ';'");
    printf("Fin del análisis de asignación\n");  // Mensaje de depuración
    return node;
}

ASTNode *parse_expression() {
    printf("Inicio del análisis de expresión\n");  // Mensaje de depuración
    ASTNode *node = create_node(NODE_EXPRESSION);
    int token = yylex();
    if (yytext) {
        printf("Expresión Token: %d, Texto: %s\n", token, yytext);  // Mensaje de depuración
    } else {
        printf("Expresión Token: %d, Texto: (null)\n", token);  // Mensaje de depuración
    }

    if (token == NUMBER) {
        node->data.expression.value = atoi(yytext);
    } else if (token == ID) {
        node->data.expression.id = strdup(yytext);
    } else {
        yyerror("Se esperaba un número o un identificador");
    }
    printf("Fin del análisis de expresión\n");  // Mensaje de depuración
    return node;
}

ASTNode *parse_condition() {
    printf("Inicio del análisis de condición\n");  // Mensaje de depuración
    ASTNode *node = create_node(NODE_CONDITION);
    if (yylex() != LPAREN) yyerror("Se esperaba '('");
    node->data.condition.condition = parse_expression();
    if (yylex() != RPAREN) yyerror("Se esperaba ')'");
    node->data.condition.stmt = parse_statement();
    printf("Fin del análisis de condición\n");  // Mensaje de depuración
    return node;
}

ASTNode *parse_action() {
    printf("Inicio del análisis de acción\n");  // Mensaje de depuración
    ASTNode *node = create_node(NODE_ACTION);
    node->data.action.expr = parse_expression();
    if (yylex() != END) yyerror("Se esperaba ';'");
    printf("Fin del análisis de acción\n");  // Mensaje de depuración
    return node;
}

void free_ast(ASTNode *node) {
    if (!node) return;
    switch (node->type) {
        case NODE_SECTION:
            free(node->data.section.name);
            free_ast(node->data.section.statements);
            break;
        case NODE_STATEMENT:
            free_ast(node->data.statement_list.stmt);
            free_ast(node->data.statement_list.next);
            break;
        case NODE_ASSIGNMENT:
            free(node->data.assignment.var);
            free_ast(node->data.assignment.expr);
            break;
        case NODE_EXPRESSION:
            if (node->data.expression.id) free(node->data.expression.id);
            break;
        case NODE_CONDITION:
            free_ast(node->data.condition.condition);
            free_ast(node->data.condition.stmt);
            break;
        case NODE_ACTION:
            free_ast(node->data.action.expr);
            break;
    }
    free(node);
}

void print_ast(ASTNode *node, int indent) {
    if (!node) return;
    for (int i = 0; i < indent; i++) printf("  ");
    switch (node->type) {
        case NODE_SECTION:
            printf("Sección: %s\n", node->data.section.name);
            print_ast(node->data.section.statements, indent + 1);
            break;
        case NODE_STATEMENT:
            printf("Declaración:\n");
            print_ast(node->data.statement_list.stmt, indent + 1);
            print_ast(node->data.statement_list.next, indent);
            break;
        case NODE_ASSIGNMENT:
            printf("Asignación: %s = \n", node->data.assignment.var);
            print_ast(node->data.assignment.expr, indent + 1);
            break;
        case NODE_EXPRESSION:
            if (node->data.expression.id) {
                printf("Expresión: %s\n", node->data.expression.id);
            } else {
                printf("Expresión: %d\n", node->data.expression.value);
            }
            break;
        case NODE_CONDITION:
            printf("Condición:\n");
            print_ast(node->data.condition.condition, indent + 1);
            print_ast(node->data.condition.stmt, indent + 1);
            break;
        case NODE_ACTION:
            printf("Acción: escribir\n");
            print_ast(node->data.action.expr, indent + 1);
            break;
    }
}

int main() {
    yyin = fopen("/home/piero/compiladores/prueba.txt", "r");
    if (!yyin) {
        perror("Error abriendo el archivo");
        return 1;
    }

    ASTNode *ast = parse();
    if (ast) {
        print_ast(ast, 0);
        free_ast(ast);
    } else {
        printf("No se reconoció ninguna sección.\n");
    }

    fclose(yyin);
    return 0;
}
