
test: test.o blackjk_imp.o
	g++ test.o blackjk_imp.o -o test

test.o: test.cpp blackjk.h
	g++ -c test.cpp 

blackjk_imp.o: blackjk_imp.cpp blackjk.h
	g++ -c blackjk_imp.cpp 
