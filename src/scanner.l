%option noyywrap yylineno
%{
#include <stdio.h>
#include <string.h>
#include "parser.tab.h"

/* FUNCTION PROTOTYPES */
void comment(void);

%}

/* BASIC DEFINITIONS */
unsigned_num	[0-9]+
identifier		[a-zA-Z_][a-zA-Z_0-9]*
floating_point	[0-9]*\.{unsigned_num}
character		\'[a-zA-Z0-9]\'

%%

"/*"			{ comment(); }
"//"[^\n]*		{;}

[ \n\t]			{;}

"int"			{return INT;}
"char"			{return CHAR;}
"double"		{return DOUBLE;}
"unsigned"		{return UNSIGNED;}
"const"			{return CONST;}
"void"			{return VOID;}

"if"			{return IF;}
"else"			{return ELSE;}

"switch"		{return SWITCH;}
"case"			{return CASE;}
"default"		{return DEFAULT;}
"break"			{return BREAK;}

"for"			{return FOR;}
"while"			{return WHILE;}

"main"			{return MAIN;}

{identifier}	{
					yylval.id = (char *) strdup(yytext);
					return IDENTIFIER;
				}
{unsigned_num}	{
					yylval.ival = atoi(yytext);
					return UNSIGNED_NUM;
				}

{floating_point} {
					char *end;
					yylval.dval = strtod(yytext, &end);
					return FLOATING;
				}
{character}		{
					yylval.cval =(char) yytext[1];
					return CHARACTER;
				}

"+="			{return ADD_ASSIGN;}
"-="			{return SUB_ASSIGN;}
"*="			{return MUL_ASSIGN;}
"/="			{return DIV_ASSIGN;}
"%="			{return MOD_ASSIGN;}
">>"			{return RIGHT_OP;}
"<<"			{return LEFT_OP;}
"++"			{return INC_OP;}
"--"			{return DEC_OP;}
"&&"			{return AND_OP;}
"||"			{return OR_OP;}
"<="			{return LE_OP;}
">="			{return GE_OP;}
"=="			{return EQ_OP;}
"!="			{return NE_OP;}
";"				{return ';';}
"{"				{return '{';}
"}"				{return '}';}
","				{return ',';}
":"				{return ':';}
"="				{return '=';}
"("				{return '(';}
")"				{return ')';}
"&"				{return '&';}
"!"				{return '!';}
"-"				{return '-';}
"+"				{return '+';}
"*"				{return '*';}
"/"				{return '/';}
"%"				{return '%';}
"<"				{return '<';}
">"				{return '>';}
"^"				{return '^';}
"|"				{return '|';}
"?"				{return '?';}
.				{printf("\ninvalid character\n");}

%%

void comment(void)
{
	char current, previous = 0;
	/* Until the file is not read completely */
	while ( (current = input()) != EOF )
	{
		/*	If the current character is equal to the symbol /, and,
			the previous character is equal to the symbol *,
           	it means that a closed comment was found */
		if ( current == '/' && previous == '*')
			return;
		previous = current;
	}
	yyerror("Comment has not been closed");
}
