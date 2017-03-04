#include <stdio.h>
#include "symbol_table.h"

/*
    the types can be:
     IDENTIFIER = 258,
     UNSIGNED_NUM = 259,
     FLOATING = 260,
     CHARACTER = 261,
     MAIN = 262,
     INC_OP = 263,
     DEC_OP = 264,
     LEFT_OP = 265,
     RIGHT_OP = 266,
     LE_OP = 267,
     GE_OP = 268,
     EQ_OP = 269,
     NE_OP = 270,
     AND_OP = 271,
     OR_OP = 272,
     MUL_ASSIGN = 273,
     DIV_ASSIGN = 274,
     MOD_ASSIGN = 275,
     ADD_ASSIGN = 276,
     SUB_ASSIGN = 277,
     CHAR = 278,
     INT = 279,
     UNSIGNED = 280,
     DOUBLE = 281,
     CONST = 282,
     VOID = 283,
     CASE = 284,
     DEFAULT = 285,
     IF = 286,
     ELSE = 287,
     SWITCH = 288,
     WHILE = 289,
     FOR = 290,
     BREAK = 291,
     LOWER_OF_ELSE = 292
*/

struct node
{
    int type;
    struct record *info;
    struct node *left, *right;
};

FILE *quadruple=NULL;

char **operation=NULL;
char condition[126];
char switch_expression[126];

struct node *axiom = NULL;

int test_empty_tree (struct node *root);
int root (struct node *rt, int *value);
struct node *left (struct node *rt);
struct node *right (struct node *rt);
struct node * build(struct node *lh, int r, struct node *rh);
int insert (struct node **root, int e, struct record *info);
void visit_in_preorder(struct node *root);
void visit_in_order(struct node *root);
void visit_in_postorder(struct node *root);
struct node *copy(struct node *rt);

void cast_mov_id(struct record *id_lh, struct record *id_rh);
void cast_op_asg_id(char op[4], struct record *id_lh, struct record *id_rh);

void fill_operations();

void read(struct node *root);

void create_quadruple(struct node *root);

int var_temp=0;
int var_temp_lh=0;
int var_temp_lh_lh=0;
int var_temp_rh=0;
int cond_if=0;
int cond_if_prec=0;
int cond_else_if=0;

int if_else[128];

int cond_while=0;
int cond_while_prec=0;

int cond_for=0;
int cond_for_prec=0;

int cond_case=0;
int cond_case_prec=0;
int end_case[128];
int cond_switch=0;
int cond_switch_prec=0;
int case_break[128];
int case_def=0;
int case_switch=0;

int cond_stat=0;

char cond_stat_type[128];
int n_cond_stat=0;

char temp_type[128];

int test_empty_tree (struct node *root)
{
    if(root)
        return 0;
    else
        return 1;
}

int root (struct node *rt, int *value)
{
    if(test_empty_tree(rt))
       return 0;
    *value=rt->type;
    return 1;
}

struct node *left (struct node *rt)
{
    if(!test_empty_tree(rt))
       return rt->left;
    else
        return NULL;
}

struct node *right (struct node *rt)
{
    if(!test_empty_tree(rt))
       return rt->right;
    else
        return NULL;
}

struct node *build(struct node *lh, int r, struct node *rh)
{
    struct node *aux;
    aux=malloc(sizeof(struct node));
    if (aux)
    {
        aux->type=r;
        aux->left=lh;
        aux->right=rh;
    }
    return aux;
}

struct node *copy(struct node *rt)
{
    struct node *aux;
    aux=malloc(sizeof(struct node));
    if (aux)
    {
        if(rt)
        {
            aux->type=rt->type;
            aux->info=rt->info;
            aux->left=copy(rt->left);
            aux->right=copy(rt->right);
        }
    }
    return aux;
}

int insert (struct node **root, int e, struct record *info)
{
    struct node *aux;
    if(*root==NULL && info!=NULL)
    {
        aux=malloc(sizeof(struct node));
        if(aux)
        {
            aux->type=e;
            aux->left=aux->right=NULL;
            aux->info=info;
            *root=aux;
            return 0;
        }
        else
        {
            printf("Memoria non allocata");
            return 1;
        }
    }
    else
        return 1;
}

void read(struct node *root)
{
    switch(root->type)
        {
            case IDENTIFIER:
                {
                    printf("%s ",root->info->value.id);
                    break;
                }
            case UNSIGNED_NUM:
				{
				    printf("%d ",root->info->value.ival);
					break;
				}
			case FLOATING:
				{
					printf("%f ",root->info->value.dval);
					break;
				}
			case CHARACTER:
				{
					printf("%c ",root->info->value.cval);
					break;
				}
            case ',':
            case '=':
            case '+':
            case '-':
            case '*':
            case '/':
            case '<':
            case '>':
            case '%':
            case '&':
            case '|':
            case '^':
            case '!':
                {
					printf("%c ",root->type);
					break;
				}
            case EQ_OP:
                {
					printf("== ");
					break;
				}
			case NE_OP:
                {
					printf("!= ");
					break;
				}
            case LEFT_OP:
                {
					printf("<< ");
					break;
				}
            case RIGHT_OP:
                {
					printf(">> ");
					break;
				}
            case LE_OP:
                {
					printf("<= ");
					break;
				}
            case GE_OP:
                {
					printf(">= ");
					break;
				}
            case INC_OP:
                {
					printf("++ ");
					break;
				}
            case DEC_OP:
                {
					printf("-- ");
					break;
				}
            case ADD_ASSIGN:
                {
					printf("+= ");
					break;
				}
            case SUB_ASSIGN:
                {
					printf("-= ");
					break;
				}
            case DIV_ASSIGN:
                {
					printf("/= ");
					break;
				}
            case MUL_ASSIGN:
                {
					printf("*= ");
					break;
				}
            case MOD_ASSIGN:
                {
					printf("%c= ",37);
					break;
				}
            case AND_OP:
                {
					printf("&& ");
					break;
				}
            case OR_OP:
                {
					printf("|| ");
					break;
				}
            case 'i':
            case 'j':
                {
                    printf("IF ");
                    break;
                }
            case 'e':
                {
                    printf("ELSE ");
                    break;
                }
            case 'f':
                {
                    printf("FOR ");
                    break;
                }
            case 'w':
                {
                    printf("WHILE ");
                    break;
                }
            case 's':
                {
                    printf("SWITCH ");
                    break;
                }
             case 'c':
                {
                    printf("CASE ");
                    break;
                }
            case 'd':
                {
                    printf("DEFAULT ");
                    break;
                }
            case 'b':
                {
                    printf("BREAK ");
                    break;
                }
            case 'n':
                {
                    printf("");
                    break;
                }
            case '\n':
                {
                    printf("\n");
                    break;
                }
            case '\t':
                {
                    printf("\t\t");
                    break;
                }
        }
}

/*  The root of the tree is analysed;
    The subtree on the left is visited in preorder;
    The subtree on the right is visited in preorder.
*/
void visit_in_preorder(struct node *root)
{
    if(root)
    {
        read(root);
        visit_in_preorder(root->left);
        visit_in_preorder(root->right);
    }
}

/*  The subtree on the left is visited in order;
    The root of the tree is analysed;
    The subtree on the right is visited in order.
*/
void visit_in_order(struct node *root)
{
    if(root)
    {
        visit_in_order(root->left);
        read(root);
        visit_in_order(root->right);
    }
    //else
    //    printf("\nnode is null\n");
}

/*  The subtree on the left is visited in postorder;
    The subtree on the right is visited in postorder;
    The root of the tree is analysed.
*/
void visit_in_postorder(struct node *root)
{
    if(root)
    {
        visit_in_postorder(root->left);
        visit_in_postorder(root->right);
        read(root);
    }
}

