%{
#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

extern int32_t line_num;  /* declared in scanner.l */
extern char buffer[];     /* declared in scanner.l */
extern FILE *yyin;        /* declared by lex */
extern char *yytext;      /* declared by lex */

extern int yylex(void); 
static void yyerror(const char *msg);
%}

%token COMMA SEMI COLON R_B L_B R_P L_P
%token PLUS_OP MINUS_OP STAR_OP SLASH_OP MOD_OP ASSIGN_OP GT_OP LT_OP LE_OP NEQ_OP GE_OP EQ_OP AND_OP OR_OP NOT_OP
%token VAR ARRAY OF BOOLEAN INTEGER REAL STRING TRUE FALSE DEF RETURN KWBEGIN END WHILE DO IF THEN ELSE FOR TO PRINT READ
%token ID
%token DEC_INT OCT_INT STRING_ID FLOAT_NUM SCIENTIFIC_NUM




%left AND_OP OR_OP NOT_OP
%left LT_OP LE_OP NEQ_OP GE_OP EQ_OP GT_OP
%left MINUS_OP
%left PLUS_OP
%left MOD_OP SLASH_OP
%left STAR_OP

%%

ProgramName: ID SEMI zero_or_variable_const_declaration zero_or_function_declaration compound_statement END;

zero_or_variable_const_declaration: variable_const_list 
                                    | 
                                    ;
variable_const_list: variable_const_list variable_declaration
                    | variable_const_list const_declaration
                    | variable_declaration
                    | const_declaration
                    ;
zero_or_function_declaration: function_declaration_definition_list
                            |
                            ;

function_declaration_definition_list: function_declaration_definition_list function_declaration
                                    | function_declaration_definition_list function_definition
                                    | function_declaration
                                    | function_definition
                                    ;

function_definition: ID L_P zero_or_arguments_declaration R_P compound_statement END
                    | ID L_P zero_or_arguments_declaration R_P COLON scalar compound_statement END
                    ;

function_declaration: ID L_P zero_or_arguments_declaration R_P SEMI 
                    | ID L_P zero_or_arguments_declaration R_P COLON scalar SEMI
                    ;
zero_or_arguments_declaration: arguments_declaration
                            |
                            ;
arguments_declaration: arguments_declaration arguments
                    | arguments
                    ;
arguments: id_list COLON scalar
        ;
id_list: ID COMMA id_list
        | ID
        ;




const_declaration: VAR id_list COLON const_literal SEMI
;
const_integer: DEC_INT
            | OCT_INT
            ;
const_literal: DEC_INT 
            | OCT_INT
            | FLOAT_NUM
            | SCIENTIFIC_NUM
            | STRING_ID
            | FALSE
            | TRUE
            ;

variable_declaration: VAR id_list COLON ARRAY const_integer OF type SEMI
                    | VAR id_list COLON scalar SEMI
                    ;
type: ARRAY const_integer OF type
    | scalar
    ;
scalar: INTEGER
    | REAL
    | STRING
    | BOOLEAN
    ;


compound_statement: KWBEGIN zero_or_variable_const_declaration zero_or_more_statements END;
zero_or_more_statements: more_statements
                        |
                        ;
more_statements: more_statements statements
                | statements
                ;
statements: compound_statement
        | basic_statement
        | for_loop_statement
        | return_statement
        | while_loop_statement
        | if_else_statement
        | function_call_statement
        ;
basic_statement: variable_name ASSIGN_OP expression SEMI
                | PRINT expression SEMI
                | PRINT variable_name SEMI
                | READ variable_name SEMI
                ;
for_loop_statement: FOR ID ASSIGN_OP const_integer TO const_integer DO compound_statement END DO ;
return_statement: RETURN expression SEMI;
while_loop_statement: WHILE expression DO compound_statement END DO ;
if_else_statement: IF expression THEN compound_statement END IF
                | IF expression THEN compound_statement ELSE compound_statement END IF
                ;
function_call_statement: ID L_P expressions_by_comma_or_not R_P SEMI ;
expressions_by_comma_or_not: expressions_by_comma
                            |
                            ;
expressions_by_comma: expression COMMA expressions_by_comma
                    | expression
                    ;

variable_name: ID index
            | ID 
            ;
index: index L_B expression R_B
    | L_B expression R_B
    ;


expression: expression OR_OP expression
            | expression AND_OP expression
            | NOT_OP expression
            | expression GE_OP expression
            | expression GT_OP expression
            | expression EQ_OP expression
            | expression LE_OP expression
            | expression LT_OP expression
            | expression NEQ_OP expression
            | expression PLUS_OP expression
            | expression MINUS_OP expression
            | expression SLASH_OP expression
            | expression MOD_OP expression
            | expression STAR_OP expression
            | MINUS_OP expression %prec STAR_OP
            | L_P expression R_P
            | const_literal
            | variable_name
            | function_call
            ;

function_call: ID L_P expressions_by_comma_or_not R_P




%%

void yyerror(const char *msg) {
    fprintf(stderr,
            "\n"
            "|-----------------------------------------------------------------"
            "---------\n"
            "| Error found in Line #%d: %s\n"
            "|\n"
            "| Unmatched token: %s\n"
            "|-----------------------------------------------------------------"
            "---------\n",
            line_num, buffer, yytext);
    exit(-1);
}

int main(int argc, const char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: ./parser <filename>\n");
        exit(-1);
    }

    yyin = fopen(argv[1], "r");
    if (yyin == NULL) {
        perror("fopen() failed:");
    }

    yyparse();

    printf("\n"
           "|--------------------------------|\n"
           "|  There is no syntactic error!  |\n"
           "|--------------------------------|\n");
    return 0;
}
