#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "tokens.h"

FILE *yyin;  // Declaración de yyin
char yytext[MAX_TOKEN_LENGTH];  // Declaración de yytext

const char* token_names[] = {
    "NUMBER", "ASSIGN", "END", "OR", "ASSIGNMENT", "COMMA", "GE", "GT", "LE", "LT",
    "NE", "LBRACE", "LPAREN", "LBRACKET", "MINUS", "PLUS", "RBRACE", "RPAREN",
    "RBRACKET", "HASH", "DIV", "MUL", "EXP", "COMMENT", "SINGLE_QUOTE",
    "DOUBLE_QUOTE", "ID", "RESERVED", "SECTION", "IF", "WRITE"
};

const char* get_token_name(enum yytokentype token_type) {
    return token_names[token_type];
}

Token tokens[MAX_TOKENS];
int token_count = 0;
YYSTYPE yylval;  // Definir yylval aquí

int is_reserved(const char *str);
int is_section(const char *str);

void add_token(enum yytokentype token_type, const char *text) {
    if (token_count < MAX_TOKENS) {
        tokens[token_count].type = token_type;
        strncpy(tokens[token_count].text, text, MAX_TOKEN_LENGTH);
        token_count++;
    } else {
        fprintf(stderr, "Error: too many tokens\n");
    }
}

void print_token(enum yytokentype token, const char *text) {
    add_token(token, text);
    switch(token) {
        case NUMBER: printf("NUMBER: %s\n", text); break;
        case ASSIGN: printf("ASSIGN: %s\n", text); break;
        case END: printf("END: %s\n", text); break;
        case OR: printf("OR: %s\n", text); break;
        case ASSIGNMENT: printf("ASSIGNMENT: %s\n", text); break;
        case COMMA: printf("COMMA: %s\n", text); break;
        case GE: printf("GE: %s\n", text); break;
        case GT: printf("GT: %s\n", text); break;
        case LE: printf("LE: %s\n", text); break;
        case LT: printf("LT: %s\n", text); break;
        case NE: printf("NE: %s\n", text); break;
        case LBRACE: printf("LBRACE: %s\n", text); break;
        case LPAREN: printf("LPAREN: %s\n", text); break;
        case LBRACKET: printf("LBRACKET: %s\n", text); break;
        case MINUS: printf("MINUS: %s\n", text); break;
        case PLUS: printf("PLUS: %s\n", text); break;
        case RBRACE: printf("RBRACE: %s\n", text); break;
        case RPAREN: printf("RPAREN: %s\n", text); break;
        case RBRACKET: printf("RBRACKET: %s\n", text); break;
        case HASH: printf("HASH: %s\n", text); break;
        case DIV: printf("DIV: %s\n", text); break;
        case MUL: printf("MUL: %s\n", text); break;
        case EXP: printf("EXP: %s\n", text); break;
        case COMMENT: printf("COMMENT: %s\n", text); break;
        case SINGLE_QUOTE: printf("SINGLE_QUOTE: %s\n", text); break;
        case DOUBLE_QUOTE: printf("DOUBLE_QUOTE: %s\n", text); break;
        case ID: printf("ID: %s\n", text); break;
        case RESERVED: printf("RESERVED: %s\n", text); break;
        case SECTION: printf("SECTION: %s\n", text); break;
        case IF: printf("IF: %s\n", text); break;
        case WRITE: printf("WRITE: %s\n", text); break;
        default: printf("UNKNOWN: %s\n", text); break;
    }
}

