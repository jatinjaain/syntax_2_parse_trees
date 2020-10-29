all:
	gcc -g -o output driver.c readGrammar.c parser.c grammar_definitions.c tokenizer.c traverse.c typeExpression.c

clean:
	rm ./output