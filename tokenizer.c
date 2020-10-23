/*
 * provides tokenizeSourceCode("sourcecode.txt", tokenStream* S)
 * which converts input code to tokens at S
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "structures.h"

#define MAX_LINE_LEN 100000

int isKeyword(char *name);
int isConstant(char *name);

Token *tokenizer(FILE *source)
{
	Token head;
	Token *tptr = &head;
	char buffer[MAX_LINE_LEN];
	char *token_name;
	char *token_value;
	int lines = 0;

	while (source != EOF)
	{
		lines++;

		fscanf(source, "%[^\n]", buffer);
		token_value = strtok(buffer, " ");

		while ((token_value = strtok(NULL, " ")) != NULL && lines != 1)
		{
			tptr->next_node = (Token *)malloc(sizeof(Token));
			tptr = tptr->next_node;
			tptr->line_num = lines;
			strcpy(tptr->value, token_value);

			if (isKeyword(token_value))
			{
				strcpy(tptr->name, "keyword");
			}
			else if (isConstant(token_value))
			{
				strcpy(tptr->name, "constant");
			}
			else
			{
				strcpy(tptr->name, "identifier");
			}
		}
		
	}
	

	return head.next_node;
}

/* Represents each token as outputted by tokenizer
 * 
 * name: name of token
 * 
 * value: string value of token
 * 
 * line_num: tokens on same line in source code have same line number
 * 
 * next_node: pointer to next token

typedef struct Token
{
	char name[MAX_NAME_LEN];
	char value[MAX_VALUE_LEN];
	int line_num;
	struct Token *next_node;
} Token;

*/