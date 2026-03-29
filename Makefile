OBJECTS := little_math.o

ifeq ($(MODE),opt)
	# does not use optimized version of build-in tangent function
	DEGUG_FLAGS := -fno-builtin
	COMPILE_FLAGS := -std=c23 -Wno-builtin-declaration-mismatch
	OPTIMIZATION_FLAGS := -O3
	LINK_FLAGS := -fno-math-errno
else
	DEGUG_FLAGS := -g3
	COMPILE_FLAGS := -std=c23 -Wno-builtin-declaration-mismatch
	OPTIMIZATION_FLAGS := -O0
	LINK_FLAGS := -fno-math-errno
endif

little_math: ${OBJECTS}
	cc -o lilm ${OBJECTS}

little_math.o: little_math.c
	cc -c little_math.c ${COMPILE_FLAGS} ${LINK_FLAGS} ${OPTIMIZATION_FLAGS} ${DEGUG_FLAGS}

clean:
	rm ${OBJECTS}
