TARGET = oss user
all: main user
main: main.c sharedMem.h
	gcc -o oss main.c -g
user: user.c sharedMem.h
	gcc -o user user.c -lpthread -lrt -g
clean:
	/bin/rm -f *.o $(TARGET)