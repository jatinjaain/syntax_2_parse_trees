#include "structures.h"
#include "grammar_definitions.h"

void printGrammar(Grammar*);

int main(){
    int option;
    Grammar G;
    while(1){
        scanf("%d",&option);
        switch(option){
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
                G=readGrammar("grammar.txt",G);
                printGrammar(&G);
                break;
        }
    } 
}

void printGrammar(Grammar* G){
    for(int i=0;i<G->num_of_rules;i++){
        printf("%s:=\t",G->rules[i].name);
        Grammar_Node* k=&(G->rules[i]);
        for(int j=1;j<G->rules[i].num_of_nodes+1;j++){
            printf("%s",k->name);
            //if(strcmp(G->rules[i].name,KEYWORD)) printf("/%s",G->rules[i].value);
            printf("/%s",k->value);
            printf("\t");
            k=k->next_node;
        }
        printf("\n");
    }
}