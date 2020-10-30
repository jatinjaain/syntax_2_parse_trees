/*
 * Group No. - 48
 * Members:
 * 2018A7PS0181P, Bikash Jena
 * 2018A7PS0276P, Jatin Jain
 * 2018A7PS0131P, Utkarsh Dwivedi
 */

#include "structures.h"

#define INTEGER 0
#define REAL 1
#define BOOLEAN 2
#define RARRAY 3
#define JARRAY 4

void traverseParseTree(Parse_Tree *statements)
{
	int num_of_statements = statements->num_of_children;
	for (int i = 0; i < num_of_statements; i++)
	{
		if (isDeclaration(statements->children[i]))
		{
			Parse_Tree *declaration_statement = statements->children[i]->children[0];
			int type = findDeclaredType(declaration_statement);
			Parse_Tree *identifier_list = findIdentifierList(declaration_statement);

			switch (type)
			{
			case INTEGER:
			case REAL:
			case BOOLEAN:
				insertPrimitiveInTable(identifier_list, type);
				break;

			case RARRAY:
			case JARRAY:
				insertArrayInTable(identifier_list, type, i, statements);
				break;

			default:
				break;
			}
		}
		else if (isAssignment(statements->children[i]))
		{
			// printf("assignment\n");
			checkAssignment(statements->children[i]->children[0]);
		}
		
	}
}

