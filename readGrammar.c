/*
 * provides readGrammar("grammar.txt",grammar* G) 
 * that reads grammar from grammar.txt,
 * populates array of linked-list grammar G 
*/

// #include "structures.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

// Grammar_Node *readGrammar(char *, Grammar_Node[]);
// Grammar_Node *readGrammar(char *filename, Grammar_Node G[])
// {

// 	FILE *fptr = fopen(filename, "r");

// 	if ((fptr) == NULL)
// 	{
// 		printf("Error! opening file");
// 		// Program exits if the file pointer returns NULL.
// 		exit(1);
// 	}

// 	G = (Grammar_Node *)malloc(MAX_RULES_NUM * sizeof(Grammar_Node));
// 	for (int i = 0;; i++)
// 	{
// 		char str[MAX_NAME_LEN];
// 		char next_str[MAX_NAME_LEN];
// 		fscanf(fptr, "%s", &str[0]);
// 		if (strcmp(next_str, ":=") == 0)
// 		{
// 			//put str in the node
// 		}
// 		if (strcmp(str, "|") == 0)
// 		{
// 			//put G[i] in the G[i+1]
// 		}

// 		Grammar_Node *temp = G;
// 		fclose(fptr);
// 	}
// }
int isterminal(char a[])
{
	if (a[0]=='<')
	{
		return !(!(isupper(a[1])));
	}
	return 0;
}
int isnonterminal(char a[])
{
	if (a[0]=='<')
	{
		return !(isupper((int)a[1]));
	}
	return 0;
}
int iskeyword(char a[])
{
	if (a[0]!='<')
		return 1;
	else
		return 0;
}
// int main()
// {
// 	int a = isterminal("<TERMINAL>");
// 	int b =isterminal("<nonterminal>");
// 	int c =isterminal("symbol");
// 	int d =isnonterminal("<TERMINAL>");
// 	int e =isnonterminal("<nonterminal>");
// 	int f =isnonterminal("symbol");
// 	int g =iskeyword("<TERMINAL>");
// 	int h =iskeyword("<nonterminal>");
// 	int i =iskeyword("symbol");
// 	printf("%d %d %d %d %d %d %d %d %d",a,b,c,d,e,f,g,h,i);
// 	return 0;
// }