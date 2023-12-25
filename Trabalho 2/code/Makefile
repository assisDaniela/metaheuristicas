all: ils sa grasp

ils: ILS.o TSPP.o
	g++ -o ils ILS.o TSPP.o

sa: SA.o TSPP.o
	g++ -o sa SA.o TSPP.o

grasp: Grasp.o TSPP.o
	g++ -o grasp Grasp.o TSPP.o

Grasp.o: Grasp.cpp TSPP.h
	g++ -o Grasp.o Grasp.cpp -c

SA.o: SA.cpp TSPP.h
	g++ -o SA.o SA.cpp -c

ILS.o: ILS.cpp TSPP.h
	g++ -o ILS.o ILS.cpp -c

TSPP.o: TSPP.cpp TSPP.h
	g++ -o TSPP.o TSPP.cpp -c

clean:
	rm -rf *.o