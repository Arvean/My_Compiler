#include "../include/lexer.h"
#include "../include/parser.h"

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

    int temp = 0;
    int *itr = &temp;

    if (!program(lexer_converted.converted, lexer_converted.converted_length, itr)) {
        printf("%s\n", "PARSER SUCCESSFULL");
    }
    else {
        perror("Error with PARSER");
        return 1;
    }
}