void create_quadruple(struct node *root)
{
    if(root)
    {
        switch(root->type)
        {
            case 'i':
            case 'j':
            case 'w':
            case 'h':
            {
                break;
            }
            default:
                create_quadruple(root->left);
        }

        switch(root->type)
        {
            case '=':
            {
                switch(root->right->type)
                {
                    case(UNSIGNED_NUM):
                    {
                        if(root->left->type==IDENTIFIER)
                        {
                            switch(root->left->info->type)
                            {
                                case(INT):
                                case(CHAR):
                                {
                                    fprintf(quadruple,"\tMOVI #%d, , %s\n", root->right->info->value.ival, root->left->info->value.id);
                                    break;
                                }
                                case(DOUBLE):
                                {
                                    temp_type[++var_temp]='F';
                                    fprintf(quadruple,"\tINT2REAL #%d, , TEMP%c%d\n", root->right->info->value.ival, temp_type[var_temp], var_temp);
                                    fprintf(quadruple,"\tMOVF TEMP%c%d, , %s\n", temp_type[var_temp], var_temp, root->left->info->value.id);
                                    break;
                                }
                            }
                        }
                        break;
                    }
                    case(FLOATING):
                    {
                        if(root->left->type==IDENTIFIER)
                            switch(root->left->info->type)
                            {
                                case(INT):
                                case(CHAR):
                                {
                                    temp_type[++var_temp]='I';
                                    fprintf(quadruple,"\tREAL2INT #%f, , TEMP%c%d\n", root->right->info->value.dval, temp_type[var_temp], var_temp);
                                    fprintf(quadruple,"\tMOVI TEMP%c%d, , %s\n", temp_type[var_temp], var_temp, root->left->info->value.id);
                                    break;
                                }
                                case(DOUBLE):
                                {
                                    fprintf(quadruple,"\tMOVF #%f, , %s\n", root->right->info->value.dval, root->left->info->value.id);
                                    break;
                                }
                            }
                        break;
                    }
                    case(CHARACTER):
                    {
                        if(root->left->type==IDENTIFIER)
                        {
                            switch(root->left->info->type)
                            {
                                case(INT):
                                case(CHAR):
                                {
                                    fprintf(quadruple,"\tMOVI #%d, , %s\n", root->right->info->value.cval, root->left->info->value.id);
                                    break;
                                }
                                case(DOUBLE):
                                {
                                    temp_type[++var_temp]='F';
                                    fprintf(quadruple,"\tINT2REAL #%d, , TEMP%c%d\n", root->right->info->value.cval, temp_type[var_temp], var_temp);
                                    fprintf(quadruple,"\tMOVF TEMP%c%d, , %s\n", temp_type[var_temp], var_temp, root->left->info->value.id);
                                    break;
                                }
                            }
                        }
                        break;
                    }
                    case(IDENTIFIER):
                    {
                        if(root->left->type==IDENTIFIER)
                        {
                            cast_mov_id(root->left->info, root->right->info);
                        }
                        break;
                    }
                    case(INC_OP):
                    case(DEC_OP):
                    {
                        if(root->left->type==IDENTIFIER && root->right->left==NULL)
                        {
                            create_quadruple(root->right);
                            cast_mov_id(root->left->info, root->right->right->info);
                        }
                        if(root->left->type==IDENTIFIER && root->right->right==NULL)
                        {
                            cast_mov_id(root->left->info, root->right->left->info);
                            create_quadruple(root->right);
                        }
                        break;
                    }
                    case ADD_ASSIGN:
                    case SUB_ASSIGN:
                    case DIV_ASSIGN:
                    case MUL_ASSIGN:
                    case MOD_ASSIGN:
                    {
                        if(root->left->type==IDENTIFIER && root->right->left->type==IDENTIFIER)
                        {
                            create_quadruple(root->right);
                            cast_mov_id(root->left->info, root->right->left->info);
                        }
                        break;
                    }
                    default:
                    {
                        create_quadruple(root->right);
                        /*  This instruction is required when at the right of the equal sign there is an expression and the final value is stored in TEMP%d.
                            Because there is an expression, eg the + that is neither an IDENTIFIER nor other data type, it is necessary to use the default. */
                        if((root->left->type==IDENTIFIER)&&(root->right->type!='='))
                        {
                            switch(root->left->info->type)
                            {
                                case(INT):
                                case(CHAR):
                                {
                                    if(temp_type[var_temp]=='I')
                                        fprintf(quadruple,"\tMOVI TEMP%c%d, , %s\n", temp_type[var_temp], var_temp, root->left->info->value.id);
                                    else if(temp_type[var_temp]=='F')
                                    {
                                        var_temp_rh=var_temp;
                                        temp_type[++var_temp]='I';
                                        fprintf(quadruple,"\tREAL2INT TEMP%c%d, , TEMP%c%d\n", temp_type[var_temp_rh], var_temp_rh, temp_type[var_temp], var_temp);
                                        fprintf(quadruple,"\tMOVI TEMP%c%d, , %s\n", temp_type[var_temp], var_temp, root->left->info->value.id);
                                    }
                                    break;
                                }
                                case(DOUBLE):
                                {
                                    if(temp_type[var_temp]=='I')
                                    {
                                        var_temp_rh=var_temp;
                                        temp_type[++var_temp]='F';
                                        fprintf(quadruple,"\tINT2REAL TEMP%c%d, , TEMP%c%d\n", temp_type[var_temp_rh], var_temp_rh, temp_type[var_temp], var_temp);
                                        fprintf(quadruple,"\tMOVF TEMP%c%d, , %s\n", temp_type[var_temp], var_temp, root->left->info->value.id);
                                    }
                                    else if(temp_type[var_temp]=='F')
                                    {
                                        fprintf(quadruple,"\tMOVF TEMP%c%d, , %s\n", temp_type[var_temp], var_temp, root->left->info->value.id);
                                    }
                                    break;
                                }
                            }
                        }
                        else if((root->left->type==IDENTIFIER)&&(root->right->type=='='))
                            {
                                cast_mov_id(root->left->info, root->right->left->info);
                            }
                        break;
                    }
                }
                break;
            }
            case ADD_ASSIGN:
            case SUB_ASSIGN:
            case DIV_ASSIGN:
            case MUL_ASSIGN:
            case MOD_ASSIGN:
            {
                if(root->left)
                {
                    if(root->left->type==IDENTIFIER)
                    {
                        switch(root->right->type)
                        {
                            case(IDENTIFIER):
                            {
                                cast_op_asg_id(operation[root->type], root->left->info, root->right->info);
                                break;
                            }
                            case(UNSIGNED_NUM):
                            {
                                switch(root->left->info->type)
                                {
                                    case(INT):
                                    case(CHAR):
                                    {
                                        fprintf(quadruple,"\t%sI %s, %d, %s\n", operation[root->type], root->left->info->value.id, root->right->info->value.ival, root->left->info->value.id);
                                        break;
                                    }
                                    case(DOUBLE):
                                    {
                                        temp_type[++var_temp]='F';
                                        fprintf(quadruple,"\tINT2REAL #%d, , TEMP%c%d\n", root->right->info->value.ival, temp_type[var_temp], var_temp);
                                        fprintf(quadruple,"\t%sF %s, TEMP%c%d, %s\n", operation[root->type], root->left->info->value.id, temp_type[var_temp], var_temp, root->left->info->value.id);
                                        break;
                                    }
                                }
                                break;
                            }
                            case(CHARACTER):
                            {
                                switch(root->left->info->type)
                                {
                                    case(INT):
                                    case(CHAR):
                                    {
                                        fprintf(quadruple,"\t%sI %s, %d, %s\n", operation[root->type], root->left->info->value.id, root->right->info->value.cval, root->left->info->value.id);
                                        break;
                                    }
                                    case(DOUBLE):
                                    {
                                        temp_type[++var_temp]='F';
                                        fprintf(quadruple,"\tINT2REAL #%d, , TEMP%c%d\n", root->right->info->value.cval, temp_type[var_temp], var_temp);
                                        fprintf(quadruple,"\t%sF %s, TEMP%c%d, %s\n", operation[root->type], root->left->info->value.id, temp_type[var_temp], var_temp, root->left->info->value.id);
                                        break;
                                    }
                                }
                                break;
                            }
                            case(FLOATING):
                            {
                                switch(root->left->info->type)
                                {
                                    case(INT):
                                    case(CHAR):
                                    {
                                        temp_type[++var_temp]='F';
                                        fprintf(quadruple,"\tREAL2INT #%f, , TEMP%c%d\n", root->right->info->value.dval, temp_type[var_temp], var_temp);
                                        fprintf(quadruple,"\t%sI %s, TEMP%c%d, %s\n", operation[root->type], root->left->info->value.id, temp_type[var_temp], var_temp, root->left->info->value.id);
                                        break;
                                    }
                                    case(DOUBLE):
                                    {
                                        fprintf(quadruple,"\t%sF %s, %f, %s\n", operation[root->type], root->left->info->value.id, root->right->info->value.dval, root->left->info->value.id);
                                        break;
                                    }
                                }
                                break;
                            }
                            case ADD_ASSIGN:
                            case SUB_ASSIGN:
                            case DIV_ASSIGN:
                            case MUL_ASSIGN:
                            case MOD_ASSIGN:
                            case '=':
                            {
                                create_quadruple(root->right);
                                if(root->right->left->type == IDENTIFIER)
                                    cast_op_asg_id(operation[root->type], root->left->info, root->right->left->info);
                                break;
                            }
                            default:
                            {
                                create_quadruple(root->right);
                                switch(root->left->info->type)
                                {
                                    case(INT):
                                    case(CHAR):
                                    {
                                        if(temp_type[var_temp]=='I')
                                            fprintf(quadruple,"\t%sI TEMP%c%d, , %s\n", operation[root->type], temp_type[var_temp], var_temp, root->left->info->value.id);
                                        else if(temp_type[var_temp]=='F')
                                        {
                                            var_temp_rh=var_temp;
                                            temp_type[++var_temp]='I';
                                            fprintf(quadruple,"\tREAL2INT TEMP%c%d, , TEMP%c%d\n", temp_type[var_temp_rh], var_temp_rh, temp_type[var_temp], var_temp);
                                            fprintf(quadruple,"\t%sI TEMP%c%d, , %s\n", operation[root->type], temp_type[var_temp], var_temp, root->left->info->value.id);
                                        }
                                        break;
                                    }
                                    case(DOUBLE):
                                    {
                                        if(temp_type[var_temp]=='I')
                                        {
                                            var_temp_rh=var_temp;
                                            temp_type[++var_temp]='F';
                                            fprintf(quadruple,"\tINT2REAL TEMP%c%d, , TEMP%c%d\n", temp_type[var_temp_rh], var_temp_rh, temp_type[var_temp], var_temp);
                                            fprintf(quadruple,"\t%sF TEMP%c%d, , %s\n", operation[root->type], temp_type[var_temp], var_temp, root->left->info->value.id);
                                        }
                                        else if(temp_type[var_temp]=='F')
                                        {
                                            fprintf(quadruple,"\t%sF TEMP%c%d, , %s\n", operation[root->type], temp_type[var_temp], var_temp, root->left->info->value.id);
                                        }
                                        break;
                                    }
                                }
                                break;
                            }
                        }
                    }
                }
                break;
            }
            case '!':
            {
                if(root->left==NULL)
                {
                    switch(root->right->type)
                    {
                        case(UNSIGNED_NUM):
                        {
                            temp_type[++var_temp]='I';
                            fprintf(quadruple,"\tNOT #%d, , TEMP%c%d\n", root->right->info->value.ival, temp_type[var_temp], var_temp);
                            break;
                        }
                        case(IDENTIFIER):
                        {
                            switch(root->right->info->type)
                            {
                                case(INT):
                                case(CHAR):
                                {
                                    temp_type[++var_temp]='I';
                                    fprintf(quadruple,"\tNOT %s, , TEMP%c%d\n", root->right->info->value.id, temp_type[var_temp], var_temp);
                                    break;
                                }
                                case(DOUBLE):
                                {
                                    temp_type[++var_temp]='F';
                                    fprintf(quadruple,"\tNOT %s, , TEMP%c%d\n", root->right->info->value.id, temp_type[var_temp], var_temp);
                                    break;
                                }
                            }
                            break;
                        }
                        case(CHARACTER):
                        {
                            temp_type[++var_temp]='I';
                            fprintf(quadruple,"\tNOT #%d, , TEMP%c%d\n", root->right->info->value.cval, temp_type[var_temp], var_temp);
                            break;
                        }
                        case(FLOATING):
                        {
                            temp_type[++var_temp]='F';
                            fprintf(quadruple,"\tNOT #%f, , TEMP%c%d\n", root->right->info->value.dval, temp_type[var_temp], var_temp);
                            break;
                        }
                        case '<':
                        {
                            root->right->type=GE_OP;
                            create_quadruple(root->right);
                            break;
                        }
                        case '>':
                        {
                            root->right->type=LE_OP;
                            create_quadruple(root->right);
                            break;
                        }
                        case LE_OP:
                        {
                            root->right->type='>';
                            create_quadruple(root->right);
                            break;
                        }
                        case GE_OP:
                        {
                            root->right->type='<';
                            create_quadruple(root->right);
                            break;
                        }
                        case EQ_OP:
                        {
                            root->right->type=NE_OP;
                            create_quadruple(root->right);
                            break;
                        }
                        case NE_OP:
                        {
                            root->right->type=EQ_OP;
                            create_quadruple(root->right);
                            break;
                        }
                        default:
                        {
                            create_quadruple(root->right);
                            var_temp_rh=var_temp;
                            if(temp_type[var_temp_rh]=='I')
                            {
                                temp_type[++var_temp]='I';
                                fprintf(quadruple,"\tNOT TEMP%c%d, , TEMP%c%d\n", temp_type[var_temp_rh], var_temp_rh, temp_type[var_temp], var_temp);
                            }
                            else if(temp_type[var_temp_rh]=='F')
                            {
                                temp_type[++var_temp]='F';
                                fprintf(quadruple,"\tNOT TEMP%c%d, , TEMP%c%d\n", temp_type[var_temp_rh], var_temp_rh, temp_type[var_temp], var_temp);
                            }
                            break;
                        }
                    }
                }
                break;
            }
            case '-':
            {
                if(root->left==NULL)
                {
                    switch(root->right->type)
                    {
                        case(UNSIGNED_NUM):
                        {
                            temp_type[++var_temp]='I';
                            fprintf(quadruple,"\tUMINUS #%d, , TEMP%c%d\n", root->right->info->value.ival, temp_type[var_temp], var_temp);
                            break;
                        }
                        case(IDENTIFIER):
                        {
                            switch(root->right->info->type)
                            {
                                case(INT):
                                case(CHAR):
                                {
                                    temp_type[++var_temp]='I';
                                    fprintf(quadruple,"\tUMINUS %s, , TEMP%c%d\n", root->right->info->value.id, temp_type[var_temp], var_temp);
                                    break;
                                }
                                case(DOUBLE):
                                {
                                    temp_type[++var_temp]='F';
                                    fprintf(quadruple,"\tUMINUS %s, , TEMP%c%d\n", root->right->info->value.id, temp_type[var_temp], var_temp);
                                    break;
                                }
                            }
                            break;
                        }
                        case(CHARACTER):
                        {
                            temp_type[++var_temp]='I';
                            fprintf(quadruple,"\tUMINUS %d, , TEMP%c%d\n", root->right->info->value.cval, temp_type[var_temp], var_temp);
                            break;
                        }
                        case(FLOATING):
                        {
                            temp_type[++var_temp]='F';
                            fprintf(quadruple,"\tUMINUS #%f, , TEMP%c%d\n", root->right->info->value.dval, temp_type[var_temp], var_temp);
                            break;
                        }
                        default:
                        {
                            create_quadruple(root->right);
                            var_temp_rh=var_temp;
                            if(temp_type[var_temp_rh]=='I')
                            {
                                temp_type[++var_temp]='I';
                                fprintf(quadruple,"\tUMINUS TEMP%c%d, , TEMP%c%d\n", temp_type[var_temp_rh], var_temp_rh, temp_type[var_temp], var_temp);
                            }
                            else if(temp_type[var_temp_rh]=='F')
                            {
                                temp_type[++var_temp]='F';
                                fprintf(quadruple,"\tUMINUS TEMP%c%d, , TEMP%c%d\n", temp_type[var_temp_rh], var_temp_rh, temp_type[var_temp], var_temp);
                            }
                            break;
                        }
                    }
                }
                // There is not the break, because the minus symbol is also defined as binary operator
            }
            case '+':
            case '*':
            case '/':
            case '%':
            case '&':
            case '|':
            case '^':
            case LEFT_OP:
            case RIGHT_OP:
            {
                if(root->left)
                {
                    switch(root->left->type)
                    {
                        case(UNSIGNED_NUM):
                        {
                            switch(root->right->type)
                            {
                                case(UNSIGNED_NUM):
                                {
                                    temp_type[++var_temp]='I';
                                    fprintf(quadruple,"\t%sI #%d, #%d, TEMP%c%d\n", operation[root->type], root->left->info->value.ival, root->right->info->value.ival, temp_type[var_temp], var_temp);
                                    break;
                                }
                                case(IDENTIFIER):
                                {
                                    switch(root->right->info->type)
                                    {
                                        case(INT):
                                        {
                                            temp_type[++var_temp]='I';
                                            fprintf(quadruple,"\t%sI #%d, %s, TEMP%c%d\n", operation[root->type], root->left->info->value.ival, root->right->info->value.id, temp_type[var_temp], var_temp);
                                            break;
                                        }
                                        case(CHAR):
                                        {
                                            temp_type[++var_temp]='I';
                                            fprintf(quadruple,"\t%sI #%d, %s, TEMP%c%d\n", operation[root->type], root->left->info->value.ival, root->right->info->value.id, temp_type[var_temp], var_temp);
                                            break;
                                        }
                                        case(DOUBLE):
                                        {
                                            var_temp_lh=++var_temp;
                                            temp_type[var_temp_lh]=temp_type[++var_temp]='F';
                                            fprintf(quadruple,"\tINT2REAL #%d, , TEMP%c%d\n", root->left->info->value.ival, temp_type[var_temp_lh], var_temp_lh);
                                            fprintf(quadruple,"\t%sF TEMP%c%d, %s, TEMP%c%d\n", operation[root->type],temp_type[var_temp_lh], var_temp_lh, root->right->info->value.id, temp_type[var_temp], var_temp);
                                            break;
                                        }
                                    }
                                    break;
                                }
                                case(CHARACTER):
                                {
                                    temp_type[++var_temp]='I';
                                    fprintf(quadruple,"\t%sI #%d, #%d, TEMP%c%d\n", operation[root->type], root->left->info->value.ival, root->right->info->value.cval, temp_type[var_temp], var_temp);
                                    break;
                                }
                                case(FLOATING):
                                {
                                    var_temp_lh=++var_temp;
                                    temp_type[var_temp_lh]=temp_type[++var_temp]='F';
                                    fprintf(quadruple,"\tINT2REAL #%d, , TEMP%c%d\n", root->left->info->value.ival, temp_type[var_temp_lh], var_temp_lh);
                                    fprintf(quadruple,"\t%sF TEMP%c%d, #%f, TEMP%c%d\n", operation[root->type], temp_type[var_temp_lh], var_temp_lh, root->right->info->value.dval, temp_type[var_temp], var_temp);
                                    break;
                                }
                                default:
                                {
                                    create_quadruple(root->right);
                                    /*  It is necessary to store the var_temp's value in var_temp_rh, because the var_temp's value is incremented in the fprintf. */
                                    var_temp_rh=var_temp;
                                    if(temp_type[var_temp_rh]=='I')
                                    {
                                        temp_type[++var_temp]='I';
                                        fprintf(quadruple,"\t%sI #%d, TEMP%c%d, TEMP%c%d\n", operation[root->type], root->left->info->value.ival, temp_type[var_temp_rh], var_temp_rh, temp_type[var_temp], var_temp);
                                    }
                                    else if(temp_type[var_temp_rh]=='F')
                                    {
                                        var_temp_lh=++var_temp;
                                        temp_type[var_temp_lh]=temp_type[++var_temp]='F';
                                        fprintf(quadruple,"\tINT2REAL #%d, , TEMP%c%d\n", root->left->info->value.ival, temp_type[var_temp_lh], var_temp_lh);
                                        fprintf(quadruple,"\t%sF TEMP%c%d, TEMP%c%d, TEMP%c%d\n", operation[root->type], temp_type[var_temp_lh], var_temp_lh, temp_type[var_temp_rh], var_temp_rh, temp_type[var_temp], var_temp);
                                    }
                                    break;
                                }
                            }
                            break;
                        }
                        case(CHARACTER):
                        {
                            switch(root->right->type)
                            {
                                case(UNSIGNED_NUM):
                                {
                                    temp_type[++var_temp]='I';
                                    fprintf(quadruple,"\t%sI #%d, #%d, TEMP%c%d\n", operation[root->type], root->left->info->value.cval, root->right->info->value.ival, temp_type[var_temp], var_temp);
                                    break;
                                }
                                case(IDENTIFIER):
                                {
                                    switch(root->right->info->type)
                                    {
                                        case(INT):
                                        {
                                            temp_type[++var_temp]='I';
                                            fprintf(quadruple,"\t%sI #%d, %s, TEMP%c%d\n", operation[root->type], root->left->info->value.cval, root->right->info->value.id, temp_type[var_temp], var_temp);
                                            break;
                                        }
                                        case(CHAR):
                                        {
                                            temp_type[++var_temp]='I';
                                            fprintf(quadruple,"\t%sI #%d, %s, TEMP%c%d\n", operation[root->type], root->left->info->value.cval, root->right->info->value.id, temp_type[var_temp], var_temp);
                                            break;
                                        }
                                        case(DOUBLE):
                                        {
                                            var_temp_lh=++var_temp;
                                            temp_type[var_temp_lh]=temp_type[++var_temp]='F';
                                            fprintf(quadruple,"\tINT2REAL #%d, , TEMP%c%d\n", root->left->info->value.cval, temp_type[var_temp_lh], var_temp_lh);
                                            fprintf(quadruple,"\t%sF TEMP%c%d, %s, TEMP%c%d\n", operation[root->type],temp_type[var_temp_lh], var_temp_lh, root->right->info->value.id, temp_type[var_temp], var_temp);
                                        }
                                    }
                                    break;
                                }
                                case(CHARACTER):
                                {
                                    temp_type[++var_temp]='I';
                                    fprintf(quadruple,"\t%sI #%d, #%d, TEMP%c%d\n", operation[root->type], root->left->info->value.cval, root->right->info->value.cval, temp_type[var_temp], var_temp);
                                    break;
                                }
                                case(FLOATING):
                                {
                                    var_temp_lh=++var_temp;
                                    temp_type[var_temp_lh]=temp_type[++var_temp]='F';
                                    fprintf(quadruple,"\tINT2REAL #%d, , TEMP%c%d\n", root->left->info->value.cval, temp_type[var_temp_lh], var_temp_lh);
                                    fprintf(quadruple,"\t%sF TEMP%c%d, #%f, TEMP%c%d\n", operation[root->type], temp_type[var_temp_lh], var_temp_lh, root->right->info->value.dval, temp_type[var_temp], var_temp);
                                    break;
                                }
                                default:
                                {
                                    create_quadruple(root->right);
                                    var_temp_rh=var_temp;
                                    if(temp_type[var_temp_rh]=='I')
                                    {
                                        temp_type[++var_temp]='I';
                                        fprintf(quadruple,"\t%sI #%d, TEMP%c%d, TEMP%c%d\n", operation[root->type], root->left->info->value.cval, temp_type[var_temp_rh], var_temp_rh, temp_type[var_temp], var_temp);
                                    }
                                    else if(temp_type[var_temp_rh]=='F')
                                    {
                                        var_temp_lh=++var_temp;
                                        temp_type[var_temp_lh]=temp_type[++var_temp]='F';
                                        fprintf(quadruple,"\tINT2REAL #%d, , TEMP%c%d\n", root->left->info->value.cval, temp_type[var_temp_lh], var_temp_lh);
                                        fprintf(quadruple,"\t%sF TEMP%c%d, TEMP%c%d, TEMP%c%d\n", operation[root->type], temp_type[var_temp_lh], var_temp_lh, temp_type[var_temp_rh], var_temp_rh, temp_type[var_temp], var_temp);
                                    }
                                    break;
                                }
                            }
                            break;
                        }
                        case(FLOATING):
                        {
                            switch(root->right->type)
                            {
                                case(UNSIGNED_NUM):
                                {
                                    var_temp_rh=++var_temp;
                                    temp_type[var_temp_rh]=temp_type[++var_temp]='F';
                                    fprintf(quadruple,"\tINT2REAL #%d, , TEMP%c%d\n", root->right->info->value.ival, temp_type[var_temp_rh], var_temp_rh);
                                    fprintf(quadruple,"\t%sF #%f, TEMP%c%d, TEMP%c%d\n", operation[root->type], root->left->info->value.dval, temp_type[var_temp_rh], var_temp_rh, temp_type[var_temp], var_temp);
                                    break;
                                }
                                case(IDENTIFIER):
                                {
                                     switch(root->right->info->type)
                                    {
                                        case(INT):
                                        {
                                            var_temp_rh=++var_temp;
                                            temp_type[var_temp_rh]=temp_type[++var_temp]='F';
                                            fprintf(quadruple,"\tINT2REAL %s, , TEMP%c%d\n", root->right->info->value.id, temp_type[var_temp_rh], var_temp_rh);
                                            fprintf(quadruple,"\t%sF #%f, TEMP%c%d, TEMP%c%d\n", operation[root->type], root->left->info->value.dval, temp_type[var_temp_rh], var_temp_rh, temp_type[var_temp], var_temp);
                                            break;
                                        }
                                        case(CHAR):
                                        {
                                            var_temp_rh=++var_temp;
                                            temp_type[var_temp_rh]=temp_type[++var_temp]='F';
                                            fprintf(quadruple,"\tINT2REAL %s, , TEMP%c%d\n", root->right->info->value.id, temp_type[var_temp_rh], var_temp_rh);
                                            fprintf(quadruple,"\t%sF #%f, TEMP%c%d, TEMP%c%d\n", operation[root->type], root->left->info->value.dval, temp_type[var_temp_rh], var_temp_rh, temp_type[var_temp], var_temp);
                                            break;
                                        }
                                        case(DOUBLE):
                                        {
                                            temp_type[++var_temp]='F';
                                            fprintf(quadruple,"\t%sF #%f, %s, TEMP%c%d\n", operation[root->type], root->left->info->value.dval, root->right->info->value.id, temp_type[var_temp], var_temp);
                                            break;
                                        }
                                    }
                                    break;
                                }
                                case(CHARACTER):
                                {
                                    var_temp_rh=++var_temp;
                                    temp_type[var_temp_rh]=temp_type[++var_temp]='F';
                                    fprintf(quadruple,"\tINT2REAL #%d, , TEMP%c%d\n", root->right->info->value.cval, temp_type[var_temp_rh], var_temp_rh);
                                    fprintf(quadruple,"\t%sF #%f, TEMP%c%d, TEMP%c%d\n", operation[root->type], root->left->info->value.dval, temp_type[var_temp_rh], var_temp_rh, temp_type[var_temp], var_temp);
                                    break;
                                }
                                case(FLOATING):
                                {
                                    temp_type[++var_temp]='F';
                                    fprintf(quadruple,"\t%sF #%f, #%f, TEMP%c%d\n", operation[root->type], root->left->info->value.dval, root->right->info->value.dval, temp_type[var_temp], var_temp);
                                    break;
                                }
                                default:
                                {
                                    create_quadruple(root->right);
                                    var_temp_rh=var_temp;
                                    if(temp_type[var_temp_rh]=='I')
                                    {
                                        var_temp_lh=++var_temp;
                                        temp_type[var_temp_lh]=temp_type[++var_temp]='F';
                                        fprintf(quadruple,"\tINT2REAL TEMP%c%d, , TEMP%c%d\n", temp_type[var_temp_rh], var_temp_rh, temp_type[var_temp_lh], var_temp_lh);
                                        fprintf(quadruple,"\t%sF #%f, TEMP%c%d, TEMP%c%d\n", operation[root->type], root->left->info->value.dval, temp_type[var_temp_lh], var_temp_lh, temp_type[var_temp], var_temp);
                                    }
                                    else if(temp_type[var_temp_rh]=='F')
                                    {
                                        temp_type[++var_temp]='F';
                                        fprintf(quadruple,"\t%sF #%f, TEMP%c%d, TEMP%c%d\n", operation[root->type], root->left->info->value.dval, temp_type[var_temp_rh], var_temp_rh, temp_type[var_temp], var_temp);
                                    }
                                    break;
                                }
                            }
                            break;
                        }
                        case(IDENTIFIER):
                        {
                            switch(root->right->type)
                            {
                                case(UNSIGNED_NUM):
                                {
                                    switch(root->left->info->type)
                                    {
                                        case(INT):
                                        {
                                            temp_type[++var_temp]='I';
                                            fprintf(quadruple,"\t%sI %s, #%d, TEMP%c%d\n", operation[root->type], root->left->info->value.id, root->right->info->value.ival, temp_type[var_temp], var_temp);
                                            break;
                                        }
                                        case(CHAR):
                                        {
                                            temp_type[++var_temp]='I';
                                            fprintf(quadruple,"\t%sI %s, #%d, TEMP%c%d\n", operation[root->type], root->left->info->value.id, root->right->info->value.ival, temp_type[var_temp], var_temp);
                                            break;
                                        }
                                        case(DOUBLE):
                                        {
                                            var_temp_rh=++var_temp;
                                            temp_type[var_temp_rh]=temp_type[++var_temp]='F';
                                            fprintf(quadruple,"\tINT2REAL #%d, , TEMP%c%d\n", root->right->info->value.ival, temp_type[var_temp_rh], var_temp_rh);
                                            fprintf(quadruple,"\t%sF %s, TEMP%c%d, TEMP%c%d\n", operation[root->type], root->left->info->value.id, temp_type[var_temp_rh], var_temp_rh, temp_type[var_temp], var_temp);
                                            break;
                                        }
                                    }
                                    break;
                                }
                                case(CHARACTER):
                                {
                                    switch(root->left->info->type)
                                    {
                                        case(INT):
                                        {
                                            temp_type[++var_temp]='I';
                                            fprintf(quadruple,"\t%sI %s, #%d, TEMP%c%d\n", operation[root->type], root->left->info->value.id, root->right->info->value.cval, temp_type[var_temp], var_temp);
                                            break;
                                        }
                                        case(CHAR):
                                        {
                                            temp_type[++var_temp]='I';
                                            fprintf(quadruple,"\t%sI %s, #%d, TEMP%c%d\n", operation[root->type], root->left->info->value.id, root->right->info->value.cval, temp_type[var_temp], var_temp);
                                            break;
                                        }
                                        case(DOUBLE):
                                        {
                                            var_temp_rh=++var_temp;
                                            temp_type[var_temp_rh]=temp_type[++var_temp]='F';
                                            fprintf(quadruple,"\tINT2REAL #%d, , TEMP%c%d\n", root->right->info->value.cval, temp_type[var_temp_rh], var_temp_rh);
                                            fprintf(quadruple,"\t%sF %s, TEMP%c%d, TEMP%c%d\n", operation[root->type], root->left->info->value.id, temp_type[var_temp_rh], var_temp_rh, temp_type[var_temp], var_temp);
                                            break;
                                        }
                                    }
                                    break;
                                }
                                case(FLOATING):
                                {
                                    switch(root->left->info->type)
                                    {
                                        case(CHAR):
                                        case(INT):
                                        {
                                            var_temp_rh=++var_temp;
                                            temp_type[var_temp_rh]=temp_type[++var_temp]='F';
                                            fprintf(quadruple,"\tINT2REAL %s, , TEMP%c%d\n", root->left->info->value.id, temp_type[var_temp_rh], var_temp_rh);
                                            fprintf(quadruple,"\t%sF TEMP%c%d, #%f, TEMP%c%d\n", operation[root->type], temp_type[var_temp_rh], var_temp_rh,root->right->info->value.dval, temp_type[var_temp], var_temp);
                                            break;
                                        }
                                        case(DOUBLE):
                                        {
                                            temp_type[++var_temp]='F';
                                            fprintf(quadruple,"\t%sF %s, #%f, TEMP%c%d\n", operation[root->type], root->left->info->value.id, root->right->info->value.dval, temp_type[var_temp], var_temp);
                                            break;
                                        }
                                    }
                                    break;
                                }
                                case(IDENTIFIER):
                                {
                                    switch(root->left->info->type)
                                    {
                                        case(CHAR):
                                        case(INT):
                                        {
                                            switch(root->right->info->type)
                                            {
                                                case(CHAR):
                                                case(INT):
                                                {
                                                    temp_type[++var_temp]='I';
                                                    //fprintf(quadruple,"\t%s %s, %s, TEMP%c%d\n", operation[root->type], root->left->info->value.id, root->right->info->value.id, temp_type[var_temp], var_temp);
                                                    fprintf(quadruple,"\t%sI %s, %s, TEMP%c%d\n", operation[root->type], root->left->info->value.id, root->right->info->value.id, temp_type[var_temp], var_temp);
                                                    break;
                                                }
                                                case(DOUBLE):
                                                {
                                                    var_temp_lh=++var_temp;
                                                    temp_type[var_temp_lh]=temp_type[++var_temp]='F';
                                                    fprintf(quadruple,"\tINT2REAL %s, , TEMP%c%d\n", root->left->info->value.id, temp_type[var_temp_lh], var_temp_lh);
                                                    fprintf(quadruple,"\t%sF TEMP%c%d, %s, TEMP%c%d\n", operation[root->type], temp_type[var_temp_lh], var_temp_lh, root->right->info->value.id, temp_type[var_temp], var_temp);
                                                    break;
                                                }
                                            }
                                            break;
                                        }
                                        case(DOUBLE):
                                        {
                                            switch(root->right->info->type)
                                            {
                                                case(CHAR):
                                                case(INT):
                                                {
                                                    var_temp_rh=++var_temp;
                                                    temp_type[var_temp_rh]=temp_type[++var_temp]='F';
                                                    fprintf(quadruple,"\tINT2REAL %s, , TEMP%c%d\n", root->right->info->value.id, temp_type[var_temp_rh], var_temp_rh);
                                                    fprintf(quadruple,"\t%sF %s, TEMP%c%d, TEMP%c%d\n", operation[root->type], root->left->info->value.id, temp_type[var_temp_rh], var_temp_rh, temp_type[var_temp], var_temp);
                                                    break;
                                                }
                                                case(DOUBLE):
                                                {
                                                    temp_type[++var_temp]='F';
                                                    fprintf(quadruple,"\t%sF %s, %s, TEMP%c%d\n", operation[root->type], root->left->info->value.id, root->right->info->value.id,temp_type[var_temp], var_temp);
                                                    break;
                                                }
                                            }
                                            break;
                                        }
                                    }
                                    break;
                                }
                                default:
                                {
                                    create_quadruple(root->right);
                                    var_temp_rh=var_temp;
                                    if(temp_type[var_temp_rh]=='I')
                                    {
                                        switch(root->left->info->type)
                                        {
                                            case(CHAR):
                                            case(INT):
                                            {
                                                temp_type[++var_temp]='I';
                                                fprintf(quadruple,"\t%sI %s, TEMP%c%d, TEMP%c%d\n", operation[root->type], root->left->info->value.id, temp_type[var_temp_rh], var_temp_rh, temp_type[var_temp], var_temp);
                                                break;
                                            }
                                            case(DOUBLE):
                                            {
                                                var_temp_lh=++var_temp;
                                                temp_type[var_temp_lh]=temp_type[++var_temp]='F';
                                                fprintf(quadruple,"\tINT2REAL TEMP%c%d, , TEMP%c%d\n", temp_type[var_temp_rh], var_temp_rh, temp_type[var_temp_lh], var_temp_lh);
                                                fprintf(quadruple,"\t%sF %s, TEMP%c%d, TEMP%c%d\n", operation[root->type], root->left->info->value.id, temp_type[var_temp_lh], var_temp_lh, temp_type[var_temp], var_temp);
                                                break;
                                            }
                                        }
                                    }
                                    break;
                                }
                            }
                            break;
                        }
                        default:
                        {
                            switch(root->right->type)
                            {
                                case(UNSIGNED_NUM):
                                {
                                    var_temp_lh=var_temp;
                                    if(temp_type[var_temp_lh]=='I')
                                    {
                                        temp_type[++var_temp]='I';
                                        fprintf(quadruple,"\t%sI TEMP%c%d, #%d, TEMP%c%d\n", operation[root->type], temp_type[var_temp_lh], var_temp_lh, root->right->info->value.ival, temp_type[var_temp], var_temp);
                                    }
                                    else if(temp_type[var_temp_lh]=='F')
                                    {
                                        var_temp_rh=++var_temp;
                                        temp_type[var_temp_rh]=temp_type[++var_temp]='F';
                                        fprintf(quadruple,"\tINT2REAL #%d, , TEMP%c%d\n", root->right->info->value.ival, temp_type[var_temp_rh], var_temp_rh);
                                        fprintf(quadruple,"\t%sF TEMP%c%d, TEMP%c%d, TEMP%c%d\n", operation[root->type], temp_type[var_temp_lh], var_temp_lh, temp_type[var_temp_rh], var_temp_rh, temp_type[var_temp], var_temp);
                                    }
                                   break;
                                }
                                case(CHARACTER):
                                {
                                    var_temp_lh=var_temp;
                                    if(temp_type[var_temp_lh]=='I')
                                    {
                                        temp_type[++var_temp]='I';
                                        fprintf(quadruple,"\t%sI TEMP%c%d, #%d, TEMP%c%d\n", operation[root->type], temp_type[var_temp_lh], var_temp_lh, root->right->info->value.cval, temp_type[var_temp], var_temp);
                                    }
                                    else if(temp_type[var_temp_lh]=='F')
                                    {
                                        var_temp_rh=++var_temp;
                                        temp_type[var_temp_rh]=temp_type[++var_temp]='F';
                                        fprintf(quadruple,"\tINT2REAL #%d, , TEMP%c%d\n", root->right->info->value.cval, temp_type[var_temp_rh], var_temp_rh);
                                        fprintf(quadruple,"\t%sF TEMP%c%d, TEMP%c%d, TEMP%c%d\n", operation[root->type], temp_type[var_temp_lh], var_temp_lh, temp_type[var_temp_rh], var_temp_rh, temp_type[var_temp], var_temp);
                                    }
                                    break;
                                }
                                case(FLOATING):
                                {
                                    var_temp_lh=var_temp;
                                    if(temp_type[var_temp_lh]=='I')
                                    {
                                        var_temp_rh=++var_temp;
                                        temp_type[var_temp_rh]=temp_type[++var_temp]='F';
                                        fprintf(quadruple,"\tINT2REAL TEMP%c%d, , TEMP%c%d\n", temp_type[var_temp_lh], var_temp_lh, temp_type[var_temp_rh], var_temp_rh);
                                        fprintf(quadruple,"\t%sF TEMP%c%d, #%f, TEMP%c%d\n", operation[root->type], temp_type[var_temp_rh], var_temp_rh, root->right->info->value.dval, temp_type[var_temp], var_temp);
                                    }
                                    else if(temp_type[var_temp_lh]=='F')
                                    {
                                        temp_type[++var_temp]='F';
                                        fprintf(quadruple,"\t%sF TEMP%c%d, #%f, TEMP%c%d\n", operation[root->type], temp_type[var_temp_lh], var_temp_lh, root->right->info->value.dval, temp_type[var_temp], var_temp);
                                    }
                                    break;
                                }
                                case(IDENTIFIER):
                                {
                                    switch(root->right->info->type)
                                    {
                                        case(CHAR):
                                        case(INT):
                                        {
                                            var_temp_lh=var_temp;
                                            if(temp_type[var_temp_lh]=='I')
                                            {
                                                temp_type[++var_temp]='I';
                                                fprintf(quadruple,"\t%sI TEMP%c%d, %s, TEMP%c%d\n", operation[root->type], temp_type[var_temp_lh], var_temp_lh, root->right->info->value.id, temp_type[var_temp], var_temp);
                                            }
                                            else if(temp_type[var_temp_lh]=='F')
                                            {
                                                var_temp_rh=++var_temp;
                                                temp_type[var_temp_rh]=temp_type[++var_temp]='F';
                                                fprintf(quadruple,"\tINT2REAL %s, , TEMP%c%d\n", root->right->info->value.id, temp_type[var_temp_rh], var_temp_rh);
                                                fprintf(quadruple,"\t%sF TEMP%c%d, TEMP%c%d, TEMP%c%d\n", operation[root->type], temp_type[var_temp_lh], var_temp_lh, temp_type[var_temp_rh], var_temp_rh, temp_type[var_temp], var_temp);
                                            }
                                            break;
                                        }
                                        case(DOUBLE):
                                        {
                                            var_temp_lh=var_temp;
                                            if(temp_type[var_temp_lh]=='I')
                                            {
                                                var_temp_rh=++var_temp;
                                                temp_type[var_temp_rh]=temp_type[++var_temp]='F';
                                                fprintf(quadruple,"\tINT2REAL TEMP%c%d, , TEMP%c%d\n", temp_type[var_temp_lh], var_temp_lh, temp_type[var_temp_rh], var_temp_rh);
                                                fprintf(quadruple,"\t%sF TEMP%c%d, %s, TEMP%c%d\n", operation[root->type], temp_type[var_temp_rh], var_temp_rh, root->right->info->value.id, temp_type[var_temp], var_temp);
                                            }
                                            else if(temp_type[var_temp_lh]=='F')
                                            {
                                                temp_type[++var_temp]='F';
                                                fprintf(quadruple,"\t%sF TEMP%c%d, %s, TEMP%c%d\n", operation[root->type], temp_type[var_temp_lh], var_temp_lh, root->right->info->value.id, temp_type[var_temp], var_temp);
                                            }
                                            break;
                                        }
                                    }
                                    break;
                                }
                                default:
                                {
                                    var_temp_lh_lh=var_temp;
                                    create_quadruple(root->right);
                                    var_temp_rh=var_temp;
                                    if(temp_type[var_temp_lh_lh]=='I')
                                    {
                                        if(temp_type[var_temp_rh]=='I')
                                        {
                                            temp_type[++var_temp]='I';
                                            //fprintf(quadruple,"\t%s TEMP%c%d, TEMP%c%d, TEMP%c%d\n", operation[root->type], temp_type[var_temp_lh_lh], var_temp_lh_lh, temp_type[var_temp_rh], var_temp_rh, temp_type[var_temp], var_temp);
                                            fprintf(quadruple,"\t%sI TEMP%c%d, TEMP%c%d, TEMP%c%d\n", operation[root->type], temp_type[var_temp_lh_lh], var_temp_lh_lh, temp_type[var_temp_rh], var_temp_rh, temp_type[var_temp], var_temp);
                                        }
                                        else if(temp_type[var_temp_rh]=='F')
                                        {
                                            var_temp_lh=++var_temp;
                                            temp_type[var_temp_lh]=temp_type[++var_temp]='F';
                                            fprintf(quadruple,"\tINT2REAL TEMP%c%d, , TEMP%c%d\n", temp_type[var_temp_lh_lh], var_temp_lh_lh, temp_type[var_temp_lh], var_temp_lh);
                                            //fprintf(quadruple,"\t%s TEMP%c%d, TEMP%c%d, TEMP%c%d\n", operation[root->type], temp_type[var_temp_lh], var_temp_lh, temp_type[var_temp_rh], var_temp_rh, temp_type[var_temp], var_temp);
                                            fprintf(quadruple,"\t%sF TEMP%c%d, TEMP%c%d, TEMP%c%d\n", operation[root->type], temp_type[var_temp_lh], var_temp_lh, temp_type[var_temp_rh], var_temp_rh, temp_type[var_temp], var_temp);
                                        }
                                    }
                                    else if(temp_type[var_temp_lh_lh]=='F')
                                    {
                                        if(temp_type[var_temp_rh]=='I')
                                        {
                                            var_temp_lh=++var_temp;
                                            temp_type[var_temp_lh]=temp_type[++var_temp]='F';
                                            fprintf(quadruple,"\tINT2REAL TEMP%c%d, , TEMP%c%d\n", temp_type[var_temp_rh], var_temp_rh, temp_type[var_temp_lh], var_temp_lh);
                                            //fprintf(quadruple,"\t%s TEMP%c%d, TEMP%c%d, TEMP%c%d\n", operation[root->type],  temp_type[var_temp_lh_lh], var_temp_lh_lh, temp_type[var_temp_lh], var_temp_lh, temp_type[var_temp], var_temp);
                                            fprintf(quadruple,"\t%sF TEMP%c%d, TEMP%c%d, TEMP%c%d\n", operation[root->type],  temp_type[var_temp_lh_lh], var_temp_lh_lh, temp_type[var_temp_lh], var_temp_lh, temp_type[var_temp], var_temp);
                                        }
                                        else if(temp_type[var_temp_rh]=='F')
                                        {
                                            temp_type[++var_temp]='F';
                                            //fprintf(quadruple,"\t%s TEMP%c%d, TEMP%c%d, TEMP%c%d\n", operation[root->type], temp_type[var_temp_lh_lh], var_temp_lh_lh, temp_type[var_temp_rh], var_temp_rh, temp_type[var_temp], var_temp);
                                            fprintf(quadruple,"\t%sF TEMP%c%d, TEMP%c%d, TEMP%c%d\n", operation[root->type], temp_type[var_temp_lh_lh], var_temp_lh_lh, temp_type[var_temp_rh], var_temp_rh, temp_type[var_temp], var_temp);
                                        }
                                    }
                                    break;
                                }
                            }
                            break;
                        }
                    }
                }
                break;
            }
            case INC_OP:
            case DEC_OP:
            {
                // ++y
                if(root->right)
                {
                    switch(root->right->type)
                    {
                        case(IDENTIFIER):
                        {
                            switch(root->right->info->type)
                            {
                                case(CHAR):
                                case(INT):
                                {
                                    fprintf(quadruple,"\t%sI #1, %s, %s\n", operation[root->type], root->right->info->value.id, root->right->info->value.id);
                                    break;
                                }
                                case(DOUBLE):
                                {
                                    fprintf(quadruple,"\t%sF #1.000000, %s, %s\n", operation[root->type], root->right->info->value.id, root->right->info->value.id);
                                    break;
                                }
                            }
                            break;
                        }
                    }
                }
                // y++
                if(root->left)
                {
                    switch(root->left->type)
                    {
                        case(IDENTIFIER):
                        {
                            switch(root->left->info->type)
                            {
                                case(CHAR):
                                case(INT):
                                {
                                    fprintf(quadruple,"\t%sI #1, %s, %s\n", operation[root->type], root->left->info->value.id, root->left->info->value.id);
                                    break;
                                }
                                case(DOUBLE):
                                {
                                    fprintf(quadruple,"\t%sF #1.000000, %s, %s\n", operation[root->type], root->left->info->value.id, root->left->info->value.id);
                                    break;
                                }
                            }
                            break;
                        }
                    }
                }
                break;
            }
            case '<':
            case '>':
            case LE_OP:
            case GE_OP:
            case EQ_OP:
            case NE_OP:
            {
                switch(root->left->type)
                {
                    case(UNSIGNED_NUM):
                    {
                        switch(root->right->type)
                        {
                            case(UNSIGNED_NUM):
                            {
                                if(cond_stat==1)
                                {
                                    sprintf(condition,"%s #%d, #%d", operation[root->type], root->left->info->value.ival, root->right->info->value.ival);
                                    cond_stat=0;
                                }
                                else
                                {
                                    temp_type[++var_temp]='I';
                                    fprintf(quadruple,"\tCMPI%s #%d, #%d, TEMP%c%d\n", operation[root->type], root->left->info->value.ival, root->right->info->value.ival, temp_type[var_temp], var_temp);
                                }
                                break;
                            }
                            case(CHARACTER):
                            {
                                if(cond_stat==1)
                                {
                                    sprintf(condition,"%s #%d, #%d", operation[root->type], root->left->info->value.ival, root->right->info->value.cval);
                                    cond_stat=0;
                                }
                                else
                                {
                                    temp_type[++var_temp]='I';
                                    fprintf(quadruple,"\tCMPI%s #%d, #%d, TEMP%c%d\n", operation[root->type], root->left->info->value.ival, root->right->info->value.cval, temp_type[var_temp], var_temp);
                                }
                                break;
                            }
                            case(FLOATING):
                            {
                                var_temp_lh=++var_temp;
                                temp_type[var_temp_lh]='F';
                                fprintf(quadruple,"\tINT2REAL #%d, , TEMP%c%d\n", root->left->info->value.ival, temp_type[var_temp_lh], var_temp_lh);
                                if(cond_stat==1)
                                {
                                    sprintf(condition,"%s TEMP%c%d, #%f", operation[root->type], temp_type[var_temp_lh], var_temp_lh, root->right->info->value.dval);
                                    cond_stat=0;
                                }
                                else
                                {
                                    temp_type[++var_temp]='F';
                                    fprintf(quadruple,"\tCMPF%s TEMP%c%d, #%f, TEMP%c%d\n", operation[root->type], temp_type[var_temp_lh], var_temp_lh, root->right->info->value.dval, temp_type[var_temp], var_temp);
                                }
                                break;
                            }
                            case(IDENTIFIER):
                            {
                                switch(root->right->info->type)
                                {
                                    case(CHAR):
                                    case(INT):
                                    {
                                        if(cond_stat==1)
                                        {
                                            sprintf(condition,"%s #%d, %s", operation[root->type], root->left->info->value.ival, root->right->info->value.id);
                                            cond_stat=0;
                                        }
                                        else
                                        {
                                            temp_type[++var_temp]='I';
                                            fprintf(quadruple,"\tCMPI%s #%d, %s, TEMP%c%d\n", operation[root->type], root->left->info->value.ival, root->right->info->value.id, temp_type[var_temp], var_temp);
                                        }
                                        break;
                                    }
                                    case(DOUBLE):
                                    {
                                        var_temp_lh=++var_temp;
                                        temp_type[var_temp_lh]='F';
                                        fprintf(quadruple,"\tINT2REAL #%d, , TEMP%c%d\n", root->left->info->value.ival, temp_type[var_temp_lh], var_temp_lh);
                                        if(cond_stat==1)
                                        {
                                            sprintf(condition,"%s TEMP%c%d, %s", operation[root->type], temp_type[var_temp_lh], var_temp_lh, root->right->info->value.id);
                                            cond_stat=0;
                                        }
                                        else
                                        {
                                            temp_type[++var_temp]='F';
                                            fprintf(quadruple,"\tCMPF%s TEMP%c%d, %s, TEMP%c%d\n", operation[root->type], temp_type[var_temp_lh], var_temp_lh, root->right->info->value.id, temp_type[var_temp], var_temp);
                                        }
                                        break;
                                    }
                                }
                                break;
                            }
                            default:
                            {
                                create_quadruple(root->right);
                                var_temp_rh=var_temp;
                                if(temp_type[var_temp_rh]=='I')
                                {
                                    if(cond_stat==1)
                                    {
                                        sprintf(condition,"%s #%d, TEMP%c%d", operation[root->type], root->left->info->value.ival, temp_type[var_temp_rh], var_temp_rh);
                                        cond_stat=0;
                                    }
                                    else
                                    {
                                        temp_type[++var_temp]='I';
                                        fprintf(quadruple,"\tCMPI%s #%d, TEMP%c%d, TEMP%c%d\n", operation[root->type], root->left->info->value.ival, temp_type[var_temp_rh], var_temp_rh, temp_type[var_temp], var_temp);
                                    }
                                }
                                else if(temp_type[var_temp_rh]=='F')
                                {
                                    var_temp_lh=++var_temp;
                                    temp_type[var_temp_lh]='F';
                                    fprintf(quadruple,"\tINT2REAL #%d, , TEMP%c%d\n", root->left->info->value.ival, temp_type[var_temp_lh], var_temp_lh);
                                    if(cond_stat==1)
                                    {
                                        sprintf(condition,"%s TEMP%c%d, TEMP%c%d", operation[root->type], temp_type[var_temp_lh], var_temp_lh, temp_type[var_temp_rh], var_temp_rh);
                                        cond_stat=0;
                                    }
                                    else
                                    {
                                        temp_type[++var_temp]='F';
                                        fprintf(quadruple,"\tCMPF%s TEMP%c%d, TEMP%c%d, TEMP%c%d\n", operation[root->type], temp_type[var_temp_lh], var_temp_lh, temp_type[var_temp_rh], var_temp_rh, temp_type[var_temp], var_temp);
                                    }
                                }
                                break;
                            }
                        }
                        break;
                    }
                    case(CHARACTER):
                    {
                        switch(root->right->type)
                        {
                            case(UNSIGNED_NUM):
                            {
                                if(cond_stat==1)
                                {
                                    sprintf(condition,"%s #%d, #%d", operation[root->type], root->left->info->value.cval, root->right->info->value.ival);
                                    cond_stat=0;
                                }
                                else
                                {
                                    temp_type[++var_temp]='I';
                                    fprintf(quadruple,"\tCMPI%s #%d, #%d, TEMP%c%d\n", operation[root->type], root->left->info->value.cval, root->right->info->value.ival, temp_type[var_temp], var_temp);
                                }
                                break;
                            }
                            case(CHARACTER):
                            {
                                if(cond_stat==1)
                                {
                                    sprintf(condition,"%s #%d, #%d", operation[root->type], root->left->info->value.cval, root->right->info->value.cval);
                                    cond_stat=0;
                                }
                                else
                                {
                                    temp_type[++var_temp]='I';
                                    fprintf(quadruple,"\tCMPI%s #%d, #%d, TEMP%c%d\n", operation[root->type], root->left->info->value.cval, root->right->info->value.cval, temp_type[var_temp], var_temp);
                                }
                                break;
                            }
                            case(FLOATING):
                            {
                                var_temp_lh=++var_temp;
                                temp_type[var_temp_lh]='F';
                                fprintf(quadruple,"\tINT2REAL #%d, , TEMP%c%d\n", root->left->info->value.cval, temp_type[var_temp_lh], var_temp_lh);
                                if(cond_stat==1)
                                {
                                    sprintf(condition,"%s TEMP%c%d, #%f", operation[root->type], temp_type[var_temp_lh], var_temp_lh, root->right->info->value.dval);
                                    cond_stat=0;
                                }
                                else
                                {
                                    temp_type[++var_temp]='F';
                                    fprintf(quadruple,"\tCMPF%s TEMP%c%d, #%f, TEMP%c%d\n", operation[root->type], temp_type[var_temp_lh], var_temp_lh, root->right->info->value.dval, temp_type[var_temp], var_temp);
                                }
                                break;
                            }
                            case(IDENTIFIER):
                            {
                                switch(root->right->info->type)
                                {
                                    case(CHAR):
                                    case(INT):
                                    {
                                        if(cond_stat==1)
                                        {
                                            sprintf(condition,"%s #%d, %s", operation[root->type], root->left->info->value.cval, root->right->info->value.id);
                                            cond_stat=0;
                                        }
                                        else
                                        {
                                            temp_type[++var_temp]='I';
                                            fprintf(quadruple,"\tCMPI%s #%d, %s, TEMP%c%d\n", operation[root->type], root->left->info->value.cval, root->right->info->value.id, temp_type[var_temp], var_temp);
                                        }
                                        break;
                                    }
                                    case(DOUBLE):
                                    {
                                        var_temp_lh=++var_temp;
                                        temp_type[var_temp_lh]='F';
                                        fprintf(quadruple,"\tINT2REAL #%d, , TEMP%c%d\n", root->left->info->value.cval, temp_type[var_temp_lh], var_temp_lh);
                                        if(cond_stat==1)
                                        {
                                            sprintf(condition,"%s TEMP%c%d, %s", operation[root->type], temp_type[var_temp_lh], var_temp_lh, root->right->info->value.id);
                                            cond_stat=0;
                                        }
                                        else
                                        {
                                            temp_type[++var_temp]='F';
                                            fprintf(quadruple,"\tCMPF%s TEMP%c%d, %s, TEMP%c%d\n", operation[root->type], temp_type[var_temp_lh], var_temp_lh, root->right->info->value.id, temp_type[var_temp], var_temp);
                                        }
                                        break;
                                    }
                                }
                                break;
                            }
                            default:
                            {
                                create_quadruple(root->right);
                                var_temp_rh=var_temp;
                                if(temp_type[var_temp_rh]=='I')
                                {
                                    if(cond_stat==1)
                                    {
                                        sprintf(condition,"%s #%d, TEMP%c%d", operation[root->type], root->left->info->value.cval, temp_type[var_temp_rh], var_temp_rh);
                                        cond_stat=0;
                                    }
                                    else
                                    {
                                        temp_type[++var_temp]='I';
                                        fprintf(quadruple,"\tCMPI%s #%d, TEMP%c%d, TEMP%c%d\n", operation[root->type], root->left->info->value.cval, temp_type[var_temp_rh], var_temp_rh, temp_type[var_temp], var_temp);
                                    }
                                }
                                else if(temp_type[var_temp_rh]=='F')
                                {
                                    var_temp_lh=++var_temp;
                                    temp_type[var_temp_lh]='F';
                                    fprintf(quadruple,"\tINT2REAL #%d, , TEMP%c%d\n", root->left->info->value.cval, temp_type[var_temp_lh], var_temp_lh);
                                    if(cond_stat==1)
                                    {
                                        sprintf(condition,"%s TEMP%c%d, TEMP%c%d", operation[root->type], temp_type[var_temp_lh], var_temp_lh, temp_type[var_temp_rh], var_temp_rh);
                                        cond_stat=0;
                                    }
                                    else
                                    {
                                        temp_type[++var_temp]='F';
                                        fprintf(quadruple,"\tCMPF%s TEMP%c%d, TEMP%c%d, TEMP%c%d\n", operation[root->type], temp_type[var_temp_lh], var_temp_lh, temp_type[var_temp_rh], var_temp_rh, temp_type[var_temp], var_temp);
                                    }
                                }
                                break;
                            }
                        }
                        break;
                    }
                    case(FLOATING):
                    {
                        switch(root->right->type)
                        {
                            case(UNSIGNED_NUM):
                            {
                                var_temp_rh=++var_temp;
                                temp_type[var_temp_rh]='F';
                                fprintf(quadruple,"\tINT2REAL #%d, , TEMP%c%d\n", root->right->info->value.ival, temp_type[var_temp_rh], var_temp_rh);
                                if(cond_stat==1)
                                {
                                    sprintf(condition,"%s #%f, TEMP%c%d", operation[root->type], root->left->info->value.dval, temp_type[var_temp_rh], var_temp_rh);
                                    cond_stat=0;
                                }
                                else
                                {
                                    temp_type[++var_temp]='F';
                                    fprintf(quadruple,"\tCMPF%s #%f, TEMP%c%d, TEMP%c%d\n", operation[root->type], root->left->info->value.dval, temp_type[var_temp_rh], var_temp_rh, temp_type[var_temp], var_temp);
                                }
                                break;
                            }
                            case(CHARACTER):
                            {
                                var_temp_rh=++var_temp;
                                temp_type[var_temp_rh]='F';
                                fprintf(quadruple,"\tINT2REAL #%d, , TEMP%c%d\n", root->right->info->value.cval, temp_type[var_temp_rh], var_temp_rh);
                                if(cond_stat==1)
                                {
                                    sprintf(condition,"%s #%f, TEMP%c%d", operation[root->type], root->left->info->value.dval, temp_type[var_temp_rh], var_temp_rh);
                                    cond_stat=0;
                                }
                                else
                                {
                                    temp_type[++var_temp]='F';
                                    fprintf(quadruple,"\tCMPF%s #%f, TEMP%c%d, TEMP%c%d\n", operation[root->type], root->left->info->value.dval, temp_type[var_temp_rh], var_temp_rh, temp_type[var_temp], var_temp);
                                }
                                break;
                            }
                            case(FLOATING):
                            {
                                if(cond_stat==1)
                                {
                                    sprintf(condition,"%s #%f, #%f", operation[root->type], root->left->info->value.dval, root->right->info->value.dval);
                                    cond_stat=0;
                                }
                                else
                                {
                                    temp_type[++var_temp]='F';
                                    fprintf(quadruple,"\tCMPF%s #%f, #%f, TEMP%c%d\n", operation[root->type], root->left->info->value.dval, root->right->info->value.dval, temp_type[var_temp], var_temp);
                                }
                                break;
                            }
                            case(IDENTIFIER):
                            {
                                switch(root->right->info->type)
                                {
                                    case(CHAR):
                                    case(INT):
                                    {

                                        var_temp_rh=++var_temp;
                                        temp_type[var_temp_rh]='F';
                                        fprintf(quadruple,"\tINT2REAL %s, , TEMP%c%d\n", root->right->info->value.id, temp_type[var_temp_rh], var_temp_rh);
                                        if(cond_stat==1)
                                        {
                                            sprintf(condition,"%s #%f, TEMP%c%d", operation[root->type], root->left->info->value.dval, temp_type[var_temp_rh], var_temp_rh);
                                            cond_stat=0;
                                        }
                                        else
                                        {
                                            temp_type[++var_temp]='F';
                                            fprintf(quadruple,"\tCMPF%s #%f, TEMP%c%d, TEMP%c%d\n", operation[root->type], root->left->info->value.dval, temp_type[var_temp_rh], var_temp_rh, temp_type[var_temp], var_temp);
                                        }
                                        break;
                                    }
                                    case(DOUBLE):
                                    {
                                        if(cond_stat==1)
                                        {
                                            sprintf(condition,"%s #%f, %s", operation[root->type], root->left->info->value.dval, root->right->info->value.id);
                                            cond_stat=0;
                                        }
                                        else
                                        {
                                            temp_type[++var_temp]='F';
                                            fprintf(quadruple,"\tCMPF%s #%f, %s, TEMP%c%d\n", operation[root->type], root->left->info->value.dval, root->right->info->value.id, temp_type[var_temp], var_temp);
                                        }
                                        break;
                                    }
                                }
                                break;
                            }
                            default:
                            {
                                create_quadruple(root->right);
                                var_temp_rh=var_temp;
                                if(temp_type[var_temp_rh]=='I')
                                {
                                    var_temp_lh=++var_temp;
                                    temp_type[var_temp_lh]='F';
                                    fprintf(quadruple,"\tINT2REAL TEMP%c%d, , TEMP%c%d\n", temp_type[var_temp_rh], var_temp_rh, temp_type[var_temp_lh], var_temp_lh);
                                    if(cond_stat==1)
                                    {
                                        sprintf(condition,"%s #%f, TEMP%c%d", operation[root->type], root->left->info->value.dval, temp_type[var_temp_lh], var_temp_lh);
                                        cond_stat=0;
                                    }
                                    else
                                    {
                                        temp_type[++var_temp]='F';
                                        fprintf(quadruple,"\tCMPF%s #%f, TEMP%c%d, TEMP%c%d\n", operation[root->type], root->left->info->value.dval, temp_type[var_temp_lh], var_temp_lh, temp_type[var_temp], var_temp);
                                    }
                                }
                                else if(temp_type[var_temp_rh]=='F')
                                {
                                    if(cond_stat==1)
                                    {
                                        sprintf(condition,"%s #%f, TEMP%c%d", operation[root->type], root->left->info->value.dval, temp_type[var_temp_rh], var_temp_rh);
                                        cond_stat=0;
                                    }
                                    else
                                    {
                                        temp_type[++var_temp]='F';
                                        fprintf(quadruple,"\tCMPF%s #%f, TEMP%c%d, TEMP%c%d\n", operation[root->type], root->left->info->value.dval, temp_type[var_temp_rh], var_temp_rh, temp_type[var_temp], var_temp);
                                    }
                                }
                                break;
                            }
                        }
                        break;
                    }
                    case(IDENTIFIER):
                    {
                        switch(root->right->type)
                        {
                            case(UNSIGNED_NUM):
                            {
                                switch(root->left->info->type)
                                {
                                    case(INT):
                                    case(CHAR):
                                    {
                                        if(cond_stat==1)
                                        {
                                            sprintf(condition,"%s %s, #%d", operation[root->type], root->left->info->value.id, root->right->info->value.ival);
                                            cond_stat=0;
                                        }
                                        else
                                        {
                                            temp_type[++var_temp]='I';
                                            fprintf(quadruple,"\tCMPI%s %s, #%d, TEMP%c%d\n", operation[root->type], root->left->info->value.id, root->right->info->value.ival, temp_type[var_temp], var_temp);
                                        }
                                        break;
                                    }
                                    case(DOUBLE):
                                    {
                                        var_temp_rh=++var_temp;
                                        temp_type[var_temp_rh]='F';
                                        fprintf(quadruple,"\tINT2REAL #%d, , TEMP%c%d\n", root->right->info->value.ival, temp_type[var_temp_rh], var_temp_rh);
                                        if(cond_stat==1)
                                        {
                                            sprintf(condition,"%s %s, TEMP%c%d", operation[root->type], root->left->info->value.id, temp_type[var_temp_rh], var_temp_rh);
                                            cond_stat=0;
                                        }
                                        else
                                        {
                                            temp_type[++var_temp]='F';
                                            fprintf(quadruple,"\tCMPF%s %s, TEMP%c%d, TEMP%c%d\n", operation[root->type], root->left->info->value.id, temp_type[var_temp_rh], var_temp_rh, temp_type[var_temp], var_temp);
                                        }
                                        break;
                                    }
                                }
                                break;
                            }
                            case(CHARACTER):
                            {
                                switch(root->left->info->type)
                                {
                                    case(INT):
                                    case(CHAR):
                                    {
                                        if(cond_stat==1)
                                        {
                                            sprintf(condition,"%s %s, #%d", operation[root->type], root->left->info->value.id, root->right->info->value.cval);
                                            cond_stat=0;
                                        }
                                        else
                                        {
                                            temp_type[++var_temp]='I';
                                            fprintf(quadruple,"\tCMPI%s %s, #%d, TEMP%c%d\n", operation[root->type], root->left->info->value.id, root->right->info->value.cval, temp_type[var_temp], var_temp);
                                        }
                                        break;
                                    }
                                    case(DOUBLE):
                                    {
                                        var_temp_rh=++var_temp;
                                        temp_type[var_temp_rh]='F';
                                        fprintf(quadruple,"\tINT2REAL #%d, , TEMP%c%d\n", root->right->info->value.cval, temp_type[var_temp_rh], var_temp_rh);
                                        if(cond_stat==1)
                                        {
                                            sprintf(condition,"%s %s, TEMP%c%d", operation[root->type], root->left->info->value.id, temp_type[var_temp_rh], var_temp_rh);
                                            cond_stat=0;
                                        }
                                        else
                                        {
                                            temp_type[++var_temp]='F';
                                            fprintf(quadruple,"\tCMPF%s %s, TEMP%c%d, TEMP%c%d\n", operation[root->type], root->left->info->value.id, temp_type[var_temp_rh], var_temp_rh, temp_type[var_temp], var_temp);
                                        }
                                        break;
                                    }
                                }
                                break;
                            }
                            case(FLOATING):
                            {
                                switch(root->left->info->type)
                                {
                                    case(INT):
                                    case(CHAR):
                                    {
                                        var_temp_lh=++var_temp;
                                        temp_type[var_temp_lh]='F';
                                        fprintf(quadruple,"\tINT2REAL %s, , TEMP%c%d\n", root->left->info->value.id, temp_type[var_temp_lh], var_temp_lh);
                                        if(cond_stat==1)
                                        {
                                            sprintf(condition,"%s TEMP%c%d, #%f", operation[root->type], temp_type[var_temp_lh], var_temp_lh, root->right->info->value.dval);
                                            cond_stat=0;
                                        }
                                        else
                                        {
                                            temp_type[++var_temp]='F';
                                            fprintf(quadruple,"\tCMPF%s TEMP%c%d, #%f, TEMP%c%d\n", operation[root->type], temp_type[var_temp_lh], var_temp_lh, root->right->info->value.dval, temp_type[var_temp], var_temp);
                                        }
                                        break;
                                    }
                                    case(DOUBLE):
                                    {
                                        if(cond_stat==1)
                                        {
                                            sprintf(condition,"%s %s, #%f", operation[root->type], root->left->info->value.id, root->right->info->value.dval);
                                            cond_stat=0;
                                        }
                                        else
                                        {
                                            temp_type[++var_temp]='F';
                                            fprintf(quadruple,"\tCMPF%s %s, #%f, TEMP%c%d\n", operation[root->type], root->left->info->value.id, root->right->info->value.dval, temp_type[var_temp], var_temp);
                                        }
                                        break;
                                    }
                                }
                                break;
                            }
                            case(IDENTIFIER):
                            {
                                switch(root->left->info->type)
                                {
                                    case(INT):
                                    case(CHAR):
                                    {
                                        switch(root->right->info->type)
                                        {
                                            case(INT):
                                            case(CHAR):
                                            {
                                                if(cond_stat==1)
                                                {
                                                    sprintf(condition,"%s %s, %s", operation[root->type], root->left->info->value.id, root->right->info->value.id);
                                                    cond_stat=0;
                                                }
                                                else
                                                {
                                                    temp_type[++var_temp]='I';
                                                    fprintf(quadruple,"\tCMPI%s %s, %s, TEMP%c%d\n", operation[root->type], root->left->info->value.id, root->right->info->value.id, temp_type[var_temp], var_temp);
                                                }
                                                break;
                                            }
                                            case(DOUBLE):
                                            {
                                                var_temp_lh=++var_temp;
                                                temp_type[var_temp_lh]='F';
                                                fprintf(quadruple,"\tINT2REAL %s, , TEMP%c%d\n", root->left->info->value.id, temp_type[var_temp_lh], var_temp_lh);
                                                if(cond_stat==1)
                                                {
                                                    sprintf(condition,"%s TEMP%c%d, %s", operation[root->type], temp_type[var_temp_lh], var_temp_lh, root->right->info->value.id);
                                                    cond_stat=0;
                                                }
                                                else
                                                {
                                                    temp_type[++var_temp]='F';
                                                    fprintf(quadruple,"\tCMPF%s TEMP%c%d, %s, TEMP%c%d\n", operation[root->type], temp_type[var_temp_lh], var_temp_lh, root->right->info->value.id, temp_type[var_temp], var_temp);
                                                }
                                                break;
                                            }
                                        }
                                        break;
                                    }
                                    case(DOUBLE):
                                    {
                                        switch(root->right->info->type)
                                        {
                                            case(INT):
                                            case(CHAR):
                                            {
                                                var_temp_rh=++var_temp;
                                                temp_type[var_temp_rh]='F';
                                                fprintf(quadruple,"\tINT2REAL %s, , TEMP%c%d\n", root->right->info->value.id, temp_type[var_temp_rh], var_temp_rh);
                                                if(cond_stat==1)
                                                {
                                                    sprintf(condition,"%s %s, TEMP%c%d", operation[root->type], root->left->info->value.id, temp_type[var_temp_rh], var_temp_rh);
                                                    cond_stat=0;
                                                }
                                                else
                                                {
                                                    temp_type[++var_temp]='F';
                                                    fprintf(quadruple,"\tCMPF%s %s, TEMP%c%d, TEMP%c%d\n", operation[root->type], root->left->info->value.id, temp_type[var_temp_rh], var_temp_rh, temp_type[var_temp], var_temp);
                                                }
                                                break;
                                            }
                                            case(DOUBLE):
                                            {
                                                if(cond_stat==1)
                                                {
                                                    sprintf(condition,"%s %s, %s", operation[root->type], root->left->info->value.id, root->right->info->value.id);
                                                    cond_stat=0;
                                                }
                                                else
                                                {
                                                    temp_type[++var_temp]='F';
                                                    fprintf(quadruple,"\tCMPF%s %s, %s, TEMP%c%d\n", operation[root->type], root->left->info->value.id, root->right->info->value.id, temp_type[var_temp], var_temp);
                                                }
                                                break;
                                            }
                                        }
                                        break;
                                    }
                                }
                                break;
                            }
                            default:
                            {
                                create_quadruple(root->right);
                                var_temp_rh=var_temp;
                                if(temp_type[var_temp_rh]=='I')
                                {
                                    switch(root->left->info->type)
                                    {
                                        case(CHAR):
                                        case(INT):
                                        {

                                            if(cond_stat==1)
                                            {
                                                sprintf(condition,"%s %s, TEMP%c%d", operation[root->type], root->left->info->value.id, temp_type[var_temp_rh], var_temp_rh);
                                                cond_stat=0;
                                            }
                                            else
                                            {
                                                temp_type[++var_temp]='I';
                                                fprintf(quadruple,"\tCMPI%s %s, TEMP%c%d, TEMP%c%d\n", operation[root->type], root->left->info->value.id, temp_type[var_temp_rh], var_temp_rh, temp_type[var_temp], var_temp);
                                            }
                                            break;
                                        }
                                        case(DOUBLE):
                                        {
                                            var_temp_lh=++var_temp;
                                            temp_type[var_temp_lh]='F';
                                            fprintf(quadruple,"\tINT2REAL TEMP%c%d, , TEMP%c%d\n", temp_type[var_temp_rh], var_temp_rh, temp_type[var_temp_lh], var_temp_lh);
                                            if(cond_stat==1)
                                            {
                                                sprintf(condition,"%s %s, TEMP%c%d", operation[root->type], root->left->info->value.id, temp_type[var_temp_lh], var_temp_lh);
                                                cond_stat=0;
                                            }
                                            else
                                            {
                                                temp_type[++var_temp]='F';
                                                fprintf(quadruple,"\tCMPF%s %s, TEMP%c%d, TEMP%c%d\n", operation[root->type], root->left->info->value.id, temp_type[var_temp_lh], var_temp_lh, temp_type[var_temp], var_temp);
                                            }
                                            break;
                                        }
                                    }
                                }
                                else if(temp_type[var_temp_rh]=='F')
                                {
                                    switch(root->left->info->type)
                                    {
                                        case(CHAR):
                                        case(INT):
                                        {
                                            var_temp_lh=++var_temp;
                                            temp_type[var_temp_lh]='F';
                                            fprintf(quadruple,"\tINT2REAL %s, , TEMP%c%d\n", root->left->info->value.id, temp_type[var_temp_lh], var_temp_lh);
                                            if(cond_stat==1)
                                            {
                                                sprintf(condition,"%s TEMP%c%d, TEMP%c%d", operation[root->type], temp_type[var_temp_lh], var_temp_lh, temp_type[var_temp_rh], var_temp_rh);
                                                cond_stat=0;
                                            }
                                            else
                                            {
                                                temp_type[++var_temp]='F';
                                                fprintf(quadruple,"\tCMPF%s TEMP%c%d, TEMP%c%d, TEMP%c%d\n", operation[root->type], temp_type[var_temp_lh], var_temp_lh, temp_type[var_temp_rh], var_temp_rh, temp_type[var_temp], var_temp);
                                            }
                                            break;
                                        }
                                        case(DOUBLE):
                                        {
                                            if(cond_stat==1)
                                            {
                                                sprintf(condition,"%s %s, TEMP%c%d", operation[root->type], root->left->info->value.id, temp_type[var_temp_rh], var_temp_rh);
                                                cond_stat=0;
                                            }
                                            else
                                            {
                                                temp_type[++var_temp]='F';
                                                fprintf(quadruple,"\tCMPF%s %s, TEMP%c%d, TEMP%c%d\n", operation[root->type], root->left->info->value.id, temp_type[var_temp_rh], var_temp_rh, temp_type[var_temp], var_temp);
                                            }
                                            break;
                                        }
                                    }
                                }
                                break;
                            }
                        }
                        break;
                    }
                    default:
                    {
                        var_temp_lh=var_temp;
                        switch(root->right->type)
                        {
                            case(UNSIGNED_NUM):
                            {
                                if(temp_type[var_temp_lh]=='I')
                                {
                                    if(cond_stat==1)
                                    {
                                        sprintf(condition,"%s TEMP%c%d, #%d", operation[root->type], temp_type[var_temp_lh], var_temp_lh, root->right->info->value.ival);
                                        cond_stat=0;
                                    }
                                    else
                                    {
                                        temp_type[++var_temp]='I';
                                        fprintf(quadruple,"\tCMPI%s TEMP%c%d, #%d, TEMP%c%d\n", operation[root->type], temp_type[var_temp_lh], var_temp_lh, root->right->info->value.ival, temp_type[var_temp], var_temp);
                                    }
                                }
                                else if(temp_type[var_temp_lh]=='F')
                                {
                                    var_temp_rh=++var_temp;
                                    temp_type[var_temp_rh]='F';
                                    fprintf(quadruple,"\tINT2REAL #%d, , TEMP%c%d\n", root->right->info->value.ival, temp_type[var_temp_rh], var_temp_rh);
                                    if(cond_stat==1)
                                    {
                                        sprintf(condition,"%s TEMP%c%d, TEMP%c%d", operation[root->type], temp_type[var_temp_lh], var_temp_lh, temp_type[var_temp_rh], var_temp_rh);
                                        cond_stat=0;
                                    }
                                    else
                                    {
                                        temp_type[++var_temp]='F';
                                        fprintf(quadruple,"\tCMPF%s TEMP%c%d, TEMP%c%d, TEMP%c%d\n", operation[root->type], temp_type[var_temp_lh], var_temp_lh, temp_type[var_temp_rh], var_temp_rh, temp_type[var_temp], var_temp);
                                    }
                                }
                                break;
                            }
                            case(CHARACTER):
                            {
                                if(temp_type[var_temp_lh]=='I')
                                {
                                    if(cond_stat==1)
                                    {
                                        sprintf(condition,"%s TEMP%c%d, #%d", operation[root->type], temp_type[var_temp_lh], var_temp_lh, root->right->info->value.cval);
                                        cond_stat=0;
                                    }
                                    else
                                    {
                                        temp_type[++var_temp]='I';
                                        fprintf(quadruple,"\tCMPI%s TEMP%c%d, #%d, TEMP%c%d\n", operation[root->type], temp_type[var_temp_lh], var_temp_lh, root->right->info->value.cval, temp_type[var_temp], var_temp);
                                    }
                                }
                                else if(temp_type[var_temp_lh]=='F')
                                {
                                    var_temp_rh=++var_temp;
                                    temp_type[var_temp_rh]='F';
                                    fprintf(quadruple,"\tINT2REAL #%d, , TEMP%c%d\n", root->right->info->value.cval, temp_type[var_temp_rh], var_temp_rh);
                                    if(cond_stat==1)
                                    {
                                        sprintf(condition,"%s TEMP%c%d, TEMP%c%d", operation[root->type], temp_type[var_temp_lh], var_temp_lh, temp_type[var_temp_rh], var_temp_rh);
                                        cond_stat=0;
                                    }
                                    else
                                    {
                                        temp_type[++var_temp]='F';
                                        fprintf(quadruple,"\tCMPF%s TEMP%c%d, TEMP%c%d, TEMP%c%d\n", operation[root->type], temp_type[var_temp_lh], var_temp_lh, temp_type[var_temp_rh], var_temp_rh, temp_type[var_temp], var_temp);
                                    }
                                }
                                break;
                            }
                            case(FLOATING):
                            {
                                if(temp_type[var_temp_lh]=='I')
                                {
                                    var_temp_rh=++var_temp;
                                    temp_type[var_temp_rh]='F';
                                    fprintf(quadruple,"\tINT2REAL TEMP%c%d, , TEMP%c%d\n", temp_type[var_temp_lh], var_temp_lh, temp_type[var_temp_rh], var_temp_rh);
                                    if(cond_stat==1)
                                    {
                                        sprintf(condition,"%s TEMP%c%d, #%f", operation[root->type], temp_type[var_temp_rh], var_temp_rh, root->right->info->value.dval);
                                        cond_stat=0;
                                    }
                                    else
                                    {
                                        temp_type[++var_temp]='F';
                                        fprintf(quadruple,"\tCMPF%s TEMP%c%d, #%f, TEMP%c%d\n", operation[root->type], temp_type[var_temp_rh], var_temp_rh, root->right->info->value.dval, temp_type[var_temp], var_temp);
                                    }
                                }
                                else if(temp_type[var_temp_lh]=='F')
                                {
                                    if(cond_stat==1)
                                    {
                                        sprintf(condition,"%s TEMP%c%d, #%f", operation[root->type], temp_type[var_temp_lh], var_temp_lh, root->right->info->value.dval);
                                        cond_stat=0;
                                    }
                                    else
                                    {
                                        temp_type[++var_temp]='F';
                                        fprintf(quadruple,"\tCMPF%s TEMP%c%d, #%f, TEMP%c%d\n", operation[root->type], temp_type[var_temp_lh], var_temp_lh, root->right->info->value.dval, temp_type[var_temp], var_temp);
                                    }
                                }
                                break;
                            }
                            case(IDENTIFIER):
                            {
                                switch(root->right->info->type)
                                {
                                    case(CHAR):
                                    case(INT):
                                    {
                                        if(temp_type[var_temp_lh]=='I')
                                        {
                                            if(cond_stat==1)
                                            {
                                                sprintf(condition,"%s TEMP%c%d, %s", operation[root->type], temp_type[var_temp_lh], var_temp_lh, root->right->info->value.id);
                                                cond_stat=0;
                                            }
                                            else
                                            {
                                                temp_type[++var_temp]='I';
                                                fprintf(quadruple,"\tCMPI%s TEMP%c%d, %s, TEMP%c%d\n", operation[root->type], temp_type[var_temp_lh], var_temp_lh, root->right->info->value.id, temp_type[var_temp], var_temp);
                                            }
                                        }
                                        else if(temp_type[var_temp_lh]=='F')
                                        {
                                            var_temp_rh=++var_temp;
                                            temp_type[var_temp_rh]='F';
                                            fprintf(quadruple,"\tINT2REAL TEMP%c%d, , TEMP%c%d\n", temp_type[var_temp_lh], var_temp_lh, temp_type[var_temp_rh], var_temp_rh);
                                            if(cond_stat==1)
                                            {
                                                sprintf(condition,"%s TEMP%c%d, %s", operation[root->type], temp_type[var_temp_rh], var_temp_rh, root->right->info->value.id);
                                                cond_stat=0;
                                            }
                                            else
                                            {
                                                temp_type[++var_temp]='F';
                                                fprintf(quadruple,"\tCMPF%s TEMP%c%d, %s, TEMP%c%d\n", operation[root->type], temp_type[var_temp_rh], var_temp_rh, root->right->info->value.id, temp_type[var_temp], var_temp);
                                            }
                                        }
                                        break;
                                    }
                                    case(DOUBLE):
                                    {
                                        if(temp_type[var_temp_lh]=='I')
                                        {
                                            var_temp_rh=++var_temp;
                                            temp_type[var_temp_rh]='F';
                                            fprintf(quadruple,"\tINT2REAL TEMP%c%d, , TEMP%c%d\n", temp_type[var_temp_lh], var_temp_lh, temp_type[var_temp_rh], var_temp_rh);
                                            if(cond_stat==1)
                                            {
                                                sprintf(condition,"%s TEMP%c%d, %s", operation[root->type], temp_type[var_temp_rh], var_temp_rh, root->right->info->value.id);
                                                cond_stat=0;
                                            }
                                            else
                                            {
                                                temp_type[++var_temp]='F';
                                                fprintf(quadruple,"\tCMPF%s TEMP%c%d, %s, TEMP%c%d\n", operation[root->type], temp_type[var_temp_rh], var_temp_rh, root->right->info->value.id, temp_type[var_temp], var_temp);
                                            }
                                        }
                                        else if(temp_type[var_temp_lh]=='F')
                                        {
                                            if(cond_stat==1)
                                            {
                                                sprintf(condition,"%s TEMP%c%d, %s", operation[root->type], temp_type[var_temp_lh], var_temp_lh, root->right->info->value.id);
                                                cond_stat=0;
                                            }
                                            else
                                            {
                                                temp_type[++var_temp]='F';
                                                fprintf(quadruple,"\tCMPF%s TEMP%c%d, %s, TEMP%c%d\n", operation[root->type], temp_type[var_temp_lh], var_temp_lh, root->right->info->value.id, temp_type[var_temp], var_temp);
                                            }
                                        }
                                        break;
                                    }
                                }
                                break;
                            }
                            default:
                            {
                                var_temp_lh_lh=var_temp;
                                create_quadruple(root->right);
                                var_temp_rh=var_temp;
                                if(temp_type[var_temp_lh_lh]=='I')
                                {
                                    if(temp_type[var_temp_rh]=='I')
                                    {
                                        if(cond_stat==1)
                                        {
                                            sprintf(condition,"%s TEMP%c%d, TEMP%c%d", operation[root->type], temp_type[var_temp_lh_lh], var_temp_lh_lh, temp_type[var_temp_rh], var_temp_rh);
                                            cond_stat=0;
                                        }
                                        else
                                        {
                                            temp_type[++var_temp]='I';
                                            fprintf(quadruple,"\tCMPF%s TEMP%c%d, TEMP%c%d, TEMP%c%d\n", operation[root->type], temp_type[var_temp_lh_lh], var_temp_lh_lh, temp_type[var_temp_rh], var_temp_rh, temp_type[var_temp], var_temp);
                                        }
                                    }
                                    else if(temp_type[var_temp_rh]=='F')
                                    {
                                        var_temp_lh=++var_temp;
                                        temp_type[var_temp_lh]='F';
                                        fprintf(quadruple,"\tINT2REAL TEMP%c%d, , TEMP%c%d\n", temp_type[var_temp_lh_lh], var_temp_lh_lh, temp_type[var_temp_lh], var_temp_lh);
                                        if(cond_stat==1)
                                        {
                                            sprintf(condition,"%s TEMP%c%d, TEMP%c%d", operation[root->type], temp_type[var_temp_lh], var_temp_lh, temp_type[var_temp_rh], var_temp_rh);
                                            cond_stat=0;
                                        }
                                        else
                                        {
                                            temp_type[++var_temp]='F';
                                            fprintf(quadruple,"\tCMPF%s TEMP%c%d, TEMP%c%d, TEMP%c%d\n", operation[root->type], temp_type[var_temp_lh], var_temp_lh, temp_type[var_temp_rh], var_temp_rh, temp_type[var_temp], var_temp);
                                        }
                                    }
                                }
                                else if(temp_type[var_temp_lh_lh]=='F')
                                {
                                    if(temp_type[var_temp_rh]=='I')
                                    {
                                        var_temp_lh=++var_temp;
                                        temp_type[var_temp_lh]='F';
                                        fprintf(quadruple,"\tINT2REAL TEMP%c%d, , TEMP%c%d\n", temp_type[var_temp_rh], var_temp_rh, temp_type[var_temp_lh], var_temp_lh);
                                        if(cond_stat==1)
                                        {
                                            sprintf(condition,"%s TEMP%c%d, TEMP%c%d", operation[root->type], temp_type[var_temp_lh_lh], var_temp_lh_lh, temp_type[var_temp_lh], var_temp_lh);
                                            cond_stat=0;
                                        }
                                        else
                                        {
                                            temp_type[++var_temp]='F';
                                            fprintf(quadruple,"\tCMPF%s TEMP%c%d, TEMP%c%d, TEMP%c%d\n", operation[root->type], temp_type[var_temp_lh_lh], var_temp_lh_lh, temp_type[var_temp_lh], var_temp_lh, temp_type[var_temp], var_temp);
                                        }
                                    }
                                    else if(temp_type[var_temp_rh]=='F')
                                    {
                                        if(cond_stat==1)
                                        {
                                            sprintf(condition,"%s TEMP%c%d, TEMP%c%d", operation[root->type], temp_type[var_temp_lh_lh], var_temp_lh_lh, temp_type[var_temp_rh], var_temp_rh);
                                            cond_stat=0;
                                        }
                                        else
                                        {
                                            temp_type[++var_temp]='F';
                                            fprintf(quadruple,"\tCMPF%s TEMP%c%d, TEMP%c%d, TEMP%c%d\n", operation[root->type], temp_type[var_temp_lh_lh], var_temp_lh_lh, temp_type[var_temp_rh], var_temp_rh, temp_type[var_temp], var_temp);
                                        }
                                    }
                                }
                                break;
                            }
                        }
                        break;
                    }
                }
                break;
            }
            case AND_OP:
            case OR_OP:
            {
                switch(root->left->type)
                {
                    case(UNSIGNED_NUM):
                    {
                        switch(root->right->type)
                        {
                            case(UNSIGNED_NUM):
                            {
                                var_temp_lh=++var_temp;
                                var_temp_rh=++var_temp;
                                temp_type[++var_temp]=temp_type[var_temp_lh]=temp_type[var_temp_rh]='I';
                                fprintf(quadruple,"\tCMPNZ #%d, , TEMPI%d\n", root->left->info->value.ival, var_temp_lh);
                                fprintf(quadruple,"\tCMPNZ #%d, , TEMPI%d\n", root->right->info->value.ival, var_temp_rh);
                                break;
                            }
                            case(IDENTIFIER):
                            {
                                var_temp_lh=++var_temp;
                                var_temp_rh=++var_temp;
                                temp_type[++var_temp]=temp_type[var_temp_lh]=temp_type[var_temp_rh]='I';
                                fprintf(quadruple,"\tCMPNZ #%d, , TEMPI%d\n", root->left->info->value.ival, var_temp_lh);
                                fprintf(quadruple,"\tCMPNZ %s, , TEMPI%d\n", root->right->info->value.id, var_temp_rh);
                                break;
                            }
                            case(FLOATING):
                            {
                                var_temp_lh=++var_temp;
                                var_temp_rh=++var_temp;
                                temp_type[++var_temp]=temp_type[var_temp_lh]=temp_type[var_temp_rh]='I';
                                fprintf(quadruple,"\tCMPNZ #%d, , TEMPI%d\n", root->left->info->value.ival, var_temp_lh);
                                fprintf(quadruple,"\tCMPNZ #%f, , TEMPI%d\n", root->right->info->value.dval, var_temp_rh);
                                break;
                            }
                            case(CHARACTER):
                            {
                                var_temp_lh=++var_temp;
                                var_temp_rh=++var_temp;
                                temp_type[++var_temp]=temp_type[var_temp_lh]=temp_type[var_temp_rh]='I';
                                fprintf(quadruple,"\tCMPNZ #%d, , TEMPI%d\n", root->left->info->value.ival, var_temp_lh);
                                fprintf(quadruple,"\tCMPNZ #%c, , TEMPI%d\n", root->right->info->value.cval, var_temp_rh);
                                break;
                            }
                            default:
                            {
                                create_quadruple(root->right);
                                var_temp_lh_lh=var_temp;
                                var_temp_lh=++var_temp;
                                var_temp_rh=++var_temp;
                                temp_type[++var_temp]=temp_type[var_temp_lh]=temp_type[var_temp_rh]='I';
                                fprintf(quadruple,"\tCMPNZ #%d, , TEMPI%d\n", root->left->info->value.ival, var_temp_lh);
                                fprintf(quadruple,"\tCMPNZ TEMP%c%d, , TEMPI%d\n", temp_type[var_temp_lh_lh], var_temp_lh_lh, var_temp_rh);
                                break;
                            }
                        }
                        fprintf(quadruple,"\t%s TEMPI%d, TEMPI%d, TEMPI%d\n", operation[root->type], var_temp_lh, var_temp_rh, var_temp);
                        break;
                    }
                    case(CHARACTER):
                    {
                        switch(root->right->type)
                        {
                            case(UNSIGNED_NUM):
                            {
                                var_temp_lh=++var_temp;
                                var_temp_rh=++var_temp;
                                temp_type[++var_temp]=temp_type[var_temp_lh]=temp_type[var_temp_rh]='I';
                                fprintf(quadruple,"\tCMPNZ #%d, , TEMPI%d\n", root->left->info->value.cval, var_temp_lh);
                                fprintf(quadruple,"\tCMPNZ #%d, , TEMPI%d\n", root->right->info->value.ival, var_temp_rh);
                                break;
                            }
                            case(IDENTIFIER):
                            {
                                var_temp_lh=++var_temp;
                                var_temp_rh=++var_temp;
                                temp_type[++var_temp]=temp_type[var_temp_lh]=temp_type[var_temp_rh]='I';
                                fprintf(quadruple,"\tCMPNZ #%d, , TEMPI%d\n", root->left->info->value.cval, var_temp_lh);
                                fprintf(quadruple,"\tCMPNZ %s, , TEMPI%d\n", root->right->info->value.id, var_temp_rh);
                                break;
                            }
                            case(FLOATING):
                            {
                                var_temp_lh=++var_temp;
                                var_temp_rh=++var_temp;
                                temp_type[++var_temp]=temp_type[var_temp_lh]=temp_type[var_temp_rh]='I';
                                fprintf(quadruple,"\tCMPNZ #%d, , TEMPI%d\n", root->left->info->value.cval, var_temp_lh);
                                fprintf(quadruple,"\tCMPNZ #%f, , TEMPI%d\n", root->right->info->value.dval, var_temp_rh);
                                break;
                            }
                            case(CHARACTER):
                            {
                                var_temp_lh=++var_temp;
                                var_temp_rh=++var_temp;
                                temp_type[++var_temp]=temp_type[var_temp_lh]=temp_type[var_temp_rh]='I';
                                fprintf(quadruple,"\tCMPNZ #%d, , TEMPI%d\n", root->left->info->value.cval, var_temp_lh);
                                fprintf(quadruple,"\tCMPNZ #%c, , TEMPI%d\n", root->right->info->value.cval, var_temp_rh);
                                break;
                            }
                            default:
                            {
                                create_quadruple(root->right);
                                var_temp_lh_lh=var_temp;
                                var_temp_lh=++var_temp;
                                var_temp_rh=++var_temp;
                                temp_type[++var_temp]=temp_type[var_temp_lh]=temp_type[var_temp_rh]='I';
                                fprintf(quadruple,"\tCMPNZ #%d, , TEMPI%d\n", root->left->info->value.cval, var_temp_lh);
                                fprintf(quadruple,"\tCMPNZ TEMP%c%d, , TEMPI%d\n", temp_type[var_temp_lh_lh], var_temp_lh_lh, var_temp_rh);
                                break;
                            }
                        }
                        fprintf(quadruple,"\t%s TEMPI%d, TEMPI%d, TEMPI%d\n", operation[root->type], var_temp_lh, var_temp_rh, var_temp);
                        break;
                    }
                    case(FLOATING):
                    {
                        switch(root->right->type)
                        {
                            case(UNSIGNED_NUM):
                            {
                                var_temp_lh=++var_temp;
                                var_temp_rh=++var_temp;
                                temp_type[++var_temp]=temp_type[var_temp_lh]=temp_type[var_temp_rh]='I';
                                fprintf(quadruple,"\tCMPNZ #%f, , TEMPI%d\n", root->left->info->value.dval, var_temp_lh);
                                fprintf(quadruple,"\tCMPNZ #%d, , TEMPI%d\n", root->right->info->value.ival, var_temp_rh);
                                break;
                            }
                            case(IDENTIFIER):
                            {
                                var_temp_lh=++var_temp;
                                var_temp_rh=++var_temp;
                                temp_type[++var_temp]=temp_type[var_temp_lh]=temp_type[var_temp_rh]='I';
                                fprintf(quadruple,"\tCMPNZ #%f, , TEMPI%d\n", root->left->info->value.dval, var_temp_lh);
                                fprintf(quadruple,"\tCMPNZ %s, , TEMPI%d\n", root->right->info->value.id, var_temp_rh);
                                break;
                            }
                            case(FLOATING):
                            {
                                var_temp_lh=++var_temp;
                                var_temp_rh=++var_temp;
                                temp_type[++var_temp]=temp_type[var_temp_lh]=temp_type[var_temp_rh]='I';
                                fprintf(quadruple,"\tCMPNZ #%f, , TEMPI%d\n", root->left->info->value.dval, var_temp_lh);
                                fprintf(quadruple,"\tCMPNZ #%f, , TEMPI%d\n", root->right->info->value.dval, var_temp_rh);
                                break;
                            }
                            case(CHARACTER):
                            {
                                var_temp_lh=++var_temp;
                                var_temp_rh=++var_temp;
                                temp_type[++var_temp]=temp_type[var_temp_lh]=temp_type[var_temp_rh]='I';
                                fprintf(quadruple,"\tCMPNZ #%f, , TEMPI%d\n", root->left->info->value.dval, var_temp_lh);
                                fprintf(quadruple,"\tCMPNZ #%c, , TEMPI%d\n", root->right->info->value.cval, var_temp_rh);
                                break;
                            }
                            default:
                            {
                                create_quadruple(root->right);
                                var_temp_lh_lh=var_temp;
                                var_temp_lh=++var_temp;
                                var_temp_rh=++var_temp;
                                temp_type[++var_temp]=temp_type[var_temp_lh]=temp_type[var_temp_rh]='I';
                                fprintf(quadruple,"\tCMPNZ #%f, , TEMPI%d\n", root->left->info->value.dval, var_temp_lh);
                                fprintf(quadruple,"\tCMPNZ TEMP%c%d, , TEMPI%d\n", temp_type[var_temp_lh_lh], var_temp_lh_lh, var_temp_rh);
                                break;
                            }
                        }
                        fprintf(quadruple,"\t%s TEMPI%d, TEMPI%d, TEMPI%d\n", operation[root->type], var_temp_lh, var_temp_rh, var_temp);
                        break;
                    }
                    case(IDENTIFIER):
                    {
                        switch(root->right->type)
                        {
                            case(UNSIGNED_NUM):
                            {
                                var_temp_lh=++var_temp;
                                var_temp_rh=++var_temp;
                                temp_type[++var_temp]=temp_type[var_temp_lh]=temp_type[var_temp_rh]='I';
                                fprintf(quadruple,"\tCMPNZ %s, , TEMPI%d\n", root->left->info->value.id, var_temp_lh);
                                fprintf(quadruple,"\tCMPNZ #%d, , TEMPI%d\n", root->right->info->value.ival, var_temp_rh);
                                break;
                            }
                            case(IDENTIFIER):
                            {
                                var_temp_lh=++var_temp;
                                var_temp_rh=++var_temp;
                                temp_type[++var_temp]=temp_type[var_temp_lh]=temp_type[var_temp_rh]='I';
                                fprintf(quadruple,"\tCMPNZ %s, , TEMPI%d\n", root->left->info->value.id, var_temp_lh);
                                fprintf(quadruple,"\tCMPNZ %s, , TEMPI%d\n", root->right->info->value.id, var_temp_rh);
                                break;
                            }
                            case(FLOATING):
                            {
                                var_temp_lh=++var_temp;
                                var_temp_rh=++var_temp;
                                temp_type[++var_temp]=temp_type[var_temp_lh]=temp_type[var_temp_rh]='I';
                                fprintf(quadruple,"\tCMPNZ %s, , TEMPI%d\n", root->left->info->value.id, var_temp_lh);
                                fprintf(quadruple,"\tCMPNZ #%f, , TEMPI%d\n", root->right->info->value.dval, var_temp_rh);
                                break;
                            }
                            case(CHARACTER):
                            {
                                var_temp_lh=++var_temp;
                                var_temp_rh=++var_temp;
                                temp_type[++var_temp]=temp_type[var_temp_lh]=temp_type[var_temp_rh]='I';
                                fprintf(quadruple,"\tCMPNZ %s, , TEMPI%d\n", root->left->info->value.id, var_temp_lh);
                                fprintf(quadruple,"\tCMPNZ #%c, , TEMPI%d\n", root->right->info->value.cval, var_temp_rh);
                                break;
                            }
                            default:
                            {
                                create_quadruple(root->right);
                                var_temp_lh_lh=var_temp;
                                var_temp_lh=++var_temp;
                                var_temp_rh=++var_temp;
                                temp_type[++var_temp]=temp_type[var_temp_lh]=temp_type[var_temp_rh]='I';
                                fprintf(quadruple,"\tCMPNZ %s, , TEMPI%d\n", root->left->info->value.id, var_temp_lh);
                                fprintf(quadruple,"\tCMPNZ TEMP%c%d, , TEMPI%d\n", temp_type[var_temp_lh_lh], var_temp_lh_lh, var_temp_rh);
                                break;
                            }
                        }
                        fprintf(quadruple,"\t%s TEMPI%d, TEMPI%d, TEMPI%d\n", operation[root->type], var_temp_lh, var_temp_rh, var_temp);
                        break;
                    }
                    default:
                    {
                        switch(root->right->type)
                        {
                            case(UNSIGNED_NUM):
                            {
                                var_temp_lh_lh=var_temp;
                                var_temp_lh=++var_temp;
                                var_temp_rh=++var_temp;
                                temp_type[++var_temp]=temp_type[var_temp_lh]=temp_type[var_temp_rh]='I';
                                fprintf(quadruple,"\tCMPNZ TEMP%c%d, , TEMPI%d\n", temp_type[var_temp_lh_lh],var_temp_lh_lh, var_temp_lh);
                                fprintf(quadruple,"\tCMPNZ #%d, , TEMPI%d\n", root->right->info->value.ival, var_temp_rh);
                                break;
                            }
                            case(IDENTIFIER):
                            {
                                var_temp_lh_lh=var_temp;
                                var_temp_lh=++var_temp;
                                var_temp_rh=++var_temp;
                                temp_type[++var_temp]=temp_type[var_temp_lh]=temp_type[var_temp_rh]='I';
                                fprintf(quadruple,"\tCMPNZ TEMP%c%d, , TEMPI%d\n", temp_type[var_temp_lh_lh],var_temp_lh_lh, var_temp_lh);
                                fprintf(quadruple,"\tCMPNZ %s, , TEMPI%d\n", root->right->info->value.id, var_temp_rh);
                                break;
                            }
                            case(FLOATING):
                            {
                                var_temp_lh_lh=var_temp;
                                var_temp_lh=++var_temp;
                                var_temp_rh=++var_temp;
                                temp_type[++var_temp]=temp_type[var_temp_lh]=temp_type[var_temp_rh]='I';
                                fprintf(quadruple,"\tCMPNZ TEMP%c%d, , TEMPI%d\n", temp_type[var_temp_lh_lh],var_temp_lh_lh, var_temp_lh);
                                fprintf(quadruple,"\tCMPNZ #%f, , TEMPI%d\n", root->right->info->value.dval, var_temp_rh);
                                break;
                            }
                            case(CHARACTER):
                            {
                                var_temp_lh_lh=var_temp;
                                var_temp_lh=++var_temp;
                                var_temp_rh=++var_temp;
                                temp_type[++var_temp]=temp_type[var_temp_lh]=temp_type[var_temp_rh]='I';
                                fprintf(quadruple,"\tCMPNZ TEMP%c%d, , TEMPI%d\n", temp_type[var_temp_lh_lh],var_temp_lh_lh, var_temp_lh);
                                fprintf(quadruple,"\tCMPNZ #%c, , TEMPI%d\n", root->right->info->value.cval, var_temp_rh);
                                break;
                            }
                            default:
                            {
                                var_temp_lh_lh=var_temp;
                                create_quadruple(root->right);
                                int var_temp_rh_dx=var_temp;
                                var_temp_lh=++var_temp;
                                var_temp_rh=++var_temp;
                                temp_type[++var_temp]=temp_type[var_temp_lh]=temp_type[var_temp_rh]='I';
                                fprintf(quadruple,"\tCMPNZ TEMP%c%d, , TEMPI%d\n", temp_type[var_temp_lh_lh],var_temp_lh_lh, var_temp_lh);
                                fprintf(quadruple,"\tCMPNZ TEMP%c%d, , TEMPI%d\n", temp_type[var_temp_rh_dx], var_temp_rh_dx, var_temp_rh);
                                break;
                            }
                        }
                        fprintf(quadruple,"\t%s TEMPI%d, TEMPI%d, TEMPI%d\n", operation[root->type], var_temp_lh, var_temp_rh, var_temp);
                        break;
                        }

                }
                break;
            }
            case 'i':
            {
                if(root->left!=NULL && root->left->left!=NULL && root->left->left->left==NULL)
                    cond_stat=1;
                create_quadruple(root->left);
                if(strcmp(condition," , ")==0)
                {
                    switch(root->left->type)
                    {
                        case '=':
                        {
                            fprintf(quadruple,"\tJMPNZ %s, , if_%d\n", root->left->left->info->value.id, ++cond_if);
                            break;
                        }
                        case(IDENTIFIER):
                        {
                            fprintf(quadruple,"\tJMPNZ %s, , if_%d\n", root->left->info->value.id, ++cond_if);
                            break;
                        }
                        case(UNSIGNED_NUM):
                        {
                            fprintf(quadruple,"\tJMPNZ #%d, , if_%d\n", root->left->info->value.ival, ++cond_if);
                            break;
                        }
                        case(CHARACTER):
                        {
                            fprintf(quadruple,"\tJMPNZ #%d, , if_%d\n", root->left->info->value.cval, ++cond_if);
                            break;
                        }
                        case(FLOATING):
                        {
                            fprintf(quadruple,"\tJMPNZ #%f, , if_%d\n", root->left->info->value.dval, ++cond_if);
                            break;
                        }
                        default:
                        {
                            fprintf(quadruple,"\tJMPNZ TEMP%c%d, , if_%d\n", temp_type[var_temp], var_temp, ++cond_if);
                            var_temp=0;
                            break;
                        }
                    }
                }
                else
                {
                    fprintf(quadruple,"\tJMP%s, if_%d\n", condition, ++cond_if);
                    var_temp=0;
                }
                fprintf(quadruple,"\tJMP , , end_if_%d\n", cond_if);
                fprintf(quadruple,"if_%d:\n", cond_if);
                cond_if_prec=cond_if;
                cond_stat=0;
                create_quadruple(root->right);
                fprintf(quadruple,"end_if_%d:\n", cond_if_prec--);
                break;
            }
            case 'j':
            {
                if(root->left!=NULL && root->left->left!=NULL && root->left->left->left==NULL)
                    cond_stat=1;
                create_quadruple(root->left);
                //  This condition is always tre, it is a redundant check but it is secure.
                if(root->right->type == 'e')
                {
                    if(strcmp(condition," , ")==0)
                    {
                        switch(root->left->type)
                        {
                            case '=':
                            {
                                fprintf(quadruple,"\tJMPNZ %s, , if_%d\n", root->left->left->info->value.id, ++cond_if);
                                break;
                            }
                            case(IDENTIFIER):
                            {
                                fprintf(quadruple,"\tJMPNZ %s, , if_%d\n", root->left->info->value.id, ++cond_if);
                                break;
                            }
                            case(UNSIGNED_NUM):
                            {
                                fprintf(quadruple,"\tJMPNZ #%d, , if_%d\n", root->left->info->value.ival, ++cond_if);
                                break;
                            }
                            case(CHARACTER):
                            {
                                fprintf(quadruple,"\tJMPNZ #%d, , if_%d\n", root->left->info->value.cval, ++cond_if);
                                break;
                            }
                            case(FLOATING):
                            {
                                fprintf(quadruple,"\tJMPNZ #%f, , if_%d\n", root->left->info->value.dval, ++cond_if);
                                break;
                            }
                            default:
                            {
                                fprintf(quadruple,"\tJMPNZ TEMP%c%d, , if_%d\n", temp_type[var_temp], var_temp, ++cond_if);
                                var_temp=0;
                                break;
                            }
                        }
                    }
                    else
                        fprintf(quadruple,"\tJMP%s, if_%d\n", condition, ++cond_if);
                    fprintf(quadruple,"\tJMP , ,else_if_%d\n", cond_if);
                    fprintf(quadruple,"if_%d:\n", cond_if);
                    cond_else_if=cond_if;
                    cond_stat=0;
                    create_quadruple(root->right);

                    if(if_else[cond_else_if]==0)
                    {
                        if_else[cond_else_if]++;
                        fprintf(quadruple,"end_if_%d:\n", cond_else_if);
                    }
                    else
                    {
                        for( ; cond_else_if>0; cond_else_if--)
                        {
                            if(if_else[cond_else_if]==0)
                            {
                                fprintf(quadruple,"end_if_%d:\n", cond_else_if);
                                if_else[cond_else_if]++;
                                break;
                            }
                        }
                    }
                    cond_else_if--;
                }
                break;
            }
            case 'e':
            {
                fprintf(quadruple,"\tJMP , ,end_if_%d\n", cond_else_if);
                fprintf(quadruple,"else_if_%d:\n", cond_else_if);
                create_quadruple(root->right);
                break;
            }
            case 'w':
            {
                if(root->left!=NULL && root->left->left!=NULL && root->left->left->left==NULL)
                    cond_stat=1;
                cond_stat_type[++n_cond_stat]='w';
                fprintf(quadruple,"while_%d:\n", ++cond_while);
                create_quadruple(root->left);
                if(strcmp(condition," , ")==0)
                {
                    switch(root->left->type)
                    {
                        case '=':
                        {
                            fprintf(quadruple,"\tJMPNZ %s, , stat_while_%d\n", root->left->left->info->value.id, cond_while);
                            break;
                        }
                        case(IDENTIFIER):
                        {
                            fprintf(quadruple,"\tJMPNZ %s, , stat_while_%d\n", root->left->info->value.id, cond_while);
                            break;
                        }
                        case(UNSIGNED_NUM):
                        {
                            fprintf(quadruple,"\tJMPNZ #%d, , stat_while_%d\n", root->left->info->value.ival, cond_while);
                            break;
                        }
                        case(CHARACTER):
                        {
                            fprintf(quadruple,"\tJMPNZ #%d, , stat_while_%d\n", root->left->info->value.cval, cond_while);
                            break;
                        }
                        case(FLOATING):
                        {
                            fprintf(quadruple,"\tJMPNZ #%f, , stat_while_%d\n", root->left->info->value.dval, cond_while);
                            break;
                        }
                        default:
                        {
                            fprintf(quadruple,"\tJMPNZ TEMP%c%d, , stat_while_%d\n", temp_type[var_temp], var_temp, cond_while);
                            var_temp=0;
                            break;
                        }
                    }
                }
                else
                {
                    fprintf(quadruple,"\tJMP%s, stat_while_%d\n", condition, cond_while);
                    var_temp=0;
                }
                fprintf(quadruple,"\tJMP , ,end_while_%d\n", cond_while);
                fprintf(quadruple,"stat_while_%d:\n", cond_while);
                cond_while_prec=cond_while;
                cond_stat=0;
                create_quadruple(root->right);
                fprintf(quadruple,"\tJMP , ,while_%d\n", cond_while_prec);
                fprintf(quadruple,"end_while_%d:\n", cond_while_prec--);
                break;
            }
            case 'f':
            {
                cond_stat_type[++n_cond_stat]='f';
                fprintf(quadruple,"\tJMP , ,cond_for_%d\n", ++cond_for);
                fprintf(quadruple,"for_%d:\n", cond_for);
                cond_for_prec=cond_for;
                if(root->right->type == 'g')
                    create_quadruple(root->right);
                fprintf(quadruple,"\tJMP , , for_%d\n", cond_for_prec);
                fprintf(quadruple,"end_for_%d:\n", cond_for_prec--);
                break;
            }
            case 'g':
            {
                if(root->right->type == 'h')
                    create_quadruple(root->right);
                cond_stat=0;
                break;
            }
            case 'h':
            {
                if(root->left!=NULL && root->left->left!=NULL && root->left->left->left==NULL)
                    cond_stat=1;
                fprintf(quadruple,"cond_for_%d:\n", cond_for);
                create_quadruple(root->left);
                if(strcmp(condition," , ")==0)
                {
                    if(root->left!=NULL)
                    {
                        switch(root->left->type)
                        {
                            case '=':
                            {
                                fprintf(quadruple,"\tJMPNZ %s, , stat_for_%d\n", root->left->left->info->value.id, cond_for);
                                break;
                            }
                            case(IDENTIFIER):
                            {
                                fprintf(quadruple,"\tJMPNZ %s, , stat_for_%d\n", root->left->info->value.id, cond_for);
                                break;
                            }
                            case(UNSIGNED_NUM):
                            {
                                fprintf(quadruple,"\tJMPNZ #%d, , stat_for_%d\n", root->left->info->value.ival, cond_for);
                                break;
                            }
                            case(CHARACTER):
                            {
                                fprintf(quadruple,"\tJMPNZ #%d, , stat_for_%d\n", root->left->info->value.cval, cond_for);
                                break;
                            }
                            case(FLOATING):
                            {
                                fprintf(quadruple,"\tJMPNZ #%f, , stat_for_%d\n", root->left->info->value.dval, cond_for);
                                break;
                            }
                            default:
                            {
                                fprintf(quadruple,"\tJMPNZ TEMP%c%d, , stat_for_%d\n", temp_type[var_temp], var_temp, cond_for);
                                var_temp=0;
                                break;
                            }
                        }
                    }
                    else
                        fprintf(quadruple,"\tJMP , , stat_for_%d\n", cond_for);
                }
                else
                {
                    fprintf(quadruple,"\tJMP%s, stat_for_%d\n", condition, cond_for);
                    var_temp=0;
                }
                fprintf(quadruple,"\tJMP , , end_for_%d\n", cond_for);
                fprintf(quadruple,"stat_for_%d:\n", cond_for);
                create_quadruple(root->right);
                break;
            }
            case 's':
            {
                cond_stat_type[++n_cond_stat]='s';
                cond_switch++;
                case_def=0;
                if(root->left->type == IDENTIFIER)
                {
                    sprintf(switch_expression, "JMPEQ %s,", root->left->info->value.id);
                }
                else
                {
                    sprintf(switch_expression, "JMPEQ %d,", root->left->info->value.ival);
                }
                cond_switch_prec=cond_switch;
                if(cond_case!=0)
                {
                    case_switch++;
                }
                create_quadruple(root->right);
                fprintf(quadruple, "end_switch_%d:\n", cond_switch_prec--);
                case_switch--;
                break;
            }
            case 'c':
            {
                cond_stat_type[++n_cond_stat]='c';
                if(cond_case!=0)
                {
                    if(case_break[cond_case]==0 && case_switch==0)
                    {
                        fprintf(quadruple, "\tJMP , , stat_case_%d\n", ++cond_case);
                        fprintf(quadruple, "end_case_%d:\n", cond_case-1);
                        end_case[cond_case-1]++;
                    }
                    else
                        cond_case++;
                }
                else
                    cond_case++;
                switch(root->left->type)
                {
                    case(UNSIGNED_NUM):
                    {
                            fprintf(quadruple,"\t%s %d, stat_case_%d\n", switch_expression, root->left->info->value.ival, cond_case);
                            break;
                    }
                    case(IDENTIFIER):
                    {
                            fprintf(quadruple,"\t%s %s, stat_case_%d\n", switch_expression, root->left->info->value.id, cond_case);
                            break;
                    }
                }
                fprintf(quadruple, "\tJMP , , end_case_%d\n", cond_case);
                fprintf(quadruple, "stat_case_%d:\n", cond_case);
                cond_case_prec=cond_case;
                create_quadruple(root->right);

                for( ; cond_case_prec>0; cond_case_prec--)
                {
                    if(end_case[cond_case_prec]==0)
                        if(case_break[cond_case_prec]==0 || case_def==0)
                        {
                            fprintf(quadruple, "end_case_%d:\n", cond_case_prec);
                            end_case[cond_case_prec]++;
                            break;
                        }
                }
                break;
            }
            case 'd':
            {
                case_def=1;
                create_quadruple(root->right);
                break;
            }
            case 'b':
            {
                switch(cond_stat_type[n_cond_stat])
                {
                   case 's':
                    {
                        fprintf(quadruple, "\tJMP , , end_switch_%d\n", cond_switch_prec);
                    }
                    case 'c':
                    {
                        case_break[cond_case_prec]++;
                        break;
                    }
                    case 'f':
                    {
                        fprintf(quadruple,"\tJMP , , end_for_%d\n", cond_for);
                        break;
                    }
                    case 'w':
                    {
                        fprintf(quadruple,"\tJMP , ,end_while_%d\n", cond_while);
                        break;
                    }
                }
                break;
            }
            case '\n':
            case ',':
            {
                var_temp=var_temp_lh=var_temp_rh=0;
                condition[0]=' ';
                condition[1]=',';
                condition[2]=' ';
                condition[3]='\0';
                create_quadruple(root->right);
                break;
            }
        }
    }
}
void fill_operations()
{
    condition[0]=' ';
    condition[1]=',';
    condition[2]=' ';
    condition[3]='\0';
    int i, n=256, l=5;
    operation = (char **)malloc(sizeof(char*)*n);

    for(i=0; i<128; i++)
    {
        if_else[i]=0;
        case_break[i]=0;
        end_case[i]=0;
        temp_type[i]='\0';
        cond_stat_type[i]='\0';
    }

    for(i=0; i<n; i++)
    {
        operation[i]=(char *)malloc(sizeof(char)*l);
        operation[i]="0";
    }
    operation['=']="MOV";
    operation['+']="ADD";
    operation['-']="SUB";
    operation['*']="MULT";
    operation['/']="DIV";
    operation['%']="MOD";
    operation[LEFT_OP]="SL";
    operation[RIGHT_OP]="SR";
    operation['<']="LT";
    operation['>']="GT";
    operation[LE_OP]="LTE";
    operation[GE_OP]="GTE";
    operation[EQ_OP]="E";
    operation[NE_OP]="NE";
    operation['&']="AND";
    operation['|']="OR";
    operation['^']="XOR";
    operation[AND_OP]="ANDL";
    operation[OR_OP]="ORL";
    operation[INC_OP]="ADD";
    operation[DEC_OP]="SUB";
    operation[ADD_ASSIGN]="ADD";
    operation[SUB_ASSIGN]="SUB";
    operation[DIV_ASSIGN]="DIV";
    operation[MUL_ASSIGN]="MUL";
    operation[MOD_ASSIGN]="MOD";
}