int yylex() {
    char ch;
    int i = 0;

    while ((ch = fgetc(yyin)) != EOF) {
        if (isspace(ch)) {
            continue;
        }

        if (isdigit(ch)) {
            yytext[i++] = ch;
            int dot_count = 0;
            while (isdigit(ch = fgetc(yyin)) || (ch == '.' && dot_count == 0)) {
                if (ch == '.') {
                    dot_count++;
                }
                yytext[i++] = ch;
            }
            ungetc(ch, yyin);
            yytext[i] = '\0';
            yylval.num = atoi(yytext);
            return NUMBER;
        } else if (isalpha(ch) || ch == '_') {
            yytext[i++] = ch;
            while (isalnum(ch = fgetc(yyin)) || ch == '_') {
                yytext[i++] = ch;
            }
            ungetc(ch, yyin);
            yytext[i] = '\0';
            yylval.str = strdup(yytext);
            if (is_reserved(yytext)) {
                return RESERVED;
            } else if (is_section(yytext)) {
                return SECTION;
            } else if (strcmp(yytext, "if") == 0) {
                return IF;
            } else if (strcmp(yytext, "write") == 0) {
                return WRITE;
            } else {
                return ID;
            }
        } else {
            yytext[i++] = ch;  // Asegurarse de almacenar el carácter no alfanumérico
            yytext[i] = '\0';
            switch (ch) {
                case '=':
                    if ((ch = fgetc(yyin)) == '=') {
                        return GE;
                    } else {
                        ungetc(ch, yyin);
                        return ASSIGN;
                    }
                case ';':
                    return END;
                case '|':
                    if ((ch = fgetc(yyin)) == '|') {
                        return OR;
                    } else {
                        ungetc(ch, yyin);
                        printf("Carácter no reconocido: |");
                    }
                    break;
                case ':':
                    return ASSIGNMENT; // Tratar los dos puntos como un token separado
                case ',':
                    return COMMA;
                case '>':
                    if ((ch = fgetc(yyin)) == '=') {
                        return GE;
                    } else {
                        ungetc(ch, yyin);
                        return GT;
                    }
                case '<':
                    if ((ch = fgetc(yyin)) == '=') {
                        return LE;
                    } else if ((ch = fgetc(yyin)) == '>') {
                        return NE;
                    } else {
                        ungetc(ch, yyin);
                        return LT;
                    }
                case '{':
                    return LBRACE;
                case '(':
                    return LPAREN;
                case '[':
                    return LBRACKET;
                case '-':
                    return MINUS;
                case '+':
                    return PLUS;
                case '}':
                    return RBRACE;
                case ')':
                    return RPAREN;
                case ']':
                    return RBRACKET;
                case '#':
                    return HASH;
                case '/':
                    if ((ch = fgetc(yyin)) == '*') {
                        // Skip block comment
                        while ((ch = fgetc(yyin)) != EOF) {
                            if (ch == '*' && (ch = fgetc(yyin)) == '/') {
                                break;
                            }
                        }
                        return COMMENT;
                    } else {
                        ungetc(ch, yyin);
                        return DIV;
                    }
                case '*':
                    if ((ch = fgetc(yyin)) == '*') {
                        return EXP;
                    } else {
                        ungetc(ch, yyin);
                        return MUL;
                    }
                case '\'':
                    return SINGLE_QUOTE;
                case '"':
                    return DOUBLE_QUOTE;
                default:
                    printf("Carácter no reconocido: %c\n", ch);
                    break;
            }
        }
    }
    return 0;
}

int is_reserved(const char *str) {
    static const char *reserved_words[] = {
        "Abs", "action", "add", "after", "aggregate", "ago", "alert", "all", "and", "any", "applicability",
        "arccos", "arcsin", "arctan", "arden", "are", "aretrue", "argument", "as", "at", "attribute",
        "author", "average", "avg", "be", "before", "Boolean", "breakloop", "by", "call", "case", "ceiling",
        "characters", "citations", "conclude", "cos", "cosine", "count", "clone", "crisp", "currenttime",
        "data", "data_driven", "data-driven", "date", "day", "days", "decrease", "default", "defuzzified",
        "delay", "destination", "do", "duration", "earliest", "elements", "else", "elseif", "enddo", "endif",
        "endswitch", "end", "eq", "equal", "event", "eventtime", "every", "evoke", "exist", "exists", "exp",
        "expired", "explanation", "extract", "false", "filename", "find", "first", "floor", "following",
        "for", "formatted", "friday", "from", "fuzzified", "fuzzy", "ge", "greater", "gt", "hour", "hours",
        "if", "in", "include", "increase", "index", "institution", "int", "interface", "interval", "is",
        "istrue", "it", "keywords", "knowledge", "language", "last", "latest", "le", "least", "left",
        "length", "less", "let", "library", "linguistic", "links", "list", "localized", "log", "log10",
        "logic", "lowercase", "lt", "maintenance", "matches", "max", "maximum", "median", "merge", "message",
        "min", "minimum", "minute", "minutes", "mlm", "mlmname", "mlm_self", "month", "monday", "months",
        "most", "names", "ne", "nearest", "new", "no", "not", "now", "null", "number", "object", "occur",
        "occurred", "occurs", "of", "or", "past", "pattern", "percent", "preceding", "present", "priority",
        "production", "purpose", "read", "refute", "remove", "replace", "research", "resources", "return",
        "reverse", "right", "round", "same", "saturday", "second", "seconds", "seqto", "set", "sin", "sine",
        "slope", "sort", "specialist", "sqrt", "starting", "stddev", "string", "substring", "sublist",
        "sum", "sunday", "support", "surrounding", "switch", "tan", "tangent", "testing", "than", "the",
        "then", "they", "thursday", "time", "title", "to", "today", "tomorrow", "triggertime", "trim", "true",
        "truncate", "truth", "tuesday", "type", "unique", "until", "uppercase", "urgency", "using",
        "validation", "value", "variable", "variance", "version", "was", "wednesday", "week", "weeks", "were",
        "where", "while", "with", "within", "write", "year", "years"
    };
    int num_reserved = sizeof(reserved_words) / sizeof(reserved_words[0]);
    for (int i = 0; i < num_reserved; i++) {
        if (strcmp(str, reserved_words[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

int is_section(const char *str) {
    static const char *sections[] = {
        "maintenance", "library", "knowledge", "resources"
    };
    int num_sections = sizeof(sections) / sizeof(sections[0]);
    for (int i = 0; i < num_sections; i++) {
        if (strcmp(str, sections[i]) == 0) {
            return 1;
        }
    }
    return 0;
}
