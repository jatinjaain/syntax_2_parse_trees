#include "structures.h"

#define INTEGER 0
#define REAL 1
#define BOOLEAN 2
#define RARRAY 3
#define JARRAY 4

int num_of_identifiers = 0;

bool isDeclaration(Parse_Tree *statement)
{
	if (strcmp(statement->children[0]->name, "declaration") == 0)
	{
		return true;
	}

	return false;
}

bool isAssignment(Parse_Tree *statement)
{
	if (strcmp(statement->children[0]->name, "assignment") == 0)
	{
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
	for (int j = 0; j < declaration->num_of_children; j++)
	{
		if (strcmp(declaration->children[j]->name, "identifier-list") == 0)
		{
			return declaration->children[j];
		}
		else if (strcmp(declaration->children[j]->name, "identifier") == 0)
		{
			return declaration->children[j];
		}
	}
}

void insertPrimitiveInTable(Parse_Tree *identifier_list, int type)
{
	Parse_Tree *itr = identifier_list;
	while (1)
	{
		Table[num_of_identifiers].type = Primitive;
		strcpy(Table[num_of_identifiers].arrayAttribute, "not_applicable");

		switch (type)
		{
		case INTEGER:
			strcpy(Table[num_of_identifiers].expression.typeName, "integer");
			break;

		case REAL:
			strcpy(Table[num_of_identifiers].expression.typeName, "real");
			break;

		case BOOLEAN:
			strcpy(Table[num_of_identifiers].expression.typeName, "boolean");
			break;

		default:
			break;
		}

		// This order in if-else ladder is for a reason
		if (itr->num_of_children == 0)
		{
			strcpy(Table[num_of_identifiers++].name, itr->value);
			break;
		}
		else if (itr->num_of_children == 2)
		{
			strcpy(Table[num_of_identifiers++].name, itr->children[0]->value);
			itr = itr->children[1];
		}
		else if (itr->num_of_children == 1)
		{
			strcpy(Table[num_of_identifiers++].name, itr->children[0]->value);
			break;
		}
	}
}

void insertArrayInTable(Parse_Tree *identifier_list, int type, int line_num, Parse_Tree *statements)
{
	bool staticEnumeration = true;
	bool jaggedMismatch = false;
	Error jerror;
	Range array_limits;
	if (type == RARRAY)
	{
		Parse_Tree *declaration = statements->children[line_num]->children[0];
		Parse_Tree *enum_list = findEnumList(declaration);
		if ((staticEnumeration = isStaticEnum(enum_list)) == true)
		{
			array_limits = calculateStaticLimits(enum_list);
		}

	}

	Parse_Tree *itr = identifier_list;
	while (1)
	{
		switch (type)
		{
		case RARRAY:
			Table[num_of_identifiers].type = Rarray;
			if (staticEnumeration == true)
			{
				strcpy(Table[num_of_identifiers].arrayAttribute, "static");
				Table[num_of_identifiers].expression.range = array_limits;
			}
			else
			{
				strcpy(Table[num_of_identifiers].arrayAttribute, "dynamic");
				strcpy(Table[num_of_identifiers].expression.typeName, "array");
			}
			break;

		case JARRAY:
			Table[num_of_identifiers].type = Jarray;
			strcpy(Table[num_of_identifiers].arrayAttribute, "not_applicable");
			if (errorJaggedArray(statements, line_num) != line_num)
			{
				strcpy(Table[num_of_identifiers].expression.typeName, "error");
				if(jaggedMismatch == false)
				{
					jaggedMismatch = true;
					jerror.depth = 3;
					jerror.line_num = identifier_list->line_num + errorJaggedArray(statements, line_num) - line_num;
					strcpy(jerror.statement_type, "declaration");
					strcpy(jerror.operator, "***");
					strcpy(jerror.first_lex, "***");
					strcpy(jerror.first_type, "***");
					strcpy(jerror.second_lex, "***");
					strcpy(jerror.second_type, "***");
					strcpy(jerror.message, "3D JA size mismatch");
				}
			}
			else
			{
				strcpy(Table[num_of_identifiers].expression.typeName, "jagged");
			}
			
			
			break;

		default:
			break;
		}

		// This order in if-else ladder is for a reason
		if (itr->num_of_children == 0)
		{
			strcpy(Table[num_of_identifiers++].name, itr->value);
			break;
		}
		else if (itr->num_of_children == 2)
		{
			strcpy(Table[num_of_identifiers++].name, itr->children[0]->value);
			itr = itr->children[1];
		}
		else if (itr->num_of_children == 1)
		{
			strcpy(Table[num_of_identifiers++].name, itr->children[0]->value);
			break;
		}
	}
	if (jaggedMismatch == true)
	{
		printError(jerror);
	}
}

Parse_Tree *findEnumList(Parse_Tree *declaration_statement)
{
	for (int k = 0; k < declaration_statement->num_of_children; k++)
	{
		if (strcmp(declaration_statement->children[k]->name, "type") == 0)
		{
			return declaration_statement->children[k]->children[0]->children[0]->children[1];
		}
	}
}

bool isStaticEnum(Parse_Tree *enum_list)
{
	Parse_Tree *enum_itr = enum_list;
	
	while (enum_itr->num_of_children == 2)
	{		
		if (strcmp(enum_itr->children[0]->children[0]->name, "static_enumeration") != 0)
		{
			return false;
		}

		enum_itr = enum_itr->children[1];
	}

	if (strcmp(enum_itr->children[0]->children[0]->name, "static_enumeration") != 0)
	{
		return false;
	}

	return true;
}

Range calculateStaticLimits(Parse_Tree *enum_list)
{
	Parse_Tree *enum_itr = enum_list;
	Parse_Tree *temp;
	Range limits;
	limits.num_of_dimensions = 0;
	while (enum_itr->num_of_children == 2)
	{
		temp = enum_itr->children[0]->children[0];

		limits.lowest[limits.num_of_dimensions] = atoi(temp->children[1]->value);
		limits.highest[limits.num_of_dimensions] = atoi(temp->children[3]->value);
		limits.num_of_dimensions++;

		enum_itr = enum_itr->children[1];
	}

	temp = enum_itr->children[0]->children[0];
	limits.lowest[limits.num_of_dimensions] = atoi(temp->children[1]->value);
	limits.highest[limits.num_of_dimensions] = atoi(temp->children[3]->value);
	limits.num_of_dimensions++;

	// printf("No. of dim=\t%d\n", limits.num_of_dimensions);

	// for (size_t i = 0; i < limits.num_of_dimensions; i++)
	// {
	// 	printf("%d\t%d\n", limits.lowest[i], limits.highest[i]);
	// }
	
	return limits;
}

int errorJaggedArray(Parse_Tree *statement_list, int line_num)
{
	Parse_Tree *brackets, *type_node = statement_list->children[line_num]->children[0];
	for (int k = 0; k < type_node->num_of_children; k++)
	{
		if (strcmp(type_node->children[k]->name, "type") == 0)
		{
			brackets = type_node->children[k]->children[0]->children[0]->children[3];
			break;
		}
	}

	if (brackets->num_of_children == 2)
	{
		return line_num;
	}

	int line_track = line_num;
	for (Parse_Tree *itr = statement_list->children[++line_track]; strcmp(itr->children[0]->name, "row") == 0; itr = statement_list->children[++line_track])
	{
		int size = atoi(itr->children[0]->children[6]->value);
		int count = 0;
		Parse_Tree *const_itr = itr->children[0]->children[10];
		while (const_itr->num_of_children == 3)
		{
			if(++count > size)
			{
				return line_track;
			}
			const_itr = const_itr->children[2];
		}

		count++;
		if (size != count)
		{
			return line_track;
		}
		
	}

	return line_num;
}

Error checkAssignment(Parse_Tree *assignment_statement)
{
	Error assignment_error;
	assignment_error.line_num = assignment_statement->line_num;
	switch (assignment_statement->children[0]->num_of_children)
	{
	case 1:
		// typeExpression to_assign = findType(assignment_statement->children[0]->children[0]);
		// assignment_error = verifyExpression(assignment_statement->children[2], to_assign);
		break;
	
	case 2:

		break;
	default:
		break;
	}
}

Error verifyExpression(Parse_Tree *expression, typeExpression left_type)
{
	Error err;
	Parse_Tree *itr = expression;
	if (strcmp(expression->children[0]->name, "arithmetic_expression") == 0)
	{
		if (strcmp(left_type.expression.typeName, "real") == 0)
		{
			while (itr->num_of_children == 3)
			{
				err = verifyTerm(itr->children[0], left_type);
				if (strcmp(err.message, "clear") != 0)
				{
					return err;
				}
				itr = itr->children[3];
			}
			err = verifyTerm(itr->children[0], left_type);
			if (strcmp(err.message, "clear") != 0)
			{
				return err;
			}
		}
		
		
	}
	
}

Error verifyTerm(Parse_Tree *term, typeExpression left_type)
{
	Error err;
	err.line_num = term->line_num;
	strcpy(err.message, "clear");
	


	return err;
}