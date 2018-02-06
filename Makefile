CC = gcc -g
PROGS = proc_fan simp_fan
OBJ1 = proc_fan.o 
OBJ2 = simp_fan.o

all : $(PROGS)

proc_fan : $(OBJ1)
	$(CC) -o $@ $^

simp_fan : $(OBJ2)
	$(CC) -o $@ $^

clean :
	rm -f *.o $(PROGS)
