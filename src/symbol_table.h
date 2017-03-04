#include <stdlib.h>
//#include "ast.h"
#include "parser.tab.h"

void yyerror(char *s);
void yywarning(char *s);

char *print_type(unsigned int type);

typedef struct record
{
    char *token;
    union value_union
    {
        char *id;
        char cval;
        int ival;
        double dval;
    }value;

    int type;
    int flag_unsigned;
    int flag_const;
    unsigned int bytes;
    unsigned int address;

    struct record *next;
}st_record;

void put_st(char *name, const void *val, int type, int flag_un, int flag_const, unsigned int byte, unsigned long addr);

st_record *get_st(char *name, const void *val);

void install(char *name, const void *val, int type, int flag_un, int flag_const, unsigned int byte, unsigned long addr);

st_record *context_check(const void *val);
st_record *context_check_id(const void *val);

int type_check(st_record *lh, st_record *rh);

void print_st();
void new_st();
void del_st();

/* Vector that points to all the symbol tables that we are going to define into the parser */
st_record **symbol_tables=NULL;

/* Vector level of the current symbol table */
int local_scope=-1;
int parent_scope=-1;
int max_scope=-1;
int *parent_child;

FILE *report_st=NULL;

void put_st(char *name, const void *val, int type, int flag_un, int flag_const, unsigned int byte, unsigned long addr)
{
    st_record *current;
    current = (st_record *) malloc(sizeof(st_record));

    current->token = strdup(name);

    if(strcmp(name,"Identifier")==0 )
    {
        current->value.id = (char*)val;
    }
    else
    {
        switch(type)
        {
            case INT:
				{
				    current->value.ival = *(int*)val;
					break;
				}
			case DOUBLE:
				{
					current->value.dval = *(double*)val;
					break;
				}
			case CHAR:
				{
					current->value.cval = *(char*)val;
					break;
				}
        }
    }
    current->type = type;
    current->flag_unsigned = flag_un;
    current->flag_const = flag_const;
    current->bytes = byte;
    current->address = addr;

    current->next = symbol_tables[local_scope];
    symbol_tables[local_scope] = current;
}

st_record *get_st(char *name, const void *val)
{
    st_record *current;

    current=symbol_tables[local_scope];

    while (current != NULL)
    {
        if((strcmp(current->token, "Identifier") == 0 && strcmp(name, "Identifier")==0 ))
        {
            if (strcmp(current->value.id, (char*)val) == 0)
                return current;
        }

        /* To prevent that the char is interpreted as an integer (or vice versa), it is also checked the token, f.i.: 97 = * (int *) a */
        if(strcmp(current->token, "Unsigned_Num")==0 && strcmp(name, "Unsigned_Num")==0)
            if(current->value.ival == *(int*)val)
                            return current;
        if(strcmp(current->token, "Floating")==0 && strcmp(name, "Floating")==0)
            if(current->value.dval == *(double*)val)
                            return current;
        if(strcmp(current->token, "Character")==0 && strcmp(name, "Character")==0)
            if(current->value.cval == *(char*)val)
                            return current;

       current = current->next;
    }
    return NULL;
}

void install(char *name, const void *val, int type, int flag_un, int flag_const, unsigned int byte, unsigned long addr)
{
    st_record *temp;
    temp = get_st(name, val);

    if(temp == NULL)
    {
        put_st(name, val, type, flag_un, flag_const, byte, addr);
    }
    else
    {
        if(strcmp("Identifier", name)==0)
        {
            char msg[128];
            sprintf(msg,"Semantic error: the variable \"%s\" has already been defined.\n", (char*)val);
            yyerror(msg);
        }
    }
}

