#ifndef PARSER_H
#define PARSER_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define BUFSIZE 1024
#define TOKEN_BUFSIZE 64

typedef struct {
    char declare_container[TOKEN_BUFSIZE][BUFSIZE];
    int *declared_list_length;
} declared_t;

int is_declared(char *identifier);

char* insert_buffer(char *buffer_ptr, char **parsed_line, int condition_start, int condition_end);
int program(int *converted, int converted_length, char **parsed_line, int *itr, FILE* header, FILE* body);
int statement(int *converted, int converted_length, char **parsed_line, int *itr, FILE* header, FILE* body);
int expression(int *converted, char **parsed_line, int *itr, FILE* header, FILE* body);
int comparison(int *converted, char **parsed_line, int *itr, FILE* header, FILE* body);
int term(int *converted, char **parsed_line, int *itr, FILE* header, FILE* body);
int unary(int *converted, char **parsed_line, int *itr, FILE* header, FILE* body);
int primary(int* converted, char **parsed_line, int *itr, FILE* header, FILE* body);

int *itr;
int *converted_peak;
int converted_length[BUFSIZE];
int if_comparison[BUFSIZE];
int if_statement[BUFSIZE];
int while_comparison[BUFSIZE];
int while_statement[BUFSIZE];
int let_1[BUFSIZE];
int expression_1[BUFSIZE];
int expression_2[BUFSIZE];
int term_1[BUFSIZE];
int term_2[BUFSIZE];
int unary_1[BUFSIZE];
int unary_2[BUFSIZE];
int primary_1;



#endif





