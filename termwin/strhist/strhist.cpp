// strhist.cpp : Defines the entry point for the console application.
//

#include <stdio.h>
#include "stdint.h"
#include "string.h"

class cstrHist{
public:
	enum{hist_size = 215};
	char hist[hist_size];
	uint16_t wi;
	uint16_t csize;

	cstrHist() : wi(0), csize(0){
	memset(hist,0,hist_size);
	}

	void add(char *t){
		uint16_t size = strlen(t);
		csize+=size+1;
		if(csize>hist_size)csize=hist_size;
		if(size>=hist_size) {
			printf("error add, no space\n");
			return;
		}
		if((wi + size + 1)<hist_size){
			// add one block
			memcpy(&hist[wi],t,size+1);
			wi+=size+1;
			//hist[wi] = 0; // clear current
		} else {
			// split in two
			uint16_t size1 = hist_size - wi;
			uint16_t size2 = size+1- size1;
			memcpy(&hist[wi],t,size1);
			memcpy(&hist[0],t+size1,size2);
			wi = size2;
			//hist[wi] = 0; // clear current
		}
	}

	uint16_t getPrev(uint16_t i){
		i=i==0?hist_size-1: i-1;
		while(1){
			i=i==0?hist_size-1: i-1;
			if(hist[i]==0) return (i+1)>=hist_size?0:i+1;
		}
	}

	void print_at(uint16_t i){
		while(hist[i]){			
			printf("%c",hist[i]);
			i++;
			if(i>=hist_size)i=0;
		}
		printf("\n");
	}
	void list_all(){
		uint16_t i = wi;		
		while(1){
			
			i = getPrev(i);					
			if(i==wi) {
				printf("all were printed\n");
				break;
			}
			if(i>csize) {
				printf("no more elements\n");
				break;
			}

			print_at(i);
			
		}
	}
	
};

int main(int argc, char* argv[])
{
	cstrHist hist;
	hist.add("[1] alma a fa altt");
	hist.add("[2] korte fa kiszaradt");
	hist.add("[3] szilvas gomboc");
	hist.add("[4] eper lekvar");
	hist.add("[5] malna a kertben");

	hist.list_all();
	printf("+++\n");
	
	return 0;
}

