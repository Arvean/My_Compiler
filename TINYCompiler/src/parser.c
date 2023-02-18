#include "../include/parser.h"
#include "../include/lexer.h"

operators_t operators[] = {
    {"=", 201}, {"+", 202}, {"-", 203}, {"*", 204}, {"/", 205},
    {"==", 206}, {"!=", 207}, {"<", 208}, {"<=", 209}, {">", 210}, {">=", 211}, 
};
int operators_length = sizeof(operators)/sizeof(operators[0]);

int program(int *converted, int converted_length, int *itr){
    if (!statement(converted, converted_length, itr)) {
        printf("%s\n", "STATEMENT");
        return 0;
    }
    else {
        perror("Error with program");
        return 1;
    }
}

int statement(int *converted, int converted_length, int *itr){
    int i;
    switch(converted[*itr]){
        case 103:  // PRINT
            
            (*itr)++;
            if (converted[1] == 3) {
                printf("%s\n", "STRING");
                return 0;
            }
            else if (!expression(converted, itr)){
                return 0; // Correct PRINT grammar
            }
            else {
                perror("Error with PRINT in statement");
                return 1;
            }
        
        case 106: // IF

            
            (*itr)++;
            printf("%s\n", "after if");
            printf("%i\n", converted[*itr]);
            if (comparison(converted, itr)) {
                perror("Comparison Error, IF");
                return 1;
            }

            if (converted[*itr] != 107) { // THEN
                printf("%s\n", "BUG");
                printf("%i\n", converted[*itr]);
                perror("Incomplete IF statement, THEN");
                return 1;
            }
            
            (*itr)++;
            printf("%i\n", converted[*itr]);

            if (statement(converted, converted_length, itr)) {
                perror("Statement Error, IF");
                return 1;
            }

            if (converted[*itr] != 108) { // ENDIF
                perror("Incomplete IF statement, ENDIF");
                return 1;
            }
            printf("%s\n", "IF");
            return 0;

        case 109: // WHILE

            if (converted[converted_length-2] != 111) { // END WHILE
                perror("Incomplete WHILE statement, ENDWHILE");
                return 1;
            }
            
            (*itr)++;
            if (comparison(converted, itr)) {
                perror("Comparison Error, WHILE");
                return 1;
            }

            if (converted[*itr] != 110) { // REPEAT
                perror("Incomplete WHILE statement, REPEAT");
                return 1;
            }
            
            (*itr)++;

            if (statement(converted, converted_length, itr)) {
                perror("Statement Error, WHILE");
                return 1;
            }

            if (converted[*itr] != 111) { // ENDWHILE
                perror("Incomplete WHILE statement, ENDWHILE");
                return 1;
            }
            printf("%s\n", "WHILE");
            return 0;

        case 105: // LET
            
            (*itr)++;
            if (converted[*itr] != 2) { // IDENT
                perror("Not IDENT in LET");
                return 1;
            }
            
            (*itr)++;
            if (converted[*itr] != 201) { // =
                perror("No = in LET");
                return 1;
            }
            
            (*itr)++;

            if (expression(converted, itr)) {
                perror("Comparison Error, WHILE");
                return 1;
            }

            printf("%s\n", "LET");
            return 0;

        case 104: // INPUT
            
            (*itr)++;

            if (converted[*itr] != 2) { // IDENT
                printf("%s\n", "NO IDENT AFTER INPUT");
                return 1;
            }
            printf("%s\n", "INPUT");
            return 0; // Correct INPUT grammar

        default:
            perror("Unkown identifier");
            return 1;
    }
}

int comparison(int *converted, int *itr){

    if (expression(converted, itr)) {
        perror("Expression Error, COMPARISON");
        return 1;
    }
    printf("%i\n", converted[*itr]);

    for (int i=0; i < operators_length; i++) {
        if (converted[*itr] == operators[i].value) {
            printf("%s\n", "found operator");
            printf("%i\n", converted[*itr]);
            break;
        }
        if (i == operators_length-1) {
            printf("%i\n", converted[*itr]);
            perror("Expected operator, COMPARISON");
            return 1;
        }
    }

    (*itr)++;

    if (expression(converted, itr)) {
        perror("Expression Error, COMPARISON");
        return 1;
    }
    printf("%s\n", "COMPARISON");
    return 0;
}


