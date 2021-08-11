CC = 		gcc
OBJS = 		main.o system.o matrix.o utils.o interpolation.o
LIBS = 		-lm
OUTPUT = 	matrixInv
CFLAGS = 	-g 
RM = 		rm -f

all: $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LIBS) -o $(OUTPUT)

main.o: main.c 
	$(CC) -c $(CFLAGS) $(LIBS) main.c
	
%.o: %.c %.h
	$(CC) -c $(CFLAGS) $(LIBS) $<

clean limpar:
	@echo "Limpando..."
	@$(RM) $(OBJS)

purge faxina: clean
	@echo "Faxina..."
	@$(RM) $(OUTPUT)


run: purge all clean
	./matrixInv < input.in