void cast_mov_id(struct record *id_lh, struct record *id_rh)
{

    switch(id_lh->type)
    {
        case(INT):
        case(CHAR):
        {
            switch(id_rh->type)
            {
                case(INT):
                case(CHAR):
                {
                    fprintf(quadruple,"\tMOVI %s, , %s\n", id_rh->value.id, id_lh->value.id);
                    break;
                }
                case(DOUBLE):
                {
                    temp_type[++var_temp]='I';
                    fprintf(quadruple,"\tREAL2INT #%s, , TEMP%c%d\n", id_rh->value.id, temp_type[var_temp], var_temp);
                    fprintf(quadruple,"\tMOVI TEMP%c%d, , %s\n", temp_type[var_temp], var_temp, id_lh->value.id);
                    break;
                }
            }
            break;
        }
        case(DOUBLE):
        {
            switch(id_rh->type)
            {
                case(INT):
                case(CHAR):
                {
                    temp_type[++var_temp]='F';
                    fprintf(quadruple,"\tINT2REAL %s, , TEMP%c%d\n", id_rh->value.id, temp_type[var_temp], var_temp);
                    fprintf(quadruple,"\tMOVF TEMP%c%d, , %s\n", temp_type[var_temp], var_temp, id_lh->value.id);
                    break;
                }
                case(DOUBLE):
                {
                    fprintf(quadruple,"\tMOVF %s, , %s\n", id_rh->value.id, id_lh->value.id);
                    break;
                }
            }
            break;
        }
    }
}


