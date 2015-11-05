# Compiler
CC = g++
CFLAGS = -Wall -Werror -ansi -pedantic

# Targets
all: rshell

# Call make-repo to store new executable
rshell: src/rshell.cpp
	@mkdir -p bin
	$(CC) $(CFLAGS) src/rshell.cpp -o bin/$@
	
clean:
	rm bin -Rf