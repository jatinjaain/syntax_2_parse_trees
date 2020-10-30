/*
 * Group No. - 48
 * Members:
 * 2018A7PS0181P, Bikash Jena
 * 2018A7PS0276P, Jatin Jain
 * 2018A7PS0131P, Utkarsh Dwivedi
 */

#include "structures.h"
#include "grammar_definitions.h"

void printGrammar(Grammar *);
void printTokenStream(Token *);

int main()
{
	int option;
	Grammar G;
	Token *stream;
	Parse_Tree *pt;
	FILE *f;
	printf("Option 0: Exit\n");
	printf("Option 1: Create parse tree\n");
	printf("Option 2: Traverse the parse tree to construct typeExpressionTable. Also print the type errors while traversing the parse tree and accessing the typeExpressionTable.\n");
	printf("Option 3: Print parse tree in the specified format\n");
	printf("Option 4: Print typeExpressionTable in the specified format.\n");

	while (1)
	{
		num_of_identifiers = 0;
		printf("\nEnter option: ");
		scanf("%d", &option);
		getchar();
		switch (option)
		{
		case 0:
			exit(1);
			break;
		case 1:
			//create parse trees
			G = readGrammar("grammar.txt", G);
			f = fopen("./sourcecode.txt", "r");
			stream = tokenizer(f);
			pt = createParseTree(stream, &G);
			break;
		case 2:
			//tranverse parse trees and report errors
			G = readGrammar("grammar.txt", G);
			f = fopen("./sourcecode.txt", "r");
			stream = tokenizer(f);
			pt = createParseTree(stream, &G);
			traverseParseTree(pt);
			break;
		case 3:
			//print parse trees
			G = readGrammar("grammar.txt", G);
			f = fopen("./sourcecode.txt", "r");
			stream = tokenizer(f);
			pt = createParseTree(stream, &G);
			printParseTree(pt);
			break;
		case 4:
			//print type expression table
			G = readGrammar("grammar.txt", G);
			f = fopen("./sourcecode.txt", "r");
			stream = tokenizer(f);
			pt = createParseTree(stream, &G);
			traverseParseTree(pt);
			printTypeExpressionTable();
			break;
		case 5:
			//testing the grammar
			G = readGrammar("grammar.txt", G);
			printf("Number of rules =  %d\n", G.num_of_rules);
			printGrammar(&G);
			break;
		case 6:
			G = readGrammar("grammar.txt", G);
			f = fopen("./sourcecode.txt", "r");
			stream = tokenizer(f);
			printTokenStream(stream);
			break;
		default:
			printf("Wrong option.\n");
			break;
		}
		fclose(f);
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
			printf("%s/%s", k->name, k->value);
			if (k->prev_node != NULL)
				printf("/%s/%s", k->prev_node->name, k->prev_node->value);
			printf(" \t");
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
		printf("%d/%s/%s ", tok->line_num, tok->name, tok->value);
		tok = tok->next_node;
	}

	printf("\n\nTotal number of lines read:\t%d\n\n", lines);
}