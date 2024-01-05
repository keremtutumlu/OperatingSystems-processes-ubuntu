#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define max_k 1000
#define max_n 5

//GLOBAL DEĞİŞKENLER//
char *infiles[max_n];
char *outfile;
int nums[max_n + 1];
int k, n;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;


void *thread_func(void *arg){
	
	int m = *((int*)arg);
	printf("%d. pthreaddeyiz.",m + 1);
	
	int size = 2;
	int *numbers = malloc(sizeof(int) * size);
   	int number;
   	
   	//Dosyadan sayıları diziye aktarıyoruz.//
   	FILE *infile = fopen(infiles[m], "r");
   	int j = 0;
 	while(!feof(infile)){
  		fscanf(infile, "%d", &number);
  		numbers[j++] = number;
  		size++;
  		numbers = realloc(numbers, sizeof(int) * size);
  	}
  	numbers[j] = -1;
  	fclose(infile);
  	
  	//Diziyi büyükten küçüğe bubble sort ile sıralıyoruz.//
  	int counter, temp;
  	do{
   		counter = 0;
 		j = 0;
  		while(numbers[j+1]!= -1){
  		
  			if(numbers[j+1] > numbers[j]){
  				counter++;
 				temp = numbers[j];
  				numbers[j] = numbers[j + 1];
  				numbers[j + 1] = temp;
  			}
  			j++;
  		}
  		
  	}while(counter != 0);
  	
  	pthread_mutex_lock(&mutex);
	// Sıralanmış dizideki k. sayı nums arrayine atanıyor.
    //İndis değerinin k-1 olma sebebi indislerin 0'dan başlamasıdır.	
	nums[m] = numbers[k-1];

	pthread_mutex_unlock(&mutex);
	pthread_exit(0);
}



int main(int argc, char *argv[]) {

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
  	for (int i = 0; i < n; i++) {
    	infiles[i] = argv[3 + i];
	}
	outfile = argv[n + 3];


	//pthreadler oluşturuluyor.//
	pthread_t threads[n];
	int thread_nums[n];
	
	for(int i = 0; i < n; i++){
		
		thread_nums[i] = i;
		printf("%s dosya adi.\n", infiles[i]);
		printf("%d. dongudeyiz.\n", i+1);
		pthread_attr_t attr;
		pthread_attr_init(&attr);
		printf("%d. dongudeyiz attr sonrasi.\n", i+1);
		pthread_create(&threads[i], &attr, thread_func, &thread_nums[i]);
		printf("%d. dongudeyiz create sonrasi.\n", i+1);
		pthread_join(threads[i], NULL);
		printf("%d. dongudeyiz join sonrasi.\n", i+1);
	}

	nums[n] = -1;
	
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
	for(int i = 0; i < n; i++){
		fprintf(outpfile, "%d\n", nums[i]);
	}
	fclose(outpfile);

	return 0;
}


