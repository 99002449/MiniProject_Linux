all: all.out 
all.out : mutex.o 
	gcc $^ -o $@ -lpthread
mutex.o: mutex.c 
	gcc $< -c
clean:
	rm -rf *.o *.out
