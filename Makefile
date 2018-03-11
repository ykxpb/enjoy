all:
	@echo "Makefile needs your attention"

hazard_pointer: hazard_pointer.cpp
	g++ -std=c++11 -pthread hazard_pointer.cpp -o out/hazard_pointer

trie: trie.cpp
	g++ -std=c++11 trie.cpp -o out/trie


# vim:ft=make
#
