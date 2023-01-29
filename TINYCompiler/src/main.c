#include "../include/lexer.h"

int main(int argc, char **argv){
    printf("> ");

    lexer_line_t lexer_line;
    lexer_parsed_line_t lexer_parsed_line;
    lexer_converted_t lexer_converted;

    read_input(&lexer_line);
    printf("%s\n", lexer_line);

    parse_line(lexer_line.line, &lexer_parsed_line);
    printf("%i\n", lexer_parsed_line.parsed_line_length);

    converter(lexer_parsed_line.parsed_line, &lexer_converted);
    for (int i=0; i<lexer_converted.converted_length;i++) {
        printf("%i\n", lexer_converted.converted[i]);
    }
}

// 105 2 201 1