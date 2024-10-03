
CC = gcc
CFLAGS = -Wall -pedantic -lm

#====================================================
# For the test of the lexer
#====================================================

#OBJ = test_lexer.o lexer.o

#exe : $(OBJ)
#	$(CC) $(CFLAGS) -o exe $(OBJ)
   
#test_lexer.o : lexer.h test_lexer.c
#	$(CC) $(CFLAGS) -c test_lexer.c 

#lexer.o : lexer.h lexer.c
#	$(CC) $(CFLAGS) -c lexer.c

#====================================================
# For the test of the parser
#====================================================

OBJ = test_parser.o parser.o lexer.o

exe : $(OBJ)
	$(CC) $(CFLAGS) -o exe $(OBJ)

test_parser.o : lexer.h parser.h test_parser.c
	$(CC) $(CFLAGS) -c test_parser.c

parser.o : lexer.h parser.h parser.c
	$(CC) $(CFLAGS) -c parser.c

lexer.o : lexer.h lexer.c
	$(CC) $(CFLAGS) -c lexer.c
