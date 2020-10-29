/*
 * Group No. - 48
 * Members:
 * 2018A7PS0181P, Bikash Jena
 * 2018A7PS0276P, Jatin Jain
 * 2018A7PS0131P, Utkarsh Dwivedi
 */

/*
 * provides tokenizeSourceCode("sourcecode.txt", tokenStream* S)
 * which converts input code to tokens at S
*/

#include "structures.h"

#define MAX_LINE_LEN 100000

bool isKeyword(char *name);

Token *tokenizer(FILE *source)
{
	Token head;
	Token *tok_ptr = &head;
	char buffer[MAX_LINE_LEN];
	char *token_name;
	char *token_value;
	int lines = 0;

	while (!feof(source))
	{
		lines++;
		fscanf(source, "%[^\n]\n", buffer);

		token_value = strtok(buffer, " \t");

		while (token_value != NULL)
		{
			tok_ptr->next_node = (Token *)malloc(sizeof(Token));
			tok_ptr = tok_ptr->next_node;
			tok_ptr->line_num = lines;
			strcpy(tok_ptr->value, token_value);

			if (isKeyword(token_value))
			{
				strcpy(tok_ptr->name, "keyword");
			}

			else if (isdigit(*token_value))
			{
				strcpy(tok_ptr->name, "constant");
			}
			else
			{
				strcpy(tok_ptr->name, "identifier");
			}

			token_value = strtok(NULL, " \t");
		}
	}

	tok_ptr->next_node = NULL;
	return head.next_node;
}

bool isKeyword(char *name)
{
	for (size_t i = 0; i < num_of_keywords; i++)
	{
		if (strcmp(keyword[i], name) == 0)
		{
			return true;
		}
	}
	return false;
}