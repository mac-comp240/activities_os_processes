CC = gcc
ICS = ..
CFLAGS =  -Og  


all: fork_example exec_example exec_helper exec2 sig_example1 sig_example2

fork_example: fork_example.c
	$(CC) $(CFLAGS) $^ -o $@

exec_example: exec_example.c
	$(CC) $(CFLAGS) $^ -o $@

exec_helper: exec_helper.c
	$(CC) $(CFLAGS) $^ -o $@

exec2: exec2.c
	$(CC) $(CFLAGS) $^ -o $@

sig_example1: sig_example1.c
	$(CC) $(CFLAGS) $^ -o $@

sig_example2: sig_example2.c
	$(CC) $(CFLAGS) $^ -o $@

clean:
	rm -f $(PROGS) *.o *~ 
	rm -f -r *.dSYM
	rm -f cache*


