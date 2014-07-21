
test: test.o blackjk_imp.o
	g++ test.o blackjk_imp.o -o test

test.o: test.cpp blackjk.h
	g++ test.cpp blackjk.h -o test.o

blackjk_imp.o: blackjk_imp.cpp blackjk.h
	g++ blackjk_imp.cpp blackjk.h -o blackjk_imp.o
