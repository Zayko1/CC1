all : sql_client

sql_client : main.o repl.o btree.o
	gcc -o sql_client main.o repl.o btree.o 

main.o : main.c 
	gcc -o main.o -c main.c

repl.o : repl.c
	gcc -o repl.o -c repl.c
	
btree.o : btree.c
	gcc -o btree.o -c btree.c

clean :
	rm -rf *.o
