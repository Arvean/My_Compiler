#ifndef LEXER_H
#define LEXER_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define BUFSIZE 1024
#define TOKEN_BUFSIZE 64
#define TOK_DELIM " "

typedef struct {
    char* line;
} lexer_line_t;

typedef struct {
    char** parsed_line;
    int parsed_line_length;
} lexer_parsed_line_t;

typedef struct {
    int* converted;
    int converted_length;
} lexer_converted_t;

int is_keyword(char const keywords[10][15], int size, char* const str, char (*keyword)[15]);
void read_input(lexer_line_t* lexer_line);
void parse_line(char* const line, lexer_parsed_line_t* lexer_parsed_line);
void converter(char** const parsed_line, lexer_converted_t* lexer_converted);


char keywords[9][15];
int keywords_length;

enum MAP {
    ENDFILE = -1,
    NEWLINE = 0,
    NUMBER = 1,
    IDENT = 2,
    STRING = 3,

    // Keywords
    KEYWORD = 102,
    PRINT = 103,
    INPUT = 104,
    LET = 105,
    IF = 106,
    THEN = 107,
    ENDIF = 108,
    WHILE = 109,
    REPEAT = 110,
    ENDWHILE = 111,

    // Operators
	EQ = 201,
	PLUS = 202,
	MINUS = 203,
	ASTERISK = 204,
	SLASH = 205,
	EQEQ = 206,
	NOTEQ = 207,
	LT = 208,
	LTEQ = 209,
	GT = 210,
	GTEQ = 211
};

#endif