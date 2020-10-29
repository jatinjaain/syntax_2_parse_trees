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