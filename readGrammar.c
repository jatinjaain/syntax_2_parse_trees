/*
 * Group No. - 48
 * Members:
 * 2018A7PS0181P, Bikash Jena
 * 2018A7PS0276P, Jatin Jain
 * 2018A7PS0131P, Utkarsh Dwivedi
 */

/*
 * provides readGrammar("grammar.txt",grammar* G) 
 * that reads grammar from grammar.txt,
 * populates array of linked-list grammar G 
*/

#include "structures.h"

bool isterminal(char[]);
bool isnonterminal(char[]);
bool iskeyword(char[]);
void insertGrammarToken(char grammar_token[], Grammar_Node *node);

int num_of_keywords=0;

Grammar readGrammar(char *filename, Grammar grammar)
{
	FILE *file = fopen(filename, "r");
	
	int counter = 0;
	while (!feof(file))
	{
		grammar.rules[counter].num_of_nodes = 0;
		char buffer[MAX_RULE_LEN], *grammar_token;
		fscanf(file, "%[^\n]\n", buffer);

		if (strcmp(buffer, "\n")==0)
		{
			continue;
		}

		grammar_token = strtok(buffer, " ");

		insertGrammarToken(grammar_token, &grammar.rules[counter]);
		grammar_token = strtok(NULL, " ");

		Grammar_Node *temp = &grammar.rules[counter];
		while (grammar_token != NULL)
		{
			grammar_token = strtok(NULL, " ");
			if (grammar_token == NULL)
			{
				break;
			}

			temp->next_node = (Grammar_Node *)malloc(sizeof(Grammar_Node));
			insertGrammarToken(grammar_token, temp->next_node);
			grammar.rules[counter].num_of_nodes++;
			temp->next_node->prev_node = temp;
			temp = temp->next_node;
		}
		counter++;
	}
	grammar.num_of_rules = counter;
	fclose(file);
	return grammar;
}

void insertGrammarToken(char *grammar_token, Grammar_Node *node)
{
	if (isterminal(grammar_token))
	{
		grammar_token[strlen(grammar_token) - 1] = '\0';
		grammar_token++;
		strcpy(node->name, grammar_token);
	}
	else if (isnonterminal(grammar_token))
	{
		grammar_token[strlen(grammar_token) - 1] = '\0';
		grammar_token++;
		strcpy(node->name, grammar_token);
	}
	else if (iskeyword(grammar_token))
	{
		strcpy(node->name, "keyword");
	}

	strcpy(node->value, grammar_token);
}

bool isterminal(char a[])
{
	if (a[0] == '<')
	{
		return !(!(isupper(a[1])));
	}
	return false;
}

bool isnonterminal(char a[])
{
	if (a[0] == '<')
	{
		return !(isupper((int)a[1]));
	}
	return false;
}

bool iskeyword(char a[])
{
	if (a[0] != '<')
	{
		for (size_t i = 0; i < num_of_keywords; i++)
		{
			if (strcmp(keyword[i], a) == 0)
			{
				return true;
			}
		}
		strcpy(keyword[num_of_keywords++], a);
		return true;
	}
	else
		return false;
}