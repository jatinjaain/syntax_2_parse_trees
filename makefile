all:
	gcc -g -o output driver.c readGrammar.c parser.c grammar_definitions.c tokenizer.c traverse.c

clean:
	rm ./output