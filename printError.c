/*
 * Group No. - 48
 * Members:
 * 2018A7PS0181P, Bikash Jena
 * 2018A7PS0276P, Jatin Jain
 * 2018A7PS0131P, Utkarsh Dwivedi
 */

#include "structures.h"

void printError(Error err)
{
  printf("Error at line %d, ", err.line_num);
  printf("%s, ", err.statement_type);
  printf("operator: %s,\t", err.operator);
  printf("first operand: %s ", err.first_lex);
  printf("%s,\t", err.first_type);
  printf("second operand: %s ", err.second_lex);
  printf("%s,\t", err.second_type);
  printf("depth: %d,\t", err.depth);
  printf("%s\n", err.message);
}