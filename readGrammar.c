/*
 * provides readGrammar("grammar.txt",grammar* G) 
 * that reads grammar from grammar.txt,
 * populates array of linked-list grammar G 
*/
#include "grammar_definitions.h"
#include "structures.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

Grammar_Node *readGrammar(char *, Grammar_Node[]);

Grammar_Node *readGrammar(char *filename, Grammar_Node G[])
{

	FILE *fptr = fopen(filename, "r");

	if ((fptr) == NULL)
	{
		printf("Error! opening file");
		// Program exits if the file pointer returns NULL.
		exit(1);
	}
	
	G = (Grammar_Node *)malloc(MAX_RULES_NUM * sizeof(Grammar_Node));
	for (int i = 0;; i++)
	{
		char str[MAX_NAME_LEN];
		char next_str[MAX_NAME_LEN];
		fscanf(fptr,"%s", &str[0]);
		if (strcmp(next_str, ":=") == 0)
		{
			//put str in the node
		}
		if (strcmp(str, "|") == 0)
		{
			//put G[i] in the G[i+1]
		}

		Grammar_Node *temp = G;
		fclose(fptr);
	}
