%{
#include <stdio.h>
#include <string.h>
#include "ast.h"

#define SIZE_VOID 1
#define SIZE_CHAR 1
#define SIZE_INT 4
#define SIZE_UNSIGNED 4
#define SIZE_DOUBLE 8

void check_main();

void install_identifier(int type, char *id, unsigned int ind);

int flag_main=0;
int flag_unsigned=0;
int flag_const=0;
int flag_external_main=1; 
int flag_type=0;
int flag_type_identifier=0;
int flag_identifier=0;
int flag_case_default=0;
int flag_switch=0;

int flag_break=0;

int errors=0;
int warning=0;

extern int yylineno;

struct node *zero_i=NULL;
struct node *zero_d=NULL;

struct node *if_else_exp=NULL;

%}

/*	Data types	*/
/*	Redefining of YYSTYPE	*/
%union
{
	struct node *a;
	int ival;
	double dval;
	char cval;
	char *id;
}

/*	Axiom		*/
%start programming_unit

/*	Terminal Symbols	*/
%token <id> IDENTIFIER
%token <ival> UNSIGNED_NUM
%token <dval> FLOATING
%token <cval> CHARACTER
%token <ival> MAIN
%token INC_OP DEC_OP LEFT_OP RIGHT_OP LE_OP GE_OP EQ_OP NE_OP
%token AND_OP OR_OP MUL_ASSIGN DIV_ASSIGN MOD_ASSIGN ADD_ASSIGN SUB_ASSIGN

%token CHAR INT UNSIGNED DOUBLE CONST VOID

%token CASE DEFAULT IF ELSE SWITCH WHILE FOR BREAK

/*	To remove the shift/reduce conflict of the IF ELSE 	*/
%nonassoc LOWER_OF_ELSE
%nonassoc ELSE

/*	Associativity and Operators	precedence. */
/*	The associativity is assigned with %right or %left.
	Operators are listed top to bottom, in ascending precedence. */
%right '=' ADD_ASSIGN SUB_ASSIGN MUL_ASSIGN DIV_ASSIGN MOD_ASSIGN
%right '?' ':'
%left OR_OP
%left AND_OP
%left '|'
%left '^'
%left '&'
%left EQ_OP NE_OP
%left '<' LE_OP '>' GE_OP
%left LEFT_OP RIGHT_OP
%left '+' '-'
%left '*' '/' '%'
/* 	since ++ and -- can be prefixes and postfixes,
	the are inserted according to their associativity priority */
%right INC_OP DEC_OP '!'

%%
          /****************************************/
          /*             Declaration             */
          /****************************************/

/* Declaration is referred to the variables */
declaration
	: declaration_specifications initial_declaration_list ';'
		{	
			flag_const=0;			
			flag_type_identifier=0;				
			$<a>$=$<a>2;
		}		
	;

declaration_specifications
	: type_specifications
	| type_specifications declaration_specifications
		{
			if($<ival>1==UNSIGNED)
			{	 
				if($<ival>2!=CHAR && $<ival>2!=INT)
					yyerror("Error: type not supported.");
				else
				{
					flag_unsigned=1;
					$<ival>$=$<ival>2;
				}
			}
			else
				yyerror("Error: type not supported.");
		}
	| type_qualifier declaration_specifications
		{
			if($<ival>2==VOID || $<ival>2==CONST)
				yyerror("Error: type not supported.");
			else	
			{						
				flag_const=1;
				$<ival>$=$<ival>2;
			}
		}
	;

type_specifications
	: VOID
		{
			$<ival>$ = VOID;
			flag_type_identifier = VOID;
		}
	| CHAR
		{
			$<ival>$ = CHAR;
			flag_type_identifier = CHAR;
		}
	| INT
		{
			$<ival>$ = INT;
			flag_type_identifier = INT;
		}
	| DOUBLE
		{
			$<ival>$ = DOUBLE;
			flag_type_identifier = DOUBLE;
		}
	| UNSIGNED
		{
			$<ival>$ = UNSIGNED;
			flag_type_identifier = UNSIGNED;
		}
	;

