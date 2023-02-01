#ifndef PARSER_H
#define PARSER_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>


int program(int *converted);
int statement(int *converted);
int expression(int *converted);
int comparison(int *converted);

int converted_length;
int* if_comparison;
int* if_statement;
int* while_comparison;
int* while_statement;



#endif





