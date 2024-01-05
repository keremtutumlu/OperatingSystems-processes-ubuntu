#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/wait.h>

#define max_k 1000
#define max_n 5

struct msgbuf {
	long mtype;
	int mtext;
};

int main(int argc, char *argv[]) {
	int k, n, i, j;
	char *infiles[max_n];
	char *outfile;

	n = atoi(argv[2]);

  	if (argc != (4 + n) || argc > 9) {
  		if(argc != (4 + n)){
  			fprintf(stderr, "Kullanım: findtopk <k> <N> <infile1> ...<infileN> <outfile> seklinde olmalidir.\n");
  		}
		else{
			fprintf(stderr, "%d 'den fazla girdi girilemez.", max_n);
		}
    	exit(1);
  	}

  	k = atoi(argv[1]);
  	for (i = 0; i < n; i++) {
    	infiles[i] = argv[3 + i];
	}
	outfile = argv[n + 3];
	
	//POSIX message queue oluşturuluyor.
	key_t key = ftok(".", 'a');
	int msgid = msgget(key, IPC_CREAT | 0666);
	if(msgid == -1){
		perror("msgget");
		return 1;
	}

 	pid_t child_pids[max_n];
  	for (i = 0; i < n; i++) {
    	if((child_pids[i]= fork()) < 0){
    		perror("Fork error!");
    		abort();
    	}
    	else if (child_pids[i] == 0) {
    		printf("%d", argc);
    		printf("Child Process -> %d\n", i+1);
    		// Bu child processtir.
			
    		int size = 2;
			int *numbers = malloc(sizeof(int) * size);
    		int number;
    		FILE *infile = fopen(infiles[i], "r");
    		printf("Process %d infile atama\n", i+1);
    		//Dosyadan sayıları diziye aktarıyoruz.
      		j = 0;
      		while(!feof(infile)){
      			fscanf(infile, "%d", &number);
      			numbers[j++] = number;
      			size++;
  				numbers = realloc(numbers, sizeof(int) * size);
      		}
      		numbers[j] = -1;
      		
      		//Diziyi büyükten küçüğe bubble sort ile sıralıyoruz.
      		int counter, temp;
      		do{
      			counter = 0;
      			j = 0;
      			while(numbers[j+1]!= -1){
      			
      				if(numbers[j+1] > numbers[j]){
      					counter++;
      					temp = numbers [j];
      					numbers[j] = numbers[j + 1];
      					numbers[j + 1] = temp;
      				}
      				j++;
      			}
      			
      		}while(counter != 0);
      		
        	// Sıralanmış dizideki k. sayı POSIX queueye gönderiliyor.
        	//İndis değerinin k-1 olma sebebi indislerin 0'dan başlamasıdır.
			struct msgbuf msg;
			msg.mtype = i + 1;
			msg.mtext = numbers[k-1];
			if(msgsnd(msgid, &msg, sizeof(msg.mtext), 0) == -1){
				perror("msgsnd");
				return 1;
			}

      		fclose(infile);

      		// Child process burada bitiyor..
      		
      		printf("%d. Child process sonu...\n", i+1);
      		exit(0);
    	}
    	else{
    		wait(NULL);
    		printf("%d. child tamamlandı.\n",i+1);
    		if(i == n-1){
    			
    			//POSIX mesajlarını alarak bir diziye atıyoruz.
    			int nums[max_n + 1];
				for(j = 0; j < n; j++){
					struct msgbuf msg;
					if(msgrcv(msgid, &msg, sizeof(msg.mtext), j+1, 0) == -1){
						perror("msgrcv");
						return 1;
					}
					nums[j] = msg.mtext;
				}
				nums[j] = -1;		
	
				//POSIX message queueyi kapatıyoruz.
				msgctl(msgid, IPC_RMID, NULL);
	
				//Dizideki sayıları büyükten küçüğe bubble sort ile sıralıyoruz.
				int counter, temp;
    			do{
      				counter = 0;
      				int j = 0;
      				while(nums[j+1]!=-1){
      			
      					if(nums[j+1] > nums[j]){
      						counter++;
      						temp = nums [j];
      						nums[j] = nums[j + 1];
      						nums[j + 1] = temp;
      					}
      				j++;	
      				}
      			}while(counter != 0);
	
	
				//Değerleri output dosyamıza yazıyoruz.
				FILE *outpfile = fopen(outfile, "w");
				for(i = 0; i < n; i++){
				fprintf(outpfile, "%d\n", nums[i]);
				}
				fclose(outpfile);
    			
    		}
    	}
  	}	

  return 0;
}
