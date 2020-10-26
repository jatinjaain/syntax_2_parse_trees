#include "structures.h"
#include "grammar_definitions.h"

void printGrammar(Grammar *);
void printTokenStream(Token *);

int main()
{
	int option;
	Grammar G;
	while (1)
	{
		scanf("%d", &option);
		switch (option)
		{
		case 0:
			exit(1);
			break;
		case 1:
			//create parse trees
		case 2:
			//transverse parse trees
			break;
		case 3:
			//print parse trees
			break;
		case 4:
			//print type expression table
			break;
		case 5:
			//testing the grammar
			G = readGrammar("grammar.txt", G);
			printGrammar(&G);
			break;
		case 6:
			G = readGrammar("grammar.txt", G);
			FILE *f = fopen("./sourcecode.txt", "r");
			Token *stream = tokenizer(f);
			printTokenStream(stream);
			break;
		}
	}
}

void printGrammar(Grammar *G)
{
	for (int i = 0; i < G->num_of_rules; i++)
	{
		printf("%s:=\t", G->rules[i].name);
		Grammar_Node *k = (G->rules[i].next_node);
		for (int j = 0; j < G->rules[i].num_of_nodes; j++)
		{
			// printf("%s", k->name);
			//if(strcmp(G->rules[i].name,KEYWORD)) printf("/%s",G->rules[i].value);
			printf("%s/%s", k->name,k->value);
			printf("\t");
			k = k->next_node;
		}
		printf("\n");
	}
}

void printTokenStream(Token *tok)
{
	printf("\nPrinting tokenstream\n");
	int lines = 1;
	while (tok != NULL)
	{
		if (tok->line_num != lines)
		{
			printf("\n");
			lines = tok->line_num;
		}
		printf("%s/%s ",tok->name ,tok->value);
		tok = tok->next_node;
	}

	printf("\n\nTotal number of lines read:\t%d\n\n", lines);
}