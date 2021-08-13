CC = 		gcc
OBJS = 		main.o system.o matrix.o utils.o input.o
LIKWID_PATH = /home/soft/likwid
LIBS = 		-lm
OUTPUT = 	interpolation
CFLAGS = 	-O3 -mavx -march=native -g
RM = 		rm -f

all: $(OBJS)
	$(CC) -L$(LIKWID_PATH)/lib $(OBJS) $(LIBS) -o $(OUTPUT) -llikwid

main.o: main.c 
	$(CC) -DLIKWID_PERFMON -I${LIKWID_PATH}/include -c $(CFLAGS) $(LIBS) main.c
	
%.o: %.c %.h
	$(CC) -DLIKWID_PERFMON -I${LIKWID_PATH}/include -c $(CFLAGS) $(LIBS) $<

clean limpar:
	@echo "Limpando..."
	@$(RM) $(OBJS)

purge faxina: clean
	@echo "Faxina..."
	@$(RM) $(OUTPUT)


run: all
	./matrixInv < input.in