type_qualifier
	: CONST
		{
			$<ival>$ = CONST;
		}
	;

initial_declaration_list
	: initial_declarations
	| initial_declaration_list ',' initial_declarations
		{
			$<a>$ = build($<a>1, ',', $<a>3);
		}
	; 

initial_declarations
	: declarator
		{	
			if (flag_identifier == IDENTIFIER)
			{	
				install_identifier(flag_type_identifier, $<id>1, (unsigned long)$<id>1);
				struct node *temp_node=NULL;
				insert(&temp_node, IDENTIFIER, context_check_id($<id>1));
				
				switch(flag_type_identifier)
				{
					case(INT):
					case(CHAR):				
					{
						$<a>$ = build(temp_node, '=', zero_i);
						break;
					}
					case(DOUBLE):				
					{
						$<a>$ = build(temp_node, '=', zero_d);
						break;
					}
				}
			}
		}	
	| declarator
		{	
			if (flag_identifier == IDENTIFIER)
				install_identifier(flag_type_identifier, $<id>1, (unsigned long)$<id>1);

		}
		'=' initialiser
		{	
			
			if(flag_type==IDENTIFIER && flag_external_main==1)
				yyerror("Error: the initialiser element is not a constant.");
			else 
			{
				if($<a>4!=NULL)
				{	
					if(flag_identifier==IDENTIFIER)
					{
						switch($<a>4->type)
						{				
							case(UNSIGNED_NUM):
							{	
								type_check(context_check_id($<id>1), context_check(&$<a>4->info->value.ival));
								struct node *temp_node=NULL;
								insert(&temp_node, IDENTIFIER, context_check_id($<id>1));
								$<a>$ = build(temp_node, '=', $<a>4);
								break;
							}
							case(FLOATING):
							{
								type_check(context_check_id($<id>1), context_check(&$<a>4->info->value.dval));
								struct node *temp_node=NULL;
								insert(&temp_node, IDENTIFIER, context_check_id($<id>1));
								$<a>$ = build(temp_node, '=', $<a>4);
								break;
							}
							case(CHARACTER):
							{
								type_check(context_check_id($<id>1), context_check(&$<a>4->info->value.cval));
								struct node *temp_node=NULL;
								insert(&temp_node, IDENTIFIER, context_check_id($<id>1));
						
								$<a>$ = build(temp_node, '=', $<a>4);
								break;
							}
							case(IDENTIFIER):
							{
								if($<a>4->type==IDENTIFIER)
								type_check(context_check_id($<id>1), context_check_id($<a>4->info->value.id));
								struct node *temp_node=NULL;
								insert(&temp_node, IDENTIFIER, context_check_id($<id>1));
						
								$<a>$ = build(temp_node, '=', $<a>4);
								break;
							}
							case('='):
							{
								type_check(context_check_id($<id>1), context_check(&$<a>4->left->info->value.id));
								struct node *temp_node=NULL;
								insert(&temp_node, IDENTIFIER, context_check_id($<id>1));

								$<a>$ = build(temp_node, '=', $<a>4);
								break;
							}
							default:
							{
								struct node *temp_node=NULL;
								insert(&temp_node, IDENTIFIER, context_check_id($<id>1));
								$<a>$ = build(temp_node, '=', $<a>4);
								break;
							}
						}
					}
				}
				else
					$<a>$=NULL;
			}
		}
	/* **************** GRAMMAR EXTENSION ********************* */
	|  UNSIGNED_NUM '=' initialiser
	{
		yyerror("Error: there is a constant on the left of the equal sign!\n");
		$<a>$=NULL;
	}		
	;

