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
  printf("No. of identifiers:\t%d\n\n", num_of_identifiers);
  printf("%-20s%-20s%-20s%-30s\n", "name", "enum_type", "attribute", "type_expression/range");
  for (int i = 0; i < num_of_identifiers; i++)
  {
    printf("%-20s", Table[i].name);
    printf("%-20d", Table[i].type);
    printf("%-20s", Table[i].arrayAttribute);
    if (strcmp(Table[i].arrayAttribute, "static") == 0)
    {
      for (int j = 0; j < Table[i].expression.range.num_of_dimensions; j++)
      {
        printf("(%d,%d) ", Table[i].expression.range.lowest[j], Table[i].expression.range.highest[j]);
      }
      printf("\n");
    }
    else
    {
      printf("%-30s\n", Table[i].expression.typeName);
    }
    
    
  }
  
}