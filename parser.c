/*
 * provides createParseTree(parseTree* T,tokenStream* S, grammar* G)
 * creates parse tree at T of token-stream S using grammar G
 * 
 * algorithm:
 * 
 *    Detailed in the parsingAlgorithm
*/

#include <structures.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

#define MAX_NAME_LEN 50
#define MAX_VALUE_LEN 50

Grammar* grammar;

//Stack
/*
 * stack that will be utilised in parsing algorithm (see parsingAlgorithm at draw.io)
*/
typedef struct Stack_Element{
    Token* current;
    int current_len;
    Parse_Tree* subtree;
} Stack_Element;

// stack that will be used in the parsing algorithm
typedef struct Stack{
    int index;
    Stack_Element elements[MAX_CAPACITY_STACK];
} Stack;

void push(Stack* s,Stack_Element* se){
    s->elements[s->index++] = *se;
}
Stack_Element pop(Stack* s){
    return s->elements[--(s->index)];
}

//Parse_Tree
void delete_subtree(Parse_Tree* pt, int delete_self){
//given a parse tree it frees its memory as it was dynamically allocated
    for(int i=0;i<pt->num_of_children;i++){
        if(pt->children[i]->num_of_children != 0) delete_subtree(pt->children[i],0);
        free(pt->children[i]);
    }
    if(delete_self == 1) free(pt);
}


//create parse tree

typedef struct Parse_Result{
    bool match;
    Parse_Tree* subtree;
} Parse_Result;

typedef struct Label{
    char name[MAX_NAME_LEN];
    char value[MAX_VALUE_LEN];
} Label;

void createParseTree(Parse_Tree* t, Token* s, Grammar* g){

}

Parse_Result Parse(Token* starting_node, Label label, int length){
    Parse_Result res;
    bool is_declaration = false;

    //checking and handling terminals
    if(length == 1 /* && label == terminal*/){
        Parse_Tree* pt = (Parse_Tree*)malloc(sizeof(Parse_Tree));
        pt->num_of_children = 0;
        pt->children = NULL;
        strcpy(pt->name,label.name);
        strcpy(pt->value,label.value);
        res.subtree = pt;
        if(is_keyword(starting_node,label) || is_constant(starting_node,label) || is_identifier(starting_node,label)){
            res.match = true;
            return res;
        }
        res.match = false;
        return res;
    }

    if(/* starting token is declare*/ NULL){
        is_declaration = true;
    }

    //checking each rule
    for(int i=0;i<grammar->num_of_rules;i++){
        if(!strcmp(grammar->rules[i].name,starting_node->name)) continue; //skip if left non terminal does not match
        
        Stack stack;
        Token* t = starting_node->next_node;
        Grammar_Node* rn;
        int l = 1;
        int tokens_parsed = 0;

        while(true){
            if(rn==NULL){
                if(tokens_parsed==length)
            }
        }
    }
}

/*
 * checks if token is the keyword or constant or identifier specified by label 
*/
bool is_keyword(Token* t, Label l){

}
bool is_constant(Token* t, Label l){

}
bool is_identifier(Token* t, Label l){

}