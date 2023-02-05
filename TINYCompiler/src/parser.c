#include "../include/parser.h"
#include "../include/lexer.h"

operators_t operators[] = {
    {"=", 201}, {"+", 202}, {"-", 203}, {"*", 204}, {"/", 205},
    {"==", 206}, {"!=", 207}, {"<", 208}, {"<=", 209}, {">", 210}, {">=", 211}, 
};
int operators_length = sizeof(operators)/sizeof(operators[0]);

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
                return 0; // Correct PRINT grammar
            }
            else {
                perror("Error with PRINT in statement");
                return 1;
            }
        
        case 106: // IF
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
                    return 0; // Correct IF grammar
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
                    return 0; // Correct WHILE grammar
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
            i = 3;
            if (converted[1] != 2) { // IDENT
                perror("NO IDENT AFTER LET");
                return 1;
            }
            else if (converted[2] != 201) { // =
                perror("NO = AFTER IDENT");
                return 1;
            }
            while (i < converted_length) {
                let_1[i-3] = converted[i];
                i++;
            }
            if (!expression(let_1)){
                printf("%s\n", "LET");
                return 0; // Correct LET grammar
            }
            perror("Error with LET in statement");
            return 1;

        case 104: // INPUT
            if (converted[1] != 2) { // IDENT
                printf("%s\n", "NO IDENT AFTER LET");
                return 1;
            }
            printf("%s\n", "INPUT");
            return 0; // Correct INPUT grammar

        default:
            perror("Unkown identifier");
            return 1;
    }
}

int comparison(int *comparison){
    int comparison_length = sizeof(comparison) / sizeof(comparison[0]);
    // Check grammar of COMPARISON statement 
    int i = 0; 
    int is_equality_operator = 0;

    while (!is_equality_operator) {
        for (int j = 0; j < operators_length; j++) {
            if (comparison[i] == operators[j].value) {
                is_equality_operator = 1;
                break;
            }
        }
        expression_1[i] = comparison[i];
        i++;
        if (i == comparison_length) {
            perror("No operator in comparison!");
            return 1;
        }
    }
    while (i < comparison_length) {
        expression_2[i] = comparison[i];
        i++;
    }

    if (!expression(expression_1)){ 
        printf("%s\n", "EXPRESSION_1");
        if (!expression(expression_2)){
            printf("%s\n", "EXPRESSION_2");
            return 0; // Correct EXPRESSION grammar 
        }
        else {
            perror("Error with EXPRESSION_2");
            return 1;
        }
    }
    else {
        perror("Error with EXPRESSION_1");
        return 1;
    }
}

int expression(int *expression){
    int expression_length = sizeof(expression) / sizeof(expression[0]);
    printf("%s\n", "EXPRESSION");
    // Check grammar of EXPRESSION 
    int i = -1; 
    int k = 0;

    while (i!= expression_length){
        i++;
        while (expression[k] != 202 || expression[k] != 203) { // +, -
            term_1[k] = expression[i];
            if (i == expression_length) {
                if (!term(term_1)) {
                    return 0;
                }
                else {
                    return 1;
                }
            } // BUG HERE: Did not account for {} (one or MORE) splitting the array up incorrectly
            k++;
            i++;
        } // FIX: Iterate element by element and check for no grammar faults
        if (term(term_1)) {
            perror("Expression fails term check");
            return 1;
        }
        if (i == expression_length) {
            perror("Need to define term after operator");
            return 1;
        }
        k = 0;
    }

    if (!term(term_1)){ 
        printf("%s\n", "TERM_1");
        if (!term(term_2)){
            printf("%s\n", "TERM_2");
            return 0; // Correct TERM grammar 
        }
        else {
            perror("Error with TERM_2");
            return 1;
        }
    }
    else {
        perror("Error with TERM_1");
        return 1;
    }
}

int term(int *term){
    int term_length = sizeof(term) / sizeof(term[0]);
    // Check grammar of EXPRESSION 
    int i = 0; 

    while (term[i] != 204 || term[i] != 205) { // *, /
        unary_1[i] = term[i];
        i++;
        if (i == term_length) {
            perror("No sign operator in term!");
            return 1;
        }
    }
    while (i < term_length) {
        unary_2[i] = term[i];
        i++;
    }

    if (!unary(unary_1)){ 
        printf("%s\n", "UNARY_1");
        if (!unary(unary_2)){
            printf("%s\n", "UNARY_2");
            return 0; // Correct UNARY grammar 
        }
        else {
            perror("Error with UNARY_2");
            return 1;
        }
    }
    else {
        perror("Error with UNARY_1");
        return 1;
    }
}

int unary(int *unary){
    int primary_1;
    int i = 1;

    if (unary[0] != 202 || unary[0] != 203){
        perror("Error with UNARY, no sign");
        return 1;
    }
    primary_1 = unary[1];

    if (!primary(primary_1)){
        printf("%s\n", "PRIMARY_1");
        return 0; // Correct UNARY grammar 
    }
    else {
        perror("Error with PRIMARY_1");
        return 1;
    }
}

int primary(int primary){
    if (primary == 1 || primary == 2){ // NUMBER, IDENT
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