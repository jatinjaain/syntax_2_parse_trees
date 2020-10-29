all:
	gcc -g -o output driver.c readGrammar.c parser.c grammar_definitions.c tokenizer.c traverse.c typeExpression.c printError.c traverse_helper.c

clean:
	rm ./output