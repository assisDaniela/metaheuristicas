all: p0 p1 p2

p0: TP1_P0.o TSPP.o
	g++ -o p0 TP1_P0.o TSPP.o

p1: TP1_P1.o TSPP.o
	g++ -o p1 TP1_P1.o TSPP.o

p2: TP1_P2.o TSPP.o
	g++ -o p2 TP1_P2.o TSPP.o

TP1_P0.o: TP1_P0.cpp TSPP.h
	g++ -o TP1_P0.o TP1_P0.cpp -c

TP1_P1.o: TP1_P1.cpp TSPP.h
	g++ -o TP1_P1.o TP1_P1.cpp -c

TP1_P2.o: TP1_P2.cpp TSPP.h
	g++ -o TP1_P2.o TP1_P2.cpp -c

TSPP.o: TSPP.cpp TSPP.h
	g++ -o TSPP.o TSPP.cpp -c

clean:
	rm -rf *.o