void print_st()
{
    st_record *current;

    int i, j;

    for(i=max_scope; i>=0; i--)
    {
        current=symbol_tables[i];
        printf("\n\n*******************ST %d**************\n\n",i);

        while (current != NULL)
        {
            printf("Name: %s\n", current->token);
            if(strcmp(current->token,"Identifier")==0)
            {
                printf("Value: %s\n", current->value.id);
            }
            else
            {
                switch(current->type)
                {
                case INT:
                    {
                        printf("Value: %d\n", current->value.ival);
                        break;
                    }
                case DOUBLE:
                    {
                        printf("Value: %f\n", current->value.dval);
                        break;
                    }
                case CHAR:
                    {
                        printf("Value: %c\n", current->value.cval);
                        break;
                    }
                }
            }

            printf("type: %s\nUnsigned: %d\nConst: %d\nByte: %u\nAddress: %u\n\n",
                   print_type(current->type), current->flag_unsigned, current->flag_const, current->bytes, current->address);

            current = current->next;
        }
    }
}

char *print_type(unsigned int type)
{
        char *type_var;
        switch(type)
        {
            case INT:
				{
				    type_var="int";
					break;
				}
				case VOID:
				{
					type_var="void";
					break;
				}
				case DOUBLE:
				{
					type_var="double";
					break;
				}
				case CHAR:
				{
					type_var="char";
					break;
				}
        }
        return type_var;
}

void new_st()
{
    if(parent_scope==-1 && local_scope==-1)
    {
        local_scope=parent_scope=max_scope=0;
        fprintf(report_st,"\nlocal scope %d, parent scope %d e maximum scope %d", local_scope, parent_scope, max_scope);
    }
    else
    {
        parent_scope=local_scope;
        local_scope=++max_scope;
        parent_child[local_scope]=parent_scope;
        fprintf(report_st,"\nlocal scope %d, parent scope %d e maximum scope %d", local_scope, parent_scope, max_scope);
    }
}

st_record *context_check(const void *val)
{
    st_record *current;

    int i, j;
    j = parent_scope;
    i = local_scope;

    while(i>=0)
    {
        current=symbol_tables[i];

        while (current != NULL)
        {
            switch(current->type)
            {
                case INT:
                    {
                        if(current->value.ival == *(int*)val)
                            return current;
                        break;
                    }
                case DOUBLE:
                    {
                        if(current->value.dval == *(double*)val)
                            return current;
                        break;
                    }
                case CHAR:
                    {
                        if(current->value.cval == *(char*)val)
                            return current;
                        break;
                    }
            }

            current = current->next;
        }

        //local_scope=parent_scope;
        i=j;
        //parent_scope=parent_child[local_scope]
        j=parent_child[i];
    }
    return NULL;
}

st_record *context_check_id(const void *val)
{
    st_record *current;

    int i, j;
    j = parent_scope;
    i = local_scope;

    while(i>=0)
    {
        current=symbol_tables[i];

        while (current != NULL)
        {
            if(strcmp(current->token, "Identifier") == 0)
            {
                if (strcmp(current->value.id, (char*)val) == 0)
                    return current;
            }
            current = current->next;
        }
        i=j;
        j=parent_child[i];
    }

    char msg[128];

    sprintf(msg,"Semantic error: the variable \"%s\" has not been defined!\n", (char*)val);
    yyerror(msg);
    return NULL;
}

int type_check(st_record *lh, st_record *rh)
{
    if(lh==NULL && rh!=NULL)
    {
        return 1;
    }

    if(lh!=NULL && rh==NULL)
    {
        return 2;
    }

    if(lh!=NULL && rh!=NULL)
    {
        if(lh->type==rh->type)
        {
            return 0;
        }
        else
        {
            if(strcmp(lh->token,"Identifier")==0)
            {
                char msg[128];
                sprintf(msg,"Warning: the variable \"%s\" has been defined as %s but it is used as %s.\n", lh->value.id, print_type(lh->type), print_type(rh->type) );
                yywarning(msg);
                return 3;
            }
            return 4;
        }
    }
    else
    {
        return -1;
    }
}
