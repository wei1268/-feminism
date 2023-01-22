%{
#include "parser.tab.c"

%}

%x CMT

%%

"{" |
"}" |
"(" |
")" |
"[" |
"]" |
":" |
"=" |
"*" |
"+" |
"/" |
"-" |
"%" |
"!" |
","                                             {return yytext[0];}

"<"												{yylval.op=1;return COMP;}
">"												{yylval.op=2;return COMP;}
"<="											{yylval.op=1;return COMP;}
">="											{yylval.op=2;return COMP;}
"=="											{yylval.op=3;return COMP;}
"!="											{yylval.op=6;return COMP;}

";"                                             {BEGIN(CMT);}
<CMT>.                                          {}
<CMT>\n                                         {BEGIN(0);line_no++;}

">>>"											{return out_close_nl;}
"<<<"											{return out_open_nl;}
">>"											{return out_close;}
"<<"											{return out_open;}
"->"                                            {return ARROW;}
[_a-zA-Z][0-9a-zA-Z_]*                          {yylval.txt = strdup(yytext); return ID;}


[0-9]+                                          {yylval.txt = strdup(yytext);return INTEGER;}
[0-9]+\.                                        {yylval.txt = strdup(yytext); return REAL;}
[0-9]+\.[0-9]+                                  {yylval.txt = strdup(yytext); return REAL;}


\n                                              {line_no++;}
.												{}

%%