int expression(int *converted, int *itr){

    if (term(converted, itr)) {
        perror("Term Error, EXPRESSION");
        return 1;
    }

    while (converted[*itr+1] == 202 || converted[*itr+1] == 203){ // +,-
        (*itr)+=2;
        if (term(converted, itr)) {
            perror("Term Error, EXPRESSION");
            return 1;
        }
    }
    printf("%i\n", converted[*itr]);
    printf("%s\n", "EXPRESSION");
    return 0;
}

int term(int *converted, int *itr){

    if (unary(converted, itr)) {
        perror("Unary Error, TERM");
        return 1;
    }

    while (converted[*itr+1] == 204 || converted[*itr+1] == 205){ // *,/
        (*itr)+=2;
        if (unary(converted, itr)) {
            perror("Unary Error, TERM");
            return 1;
        }
    }
    printf("%i\n", converted[*itr]);
    printf("%s\n", "TERM");
    return 0;
}

int unary(int *converted, int *itr){

    if (converted[*itr] == 202 || converted[*itr] == 203){ // +,-
        
        (*itr)++;
    }

    if (primary(converted, itr)) {
        perror("Primary Error, UNARY");
        return 1;
    }
    printf("%i\n", converted[*itr]);
    printf("%s\n", "UNARY");
    return 0;
}

int primary(int *converted, int *itr){

    if (converted[*itr] == 1 || converted[*itr] == 2){ // NUMBER, IDENT
        
        (*itr)++;
        printf("%i\n", converted[*itr]);
        printf("%s\n", "PRIMARY");
        return 0;
    }
    perror("Error with PRIMARY, not the right data type");
    return 1;
}

int checkGrammer(lexer_converted_t* lexer_converted, int (*function_1)(int*), int (*function_2)(int*), int *buffer_1, int *buffer_2, int label_1, int label_2){
    // Function to break up lexer based on the second term to provide as input to functions specific to the grammar rule. Checks grammar

    // DEFINE BUFFER SIZES IN MAIN (PLACE IN MAIN)
    //*buffer_1 = TOKEN_BUFSIZE*converted_length;
    //*buffer_2 = TOKEN_BUFSIZE*converted_length;

    // Check grammar of IF statement 
    int i = 1; // CONTINUE: Change based on rule
    if (lexer_converted->converted[0] == 106 || lexer_converted->converted[0] == 109){ // IF, WHILE
        if (lexer_converted->converted[lexer_converted->converted_length-2] == label_1) { // ENDIF, ENDWHILE
            perror("Incomplete END to conditional statement or loop");
            return 1;
        }
    }
    // CONTINUE: Automate instead of passing as parameter label 1 and label 2 checks
    while (lexer_converted->converted[i] != label_2) { // THEN, REPEAT, ==, etc.
        buffer_1[i-1] = lexer_converted->converted[i];
        i++;
        if (lexer_converted->converted[i] == label_1) { // ENDIF, ENDWHILE, NULL (THEN, REPEAT, ==, etc. doesn't exist)
            perror("Incomplete Second Term (THEN, REPEAT, etc.)");
            return 1;
        }
    }
    while (lexer_converted->converted[i] != label_1) { // ENDIF, ENDWHILE, NULL
        buffer_2[i-1] = lexer_converted->converted[i];
        i++;
    }

    if (!function_1(buffer_1)){ //Check if both the function 1 (comparison, etc.) and function 2 (statement, etc.) are valid
        printf("%s\n", "Buffer_1");
        if (!function_2(buffer_2)){
            printf("%s\n", "Buffer_2");
            return 0; // Return 0 indicating correct grammar check
        }
        else {
            perror("Error with Buffer_1 (IF_STATEMENT, etc.)");
            return 1;
        }
    }
    else {
        perror("Error with Buffer_2 (IF_COMPARISON, etc.)");
        return 1;
    }
}