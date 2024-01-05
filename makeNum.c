#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]){
	if(argc != 2){
	    fprintf(stderr, "Kullanim: makeNum <sayi> seklinde olmalidir.\n");
	    exit(1);
	}
	int n = atoi(argv[1]);
	
	char fileNames[5][20] = {"dosya1.ASCII", "dosya2.ASCII", "dosya3.ASCII", "dosya4.ASCII", "dosya5.ASCII"}; 
	
	FILE *files[5];
	for(int i = 0; i < 5; i++){
		files[i] = fopen(fileNames[i], "w");
		for (int j = 0; j < ((rand() % n) + 2000); j++){
		
			int sayi = (rand() % 1000) + 1;
			fprintf(files[i], "%d ", sayi);
		
		}
		int sayi = (rand() % 1000) + 1;
		fprintf(files[i], "%d", sayi);
		fclose(files[i]);
	}
	
	FILE *f = fopen("output.ASCII", "w");
	fclose(f);
	
	return 0;
}
