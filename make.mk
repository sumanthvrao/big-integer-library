a.out: intal.o svr_client.o
		gcc intal.o svr_client.o

intal.o: intal.h intal.c
		gcc -c intal.c

svr_client.o: intal.h svr_client.c
		gcc -c svr_client.c