declarator
	: IDENTIFIER
		{
			flag_identifier = IDENTIFIER;
			$<id>$ = $<id>1;
		}
	| MAIN '(' ')'
		{
			$1 = MAIN;
			check_main();
		}
	;

         /****************************************/
         /*               Axiom                  */
         /****************************************/

programming_unit
	: external_declaration
		{
			
			axiom = $<a>1;
		}	
	;

external_declaration
	: external_operations
	| external_operations external_declaration
		{
			$<a>$ = build($<a>1, '\n', $<a>2);
		}
	;

external_operations
	: declaration
	/* 	This serves to initialise out of the main.
		This grammars is called out of the main.
		There is primary_expression to avoid the unary operators. */
	| declarator '=' primary_expression ';'
		{
			if(flag_type==IDENTIFIER && flag_external_main==1)
			{
				yyerror("Error: the initialiser element is not a constant.");
				$<a>$=NULL;
			}
			else
			{	
				struct node *temp_node=NULL;
				insert(&temp_node, IDENTIFIER, context_check_id($<id>1));
				st_record *current = get_st("Identifier",$<id>1);
				
				if(current->flag_const == 0)
				{	
					switch($<a>3->type)
					{				
						case(UNSIGNED_NUM):
						{				
							type_check(context_check_id($<id>1), context_check(&$<a>3->info->value.ival));
							break;
						}
						case(FLOATING):
						{
							type_check(context_check_id($<id>1), context_check(&$<a>3->info->value.dval));
							break;
						}
						case(CHARACTER):
						{
							type_check(context_check_id($<id>1), context_check(&$<a>3->info->value.cval));
							break;
						}
					}
					$<a>$=build(temp_node,'=',$<a>3);
				}
				else
				{	
					char msg[128];
            				sprintf(msg,"Error: the variable \"%s\" is read only", current->value.id);
            				yyerror(msg);					
				}				
			}
		}
	| function_definition
	| ';'
		{
			$<a>$=NULL;
		}
	;

/* function_definition is referred to the functions */
function_definition
	: declarator
		{
			flag_external_main=0;
		}
		compound_statement
		{
			flag_external_main=1;	
			$<a>$ = $<a>3;					
		}
	;

compound_statement
	: '{' '}'
		{
			$<a>$=NULL;
		}
	| '{'	
		{
			new_st();			
			//printf("\nWhen it starts: the local_scope %d and the parent_scope %d and the scope_max %d", local_scope, parent_scope, scope_max);
			
		} 
		list_elements '}'
		{
			local_scope=parent_scope;
			parent_scope=parent_child[local_scope];
			if(parent_scope<0)
				parent_scope=0;
			//printf("\nWhen it finishes: the local_scope %d and the parent_scope %d and the scope_max %d", local_scope, parent_scope, scope_max);
			$<a>$=$<a>3;
		}	
	;

list_elements
	: elements
	| list_elements elements
		{
			$<a>$ = build($<a>1, '\n', $<a>2);
		}
	;

elements
	: declaration
	| statement
	;

statement
	: labeled_statement
	| compound_statement
	| expression_statement
	| selection_statement
	| iteration_statement
	| jump_statement
	;

labeled_statement
	: CASE primary_expression ':' statement
		{
			if(flag_case_default==0)
			{
				if($<a>2->info->type!=DOUBLE)
					$<a>$ = build($<a>2, 'c', $<a>4);
				else
				{
					yyerror("Error: the double type is not permitted\n");
					$<a>$=NULL;
				}			
			}
			else
			{
				yyerror("Error: another case after the default label is not permitted.");
				$<a>$=NULL;
			}
		}
	| DEFAULT ':' statement
		{
			if(flag_case_default==0)
			{
				flag_case_default=1;
				$<a>$ = build(NULL, 'd', $<a>3);
			}
			else
			{
				yyerror("Error: another default is not permitted.");
				$<a>$=NULL;
			}					
		}
	;

