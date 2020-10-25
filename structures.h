#ifndef STRUCTURES_H
#define STRUCTURES_H

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define MAX_NAME_LEN 50 //maximum length of names of tokens, variables
#define MAX_VALUE_LEN 50 //maximum length of value of tokens
#define MAX_RULE_LEN 1000 //maximum length of a rule line
#define MAX_RULES_NUM 100 //maximum number of rules
#define MAX_CAPACITY_STACK 20 //maximum capacity of stack

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
typedef struct Grammar_Node{
    char name[MAX_NAME_LEN];
    char value[MAX_VALUE_LEN];
    int num_of_nodes;
    struct Grammar_Node* next_node;
    struct Grammar_Node* prev_node;
} Grammar_Node;

/* Represents grammar
 *
 * num_of_rules: total number of rules in the grammar;
 * 
 * rules: array of pointers to single left variable of each rule 
*/
typedef struct{
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
typedef struct Token{
    char name[MAX_NAME_LEN];
    char value[MAX_VALUE_LEN];
    int line_num;
    struct Token* next_node;
    struct Token* prev_node;
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
typedef struct Parse_Tree{
    char name[MAX_NAME_LEN];
    char value[MAX_VALUE_LEN];
    int num_of_children;
    struct Parse_Tree** children; //array of children
} Parse_Tree;

char keyword[1000][MAX_VALUE_LEN];    //Array of keywords in the grammar
int num_of_keywords = 0;

#endif
