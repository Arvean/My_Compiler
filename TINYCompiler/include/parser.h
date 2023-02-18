#ifndef PARSER_H
#define PARSER_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define BUFSIZE 1024
#define TOKEN_BUFSIZE 64


int program(int *converted, int converted_length, int *itr);
int statement(int *converted, int converted_length, int *itr);
int expression(int *converted, int *itr);
int comparison(int *converted, int *itr);
int term(int *converted, int *itr);
int unary(int *converted, int *itr);
int primary(int* converted, int *itr);

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





