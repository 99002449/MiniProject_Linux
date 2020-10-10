all: all.out 
all.out : mutex.o 
	g++ $^ -o $@ -lpthread
mutex.o: mutex.c 
	g++ $< -c
clean:
	rm -rf *.o *.out
