
Hello! Welcome to Arvean's Compiler. 
Follow the steps below:

1. Write your code in user_input.txt. The grammar rules are shown below
Note: Seperate all values with a space. Write IF and WHILE loops in one line (Next update will allow for multiple lines)
2. Compile with the following: gcc -g -o main src/main.c src/lexer.c src/parser.c. 
3. Run the lexer, parser, C emitter executable with ./main
4. Copy data written in output.txt to output.c.
5. Compile output.c with gcc -g -o main output.c 
6. Run the C executable with ./main

An example of an user_input.txt file is shown below Grammar Rules

  -------------------------------------------------------------------------
Grammar Rules:

    program ::= {statement}
    statement ::= "PRINT" (expression | string) nl
        | "IF" comparison "THEN" nl {statement} "ENDIF" nl
        | "WHILE" comparison "REPEAT" nl {statement} "ENDWHILE" nl
        | "LET" ident "=" expression nl
        | "INPUT" ident nl
    comparison ::= expression (("==" | "!=" | ">" | ">=" | "<" | "<=") expression)+
    expression ::= term {( "-" | "+" ) term}
    term ::= unary {( "/" | "*" ) unary}
    unary ::= ["+" | "-"] primary
    primary ::= number | ident
    nl ::= '\n'+

  -------------------------------------------------------------------------

  (1) Example user_input.txt:

    LET x = 5
    IF x == 5 THEN LET x = x + 1 ENDIF

    WHILE x < 10 REPEAT LET x = x + 1 ENDWHILE

    PRINT x
    INPUT foobar
    PRINT foobar
  -------------------------------------------------------------------------

  (1) Example output.c:

    #include <stdlib.h>
    #include <stdio.h>
    int main(void){
    int x;
    int foobar; 
    x=5;
    if (x==5){
    x=x+1;
    }
    while (x<10){
    x=x+1;
    }
    printf("%i\n", x);
    scanf("%i", &foobar);
    printf("%i\n", foobar);
    return 0;
    }
  -------------------------------------------------------------------------

To DO:
 1. Instead of compiling line by line, store in buffer and compile buffer
  1.1. This is to allow for multiple line statements
  -------------------------------------------------------------------------
