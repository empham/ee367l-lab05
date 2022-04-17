# Make file

sockTest: serverMain.o clientMain.o socket.o
	gcc -o server serverMain.o socket.o
	gcc -o client clientMain.o socket.o

net367: host.o packet.o man.o main.o net.o switch.o jobs.o socket.o
	gcc -o net367 host.o man.o main.o net.o packet.o switch.o jobs.o socket.o

main.o: main.c
	gcc -c main.c

serverMain.o: serverMain.c
	gcc -c serverMain.c

clientMain.o: clientMain.c
	gcc -c clientMain.c

host.o: host.c 
	gcc -c host.c

man.o:  man.c
	gcc -c man.c

net.o:  net.c
	gcc -c net.c

packet.o:  packet.c
	gcc -c packet.c

switch.o: switch.c switch.h 
	gcc -c switch.c  

jobs.o: jobs.c jobs.h 
	gcc -c jobs.c  

socket.o: socket.c socket.h 
	gcc -c socket.c

clean:
	rm *.o

