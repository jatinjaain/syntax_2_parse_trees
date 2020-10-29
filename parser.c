/*
 * Group No. - 48
 * Members:
 * 2018A7PS0181P, Bikash Jena
 * 2018A7PS0276P, Jatin Jain
 * 2018A7PS0131P, Utkarsh Dwivedi
 */

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
    Grammar_Node* rule_node;
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
        //if(pt->children[i]->num_of_children != 0) delete_subtree(pt->children[i],0);
        delete_subtree(pt->children[i],0);
        //free(pt->children[i]);
    }
    free(pt->children);
    free(pt);
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

Parse_Result Parse(Token*,Label,int,int,int);
bool is_identifier(Token*, Label);
bool is_keyword(Token*,Label);
bool is_constant(Token*,Label);

Parse_Tree* createParseTree(Token* s, Grammar* g){
    grammar = g;
    s = s->next_node->next_node->next_node->next_node;//skipping first four tokens "program", "(",")" and "{"
    Parse_Tree* final_parsed_tree = (Parse_Tree*)malloc(sizeof(Parse_Tree));
    final_parsed_tree->children = (Parse_Tree**)malloc(MAX_LINES*sizeof(Parse_Tree*));
    final_parsed_tree->num_of_children = 0;
    Token* itr = s;
    Token* statement_start = s;
    Label statement_label;
    int statement_length = 0;
    strcpy(statement_label.name,"statement");
    int line = 3;
    Parse_Result temp_pr;
    while(itr!=NULL){
        if(itr->line_num!=line){

            //printf("\nGoing to parse:");
            //Token* k=statement_start;
            //for(int i=0;i<statement_length;i++) {printf("%s ",k->value);k=k->next_node;}
            
            temp_pr = Parse(statement_start,statement_label,statement_length,line,1);
            //printf("line parse result: %d %s\n",temp_pr.match,temp_pr.subtree->name);
            statement_length = 0;
            if(temp_pr.match==false) printf("syntatically wrong statement %d\n",line);
            final_parsed_tree->children[final_parsed_tree->num_of_children++] = temp_pr.subtree;
            //printf("Filled %d\n",final_parsed_tree->num_of_children-1);
            statement_start = itr;
            line=itr->line_num;
        }
        statement_length++;
        itr=itr->next_node;
    }
    strcpy(final_parsed_tree->name,"statement-list");
    final_parsed_tree->rule_num = 1;
    //printf("Total children %d\n",final_parsed_tree->num_of_children);
    return final_parsed_tree;
}

