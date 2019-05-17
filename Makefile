# macro definition - compiler flags
CFLAGS = -std=c99 -c

# macro definition - object file(s)
OBJ = random_nums.o

# macro definition - executable
EXEC = main


# compiling and linking
${EXEC}: main.c ${OBJ}
	gcc -std=c99 main.c ${OBJ}

random_nums.o: random_nums.c random_nums.h
	gcc ${CFLAGS} random_nums.c


# clean/remove files
.PHONY: clean
clean:
	rm -f ${OBJ} ${EXEC}
