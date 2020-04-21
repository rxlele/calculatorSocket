#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/signal.h>
#include <pthread.h>
#include <time.h>
#include <string.h>

#define NAME "mySocket"
#define N 256
#define PORT 3000


#define clear() printf("\e[H\e[J\e[3J")

pthread_mutex_t mutex;

void getMonth(int numMese, char* mese){
    switch(numMese){
        case 0:{
            strcpy(mese,"Gennaio");
            break;
        }
        case 1:{
            strcpy(mese,"Febbraio");
            break;
        }
        case 2:{
            strcpy(mese,"Marzo");
            break;
        }
        case 3:{
            strcpy(mese,"Aprile");
            break;
        }
        case 4:{
            strcpy(mese,"Maggio");
            break;
        }
        case 5:{
            strcpy(mese,"Giugno");
            break;
        }
        case 6:{
            strcpy(mese,"Luglio");
            break;
        }
        case 7:{
            strcpy(mese,"Agosto");
            break;
        }
        case 8:{
            strcpy(mese,"Settembre");
            break;
        }
        case 9:{
            strcpy(mese,"Ottobre");
            break;
        }
        case 10:{
            strcpy(mese,"Novembre");
            break;
        }
        case 11:{
            strcpy(mese,"Dicembre");

        }
    }
}

void openServerLog(){
 
    FILE *pf;
    int ora, minuto, secondi,
        giorno, anno;
    char mese[9];
    time_t nowtime;
    struct tm* dati;

    time (&nowtime);
    dati=localtime(&nowtime);
    ora=dati->tm_hour;
    minuto=dati->tm_min;
    secondi=dati->tm_sec;

    giorno=dati->tm_mday;
    getMonth(dati->tm_mon,mese);
    anno=dati->tm_year + 1900;
    pthread_mutex_lock(&mutex);
    pf=fopen("log.txt","a+");
    fprintf(pf,"\n\nSERVER APERTO -------------------------- %d : %d : %d ----- %d - %s - %d\n",ora,minuto,secondi,giorno,mese,anno);
    fclose(pf);
    pthread_mutex_unlock(&mutex);	
}

void closeServerLog(){
	
    FILE *pf;
    int ora, minuto, secondi,
        giorno, anno;
    char mese[9];
    time_t nowtime;
    struct tm* dati;

    time (&nowtime);
    dati=localtime(&nowtime);
    ora=dati->tm_hour;
    minuto=dati->tm_min;
    secondi=dati->tm_sec;

    giorno=dati->tm_mday;
    getMonth(dati->tm_mon,mese);
    anno=dati->tm_year + 1900;
    pthread_mutex_lock(&mutex);
    pf=fopen("log.txt","a+");
    fprintf(pf,"SERVER CHIUSO -------------------------- %d : %d : %d ----- %d - %s - %d\n\n\n",ora,minuto,secondi,giorno,mese,anno);
    fclose(pf);
    pthread_mutex_unlock(&mutex);

}

void connectionLog(int c_fd){


    FILE *pf;
    int ora, minuto, secondi,
        giorno, anno;
    char mese[9];
    time_t nowtime;
    struct tm* dati;

    time (&nowtime);
    dati=localtime(&nowtime);
    ora=dati->tm_hour;
    minuto=dati->tm_min;
    secondi=dati->tm_sec;

    giorno=dati->tm_mday;
    getMonth(dati->tm_mon,mese);
    anno=dati->tm_year + 1900;
    pthread_mutex_lock(&mutex);
    pf=fopen("log.txt","a+");
    fprintf(pf,"Utente %d connesso\t\t%d : %d : %d\t\t%d - %s - %d\n",c_fd-4,ora,minuto,secondi,giorno,mese,anno);
    fclose(pf);
    pthread_mutex_unlock(&mutex);
	
}

void disconnessioneLog(int c_fd){

    FILE *pf;
    int ora, minuto, secondi,
        giorno, anno;
    char mese[9];
    time_t nowtime;
    struct tm* dati;

    time (&nowtime);
    dati=localtime(&nowtime);
    ora=dati->tm_hour;
    minuto=dati->tm_min;
    secondi=dati->tm_sec;

    giorno=dati->tm_mday;
    getMonth(dati->tm_mon,mese);
    anno=dati->tm_year + 1900;
    pthread_mutex_lock(&mutex);
    pf=fopen("log.txt","a+");
    fprintf(pf,"Utente %d disconnesso\t\t%d : %d : %d\t\t%d - %s - %d\n",c_fd-4,ora,minuto,secondi,giorno,mese,anno);
    fclose(pf);
    pthread_mutex_unlock(&mutex);
}