expression_statement
	: ';'
		{
			$<a>$=NULL;
		}
	| expression ';'
	;

selection_statement
	: IF '(' expression ')' statement	%prec LOWER_OF_ELSE
		{
			$<a>$ = build($<a>3, 'i', $<a>5);
		}		
	| IF '(' expression ')' statement ELSE statement
		{	
			$<a>$ = build($<a>3, 'j', build($<a>5 ,'e', $<a>7));
		}
	| SWITCH '(' primary_expression ')'
		{
			flag_break++;

			if(flag_switch!=1)
				flag_switch=1;
			else
			{
				yyerror("It is permitted only one level of switch.");
				$<a>$=NULL;
			}
	
		}
		statement
		{
			if($<a>3->info->type==INT || $<a>3->info->type==CHAR)
			{
				flag_case_default=0;			
				flag_switch = 0;				
				flag_break--;
				$<a>$ = build($<a>3, 's', $<a>6);
			}
			else
			{
				yyerror("Error: only the int type is allowed.");
				$<a>$ = NULL;
			}	
		}
	;

iteration_statement
	: WHILE '(' expression ')'
		{
			flag_break++;
		}
		statement
		{
			flag_break--;			
			$<a>$ = build($<a>3, 'w', $<a>6);
		}
	/* Where there is expression_statement, it could also be only the ';' */
	/*	for(i=0; i<n; )	*/
	| FOR '(' expression_statement expression_statement ')'
		{
			flag_break++;
		}
		statement
		{
			flag_break--;
			$<a>$ = build($<a>3, 'f', build(NULL, 'g', build($<a>4, 'h', $<a>7)));
		}
	/*	for(i=0; i<n; ++i)	*/
	| FOR '(' expression_statement expression_statement expression ')'
		{
			flag_break++;
		}
		statement
		{
			flag_break--;			
			$<a>$ = build($<a>3, 'f', build($<a>5, 'g', build($<a>4, 'h', $<a>8)));
		}
	/*	for(int i=0; i<n; )	*/
	| FOR '(' declaration expression_statement ')'
		{
			flag_break++;
		}
		statement
		{
			flag_break--;			
			$<a>$ = build($<a>3, 'f', build(NULL, 'g', build($<a>4, 'h', $<a>7)));
		}
	/*	for(int i=0; i<n; ++i)	*/
	| FOR '(' declaration expression_statement expression ')'
		{
			flag_break++;
		}
		statement
		{
			flag_break--;
			$<a>$ = build($<a>3, 'f', build($<a>5, 'g', build($<a>4, 'h', $<a>8)));
		}
	;

jump_statement
	: BREAK ';'
		{
			if(flag_break != 0)
				$<a>$ = build(NULL, 'b', NULL);
			else
			{
				yyerror("The break is not in a cycle or in a switch statement.");
				$<a>$=NULL;
			}
		}
	;

initialiser
	: assignment_expression
	;

      /******************************************/
      /*               Expressions              */
	  /******************************************/

expression
	: assignment_expression
	/* The following expression could be related with the for(i,j ;.. ; ..) */
	| expression ',' assignment_expression
		{
			$<a>$ = build($<a>1, ',', $<a>3);
		}
	;

assignment_expression
	: conditional_expression
	| unary_expression assignment_operator assignment_expression
		{	
			if( $<a>3!=NULL)
			{
				switch($<a>1->type)
				{				
					case(UNSIGNED_NUM):
					case(FLOATING):
					case(CHARACTER):
					{
						yyerror("Error: the left operand of the assignment operator is not an identifier!");
						$<a>$=build($<a>1, 0, NULL);
						break;
					}
					case(IDENTIFIER):
					{
						$<a>$=build($<a>1, $<ival>2, $<a>3);
						break;	
					}
					default:
					{
						$<a>$=build($<a>1, $<ival>2, $<a>3);
						break;	
					}					
				}		
			}
		}
	/* ********** GRAMMAR EXTENSION ***************/
	| shift_expression assignment_operator assignment_expression
	{	
		yyerror("Error: there is a not permitted expression to left of the assignment operator\n");
		$<a>$=NULL;
	}		
	;

