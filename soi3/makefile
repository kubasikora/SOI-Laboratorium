CC = gcc

all: rx tx spamtx bursttx px spampx burstpx

rx: chatreceive.o semaphores.o 
	$(CC) chatreceive.o semaphores.o -o rx

chatreceive.o: chatreceive.c
	$(CC) -c chatreceive.c -o chatreceive.o


tx: chatsender.o semaphores.o 
	$(CC) chatsender.o semaphores.o -o tx

chatsender.o: chatsender.c 
	$(CC) -c chatsender.c -o chatsender.o


spamtx: spam_tx.o semaphores.o
	$(CC) spam_tx.o semaphores.o -o spamtx

spam_tx.o: spam_tx.c 
	$(CC) -c spam_tx.c -o spam_tx.o


bursttx: burst_tx.o semaphores.o
	$(CC) burst_tx.o semaphores.o -o bursttx

burst_tx.o: burst_tx.c 
	$(CC) -c burst_tx.c -o burst_tx.o


px: prisender.o semaphores.o
	$(CC) prisender.o semaphores.o -o px

prisender.o: prisender.c
	$(CC) -c prisender.c -o prisender.o



spampx: spam_pri.o semaphores.o
	$(CC) spam_pri.o semaphores.o -o spampx

spam_pri.o: spam_pri.c
	$(CC) -c spam_pri.c -o spam_pri.o



burstpx: burst_pri.o semaphores.o
	$(CC) burst_pri.o semaphores.o -o burstpx

burst_pri.o: burst_pri.c
	$(CC) -c burst_pri.c -o burst_pri.o



semaphores.o: semaphores.c
	$(CC) -c semaphores.c -o semaphores.o

clean:
	rm -rf *.o rx tx spamtx bursttx px spampx burstpx 