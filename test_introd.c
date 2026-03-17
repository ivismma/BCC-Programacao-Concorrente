#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

void* printMessage(void *ptr){
	char* msg = (char *) ptr;
	printf("%s\n", msg);
	return NULL;
}

int main(void){
	pthread_t t1, t2, t3;

	char *msg1 = "Exec thread1...";
	char *msg2 = "Exec thread2...";
	char *msg3 = "Exec thread3...";

	pthread_create(&t1, NULL, printMessage, (void *) msg1);
	pthread_create(&t2, NULL, printMessage, (void *) msg2);
	pthread_create(&t3, NULL, printMessage, (void *) msg3);

	pthread_join(t1, NULL);
	pthread_join(t2, NULL);
 	pthread_join(t3, NULL);	

	return 0;
}
