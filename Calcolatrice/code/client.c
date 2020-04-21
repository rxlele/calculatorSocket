#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/signal.h>
#include <pthread.h>
#include <string.h>

#define NAME "mySocket"
#define N 256
#define PORT 3000

#define clear() printf("\e[H\e[J\e[3J")

int soc;


void handler(int signo){
    pid_t pid=getpid();

    switch(signo){
        case SIGINT:{
            printf("\n(SIGINT) Disconnessione...\n");
            sleep(1);
            close(soc);
            exit(1);
            break;
        }
        case SIGPIPE:{
            printf("\n(SIGPIPE) Disconnessione...\n");
            sleep(1);
            close(soc);
            exit(1);
            break;
        }
        case SIGHUP:{
            printf("\n(SIGHUP) Disconnessione...\n");
            sleep(1);
            close(soc);
            exit(1);
            break;
        }
        case SIGQUIT:{
            printf("\n(SIGQUIT) Disconnessione...\n");
            sleep(1);
            close(soc);
            exit(1);  
        }
        case SIGTSTP:{
            printf("\n(SIGTSTP) Disconnessione...\n");
            sleep(1);
            close(soc);
            exit(1);
	}
    }
}

char scegliOperazione(){
	char choice='\0';
	do{	
		printf("Operazione scelta: ");	
		scanf("%s",&choice);
		if(choice!='a' && choice!='s' && choice!='m' && choice!='d'){
                	printf("Errore, puoi digitare a, s, m oppure d. Riprova!\n");
            	}
	}while(choice!='a' && choice!='s' && choice!='m' && choice!='d');

	return choice;
}

char sceltaFinale(){
	char choice='\0';
	do{	
		scanf("%s",&choice);
                if(choice!='c' && choice!='e'){
                	printf("Errore, puoi digitare c oppure e. Riprova!\n");
			printf("Clicca (c) per continuare oppure (e) per disconnetterti: ");
           	 }
	}while(choice!='c' && choice!='e');

	return choice;

}
int main(int argc, char *argv[]){


	signal(SIGINT,handler);
	signal(SIGPIPE,handler);
	signal(SIGHUP,handler);
	signal(SIGQUIT,handler);
	signal(SIGTSTP,handler);


	int c_fd,
	    ret;
    	struct sockaddr_in s_address;	
	char buffer[N];

	if((c_fd=socket(AF_INET,SOCK_STREAM,0))<0){
		perror("Errore nella creazione del socket");
		return -1;
	}
 	s_address.sin_family=AF_INET;
	s_address.sin_addr.s_addr=htonl(INADDR_ANY);
	s_address.sin_port=htons(PORT);
	    
	if(connect(c_fd,(struct sockaddr *)&s_address,sizeof(s_address))<0){
		perror("Errore nella connessione del client");
		exit(1);
	}
	
	soc=c_fd;

   	while(1){
		clear();
		float res=0, num1=0,num2=0;
		char choice='\0',fine='\0';

        	if(read(c_fd,buffer,N)<=0){
			printf("Errore nella lettura sulla socket!(Primo valore)\n");
			sleep(1);
			close(c_fd);
			exit(1);
		}
		printf("%s",buffer);
		scanf("%f", &num1);

		write(c_fd,&num1,sizeof(float));

		bzero(buffer,N);

    		if(read(c_fd,buffer,N)<=0){
			printf("Errore nella lettura sulla socket!(Secondo valore)\nDisconnessione...\n");
			sleep(1);
			close(c_fd);
			exit(1);
		}
		printf("%s",buffer);
		scanf("%f", &num2);

		write(c_fd,&num2,sizeof(float));

		bzero(buffer,N);
		
		if(read(c_fd,buffer,N)<=0){
			printf("Errore nella lettura sulla socket!(Scelta operazione)\nDisconnessione...\n");
			sleep(1);
			close(c_fd);
			exit(1);
		}
		printf("%s",buffer);
		
		choice=scegliOperazione();
		write(c_fd,&choice,1);

		bzero(buffer,N);

		if(read(c_fd,&res,sizeof(float))<=0){
			printf("Errore nella lettura sulla socket!(Risultato)\nDisconnessione...\n");
			sleep(1);
			close(c_fd);
			exit(1);
		}
		printf("\n\nIl risultato è: %.3f\n\n",res);
		
		
		write(c_fd,"1",1);
		bzero(buffer,N);
		if(read(c_fd,buffer,N)<=0){
			printf("Errore nella lettura sulla socket!(1)\nDisconnessione...\n");
			sleep(1);
			close(c_fd);
			exit(1);
		}
		printf("%s",buffer);

		fine=sceltaFinale();
		write(c_fd,&fine,1);

		bzero(buffer,N);
		switch(fine){
			case'e':{
				printf("Disconnessione...\n");
				sleep(1);
				close(c_fd);
				exit(1);
			}
			case 'c':{
				bzero(buffer,N);
				break;	
			}
		}
	}
	return 0;
}