void getOperation(char choice, char* oper){
	switch(choice){
		case 'a':{
		    strcpy(oper,"Addizione");
		    break;
		}
		case 's':{
		    strcpy(oper,"Sottrazione");
		    break;
		}
		case 'm':{
		    strcpy(oper,"Moltiplicazione");
		    break;
		}
		case 'd':{
		    strcpy(oper,"Divisione");
		    break;
		}
	}
}

void operazioneLog(int c_fd,float num1,float num2,char choice,float res){
    FILE *pf;
    int ora, minuto, secondi,
        giorno, anno;
    char mese[9],oper[12];

    time_t nowtime;
    struct tm* dati;

    time (&nowtime);
    dati=localtime(&nowtime);
    ora=dati->tm_hour;
    minuto=dati->tm_min;
    secondi=dati->tm_sec;

    giorno=dati->tm_mday;
    getMonth(dati->tm_mon,mese);
    anno=dati->tm_year + 1900;

    getOperation(choice,oper);

	pthread_mutex_lock(&mutex);
    pf=fopen("log.txt","a+");
    fprintf(pf,"Utente %d ha scelto come elementi %.2f e %.2f e come operazione la %s, risultato %.2f\t\t%d : %d : %d\t\t%d - %s - %d\n",c_fd-4,num1,num2,oper,res,ora,minuto,secondi,giorno,mese,anno);

	fclose(pf);
	pthread_mutex_unlock(&mutex);
    
}

void handler(int signo){
    pid_t pid=getpid();

    switch(signo){
        case SIGINT:{
            closeServerLog();
	    printf("Disconnessione...\n");
	    sleep(1);
            kill(pid,SIGTERM);
            break;
        }
        case SIGPIPE:{
            pthread_exit(NULL);
            break;
        }
        case SIGHUP:{
            closeServerLog();
	    printf("Disconnessione...\n");
	    sleep(1);
            kill(pid,SIGTERM);
            break;
        }
        case SIGQUIT:{
            closeServerLog();
	    printf("Disconnessione...\n");
	    sleep(1);
            kill(pid,SIGTERM);
        }
	case SIGTSTP:{
	    closeServerLog();
	    printf("Disconnessione...\n");
	    sleep(1);
            kill(pid,SIGTERM);
            break;
	}

    }
}

