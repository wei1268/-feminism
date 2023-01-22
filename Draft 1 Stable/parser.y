%{
#include "ASTs.c"

void all_evaluate(struct generic_ast* head){
    // printf("HERE5\n");
    evaluate(head);
    eval_functions();
    printf("HERE8\n");
    fflush(stdout);
}

void all_output(struct generic_ast* head){
    grand_print(head);
}

void yyerror(const char* s);
extern int yylex();
extern int yyparse();
extern FILE* yyin;

%}

%union{
    char* txt;
    int op;
    struct generic_ast* val;
}

%token <txt> ID REAL INTEGER

%token <op> COMP

%token ARROW
%token out_open out_close out_open_nl out_close_nl

%left COMP
%left '%'
%left '+' '-'
%left '*' '/'

%type <val> decl_stmt_exp_list decleartion function_decleration variable_decleration expression statement while_loop if_statement if_then_list

%start start

%%

start: decl_stmt_exp_list                                           {all_evaluate($1);all_output($1);}
    ;

decl_stmt_exp_list: decleartion decl_stmt_exp_list                  {$$ = create_ast_generic_list($2, $1);}
    | expression decl_stmt_exp_list                                 {$$ = create_ast_generic_list($2, $1);}
    | statement decl_stmt_exp_list                                  {$$ = create_ast_generic_list($2, $1);}
    | statement                                                     {$$ = $1;}
    | expression                                                    {$$ = $1;}
    | decleartion                                                   {$$ = $1;}
    ;

decleartion: function_decleration                                   {$$ = $1;}
    | variable_decleration                                          {$$ = $1;}
    ;

function_decleration: ID '=' '(' decl_stmt_exp_list ')'             {$$ = create_generic_ast(); dcl_add(create_ast_procedure($4, $1));}
    ;

variable_decleration: expression ARROW ID                           {$$ = create_ast_decl($3, $1);}
    ;

expression: expression '+' expression                               {$$ = create_ast_fork(1, $1, $3);}
    | expression '-' expression                                     {$$ = create_ast_fork(2, $1, $3);}
    | expression '*' expression                                     {$$ = create_ast_fork(3, $1, $3);}
    | expression '/' expression                                     {$$ = create_ast_fork(4, $1, $3);}
    | expression '%' expression                                     {$$ = create_ast_fork(5, $1, $3);}
    | expression COMP expression                                    {$$ = create_ast_fork($2 + 5, $1, $3);}
    | '(' expression ')'                                            {$$ = $2;}
    | REAL                                                          {$$ = create_ast_value(2, $1);}
    | INTEGER                                                       {$$ = create_ast_value(1, $1);}
    | ID                                                            {$$ = create_ast_value(-1, $1);}
    ;

statement: while_loop                                               {$$ = $1;}
    | if_statement                                                  {$$ = $1;}
    | out_open expression out_close                                 {$$ = create_ast_out($2, 0);}
    | out_open_nl expression out_close_nl                           {$$ = create_ast_out($2, 1);}
    | '!' ID                                                        {$$ = create_ast_procedure_call($2);}
    ;

while_loop: '[' expression ']' '(' decl_stmt_exp_list ')'           {$$ = create_ast_while($2, $5);}

if_statement: '{' if_then_list '}'                                  {$$ = $2;}

if_then_list: expression ':' statement ',' if_then_list             {$$ = create_ast_if($1, $3, $5);}
    | expression ':' variable_decleration ',' if_then_list          {$$ = create_ast_if($1, $3, $5);}
    | statement                                                     {$$ = $1;}
    | variable_decleration                                          {$$ = $1;}
    |                                                               {$$ = create_ast_value(1,"-1;");}
    ;

%%

int main(int argc, char** argv){
    setup();
    if(argc > 2 || argc == 1){
	yyerror("File Argument Not Found or Too Many Arguments!");
    }

    FILE *fptr;

    fptr = fopen(argv[1], "r");

    if(!fptr){
        printf("FAILED TO OPEN");
        fflush(stdout);
        abort();
    }

    yyin = fptr;

    yyparse();
    /* printf("HELLO110"); */
    fflush(stdout);

    while(!feof(yyin)){
        yyparse();
    }

    return 0;
}

void yyerror(const char* s) {
    fprintf(stderr, "ERROR: %s (NMT: \"%s\")\n", s, yytext);
    exit(1);
}