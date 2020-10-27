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

Parse_Tree* createParseTree(Token* s, Grammar* g){
    grammar = g;
    s = s->next_node;//skipping two tokens "program" and "{"
    Parse_Tree* final_parsed_tree = (Parse_Tree*)malloc(sizeof(Parse_Tree));
    final_parsed_tree->children = (Parse_Tree**)malloc(MAX_LINES*sizeof(Parse_Tree*));
    final_parsed_tree->num_of_children = 0;
    Token* itr = s;
    Token* statement_start = s;
    Label statement_label;
    int statement_length = 0;
    strcpy(statement_label.name,"statement");
    int line = 2;
    Parse_Result temp_pr;
    while(itr!=NULL){
        if(itr->line_num!=line){
            line=itr->line_num;

            //printf("\nGoing to parse:");
            //Token* k=statement_start;
            //for(int i=0;i<statement_length;i++) {printf("%s ",k->value);k=k->next_node;}
            
            temp_pr = Parse(statement_start,statement_label,statement_length);
            
            statement_length = 0;
            final_parsed_tree->children[final_parsed_tree->num_of_children++] = temp_pr.subtree;
            //printf("Parsed %d\n",line);
            statement_start = itr;
        }
        statement_length++;
        itr=itr->next_node;
    }
    strcpy(final_parsed_tree->name,"statement-list");
    return final_parsed_tree;
}

Parse_Result Parse(Token* starting_node, Label label, int length){
    Token* k=starting_node;
    printf("---\nTesting %s %s for %d\n",label.name,strcmp(label.name,KEYWORD)==0?label.value:"",length);
    for(int i=0;i<length;i++) {printf("%s ",k->value);k=k->next_node;}
    printf("\n");

    Parse_Result res;

    //checking and handling terminals
    if(length == 1 && (strcmp(IDENTIFIER,label.name)==0 || strcmp(CONSTANT,label.name)==0 || strcmp(KEYWORD,label.name)==0)){//check if length is one and its a terminal
        Parse_Tree* pt = (Parse_Tree*)malloc(sizeof(Parse_Tree));
        pt->num_of_children = 0;
        pt->children = NULL;
        strcpy(pt->name,label.name);
        strcpy(pt->value,label.value);
        res.subtree = pt;
        if(is_keyword(starting_node,label) || is_constant(starting_node,label) || is_identifier(starting_node,label)){
            res.match = true;
            printf("returned true\n");
            return res;
        }
        printf("returned false\n");
        res.match = false;
        return res;
    }
    //checking each rule
    for(int i=0;i<grammar->num_of_rules;i++){
        //printf("comparing %s and %s which is %d\n",grammar->rules[i].name,label.name,strcmp(grammar->rules[i].name,label.name));
        if(!(strcmp(grammar->rules[i].name,label.name)==0)) continue; //skip if left non terminal does not match
        
        printf("rule %d\n",i);

        Stack stack;
        stack.index=0;

        Token* t = starting_node;
        Grammar_Node* rn = grammar->rules[i].next_node;
        int l = 1;
        int tokens_parsed = 0;
        int rule_node_num = 1;

        Parse_Result temp_pr;
        Label temp_l;

        while(true){
            if(rn==NULL || rule_node_num>grammar->rules[i].num_of_nodes){
                printf("inside 1 %d %d\n",rule_node_num,grammar->rules[i].num_of_nodes);
                if(tokens_parsed==length){
                    Parse_Tree* pt = (Parse_Tree*)malloc(sizeof(Parse_Tree));
                    pt->children = (Parse_Tree**)malloc(stack.index*sizeof(Parse_Tree*));
                    for(int j=0;j<stack.index;j++) pt->children[j] = stack.elements[j].subtree;
                    res.match = true;
                    res.subtree = pt;
                    printf("returned true 1\n");
                    return res;
                }
                else{
                    Stack_Element popped = pop(&stack);
                    if(stack.index==0){
                        res.match=false;
                        return res;
                    }
                    t = popped.current;
                    tokens_parsed -= l;
                    l = popped.current_len+1;
                    rn = rn->prev_node;
                    rule_node_num--;
                    Parse_Tree* temp_tree = popped.subtree;
                    while(l>= l - grammar->rules[i].num_of_nodes + 1){
                        //delete_subtree(temp_tree,1);
                        if(stack.index==0){
                            res.match = false;
                            printf("return false 1\n");
                            return res;
                        }
                        else{
                            popped = pop(&stack);
                            t = popped.current;
                            tokens_parsed -= popped.current_len;
                            l = popped.current_len+1;
                            rn = rn->prev_node;
                            rule_node_num--; 
                            temp_tree = popped.subtree;
                            printf("popping 1\n");
                        }
                    }
                    l++;
                    continue;
                }
            }

            strcpy(temp_l.name,rn->name);
            strcpy(temp_l.value,rn->value);
            
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
                rn=rn->next_node;
                rule_node_num++;
                if(tokens_parsed==length && rn!=NULL){
                    res.match=false;
                    return res;
                }
                continue;   
            }

            if(!temp_pr.match){
                printf("inside 2\n");
                while(l>=length - grammar->rules[i].num_of_nodes +1){
                    //delete_subtree(temp_tree,1);
                    if(stack.index==0){
                        res.match = false;
                        printf("returned true 2\n");
                        return res;
                    }
                    else{
                        Stack_Element popped = pop(&stack);
                        printf("popped 2 %d %d %d\n",popped.current_len,length,grammar->rules[i].num_of_nodes);
                        t = popped.current;
                        tokens_parsed -= popped.current_len;
                        l = popped.current_len;
                        rn = rn->prev_node;
                        rule_node_num--; 
                        temp_tree=popped.subtree;
                    }
                }
                l++;
                continue;
            }
        }
    }
    printf("returned false 3\n");
    res.match = false;
    return res;
}

/*
 * checks if token is the keyword or constant or identifier specified by label 
*/
bool is_keyword(Token* t, Label l){
    if(strcmp(t->name,KEYWORD)==0 && strcmp(t->value,l.value)==0 && strcmp(l.name,KEYWORD)==0) {return true;}
    return false;
}
bool is_constant(Token* t, Label l){
    if(strcmp(t->name,CONSTANT)==0 && strcmp(l.name,CONSTANT)==0) {return true;}
    return false;
}
bool is_identifier(Token* t, Label l){
    if(strcmp(t->name,IDENTIFIER)==0 && strcmp(l.name,IDENTIFIER)==0) {return true;}
    return false;
}