void* gestisci(void* arg){
	int c_fd=*(int *)arg;
	

	while(1){
		char fine='\0';
		float res=0, num1=0,num2=0;
		char choice='\0';
			//lettura del messaggio scritto dal client
		if(write(c_fd, "Inserisci il primo valore: ",strlen("Inserisci il primo valore: "))<=0){
			printf("Errore nella scrittura sulla socket!(Primo valore)\n");
              		disconnessioneLog(c_fd);
               		close(c_fd);
               		pthread_exit(0);
		}
		if(read(c_fd, &num1, sizeof(float))<=0){
			printf("Errore nella lettura sulla socket!(Primo valore)..Utente %d disconnesso!\n", c_fd-4);
			disconnessioneLog(c_fd);
			close(c_fd);
			pthread_exit(0);
		}


		if(write(c_fd, "Inserisci il secondo valore: ",strlen("Inserisci il secondo valore: "))<=0){
			printf("Errore nella scrittura sulla socket!(Secondo Valore)\n");
			disconnessioneLog(c_fd);	
			close(c_fd);
			pthread_exit(0);
		}
			
		if(read(c_fd, &num2, sizeof(float))<=0){
			printf("Errore nella lettura sulla socket!(Secondo Valore)..Utente %d disconnesso!\n", c_fd-4);
			disconnessioneLog(c_fd);
			close(c_fd);
			pthread_exit(0);
		}


		if(write(c_fd,"Che operazione vuoi effettuare?\n\ta.\tAddizione\n\ts.\tSottrazione\n\tm.\tMoltiplicazione\n\td.\tDivisione\n",strlen("Che operazione vuoi effettuare?\n\ta.\tAddizione\n\ts.\tSottrazione\n\tm.\tMoltiplicazione\n\td\t.Divisione\n"))<=0){
			printf("Errore nella scrittura del socket!(Scelta operazione)\n");
			disconnessioneLog(c_fd);
			close(c_fd);
			pthread_exit(0);
		}

		if(read(c_fd,&choice,N)<=0){
			printf("Errore nella lettura sulla socket!(Scelta operazione)..Utente %d disconnesso!\n", c_fd-4);
			disconnessioneLog(c_fd);
			close(c_fd);
			pthread_exit(0);
		}

			
		switch(choice){
			case 'a':{
				res=num1+num2;
				break;
			}
			case 's':{
				res=num1-num2;
				break;
			}
			case 'm':{
				res=num1*num2;
				break;
			}
			case 'd':{
				res=num1/num2;
				break;
			}
		}
		if(write(c_fd,&res,sizeof(float))<=0){
			printf("Errore nella scrittura del socket!(Risultato)\n");
			disconnessioneLog(c_fd);
			close(c_fd);
			pthread_exit(0);
		}
		operazioneLog(c_fd,num1,num2,choice,res);

		char buffer2[N];
		if(read(c_fd,buffer2,N)<=0){
			printf("Errore nella lettura sulla socket!(1)..Utente %d disconnesso!\n", c_fd-4);
			disconnessioneLog(c_fd);
			close(c_fd);
			pthread_exit(0);
		}


		if(write(c_fd,"Clicca (c) per continuare oppure (e) per disconnetterti: ",strlen("Clicca (c) per continuare oppure (e) per disconnetterti: "))<=0){
			printf("Errore nella scrittura del socket!(Fine)\n");
			disconnessioneLog(c_fd);
			close(c_fd);
			pthread_exit(0);
		}

		if(read(c_fd, &fine, 1)<=0){
			printf("Errore nella lettura sulla socket!(Fine)..Utente %d disconnesso!\n", c_fd-4);
			disconnessioneLog(c_fd);
			close(c_fd);
			pthread_exit(0);
		}

		switch(fine){
			case'e':{
				printf("Utente %d disconnesso\n",c_fd-4);
				disconnessioneLog(c_fd);
				close(c_fd);
				pthread_exit(0);
				
				break;
			}
			case 'c':
				break;	
		}
	
	}
}


int main(int argc, char* argv[]){

	signal(SIGINT,handler);
	signal(SIGPIPE,handler);
	signal(SIGHUP,handler);
	signal(SIGQUIT,handler);
	signal(SIGTSTP,handler);
  
	int s_fd, c_fd, c_len, s_len;
	pthread_t tid;

        struct sockaddr_in s_address, c_address;
	//ASSEGNAZIONE DI UN INDIRIZZO ALLA SOCKET

        s_address.sin_family= AF_INET;
        s_address.sin_addr.s_addr = htonl(INADDR_ANY);
        s_address.sin_port = htons(PORT);

	s_fd = socket(AF_INET, SOCK_STREAM, 0);

	if((s_fd = socket(AF_INET, SOCK_STREAM, 0))<0){
		perror("Errore nella creazione della socket!");
		closeServerLog();
		return -1;
   	}

	
	 if(bind(s_fd,(struct sockaddr*)&s_address, sizeof(s_address))<0){
		perror("bind fallito!");
		closeServerLog();
		return -1;
   	}

	if(pthread_mutex_init(&mutex,NULL)!=0){
			printf("Errore nella creazione del semaforo!\n");
			closeServerLog();
			return -1;
		}
	clear();
	printf("Server in esecuziuone\n");

	openServerLog();

	//SETTAGGIO SOCKET IN ASCOLTO
	listen(s_fd,10);
	c_len=sizeof(struct sockaddr_in);
	while(1){
		c_fd = accept(s_fd, (struct sockaddr*) &c_address, &c_len);
		connectionLog(c_fd);
		printf("Utente %d connesso\n",c_fd-4);

		
		pthread_create(&tid, NULL, gestisci, (void*)&c_fd);
		pthread_detach(tid);
		

	}
	return 0;
}
	


