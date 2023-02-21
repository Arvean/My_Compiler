#include "../include/parser.h"
#include "../include/lexer.h"

operators_t operators[] = {
    {"=", 201}, {"+", 202}, {"-", 203}, {"*", 204}, {"/", 205},
    {"==", 206}, {"!=", 207}, {"<", 208}, {"<=", 209}, {">", 210}, {">=", 211}, 
};
int operators_length = sizeof(operators)/sizeof(operators[0]);

declared_t declared_idents;
int declare_size = 0;

char* insert_buffer(char *buffer_ptr, char **parsed_line, int condition_start, int condition_end) {
    for (int i = condition_start; i < condition_end; i++) {
        strncpy(buffer_ptr, parsed_line[i], strlen(parsed_line[i]));
        buffer_ptr += strlen(parsed_line[i]);
    }
    return buffer_ptr;
}

int is_declared(char *identifier) {

    declared_idents.declared_list_length = &declare_size;
    int i;
    for (i=0; i < *declared_idents.declared_list_length; i++) {
        if (strcmp(identifier, declared_idents.declare_container[i]) == 0) {
            return 1;
        }
    }

    strcpy(declared_idents.declare_container[i], identifier);
    (*declared_idents.declared_list_length)++;
    return 0;
}

int program(int *converted, int converted_length, char **parsed_line, int *itr, FILE* output, FILE* body){
    
    if (converted_length == 0) {
        printf("%s\n", "STATEMENT");
        return 0;
    }

    if (!statement(converted, converted_length, parsed_line, itr, output, body)) {
        printf("%s\n", "STATEMENT");
        return 0;
    }
    else {
        perror("Error with program");
        return 1;
    }
}

int statement(int *converted, int converted_length, char **parsed_line, int *itr, FILE* output, FILE* body){
    int i;
    char buffer[500];

    switch(converted[*itr]){
        case 103:  // PRINT
        
            (*itr)++;
            int condition_start = *itr;
            if (converted[*itr] == 3) {
                sprintf(buffer, "printf(\"%s\");\n", parsed_line[*itr]); // Format string

                fputs(buffer, body);
                printf("%s\n", "STRING");
                return 0;
            }
            
            else if (!expression(converted, parsed_line, itr, output, body)){
                char *buffer_ptr = (char *) buffer;
                int condition_end = *itr;
                char print_temp[100] = "printf(\"%i\\n\", ";
                char *tmp_ptr = (char *) print_temp;
                char print_buf[100];
                char *print_ptr = (char *) print_buf;

                buffer_ptr = insert_buffer(buffer_ptr, parsed_line, condition_start, condition_end);
                *buffer_ptr = '\0';

                strncpy(print_buf, tmp_ptr, strlen(tmp_ptr));
                print_ptr+=strlen(tmp_ptr);

                sprintf(print_ptr, "%s);\n", buffer); // Format string
                fputs(print_buf, body);

                return 0; // Correct PRINT grammar
            }
            else {
                perror("Error with PRINT in statement");
                return 1;
            }
        
        case 106: // IF
            
            (*itr)++;
            condition_start = *itr;
            sprintf(buffer, "if (%s", parsed_line[*itr]); // Format string
            condition_start++;
            
            char *buffer_ptr = (char *) buffer;
            buffer_ptr += strlen(buffer);

            printf("%i\n", converted[*itr]);
            if (comparison(converted, parsed_line, itr, output, body)) {
                perror("Comparison Error, IF");
                return 1;
            }

            int condition_end = *itr;
            buffer_ptr = insert_buffer(buffer_ptr, parsed_line, condition_start, condition_end);
            
            *buffer_ptr = ')';
            buffer_ptr++;
            *buffer_ptr = '{';
            buffer_ptr++;
            *buffer_ptr = '\n';
            buffer_ptr++;
            *buffer_ptr = '\0';

            fputs(buffer, body);
            memset(buffer, 0, sizeof(buffer));
            buffer_ptr = (char *) buffer;

            if (converted[*itr] != 107) { // THEN
                perror("Incomplete IF statement, THEN");
                return 1;
            }
            (*itr)++;

            printf("%i\n", converted[*itr]);
            if (statement(converted, converted_length, parsed_line, itr, output, body)) {
                perror("Statement Error, IF");
                return 1;
            }

            fputs("}\n", body);

            if (converted[*itr] != 108) { // ENDIF
                perror("Incomplete IF statement, ENDIF");
                return 1;
            }
            printf("%s\n", "IF");
            return 0;

        case 109: // WHILE
            
            (*itr)++;
            condition_start = *itr;
            sprintf(buffer, "while (%s", parsed_line[*itr]); // Format string
            condition_start++;

            buffer_ptr = (char *) buffer;
            buffer_ptr += strlen(buffer);

            printf("%i\n", converted[*itr]);
            if (comparison(converted, parsed_line, itr, output, body)) {
                perror("Comparison Error, WHILE");
                return 1;
            }

            condition_end = *itr;
            buffer_ptr = insert_buffer(buffer_ptr, parsed_line, condition_start, condition_end);
            
            *buffer_ptr = ')';
            buffer_ptr++;
            *buffer_ptr = '{';
            buffer_ptr++;
            *buffer_ptr = '\n';
            buffer_ptr++;
            *buffer_ptr = '\0';

            fputs(buffer, body);
            memset(buffer, 0, sizeof(buffer));
            buffer_ptr = (char *) buffer;

            if (converted[*itr] != 110) { // REPEAT
                perror("Incomplete WHILE statement, REPEAT");
                return 1;
            }
            
            (*itr)++;

            printf("%i\n", converted[*itr]);
            if (statement(converted, converted_length, parsed_line, itr, output, body)) {
                perror("Statement Error, WHILE");
                return 1;
            }

            fputs("}\n", body);

            if (converted[*itr] != 111) { // ENDWHILE
                printf("%i\n", converted[*itr]);
                perror("Incomplete WHILE statement, ENDWHILE");
                return 1;
            }
            printf("%s\n", "WHILE");
            return 0;

        case 105: // LET

            (*itr)++;
            buffer_ptr = (char *) buffer;
            condition_start = *itr;

            printf("%i\n", converted[*itr]);
            if (converted[*itr] != 2) { // IDENT
                perror("Not IDENT in LET");
                return 1;
            }
            
            (*itr)++;
            printf("%i\n", converted[*itr]);
            if (converted[*itr] != 201) { // =
                perror("No = in LET");
                return 1;
            }
            
            (*itr)++;

            printf("%i\n", converted[*itr]);
            if (expression(converted, parsed_line, itr, output, body)) {
                perror("Comparison Error, WHILE");
                return 1;
            }
            condition_end = *itr;

            buffer_ptr = insert_buffer(buffer_ptr, parsed_line, condition_start, condition_end);
            *buffer_ptr = '\0';

            if (converted[*itr-1] == 1) { // NUMBER
                if (!is_declared(parsed_line[condition_start])) {
                    char let_buf[500];
                    char *let_ptr = (char *) let_buf;
                    sprintf(let_buf, "%s", "int "); 
                    let_ptr += strlen(let_buf);
                    strncpy(let_ptr, parsed_line[condition_start], strlen(parsed_line[condition_start]));
                    let_ptr += strlen(parsed_line[condition_start]);
                    *let_ptr = ';';
                    let_ptr++;
                    *let_ptr = '\n';
                    let_ptr++;
                    *let_ptr = '\0';
                    fputs(let_buf, output);
                }
            }
            *buffer_ptr = ';';
            buffer_ptr++;
            *buffer_ptr = '\n';
            buffer_ptr++;
            *buffer_ptr = '\0';
            fputs(buffer, body);

            printf("%s\n", "LET");
            return 0;

        case 104: // INPUT
            
            (*itr)++;

            if (converted[*itr] != 2) { // IDENT
                printf("%s\n", "NO IDENT AFTER INPUT");
                return 1;
            }

            sprintf(buffer, "int %s; \n", parsed_line[*itr]);
            fputs(buffer, output);

            char scanf_buffer[100];
            char *scan_ptr = (char *) scanf_buffer;
            char input_temp[100] = "scanf(\"%i\", ";
            char *tmp_ptr = (char *) input_temp;

            strncpy(scanf_buffer, tmp_ptr, strlen(tmp_ptr));
            scan_ptr+=strlen(tmp_ptr);
            sprintf(scan_ptr, "&%s);\n", parsed_line[*itr]);

            fputs(scanf_buffer, body);

            printf("%s\n", "INPUT");
            return 0; // Correct INPUT grammar

        default:
            perror("Unkown identifier");
            return 1;
    }
}