Parse_Result Parse(Token* starting_node, Label label, int length, int line,int depth){
    Token* k=starting_node;
    //printf("---\nTesting %s %s for %d\n",label.name,strcmp(label.name,KEYWORD)==0?label.value:"",length);
    //for(int i=0;i<length;i++) {printf("%s ",k->value);k=k->next_node;}
    //k=starting_node;
    //printf("\n");

    //if(strcmp(label.name,"assignment")==0) printf("-----------------------------------------------------------------------------------------\n");
    //if(strcmp(label.name,"expression")==0) printf("00000000000000000000000000000000000000000000000000000000000\n");
    //if(strcmp(label.name,"arithmetic_expression")==0) printf("XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX\n");

    Parse_Result res;
    Parse_Tree* temp_tree;

    if(length!=1 && (strcmp(IDENTIFIER,label.name)==0 || strcmp(CONSTANT,label.name)==0 || strcmp(KEYWORD,label.name)==0)){
        res.match = false;
        //printf("return false imm\n");
        return res;
    }
    //checking and handling terminals
    if(length == 1 && (strcmp(IDENTIFIER,label.name)==0 || strcmp(CONSTANT,label.name)==0 || strcmp(KEYWORD,label.name)==0)){//check if length is one and its a terminal
        Parse_Tree* pt = (Parse_Tree*)malloc(sizeof(Parse_Tree));
        pt->num_of_children = 0;
        pt->children = NULL;
        strcpy(pt->name,label.name);
        strcpy(pt->value,starting_node->value);
        pt->line_num=line;
        res.subtree = pt;
        res.subtree->depth=depth;
        if(is_keyword(starting_node,label) || is_constant(starting_node,label) || is_identifier(starting_node,label)){
            res.match = true;
            //printf("returned true\n");
            return res;
        }
        //printf("returned false\n");
        res.match = false;
        delete_subtree(res.subtree,1);
        return res;
    }
    //checking each rule
    for(int i=0;i<grammar->num_of_rules;i++){
        //printf("comparing %s and %s which is %d\n",grammar->rules[i].name,label.name,strcmp(grammar->rules[i].name,label.name));
        if(!(strcmp(grammar->rules[i].name,label.name)==0)) continue; //skip if left non terminal does not match
        
        //printf("rule %d ",i);
        //for(int j=0;j<length;j++) {printf("%s ",k->value);k=k->next_node;}
        //k=starting_node;
        //printf("\n");

        Stack stack;
        stack.index=0;

        Token* t = starting_node;
        Grammar_Node* rn = grammar->rules[i].next_node;
        int l = 1;
        int tokens_parsed = 0;
        int rule_node_num = 1;

        Parse_Result temp_pr;
        Label temp_l;

        bool next_rule = false;

        while(true){
            if(rn==NULL || rule_node_num>grammar->rules[i].num_of_nodes){
                //printf("inside 1 %d %d\n",rule_node_num,grammar->rules[i].num_of_nodes);
                if(tokens_parsed==length){
                    Parse_Tree* pt = (Parse_Tree*)malloc(sizeof(Parse_Tree));
                    pt->rule_num=i+1;
                    pt->num_of_children=stack.index;
                    pt->line_num=line;
                    pt->children = (Parse_Tree**)malloc(stack.index*sizeof(Parse_Tree*));
                    for(int j=0;j<stack.index;j++) pt->children[j] = stack.elements[j].subtree;
                    strcpy(pt->name,grammar->rules[i].name);
                    res.match = true;
                    res.subtree = pt;
                    res.subtree->depth=depth;
                    //printf("returned true 1\n");
                    return res;
                }
                else{
                    if(stack.index==0){
                        //res.match=false;
                        //return res;
                        next_rule=true;
                        //printf("gonna next rule -1\n");
                        break;
                    }
                    Stack_Element popped = pop(&stack);
                    t = popped.current;
                    tokens_parsed -= popped.current_len;
                    l = popped.current_len+1;
                    rn = popped.rule_node;
                    rule_node_num--;
                    temp_tree = popped.subtree;
                    delete_subtree(temp_tree,1);
                    //printf("trying with  A %d %d %d %d %d\n",length,l,grammar->rules[i].num_of_nodes,stack.index,tokens_parsed);
                    while(l> length - grammar->rules[i].num_of_nodes + stack.index - tokens_parsed+1){
                        if(stack.index==0){
                            res.match = false;
                            //printf("return false 1\n");
                            //return res;
                            next_rule = true;
                            //printf("gonna next rule -2\n");
                            break;
                        }
                        else{
                            popped = pop(&stack);
                            t = popped.current;
                            tokens_parsed -= popped.current_len;
                            l = popped.current_len+1;
                            rn = popped.rule_node;
                            rule_node_num--; 
                            temp_tree = popped.subtree;
                            delete_subtree(temp_tree,1);
                            //printf("popping 1 %s\n",popped.rule_node->name);
                        }
                    }
                    //l++;
                }
            }

            if(next_rule) break;

            strcpy(temp_l.name,rn->name);
            strcpy(temp_l.value,rn->value);
            
            temp_pr = Parse(t,temp_l,l,line,depth+1);
            temp_tree = temp_pr.subtree;

            if(temp_pr.match){
                Stack_Element to_push;
                to_push.current = t;
                to_push.current_len = l;
                to_push.subtree = temp_pr.subtree;
                to_push.rule_node=rn;
                push(&stack,&to_push);
                //printf("pushed %s\n",to_push.rule_node->name);
                for(int i=0;i<l;i++) t=t->next_node;
                tokens_parsed+=l;
                l=1;
                rn=rn->next_node;
                rule_node_num++;
                /*if(tokens_parsed==length && rn!=NULL){
                    //res.match=false;
                    //return res;
                    printf("return false 4\n");
                    break;
                }*/

                if(tokens_parsed==length && rn!=NULL){
                    Stack_Element popped;
                    popped=pop(&stack);
                    delete_subtree(popped.subtree,1);

                    if(stack.index==0) break; //next_rule
                    
                    popped = pop(&stack);
                    t = popped.current;
                    tokens_parsed -= popped.current_len;
                    l = popped.current_len+1;
                    rn = popped.rule_node;
                    rule_node_num--;
                    temp_tree = popped.subtree;
                    delete_subtree(temp_tree,1);
                    //printf("trying with B %d %d %d %d %d\n",length,l,grammar->rules[i].num_of_nodes,stack.index,tokens_parsed);
                    while(l> length - grammar->rules[i].num_of_nodes + stack.index - tokens_parsed+1){
                        //delete_subtree(temp_tree,1);
                        if(stack.index==0){
                            res.match = false;
                            //printf("return false 5\n");
                            //return res;
                            next_rule = true;
                            //printf("gonna next rule -5\n");
                            break;
                        }
                        else{
                            popped = pop(&stack);
                            t = popped.current;
                            tokens_parsed -= popped.current_len;
                            l = popped.current_len+1;
                            rn = popped.rule_node;
                            rule_node_num--; 
                            temp_tree = popped.subtree;
                            delete_subtree(temp_tree,1);
                            //printf("popping 5 %s\n",popped.rule_node->name);
                        }
                    }
                    if(next_rule) break;
                }
                continue;   
            }

            if(!temp_pr.match){
                l++;
                //printf("inside 2 %d %d %d\n",l,length,grammar->rules[i].num_of_nodes);
                while(l>length - grammar->rules[i].num_of_nodes + stack.index - tokens_parsed+1){
                    //delete_subtree(temp_tree,1);
                    if(stack.index==0){
                        //res.match = false;
                        //printf("returned false 2\n");
                        //return res;
                        next_rule=true;
                        break;
                    }
                    else{
                        Stack_Element popped = pop(&stack);
                        //printf("popped 2 %d %d %d\n",popped.current_len,length,grammar->rules[i].num_of_nodes);
                        t = popped.current;
                        tokens_parsed -= popped.current_len;
                        l = popped.current_len+1;
                        rn = rn->prev_node;
                        rule_node_num--; 
                        temp_tree=popped.subtree;
                        delete_subtree(temp_tree,1);
                    }
                }
            }
        }
        //printf("out of rule %d\n",i);
        //if(strcmp(label.name,"arithmetic_expression")==0) printf("NNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNN\n");
    }
    //if(strcmp(label.name,"arithmetic_expression")==0) printf("MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM\n");
    //printf("returned false 3\n");
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

void printParseTree(Parse_Tree* pt){
    /*
    printf("---\n");
    printf("%d\n"
    printf("%s with %d",pt->name,pt->num_of_children);
    if(strcmp(pt->name,KEYWORD)==0 || strcmp(pt->name,IDENTIFIER)==0 || strcmp(pt->name,CONSTANT)==0) printf(" %s",pt->value);
    printf("\n");
    for(int i=0;i<pt->num_of_children;i++) printParseTree(pt->children[i],depth+1);
    */

    
    printf("---\n");
    if(strcmp(pt->name,KEYWORD)==0 || strcmp(pt->name,IDENTIFIER)==0 || strcmp(pt->name,CONSTANT)==0){
        printf("terminal\n");
        printf("%s (%s)\n",pt->value,pt->name);
        printf("line number: %d\n",pt->line_num);
        printf("depth: %d\n",pt->depth);
    }
    else{
        printf("non-terminal\n");
        printf("%s\n",pt->name);
        printf("children: %d\n",pt->num_of_children);
        printf("grammar rule: %d\n",pt->rule_num);
        printf("depth: %d\n",pt->depth);
        for(int i=0;i<pt->num_of_children;i++) printParseTree(pt->children[i]);
    }
    
}