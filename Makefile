DEGUG_FLAGS := -ggdb
COMPILE_FLAGS := -std=c23 -Wno-builtin-declaration-mismatch
LINK_FLAGS := -fno-math-errno

OBJECTS := little_math.o

little_math: ${OBJECTS}
	cc -o lilm ${OBJECTS}

little_math.o: little_math.c
	gcc -c little_math.c ${COMPILE_FLAGS} ${LINK_FLAGS} ${DEGUG_FLAGS}

clean:
	rm ${OBJECTS}
