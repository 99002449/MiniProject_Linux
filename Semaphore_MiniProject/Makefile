all: all.out 
all.out : sema.o 
	g++ $^ -o $@ -lpthread
sema.o: sema.c 
	g++ $< -c
clean:
	rm -rf *.o *.out
