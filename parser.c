/*
 * provides createParseTree(parseTree* T,tokenStream* S, grammar* G)
 * creates parse tree at T of token-stream S using grammar G
 * 
 * algorithm:
 * 
 *    Detailed in the parsingAlgorithm
*/

#include "structures.h"
#include "grammar_definitions.h"

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

Parse_Result Parse(Token*,Label,int);
bool is_identifier(Token*, Label);
bool is_keyword(Token*,Label);
bool is_constant(Token*,Label);

Parse_Tree* createParseTree(Parse_Tree* t, Token* s, Grammar* g){
    s = s->next_node->next_node;//skipping two tokens "program" and "{"
    Parse_Tree* final_parsed_tree;
    final_parsed_tree->children = (Parse_Tree**)malloc(MAX_LINES*sizeof(Parse_Tree*));
    final_parsed_tree->num_of_children = 0;
    Token* itr = s;
    Token* statement_start = s;
    Label statement_label;
    int statement_length = 0;
    strcpy(statement_label.name,"assignment");
    int line = 1;
    Parse_Result temp_pr;
    while(itr!=NULL){
        statement_length++;
        if(itr->line_num!=line){
            statement_start = itr;
            line=itr->line_num;
            temp_pr = Parse(statement_start,statement_label,statement_length);
            statement_length = 0;
            final_parsed_tree->children[final_parsed_tree->num_of_children++] = temp_pr.subtree;
        }
    }
    strcpy(final_parsed_tree->name,"statement-list");
    return final_parsed_tree;
}

Parse_Result Parse(Token* starting_node, Label label, int length){
    Parse_Result res;

    //checking and handling terminals
    if(length == 1 && (strcmp(IDENTIFIER,label.name) || strcmp(CONSTANT,label.name) || strcmp(KEYWORD,label.name))){//check if length is one and its a terminal
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
    //checking each rule
    for(int i=0;i<grammar->num_of_rules;i++){
        if(!strcmp(grammar->rules[i].name,starting_node->name)) continue; //skip if left non terminal does not match
        
        Stack stack;
        stack.index=0;

        Token* t = starting_node->next_node;
        Grammar_Node* rn;
        int l = 1;
        int tokens_parsed = 0;

        Parse_Result temp_pr;
        Label temp_l;

        while(true){
            if(rn==NULL){
                if(tokens_parsed==length){
                    Parse_Tree* pt = (Parse_Tree*)malloc(sizeof(Parse_Tree));
                    pt->children = (Parse_Tree**)malloc(stack.index*sizeof(Parse_Tree*));
                    for(int j=0;j<stack.index;j++) pt->children[j] = stack.elements[j].subtree;
                    res.match = true;
                    res.subtree = pt;
                    return res;
                }
                else{
                    Stack_Element popped = pop(&stack);
                    t = popped.current;
                    tokens_parsed -= l;
                    l = popped.current_len+1;
                    rn = rn->prev_node;
                    Parse_Tree* temp_tree = popped.subtree;
                    while(l==l - grammar->rules[i].num_of_nodes + 1){
                        delete_subtree(temp_tree,1);
                        if(stack.index==0){
                            res.match = false;
                            return res;
                        }
                        else{
                            popped = pop(&stack);
                            t = popped.current;
                            tokens_parsed -= popped.current_len;
                            l = popped.current_len+1;
                            rn = rn->prev_node; 
                            temp_tree = popped.subtree;
                        }
                    }
                    l++;
                    continue;
                }
            }

            strcpy(temp_l.name,grammar->rules[i].name);
            strcpy(temp_l.value,grammar->rules[i].value);
            
            temp_pr = Parse(t,temp_l,l);
            Parse_Tree* temp_tree = temp_pr.subtree;

            if(temp_pr.match){
                Stack_Element to_push;
                to_push.current = t;
                to_push.current_len = l;
                to_push.subtree = temp_pr.subtree;
                push(&stack,&to_push);
                for(int i=0;i<l;i++) t=t->next_node;
                tokens_parsed+=l;
                l=1;
                continue;   
            }

            if(!temp_pr.match){ 
                while(l==length - grammar->rules[i].num_of_nodes +1){
                    delete_subtree(temp_tree,1);
                    if(stack.index==0){
                        res.match = false;
                        return res;
                    }
                    else{
                        Stack_Element popped = pop(&stack);
                        t = popped.current;
                        tokens_parsed -= popped.current_len;
                        l = popped.current_len+1;
                        rn = rn->prev_node; 
                        temp_tree=popped.subtree;
                    }
                }
                l++;
                continue;
            }
        }
    }
}

/*
 * checks if token is the keyword or constant or identifier specified by label 
*/
bool is_keyword(Token* t, Label l){
    if(strcmp(t->name,KEYWORD) && strcmp(t->value,l.value)) return true;
    return false;
}
bool is_constant(Token* t, Label l){
    if(strcmp(t->name,CONSTANT)) return true;
    return false;
}
bool is_identifier(Token* t, Label l){
    if(strcmp(t->name,IDENTIFIER)) return true;
    return false;
}