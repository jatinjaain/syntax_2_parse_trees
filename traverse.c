#include "structures.h"

#define INTEGER 0
#define REAL 1
#define BOOLEAN 2
#define RARRAY 3
#define JARRAY 4

bool isDeclaration(Parse_Tree *statement);
int findDeclaredType(Parse_Tree *declaration);
Parse_Tree *findIdentifierList(Parse_Tree *declaration);

int sizeJaggedArray(Parse_Tree *declaration);
void insertPrimitiveInTable(Parse_Tree *identifier_list, int type);
void fillTypeExpressionTable(Parse_Tree *identifier_list, int type);

int num_of_identifiers = 0;

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
      printf("type:\t%d\n", type);
      fflush(stdout);

      switch (type)
      {
      case INTEGER:
      case REAL:
      case BOOLEAN:
        insertPrimitiveInTable(identifier_list, type);
        break;

      default:
        break;
      }

      // Parse_Tree *identifier_list = findIdentifierList(statements->children[i]);
      // if (type == JARRAY)
      // {
      //   i += sizeJaggedArray(statements->children[i]) + 1;
      // }

      // fillTypeExpressionTable(identifier_list, type);
    }
  }
  printf("No. of identifiers:\t%d\n", num_of_identifiers);
}

bool isDeclaration(Parse_Tree *statement)
{
  if (strcmp(statement->children[0]->name, "declaration") == 0)
  {
    printf("declaration\n");
    return true;
  }

  return false;
}

int findDeclaredType(Parse_Tree *declaration)
{
  Parse_Tree *itr;
  for (int j = 0; j < declaration->num_of_children; j++)
  {
    if (strcmp(declaration->children[j]->name, "type") == 0)
      itr = declaration->children[j];
  }

  itr = itr->children[0];
  if (strcmp(itr->name, "primitive_type") == 0)
  {
    itr = itr->children[0];
    if (strcmp(itr->value, "integer") == 0)
    {
      return INTEGER;
    }
    else if (strcmp(itr->value, "real") == 0)
    {
      return REAL;
    }
    else
    {
      return BOOLEAN;
    }
  }
  else
  {
    itr = itr->children[0];
    if (strcmp(itr->name, "rectangular_array") == 0)
    {
      return RARRAY;
    }
    else
    {
      return JARRAY;
    }
  }
}

Parse_Tree *findIdentifierList(Parse_Tree *declaration)
{
  // Parse_Tree *itr;
  for (int j = 0; j < declaration->num_of_children; j++)
  {
    printf("%d\t%s\n", j, declaration->children[j]->value);

    if (strcmp(declaration->children[j]->name, "identifier-list") == 0)
    {
      printf("found list\n");
      fflush(stdout);
      return declaration->children[j];
    }
    else if (strcmp(declaration->children[j]->name, "identifier") == 0)
    {
      printf("found single\n");
      fflush(stdout);
      return declaration->children[j];
    }
  }
}

void insertPrimitiveInTable(Parse_Tree *identifier_list, int type)
{
  Parse_Tree *itr = identifier_list;

  while (itr->num_of_children == 2)
  {
    strcpy(Table[num_of_identifiers].name, itr->children[0]->value);
		Table[num_of_identifiers].type = Primitive;
		strcpy(Table[num_of_identifiers].arrayAttribute, "not_applicable");
		switch (type)
		{
		case INTEGER:
			strcpy(Table[num_of_identifiers].expression, "integer");
			break;
		
		case REAL:
			strcpy(Table[num_of_identifiers].expression, "real");
			break;

		case BOOLEAN:
			strcpy(Table[num_of_identifiers].expression, "boolean");
			break;

		default:
			break;
		}

		num_of_identifiers++;
    itr = itr->children[1];
  }

  if (itr->num_of_children == 1)
  {
    itr = itr->children[0];
  }
  
  strcpy(Table[num_of_identifiers].name, itr->value);
	Table[num_of_identifiers].type = Primitive;
	strcpy(Table[num_of_identifiers].arrayAttribute, "not_applicable");
	switch (type)
	{
	case INTEGER:
		strcpy(Table[num_of_identifiers].expression, "integer");
		break;

	case REAL:
		strcpy(Table[num_of_identifiers].expression, "real");
		break;

	case BOOLEAN:
		strcpy(Table[num_of_identifiers].expression, "boolean");
		break;

	default:
		break;
	}

	num_of_identifiers++;

  // for (size_t j = 0; j < num_of_identifiers; j++)
  // {
  //   printf("%s\t%d\t%s\t%s\n", Table[j].name, Table[j].type, Table[j].arrayAttribute, Table[j].expression);
  // }
}