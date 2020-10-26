all:
	gcc -o output driver.c readGrammar.c parser.c grammar_definitions.c tokenizer.c

clean:
	rm ./output