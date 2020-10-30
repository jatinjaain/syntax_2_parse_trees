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
  printf("Error at line %-8d ", err.line_num);
  printf("%-20s ", err.statement_type);
  printf("operator: %-10s ", err.operator);
  printf("first operand: %-7s ", err.first_lex);
  printf("%-20s", err.first_type);
  printf("second operand: %-7s ", err.second_lex);
  printf("%-20s", err.second_type);
  printf("depth: %-10d ", err.depth);
  printf("%-30s\n", err.message);
}