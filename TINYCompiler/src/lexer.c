#include "../include/lexer.h"
char input[BUFSIZE];

char parsed_line_container[TOKEN_BUFSIZE][BUFSIZE];
char token_container[BUFSIZE];
char* token = token_container;

int converted_container[TOKEN_BUFSIZE];

void read_input(lexer_line_t* lexer_line){
    // Read an input from the command line of the shell
    // Returns a pointer to the line array
    lexer_line->line = input;

    int c;
    int i = 0;

    while (1) {
        // Read a character
        c = getchar();

        // If we hit EOF, replace it with a null character and return.
        if (c == EOF || c == '\n') {
            lexer_line->line[i] = '\0';
            break;
        } else {
            lexer_line->line[i] = (char) c;
        }
        i++;

        // If we have exceeded the buffer, reallocate.
        if (i >= BUFSIZE) {
            fprintf(stderr, "lsh: allocation error\n");
            exit(EXIT_FAILURE);
        }
    }
}

void parse_line(char* const line, lexer_parsed_line_t* lexer_parsed_line) {

    lexer_parsed_line->parsed_line = parsed_line_container;
    int i = 0;

    token = strtok(line, TOK_DELIM);

    while (token != NULL) {
        lexer_parsed_line->parsed_line[i] = token;
        i++;

        if (i >= TOKEN_BUFSIZE) {
            fprintf(stderr, "lsh: allocation error\n");
            exit(EXIT_FAILURE);
        }
        token = strtok(NULL, TOK_DELIM);
    }
    lexer_parsed_line->parsed_line[i] = NULL;
    lexer_parsed_line->parsed_line_length = i;
}

void converter(char** const parsed_line, lexer_converted_t* lexer_converted) {
    
    int j = 0;
    lexer_converted->converted = converted_container;

    keyword_t keywords[] = {
        {"KEYWORD", 102},
        {"PRINT", 103},
        {"INPUT", 104},
        {"LET", 105},
        {"IF", 106},
        {"THEN", 107},
        {"ENDIF", 108},
        {"WHILE", 109},
        {"REPEAT", 110},
        {"ENDWHILE", 111},
    };
    operators_t operators[] = {
        {"=", 201},
        {"+", 202},
        {"-", 203},
        {"*", 204},
        {"/", 205},
        {"==", 206},
        {"!=", 207},
        {"<", 208},
        {"<=", 209},
        {">", 210},
        {">=", 211},
    };
    map_t map[] = {
        {"\n", 0},
        {"NUMBER", 1},
        {"IDENT", 2},
        {"", 3},
    };

    int keywords_length = sizeof(keywords)/sizeof(keywords[0]);
    int operators_length = sizeof(operators)/sizeof(operators[0]);
    int map_length = sizeof(map)/sizeof(map[0]);
    
    while (parsed_line[j] != NULL){
        if (isalpha(parsed_line[j][0])) {
            lexer_converted->converted[j] = map[2].value;
            for (int i = 0; i < keywords_length; i++) {
                if (strcmp(parsed_line[j], keywords[i].key) == 0) {
                    lexer_converted->converted[j] = keywords[i].value;
                    break;
                }
            }
        }
        else if (isdigit(parsed_line[j][0])) {
            lexer_converted->converted[j] = map[1].value; //Set to NUMBER value
        }
        else if (strcmp(parsed_line[j], "\n") == 0) {
            lexer_converted->converted[j] = map[0].value; //Set to NEWLINE value
        }
        else {
            for (int i = 0; i < operators_length; i++) {
                if (strcmp(parsed_line[j], operators[i].key) == 0) {
                    lexer_converted->converted[j] = operators[i].value;
                    break;
                }
            }
        }
        j++;
    }

    lexer_converted->converted[j] = '\0';
    lexer_converted->converted_length = j;
}

// Convert each token for each line for all lines in the file to the lexed classifier
// Lexed classified array for each line