int comparison(int *converted, char **parsed_line, int *itr, FILE* output, FILE* body){

    if (expression(converted, parsed_line, itr, output, body)) {
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

    if (expression(converted, parsed_line, itr, output, body)) {
        perror("Expression Error, COMPARISON");
        return 1;
    }
    printf("%s\n", "COMPARISON");
    return 0;
}


int expression(int *converted, char **parsed_line, int *itr, FILE* output, FILE* body){

    printf("%i\n", converted[*itr]);
    if (term(converted, parsed_line, itr, output, body)) {
        perror("Term Error, EXPRESSION");
        return 1;
    }

    while (converted[*itr] == 202 || converted[*itr] == 203){ // +,-
        (*itr)++;
        printf("%i\n", converted[*itr]);
        if (term(converted, parsed_line, itr, output, body)) {
            perror("Term Error, EXPRESSION");
            return 1;
        }
    }
    printf("%i\n", converted[*itr]);
    printf("%s\n", "EXPRESSION");
    return 0;
}

int term(int *converted, char **parsed_line, int *itr, FILE* output, FILE* body){

    printf("%i\n", converted[*itr]);
    if (unary(converted, parsed_line, itr, output, body)) {
        perror("Unary Error, TERM");
        return 1;
    }

    while (converted[*itr+1] == 204 || converted[*itr+1] == 205){ // *,/
        (*itr)+=2;
        printf("%i\n", converted[*itr]);
        if (unary(converted, parsed_line, itr, output, body)) {
            perror("Unary Error, TERM");
            return 1;
        }
    }
    printf("%i\n", converted[*itr]);
    printf("%s\n", "TERM");
    return 0;
}

int unary(int *converted, char **parsed_line, int *itr, FILE* output, FILE* body){

    if (converted[*itr] == 202 || converted[*itr] == 203){ // +,-
        
        (*itr)++;
    }

    printf("%i\n", converted[*itr]);
    if (primary(converted, parsed_line, itr, output, body)) {
        perror("Primary Error, UNARY");
        return 1;
    }
    printf("%i\n", converted[*itr]);
    printf("%s\n", "UNARY");
    return 0;
}

int primary(int *converted, char **parsed_line, int *itr, FILE* output, FILE* body){
    printf("%i\n", converted[*itr]);
    if (converted[*itr] == 1 || converted[*itr] == 2){ // NUMBER, IDENT
        
        (*itr)++;
        printf("%i\n", converted[*itr]);
        printf("%s\n", "PRIMARY");
        return 0;
    }
    perror("Error with PRIMARY, not the right data type");
    return 1;
}