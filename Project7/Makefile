#
#	This is a sample make file for RTP

CFLAGS = -Wall -std=c11 -pedantic -O2

NETWORK_OBJECT = network.o
LIBS = -lpthread
AFLAGS = -D LINUX
SUBMIT = *.c Makefile *.h *.o prj5-server

client: client.c rtp.o $(NETWORK_OBJECT)
	gcc $(CFLAGS) -o prj5-client client.c rtp.o queue.c $(NETWORK_OBJECT) $(LIBS)
rtp.o : rtp.c rtp.h
	gcc -c $(CFLAGS) $(AFLAGS) rtp.c

submit:	clean
	tar cvfz prj5-submit.tar.gz $(SUBMIT)

clean:
	rm -rf rtp.o queue.o prj5-client
