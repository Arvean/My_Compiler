#include "../include/lexer.h"
char input[BUFSIZE];

char parsed_line_container[TOKEN_BUFSIZE][BUFSIZE];
char token_container[BUFSIZE];
char* token = token_container;

int converted_container[TOKEN_BUFSIZE];


int is_keyword(char const keywords[10][15], int size, char* const str, char (*keyword)[15]) {
    int i = 0;
    while (strcmp(keywords[i], "")!=0){
        if (strcmp(keywords[i], str) == 0) {
            memcpy(keyword, keywords[i], sizeof(keywords[i]));
            return 1;
        }
        i++;
    }
    return 0;
}

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
    
    int i = 0;
    lexer_converted->converted = converted_container;
    char const keywords[10][15] = {"PRINT", "INPUT", "LET", "IF", "THEN", "ENDIF", "WHILE", "REPEAT", "ENDWHILE", ""};
    int keywords_length = sizeof(keywords)/sizeof(keywords[0]);
    char keyword[15];
    

    while (parsed_line[i] != NULL){
        if (strcmp(parsed_line[i], "+") == 0) {
            enum MAP plus = PLUS;
            lexer_converted->converted[i] = plus;
        }
        else if (strcmp(parsed_line[i], "-") == 0) {
            enum MAP minus = MINUS;
            lexer_converted->converted[i] = minus;
        }
        else if (strcmp(parsed_line[i], "*") == 0) {
            enum MAP asterisk = ASTERISK;
            lexer_converted->converted[i] = asterisk;
        }
        else if (strcmp(parsed_line[i], "/") == 0) {
            enum MAP slash = SLASH;
            lexer_converted->converted[i] = slash;
        }
        else if (strcmp(parsed_line[i], "=") == 0) {
            enum MAP eq = EQ;
            lexer_converted->converted[i] = eq;
        }
        else if (strcmp(parsed_line[i], "==") == 0) {
            enum MAP eqeq = EQEQ;
            lexer_converted->converted[i] = eqeq;
        }
        else if (strcmp(parsed_line[i], "!=") == 0) {
            enum MAP noteq = NOTEQ;
            lexer_converted->converted[i] = noteq;
        }
        else if (strcmp(parsed_line[i], "<") == 0) {
            enum MAP lt = LT;
            lexer_converted->converted[i] = lt;
        }
        else if (strcmp(parsed_line[i], ">") == 0) {
            enum MAP gt = GT;
            lexer_converted->converted[i] = gt;
        }
        else if (strcmp(parsed_line[i], "<=") == 0) {
            enum MAP lteq = LTEQ;
            lexer_converted->converted[i] = lteq;
        }
        else if (strcmp(parsed_line[i], ">=") == 0) {
            enum MAP gteq = GTEQ;
            lexer_converted->converted[i] = gteq;
        }
        else if (strcmp(parsed_line[i], "\n") == 0) {
            enum MAP new_line = NEWLINE;
            lexer_converted->converted[i] = new_line;
        }
        else if (isdigit(parsed_line[i][0])) { // Check the first and last element for a string
            enum MAP number = NUMBER;
            lexer_converted->converted[i] = number;
        }
        else if (isalpha(parsed_line[i][0])){
            if (is_keyword(keywords, keywords_length, parsed_line[i], &keyword)) {
                if (strcmp(keyword, "LET") == 0){
                    enum MAP keyword = LET;
                    lexer_converted->converted[i] = keyword;
                }
            }
            else {
                enum MAP ident = IDENT; 
                lexer_converted->converted[i] = ident;
            }
        }
        i++;
    }
    enum MAP eof = ENDFILE;
    lexer_converted->converted[i] = eof;

    lexer_converted->converted_length = i;
}

// Convert each token for each line for all lines in the file to the lexed classifier
// Lexed classified array for each line