unary_expression
	: postfix_expression
	/* This could be f.i. ++i or --i */
	| INC_OP primary_expression
		{
			switch($<a>2->type)
				{	
					case(IDENTIFIER):
					{
						$<a>$ = build(NULL, INC_OP, $<a>2);
						break;	
					}
					case(UNSIGNED_NUM):
					case(FLOATING):
					case(CHARACTER):
					default:
					{
						yyerror("Error: the right operand is not an identifier!");
						$<a>$=NULL;
						break;
					}
				}
		}
	| DEC_OP primary_expression
		{
			switch($<a>2->type)
				{	
					case(IDENTIFIER):
					{
						$<a>$ = build(NULL, DEC_OP, $<a>2);
						break;	
					}
					case(UNSIGNED_NUM):
					case(FLOATING):
					case(CHARACTER):
					default:
					{
						yyerror("Error: the right operand is not an identifier!");
						$<a>$=NULL;
						break;
					}
				}			
		}
	| unary_operator unary_expression
		{
			if($<ival>1=='+')
				$<a>$=$<a>2;			
			else
				$<a>$=build(NULL , $<ival>1, $<a>2);
		}
	;

unary_operator
	: '+'	
		{$<ival>$ = '+';}
	| '-'
		{$<ival>$ = '-';}
	| '!'
		{$<ival>$ = '!';}
	;

assignment_operator
	: '='
		{$<ival>$ = '=';}
	| MUL_ASSIGN
		{$<ival>$ = MUL_ASSIGN;}
	| DIV_ASSIGN
		{$<ival>$ = DIV_ASSIGN;}
	| MOD_ASSIGN
		{$<ival>$ = MOD_ASSIGN;}
	| ADD_ASSIGN
		{$<ival>$ = ADD_ASSIGN;}
	| SUB_ASSIGN
		{$<ival>$ = SUB_ASSIGN;}
	;

primary_expression
	: IDENTIFIER
		{	
			flag_type = IDENTIFIER;
			struct node *temp_node=NULL;
			int result=insert(&temp_node, IDENTIFIER, context_check_id($<id>1));
			if(result==0)
				$<a>$ = temp_node;
			else
				$<a>$ = NULL;			
		}
	| UNSIGNED_NUM
		{  	
			flag_unsigned = 1;
			flag_const = 1;
			install("Unsigned_Num", &$1, INT, flag_unsigned,  flag_const, SIZE_INT, (unsigned long)&$1);
			flag_unsigned = 0;
			flag_type = UNSIGNED_NUM;
			struct node *temp_node=NULL;
			insert(&temp_node, UNSIGNED_NUM, context_check(&$1));
			$<a>$ = temp_node;
		}
	| FLOATING
		{	
			flag_unsigned=0;
			flag_const = 1;
			install("Floating", &$1, DOUBLE, flag_unsigned, flag_const, SIZE_DOUBLE, (unsigned long)&$1);
			flag_type = FLOATING;
			struct node *temp_node=NULL;
			insert(&temp_node, FLOATING, context_check(&$1));
			$<a>$ = temp_node;
		}
	| CHARACTER
		{	
			flag_unsigned=0;
			flag_const = 1;
			install("Character", &$1, CHAR, flag_unsigned,  flag_const, SIZE_CHAR, (unsigned long)&$1);
			flag_type = CHARACTER;
			struct node *temp_node=NULL;
			insert(&temp_node, CHARACTER, context_check(&$1));
			$<a>$ = temp_node;
		}		
	| '(' expression ')'
		{
			$<a>$=$<a>2;	
		}
	;

