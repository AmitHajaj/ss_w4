all:
	gcc -c -w TRIE.c
	gcc TRIE.c -o frequency

frequency:
	gcc TRIE.c -o frequency
	
clean:
	rm *.o TRIE
