#include "../include/lexer.h"
#include "../include/parser.h"

void compile(lexer_line_t* lexer_line, lexer_parsed_line_t* lexer_parsed_line, lexer_converted_t* lexer_converted, FILE* output, FILE* body) {

    parse_line(lexer_line->line, lexer_parsed_line);
    printf("%i\n", lexer_parsed_line->parsed_line_length);

    converter(lexer_parsed_line->parsed_line, lexer_converted);
    for (int i=0; i<lexer_converted->converted_length;i++) {
        printf("%i\n", lexer_converted->converted[i]);
    }

    int temp = 0;
    int *itr = &temp;

    if (!program(lexer_converted->converted, lexer_converted->converted_length, lexer_parsed_line->parsed_line, itr, output, body)) {
        printf("%s\n", "PARSER SUCCESSFULL");
    }
    else {
        perror("Error with PARSER");
    }
}

int main(int argc, char **argv){
    printf("> ");

    FILE *clear_output = fopen("output.txt", "w"); // truncate existing output file
    fclose(clear_output); // close file
    FILE *clear_body = fopen("body.txt", "w"); // truncate existing body file
    fclose(clear_body); // close file

    FILE *output = fopen("output.txt", "a"); // For writing output C compiled code (include, declarations)
    FILE *body = fopen("body.txt", "a"); // For writing body C compiled code (main, definitions)

    fputs("#include <stdlib.h>\n", output);
    fputs("#include <stdio.h>\n", output);
    fputs("int main(void){\n", output);

    if (output == NULL) {
        printf("Error opening output file!");
        exit(1);
    }

    if (body == NULL) {
        printf("Error opening body file!");
        exit(1);
    }

    lexer_line_t lexer_line;
    int debug = 0;

    if (!debug) {
        FILE *fp;
        char txt_line[BUFSIZE];
        lexer_line.line = txt_line;

        fp = fopen("user_input.txt", "r"); 
        if (fp == NULL) {
            printf("Error opening file\n");
            return 1;
        }

        while (fgets(lexer_line.line, BUFSIZE, fp) != NULL) {

            size_t newline_pos = strcspn(lexer_line.line, "\n"); 
            if (lexer_line.line[newline_pos] == '\n') {
                lexer_line.line[newline_pos] = '\0';
            }
            lexer_parsed_line_t lexer_parsed_line;
            lexer_converted_t lexer_converted;

            compile(&lexer_line, &lexer_parsed_line, &lexer_converted, output, body);
        }
        fclose(fp);
    }
    else {
        read_input(&lexer_line, debug);

        lexer_parsed_line_t lexer_parsed_line;
        lexer_converted_t lexer_converted;

        compile(&lexer_line, &lexer_parsed_line, &lexer_converted, output, body);
    }
    fputs("return 0;\n", body);
    fputs("}\n", body);

    fclose(body);

    body = fopen("body.txt", "r");

    size_t nread;
    char buffer[1024];
        // Copy contents of first input file to output file.
    while ((nread = fread(buffer, 1, sizeof(buffer), body)) > 0) {
        fwrite(buffer, 1, nread, output);
    }

    fclose(output);
}