postfix_expression
	: primary_expression
	/* The following expressions could be f.i. i++ or i-- */
	| postfix_expression INC_OP
		{
			switch($<a>1->type)
				{				
					case(IDENTIFIER):
					{
						$<a>$ = build($<a>1, INC_OP, NULL);
						break;	
					}
					case(UNSIGNED_NUM):
					case(FLOATING):
					case(CHARACTER):
					default:
					{
						yyerror("Error: the left operand is not an identifier!");
						$<a>$=NULL;
						break;
					}
				}
		}
	| postfix_expression DEC_OP
		{
			switch($<a>1->type)
				{	
					case(IDENTIFIER):
					{
						$<a>$ = build($<a>1, DEC_OP, NULL);
						break;	
					}
					case(UNSIGNED_NUM):
					case(FLOATING):
					case(CHARACTER):
					default:
					{
						yyerror("Errore: the left operand is not an identifier!");
						$<a>$=NULL;
						break;
					}
				}
		}
	;

conditional_expression
	: logical_or_expression
	| logical_or_expression '?' expression ':' conditional_expression
		{	
			$<a>$ = build($<a>1, 'j', build($<a>3 ,'e', $<a>5));
		}
	;

logical_or_expression
	: logical_and_expression
	| logical_or_expression OR_OP logical_and_expression
		{
			$<a>$=build($<a>1, OR_OP, $<a>3);
		}
	;

logical_and_expression
	: inclusive_or_expression
	| logical_and_expression AND_OP inclusive_or_expression
		{
			$<a>$=build($<a>1, AND_OP, $<a>3);
		}
	;

inclusive_or_expression
	: exclusive_or_expression
	| inclusive_or_expression '|' exclusive_or_expression
		{
			$<a>$=build($<a>1, '|', $<a>3);
		}
	;

exclusive_or_expression
	: and_expression
	| exclusive_or_expression '^' and_expression
		{
			$<a>$=build($<a>1, '^', $<a>3);
		}
	;

and_expression
	: equality_expression
	| and_expression '&' equality_expression
		{
			$<a>$=build($<a>1, '&', $<a>3);
		}
	;

equality_expression
	: relational_expression
	| equality_expression EQ_OP relational_expression
		{
			$<a>$=build($<a>1, EQ_OP, $<a>3);
		}
	| equality_expression NE_OP relational_expression
		{
			$<a>$=build($<a>1, NE_OP, $<a>3);
		}
	;

relational_expression
	: shift_expression
	| relational_expression '<' shift_expression
		{
			$<a>$=build($<a>1, '<', $<a>3);
		}
	| relational_expression '>' shift_expression
		{
			$<a>$=build($<a>1, '>', $<a>3);
		}
	| relational_expression LE_OP shift_expression
		{
			$<a>$=build($<a>1, LE_OP, $<a>3);
		}
	| relational_expression GE_OP shift_expression
		{
			$<a>$=build($<a>1, GE_OP, $<a>3);
		}
	;

shift_expression
	: additive_expression
	/* 	LEFT_OP x<<y This shifts the bit values of x of y position.
		It multiplies the value of x for 2y */
	| shift_expression LEFT_OP additive_expression
		{
			$<a>$=build($<a>1, LEFT_OP, $<a>3);
		}
	| shift_expression RIGHT_OP additive_expression
		{
			$<a>$=build($<a>1, RIGHT_OP, $<a>3);
		}
	;

additive_expression
	: multiplicative_expressions
	| additive_expression '+' multiplicative_expressions
		{
			$<a>$=build($<a>1, '+', $<a>3);
		}
	| additive_expression '-' multiplicative_expressions
		{
			$<a>$=build($<a>1, '-', $<a>3);

		}
	;

