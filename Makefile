objects = main.o Timer.o SimpleTimer.o

SimpleTimer : $(objects)
		g++ -lpython3.5m -ldl -o SimpleTimer $(objects)

main.o : main.cpp
Timer.o : Timer.cpp
SimpleTimer.o : SimpleTimer.cpp
		g++ -I /usr/include/python3.5m -std=c++11 -c SimpleTimer.cpp