void cast_op_asg_id(char op[4], struct record *id_lh, struct record *id_rh)
{
    switch(id_lh->type)
    {
        case(CHAR):
        case(INT):
        {
            switch(id_rh->type)
            {
                case(CHAR):
                case(INT):
                {
                    fprintf(quadruple,"\t%s %s, %s, %s\n", op, id_lh->value.id, id_rh->value.id, id_lh->value.id);
                    break;
                }
                case(DOUBLE):
                {
                    temp_type[++var_temp]='I';
                    fprintf(quadruple,"\tREAL2INT %s, , TEMP%c%d\n", id_rh->value.id, temp_type[var_temp], var_temp);
                    fprintf(quadruple,"\t%sI %s, TEMP%c%d, %s\n", op, id_lh->value.id, temp_type[var_temp], var_temp, id_lh->value.id);
                    break;
                }
            }
            break;
        }
        case(DOUBLE):
        {
            switch(id_rh->type)
            {
                case(CHAR):
                case(INT):
                {
                    temp_type[++var_temp]='F';
                    fprintf(quadruple,"\tINT2REAL %s, , TEMP%c%d\n", id_rh->value.id, temp_type[var_temp], var_temp);
                    fprintf(quadruple,"\t%sF %s, TEMP%c%d, %s\n", op, id_lh->value.id, temp_type[var_temp], var_temp, id_lh->value.id);
                    break;
                }
                case(DOUBLE):
                {
                    temp_type[++var_temp]='F';
                    fprintf(quadruple,"\t%sF %s, %s, %s\n", op, id_lh->value.id, id_rh->value.id, id_lh->value.id);
                    break;
                }
            }
            break;
        }
    }
}
