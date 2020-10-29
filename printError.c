#include "structures.h"

void printError(Error err)
{
  printf("Error at line %d, ", err.line_num);
  printf("%s, ", err.statement_type);
  printf("operator: %s, ", err.operator);
  printf("first operand: %s ", err.first_lex);
  printf("%s,\t", err.first_type);
  printf("second operand: %s ", err.second_lex);
  printf("%s,\t", err.second_type);
  printf("depth: %d,\t", err.depth);
  printf("%s\n", err.message);
}