/*
 * Group No. - 48
 * Members:
 * 2018A7PS0181P, Bikash Jena
 * 2018A7PS0276P, Jatin Jain
 * 2018A7PS0131P, Utkarsh Dwivedi
 */

#ifndef STRUCTURES_H
#define STRUCTURES_H

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define MAX_NAME_LEN 50				//maximum length of names of tokens, variables
#define MAX_VALUE_LEN 50			//maximum length of value of tokens
#define MAX_RULE_LEN 1000			//maximum length of a rule line
#define MAX_RULES_NUM 100			//maximum number of rules
#define MAX_CAPACITY_STACK 20 //maximum capacity of stack
#define MAX_LINES 100					//max number of lines expected
#define MAX_IDENTIFIER 1000

/*
 * defines common data-structures that will be used by individual files of program
 * 
*/

/*
 * defines common data-structures that will be used by individual files of program
 * 
*/

/* Represents variables/tokens in grammar
 *
 * node_type:
 *    2 - for single left variable in a rule,
 *    1 - for all right variables in a rule,
 *    0 - for all tokens in a rule
 * 
 * name: name of a variable or token
 * 
 * value: string value (applicable only for tokens)
 * 
 * num_of_nodes: integer number of nodes representing right variables and tokens
 * (applicable only for single left variable)
 * 
 * next_node: next node in a rule for right tokens and variables and first right variable/token for single left variable 
*/
typedef struct Grammar_Node
{
	char name[MAX_NAME_LEN];
	char value[MAX_VALUE_LEN];
	int num_of_nodes;
	struct Grammar_Node *next_node;
	struct Grammar_Node *prev_node;
} Grammar_Node;

/* Represents grammar
 *
 * num_of_rules: total number of rules in the grammar;
 * 
 * rules: array of pointers to single left variable of each rule 
*/
typedef struct
{
	int num_of_rules;
	Grammar_Node rules[MAX_RULES_NUM];
} Grammar;

/* Represents each token as outputted by tokenizer
 * 
 * name: name of token
 * 
 * value: string value of token
 * 
 * line_num: tokens on same line in source code have same line number
 * 
 * next_node: pointer to next token
*/
typedef struct Token
{
	char name[MAX_NAME_LEN];
	char value[MAX_VALUE_LEN];
	int line_num;
	struct Token *next_node;
	struct Token *prev_node;
} Token;

/*
 * Represents the parse tree as outputted by the parser
 * 
 * name: name of variable/token
 * 
 * value: value in case of token
 * 
 * number_of_children: in case of non-terminal, number of children
 * 
 * children: dynamically allocated array of children
*/
typedef struct Parse_Tree
{
	char name[MAX_NAME_LEN];
	char value[MAX_VALUE_LEN];
	int num_of_children;
	int line_num;
	int rule_num;
	int depth;
	struct Parse_Tree **children; //array of children
} Parse_Tree;


/*
 * Contains the number of dimensions of static
 * rectangular array alongwith lower and upper
 * bounds for each dimension
 */
typedef struct Range
{
	int num_of_dimensions;
	int lowest[MAX_RULE_LEN];
	int highest[MAX_RULE_LEN];
} Range;


/*
 * Type Expression struct used
 * to construct typeExpressionTable (array)
 */
typedef struct typeExpression
{
	char name[MAX_NAME_LEN];
	enum State
	{
		Primitive,
		Rarray,
		Jarray
	} type;

	char arrayAttribute[MAX_NAME_LEN];

	union Specify
	{
		char typeName[MAX_NAME_LEN];
		Range range;
	} expression;
} typeExpression;

typeExpression Table[MAX_IDENTIFIER];
extern int num_of_identifiers;


typedef struct Error
{
	int line_num;
	char statement_type[MAX_NAME_LEN];
	char operator[MAX_NAME_LEN];
	char first_lex[MAX_VALUE_LEN];
	char first_type[MAX_NAME_LEN];
	char second_lex[MAX_VALUE_LEN];
	char second_type[MAX_NAME_LEN];
	int depth;
	char message[31];
} Error;

char keyword[1000][MAX_VALUE_LEN]; //Array of keywords in the grammar
extern int num_of_keywords;

Grammar readGrammar(char *, Grammar);
Token *tokenizer(FILE *);
Parse_Tree *createParseTree(Token *, Grammar *);
void printParseTree(Parse_Tree *);

void traverseParseTree(Parse_Tree *statements);
void printTypeExpressionTable();
void printError(Error err);


// Traverse helpers begin
bool isDeclaration(Parse_Tree *statement);
bool isAssignment(Parse_Tree *statement);
int findDeclaredType(Parse_Tree *declaration);
Parse_Tree *findIdentifierList(Parse_Tree *declaration);

void insertPrimitiveInTable(Parse_Tree *identifier_list, int type);
void insertArrayInTable(Parse_Tree *identifier_list, int type, int line_num, Parse_Tree *statements);

void checkAssignment(Parse_Tree *assignment);
// Traverse helpers end

#endif
