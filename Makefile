
blackjk: main.o blackjk_imp.o
	g++ main.o blackjk_imp.o -o blackjk

blackjk.o: main.cpp blackjk.h
	g++ -c main.cpp  

blackjk_imp.o: blackjk_imp.cpp blackjk.h
	g++ -c blackjk_imp.cpp 
