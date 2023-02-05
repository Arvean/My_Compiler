#ifndef PARSER_H
#define PARSER_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define BUFSIZE 1024
#define TOKEN_BUFSIZE 64


int program(int *converted);
int statement(int *converted);
int expression(int *expression);
int comparison(int *comparison);
int term(int *term);
int unary(int *unary);
int primary(int primary);

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





