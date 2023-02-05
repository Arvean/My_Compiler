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

typedef struct {
    char *key;
    int value;
} keyword_t;

typedef struct {
    char *key;
    int value;
} operators_t;

typedef struct {
    char *key;
    int value;
} map_t;

keyword_t keywords[10];
operators_t operators[11];
map_t map[4];
int keywords_length;
int operators_length;
int map_length;

#endif