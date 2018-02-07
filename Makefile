CC = gcc -g
PROGS = proc_fan testsim
OBJS1 = proc_fan.o makeargv.o 
OBJS2 = testsim.o

all : $(PROGS)

proc_fan : $(OBJS1)
	$(CC) -o $@ $^

simp_fan : $(OBJS2)
	$(CC) -o $@ $^

clean :
	rm -f *.o $(PROGS)
