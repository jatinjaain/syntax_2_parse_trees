/*
 * Group No. - 48
 * Members:
 * 2018A7PS0181P, Bikash Jena
 * 2018A7PS02576P, Jatin Jain
 * 2018A7PS0131P, Utkarsh Dwivedi
 */

#include "structures.h"

void printTypeExpressionTable()
{
  printf("No. of identifiers:\t%d\n", num_of_identifiers);
  for (int i = 0; i < num_of_identifiers; i++)
  {
    printf("%s\t", Table[i].name);
    printf("%d\t", Table[i].type);
    printf("%s\t\t", Table[i].arrayAttribute);
    if (strcmp(Table[i].arrayAttribute, "static") == 0)
    {
      for (int j = 0; j < Table[i].expression.range.num_of_dimensions; j++)
      {
        printf("%d, %d\t", Table[i].expression.range.lowest[j], Table[i].expression.range.highest[j]);
      }
      printf("\n");
    }
    else
    {
      printf("%s\n", Table[i].expression.typeName);
    }
    
    
  }
  
}