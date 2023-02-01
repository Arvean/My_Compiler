#include "../include/parser.h"
#include "../include/lexer.h"

int program(int *converted){
    if (!statement(converted)) {
        printf("%s\n", "STATEMENT");
        return 0;
    }
    else {
        perror("Error with program");
        return 1;
    }
}

int statement(int *converted){
    int converted_length = sizeof(converted)/sizeof(int);
    int i;
    switch(converted[0]){
        case 103:  // PRINT
            if (converted[1] == 3) {
                printf("%s\n", "STRING");
                return 0;
            }
            else if (!expression(converted)){
                return 0;
            }
            else {
                perror("Error with PRINT in statement");
                return 1;
            }
        
        case 106: // IF
            *if_comparison = TOKEN_BUFSIZE*converted_length;
            *if_statement = TOKEN_BUFSIZE*converted_length;
            // Check grammar of IF statement 
            i = 1; // Start with 1 to not include IF in the comparison at the 0 index
            if (converted[converted_length-2] == 108) { // ENDIF
                perror("Incomplete IF statement, ENDIF");
                return 1;
            }
            while (converted[i] != 107) { // THEN
                if_comparison[i-1] = converted[i];
                i++;
                if (converted[i] == 108) { // If there is no "THEN", ENDIF
                    perror("Incomplete IF statement, THEN");
                    return 1;
                }
            }
            while (converted[i] != 108) { // ENDIF
                if_statement[i-1] = converted[i];
                i++;
            }

            if (!comparison(if_comparison)){ //Check if both the comparison and the statement are valid
                printf("%s\n", "IF_COMPARISON");
                if (!statement(if_statement)){
                    printf("%s\n", "IF_STATEMENT");
                    return 0; // Return 0 indicating correct grammar check
                }
                else {
                    perror("Error with IF_STATEMENT");
                    return 1;
                }
            }
            else {
                perror("Error with IF_COMPARISON");
                return 1;
            }

        case 109: // WHILE
            *while_comparison = TOKEN_BUFSIZE*converted_length;
            *while_statement = TOKEN_BUFSIZE*converted_length;
            // Check grammar of WHILE statement 
            i = 1; // Start with 1 to not include WHILE in the comparison at the 0 index
            if (converted[converted_length-2] == 111) { // ENDWHILE
                perror("Incomplete WHILE statement, ENDWHILE");
                return 1;
            }
            while (converted[i] != 110) { // REPEAT
                while_comparison[i-1] = converted[i];
                i++;
                if (converted[i] == 111) { // If there is no "REPEAT", ENDWHILE
                    perror("Incomplete WHILE statement, REPEAT");
                    return 1;
                }
            }
            while (converted[i] != 111) { // ENDWHILE
                while_statement[i-1] = converted[i];
                i++;
            }

            if (!comparison(while_comparison)){ //Check if both the comparison and the statement are valid
                printf("%s\n", "WHILE_COMPARISON");
                if (!statement(while_statement)){
                    printf("%s\n", "WHILE_STATEMENT");
                    return 0; // Return 0 indicating correct grammar check
                }
                else {
                    perror("Error with WHILE_STATEMENT");
                    return 1;
                }
            }
            else {
                perror("Error with WHILE_COMPARISON");
                return 1;
            }

        case 105: // LET
            if (converted[1] != 2) { // IDENT
                perror("NO IDENT AFTER LET");
                return 1;
            }
            else if (converted[2] != 201) { // =
                perror("NO = AFTER IDENT");
                return 1;
            }
            else if (!expression(converted)){
                printf("%s\n", "LET");
                return 0;
            }
            perror("Error with PRINT in statement");
            return 1;

        case 104: // INPUT
            if (converted[1] != 2) { // IDENT
                printf("%s\n", "NO IDENT AFTER LET");
                return 1;
            }
            printf("%s\n", "INPUT");
            return 0;

        default:
            perror("Unkown identifier");
            return 1;
    }
}

int comparison(int *converted){
    printf("%s\n", "COMPARISON");

    return 0;
}

int expression(int *converted){
    printf("%s\n", "EXPRESSION");
    return 0;
}


int checkGrammer(lexer_converted_t* lexer_converted, int (*function_1)(int*), int (*function_2)(int*), int *buffer_1, int *buffer_2, int label_1, int label_2){
    // Function to break up lexer based on the second term to provide as input to functions specific to the grammar rule. Checks grammar

    // DEFINE BUFFER SIZES IN MAIN (PLACE IN MAIN)
    //*buffer_1 = TOKEN_BUFSIZE*converted_length;
    //*buffer_2 = TOKEN_BUFSIZE*converted_length;

    // Check grammar of IF statement 
    int i = 1; // Start with 1 to not include IF in the comparison at the 0 index
    if (lexer_converted->converted[0] == 106 || lexer_converted->converted[0] == 109){ // IF, WHILE
        if (lexer_converted->converted[lexer_converted->converted_length-2] == label_1) { // ENDIF, ENDWHILE
            perror("Incomplete END to conditional statement or loop");
            return 1;
        }
    }

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