multiplicative_expressions
	: unary_expression
	| multiplicative_expressions '*' unary_expression
		{
			$<a>$=build($<a>1, '*', $<a>3);
		}
	| multiplicative_expressions '/' unary_expression
		{
			if($<a>3->info->value.ival==0)
				{
					yywarning("Attempting to divide by 0. This could be a problem.");
				}
			$<a>$=build($<a>1, '/', $<a>3);
		}
	| multiplicative_expressions '%' unary_expression
		{
			$<a>$=build($<a>1, '%', $<a>3);
		}
	;


%%

	
int main(int argc, char **argv)
{	
	extern FILE *yyin;

	int i, n=128;	

	quadruple = fopen("quadruple.txt","w");	

	report_st = fopen("parent_child.txt","w");

	switch(argc)
	{
		case 1:
		{
			fprintf(stderr, "Error, there is not an input file.\n");
			fprintf(stderr, "Example: %s <nomefile.c>\n", argv[0]);
			return 0;
		}
		case 2: 
		{	yyin = fopen(argv[1], "r");
			if(yyin == NULL)
			{
				fprintf(stderr, "Error opening file.\n");
				return 0;
			}
			/* If the opening file is correct. */
			/* Initialise the symbol table. */
			symbol_tables=(st_record **)malloc(sizeof(st_record *)*n);
			
			parent_child=(int *)malloc(sizeof(int)*n);

			for(i=0; i<n; i++)
				parent_child[i]=-1;

			/* Set the level 0, that is out of the main(). */
			new_st();

			int zeroi=0;
			install("Unsigned_Num", &zeroi, INT, 1,  1, SIZE_INT, (unsigned long)&zeroi);
			insert(&zero_i, UNSIGNED_NUM, context_check(&zeroi));

			double zerod=0.0;
			install("Floating", &zerod, INT, 1,  1, SIZE_INT, (unsigned long)&zerod);
			insert(&zero_d, FLOATING, context_check(&zerod));

			yylineno = 1;
			break;
		}
		default:
		{	
			fprintf(stderr, "Error, too many input files.\n");
			fprintf(stderr, "Only a file is accepted.\n");
			return 0;
		}
	}	
	
	yyparse();
	
	if(flag_main == 0)
		yyerror("Error: The main is not present!\n");
	
	print_st();

	fclose(report_st);
	printf("\nStart of the tree:\n");
	visit_in_order(axiom);
	//visit_in_preorder(axiom);
	
	fill_operations();
	if(errors==0)
	{
		create_quadruple(axiom);
		if(warning!=0)
			printf("\n\n*** Exit from the compiler with the SUCCESS state. There was detected %d warnings.***\n", warning);
		else
			printf("\n\n*** Exit from the compiler with the SUCCESS state. :-) ***\n");
	}
	else
		printf("\n\n*** Exit from the compiler with the ERROR state. Committed %d semantic errors***\n",errors);
		
	printf("\n");

	return 0;
}


void yyerror(char *s)
{
	fprintf(stderr, "Line %d: %s\n", yylineno, s);
	errors++;
}

void yywarning(char *s)
{
	fprintf(stderr, "Line %d: %s\n", yylineno, s);
	warning++;
}

void check_main()
{	
	if(flag_main==0)
	{
		flag_main=1;			
	}
	else
	{	
		yyerror("Error: The code must contain only a main\n");
		exit(1);
	}
}

void install_identifier(int type, char *id, unsigned int ind)
{
	switch(type)
		{	
			case INT:
			{
				install("Identifier", id, type, flag_unsigned, flag_const, SIZE_INT, ind);
				break;
			}
			case VOID:
			{
				install("Identifier", id, type, flag_unsigned, flag_const, SIZE_VOID, ind);
				break;
			}
			case DOUBLE:
			{
				install("Identifier", id, type, flag_unsigned, flag_const, SIZE_DOUBLE, ind);
				break;
			}
			case CHAR:
			{
				install("Identifier", id, type, flag_unsigned, flag_const, SIZE_CHAR, ind);
				break;
			}
		}
}

